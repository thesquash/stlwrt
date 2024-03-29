/* gtkiconview.c
 * Copyright (C) 2002, 2004  Anders Carlsson <andersca@gnu.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <stlwrt.h>
#include <string.h>

#include <atk/atk.h>

#include <gdkkeysyms.h>

#include <gtkiconview.h>
#include <gtkcelllayout.h>
#include <gtkcellrenderer.h>
#include <gtkcellrenderertext.h>
#include <gtkcellrendererpixbuf.h>

#include <gtkbindings.h>
#include <gtkdnd.h>
#include <gtkmain.h>
#include <gtkintl.h>
#include <gtkaccessible.h>
#include <gtkwindow.h>
#include <gtkentry.h>
#include <gtkcombobox.h>
#include <gtktextbuffer.h>
#include <gtktreednd.h>
#include <gtkprivate.h>


#undef DEBUG_ICON_VIEW

#define SCROLL_EDGE_SIZE 15



typedef struct _GtkIconViewItem GtkIconViewItem;
struct _GtkIconViewItem
{
  GtkTreeIter iter;
  gint index;
  
  gint row, col;

  /* Bounding box */
  gint x, y, width, height;

  /* Individual cells.
   * box[i] is the actual area occupied by cell i,
   * before, after are used to calculate the cell 
   * area relative to the box. 
   * See gtk_icon_view_get_cell_area().
   */
  gint n_cells;
  GdkRectangle *box;
  gint *before;
  gint *after;

  guint selected : 1;
  guint selected_before_rubberbanding : 1;

};

typedef struct _GtkIconViewCellInfo GtkIconViewCellInfo;
struct _GtkIconViewCellInfo
{
  GtkCellRenderer *cell;

  guint expand : 1;
  guint pack : 1;
  guint editing : 1;

  gint position;

  GSList *attributes;

  GtkCellLayoutDataFunc func;
  gpointer func_data;
  GDestroyNotify destroy;
};

typedef struct _GtkIconViewChild GtkIconViewChild;
struct _GtkIconViewChild
{
  GtkWidget *widget;
  GtkIconViewItem *item;
  gint cell;
};


/* Signals */
enum
{
  ITEM_ACTIVATED,
  SELECTION_CHANGED,
  SELECT_ALL,
  UNSELECT_ALL,
  SELECT_CURSOR_ITEM,
  TOGGLE_CURSOR_ITEM,
  MOVE_CURSOR,
  ACTIVATE_CURSOR_ITEM,
  LAST_SIGNAL
};

/* Properties */
enum
{
  PROP_0,
  PROP_PIXBUF_COLUMN,
  PROP_TEXT_COLUMN,
  PROP_MARKUP_COLUMN,  
  PROP_SELECTION_MODE,
  PROP_ORIENTATION,
  PROP_ITEM_ORIENTATION,
  PROP_MODEL,
  PROP_COLUMNS,
  PROP_ITEM_WIDTH,
  PROP_SPACING,
  PROP_ROW_SPACING,
  PROP_COLUMN_SPACING,
  PROP_MARGIN,
  PROP_REORDERABLE,
  PROP_TOOLTIP_COLUMN,
  PROP_ITEM_PADDING
};

struct _GtkIconViewPrivate
{
  gint width, height;

  GtkSelectionMode selection_mode;

  GdkWindow *bin_window;

  GList *children;

  GtkTreeModel *model;
  
  GList *items;
  
  GtkAdjustment *hadjustment;
  GtkAdjustment *vadjustment;

  guint layout_idle_id;
  
  gboolean doing_rubberband;
  gint rubberband_x1, rubberband_y1;
  gint rubberband_x2, rubberband_y2;

  guint scroll_timeout_id;
  gint scroll_value_diff;
  gint event_last_x, event_last_y;

  GtkIconViewItem *anchor_item;
  GtkIconViewItem *cursor_item;
  GtkIconViewItem *edited_item;
  GtkCellEditable *editable;

  GtkIconViewItem *last_single_clicked;

  GList *cell_list;
  guint n_cells;

  gint cursor_cell;

  GtkOrientation item_orientation;

  gint columns;
  gint item_width;
  gint spacing;
  gint row_spacing;
  gint column_spacing;
  gint margin;
  gint item_padding;

  gint text_column;
  gint markup_column;
  gint pixbuf_column;

  gint pixbuf_cell;
  gint text_cell;

  gint tooltip_column;

  /* Drag-and-drop. */
  GdkModifierType start_button_mask;
  gint pressed_button;
  gint press_start_x;
  gint press_start_y;

  GdkDragAction source_actions;
  GdkDragAction dest_actions;

  GtkTreeRowReference *dest_item;
  GtkIconViewDropPosition dest_pos;

  /* scroll to */
  GtkTreeRowReference *scroll_to_path;
  gfloat scroll_to_row_align;
  gfloat scroll_to_col_align;
  guint scroll_to_use_align : 1;

  guint source_set : 1;
  guint dest_set : 1;
  guint reorderable : 1;
  guint empty_view_drop :1;

  guint modify_selection_pressed : 1;
  guint extend_selection_pressed : 1;

  guint draw_focus : 1;
};

/* GObject vfuncs */
static void             gtk_icon_view_cell_layout_init          (GtkCellLayoutIface *iface);
static void             gtk_icon_view_finalize                  (GObject            *object);
static void             gtk_icon_view_set_property              (GObject            *object,
								 guint               prop_id,
								 const GValue       *value,
								 GParamSpec         *pspec);
static void             gtk_icon_view_get_property              (GObject            *object,
								 guint               prop_id,
								 GValue             *value,
								 GParamSpec         *pspec);

/* GtkWidget vfuncs */
static void             gtk_icon_view_realize                   (GtkWidget          *widget);
static void             gtk_icon_view_unrealize                 (GtkWidget          *widget);
static void             gtk_icon_view_style_set                 (GtkWidget        *widget,
						                 GtkStyle         *previous_style);
static void             gtk_icon_view_state_changed             (GtkWidget        *widget,
			                                         GtkStateType      previous_state);
static void             gtk_icon_view_size_request              (GtkWidget          *widget,
								 GtkRequisition     *requisition);
static void             gtk_icon_view_size_allocate             (GtkWidget          *widget,
								 GtkAllocation      *allocation);
static gboolean         gtk_icon_view_expose                    (GtkWidget          *widget,
								 GdkEventExpose     *expose);
static gboolean         gtk_icon_view_motion                    (GtkWidget          *widget,
								 GdkEventMotion     *event);
static gboolean         gtk_icon_view_button_press              (GtkWidget          *widget,
								 GdkEventButton     *event);
static gboolean         gtk_icon_view_button_release            (GtkWidget          *widget,
								 GdkEventButton     *event);
static gboolean         gtk_icon_view_key_press                 (GtkWidget          *widget,
								 GdkEventKey        *event);
static gboolean         gtk_icon_view_key_release               (GtkWidget          *widget,
								 GdkEventKey        *event);
static AtkObject       *gtk_icon_view_get_accessible            (GtkWidget          *widget);


/* GtkContainer vfuncs */
static void             gtk_icon_view_remove                    (GtkContainer       *container,
								 GtkWidget          *widget);
static void             gtk_icon_view_forall                    (GtkContainer       *container,
								 gboolean            include_internals,
								 GtkCallback         callback,
								 gpointer            callback_data);

/* GtkIconView vfuncs */
static void             gtk_icon_view_set_adjustments           (GtkIconView        *icon_view,
								 GtkAdjustment      *hadj,
								 GtkAdjustment      *vadj);
static void             gtk_icon_view_real_select_all           (GtkIconView        *icon_view);
static void             gtk_icon_view_real_unselect_all         (GtkIconView        *icon_view);
static void             gtk_icon_view_real_select_cursor_item   (GtkIconView        *icon_view);
static void             gtk_icon_view_real_toggle_cursor_item   (GtkIconView        *icon_view);
static gboolean         gtk_icon_view_real_activate_cursor_item (GtkIconView        *icon_view);

 /* Internal functions */
static void                 gtk_icon_view_adjustment_changed             (GtkAdjustment          *adjustment,
									  GtkIconView            *icon_view);
static void                 gtk_icon_view_layout                         (GtkIconView            *icon_view);
static void                 gtk_icon_view_paint_item                     (GtkIconView            *icon_view,
									  cairo_t *cr,

									  GtkIconViewItem        *item,
									  GdkRectangle           *area,
									  GdkDrawable *drawable,
									  gint         x,
									  gint         y,
									  gboolean     draw_focus);
static void                 gtk_icon_view_paint_rubberband               (GtkIconView            *icon_view,
								          cairo_t *cr,
									  GdkRectangle           *area);
static void                 gtk_icon_view_queue_draw_path                (GtkIconView *icon_view,
									  GtkTreePath *path);
static void                 gtk_icon_view_queue_draw_item                (GtkIconView            *icon_view,
									  GtkIconViewItem        *item);
static void                 gtk_icon_view_queue_layout                   (GtkIconView            *icon_view);
static void                 __gtk_icon_view_set_cursor_item                (GtkIconView            *icon_view,
									  GtkIconViewItem        *item,
									  gint                    cursor_cell);
static void                 gtk_icon_view_start_rubberbanding            (GtkIconView            *icon_view,
									  gint                    x,
									  gint                    y);
static void                 gtk_icon_view_stop_rubberbanding             (GtkIconView            *icon_view);
static void                 gtk_icon_view_update_rubberband_selection    (GtkIconView            *icon_view);
static gboolean             gtk_icon_view_item_hit_test                  (GtkIconView            *icon_view,
									  GtkIconViewItem        *item,
									  gint                    x,
									  gint                    y,
									  gint                    width,
									  gint                    height);
static gboolean             __gtk_icon_view_unselect_all_internal          (GtkIconView            *icon_view);
static void                 gtk_icon_view_calculate_item_size            (GtkIconView            *icon_view,
									  GtkIconViewItem        *item);
static void                 gtk_icon_view_calculate_item_size2           (GtkIconView            *icon_view,
									  GtkIconViewItem        *item,
									  gint                   *max_height);
static void                 gtk_icon_view_update_rubberband              (gpointer                data);
static void                 gtk_icon_view_item_invalidate_size           (GtkIconViewItem        *item);
static void                 gtk_icon_view_invalidate_sizes               (GtkIconView            *icon_view);
static void                 gtk_icon_view_add_move_binding               (GtkBindingSet          *binding_set,
									  guint                   keyval,
									  guint                   modmask,
									  GtkMovementStep         step,
									  gint                    count);
static gboolean             gtk_icon_view_real_move_cursor               (GtkIconView            *icon_view,
									  GtkMovementStep         step,
									  gint                    count);
static void                 gtk_icon_view_move_cursor_up_down            (GtkIconView            *icon_view,
									  gint                    count);
static void                 gtk_icon_view_move_cursor_page_up_down       (GtkIconView            *icon_view,
									  gint                    count);
static void                 gtk_icon_view_move_cursor_left_right         (GtkIconView            *icon_view,
									  gint                    count);
static void                 gtk_icon_view_move_cursor_start_end          (GtkIconView            *icon_view,
									  gint                    count);
static void                 gtk_icon_view_scroll_to_item                 (GtkIconView            *icon_view,
									  GtkIconViewItem        *item);
static void                 gtk_icon_view_select_item                    (GtkIconView            *icon_view,
									  GtkIconViewItem        *item);
static void                 gtk_icon_view_unselect_item                  (GtkIconView            *icon_view,
									  GtkIconViewItem        *item);
static gboolean             __gtk_icon_view_select_all_between             (GtkIconView            *icon_view,
									  GtkIconViewItem        *anchor,
									  GtkIconViewItem        *cursor);
static GtkIconViewItem *    gtk_icon_view_get_item_at_coords             (GtkIconView            *icon_view,
									  gint                    x,
									  gint                    y,
									  gboolean                only_in_cell,
									  GtkIconViewCellInfo   **cell_at_pos);
static void                 gtk_icon_view_get_cell_area                  (GtkIconView            *icon_view,
									  GtkIconViewItem        *item,
									  GtkIconViewCellInfo    *cell_info,
									  GdkRectangle           *cell_area);
static void                 gtk_icon_view_get_cell_box                   (GtkIconView            *icon_view,
									  GtkIconViewItem        *item,
									  GtkIconViewCellInfo    *info,
									  GdkRectangle           *box);
static GtkIconViewCellInfo *gtk_icon_view_get_cell_info                  (GtkIconView            *icon_view,
									  GtkCellRenderer        *renderer);
static void                 gtk_icon_view_set_cell_data                  (GtkIconView            *icon_view,
									  GtkIconViewItem        *item);
static void                 gtk_icon_view_cell_layout_pack_start         (GtkCellLayout          *layout,
									  GtkCellRenderer        *renderer,
									  gboolean                expand);
static void                 gtk_icon_view_cell_layout_pack_end           (GtkCellLayout          *layout,
									  GtkCellRenderer        *renderer,
									  gboolean                expand);
static void                 gtk_icon_view_cell_layout_add_attribute      (GtkCellLayout          *layout,
									  GtkCellRenderer        *renderer,
									  const gchar            *attribute,
									  gint                    column);
static void                 gtk_icon_view_cell_layout_clear              (GtkCellLayout          *layout);
static void                 gtk_icon_view_cell_layout_clear_attributes   (GtkCellLayout          *layout,
									  GtkCellRenderer        *renderer);
static void                 gtk_icon_view_cell_layout_set_cell_data_func (GtkCellLayout          *layout,
									  GtkCellRenderer        *cell,
									  GtkCellLayoutDataFunc   func,
									  gpointer                func_data,
									  GDestroyNotify          destroy);
static void                 gtk_icon_view_cell_layout_reorder            (GtkCellLayout          *layout,
									  GtkCellRenderer        *cell,
									  gint                    position);
static GList *              gtk_icon_view_cell_layout_get_cells          (GtkCellLayout          *layout);

static void                 gtk_icon_view_item_activate_cell             (GtkIconView            *icon_view,
									  GtkIconViewItem        *item,
									  GtkIconViewCellInfo    *cell_info,
									  GdkEvent               *event);
static void                 gtk_icon_view_item_selected_changed          (GtkIconView            *icon_view,
		                                                          GtkIconViewItem        *item);
static void                 gtk_icon_view_put                            (GtkIconView            *icon_view,
									  GtkWidget              *widget,
									  GtkIconViewItem        *item,
									  gint                    cell);
static void                 gtk_icon_view_remove_widget                  (GtkCellEditable        *editable,
									  GtkIconView            *icon_view);
static void                 gtk_icon_view_start_editing                  (GtkIconView            *icon_view,
									  GtkIconViewItem        *item,
									  GtkIconViewCellInfo    *cell_info,
									  GdkEvent               *event);
static void                 gtk_icon_view_stop_editing                   (GtkIconView            *icon_view,
									  gboolean                cancel_editing);

/* Source side drag signals */
static void gtk_icon_view_drag_begin       (GtkWidget        *widget,
                                            GdkDragContext   *context);
static void gtk_icon_view_drag_end         (GtkWidget        *widget,
                                            GdkDragContext   *context);
static void gtk_icon_view_drag_data_get    (GtkWidget        *widget,
                                            GdkDragContext   *context,
                                            GtkSelectionData *selection_data,
                                            guint             info,
                                            guint             time);
static void gtk_icon_view_drag_data_delete (GtkWidget        *widget,
                                            GdkDragContext   *context);

/* Target side drag signals */
static void     gtk_icon_view_drag_leave         (GtkWidget        *widget,
                                                  GdkDragContext   *context,
                                                  guint             time);
static gboolean gtk_icon_view_drag_motion        (GtkWidget        *widget,
                                                  GdkDragContext   *context,
                                                  gint              x,
                                                  gint              y,
                                                  guint             time);
static gboolean gtk_icon_view_drag_drop          (GtkWidget        *widget,
                                                  GdkDragContext   *context,
                                                  gint              x,
                                                  gint              y,
                                                  guint             time);
static void     gtk_icon_view_drag_data_received (GtkWidget        *widget,
                                                  GdkDragContext   *context,
                                                  gint              x,
                                                  gint              y,
                                                  GtkSelectionData *selection_data,
                                                  guint             info,
                                                  guint             time);
static gboolean gtk_icon_view_maybe_begin_drag   (GtkIconView             *icon_view,
					   	  GdkEventMotion          *event);

static void     remove_scroll_timeout            (GtkIconView *icon_view);

static void     adjust_wrap_width                (GtkIconView     *icon_view,
						  GtkIconViewItem *item);

/* GtkBuildable */
static GtkBuildableIface *parent_buildable_iface;
static void     gtk_icon_view_buildable_init             (GtkBuildableIface *iface);
static gboolean gtk_icon_view_buildable_custom_tag_start (GtkBuildable  *buildable,
							  GtkBuilder    *builder,
							  GObject       *child,
							  const gchar   *tagname,
							  GMarkupParser *parser,
							  gpointer      *data);
static void     gtk_icon_view_buildable_custom_tag_end   (GtkBuildable  *buildable,
							  GtkBuilder    *builder,
							  GObject       *child,
							  const gchar   *tagname,
							  gpointer      *data);

static guint icon_view_signals[LAST_SIGNAL] = { 0 };

STLWRT_DEFINE_FTYPE_VPARENT (GtkIconView, gtk_icon_view, GTK_TYPE_CONTAINER, G_TYPE_FLAG_NONE,
                             G_IMPLEMENT_INTERFACE (GTK_TYPE_CELL_LAYOUT,
                                                    gtk_icon_view_cell_layout_init)
                             G_IMPLEMENT_INTERFACE (GTK_TYPE_BUILDABLE,
                                                    gtk_icon_view_buildable_init)
                             G_ADD_PRIVATE (GtkIconView))

static void
gtk_icon_view_class_init (GtkIconViewClass *klass)
{
  GObjectClass *gobject_class;
  GtkWidgetClass *widget_class;
  GtkContainerClass *container_class;
  GtkBindingSet *binding_set;
  
  binding_set = __gtk_binding_set_by_class (klass);

  gobject_class = (GObjectClass *) klass;
  widget_class = (GtkWidgetClass *) klass;
  container_class = (GtkContainerClass *) klass;

  gobject_class->finalize = gtk_icon_view_finalize;
  gobject_class->set_property = gtk_icon_view_set_property;
  gobject_class->get_property = gtk_icon_view_get_property;
  
  widget_class->realize = gtk_icon_view_realize;
  widget_class->unrealize = gtk_icon_view_unrealize;
  widget_class->style_set = gtk_icon_view_style_set;
  widget_class->get_accessible = gtk_icon_view_get_accessible;
  widget_class->size_request = gtk_icon_view_size_request;
  widget_class->size_allocate = gtk_icon_view_size_allocate;
  widget_class->expose_event = gtk_icon_view_expose;
  widget_class->motion_notify_event = gtk_icon_view_motion;
  widget_class->button_press_event = gtk_icon_view_button_press;
  widget_class->button_release_event = gtk_icon_view_button_release;
  widget_class->key_press_event = gtk_icon_view_key_press;
  widget_class->key_release_event = gtk_icon_view_key_release;
  widget_class->drag_begin = gtk_icon_view_drag_begin;
  widget_class->drag_end = gtk_icon_view_drag_end;
  widget_class->drag_data_get = gtk_icon_view_drag_data_get;
  widget_class->drag_data_delete = gtk_icon_view_drag_data_delete;
  widget_class->drag_leave = gtk_icon_view_drag_leave;
  widget_class->drag_motion = gtk_icon_view_drag_motion;
  widget_class->drag_drop = gtk_icon_view_drag_drop;
  widget_class->drag_data_received = gtk_icon_view_drag_data_received;
  widget_class->state_changed = gtk_icon_view_state_changed;

  container_class->remove = gtk_icon_view_remove;
  container_class->forall = gtk_icon_view_forall;

  klass->set_scroll_adjustments = gtk_icon_view_set_adjustments;
  klass->select_all = gtk_icon_view_real_select_all;
  klass->unselect_all = gtk_icon_view_real_unselect_all;
  klass->select_cursor_item = gtk_icon_view_real_select_cursor_item;
  klass->toggle_cursor_item = gtk_icon_view_real_toggle_cursor_item;
  klass->activate_cursor_item = gtk_icon_view_real_activate_cursor_item;  
  klass->move_cursor = gtk_icon_view_real_move_cursor;
  
  /* Properties */
  /**
   * GtkIconView:selection-mode:
   * 
   * The ::selection-mode property specifies the selection mode of
   * icon view. If the mode is #GTK_SELECTION_MULTIPLE, rubberband selection
   * is enabled, for the other modes, only keyboard selection is possible.
   *
   * Since: 2.6
   */
  g_object_class_install_property (gobject_class,
				   PROP_SELECTION_MODE,
				   g_param_spec_enum ("selection-mode",
						      P_("Selection mode"),
						      P_("The selection mode"),
						      GTK_TYPE_SELECTION_MODE,
						      GTK_SELECTION_SINGLE,
						      GTK_PARAM_READWRITE));

  /**
   * GtkIconView:pixbuf-column:
   *
   * The ::pixbuf-column property contains the number of the model column
   * containing the pixbufs which are displayed. The pixbuf column must be 
   * of type #GDK_TYPE_PIXBUF. Setting this property to -1 turns off the
   * display of pixbufs.
   *
   * Since: 2.6
   */
  g_object_class_install_property (gobject_class,
				   PROP_PIXBUF_COLUMN,
				   g_param_spec_int ("pixbuf-column",
						     P_("Pixbuf column"),
						     P_("Model column used to retrieve the icon pixbuf from"),
						     -1, G_MAXINT, -1,
						     GTK_PARAM_READWRITE));

  /**
   * GtkIconView:text-column:
   *
   * The ::text-column property contains the number of the model column
   * containing the texts which are displayed. The text column must be 
   * of type #G_TYPE_STRING. If this property and the :markup-column 
   * property are both set to -1, no texts are displayed.   
   *
   * Since: 2.6
   */
  g_object_class_install_property (gobject_class,
				   PROP_TEXT_COLUMN,
				   g_param_spec_int ("text-column",
						     P_("Text column"),
						     P_("Model column used to retrieve the text from"),
						     -1, G_MAXINT, -1,
						     GTK_PARAM_READWRITE));

  
  /**
   * GtkIconView:markup-column:
   *
   * The ::markup-column property contains the number of the model column
   * containing markup information to be displayed. The markup column must be 
   * of type #G_TYPE_STRING. If this property and the :text-column property 
   * are both set to column numbers, it overrides the text column.
   * If both are set to -1, no texts are displayed.   
   *
   * Since: 2.6
   */
  g_object_class_install_property (gobject_class,
				   PROP_MARKUP_COLUMN,
				   g_param_spec_int ("markup-column",
						     P_("Markup column"),
						     P_("Model column used to retrieve the text if using Pango markup"),
						     -1, G_MAXINT, -1,
						     GTK_PARAM_READWRITE));
  
  g_object_class_install_property (gobject_class,
                                   PROP_MODEL,
                                   g_param_spec_object ("model",
							P_("Icon View Model"),
							P_("The model for the icon view"),
							GTK_TYPE_TREE_MODEL,
							GTK_PARAM_READWRITE));
  
  /**
   * GtkIconView:columns:
   *
   * The columns property contains the number of the columns in which the
   * items should be displayed. If it is -1, the number of columns will
   * be chosen automatically to fill the available area.
   *
   * Since: 2.6
   */
  g_object_class_install_property (gobject_class,
				   PROP_COLUMNS,
				   g_param_spec_int ("columns",
						     P_("Number of columns"),
						     P_("Number of columns to display"),
						     -1, G_MAXINT, -1,
						     GTK_PARAM_READWRITE));
  

  /**
   * GtkIconView:item-width:
   *
   * The item-width property specifies the width to use for each item. 
   * If it is set to -1, the icon view will automatically determine a 
   * suitable item size.
   *
   * Since: 2.6
   */
  g_object_class_install_property (gobject_class,
				   PROP_ITEM_WIDTH,
				   g_param_spec_int ("item-width",
						     P_("Width for each item"),
						     P_("The width used for each item"),
						     -1, G_MAXINT, -1,
						     GTK_PARAM_READWRITE));  

  /**
   * GtkIconView:spacing:
   *
   * The spacing property specifies the space which is inserted between
   * the cells (i.e. the icon and the text) of an item.
   *
   * Since: 2.6
   */
  g_object_class_install_property (gobject_class,
                                   PROP_SPACING,
                                   g_param_spec_int ("spacing",
						     P_("Spacing"),
						     P_("Space which is inserted between cells of an item"),
						     0, G_MAXINT, 0,
						     GTK_PARAM_READWRITE));

  /**
   * GtkIconView:row-spacing:
   *
   * The row-spacing property specifies the space which is inserted between
   * the rows of the icon view.
   *
   * Since: 2.6
   */
  g_object_class_install_property (gobject_class,
                                   PROP_ROW_SPACING,
                                   g_param_spec_int ("row-spacing",
						     P_("Row Spacing"),
						     P_("Space which is inserted between grid rows"),
						     0, G_MAXINT, 6,
						     GTK_PARAM_READWRITE));

  /**
   * GtkIconView:column-spacing:
   *
   * The column-spacing property specifies the space which is inserted between
   * the columns of the icon view.
   *
   * Since: 2.6
   */
  g_object_class_install_property (gobject_class,
                                   PROP_COLUMN_SPACING,
                                   g_param_spec_int ("column-spacing",
						     P_("Column Spacing"),
						     P_("Space which is inserted between grid columns"),
						     0, G_MAXINT, 6,
						     GTK_PARAM_READWRITE));

  /**
   * GtkIconView:margin:
   *
   * The margin property specifies the space which is inserted 
   * at the edges of the icon view.
   *
   * Since: 2.6
   */
  g_object_class_install_property (gobject_class,
                                   PROP_MARGIN,
                                   g_param_spec_int ("margin",
						     P_("Margin"),
						     P_("Space which is inserted at the edges of the icon view"),
						     0, G_MAXINT, 6,
						     GTK_PARAM_READWRITE));

  /**
   * GtkIconView:orientation:
   *
   * The orientation property specifies how the cells (i.e. the icon and 
   * the text) of the item are positioned relative to each other.
   *
   * Since: 2.6
   *
   * Deprecated: 2.22: Use the #GtkIconView::item-orientation property
   */
  g_object_class_install_property (gobject_class,
				   PROP_ORIENTATION,
				   g_param_spec_enum ("orientation",
						      P_("Orientation"),
						      P_("How the text and icon of each item are positioned relative to each other"),
						      GTK_TYPE_ORIENTATION,
						      GTK_ORIENTATION_VERTICAL,
						      GTK_PARAM_READWRITE | G_PARAM_DEPRECATED));

  /**
   * GtkIconView:item-orientation:
   *
   * The item-orientation property specifies how the cells (i.e. the icon and
   * the text) of the item are positioned relative to each other.
   *
   * Since: 2.22
   */
  g_object_class_install_property (gobject_class,
                                   PROP_ITEM_ORIENTATION,
                                   g_param_spec_enum ("item-orientation",
                                                      P_("Item Orientation"),
                                                      P_("How the text and icon of each item are positioned relative to each other"),
                                                      GTK_TYPE_ORIENTATION,
                                                      GTK_ORIENTATION_VERTICAL,
                                                      GTK_PARAM_READWRITE));

  /**
   * GtkIconView:reorderable:
   *
   * The reorderable property specifies if the items can be reordered
   * by DND.
   *
   * Since: 2.8
   */
  g_object_class_install_property (gobject_class,
                                   PROP_REORDERABLE,
                                   g_param_spec_boolean ("reorderable",
							 P_("Reorderable"),
							 P_("View is reorderable"),
							 FALSE,
							 G_PARAM_READWRITE));

    g_object_class_install_property (gobject_class,
                                     PROP_TOOLTIP_COLUMN,
                                     g_param_spec_int ("tooltip-column",
                                                       P_("Tooltip Column"),
                                                       P_("The column in the model containing the tooltip texts for the items"),
                                                       -1,
                                                       G_MAXINT,
                                                       -1,
                                                       GTK_PARAM_READWRITE));

  /**
   * GtkIconView:item-padding:
   *
   * The item-padding property specifies the padding around each
   * of the icon view's item.
   *
   * Since: 2.18
   */
  g_object_class_install_property (gobject_class,
                                   PROP_ITEM_PADDING,
                                   g_param_spec_int ("item-padding",
						     P_("Item Padding"),
						     P_("Padding around icon view items"),
						     0, G_MAXINT, 6,
						     GTK_PARAM_READWRITE));



  /* Style properties */
  __gtk_widget_class_install_style_property (widget_class,
                                           g_param_spec_boxed ("selection-box-color",
                                                               P_("Selection Box Color"),
                                                               P_("Color of the selection box"),
                                                               GDK_TYPE_COLOR,
                                                               GTK_PARAM_READABLE));

  __gtk_widget_class_install_style_property (widget_class,
                                           g_param_spec_uchar ("selection-box-alpha",
                                                               P_("Selection Box Alpha"),
                                                               P_("Opacity of the selection box"),
                                                               0, 0xff,
                                                               0x40,
                                                               GTK_PARAM_READABLE));

  /* Signals */
  /**
   * GtkIconView::set-scroll-adjustments
   * @horizontal: the horizontal #GtkAdjustment
   * @vertical: the vertical #GtkAdjustment
   *
   * Set the scroll adjustments for the icon view. Usually scrolled containers
   * like #GtkScrolledWindow will emit this signal to connect two instances
   * of #GtkScrollbar to the scroll directions of the #GtkIconView.
   */
  widget_class->set_scroll_adjustments_signal =
    g_signal_new (I_("set-scroll-adjustments"),
		  G_TYPE_FROM_CLASS (gobject_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GtkIconViewClass, set_scroll_adjustments),
		  NULL, NULL, 
		  NULL,
		  G_TYPE_NONE, 2,
		  GTK_TYPE_ADJUSTMENT, GTK_TYPE_ADJUSTMENT);

  /**
   * GtkIconView::item-activated:
   * @iconview: the object on which the signal is emitted
   * @path: the #GtkTreePath for the activated item
   *
   * The ::item-activated signal is emitted when the method
   * __gtk_icon_view_item_activated() is called or the user double 
   * clicks an item. It is also emitted when a non-editable item
   * is selected and one of the keys: Space, Return or Enter is
   * pressed.
   */
  icon_view_signals[ITEM_ACTIVATED] =
    g_signal_new (I_("item-activated"),
		  G_TYPE_FROM_CLASS (gobject_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GtkIconViewClass, item_activated),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__BOXED,
		  G_TYPE_NONE, 1,
		  GTK_TYPE_TREE_PATH);

  /**
   * GtkIconView::selection-changed:
   * @iconview: the object on which the signal is emitted
   *
   * The ::selection-changed signal is emitted when the selection
   * (i.e. the set of selected items) changes.
   */
  icon_view_signals[SELECTION_CHANGED] =
    g_signal_new (I_("selection-changed"),
		  G_TYPE_FROM_CLASS (gobject_class),
		  G_SIGNAL_RUN_FIRST,
		  G_STRUCT_OFFSET (GtkIconViewClass, selection_changed),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);
  
  /**
   * GtkIconView::select-all:
   * @iconview: the object on which the signal is emitted
   *
   * A <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted when the user selects all items.
   *
   * Applications should not connect to it, but may emit it with
   * g_signal_emit_by_name() if they need to control selection
   * programmatically.
   * 
   * The default binding for this signal is Ctrl-a.
   */
  icon_view_signals[SELECT_ALL] =
    g_signal_new (I_("select-all"),
		  G_TYPE_FROM_CLASS (gobject_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkIconViewClass, select_all),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);
  
  /**
   * GtkIconView::unselect-all:
   * @iconview: the object on which the signal is emitted
   *
   * A <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted when the user unselects all items.
   *
   * Applications should not connect to it, but may emit it with
   * g_signal_emit_by_name() if they need to control selection
   * programmatically.
   * 
   * The default binding for this signal is Ctrl-Shift-a. 
   */
  icon_view_signals[UNSELECT_ALL] =
    g_signal_new (I_("unselect-all"),
		  G_TYPE_FROM_CLASS (gobject_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkIconViewClass, unselect_all),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  /**
   * GtkIconView::select-cursor-item:
   * @iconview: the object on which the signal is emitted
   *
   * A <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted when the user selects the item that is currently
   * focused.
   *
   * Applications should not connect to it, but may emit it with
   * g_signal_emit_by_name() if they need to control selection
   * programmatically.
   * 
   * There is no default binding for this signal.
   */
  icon_view_signals[SELECT_CURSOR_ITEM] =
    g_signal_new (I_("select-cursor-item"),
		  G_TYPE_FROM_CLASS (gobject_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkIconViewClass, select_cursor_item),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  /**
   * GtkIconView::toggle-cursor-item:
   * @iconview: the object on which the signal is emitted
   *
   * A <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted when the user toggles whether the currently
   * focused item is selected or not. The exact effect of this 
   * depend on the selection mode.
   *
   * Applications should not connect to it, but may emit it with
   * g_signal_emit_by_name() if they need to control selection
   * programmatically.
   * 
   * There is no default binding for this signal is Ctrl-Space.
   */
  icon_view_signals[TOGGLE_CURSOR_ITEM] =
    g_signal_new (I_("toggle-cursor-item"),
		  G_TYPE_FROM_CLASS (gobject_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkIconViewClass, toggle_cursor_item),
		  NULL, NULL,
		  g_cclosure_marshal_VOID__VOID,
		  G_TYPE_NONE, 0);

  /**
   * GtkIconView::activate-cursor-item:
   * @iconview: the object on which the signal is emitted
   *
   * A <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted when the user activates the currently 
   * focused item. 
   *
   * Applications should not connect to it, but may emit it with
   * g_signal_emit_by_name() if they need to control activation
   * programmatically.
   * 
   * The default bindings for this signal are Space, Return and Enter.
   */
  icon_view_signals[ACTIVATE_CURSOR_ITEM] =
    g_signal_new (I_("activate-cursor-item"),
		  G_TYPE_FROM_CLASS (gobject_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkIconViewClass, activate_cursor_item),
		  NULL, NULL,
		  NULL,
		  G_TYPE_BOOLEAN, 0);
  
  /**
   * GtkIconView::move-cursor:
   * @iconview: the object which received the signal
   * @step: the granularity of the move, as a #GtkMovementStep
   * @count: the number of @step units to move
   *
   * The ::move-cursor signal is a
   * <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted when the user initiates a cursor movement.
   *
   * Applications should not connect to it, but may emit it with
   * g_signal_emit_by_name() if they need to control the cursor
   * programmatically.
   *
   * The default bindings for this signal include
   * <itemizedlist>
   * <listitem>Arrow keys which move by individual steps</listitem>
   * <listitem>Home/End keys which move to the first/last item</listitem>
   * <listitem>PageUp/PageDown which move by "pages"</listitem>
   * </itemizedlist>
   *
   * All of these will extend the selection when combined with
   * the Shift modifier.
   */
  icon_view_signals[MOVE_CURSOR] =
    g_signal_new (I_("move-cursor"),
		  G_TYPE_FROM_CLASS (gobject_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkIconViewClass, move_cursor),
		  NULL, NULL,
		  NULL,
		  G_TYPE_BOOLEAN, 2,
		  GTK_TYPE_MOVEMENT_STEP,
		  G_TYPE_INT);

  /* Key bindings */
  __gtk_binding_entry_add_signal (binding_set, GDK_a, GDK_CONTROL_MASK, 
				"select-all", 0);
  __gtk_binding_entry_add_signal (binding_set, GDK_a, GDK_CONTROL_MASK | GDK_SHIFT_MASK, 
				"unselect-all", 0);
  __gtk_binding_entry_add_signal (binding_set, GDK_space, GDK_CONTROL_MASK, 
				"toggle-cursor-item", 0);
  __gtk_binding_entry_add_signal (binding_set, GDK_KP_Space, GDK_CONTROL_MASK,
				"toggle-cursor-item", 0);

  __gtk_binding_entry_add_signal (binding_set, GDK_space, 0, 
				"activate-cursor-item", 0);
  __gtk_binding_entry_add_signal (binding_set, GDK_KP_Space, 0,
				"activate-cursor-item", 0);
  __gtk_binding_entry_add_signal (binding_set, GDK_Return, 0, 
				"activate-cursor-item", 0);
  __gtk_binding_entry_add_signal (binding_set, GDK_ISO_Enter, 0, 
				"activate-cursor-item", 0);
  __gtk_binding_entry_add_signal (binding_set, GDK_KP_Enter, 0, 
				"activate-cursor-item", 0);

  gtk_icon_view_add_move_binding (binding_set, GDK_Up, 0,
				  GTK_MOVEMENT_DISPLAY_LINES, -1);
  gtk_icon_view_add_move_binding (binding_set, GDK_KP_Up, 0,
				  GTK_MOVEMENT_DISPLAY_LINES, -1);

  gtk_icon_view_add_move_binding (binding_set, GDK_Down, 0,
				  GTK_MOVEMENT_DISPLAY_LINES, 1);
  gtk_icon_view_add_move_binding (binding_set, GDK_KP_Down, 0,
				  GTK_MOVEMENT_DISPLAY_LINES, 1);

  gtk_icon_view_add_move_binding (binding_set, GDK_p, GDK_CONTROL_MASK,
				  GTK_MOVEMENT_DISPLAY_LINES, -1);

  gtk_icon_view_add_move_binding (binding_set, GDK_n, GDK_CONTROL_MASK,
				  GTK_MOVEMENT_DISPLAY_LINES, 1);

  gtk_icon_view_add_move_binding (binding_set, GDK_Home, 0,
				  GTK_MOVEMENT_BUFFER_ENDS, -1);
  gtk_icon_view_add_move_binding (binding_set, GDK_KP_Home, 0,
				  GTK_MOVEMENT_BUFFER_ENDS, -1);

  gtk_icon_view_add_move_binding (binding_set, GDK_End, 0,
				  GTK_MOVEMENT_BUFFER_ENDS, 1);
  gtk_icon_view_add_move_binding (binding_set, GDK_KP_End, 0,
				  GTK_MOVEMENT_BUFFER_ENDS, 1);

  gtk_icon_view_add_move_binding (binding_set, GDK_Page_Up, 0,
				  GTK_MOVEMENT_PAGES, -1);
  gtk_icon_view_add_move_binding (binding_set, GDK_KP_Page_Up, 0,
				  GTK_MOVEMENT_PAGES, -1);

  gtk_icon_view_add_move_binding (binding_set, GDK_Page_Down, 0,
				  GTK_MOVEMENT_PAGES, 1);
  gtk_icon_view_add_move_binding (binding_set, GDK_KP_Page_Down, 0,
				  GTK_MOVEMENT_PAGES, 1);

  gtk_icon_view_add_move_binding (binding_set, GDK_Right, 0, 
				  GTK_MOVEMENT_VISUAL_POSITIONS, 1);
  gtk_icon_view_add_move_binding (binding_set, GDK_Left, 0, 
				  GTK_MOVEMENT_VISUAL_POSITIONS, -1);

  gtk_icon_view_add_move_binding (binding_set, GDK_KP_Right, 0, 
				  GTK_MOVEMENT_VISUAL_POSITIONS, 1);
  gtk_icon_view_add_move_binding (binding_set, GDK_KP_Left, 0, 
				  GTK_MOVEMENT_VISUAL_POSITIONS, -1);
}

static void
gtk_icon_view_buildable_init (GtkBuildableIface *iface)
{
  parent_buildable_iface = g_type_interface_peek_parent (iface);
  iface->add_child = ___gtk_cell_layout_buildable_add_child;
  iface->custom_tag_start = gtk_icon_view_buildable_custom_tag_start;
  iface->custom_tag_end = gtk_icon_view_buildable_custom_tag_end;
}

static void
gtk_icon_view_cell_layout_init (GtkCellLayoutIface *iface)
{
  iface->pack_start = gtk_icon_view_cell_layout_pack_start;
  iface->pack_end = gtk_icon_view_cell_layout_pack_end;
  iface->clear = gtk_icon_view_cell_layout_clear;
  iface->add_attribute = gtk_icon_view_cell_layout_add_attribute;
  iface->set_cell_data_func = gtk_icon_view_cell_layout_set_cell_data_func;
  iface->clear_attributes = gtk_icon_view_cell_layout_clear_attributes;
  iface->reorder = gtk_icon_view_cell_layout_reorder;
  iface->get_cells = gtk_icon_view_cell_layout_get_cells;
}

static void
gtk_icon_view_init (GtkIconView *icon_view)
{
  gtk_icon_view_get_props (icon_view)->priv = gtk_icon_view_get_instance_private (icon_view);
  
  gtk_icon_view_get_props (icon_view)->priv->width = 0;
  gtk_icon_view_get_props (icon_view)->priv->height = 0;
  gtk_icon_view_get_props (icon_view)->priv->selection_mode = GTK_SELECTION_SINGLE;
  gtk_icon_view_get_props (icon_view)->priv->pressed_button = -1;
  gtk_icon_view_get_props (icon_view)->priv->press_start_x = -1;
  gtk_icon_view_get_props (icon_view)->priv->press_start_y = -1;
  gtk_icon_view_get_props (icon_view)->priv->text_column = -1;
  gtk_icon_view_get_props (icon_view)->priv->markup_column = -1;  
  gtk_icon_view_get_props (icon_view)->priv->pixbuf_column = -1;
  gtk_icon_view_get_props (icon_view)->priv->text_cell = -1;
  gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell = -1;  
  gtk_icon_view_get_props (icon_view)->priv->tooltip_column = -1;  

  __gtk_widget_set_can_focus (GTK_WIDGET (icon_view), TRUE);
  
  gtk_icon_view_set_adjustments (icon_view, NULL, NULL);

  gtk_icon_view_get_props (icon_view)->priv->cell_list = NULL;
  gtk_icon_view_get_props (icon_view)->priv->n_cells = 0;
  gtk_icon_view_get_props (icon_view)->priv->cursor_cell = -1;

  gtk_icon_view_get_props (icon_view)->priv->item_orientation = GTK_ORIENTATION_VERTICAL;

  gtk_icon_view_get_props (icon_view)->priv->columns = -1;
  gtk_icon_view_get_props (icon_view)->priv->item_width = -1;
  gtk_icon_view_get_props (icon_view)->priv->spacing = 0;
  gtk_icon_view_get_props (icon_view)->priv->row_spacing = 6;
  gtk_icon_view_get_props (icon_view)->priv->column_spacing = 6;
  gtk_icon_view_get_props (icon_view)->priv->margin = 6;
  gtk_icon_view_get_props (icon_view)->priv->item_padding = 6;

  gtk_icon_view_get_props (icon_view)->priv->draw_focus = TRUE;
}

/* GObject methods */
static void
gtk_icon_view_finalize (GObject *object)
{
  gtk_icon_view_cell_layout_clear (GTK_CELL_LAYOUT (object));

  G_OBJECT_CLASS (gtk_icon_view_parent_class)->finalize (object);
}


static void
gtk_icon_view_set_property (GObject      *object,
			    guint         prop_id,
			    const GValue *value,
			    GParamSpec   *pspec)
{
  GtkIconView *icon_view;

  icon_view = GTK_ICON_VIEW (object);

  switch (prop_id)
    {
    case PROP_SELECTION_MODE:
      __gtk_icon_view_set_selection_mode (icon_view, g_value_get_enum (value));
      break;
    case PROP_PIXBUF_COLUMN:
      __gtk_icon_view_set_pixbuf_column (icon_view, g_value_get_int (value));
      break;
    case PROP_TEXT_COLUMN:
      __gtk_icon_view_set_text_column (icon_view, g_value_get_int (value));
      break;
    case PROP_MARKUP_COLUMN:
      __gtk_icon_view_set_markup_column (icon_view, g_value_get_int (value));
      break;
    case PROP_MODEL:
      __gtk_icon_view_set_model (icon_view, g_value_get_object (value));
      break;
    case PROP_ORIENTATION:
    case PROP_ITEM_ORIENTATION:
      __gtk_icon_view_set_item_orientation (icon_view, g_value_get_enum (value));
      break;
    case PROP_COLUMNS:
      __gtk_icon_view_set_columns (icon_view, g_value_get_int (value));
      break;
    case PROP_ITEM_WIDTH:
      __gtk_icon_view_set_item_width (icon_view, g_value_get_int (value));
      break;
    case PROP_SPACING:
      __gtk_icon_view_set_spacing (icon_view, g_value_get_int (value));
      break;
    case PROP_ROW_SPACING:
      __gtk_icon_view_set_row_spacing (icon_view, g_value_get_int (value));
      break;
    case PROP_COLUMN_SPACING:
      __gtk_icon_view_set_column_spacing (icon_view, g_value_get_int (value));
      break;
    case PROP_MARGIN:
      __gtk_icon_view_set_margin (icon_view, g_value_get_int (value));
      break;
    case PROP_REORDERABLE:
      __gtk_icon_view_set_reorderable (icon_view, g_value_get_boolean (value));
      break;
      
    case PROP_TOOLTIP_COLUMN:
      __gtk_icon_view_set_tooltip_column (icon_view, g_value_get_int (value));
      break;

    case PROP_ITEM_PADDING:
      __gtk_icon_view_set_item_padding (icon_view, g_value_get_int (value));
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_icon_view_get_property (GObject      *object,
			    guint         prop_id,
			    GValue       *value,
			    GParamSpec   *pspec)
{
  GtkIconView *icon_view;

  icon_view = GTK_ICON_VIEW (object);

  switch (prop_id)
    {
    case PROP_SELECTION_MODE:
      g_value_set_enum (value, gtk_icon_view_get_props (icon_view)->priv->selection_mode);
      break;
    case PROP_PIXBUF_COLUMN:
      g_value_set_int (value, gtk_icon_view_get_props (icon_view)->priv->pixbuf_column);
      break;
    case PROP_TEXT_COLUMN:
      g_value_set_int (value, gtk_icon_view_get_props (icon_view)->priv->text_column);
      break;
    case PROP_MARKUP_COLUMN:
      g_value_set_int (value, gtk_icon_view_get_props (icon_view)->priv->markup_column);
      break;
    case PROP_MODEL:
      g_value_set_object (value, gtk_icon_view_get_props (icon_view)->priv->model);
      break;
    case PROP_ORIENTATION:
    case PROP_ITEM_ORIENTATION:
      g_value_set_enum (value, gtk_icon_view_get_props (icon_view)->priv->item_orientation);
      break;
    case PROP_COLUMNS:
      g_value_set_int (value, gtk_icon_view_get_props (icon_view)->priv->columns);
      break;
    case PROP_ITEM_WIDTH:
      g_value_set_int (value, gtk_icon_view_get_props (icon_view)->priv->item_width);
      break;
    case PROP_SPACING:
      g_value_set_int (value, gtk_icon_view_get_props (icon_view)->priv->spacing);
      break;
    case PROP_ROW_SPACING:
      g_value_set_int (value, gtk_icon_view_get_props (icon_view)->priv->row_spacing);
      break;
    case PROP_COLUMN_SPACING:
      g_value_set_int (value, gtk_icon_view_get_props (icon_view)->priv->column_spacing);
      break;
    case PROP_MARGIN:
      g_value_set_int (value, gtk_icon_view_get_props (icon_view)->priv->margin);
      break;
    case PROP_REORDERABLE:
      g_value_set_boolean (value, gtk_icon_view_get_props (icon_view)->priv->reorderable);
      break;
    case PROP_TOOLTIP_COLUMN:
      g_value_set_int (value, gtk_icon_view_get_props (icon_view)->priv->tooltip_column);
      break;

    case PROP_ITEM_PADDING:
      g_value_set_int (value, gtk_icon_view_get_props (icon_view)->priv->item_padding);
      break;

    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

/* GtkWidget signals */
static void
gtk_icon_view_realize (GtkWidget *widget)
{
  GtkIconView *icon_view;
  GdkWindowAttr attributes;
  gint attributes_mask;
  
  icon_view = GTK_ICON_VIEW (widget);

  __gtk_widget_set_realized (widget, TRUE);

  /* Make the main, clipping window */
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = gtk_widget_get_props (widget)->allocation.x;
  attributes.y = gtk_widget_get_props (widget)->allocation.y;
  attributes.width = gtk_widget_get_props (widget)->allocation.width;
  attributes.height = gtk_widget_get_props (widget)->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = __gtk_widget_get_visual (widget);
  attributes.colormap = __gtk_widget_get_colormap (widget);
  attributes.event_mask = GDK_VISIBILITY_NOTIFY_MASK;

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  gtk_widget_get_props (widget)->window = __gdk_window_new (__gtk_widget_get_parent_window (widget),
				   &attributes, attributes_mask);
  __gdk_window_set_back_pixmap (gtk_widget_get_props (widget)->window, NULL, FALSE);
  __gdk_window_set_user_data (gtk_widget_get_props (widget)->window, widget);

  /* Make the window for the icon view */
  attributes.x = 0;
  attributes.y = 0;
  attributes.width = MAX (gtk_icon_view_get_props (icon_view)->priv->width, gtk_widget_get_props (widget)->allocation.width);
  attributes.height = MAX (gtk_icon_view_get_props (icon_view)->priv->height, gtk_widget_get_props (widget)->allocation.height);
  attributes.event_mask = (GDK_EXPOSURE_MASK |
			   GDK_SCROLL_MASK |
			   GDK_POINTER_MOTION_MASK |
			   GDK_BUTTON_PRESS_MASK |
			   GDK_BUTTON_RELEASE_MASK |
			   GDK_KEY_PRESS_MASK |
			   GDK_KEY_RELEASE_MASK) |
    __gtk_widget_get_events (widget);
  
  gtk_icon_view_get_props (icon_view)->priv->bin_window = __gdk_window_new (gtk_widget_get_props (widget)->window,
						&attributes, attributes_mask);
  __gdk_window_set_user_data (gtk_icon_view_get_props (icon_view)->priv->bin_window, widget);

  gtk_widget_get_props (widget)->style = __gtk_style_attach (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window);
  __gdk_window_set_background (gtk_icon_view_get_props (icon_view)->priv->bin_window, &gtk_widget_get_props (widget)->style->base[gtk_widget_get_props (widget)->state]);

  __gdk_window_show (gtk_icon_view_get_props (icon_view)->priv->bin_window);
}

static void
gtk_icon_view_unrealize (GtkWidget *widget)
{
  GtkIconView *icon_view;

  icon_view = GTK_ICON_VIEW (widget);

  __gdk_window_set_user_data (gtk_icon_view_get_props (icon_view)->priv->bin_window, NULL);
  __gdk_window_destroy (gtk_icon_view_get_props (icon_view)->priv->bin_window);
  gtk_icon_view_get_props (icon_view)->priv->bin_window = NULL;

  GTK_WIDGET_CLASS (gtk_icon_view_parent_class)->unrealize (widget);
}

static void
gtk_icon_view_state_changed (GtkWidget      *widget,
		 	     GtkStateType    previous_state)
{
  GtkIconView *icon_view = GTK_ICON_VIEW (widget);

  if (__gtk_widget_get_realized (widget))
    {
      __gdk_window_set_background (gtk_widget_get_props (widget)->window, &gtk_widget_get_props (widget)->style->base[gtk_widget_get_props (widget)->state]);
      __gdk_window_set_background (gtk_icon_view_get_props (icon_view)->priv->bin_window, &gtk_widget_get_props (widget)->style->base[gtk_widget_get_props (widget)->state]);
    }

  __gtk_widget_queue_draw (widget);
}

static void
gtk_icon_view_style_set (GtkWidget *widget,
			 GtkStyle *previous_style)
{
  GtkIconView *icon_view = GTK_ICON_VIEW (widget);

  if (__gtk_widget_get_realized (widget))
    {
      __gdk_window_set_background (gtk_widget_get_props (widget)->window, &gtk_widget_get_props (widget)->style->base[gtk_widget_get_props (widget)->state]);
      __gdk_window_set_background (gtk_icon_view_get_props (icon_view)->priv->bin_window, &gtk_widget_get_props (widget)->style->base[gtk_widget_get_props (widget)->state]);
    }

  __gtk_widget_queue_resize (widget);
}

static void
gtk_icon_view_size_request (GtkWidget      *widget,
			    GtkRequisition *requisition)
{
  GtkIconView *icon_view = GTK_ICON_VIEW (widget);
  GList *tmp_list;

  requisition->width = gtk_icon_view_get_props (icon_view)->priv->width;
  requisition->height = gtk_icon_view_get_props (icon_view)->priv->height;

  tmp_list = gtk_icon_view_get_props (icon_view)->priv->children;

  while (tmp_list)
    {
      GtkIconViewChild *child = tmp_list->data;
      GtkRequisition child_requisition;

      tmp_list = tmp_list->next;

      if (__gtk_widget_get_visible (child->widget))
        __gtk_widget_size_request (child->widget, &child_requisition);
    }  
}

static void
gtk_icon_view_allocate_children (GtkIconView *icon_view)
{
  GList *list;

  for (list = gtk_icon_view_get_props (icon_view)->priv->children; list; list = list->next)
    {
      GtkIconViewChild *child = list->data;
      GtkAllocation allocation;

      /* totally ignore our child's requisition */
      if (child->cell < 0)
	{
	  allocation.x = child->item->x + gtk_icon_view_get_props (icon_view)->priv->item_padding;
	  allocation.y = child->item->y + gtk_icon_view_get_props (icon_view)->priv->item_padding;
	  allocation.width = child->item->width - gtk_icon_view_get_props (icon_view)->priv->item_padding * 2;
	  allocation.height = child->item->height - gtk_icon_view_get_props (icon_view)->priv->item_padding * 2;
	}
      else
	{
	  GdkRectangle *box = &child->item->box[child->cell];

	  allocation.x = box->x;
	  allocation.y = box->y;
	  allocation.width = box->width;
	  allocation.height = box->height;
	}

      __gtk_widget_size_allocate (child->widget, &allocation);
    }
}

static void
gtk_icon_view_size_allocate (GtkWidget      *widget,
			     GtkAllocation  *allocation)
{
  GtkIconView *icon_view = GTK_ICON_VIEW (widget);

  GtkAdjustment *hadjustment, *vadjustment;

  gtk_widget_get_props (widget)->allocation = *allocation;
  
  if (__gtk_widget_get_realized (widget))
    {
      __gdk_window_move_resize (gtk_widget_get_props (widget)->window,
			      allocation->x, allocation->y,
			      allocation->width, allocation->height);
      __gdk_window_resize (gtk_icon_view_get_props (icon_view)->priv->bin_window,
			 MAX (gtk_icon_view_get_props (icon_view)->priv->width, allocation->width),
			 MAX (gtk_icon_view_get_props (icon_view)->priv->height, allocation->height));
    }

  gtk_icon_view_layout (icon_view);
  
  gtk_icon_view_allocate_children (icon_view);

  hadjustment = gtk_icon_view_get_props (icon_view)->priv->hadjustment;
  vadjustment = gtk_icon_view_get_props (icon_view)->priv->vadjustment;

  gtk_adjustment_get_props (hadjustment)->page_size = allocation->width;
  gtk_adjustment_get_props (hadjustment)->page_increment = allocation->width * 0.9;
  gtk_adjustment_get_props (hadjustment)->step_increment = allocation->width * 0.1;
  gtk_adjustment_get_props (hadjustment)->lower = 0;
  gtk_adjustment_get_props (hadjustment)->upper = MAX (allocation->width, gtk_icon_view_get_props (icon_view)->priv->width);

  if (gtk_adjustment_get_props (hadjustment)->value > gtk_adjustment_get_props (hadjustment)->upper - gtk_adjustment_get_props (hadjustment)->page_size)
    __gtk_adjustment_set_value (hadjustment, MAX (0, gtk_adjustment_get_props (hadjustment)->upper - gtk_adjustment_get_props (hadjustment)->page_size));

  gtk_adjustment_get_props (vadjustment)->page_size = allocation->height;
  gtk_adjustment_get_props (vadjustment)->page_increment = allocation->height * 0.9;
  gtk_adjustment_get_props (vadjustment)->step_increment = allocation->height * 0.1;
  gtk_adjustment_get_props (vadjustment)->lower = 0;
  gtk_adjustment_get_props (vadjustment)->upper = MAX (allocation->height, gtk_icon_view_get_props (icon_view)->priv->height);

  if (gtk_adjustment_get_props (vadjustment)->value > gtk_adjustment_get_props (vadjustment)->upper - gtk_adjustment_get_props (vadjustment)->page_size)
    __gtk_adjustment_set_value (vadjustment, MAX (0, gtk_adjustment_get_props (vadjustment)->upper - gtk_adjustment_get_props (vadjustment)->page_size));

  if (__gtk_widget_get_realized (widget) &&
      gtk_icon_view_get_props (icon_view)->priv->scroll_to_path)
    {
      GtkTreePath *path;
      path = __gtk_tree_row_reference_get_path (gtk_icon_view_get_props (icon_view)->priv->scroll_to_path);
      __gtk_tree_row_reference_free (gtk_icon_view_get_props (icon_view)->priv->scroll_to_path);
      gtk_icon_view_get_props (icon_view)->priv->scroll_to_path = NULL;

      __gtk_icon_view_scroll_to_path (icon_view, path,
				    gtk_icon_view_get_props (icon_view)->priv->scroll_to_use_align,
				    gtk_icon_view_get_props (icon_view)->priv->scroll_to_row_align,
				    gtk_icon_view_get_props (icon_view)->priv->scroll_to_col_align);
      __gtk_tree_path_free (path);
    }
  else
    {
      __gtk_adjustment_changed (hadjustment);
      __gtk_adjustment_changed (vadjustment);
    }
}

static gboolean
gtk_icon_view_expose (GtkWidget *widget,
		      GdkEventExpose *expose)
{
  GtkIconView *icon_view;
  GList *icons;
  cairo_t *cr;
  GtkTreePath *path;
  gint dest_index;
  GtkIconViewDropPosition dest_pos;
  GtkIconViewItem *dest_item = NULL;

  icon_view = GTK_ICON_VIEW (widget);

  if (expose->window != gtk_icon_view_get_props (icon_view)->priv->bin_window)
    return FALSE;

  /* If a layout has been scheduled, do it now so that all
   * cell view items have valid sizes before we proceed. */
  if (gtk_icon_view_get_props (icon_view)->priv->layout_idle_id != 0)
    gtk_icon_view_layout (icon_view);

  cr = __gdk_cairo_create ((GdkDrawable *) (gtk_icon_view_get_props (icon_view)->priv->bin_window));
  cairo_set_line_width (cr, 1.);

  __gtk_icon_view_get_drag_dest_item (icon_view, &path, &dest_pos);

  if (path)
    {
      dest_index = __gtk_tree_path_get_indices (path)[0];
      __gtk_tree_path_free (path);
    }
  else
    dest_index = -1;

  for (icons = gtk_icon_view_get_props (icon_view)->priv->items; icons; icons = icons->next) 
    {
      GtkIconViewItem *item = icons->data;
      GdkRectangle area;
      
      area.x = item->x;
      area.y = item->y;
      area.width = item->width;
      area.height = item->height;
	
      if (__gdk_region_rect_in (expose->region, &area) == GDK_OVERLAP_RECTANGLE_OUT)
	continue;
      
      gtk_icon_view_paint_item (icon_view, cr, item, &expose->area, 
				gtk_icon_view_get_props (icon_view)->priv->bin_window,
				item->x, item->y,
				gtk_icon_view_get_props (icon_view)->priv->draw_focus); 
 
      if (dest_index == item->index)
	dest_item = item;
    }

  if (dest_item)
    {
      switch (dest_pos)
	{
	case GTK_ICON_VIEW_DROP_INTO:
	  __gtk_paint_focus (gtk_widget_get_props (widget)->style,
			   gtk_icon_view_get_props (icon_view)->priv->bin_window,
			   __gtk_widget_get_state (widget),
			   NULL,
			   widget,
			   "iconview-drop-indicator",
			   dest_item->x, dest_item->y,
			   dest_item->width, dest_item->height);
	  break;
	case GTK_ICON_VIEW_DROP_ABOVE:
	  __gtk_paint_focus (gtk_widget_get_props (widget)->style,
			   gtk_icon_view_get_props (icon_view)->priv->bin_window,
			   __gtk_widget_get_state (widget),
			   NULL,
			   widget,
			   "iconview-drop-indicator",
			   dest_item->x, dest_item->y - 1,
			   dest_item->width, 2);
	  break;
	case GTK_ICON_VIEW_DROP_LEFT:
	  __gtk_paint_focus (gtk_widget_get_props (widget)->style,
			   gtk_icon_view_get_props (icon_view)->priv->bin_window,
			   __gtk_widget_get_state (widget),
			   NULL,
			   widget,
			   "iconview-drop-indicator",
			   dest_item->x - 1, dest_item->y,
			   2, dest_item->height);
	  break;
	case GTK_ICON_VIEW_DROP_BELOW:
	  __gtk_paint_focus (gtk_widget_get_props (widget)->style,
			   gtk_icon_view_get_props (icon_view)->priv->bin_window,
			   __gtk_widget_get_state (widget),
			   NULL,
			   widget,
			   "iconview-drop-indicator",
			   dest_item->x, dest_item->y + dest_item->height - 1,
			   dest_item->width, 2);
	  break;
	case GTK_ICON_VIEW_DROP_RIGHT:
	  __gtk_paint_focus (gtk_widget_get_props (widget)->style,
			   gtk_icon_view_get_props (icon_view)->priv->bin_window,
			   __gtk_widget_get_state (widget),
			   NULL,
			   widget,
			   "iconview-drop-indicator",
			   dest_item->x + dest_item->width - 1, dest_item->y,
			   2, dest_item->height);
	case GTK_ICON_VIEW_NO_DROP: ;
	  break;
	}
    }
  
  if (gtk_icon_view_get_props (icon_view)->priv->doing_rubberband)
    {
      GdkRectangle *rectangles;
      gint n_rectangles;
      
      __gdk_region_get_rectangles (expose->region,
				 &rectangles,
				 &n_rectangles);
      
      while (n_rectangles--)
	gtk_icon_view_paint_rubberband (icon_view, cr, &rectangles[n_rectangles]);

      g_free (rectangles);
    }

  cairo_destroy (cr);

  GTK_WIDGET_CLASS (gtk_icon_view_parent_class)->expose_event (widget, expose);

  return TRUE;
}

static gboolean
rubberband_scroll_timeout (gpointer data)
{
  GtkIconView *icon_view;
  gdouble value;

  icon_view = data;

  value = MIN (gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->value +
	       gtk_icon_view_get_props (icon_view)->priv->scroll_value_diff,
	       gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->upper -
	       gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->page_size);

  __gtk_adjustment_set_value (gtk_icon_view_get_props (icon_view)->priv->vadjustment, value);

  gtk_icon_view_update_rubberband (icon_view);
  
  return TRUE;
}

static gboolean
gtk_icon_view_motion (GtkWidget      *widget,
		      GdkEventMotion *event)
{
  GtkIconView *icon_view;
  gint abs_y;
  
  icon_view = GTK_ICON_VIEW (widget);

  gtk_icon_view_maybe_begin_drag (icon_view, event);

  if (gtk_icon_view_get_props (icon_view)->priv->doing_rubberband)
    {
      gtk_icon_view_update_rubberband (widget);
      
      abs_y = event->y - gtk_icon_view_get_props (icon_view)->priv->height *
	(gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->value /
	 (gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->upper -
	  gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->lower));

      if (abs_y < 0 || abs_y > gtk_widget_get_props (widget)->allocation.height)
	{
	  if (abs_y < 0)
	    gtk_icon_view_get_props (icon_view)->priv->scroll_value_diff = abs_y;
	  else
	    gtk_icon_view_get_props (icon_view)->priv->scroll_value_diff = abs_y - gtk_widget_get_props (widget)->allocation.height;

	  gtk_icon_view_get_props (icon_view)->priv->event_last_x = event->x;
	  gtk_icon_view_get_props (icon_view)->priv->event_last_y = event->y;

	  if (gtk_icon_view_get_props (icon_view)->priv->scroll_timeout_id == 0)
	    gtk_icon_view_get_props (icon_view)->priv->scroll_timeout_id = __gdk_threads_add_timeout (30, rubberband_scroll_timeout, 
								icon_view);
 	}
      else 
	remove_scroll_timeout (icon_view);
    }
  
  return TRUE;
}

static void
gtk_icon_view_remove (GtkContainer *container,
		      GtkWidget    *widget)
{
  GtkIconView *icon_view;
  GtkIconViewChild *child = NULL;
  GList *tmp_list;

  icon_view = GTK_ICON_VIEW (container);
  
  tmp_list = gtk_icon_view_get_props (icon_view)->priv->children;
  while (tmp_list)
    {
      child = tmp_list->data;
      if (child->widget == widget)
	{
	  __gtk_widget_unparent (widget);

	  gtk_icon_view_get_props (icon_view)->priv->children = g_list_remove_link (gtk_icon_view_get_props (icon_view)->priv->children, tmp_list);
	  g_list_free_1 (tmp_list);
	  g_free (child);
	  return;
	}

      tmp_list = tmp_list->next;
    }
}

static void
gtk_icon_view_forall (GtkContainer *container,
		      gboolean      include_internals,
		      GtkCallback   callback,
		      gpointer      callback_data)
{
  GtkIconView *icon_view;
  GtkIconViewChild *child = NULL;
  GList *tmp_list;

  icon_view = GTK_ICON_VIEW (container);

  tmp_list = gtk_icon_view_get_props (icon_view)->priv->children;
  while (tmp_list)
    {
      child = tmp_list->data;
      tmp_list = tmp_list->next;

      (* callback) (child->widget, callback_data);
    }
}

static void
gtk_icon_view_item_activate_cell (GtkIconView         *icon_view, 
				  GtkIconViewItem     *item, 
				  GtkIconViewCellInfo *info,
				  GdkEvent            *event)
{
  GtkTreePath *path;  
  gchar *path_string;
  GdkRectangle cell_area;
  gboolean visible, mode;

  gtk_icon_view_set_cell_data (icon_view, item);

  g_object_get (info->cell,
		"visible", &visible,
		"mode", &mode,
		NULL);

  if (visible && mode == GTK_CELL_RENDERER_MODE_ACTIVATABLE)
    {
      gtk_icon_view_get_cell_area (icon_view, item, info, &cell_area);

      path = __gtk_tree_path_new_from_indices (item->index, -1);
      path_string = __gtk_tree_path_to_string (path);
      __gtk_tree_path_free (path);

      __gtk_cell_renderer_activate (info->cell, 
				  event, 
				  GTK_WIDGET (icon_view),
				  path_string, 
				  &cell_area, 
				  &cell_area, 
				  0);

      g_free (path_string);      
    }
}

static void 
gtk_icon_view_item_selected_changed (GtkIconView      *icon_view,
                                     GtkIconViewItem  *item)
{
  AtkObject *obj;
  AtkObject *item_obj;

  obj = __gtk_widget_get_accessible (GTK_WIDGET (icon_view));
  if (obj != NULL)
    {
      item_obj = atk_object_ref_accessible_child (obj, item->index);
      if (item_obj != NULL)
        {
          atk_object_notify_state_change (item_obj, ATK_STATE_SELECTED, item->selected);
          g_object_unref (item_obj);
        }
    }
}

static void 
gtk_icon_view_put (GtkIconView     *icon_view,
		   GtkWidget       *widget,
		   GtkIconViewItem *item,
		   gint             cell)
{
  GtkIconViewChild *child;
  
  child = g_new (GtkIconViewChild, 1);
  
  child->widget = widget;
  child->item = item;
  child->cell = cell;

  gtk_icon_view_get_props (icon_view)->priv->children = g_list_append (gtk_icon_view_get_props (icon_view)->priv->children, child);

  if (__gtk_widget_get_realized (GTK_WIDGET (icon_view)))
    __gtk_widget_set_parent_window (child->widget, gtk_icon_view_get_props (icon_view)->priv->bin_window);
  
  __gtk_widget_set_parent (widget, GTK_WIDGET (icon_view));
}

static void
gtk_icon_view_remove_widget (GtkCellEditable *editable,
			     GtkIconView     *icon_view)
{
  GList *l;
  GtkIconViewItem *item;

  if (gtk_icon_view_get_props (icon_view)->priv->edited_item == NULL)
    return;

  item = gtk_icon_view_get_props (icon_view)->priv->edited_item;
  gtk_icon_view_get_props (icon_view)->priv->edited_item = NULL;
  gtk_icon_view_get_props (icon_view)->priv->editable = NULL;
  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list; l; l = l->next)
    {
      GtkIconViewCellInfo *info = l->data;

      info->editing = FALSE;
    }

  if (__gtk_widget_has_focus (GTK_WIDGET (editable)))
    __gtk_widget_grab_focus (GTK_WIDGET (icon_view));
  
  g_signal_handlers_disconnect_by_func (editable,
					gtk_icon_view_remove_widget,
					icon_view);

  __gtk_container_remove (GTK_CONTAINER (icon_view),
			GTK_WIDGET (editable));  

  gtk_icon_view_queue_draw_item (icon_view, item);
}


static void
gtk_icon_view_start_editing (GtkIconView         *icon_view, 
			     GtkIconViewItem     *item, 
			     GtkIconViewCellInfo *info,
			     GdkEvent            *event)
{
  GtkTreePath *path;  
  gchar *path_string;
  GdkRectangle cell_area;
  gboolean visible, mode;
  GtkCellEditable *editable;

  gtk_icon_view_set_cell_data (icon_view, item);

  g_object_get (info->cell,
		"visible", &visible,
		"mode", &mode,
		NULL);
  if (visible && mode == GTK_CELL_RENDERER_MODE_EDITABLE)
    {
      gtk_icon_view_get_cell_area (icon_view, item, info, &cell_area);

      path = __gtk_tree_path_new_from_indices (item->index, -1);
      path_string = __gtk_tree_path_to_string (path);
      __gtk_tree_path_free (path);

      editable = __gtk_cell_renderer_start_editing (info->cell, 
						  event, 
						  GTK_WIDGET (icon_view),
						  path_string, 
						  &cell_area, 
						  &cell_area, 
						  0);
      g_free (path_string);      

      /* the rest corresponds to tree_view_real_start_editing... */
      gtk_icon_view_get_props (icon_view)->priv->edited_item = item;
      gtk_icon_view_get_props (icon_view)->priv->editable = editable;
      info->editing = TRUE;

      gtk_icon_view_put (icon_view, GTK_WIDGET (editable), item, 
			 info->position);
      __gtk_cell_editable_start_editing (GTK_CELL_EDITABLE (editable), 
				       (GdkEvent *)event);
      __gtk_widget_grab_focus (GTK_WIDGET (editable));
      g_signal_connect (editable, "remove-widget",
			G_CALLBACK (gtk_icon_view_remove_widget), 
			icon_view);

    }
}

static void
gtk_icon_view_stop_editing (GtkIconView *icon_view,
			    gboolean     cancel_editing)
{
  GtkCellRenderer *cell = NULL;
  GtkIconViewItem *item;
  GList *l;

  if (gtk_icon_view_get_props (icon_view)->priv->edited_item == NULL)
    return;

  /*
   * This is very evil. We need to do this, because
   * __gtk_cell_editable_editing_done may trigger gtk_icon_view_row_changed
   * later on. If gtk_icon_view_row_changed notices
   * gtk_icon_view_get_props (icon_view)->priv->edited_item != NULL, it'll call
   * gtk_icon_view_stop_editing again. Bad things will happen then.
   *
   * Please read that again if you intend to modify anything here.
   */

  item = gtk_icon_view_get_props (icon_view)->priv->edited_item;
  gtk_icon_view_get_props (icon_view)->priv->edited_item = NULL;

  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list; l; l = l->next)
    {
      GtkIconViewCellInfo *info = l->data;

      if (info->editing)
	{
	  cell = info->cell;
	  break;
	}
    }

  if (cell == NULL)
    return;

  __gtk_cell_renderer_stop_editing (cell, cancel_editing);
  if (!cancel_editing)
    __gtk_cell_editable_editing_done (gtk_icon_view_get_props (icon_view)->priv->editable);

  gtk_icon_view_get_props (icon_view)->priv->edited_item = item;

  __gtk_cell_editable_remove_widget (gtk_icon_view_get_props (icon_view)->priv->editable);
}

/**
 * __gtk_icon_view_set_cursor:
 * @icon_view: A #GtkIconView
 * @path: A #GtkTreePath
 * @cell: (allow-none): One of the cell renderers of @icon_view, or %NULL
 * @start_editing: %TRUE if the specified cell should start being edited.
 *
 * Sets the current keyboard focus to be at @path, and selects it.  This is
 * useful when you want to focus the user's attention on a particular item.
 * If @cell is not %NULL, then focus is given to the cell specified by 
 * it. Additionally, if @start_editing is %TRUE, then editing should be 
 * started in the specified cell.  
 *
 * This function is often followed by <literal>__gtk_widget_grab_focus 
 * (icon_view)</literal> in order to give keyboard focus to the widget.  
 * Please note that editing can only happen when the widget is realized.
 *
 * Since: 2.8
 **/
void
__gtk_icon_view_set_cursor (GtkIconView     *icon_view,
			  GtkTreePath     *path,
			  GtkCellRenderer *cell,
			  gboolean         start_editing)
{
  GtkIconViewItem *item = NULL;
  GtkIconViewCellInfo *info =  NULL;
  GList *l;
  gint i, cell_pos;

  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  g_return_if_fail (path != NULL);
  g_return_if_fail (cell == NULL || GTK_IS_CELL_RENDERER (cell));

  gtk_icon_view_stop_editing (icon_view, TRUE);

  if (__gtk_tree_path_get_depth (path) == 1)
    item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items,
			    __gtk_tree_path_get_indices(path)[0]);
  
  if (!item)
    return;

  cell_pos = -1;
  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list, i = 0; l; l = l->next, i++)
    {
      info = l->data;
      
      if (info->cell == cell)
	{
	  cell_pos = i;
	  break;
	}
	  
      info = NULL;
    }
  
  g_return_if_fail (cell == NULL || info != NULL);

  __gtk_icon_view_set_cursor_item (icon_view, item, cell_pos);
  __gtk_icon_view_scroll_to_path (icon_view, path, FALSE, 0.0, 0.0);
  
  if (info && start_editing)
    gtk_icon_view_start_editing (icon_view, item, info, NULL);
}

/**
 * __gtk_icon_view_get_cursor:
 * @icon_view: A #GtkIconView
 * @path: (allow-none): Return location for the current cursor path, or %NULL
 * @cell: (allow-none): Return location the current focus cell, or %NULL
 *
 * Fills in @path and @cell with the current cursor path and cell. 
 * If the cursor isn't currently set, then *@path will be %NULL.  
 * If no cell currently has focus, then *@cell will be %NULL.
 *
 * The returned #GtkTreePath must be freed with __gtk_tree_path_free().
 *
 * Return value: %TRUE if the cursor is set.
 *
 * Since: 2.8
 **/
gboolean
__gtk_icon_view_get_cursor (GtkIconView      *icon_view,
			  GtkTreePath     **path,
			  GtkCellRenderer **cell)
{
  GtkIconViewItem *item;
  GtkIconViewCellInfo *info;

  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), FALSE);

  item = gtk_icon_view_get_props (icon_view)->priv->cursor_item;
  if (gtk_icon_view_get_props (icon_view)->priv->cursor_cell < 0)
    info = NULL;
  else
    info = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->cell_list, 
			    gtk_icon_view_get_props (icon_view)->priv->cursor_cell);

  if (path != NULL)
    {
      if (item != NULL)
	*path = __gtk_tree_path_new_from_indices (item->index, -1);
      else
	*path = NULL;
    }

  if (cell != NULL)
    {
      if (info != NULL)
	*cell = info->cell;
      else 
	*cell = NULL;
    }

  return (item != NULL);
}

static gboolean
gtk_icon_view_button_press (GtkWidget      *widget,
			    GdkEventButton *event)
{
  GtkIconView *icon_view;
  GtkIconViewItem *item;
  GtkIconViewCellInfo *info = NULL;
  gboolean dirty = FALSE;
  GtkCellRendererMode mode;
  gint cursor_cell = -1;

  icon_view = GTK_ICON_VIEW (widget);

  if (event->window != gtk_icon_view_get_props (icon_view)->priv->bin_window)
    return FALSE;

  if (!__gtk_widget_has_focus (widget))
    __gtk_widget_grab_focus (widget);

  if (event->button == 1 && event->type == GDK_BUTTON_PRESS)
    {
      item = gtk_icon_view_get_item_at_coords (icon_view, 
					       event->x, event->y,
					       FALSE,
					       &info);    

      /*
       * We consider only the the cells' area as the item area if the
       * item is not selected, but if it *is* selected, the complete
       * selection rectangle is considered to be part of the item.
       */
      if (item != NULL && (info != NULL || item->selected))
	{
	  if (info != NULL)
	    {
	      g_object_get (info->cell, "mode", &mode, NULL);

	      if (mode == GTK_CELL_RENDERER_MODE_ACTIVATABLE ||
		  mode == GTK_CELL_RENDERER_MODE_EDITABLE)
		cursor_cell = g_list_index (gtk_icon_view_get_props (icon_view)->priv->cell_list, info);
	    }

	  gtk_icon_view_scroll_to_item (icon_view, item);
	  
	  if (gtk_icon_view_get_props (icon_view)->priv->selection_mode == GTK_SELECTION_NONE)
	    {
	      __gtk_icon_view_set_cursor_item (icon_view, item, cursor_cell);
	    }
	  else if (gtk_icon_view_get_props (icon_view)->priv->selection_mode == GTK_SELECTION_MULTIPLE &&
		   (event->state & GTK_EXTEND_SELECTION_MOD_MASK))
	    {
	      __gtk_icon_view_unselect_all_internal (icon_view);

	      __gtk_icon_view_set_cursor_item (icon_view, item, cursor_cell);
	      if (!gtk_icon_view_get_props (icon_view)->priv->anchor_item)
		gtk_icon_view_get_props (icon_view)->priv->anchor_item = item;
	      else 
		__gtk_icon_view_select_all_between (icon_view,
						  gtk_icon_view_get_props (icon_view)->priv->anchor_item,
						  item);
	      dirty = TRUE;
	    }
	  else 
	    {
	      if ((gtk_icon_view_get_props (icon_view)->priv->selection_mode == GTK_SELECTION_MULTIPLE ||
		  ((gtk_icon_view_get_props (icon_view)->priv->selection_mode == GTK_SELECTION_SINGLE) && item->selected)) &&
		  (event->state & GTK_MODIFY_SELECTION_MOD_MASK))
		{
		  item->selected = !item->selected;
		  gtk_icon_view_queue_draw_item (icon_view, item);
		  dirty = TRUE;
		}
	      else
		{
		  __gtk_icon_view_unselect_all_internal (icon_view);

		  item->selected = TRUE;
		  gtk_icon_view_queue_draw_item (icon_view, item);
		  dirty = TRUE;
		}
	      __gtk_icon_view_set_cursor_item (icon_view, item, cursor_cell);
	      gtk_icon_view_get_props (icon_view)->priv->anchor_item = item;
	    }

	  /* Save press to possibly begin a drag */
	  if (gtk_icon_view_get_props (icon_view)->priv->pressed_button < 0)
	    {
	      gtk_icon_view_get_props (icon_view)->priv->pressed_button = event->button;
	      gtk_icon_view_get_props (icon_view)->priv->press_start_x = event->x;
	      gtk_icon_view_get_props (icon_view)->priv->press_start_y = event->y;
	    }

	  if (!gtk_icon_view_get_props (icon_view)->priv->last_single_clicked)
	    gtk_icon_view_get_props (icon_view)->priv->last_single_clicked = item;

	  /* cancel the current editing, if it exists */
	  gtk_icon_view_stop_editing (icon_view, TRUE);

	  if (info != NULL)
	    {
	      if (mode == GTK_CELL_RENDERER_MODE_ACTIVATABLE)
		gtk_icon_view_item_activate_cell (icon_view, item, info, 
					          (GdkEvent *)event);
	      else if (mode == GTK_CELL_RENDERER_MODE_EDITABLE)
		gtk_icon_view_start_editing (icon_view, item, info, 
					     (GdkEvent *)event);
	    }
	}
      else
	{
	  if (gtk_icon_view_get_props (icon_view)->priv->selection_mode != GTK_SELECTION_BROWSE &&
	      !(event->state & GTK_MODIFY_SELECTION_MOD_MASK))
	    {
	      dirty = __gtk_icon_view_unselect_all_internal (icon_view);
	    }
	  
	  if (gtk_icon_view_get_props (icon_view)->priv->selection_mode == GTK_SELECTION_MULTIPLE)
	    gtk_icon_view_start_rubberbanding (icon_view, event->x, event->y);
	}

      /* don't draw keyboard focus around an clicked-on item */
      gtk_icon_view_get_props (icon_view)->priv->draw_focus = FALSE;
    }

  if (event->button == 1 && event->type == GDK_2BUTTON_PRESS)
    {
      item = gtk_icon_view_get_item_at_coords (icon_view,
					       event->x, event->y,
					       FALSE,
					       NULL);

      if (item && item == gtk_icon_view_get_props (icon_view)->priv->last_single_clicked)
	{
	  GtkTreePath *path;

	  path = __gtk_tree_path_new_from_indices (item->index, -1);
	  __gtk_icon_view_item_activated (icon_view, path);
	  __gtk_tree_path_free (path);
	}

      gtk_icon_view_get_props (icon_view)->priv->last_single_clicked = NULL;
      gtk_icon_view_get_props (icon_view)->priv->pressed_button = -1;
    }
  
  if (dirty)
    g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0);

  return event->button == 1;
}

static gboolean
gtk_icon_view_button_release (GtkWidget      *widget,
			      GdkEventButton *event)
{
  GtkIconView *icon_view;

  icon_view = GTK_ICON_VIEW (widget);
  
  if (gtk_icon_view_get_props (icon_view)->priv->pressed_button == event->button)
    gtk_icon_view_get_props (icon_view)->priv->pressed_button = -1;

  gtk_icon_view_stop_rubberbanding (icon_view);

  remove_scroll_timeout (icon_view);

  return TRUE;
}

static gboolean
gtk_icon_view_key_press (GtkWidget      *widget,
			 GdkEventKey    *event)
{
  GtkIconView *icon_view = GTK_ICON_VIEW (widget);

  if (gtk_icon_view_get_props (icon_view)->priv->doing_rubberband)
    {
      if (event->keyval == GDK_Escape)
	gtk_icon_view_stop_rubberbanding (icon_view);

      return TRUE;
    }

  return GTK_WIDGET_CLASS (gtk_icon_view_parent_class)->key_press_event (widget, event);
}

static gboolean
gtk_icon_view_key_release (GtkWidget      *widget,
			   GdkEventKey    *event)
{
  GtkIconView *icon_view = GTK_ICON_VIEW (widget);

  if (gtk_icon_view_get_props (icon_view)->priv->doing_rubberband)
    return TRUE;

  return GTK_WIDGET_CLASS (gtk_icon_view_parent_class)->key_press_event (widget, event);
}

static void
gtk_icon_view_update_rubberband (gpointer data)
{
  GtkIconView *icon_view;
  gint x, y;
  GdkRectangle old_area;
  GdkRectangle new_area;
  GdkRectangle common;
  GdkRegion *invalid_region;
  
  icon_view = GTK_ICON_VIEW (data);

  __gdk_window_get_pointer (gtk_icon_view_get_props (icon_view)->priv->bin_window, &x, &y, NULL);

  x = MAX (x, 0);
  y = MAX (y, 0);

  old_area.x = MIN (gtk_icon_view_get_props (icon_view)->priv->rubberband_x1,
		    gtk_icon_view_get_props (icon_view)->priv->rubberband_x2);
  old_area.y = MIN (gtk_icon_view_get_props (icon_view)->priv->rubberband_y1,
		    gtk_icon_view_get_props (icon_view)->priv->rubberband_y2);
  old_area.width = ABS (gtk_icon_view_get_props (icon_view)->priv->rubberband_x2 -
			gtk_icon_view_get_props (icon_view)->priv->rubberband_x1) + 1;
  old_area.height = ABS (gtk_icon_view_get_props (icon_view)->priv->rubberband_y2 -
			 gtk_icon_view_get_props (icon_view)->priv->rubberband_y1) + 1;
  
  new_area.x = MIN (gtk_icon_view_get_props (icon_view)->priv->rubberband_x1, x);
  new_area.y = MIN (gtk_icon_view_get_props (icon_view)->priv->rubberband_y1, y);
  new_area.width = ABS (x - gtk_icon_view_get_props (icon_view)->priv->rubberband_x1) + 1;
  new_area.height = ABS (y - gtk_icon_view_get_props (icon_view)->priv->rubberband_y1) + 1;

  invalid_region = __gdk_region_rectangle (&old_area);
  __gdk_region_union_with_rect (invalid_region, &new_area);

  __gdk_rectangle_intersect (&old_area, &new_area, &common);
  if (common.width > 2 && common.height > 2)
    {
      GdkRegion *common_region;

      /* make sure the border is invalidated */
      common.x += 1;
      common.y += 1;
      common.width -= 2;
      common.height -= 2;
      
      common_region = __gdk_region_rectangle (&common);

      __gdk_region_subtract (invalid_region, common_region);
      __gdk_region_destroy (common_region);
    }
  
  __gdk_window_invalidate_region (gtk_icon_view_get_props (icon_view)->priv->bin_window, invalid_region, TRUE);
    
  __gdk_region_destroy (invalid_region);

  gtk_icon_view_get_props (icon_view)->priv->rubberband_x2 = x;
  gtk_icon_view_get_props (icon_view)->priv->rubberband_y2 = y;  

  gtk_icon_view_update_rubberband_selection (icon_view);
}

static void
gtk_icon_view_start_rubberbanding (GtkIconView  *icon_view,
				   gint          x,
				   gint          y)
{
  GList *items;

  g_assert (!gtk_icon_view_get_props (icon_view)->priv->doing_rubberband);

  for (items = gtk_icon_view_get_props (icon_view)->priv->items; items; items = items->next)
    {
      GtkIconViewItem *item = items->data;

      item->selected_before_rubberbanding = item->selected;
    }
  
  gtk_icon_view_get_props (icon_view)->priv->rubberband_x1 = x;
  gtk_icon_view_get_props (icon_view)->priv->rubberband_y1 = y;
  gtk_icon_view_get_props (icon_view)->priv->rubberband_x2 = x;
  gtk_icon_view_get_props (icon_view)->priv->rubberband_y2 = y;

  gtk_icon_view_get_props (icon_view)->priv->doing_rubberband = TRUE;

  __gtk_grab_add (GTK_WIDGET (icon_view));
}

static void
gtk_icon_view_stop_rubberbanding (GtkIconView *icon_view)
{
  if (!gtk_icon_view_get_props (icon_view)->priv->doing_rubberband)
    return;

  gtk_icon_view_get_props (icon_view)->priv->doing_rubberband = FALSE;

  __gtk_grab_remove (GTK_WIDGET (icon_view));
  
  __gtk_widget_queue_draw (GTK_WIDGET (icon_view));
}

static void
gtk_icon_view_update_rubberband_selection (GtkIconView *icon_view)
{
  GList *items;
  gint x, y, width, height;
  gboolean dirty = FALSE;
  
  x = MIN (gtk_icon_view_get_props (icon_view)->priv->rubberband_x1,
	   gtk_icon_view_get_props (icon_view)->priv->rubberband_x2);
  y = MIN (gtk_icon_view_get_props (icon_view)->priv->rubberband_y1,
	   gtk_icon_view_get_props (icon_view)->priv->rubberband_y2);
  width = ABS (gtk_icon_view_get_props (icon_view)->priv->rubberband_x1 - 
	       gtk_icon_view_get_props (icon_view)->priv->rubberband_x2);
  height = ABS (gtk_icon_view_get_props (icon_view)->priv->rubberband_y1 - 
		gtk_icon_view_get_props (icon_view)->priv->rubberband_y2);
  
  for (items = gtk_icon_view_get_props (icon_view)->priv->items; items; items = items->next)
    {
      GtkIconViewItem *item = items->data;
      gboolean is_in;
      gboolean selected;
      
      is_in = gtk_icon_view_item_hit_test (icon_view, item, 
					   x, y, width, height);

      selected = is_in ^ item->selected_before_rubberbanding;

      if (item->selected != selected)
	{
	  item->selected = selected;
	  dirty = TRUE;
	  gtk_icon_view_queue_draw_item (icon_view, item);
	}
    }

  if (dirty)
    g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0);
}

static gboolean
gtk_icon_view_item_hit_test (GtkIconView      *icon_view,
			     GtkIconViewItem  *item,
			     gint              x,
			     gint              y,
			     gint              width,
			     gint              height)
{
  GList *l;
  GdkRectangle box;
 
  if (MIN (x + width, item->x + item->width) - MAX (x, item->x) <= 0 ||
      MIN (y + height, item->y + item->height) - MAX (y, item->y) <= 0)
    return FALSE;

  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list; l; l = l->next)
    {
      GtkIconViewCellInfo *info = (GtkIconViewCellInfo *)l->data;
      
      if (!gtk_cell_renderer_get_props (info->cell)->visible)
	continue;
      
      gtk_icon_view_get_cell_box (icon_view, item, info, &box);

      if (MIN (x + width, box.x + box.width) - MAX (x, box.x) > 0 &&
	MIN (y + height, box.y + box.height) - MAX (y, box.y) > 0)
	return TRUE;
    }

  return FALSE;
}

static gboolean
__gtk_icon_view_unselect_all_internal (GtkIconView  *icon_view)
{
  gboolean dirty = FALSE;
  GList *items;

  if (gtk_icon_view_get_props (icon_view)->priv->selection_mode == GTK_SELECTION_NONE)
    return FALSE;

  for (items = gtk_icon_view_get_props (icon_view)->priv->items; items; items = items->next)
    {
      GtkIconViewItem *item = items->data;

      if (item->selected)
	{
	  item->selected = FALSE;
	  dirty = TRUE;
	  gtk_icon_view_queue_draw_item (icon_view, item);
	  gtk_icon_view_item_selected_changed (icon_view, item);
	}
    }

  return dirty;
}


/* GtkIconView signals */
static void
gtk_icon_view_set_adjustments (GtkIconView   *icon_view,
			       GtkAdjustment *hadj,
			       GtkAdjustment *vadj)
{
  gboolean need_adjust = FALSE;

  if (hadj)
    g_return_if_fail (GTK_IS_ADJUSTMENT (hadj));
  else
    hadj = GTK_ADJUSTMENT (__gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
  if (vadj)
    g_return_if_fail (GTK_IS_ADJUSTMENT (vadj));
  else
    vadj = GTK_ADJUSTMENT (__gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0));

  if (gtk_icon_view_get_props (icon_view)->priv->hadjustment && (gtk_icon_view_get_props (icon_view)->priv->hadjustment != hadj))
    {
      g_signal_handlers_disconnect_matched (gtk_icon_view_get_props (icon_view)->priv->hadjustment, G_SIGNAL_MATCH_DATA,
					   0, 0, NULL, NULL, icon_view);
      g_object_unref (gtk_icon_view_get_props (icon_view)->priv->hadjustment);
    }

  if (gtk_icon_view_get_props (icon_view)->priv->vadjustment && (gtk_icon_view_get_props (icon_view)->priv->vadjustment != vadj))
    {
      g_signal_handlers_disconnect_matched (gtk_icon_view_get_props (icon_view)->priv->vadjustment, G_SIGNAL_MATCH_DATA,
					    0, 0, NULL, NULL, icon_view);
      g_object_unref (gtk_icon_view_get_props (icon_view)->priv->vadjustment);
    }

  if (gtk_icon_view_get_props (icon_view)->priv->hadjustment != hadj)
    {
      gtk_icon_view_get_props (icon_view)->priv->hadjustment = hadj;
      g_object_ref_sink (gtk_icon_view_get_props (icon_view)->priv->hadjustment);

      g_signal_connect (gtk_icon_view_get_props (icon_view)->priv->hadjustment, "value-changed",
			G_CALLBACK (gtk_icon_view_adjustment_changed),
			icon_view);
      need_adjust = TRUE;
    }

  if (gtk_icon_view_get_props (icon_view)->priv->vadjustment != vadj)
    {
      gtk_icon_view_get_props (icon_view)->priv->vadjustment = vadj;
      g_object_ref_sink (gtk_icon_view_get_props (icon_view)->priv->vadjustment);

      g_signal_connect (gtk_icon_view_get_props (icon_view)->priv->vadjustment, "value-changed",
			G_CALLBACK (gtk_icon_view_adjustment_changed),
			icon_view);
      need_adjust = TRUE;
    }

  if (need_adjust)
    gtk_icon_view_adjustment_changed (NULL, icon_view);
}

static void
gtk_icon_view_real_select_all (GtkIconView *icon_view)
{
  __gtk_icon_view_select_all (icon_view);
}

static void
gtk_icon_view_real_unselect_all (GtkIconView *icon_view)
{
  __gtk_icon_view_unselect_all (icon_view);
}

static void
gtk_icon_view_real_select_cursor_item (GtkIconView *icon_view)
{
  __gtk_icon_view_unselect_all (icon_view);
  
  if (gtk_icon_view_get_props (icon_view)->priv->cursor_item != NULL)
    gtk_icon_view_select_item (icon_view, gtk_icon_view_get_props (icon_view)->priv->cursor_item);
}

static gboolean
gtk_icon_view_real_activate_cursor_item (GtkIconView *icon_view)
{
  GtkTreePath *path;
  GtkCellRendererMode mode;
  GtkIconViewCellInfo *info = NULL;
  
  if (!gtk_icon_view_get_props (icon_view)->priv->cursor_item)
    return FALSE;

  info = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->cell_list, 
			  gtk_icon_view_get_props (icon_view)->priv->cursor_cell);

  if (info) 
    {  
      g_object_get (info->cell, "mode", &mode, NULL);

      if (mode == GTK_CELL_RENDERER_MODE_ACTIVATABLE)
	{
	  gtk_icon_view_item_activate_cell (icon_view, 
					    gtk_icon_view_get_props (icon_view)->priv->cursor_item, 
					    info, NULL);
	  return TRUE;
	}
      else if (mode == GTK_CELL_RENDERER_MODE_EDITABLE)
	{
	  gtk_icon_view_start_editing (icon_view, 
				       gtk_icon_view_get_props (icon_view)->priv->cursor_item, 
				       info, NULL);
	  return TRUE;
	}
    }
  
  path = __gtk_tree_path_new_from_indices (gtk_icon_view_get_props (icon_view)->priv->cursor_item->index, -1);
  __gtk_icon_view_item_activated (icon_view, path);
  __gtk_tree_path_free (path);

  return TRUE;
}

static void
gtk_icon_view_real_toggle_cursor_item (GtkIconView *icon_view)
{
  if (!gtk_icon_view_get_props (icon_view)->priv->cursor_item)
    return;

  switch (gtk_icon_view_get_props (icon_view)->priv->selection_mode)
    {
    case GTK_SELECTION_NONE:
      break;
    case GTK_SELECTION_BROWSE:
      gtk_icon_view_select_item (icon_view, gtk_icon_view_get_props (icon_view)->priv->cursor_item);
      break;
    case GTK_SELECTION_SINGLE:
      if (gtk_icon_view_get_props (icon_view)->priv->cursor_item->selected)
	gtk_icon_view_unselect_item (icon_view, gtk_icon_view_get_props (icon_view)->priv->cursor_item);
      else
	gtk_icon_view_select_item (icon_view, gtk_icon_view_get_props (icon_view)->priv->cursor_item);
      break;
    case GTK_SELECTION_MULTIPLE:
      gtk_icon_view_get_props (icon_view)->priv->cursor_item->selected = !gtk_icon_view_get_props (icon_view)->priv->cursor_item->selected;
      g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0); 
      
      gtk_icon_view_item_selected_changed (icon_view, gtk_icon_view_get_props (icon_view)->priv->cursor_item);      
      gtk_icon_view_queue_draw_item (icon_view, gtk_icon_view_get_props (icon_view)->priv->cursor_item);
      break;
    }
}

/* Internal functions */
static void
gtk_icon_view_adjustment_changed (GtkAdjustment *adjustment,
				  GtkIconView   *icon_view)
{
  if (__gtk_widget_get_realized (GTK_WIDGET (icon_view)))
    {
      __gdk_window_move (gtk_icon_view_get_props (icon_view)->priv->bin_window,
		       - gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->hadjustment)->value,
		       - gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->value);

      if (gtk_icon_view_get_props (icon_view)->priv->doing_rubberband)
	gtk_icon_view_update_rubberband (GTK_WIDGET (icon_view));

      __gdk_window_process_updates (gtk_icon_view_get_props (icon_view)->priv->bin_window, TRUE);
    }
}

static GList *
gtk_icon_view_layout_single_row (GtkIconView *icon_view, 
				 GList       *first_item, 
				 gint         item_width,
				 gint         row,
				 gint        *y, 
				 gint        *maximum_width)
{
  gint focus_width;
  gint x, current_width;
  GList *items, *last_item;
  gint col;
  gint colspan;
  gint *max_height;
  gint i;
  gboolean rtl;

  rtl = __gtk_widget_get_direction (GTK_WIDGET (icon_view)) == GTK_TEXT_DIR_RTL;
  max_height = g_new0 (gint, gtk_icon_view_get_props (icon_view)->priv->n_cells);

  x = 0;
  col = 0;
  items = first_item;
  current_width = 0;

  __gtk_widget_style_get (GTK_WIDGET (icon_view),
			"focus-line-width", &focus_width,
			NULL);

  x += gtk_icon_view_get_props (icon_view)->priv->margin + focus_width;
  current_width += 2 * (gtk_icon_view_get_props (icon_view)->priv->margin + focus_width);

  items = first_item;
  while (items)
    {
      GtkIconViewItem *item = items->data;

      gtk_icon_view_calculate_item_size (icon_view, item);
      colspan = 1 + (item->width - 1) / (item_width + gtk_icon_view_get_props (icon_view)->priv->column_spacing);

      item->width = colspan * item_width + (colspan - 1) * gtk_icon_view_get_props (icon_view)->priv->column_spacing;

      current_width += item->width;

      if (items != first_item)
	{
	  if ((gtk_icon_view_get_props (icon_view)->priv->columns <= 0 && current_width > gtk_widget_get_props (GTK_WIDGET (icon_view))->allocation.width) ||
	      (gtk_icon_view_get_props (icon_view)->priv->columns > 0 && col >= gtk_icon_view_get_props (icon_view)->priv->columns))
	    break;
	}

      current_width += gtk_icon_view_get_props (icon_view)->priv->column_spacing + 2 * focus_width;

      item->y = *y + focus_width;
      item->x = x;

      x = current_width - (gtk_icon_view_get_props (icon_view)->priv->margin + focus_width); 

      for (i = 0; i < gtk_icon_view_get_props (icon_view)->priv->n_cells; i++)
	max_height[i] = MAX (max_height[i], item->box[i].height);
	      
      if (current_width > *maximum_width)
	*maximum_width = current_width;

      item->row = row;
      item->col = col;

      col += colspan;
      items = items->next;
    }

  last_item = items;

  /* Now go through the row again and align the icons */
  for (items = first_item; items != last_item; items = items->next)
    {
      GtkIconViewItem *item = items->data;

      if (rtl)
	{
	  item->x = *maximum_width - item->width - item->x;
	  item->col = col - 1 - item->col;
	}

      gtk_icon_view_calculate_item_size2 (icon_view, item, max_height);

      /* We may want to readjust the new y coordinate. */
      if (item->y + item->height + focus_width + gtk_icon_view_get_props (icon_view)->priv->row_spacing > *y)
	*y = item->y + item->height + focus_width + gtk_icon_view_get_props (icon_view)->priv->row_spacing;
    }

  g_free (max_height);
  
  return last_item;
}

static void
gtk_icon_view_set_adjustment_upper (GtkAdjustment *adj,
				    gdouble        upper)
{
  if (upper != gtk_adjustment_get_props (adj)->upper)
    {
      gdouble min = MAX (0.0, upper - gtk_adjustment_get_props (adj)->page_size);
      gboolean value_changed = FALSE;
      
      gtk_adjustment_get_props (adj)->upper = upper;

      if (gtk_adjustment_get_props (adj)->value > min)
	{
	  gtk_adjustment_get_props (adj)->value = min;
	  value_changed = TRUE;
	}
      
      __gtk_adjustment_changed (adj);
      
      if (value_changed)
	__gtk_adjustment_value_changed (adj);
    }
}

static void
gtk_icon_view_layout (GtkIconView *icon_view)
{
  gint y = 0, maximum_width = 0;
  GList *icons;
  GtkWidget *widget;
  gint row;
  gint item_width;

  if (gtk_icon_view_get_props (icon_view)->priv->layout_idle_id != 0)
    {
      g_source_remove (gtk_icon_view_get_props (icon_view)->priv->layout_idle_id);
      gtk_icon_view_get_props (icon_view)->priv->layout_idle_id = 0;
    }
  
  if (gtk_icon_view_get_props (icon_view)->priv->model == NULL)
    return;

  widget = GTK_WIDGET (icon_view);

  item_width = gtk_icon_view_get_props (icon_view)->priv->item_width;

  if (item_width < 0)
    {
      for (icons = gtk_icon_view_get_props (icon_view)->priv->items; icons; icons = icons->next)
	{
	  GtkIconViewItem *item = icons->data;
	  gtk_icon_view_calculate_item_size (icon_view, item);
	  item_width = MAX (item_width, item->width);
	}
    }


  icons = gtk_icon_view_get_props (icon_view)->priv->items;
  y += gtk_icon_view_get_props (icon_view)->priv->margin;
  row = 0;

  if (icons)
    {
      gtk_icon_view_set_cell_data (icon_view, icons->data);
      adjust_wrap_width (icon_view, icons->data);
    }
  
  do
    {
      icons = gtk_icon_view_layout_single_row (icon_view, icons, 
					       item_width, row,
					       &y, &maximum_width);
      row++;
    }
  while (icons != NULL);

  if (maximum_width != gtk_icon_view_get_props (icon_view)->priv->width)
    gtk_icon_view_get_props (icon_view)->priv->width = maximum_width;

  y += gtk_icon_view_get_props (icon_view)->priv->margin;
  
  if (y != gtk_icon_view_get_props (icon_view)->priv->height)
    gtk_icon_view_get_props (icon_view)->priv->height = y;

  gtk_icon_view_set_adjustment_upper (gtk_icon_view_get_props (icon_view)->priv->hadjustment, 
				      gtk_icon_view_get_props (icon_view)->priv->width);
  gtk_icon_view_set_adjustment_upper (gtk_icon_view_get_props (icon_view)->priv->vadjustment, 
				      gtk_icon_view_get_props (icon_view)->priv->height);

  if (gtk_icon_view_get_props (icon_view)->priv->width != gtk_widget_get_props (widget)->requisition.width ||
      gtk_icon_view_get_props (icon_view)->priv->height != gtk_widget_get_props (widget)->requisition.height)
    __gtk_widget_queue_resize_no_redraw (widget);

  if (__gtk_widget_get_realized (GTK_WIDGET (icon_view)))
    __gdk_window_resize (gtk_icon_view_get_props (icon_view)->priv->bin_window,
		       MAX (gtk_icon_view_get_props (icon_view)->priv->width, gtk_widget_get_props (widget)->allocation.width),
		       MAX (gtk_icon_view_get_props (icon_view)->priv->height, gtk_widget_get_props (widget)->allocation.height));

  if (gtk_icon_view_get_props (icon_view)->priv->scroll_to_path)
    {
      GtkTreePath *path;

      path = __gtk_tree_row_reference_get_path (gtk_icon_view_get_props (icon_view)->priv->scroll_to_path);
      __gtk_tree_row_reference_free (gtk_icon_view_get_props (icon_view)->priv->scroll_to_path);
      gtk_icon_view_get_props (icon_view)->priv->scroll_to_path = NULL;
      
      __gtk_icon_view_scroll_to_path (icon_view, path,
				    gtk_icon_view_get_props (icon_view)->priv->scroll_to_use_align,
				    gtk_icon_view_get_props (icon_view)->priv->scroll_to_row_align,
				    gtk_icon_view_get_props (icon_view)->priv->scroll_to_col_align);
      __gtk_tree_path_free (path);
    }
  
  __gtk_widget_queue_draw (widget);
}

static void 
gtk_icon_view_get_cell_area (GtkIconView         *icon_view,
			     GtkIconViewItem     *item,
			     GtkIconViewCellInfo *info,
			     GdkRectangle        *cell_area)
{
  g_return_if_fail (info->position < item->n_cells);

  if (gtk_icon_view_get_props (icon_view)->priv->item_orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      cell_area->x = item->box[info->position].x - item->before[info->position];
      cell_area->y = item->y + gtk_icon_view_get_props (icon_view)->priv->item_padding;
      cell_area->width = item->box[info->position].width + 
	item->before[info->position] + item->after[info->position];
      cell_area->height = item->height - gtk_icon_view_get_props (icon_view)->priv->item_padding * 2;
    }
  else
    {
      cell_area->x = item->x + gtk_icon_view_get_props (icon_view)->priv->item_padding;
      cell_area->y = item->box[info->position].y - item->before[info->position];
      cell_area->width = item->width - gtk_icon_view_get_props (icon_view)->priv->item_padding * 2;
      cell_area->height = item->box[info->position].height + 
	item->before[info->position] + item->after[info->position];
    }
}

static void 
gtk_icon_view_get_cell_box (GtkIconView         *icon_view,
			    GtkIconViewItem     *item,
			    GtkIconViewCellInfo *info,
			    GdkRectangle        *box)
{
  g_return_if_fail (info->position < item->n_cells);

  *box = item->box[info->position];
}

/* try to guess a reasonable wrap width for an implicit text cell renderer
 */
static void
adjust_wrap_width (GtkIconView     *icon_view,
		   GtkIconViewItem *item)
{
  GtkIconViewCellInfo *text_info;
  GtkIconViewCellInfo *pixbuf_info;
  gint pixbuf_width, wrap_width;
      
  if (gtk_icon_view_get_props (icon_view)->priv->text_cell != -1 &&
      gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell != -1)
    {
      gint item_width;

      text_info = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->cell_list,
				   gtk_icon_view_get_props (icon_view)->priv->text_cell);
      pixbuf_info = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->cell_list,
				     gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell);
      
      __gtk_cell_renderer_get_size (pixbuf_info->cell, 
				  GTK_WIDGET (icon_view), 
				  NULL, NULL, NULL,
				  &pixbuf_width, 
				  NULL);
	  

      if (gtk_icon_view_get_props (icon_view)->priv->item_width > 0)
	item_width = gtk_icon_view_get_props (icon_view)->priv->item_width;
      else
	item_width = item->width;

      if (gtk_icon_view_get_props (icon_view)->priv->item_orientation == GTK_ORIENTATION_VERTICAL)
        wrap_width = item_width;
      else {
        if (item->width == -1 && item_width <= 0)
          wrap_width = MAX (2 * pixbuf_width, 50);
        else
          wrap_width = item_width - pixbuf_width - gtk_icon_view_get_props (icon_view)->priv->spacing;
        }

      wrap_width -= gtk_icon_view_get_props (icon_view)->priv->item_padding * 2;

      g_object_set (text_info->cell, "wrap-width", wrap_width, NULL);
      g_object_set (text_info->cell, "width", wrap_width, NULL);
    }
}

static void
gtk_icon_view_calculate_item_size (GtkIconView     *icon_view,
				   GtkIconViewItem *item)
{
  gint spacing;
  GList *l;

  if (item->width != -1 && item->height != -1) 
    return;

  if (item->n_cells != gtk_icon_view_get_props (icon_view)->priv->n_cells)
    {
      g_free (item->before);
      g_free (item->after);
      g_free (item->box);
      
      item->before = g_new0 (gint, gtk_icon_view_get_props (icon_view)->priv->n_cells);
      item->after = g_new0 (gint, gtk_icon_view_get_props (icon_view)->priv->n_cells);
      item->box = g_new0 (GdkRectangle, gtk_icon_view_get_props (icon_view)->priv->n_cells);

      item->n_cells = gtk_icon_view_get_props (icon_view)->priv->n_cells;
    }

  gtk_icon_view_set_cell_data (icon_view, item);

  spacing = gtk_icon_view_get_props (icon_view)->priv->spacing;

  item->width = 0;
  item->height = 0;
  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list; l; l = l->next)
    {
      GtkIconViewCellInfo *info = (GtkIconViewCellInfo *)l->data;
      
      if (!gtk_cell_renderer_get_props (info->cell)->visible)
	continue;
      
      __gtk_cell_renderer_get_size (info->cell, GTK_WIDGET (icon_view), 
				  NULL, NULL, NULL,
				  &item->box[info->position].width, 
				  &item->box[info->position].height);

      if (gtk_icon_view_get_props (icon_view)->priv->item_orientation == GTK_ORIENTATION_HORIZONTAL)
	{
	  item->width += item->box[info->position].width 
	    + (info->position > 0 ? spacing : 0);
	  item->height = MAX (item->height, item->box[info->position].height);
	}
      else
	{
	  item->width = MAX (item->width, item->box[info->position].width);
	  item->height += item->box[info->position].height + (info->position > 0 ? spacing : 0);
	}
    }

  item->width += gtk_icon_view_get_props (icon_view)->priv->item_padding * 2;
  item->height += gtk_icon_view_get_props (icon_view)->priv->item_padding * 2;
}

static void
gtk_icon_view_calculate_item_size2 (GtkIconView     *icon_view,
				    GtkIconViewItem *item,
				    gint            *max_height)
{
  GdkRectangle cell_area;
  gint spacing;
  GList *l;
  gint i, k;
  gboolean rtl;

  rtl = __gtk_widget_get_direction (GTK_WIDGET (icon_view)) == GTK_TEXT_DIR_RTL;

  gtk_icon_view_set_cell_data (icon_view, item);

  spacing = gtk_icon_view_get_props (icon_view)->priv->spacing;

  item->height = 0;
  for (i = 0; i < gtk_icon_view_get_props (icon_view)->priv->n_cells; i++)
    {
      if (gtk_icon_view_get_props (icon_view)->priv->item_orientation == GTK_ORIENTATION_HORIZONTAL)
	item->height = MAX (item->height, max_height[i]);
      else
	item->height += max_height[i] + (i > 0 ? spacing : 0);
    }

  cell_area.x = item->x + gtk_icon_view_get_props (icon_view)->priv->item_padding;
  cell_area.y = item->y + gtk_icon_view_get_props (icon_view)->priv->item_padding;
      
  for (k = 0; k < 2; k++)
    for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list, i = 0; l; l = l->next, i++)
      {
	GtkIconViewCellInfo *info = (GtkIconViewCellInfo *)l->data;
	
	if (info->pack == (k ? GTK_PACK_START : GTK_PACK_END))
	  continue;

	if (!gtk_cell_renderer_get_props (info->cell)->visible)
	  continue;

	if (gtk_icon_view_get_props (icon_view)->priv->item_orientation == GTK_ORIENTATION_HORIZONTAL)
	  {
            /* We should not subtract gtk_icon_view_get_props (icon_view)->priv->item_padding from item->height,
             * because item->height is recalculated above using
             * max_height which does not contain item padding.
             */
	    cell_area.width = item->box[info->position].width;
	    cell_area.height = item->height;
	  }
	else
	  {
            /* item->width is not recalculated and thus needs to be
             * corrected for the padding.
             */
	    cell_area.width = item->width - 2 * gtk_icon_view_get_props (icon_view)->priv->item_padding;
	    cell_area.height = max_height[i];
	  }
	
	__gtk_cell_renderer_get_size (info->cell, GTK_WIDGET (icon_view), 
				    &cell_area,
				    &item->box[info->position].x, &item->box[info->position].y,
				    &item->box[info->position].width, &item->box[info->position].height);
	
	item->box[info->position].x += cell_area.x;
	item->box[info->position].y += cell_area.y;
	if (gtk_icon_view_get_props (icon_view)->priv->item_orientation == GTK_ORIENTATION_HORIZONTAL)
	  {
	    item->before[info->position] = item->box[info->position].x - cell_area.x;
	    item->after[info->position] = cell_area.width - item->box[info->position].width - item->before[info->position];
	    cell_area.x += cell_area.width + spacing;
	  }
	else
	  {
	    if (item->box[info->position].width > item->width - gtk_icon_view_get_props (icon_view)->priv->item_padding * 2)
	      {
		item->width = item->box[info->position].width + gtk_icon_view_get_props (icon_view)->priv->item_padding * 2;
		cell_area.width = item->width;
	      }
	    item->before[info->position] = item->box[info->position].y - cell_area.y;
	    item->after[info->position] = cell_area.height - item->box[info->position].height - item->before[info->position];
	    cell_area.y += cell_area.height + spacing;
	  }
      }
  
  if (rtl && gtk_icon_view_get_props (icon_view)->priv->item_orientation == GTK_ORIENTATION_HORIZONTAL)
    {
      for (i = 0; i < gtk_icon_view_get_props (icon_view)->priv->n_cells; i++)
	{
	  item->box[i].x = item->x + item->width - 
	    (item->box[i].x + item->box[i].width - item->x);
	}      
    }

  item->height += gtk_icon_view_get_props (icon_view)->priv->item_padding * 2;
}

static void
gtk_icon_view_invalidate_sizes (GtkIconView *icon_view)
{
  g_list_foreach (gtk_icon_view_get_props (icon_view)->priv->items,
		  (GFunc)gtk_icon_view_item_invalidate_size, NULL);
}

static void
gtk_icon_view_item_invalidate_size (GtkIconViewItem *item)
{
  item->width = -1;
  item->height = -1;
}

static void
gtk_icon_view_paint_item (GtkIconView     *icon_view,
			  cairo_t         *cr,
			  GtkIconViewItem *item,
			  GdkRectangle    *area,
			  GdkDrawable     *drawable,
			  gint             x,
			  gint             y,
			  gboolean         draw_focus)
{
  gint focus_width;
  gint padding;
  GdkRectangle cell_area, box;
  GList *l;
  gint i;
  GtkCellRendererState flags;
      
  if (gtk_icon_view_get_props (icon_view)->priv->model == NULL)
    return;
  
  gtk_icon_view_set_cell_data (icon_view, item);

  __gtk_widget_style_get (GTK_WIDGET (icon_view),
			"focus-line-width", &focus_width,
			NULL);
  
  padding = focus_width; 
  
  if (item->selected)
    {
      flags = GTK_CELL_RENDERER_SELECTED;
    }
  else
    {
      flags = 0;
    }
  
#ifdef DEBUG_ICON_VIEW
  __gdk_draw_rectangle (drawable,
		      gtk_widget_get_props (GTK_WIDGET (icon_view))->style->black_gc,
		      FALSE,
		      x, y, 
		      item->width, item->height);
#endif

  if (item->selected)
    {
      __gtk_paint_flat_box (gtk_widget_get_props (GTK_WIDGET (icon_view))->style,
			  (GdkWindow *) drawable,
			  GTK_STATE_SELECTED,
			  GTK_SHADOW_NONE,
			  area,
			  GTK_WIDGET (icon_view),
			  "icon_view_item",
			  x, y,
			  item->width, item->height);
    }
  
  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list; l; l = l->next)
    {
      GtkIconViewCellInfo *info = (GtkIconViewCellInfo *)l->data;
      
      if (!gtk_cell_renderer_get_props (info->cell)->visible)
	continue;
      
      gtk_icon_view_get_cell_area (icon_view, item, info, &cell_area);
      
#ifdef DEBUG_ICON_VIEW
      __gdk_draw_rectangle (drawable,
			  gtk_widget_get_props (GTK_WIDGET (icon_view))->style->black_gc,
			  FALSE,
			  x - item->x + cell_area.x, 
			  y - item->y + cell_area.y, 
			  cell_area.width, cell_area.height);

      gtk_icon_view_get_cell_box (icon_view, item, info, &box);
	  
      __gdk_draw_rectangle (drawable,
			  gtk_widget_get_props (GTK_WIDGET (icon_view))->style->black_gc,
			  FALSE,
			  x - item->x + box.x, 
			  y - item->y + box.y, 
			  box.width, box.height);
#endif

      cell_area.x = x - item->x + cell_area.x;
      cell_area.y = y - item->y + cell_area.y;

      __gtk_cell_renderer_render (info->cell,
				drawable,
				GTK_WIDGET (icon_view),
				&cell_area, &cell_area, area, flags);
    }

  if (draw_focus &&
      __gtk_widget_has_focus (GTK_WIDGET (icon_view)) &&
      item == gtk_icon_view_get_props (icon_view)->priv->cursor_item)
    {
      for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list, i = 0; l; l = l->next, i++)
        {
          GtkIconViewCellInfo *info = (GtkIconViewCellInfo *)l->data;

          if (!gtk_cell_renderer_get_props (info->cell)->visible)
            continue;

          /* If found a editable/activatable cell, draw focus on it. */
          if (gtk_icon_view_get_props (icon_view)->priv->cursor_cell < 0 &&
              gtk_cell_renderer_get_props (info->cell)->mode != GTK_CELL_RENDERER_MODE_INERT)
            gtk_icon_view_get_props (icon_view)->priv->cursor_cell = i;

          gtk_icon_view_get_cell_box (icon_view, item, info, &box);

          if (i == gtk_icon_view_get_props (icon_view)->priv->cursor_cell)
            {
              __gtk_paint_focus (gtk_widget_get_props (GTK_WIDGET (icon_view))->style,
                               drawable,
                               GTK_STATE_NORMAL,
                               area,
                               GTK_WIDGET (icon_view),
                               "icon_view",
                               x - item->x + box.x - padding,
                               y - item->y + box.y - padding,
                               box.width + 2 * padding,
                               box.height + 2 * padding);
              break;
            }
        }

      /* If there are no editable/activatable cells, draw focus 
       * around the whole item.
       */
      if (gtk_icon_view_get_props (icon_view)->priv->cursor_cell < 0)
        __gtk_paint_focus (gtk_widget_get_props (GTK_WIDGET (icon_view))->style,
                         drawable,
                         GTK_STATE_NORMAL,
                         area,
                         GTK_WIDGET (icon_view),
                         "icon_view",
                         x - padding,
                         y - padding,
                         item->width + 2 * padding,
                         item->height + 2 * padding);
    }
}

static void
gtk_icon_view_paint_rubberband (GtkIconView     *icon_view,
				cairo_t         *cr,
				GdkRectangle    *area)
{
  GdkRectangle rect;
  GdkRectangle rubber_rect;
  GdkColor *fill_color_gdk;
  guchar fill_color_alpha;

  rubber_rect.x = MIN (gtk_icon_view_get_props (icon_view)->priv->rubberband_x1, gtk_icon_view_get_props (icon_view)->priv->rubberband_x2);
  rubber_rect.y = MIN (gtk_icon_view_get_props (icon_view)->priv->rubberband_y1, gtk_icon_view_get_props (icon_view)->priv->rubberband_y2);
  rubber_rect.width = ABS (gtk_icon_view_get_props (icon_view)->priv->rubberband_x1 - gtk_icon_view_get_props (icon_view)->priv->rubberband_x2) + 1;
  rubber_rect.height = ABS (gtk_icon_view_get_props (icon_view)->priv->rubberband_y1 - gtk_icon_view_get_props (icon_view)->priv->rubberband_y2) + 1;

  if (!__gdk_rectangle_intersect (&rubber_rect, area, &rect))
    return;

  __gtk_widget_style_get (GTK_WIDGET (icon_view),
                        "selection-box-color", &fill_color_gdk,
                        "selection-box-alpha", &fill_color_alpha,
                        NULL);

  if (!fill_color_gdk)
    fill_color_gdk = __gdk_color_copy (&gtk_widget_get_props (GTK_WIDGET (icon_view))->style->base[GTK_STATE_SELECTED]);

  cairo_set_source_rgba (cr,
			 fill_color_gdk->red / 65535.,
			 fill_color_gdk->green / 65535.,
			 fill_color_gdk->blue / 65535.,
			 fill_color_alpha / 255.);

  cairo_save (cr);
  __gdk_cairo_rectangle (cr, &rect);
  cairo_clip (cr);
  cairo_paint (cr);

  /* Draw the border without alpha */
  cairo_set_source_rgb (cr,
			fill_color_gdk->red / 65535.,
			fill_color_gdk->green / 65535.,
			fill_color_gdk->blue / 65535.);
  cairo_rectangle (cr, 
		   rubber_rect.x + 0.5, rubber_rect.y + 0.5,
		   rubber_rect.width - 1, rubber_rect.height - 1);
  cairo_stroke (cr);
  cairo_restore (cr);

  __gdk_color_free (fill_color_gdk);
}

static void
gtk_icon_view_queue_draw_path (GtkIconView *icon_view,
			       GtkTreePath *path)
{
  GList *l;
  gint index;

  index = __gtk_tree_path_get_indices (path)[0];

  for (l = gtk_icon_view_get_props (icon_view)->priv->items; l; l = l->next) 
    {
      GtkIconViewItem *item = l->data;

      if (item->index == index)
	{
	  gtk_icon_view_queue_draw_item (icon_view, item);
	  break;
	}
    }
}

static void
gtk_icon_view_queue_draw_item (GtkIconView     *icon_view,
			       GtkIconViewItem *item)
{
  gint focus_width;
  GdkRectangle rect;

  __gtk_widget_style_get (GTK_WIDGET (icon_view),
			"focus-line-width", &focus_width,
			NULL);

  rect.x = item->x - focus_width;
  rect.y = item->y - focus_width;
  rect.width = item->width + 2 * focus_width;
  rect.height = item->height + 2 * focus_width;

  if (gtk_icon_view_get_props (icon_view)->priv->bin_window)
    __gdk_window_invalidate_rect (gtk_icon_view_get_props (icon_view)->priv->bin_window, &rect, TRUE);
}

static gboolean
layout_callback (gpointer user_data)
{
  GtkIconView *icon_view;

  icon_view = GTK_ICON_VIEW (user_data);
  
  gtk_icon_view_get_props (icon_view)->priv->layout_idle_id = 0;

  gtk_icon_view_layout (icon_view);
  
  return FALSE;
}

static void
gtk_icon_view_queue_layout (GtkIconView *icon_view)
{
  if (gtk_icon_view_get_props (icon_view)->priv->layout_idle_id != 0)
    return;

  gtk_icon_view_get_props (icon_view)->priv->layout_idle_id = __gdk_threads_add_idle (layout_callback, gtk_icon_view_get_props (icon_view));
}

static void
__gtk_icon_view_set_cursor_item (GtkIconView     *icon_view,
			       GtkIconViewItem *item,
			       gint             cursor_cell)
{
  AtkObject *obj;
  AtkObject *item_obj;
  AtkObject *cursor_item_obj;

  if (gtk_icon_view_get_props (icon_view)->priv->cursor_item == item &&
      (cursor_cell < 0 || cursor_cell == gtk_icon_view_get_props (icon_view)->priv->cursor_cell))
    return;

  obj = __gtk_widget_get_accessible (GTK_WIDGET (icon_view));
  if (gtk_icon_view_get_props (icon_view)->priv->cursor_item != NULL)
    {
      gtk_icon_view_queue_draw_item (icon_view, gtk_icon_view_get_props (icon_view)->priv->cursor_item);
      if (obj != NULL)
        {
          cursor_item_obj = atk_object_ref_accessible_child (obj, gtk_icon_view_get_props (icon_view)->priv->cursor_item->index);
          if (cursor_item_obj != NULL)
            atk_object_notify_state_change (cursor_item_obj, ATK_STATE_FOCUSED, FALSE);
        }
    }
  gtk_icon_view_get_props (icon_view)->priv->cursor_item = item;
  if (cursor_cell >= 0)
    gtk_icon_view_get_props (icon_view)->priv->cursor_cell = cursor_cell;

  gtk_icon_view_queue_draw_item (icon_view, item);
  
  /* Notify that accessible focus object has changed */
  item_obj = atk_object_ref_accessible_child (obj, item->index);

  if (item_obj != NULL)
    {
      atk_focus_tracker_notify (item_obj);
      atk_object_notify_state_change (item_obj, ATK_STATE_FOCUSED, TRUE);
      g_object_unref (item_obj); 
    }
}


static GtkIconViewItem *
gtk_icon_view_item_new (void)
{
  GtkIconViewItem *item;

  item = g_new0 (GtkIconViewItem, 1);

  item->width = -1;
  item->height = -1;
  
  return item;
}

static void
gtk_icon_view_item_free (GtkIconViewItem *item)
{
  g_return_if_fail (item != NULL);

  g_free (item->before);
  g_free (item->after);
  g_free (item->box);

  g_free (item);
}


static GtkIconViewItem *
gtk_icon_view_get_item_at_coords (GtkIconView          *icon_view,
				  gint                  x,
				  gint                  y,
				  gboolean              only_in_cell,
				  GtkIconViewCellInfo **cell_at_pos)
{
  GList *items, *l;
  GdkRectangle box;

  if (cell_at_pos)
    *cell_at_pos = NULL;

  for (items = gtk_icon_view_get_props (icon_view)->priv->items; items; items = items->next)
    {
      GtkIconViewItem *item = items->data;

      if (x >= item->x - gtk_icon_view_get_props (icon_view)->priv->column_spacing/2 && x <= item->x + item->width + gtk_icon_view_get_props (icon_view)->priv->column_spacing/2 &&
	  y >= item->y - gtk_icon_view_get_props (icon_view)->priv->row_spacing/2 && y <= item->y + item->height + gtk_icon_view_get_props (icon_view)->priv->row_spacing/2)
	{
	  if (only_in_cell || cell_at_pos)
	    {
	      gtk_icon_view_set_cell_data (icon_view, item);

	      for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list; l; l = l->next)
		{
		  GtkIconViewCellInfo *info = (GtkIconViewCellInfo *)l->data;

		  if (!gtk_cell_renderer_get_props (info->cell)->visible)
		    continue;

		  gtk_icon_view_get_cell_box (icon_view, item, info, &box);

		  if ((x >= box.x && x <= box.x + box.width &&
		       y >= box.y && y <= box.y + box.height) ||
		      (x >= box.x  &&
		       x <= box.x + box.width &&
		       y >= box.y &&
		       y <= box.y + box.height))
		    {
		      if (cell_at_pos)
			*cell_at_pos = info;

		      return item;
		    }
		}

	      if (only_in_cell)
		return NULL;
	    }

	  return item;
	}
    }

  return NULL;
}

static void
gtk_icon_view_select_item (GtkIconView      *icon_view,
			   GtkIconViewItem  *item)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  g_return_if_fail (item != NULL);

  if (item->selected)
    return;
  
  if (gtk_icon_view_get_props (icon_view)->priv->selection_mode == GTK_SELECTION_NONE)
    return;
  else if (gtk_icon_view_get_props (icon_view)->priv->selection_mode != GTK_SELECTION_MULTIPLE)
    __gtk_icon_view_unselect_all_internal (icon_view);

  item->selected = TRUE;

  gtk_icon_view_item_selected_changed (icon_view, item);
  g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0);

  gtk_icon_view_queue_draw_item (icon_view, item);
}


static void
gtk_icon_view_unselect_item (GtkIconView      *icon_view,
			     GtkIconViewItem  *item)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  g_return_if_fail (item != NULL);

  if (!item->selected)
    return;
  
  if (gtk_icon_view_get_props (icon_view)->priv->selection_mode == GTK_SELECTION_NONE ||
      gtk_icon_view_get_props (icon_view)->priv->selection_mode == GTK_SELECTION_BROWSE)
    return;
  
  item->selected = FALSE;

  gtk_icon_view_item_selected_changed (icon_view, item);
  g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0);

  gtk_icon_view_queue_draw_item (icon_view, item);
}

static void
verify_items (GtkIconView *icon_view)
{
  GList *items;
  int i = 0;

  for (items = gtk_icon_view_get_props (icon_view)->priv->items; items; items = items->next)
    {
      GtkIconViewItem *item = items->data;

      if (item->index != i)
	g_error ("List item does not match its index: "
		 "item index %d and list index %d\n", item->index, i);

      i++;
    }
}

static void
gtk_icon_view_row_changed (GtkTreeModel *model,
			   GtkTreePath  *path,
			   GtkTreeIter  *iter,
			   gpointer      data)
{
  GtkIconViewItem *item;
  gint index;
  GtkIconView *icon_view;

  icon_view = GTK_ICON_VIEW (data);

  gtk_icon_view_stop_editing (icon_view, TRUE);
  
  index = __gtk_tree_path_get_indices(path)[0];
  item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items, index);

  gtk_icon_view_item_invalidate_size (item);
  gtk_icon_view_queue_layout (icon_view);

  verify_items (icon_view);
}

static void
gtk_icon_view_row_inserted (GtkTreeModel *model,
			    GtkTreePath  *path,
			    GtkTreeIter  *iter,
			    gpointer      data)
{
  gint index;
  GtkIconViewItem *item;
  gboolean iters_persist;
  GtkIconView *icon_view;
  GList *list;
  
  icon_view = GTK_ICON_VIEW (data);

  iters_persist = __gtk_tree_model_get_flags (gtk_icon_view_get_props (icon_view)->priv->model) & GTK_TREE_MODEL_ITERS_PERSIST;
  
  index = __gtk_tree_path_get_indices(path)[0];

  item = gtk_icon_view_item_new ();

  if (iters_persist)
    item->iter = *iter;

  item->index = index;

  /* FIXME: We can be more efficient here,
     we can store a tail pointer and use that when
     appending (which is a rather common operation)
  */
  gtk_icon_view_get_props (icon_view)->priv->items = g_list_insert (gtk_icon_view_get_props (icon_view)->priv->items,
					 item, index);
  
  list = g_list_nth (gtk_icon_view_get_props (icon_view)->priv->items, index + 1);
  for (; list; list = list->next)
    {
      item = list->data;

      item->index++;
    }
    
  verify_items (icon_view);

  gtk_icon_view_queue_layout (icon_view);
}

static void
gtk_icon_view_row_deleted (GtkTreeModel *model,
			   GtkTreePath  *path,
			   gpointer      data)
{
  gint index;
  GtkIconView *icon_view;
  GtkIconViewItem *item;
  GList *list, *next;
  gboolean emit = FALSE;
  
  icon_view = GTK_ICON_VIEW (data);

  index = __gtk_tree_path_get_indices(path)[0];

  list = g_list_nth (gtk_icon_view_get_props (icon_view)->priv->items, index);
  item = list->data;

  gtk_icon_view_stop_editing (icon_view, TRUE);

  if (item == gtk_icon_view_get_props (icon_view)->priv->anchor_item)
    gtk_icon_view_get_props (icon_view)->priv->anchor_item = NULL;

  if (item == gtk_icon_view_get_props (icon_view)->priv->cursor_item)
    gtk_icon_view_get_props (icon_view)->priv->cursor_item = NULL;

  if (item->selected)
    emit = TRUE;
  
  gtk_icon_view_item_free (item);

  for (next = list->next; next; next = next->next)
    {
      item = next->data;

      item->index--;
    }
  
  gtk_icon_view_get_props (icon_view)->priv->items = g_list_delete_link (gtk_icon_view_get_props (icon_view)->priv->items, list);

  verify_items (icon_view);  
  
  gtk_icon_view_queue_layout (icon_view);

  if (emit)
    g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0);
}

static void
gtk_icon_view_rows_reordered (GtkTreeModel *model,
			      GtkTreePath  *parent,
			      GtkTreeIter  *iter,
			      gint         *new_order,
			      gpointer      data)
{
  int i;
  int length;
  GtkIconView *icon_view;
  GList *items = NULL, *list;
  GtkIconViewItem **item_array;
  gint *order;
  
  icon_view = GTK_ICON_VIEW (data);

  gtk_icon_view_stop_editing (icon_view, TRUE);

  length = __gtk_tree_model_iter_n_children (model, NULL);

  order = g_new (gint, length);
  for (i = 0; i < length; i++)
    order [new_order[i]] = i;

  item_array = g_new (GtkIconViewItem *, length);
  for (i = 0, list = gtk_icon_view_get_props (icon_view)->priv->items; list != NULL; list = list->next, i++)
    item_array[order[i]] = list->data;
  g_free (order);

  for (i = length - 1; i >= 0; i--)
    {
      item_array[i]->index = i;
      items = g_list_prepend (items, item_array[i]);
    }
  
  g_free (item_array);
  g_list_free (gtk_icon_view_get_props (icon_view)->priv->items);
  gtk_icon_view_get_props (icon_view)->priv->items = items;

  gtk_icon_view_queue_layout (icon_view);

  verify_items (icon_view);  
}

static void
gtk_icon_view_build_items (GtkIconView *icon_view)
{
  GtkTreeIter iter;
  int i;
  gboolean iters_persist;
  GList *items = NULL;

  iters_persist = __gtk_tree_model_get_flags (gtk_icon_view_get_props (icon_view)->priv->model) & GTK_TREE_MODEL_ITERS_PERSIST;
  
  if (!__gtk_tree_model_get_iter_first (gtk_icon_view_get_props (icon_view)->priv->model,
				      &iter))
    return;

  i = 0;
  
  do
    {
      GtkIconViewItem *item = gtk_icon_view_item_new ();

      if (iters_persist)
	item->iter = iter;

      item->index = i;
      
      i++;

      items = g_list_prepend (items, item);
      
    } while (__gtk_tree_model_iter_next (gtk_icon_view_get_props (icon_view)->priv->model, &iter));

  gtk_icon_view_get_props (icon_view)->priv->items = g_list_reverse (items);
}

static void
gtk_icon_view_add_move_binding (GtkBindingSet  *binding_set,
				guint           keyval,
				guint           modmask,
				GtkMovementStep step,
				gint            count)
{
  
  __gtk_binding_entry_add_signal (binding_set, keyval, modmask,
                                I_("move-cursor"), 2,
                                G_TYPE_ENUM, step,
                                G_TYPE_INT, count);

  __gtk_binding_entry_add_signal (binding_set, keyval, GDK_SHIFT_MASK,
                                "move-cursor", 2,
                                G_TYPE_ENUM, step,
                                G_TYPE_INT, count);

  if ((modmask & GDK_CONTROL_MASK) == GDK_CONTROL_MASK)
   return;

  __gtk_binding_entry_add_signal (binding_set, keyval, GDK_CONTROL_MASK | GDK_SHIFT_MASK,
                                "move-cursor", 2,
                                G_TYPE_ENUM, step,
                                G_TYPE_INT, count);

  __gtk_binding_entry_add_signal (binding_set, keyval, GDK_CONTROL_MASK,
                                "move-cursor", 2,
                                G_TYPE_ENUM, step,
                                G_TYPE_INT, count);
}

static gboolean
gtk_icon_view_real_move_cursor (GtkIconView     *icon_view,
				GtkMovementStep  step,
				gint             count)
{
  GdkModifierType state;

  g_return_val_if_fail (GTK_ICON_VIEW (icon_view), FALSE);
  g_return_val_if_fail (step == GTK_MOVEMENT_LOGICAL_POSITIONS ||
			step == GTK_MOVEMENT_VISUAL_POSITIONS ||
			step == GTK_MOVEMENT_DISPLAY_LINES ||
			step == GTK_MOVEMENT_PAGES ||
			step == GTK_MOVEMENT_BUFFER_ENDS, FALSE);

  if (!__gtk_widget_has_focus (GTK_WIDGET (icon_view)))
    return FALSE;

  gtk_icon_view_stop_editing (icon_view, FALSE);
  __gtk_widget_grab_focus (GTK_WIDGET (icon_view));

  if (__gtk_get_current_event_state (&state))
    {
      if ((state & GTK_MODIFY_SELECTION_MOD_MASK) == GTK_MODIFY_SELECTION_MOD_MASK)
        gtk_icon_view_get_props (icon_view)->priv->modify_selection_pressed = TRUE;
      if ((state & GTK_EXTEND_SELECTION_MOD_MASK) == GTK_EXTEND_SELECTION_MOD_MASK)
        gtk_icon_view_get_props (icon_view)->priv->extend_selection_pressed = TRUE;
    }
  /* else we assume not pressed */

  switch (step)
    {
    case GTK_MOVEMENT_LOGICAL_POSITIONS:
    case GTK_MOVEMENT_VISUAL_POSITIONS:
      gtk_icon_view_move_cursor_left_right (icon_view, count);
      break;
    case GTK_MOVEMENT_DISPLAY_LINES:
      gtk_icon_view_move_cursor_up_down (icon_view, count);
      break;
    case GTK_MOVEMENT_PAGES:
      gtk_icon_view_move_cursor_page_up_down (icon_view, count);
      break;
    case GTK_MOVEMENT_BUFFER_ENDS:
      gtk_icon_view_move_cursor_start_end (icon_view, count);
      break;
    default:
      g_assert_not_reached ();
    }

  gtk_icon_view_get_props (icon_view)->priv->modify_selection_pressed = FALSE;
  gtk_icon_view_get_props (icon_view)->priv->extend_selection_pressed = FALSE;

  gtk_icon_view_get_props (icon_view)->priv->draw_focus = TRUE;

  return TRUE;
}

static GtkIconViewItem *
find_item (GtkIconView     *icon_view,
	   GtkIconViewItem *current,
	   gint             row_ofs,
	   gint             col_ofs)
{
  gint row, col;
  GList *items;
  GtkIconViewItem *item;

  /* FIXME: this could be more efficient 
   */
  row = current->row + row_ofs;
  col = current->col + col_ofs;

  for (items = gtk_icon_view_get_props (icon_view)->priv->items; items; items = items->next)
    {
      item = items->data;
      if (item->row == row && item->col == col)
	return item;
    }
  
  return NULL;
}

static gint
find_cell (GtkIconView     *icon_view,
	   GtkIconViewItem *item,
	   gint             cell,
	   GtkOrientation   orientation,
	   gint             step,
	   gint            *count)
{
  gint n_focusable;
  gint *focusable;
  gint current;
  gint i, k;
  GList *l;

  if (gtk_icon_view_get_props (icon_view)->priv->item_orientation != orientation)
    return cell;

  gtk_icon_view_set_cell_data (icon_view, item);

  focusable = g_new0 (gint, gtk_icon_view_get_props (icon_view)->priv->n_cells);
  n_focusable = 0;

  current = 0;
  for (k = 0; k < 2; k++)
    for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list, i = 0; l; l = l->next, i++)
      {
	GtkIconViewCellInfo *info = (GtkIconViewCellInfo *)l->data;
	
	if (info->pack == (k ? GTK_PACK_START : GTK_PACK_END))
	  continue;
	
	if (!gtk_cell_renderer_get_props (info->cell)->visible)
	  continue;

	if (gtk_cell_renderer_get_props (info->cell)->mode != GTK_CELL_RENDERER_MODE_INERT)
	  {
	    if (cell == i)
	      current = n_focusable;

	    focusable[n_focusable] = i;

	    n_focusable++;
	  }
      }
  
  if (n_focusable == 0)
    {
      g_free (focusable);
      return -1;
    }

  if (cell < 0)
    {
      current = step > 0 ? 0 : n_focusable - 1;
      cell = focusable[current];
    }

  if (current + *count < 0)
    {
      cell = -1;
      *count = current + *count;
    }
  else if (current + *count > n_focusable - 1)
    {
      cell = -1;
      *count = current + *count - (n_focusable - 1);
    }
  else
    {
      cell = focusable[current + *count];
      *count = 0;
    }
  
  g_free (focusable);
  
  return cell;
}

static GtkIconViewItem *
find_item_page_up_down (GtkIconView     *icon_view,
			GtkIconViewItem *current,
			gint             count)
{
  GList *item, *next;
  gint y, col;
  
  col = current->col;
  y = current->y + count * gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->page_size;

  item = g_list_find (gtk_icon_view_get_props (icon_view)->priv->items, current);
  if (count > 0)
    {
      while (item)
	{
	  for (next = item->next; next; next = next->next)
	    {
	      if (((GtkIconViewItem *)next->data)->col == col)
		break;
	    }
	  if (!next || ((GtkIconViewItem *)next->data)->y > y)
	    break;

	  item = next;
	}
    }
  else 
    {
      while (item)
	{
	  for (next = item->prev; next; next = next->prev)
	    {
	      if (((GtkIconViewItem *)next->data)->col == col)
		break;
	    }
	  if (!next || ((GtkIconViewItem *)next->data)->y < y)
	    break;

	  item = next;
	}
    }

  if (item)
    return item->data;

  return NULL;
}

static gboolean
__gtk_icon_view_select_all_between (GtkIconView     *icon_view,
				  GtkIconViewItem *anchor,
				  GtkIconViewItem *cursor)
{
  GList *items;
  GtkIconViewItem *item;
  gint row1, row2, col1, col2;
  gboolean dirty = FALSE;
  
  if (anchor->row < cursor->row)
    {
      row1 = anchor->row;
      row2 = cursor->row;
    }
  else
    {
      row1 = cursor->row;
      row2 = anchor->row;
    }

  if (anchor->col < cursor->col)
    {
      col1 = anchor->col;
      col2 = cursor->col;
    }
  else
    {
      col1 = cursor->col;
      col2 = anchor->col;
    }

  for (items = gtk_icon_view_get_props (icon_view)->priv->items; items; items = items->next)
    {
      item = items->data;

      if (row1 <= item->row && item->row <= row2 &&
	  col1 <= item->col && item->col <= col2)
	{
	  if (!item->selected)
	    {
	      dirty = TRUE;
	      item->selected = TRUE;
	      gtk_icon_view_item_selected_changed (icon_view, item);
	    }
	  gtk_icon_view_queue_draw_item (icon_view, item);
	}
    }

  return dirty;
}

static void 
gtk_icon_view_move_cursor_up_down (GtkIconView *icon_view,
				   gint         count)
{
  GtkIconViewItem *item;
  gint cell;
  gboolean dirty = FALSE;
  gint step;
  GtkDirectionType direction;
  
  if (!__gtk_widget_has_focus (GTK_WIDGET (icon_view)))
    return;

  direction = count < 0 ? GTK_DIR_UP : GTK_DIR_DOWN;

  if (!gtk_icon_view_get_props (icon_view)->priv->cursor_item)
    {
      GList *list;

      if (count > 0)
	list = gtk_icon_view_get_props (icon_view)->priv->items;
      else
	list = g_list_last (gtk_icon_view_get_props (icon_view)->priv->items);

      item = list ? list->data : NULL;
      cell = -1;
    }
  else
    {
      item = gtk_icon_view_get_props (icon_view)->priv->cursor_item;
      cell = gtk_icon_view_get_props (icon_view)->priv->cursor_cell;
      step = count > 0 ? 1 : -1;      
      while (item)
	{
	  cell = find_cell (icon_view, item, cell,
			    GTK_ORIENTATION_VERTICAL, 
			    step, &count);
	  if (count == 0)
	    break;

	  item = find_item (icon_view, item, step, 0);
	  count = count - step;
	}
    }

  if (!item)
    {
      if (!__gtk_widget_keynav_failed (GTK_WIDGET (icon_view), direction))
        {
          GtkWidget *toplevel = __gtk_widget_get_toplevel (GTK_WIDGET (icon_view));
          if (toplevel)
            __gtk_widget_child_focus (toplevel,
                                    direction == GTK_DIR_UP ?
                                    GTK_DIR_TAB_BACKWARD :
                                    GTK_DIR_TAB_FORWARD);
        }

      return;
    }

  if (gtk_icon_view_get_props (icon_view)->priv->modify_selection_pressed ||
      !gtk_icon_view_get_props (icon_view)->priv->extend_selection_pressed ||
      !gtk_icon_view_get_props (icon_view)->priv->anchor_item ||
      gtk_icon_view_get_props (icon_view)->priv->selection_mode != GTK_SELECTION_MULTIPLE)
    gtk_icon_view_get_props (icon_view)->priv->anchor_item = item;

  __gtk_icon_view_set_cursor_item (icon_view, item, cell);

  if (!gtk_icon_view_get_props (icon_view)->priv->modify_selection_pressed &&
      gtk_icon_view_get_props (icon_view)->priv->selection_mode != GTK_SELECTION_NONE)
    {
      dirty = __gtk_icon_view_unselect_all_internal (icon_view);
      dirty = __gtk_icon_view_select_all_between (icon_view, 
						gtk_icon_view_get_props (icon_view)->priv->anchor_item,
						item) || dirty;
    }

  gtk_icon_view_scroll_to_item (icon_view, item);

  if (dirty)
    g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0);
}

static void 
gtk_icon_view_move_cursor_page_up_down (GtkIconView *icon_view,
					gint         count)
{
  GtkIconViewItem *item;
  gboolean dirty = FALSE;
  
  if (!__gtk_widget_has_focus (GTK_WIDGET (icon_view)))
    return;
  
  if (!gtk_icon_view_get_props (icon_view)->priv->cursor_item)
    {
      GList *list;

      if (count > 0)
	list = gtk_icon_view_get_props (icon_view)->priv->items;
      else
	list = g_list_last (gtk_icon_view_get_props (icon_view)->priv->items);

      item = list ? list->data : NULL;
    }
  else
    item = find_item_page_up_down (icon_view, 
				   gtk_icon_view_get_props (icon_view)->priv->cursor_item,
				   count);

  if (item == gtk_icon_view_get_props (icon_view)->priv->cursor_item)
    __gtk_widget_error_bell (GTK_WIDGET (icon_view));

  if (!item)
    return;

  if (gtk_icon_view_get_props (icon_view)->priv->modify_selection_pressed ||
      !gtk_icon_view_get_props (icon_view)->priv->extend_selection_pressed ||
      !gtk_icon_view_get_props (icon_view)->priv->anchor_item ||
      gtk_icon_view_get_props (icon_view)->priv->selection_mode != GTK_SELECTION_MULTIPLE)
    gtk_icon_view_get_props (icon_view)->priv->anchor_item = item;

  __gtk_icon_view_set_cursor_item (icon_view, item, -1);

  if (!gtk_icon_view_get_props (icon_view)->priv->modify_selection_pressed &&
      gtk_icon_view_get_props (icon_view)->priv->selection_mode != GTK_SELECTION_NONE)
    {
      dirty = __gtk_icon_view_unselect_all_internal (icon_view);
      dirty = __gtk_icon_view_select_all_between (icon_view, 
						gtk_icon_view_get_props (icon_view)->priv->anchor_item,
						item) || dirty;
    }

  gtk_icon_view_scroll_to_item (icon_view, item);

  if (dirty)
    g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0);  
}

static void 
gtk_icon_view_move_cursor_left_right (GtkIconView *icon_view,
				      gint         count)
{
  GtkIconViewItem *item;
  gint cell = -1;
  gboolean dirty = FALSE;
  gint step;
  GtkDirectionType direction;

  if (!__gtk_widget_has_focus (GTK_WIDGET (icon_view)))
    return;

  direction = count < 0 ? GTK_DIR_LEFT : GTK_DIR_RIGHT;

  if (!gtk_icon_view_get_props (icon_view)->priv->cursor_item)
    {
      GList *list;

      if (count > 0)
	list = gtk_icon_view_get_props (icon_view)->priv->items;
      else
	list = g_list_last (gtk_icon_view_get_props (icon_view)->priv->items);

      item = list ? list->data : NULL;
    }
  else
    {
      item = gtk_icon_view_get_props (icon_view)->priv->cursor_item;
      cell = gtk_icon_view_get_props (icon_view)->priv->cursor_cell;
      step = count > 0 ? 1 : -1;
      while (item)
	{
	  cell = find_cell (icon_view, item, cell,
			    GTK_ORIENTATION_HORIZONTAL, 
			    step, &count);
	  if (count == 0)
	    break;
	  
	  item = find_item (icon_view, item, 0, step);
	  count = count - step;
	}
    }

  if (!item)
    {
      if (!__gtk_widget_keynav_failed (GTK_WIDGET (icon_view), direction))
        {
          GtkWidget *toplevel = __gtk_widget_get_toplevel (GTK_WIDGET (icon_view));
          if (toplevel)
            __gtk_widget_child_focus (toplevel,
                                    direction == GTK_DIR_LEFT ?
                                    GTK_DIR_TAB_BACKWARD :
                                    GTK_DIR_TAB_FORWARD);
        }

      return;
    }

  if (gtk_icon_view_get_props (icon_view)->priv->modify_selection_pressed ||
      !gtk_icon_view_get_props (icon_view)->priv->extend_selection_pressed ||
      !gtk_icon_view_get_props (icon_view)->priv->anchor_item ||
      gtk_icon_view_get_props (icon_view)->priv->selection_mode != GTK_SELECTION_MULTIPLE)
    gtk_icon_view_get_props (icon_view)->priv->anchor_item = item;

  __gtk_icon_view_set_cursor_item (icon_view, item, cell);

  if (!gtk_icon_view_get_props (icon_view)->priv->modify_selection_pressed &&
      gtk_icon_view_get_props (icon_view)->priv->selection_mode != GTK_SELECTION_NONE)
    {
      dirty = __gtk_icon_view_unselect_all_internal (icon_view);
      dirty = __gtk_icon_view_select_all_between (icon_view, 
						gtk_icon_view_get_props (icon_view)->priv->anchor_item,
						item) || dirty;
    }

  gtk_icon_view_scroll_to_item (icon_view, item);

  if (dirty)
    g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0);
}

static void 
gtk_icon_view_move_cursor_start_end (GtkIconView *icon_view,
				     gint         count)
{
  GtkIconViewItem *item;
  GList *list;
  gboolean dirty = FALSE;
  
  if (!__gtk_widget_has_focus (GTK_WIDGET (icon_view)))
    return;
  
  if (count < 0)
    list = gtk_icon_view_get_props (icon_view)->priv->items;
  else
    list = g_list_last (gtk_icon_view_get_props (icon_view)->priv->items);
  
  item = list ? list->data : NULL;

  if (item == gtk_icon_view_get_props (icon_view)->priv->cursor_item)
    __gtk_widget_error_bell (GTK_WIDGET (icon_view));

  if (!item)
    return;

  if (gtk_icon_view_get_props (icon_view)->priv->modify_selection_pressed ||
      !gtk_icon_view_get_props (icon_view)->priv->extend_selection_pressed ||
      !gtk_icon_view_get_props (icon_view)->priv->anchor_item ||
      gtk_icon_view_get_props (icon_view)->priv->selection_mode != GTK_SELECTION_MULTIPLE)
    gtk_icon_view_get_props (icon_view)->priv->anchor_item = item;

  __gtk_icon_view_set_cursor_item (icon_view, item, -1);

  if (!gtk_icon_view_get_props (icon_view)->priv->modify_selection_pressed &&
      gtk_icon_view_get_props (icon_view)->priv->selection_mode != GTK_SELECTION_NONE)
    {
      dirty = __gtk_icon_view_unselect_all_internal (icon_view);
      dirty = __gtk_icon_view_select_all_between (icon_view, 
						gtk_icon_view_get_props (icon_view)->priv->anchor_item,
						item) || dirty;
    }

  gtk_icon_view_scroll_to_item (icon_view, item);

  if (dirty)
    g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0);
}

/**
 * __gtk_icon_view_scroll_to_path:
 * @icon_view: A #GtkIconView.
 * @path: The path of the item to move to.
 * @use_align: whether to use alignment arguments, or %FALSE.
 * @row_align: The vertical alignment of the item specified by @path.
 * @col_align: The horizontal alignment of the item specified by @path.
 *
 * Moves the alignments of @icon_view to the position specified by @path.  
 * @row_align determines where the row is placed, and @col_align determines 
 * where @column is placed.  Both are expected to be between 0.0 and 1.0. 
 * 0.0 means left/top alignment, 1.0 means right/bottom alignment, 0.5 means 
 * center.
 *
 * If @use_align is %FALSE, then the alignment arguments are ignored, and the
 * tree does the minimum amount of work to scroll the item onto the screen.
 * This means that the item will be scrolled to the edge closest to its current
 * position.  If the item is currently visible on the screen, nothing is done.
 *
 * This function only works if the model is set, and @path is a valid row on 
 * the model. If the model changes before the @icon_view is realized, the 
 * centered path will be modified to reflect this change.
 *
 * Since: 2.8
 **/
void
__gtk_icon_view_scroll_to_path (GtkIconView *icon_view,
			      GtkTreePath *path,
			      gboolean     use_align,
			      gfloat       row_align,
			      gfloat       col_align)
{
  GtkIconViewItem *item = NULL;

  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  g_return_if_fail (path != NULL);
  g_return_if_fail (row_align >= 0.0 && row_align <= 1.0);
  g_return_if_fail (col_align >= 0.0 && col_align <= 1.0);

  if (__gtk_tree_path_get_depth (path) > 0)
    item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items,
			    __gtk_tree_path_get_indices(path)[0]);
  
  if (!item || item->width < 0 ||
      !__gtk_widget_get_realized (GTK_WIDGET (icon_view)))
    {
      if (gtk_icon_view_get_props (icon_view)->priv->scroll_to_path)
	__gtk_tree_row_reference_free (gtk_icon_view_get_props (icon_view)->priv->scroll_to_path);

      gtk_icon_view_get_props (icon_view)->priv->scroll_to_path = NULL;

      if (path)
	gtk_icon_view_get_props (icon_view)->priv->scroll_to_path = __gtk_tree_row_reference_new_proxy (G_OBJECT (icon_view), gtk_icon_view_get_props (icon_view)->priv->model, path);

      gtk_icon_view_get_props (icon_view)->priv->scroll_to_use_align = use_align;
      gtk_icon_view_get_props (icon_view)->priv->scroll_to_row_align = row_align;
      gtk_icon_view_get_props (icon_view)->priv->scroll_to_col_align = col_align;

      return;
    }

  if (use_align)
    {
      gint x, y;
      gint focus_width;
      gfloat offset, value;

      __gtk_widget_style_get (GTK_WIDGET (icon_view),
			    "focus-line-width", &focus_width,
			    NULL);
      
      __gdk_window_get_position (gtk_icon_view_get_props (icon_view)->priv->bin_window, &x, &y);
      
      offset =  y + item->y - focus_width - 
	row_align * (gtk_widget_get_props (GTK_WIDGET (icon_view))->allocation.height - item->height);
      value = CLAMP (gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->value + offset, 
		     gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->lower,
		     gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->upper - gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->page_size);
      __gtk_adjustment_set_value (gtk_icon_view_get_props (icon_view)->priv->vadjustment, value);

      offset = x + item->x - focus_width - 
	col_align * (gtk_widget_get_props (GTK_WIDGET (icon_view))->allocation.width - item->width);
      value = CLAMP (gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->hadjustment)->value + offset, 
		     gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->hadjustment)->lower,
		     gtk_adjustment_get_props (gtk_icon_view_get_instance_private (icon_view)->hadjustment)->upper - gtk_adjustment_get_props (gtk_icon_view_get_instance_private (icon_view)->hadjustment)->page_size);
      __gtk_adjustment_set_value (gtk_icon_view_get_props (icon_view)->priv->hadjustment, value);

      __gtk_adjustment_changed (gtk_icon_view_get_props (icon_view)->priv->hadjustment);
      __gtk_adjustment_changed (gtk_icon_view_get_props (icon_view)->priv->vadjustment);
    }
  else
    gtk_icon_view_scroll_to_item (icon_view, item);
}


static void     
gtk_icon_view_scroll_to_item (GtkIconView     *icon_view, 
			      GtkIconViewItem *item)
{
  gint x, y, width, height;
  gint focus_width;

  __gtk_widget_style_get (GTK_WIDGET (icon_view),
			"focus-line-width", &focus_width,
			NULL);

  __gdk_window_get_position (gtk_icon_view_get_props (icon_view)->priv->bin_window, &x, &y);
  
  if (y + item->y - focus_width < 0)
    __gtk_adjustment_set_value (gtk_icon_view_get_props (icon_view)->priv->vadjustment, 
			      gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->value + y + item->y - focus_width);
  else if (y + item->y + item->height + focus_width > gtk_widget_get_props (GTK_WIDGET (icon_view))->allocation.height)
    __gtk_adjustment_set_value (gtk_icon_view_get_props (icon_view)->priv->vadjustment, 
			      gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->value + y + item->y + item->height 
			      + focus_width - gtk_widget_get_props (GTK_WIDGET (icon_view))->allocation.height);

  if (x + item->x - focus_width < 0)
    __gtk_adjustment_set_value (gtk_icon_view_get_props (icon_view)->priv->hadjustment, 
			      gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->hadjustment)->value + x + item->x - focus_width);
  else if (x + item->x + item->width + focus_width > gtk_widget_get_props (GTK_WIDGET (icon_view))->allocation.width)
    __gtk_adjustment_set_value (gtk_icon_view_get_props (icon_view)->priv->hadjustment, 
			      gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->hadjustment)->value + x + item->x + item->width 
			      + focus_width - gtk_widget_get_props (GTK_WIDGET (icon_view))->allocation.width);

  __gtk_adjustment_changed (gtk_icon_view_get_props (icon_view)->priv->hadjustment);
  __gtk_adjustment_changed (gtk_icon_view_get_props (icon_view)->priv->vadjustment);
}

/* GtkCellLayout implementation */
static GtkIconViewCellInfo *
gtk_icon_view_get_cell_info (GtkIconView     *icon_view,
                             GtkCellRenderer *renderer)
{
  GList *i;

  for (i = gtk_icon_view_get_props (icon_view)->priv->cell_list; i; i = i->next)
    {
      GtkIconViewCellInfo *info = (GtkIconViewCellInfo *)i->data;

      if (info->cell == renderer)
        return info;
    }

  return NULL;
}

static void
gtk_icon_view_set_cell_data (GtkIconView     *icon_view, 
			     GtkIconViewItem *item)
{
  GList *i;
  gboolean iters_persist;
  GtkTreeIter iter;
  
  iters_persist = __gtk_tree_model_get_flags (gtk_icon_view_get_props (icon_view)->priv->model) & GTK_TREE_MODEL_ITERS_PERSIST;
  
  if (!iters_persist)
    {
      GtkTreePath *path;

      path = __gtk_tree_path_new_from_indices (item->index, -1);
      if (!__gtk_tree_model_get_iter (gtk_icon_view_get_props (icon_view)->priv->model, &iter, path))
        return;
      __gtk_tree_path_free (path);
    }
  else
    iter = item->iter;
  
  for (i = gtk_icon_view_get_props (icon_view)->priv->cell_list; i; i = i->next)
    {
      GSList *j;
      GtkIconViewCellInfo *info = i->data;

      g_object_freeze_notify (G_OBJECT (info->cell));

      for (j = info->attributes; j && j->next; j = j->next->next)
        {
          gchar *property = j->data;
          gint column = GPOINTER_TO_INT (j->next->data);
          GValue value = {0, };

          __gtk_tree_model_get_value (gtk_icon_view_get_props (icon_view)->priv->model, &iter,
                                    column, &value);
          g_object_set_property (G_OBJECT (info->cell),
                                 property, &value);
          g_value_unset (&value);
        }

      if (info->func)
	(* info->func) (GTK_CELL_LAYOUT (icon_view),
			info->cell,
			gtk_icon_view_get_props (icon_view)->priv->model,
			&iter,
			info->func_data);
      
      g_object_thaw_notify (G_OBJECT (info->cell));
    }  
}

static void 
free_cell_attributes (GtkIconViewCellInfo *info)
{ 
  GSList *list;

  list = info->attributes;
  while (list && list->next)
    {
      g_free (list->data);
      list = list->next->next;
    }
  
  g_slist_free (info->attributes);
  info->attributes = NULL;
}

static void
free_cell_info (GtkIconViewCellInfo *info)
{
  free_cell_attributes (info);

  g_object_unref (info->cell);
  
  if (info->destroy)
    (* info->destroy) (info->func_data);

  g_free (info);
}

static void
gtk_icon_view_cell_layout_pack_start (GtkCellLayout   *layout,
                                      GtkCellRenderer *renderer,
                                      gboolean         expand)
{
  GtkIconViewCellInfo *info;
  GtkIconView *icon_view = GTK_ICON_VIEW (layout);

  g_return_if_fail (GTK_IS_CELL_RENDERER (renderer));
  g_return_if_fail (!gtk_icon_view_get_cell_info (icon_view, renderer));

  g_object_ref_sink (renderer);

  info = g_new0 (GtkIconViewCellInfo, 1);
  info->cell = renderer;
  info->expand = expand ? TRUE : FALSE;
  info->pack = GTK_PACK_START;
  info->position = gtk_icon_view_get_props (icon_view)->priv->n_cells;
  
  gtk_icon_view_get_props (icon_view)->priv->cell_list = g_list_append (gtk_icon_view_get_props (icon_view)->priv->cell_list, info);
  gtk_icon_view_get_props (icon_view)->priv->n_cells++;
}

static void
gtk_icon_view_cell_layout_pack_end (GtkCellLayout   *layout,
                                    GtkCellRenderer *renderer,
                                    gboolean         expand)
{
  GtkIconViewCellInfo *info;
  GtkIconView *icon_view = GTK_ICON_VIEW (layout);

  g_return_if_fail (GTK_IS_CELL_RENDERER (renderer));
  g_return_if_fail (!gtk_icon_view_get_cell_info (icon_view, renderer));

  g_object_ref_sink (renderer);

  info = g_new0 (GtkIconViewCellInfo, 1);
  info->cell = renderer;
  info->expand = expand ? TRUE : FALSE;
  info->pack = GTK_PACK_END;
  info->position = gtk_icon_view_get_props (icon_view)->priv->n_cells;

  gtk_icon_view_get_props (icon_view)->priv->cell_list = g_list_append (gtk_icon_view_get_props (icon_view)->priv->cell_list, info);
  gtk_icon_view_get_props (icon_view)->priv->n_cells++;
}

static void
gtk_icon_view_cell_layout_add_attribute (GtkCellLayout   *layout,
                                         GtkCellRenderer *renderer,
                                         const gchar     *attribute,
                                         gint             column)
{
  GtkIconViewCellInfo *info;
  GtkIconView *icon_view = GTK_ICON_VIEW (layout);

  info = gtk_icon_view_get_cell_info (icon_view, renderer);
  g_return_if_fail (info != NULL);

  info->attributes = g_slist_prepend (info->attributes,
                                      GINT_TO_POINTER (column));
  info->attributes = g_slist_prepend (info->attributes,
                                      g_strdup (attribute));
}

static void
gtk_icon_view_cell_layout_clear (GtkCellLayout *layout)
{
  GtkIconView *icon_view = GTK_ICON_VIEW (layout);

  while (gtk_icon_view_get_props (icon_view)->priv->cell_list)
    {
      GtkIconViewCellInfo *info = (GtkIconViewCellInfo *)gtk_icon_view_get_props (icon_view)->priv->cell_list->data;
      free_cell_info (info);
      gtk_icon_view_get_props (icon_view)->priv->cell_list = g_list_delete_link (gtk_icon_view_get_props (icon_view)->priv->cell_list, 
						       gtk_icon_view_get_props (icon_view)->priv->cell_list);
    }

  gtk_icon_view_get_props (icon_view)->priv->n_cells = 0;
}

static void
gtk_icon_view_cell_layout_set_cell_data_func (GtkCellLayout         *layout,
                                              GtkCellRenderer       *cell,
                                              GtkCellLayoutDataFunc  func,
                                              gpointer               func_data,
                                              GDestroyNotify         destroy)
{
  GtkIconViewCellInfo *info;
  GtkIconView *icon_view = GTK_ICON_VIEW (layout);

  info = gtk_icon_view_get_cell_info (icon_view, cell);
  g_return_if_fail (info != NULL);

  if (info->destroy)
    {
      GDestroyNotify d = info->destroy;

      info->destroy = NULL;
      d (info->func_data);
    }

  info->func = func;
  info->func_data = func_data;
  info->destroy = destroy;
}

static void
gtk_icon_view_cell_layout_clear_attributes (GtkCellLayout   *layout,
                                            GtkCellRenderer *renderer)
{
  GtkIconViewCellInfo *info;

  info = gtk_icon_view_get_cell_info (GTK_ICON_VIEW (layout), renderer);
  if (info != NULL)
    free_cell_attributes (info);
}

static void
gtk_icon_view_cell_layout_reorder (GtkCellLayout   *layout,
                                   GtkCellRenderer *cell,
                                   gint             position)
{
  GtkIconView *icon_view;
  GList *link, *l;
  GtkIconViewCellInfo *info;
  gint i;

  icon_view = GTK_ICON_VIEW (layout);

  g_return_if_fail (GTK_IS_CELL_RENDERER (cell));

  info = gtk_icon_view_get_cell_info (icon_view, cell);

  g_return_if_fail (info != NULL);
  g_return_if_fail (position >= 0);

  link = g_list_find (gtk_icon_view_get_props (icon_view)->priv->cell_list, info);

  g_return_if_fail (link != NULL);

  gtk_icon_view_get_props (icon_view)->priv->cell_list = g_list_delete_link (gtk_icon_view_get_props (icon_view)->priv->cell_list,
                                                   link);
  gtk_icon_view_get_props (icon_view)->priv->cell_list = g_list_insert (gtk_icon_view_get_props (icon_view)->priv->cell_list,
                                             info, position);

  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list, i = 0; l; l = l->next, i++)
    {
      info = l->data;

      info->position = i;
    }

  __gtk_widget_queue_draw (GTK_WIDGET (icon_view));
}

static GList *
gtk_icon_view_cell_layout_get_cells (GtkCellLayout *layout)
{
  GtkIconView *icon_view = (GtkIconView *)layout;
  GList *retval = NULL, *l;

  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list; l; l = l->next)
    {
      GtkIconViewCellInfo *info = (GtkIconViewCellInfo *)l->data;

      retval = g_list_prepend (retval, info->cell);
    }

  return g_list_reverse (retval);
}

/* Public API */


/**
 * __gtk_icon_view_new:
 * 
 * Creates a new #GtkIconView widget
 * 
 * Return value: A newly created #GtkIconView widget
 *
 * Since: 2.6
 **/
GtkWidget *
__gtk_icon_view_new (void)
{
  return g_object_new (GTK_TYPE_ICON_VIEW, NULL);
}

/**
 * __gtk_icon_view_new_with_model:
 * @model: The model.
 * 
 * Creates a new #GtkIconView widget with the model @model.
 * 
 * Return value: A newly created #GtkIconView widget.
 *
 * Since: 2.6 
 **/
GtkWidget *
__gtk_icon_view_new_with_model (GtkTreeModel *model)
{
  return g_object_new (GTK_TYPE_ICON_VIEW, "model", model, NULL);
}

/**
 * __gtk_icon_view_convert_widget_to_bin_window_coords:
 * @icon_view: a #GtkIconView 
 * @wx: X coordinate relative to the widget
 * @wy: Y coordinate relative to the widget
 * @bx: (out): return location for bin_window X coordinate
 * @by: (out): return location for bin_window Y coordinate
 * 
 * Converts widget coordinates to coordinates for the bin_window,
 * as expected by e.g. __gtk_icon_view_get_path_at_pos(). 
 *
 * Since: 2.12
 */
void
__gtk_icon_view_convert_widget_to_bin_window_coords (GtkIconView *icon_view,
                                                   gint         wx,
                                                   gint         wy, 
                                                   gint        *bx,
                                                   gint        *by)
{
  gint x, y;

  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  if (gtk_icon_view_get_props (icon_view)->priv->bin_window) 
    __gdk_window_get_position (gtk_icon_view_get_props (icon_view)->priv->bin_window, &x, &y);
  else
    x = y = 0;
 
  if (bx)
    *bx = wx - x;
  if (by)
    *by = wy - y;
}

/**
 * __gtk_icon_view_get_path_at_pos:
 * @icon_view: A #GtkIconView.
 * @x: The x position to be identified
 * @y: The y position to be identified
 * 
 * Finds the path at the point (@x, @y), relative to bin_window coordinates.
 * See __gtk_icon_view_get_item_at_pos(), if you are also interested in
 * the cell at the specified position. 
 * See __gtk_icon_view_convert_widget_to_bin_window_coords() for converting
 * widget coordinates to bin_window coordinates.
 * 
 * Return value: The #GtkTreePath corresponding to the icon or %NULL
 * if no icon exists at that position.
 *
 * Since: 2.6 
 **/
GtkTreePath *
__gtk_icon_view_get_path_at_pos (GtkIconView *icon_view,
			       gint         x,
			       gint         y)
{
  GtkIconViewItem *item;
  GtkTreePath *path;
  
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), NULL);

  item = gtk_icon_view_get_item_at_coords (icon_view, x, y, TRUE, NULL);

  if (!item)
    return NULL;

  path = __gtk_tree_path_new_from_indices (item->index, -1);

  return path;
}

/**
 * __gtk_icon_view_get_item_at_pos:
 * @icon_view: A #GtkIconView.
 * @x: The x position to be identified
 * @y: The y position to be identified
 * @path: (allow-none): Return location for the path, or %NULL
 * @cell: Return location for the renderer responsible for the cell
 *   at (@x, @y), or %NULL
 * 
 * Finds the path at the point (@x, @y), relative to bin_window coordinates.
 * In contrast to __gtk_icon_view_get_path_at_pos(), this function also 
 * obtains the cell at the specified position. The returned path should
 * be freed with __gtk_tree_path_free().
 * See __gtk_icon_view_convert_widget_to_bin_window_coords() for converting
 * widget coordinates to bin_window coordinates.
 * 
 * Return value: %TRUE if an item exists at the specified position
 *
 * Since: 2.8
 **/
gboolean 
__gtk_icon_view_get_item_at_pos (GtkIconView      *icon_view,
			       gint              x,
			       gint              y,
			       GtkTreePath     **path,
			       GtkCellRenderer **cell)
{
  GtkIconViewItem *item;
  GtkIconViewCellInfo *info;
  
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), FALSE);

  item = gtk_icon_view_get_item_at_coords (icon_view, x, y, TRUE, &info);

  if (path != NULL)
    {
      if (item != NULL)
	*path = __gtk_tree_path_new_from_indices (item->index, -1);
      else
	*path = NULL;
    }

  if (cell != NULL)
    {
      if (info != NULL)
	*cell = info->cell;
      else 
	*cell = NULL;
    }

  return (item != NULL);
}

/**
 * __gtk_icon_view_set_tooltip_item:
 * @icon_view: a #GtkIconView
 * @tooltip: a #GtkTooltip
 * @path: a #GtkTreePath
 * 
 * Sets the tip area of @tooltip to be the area covered by the item at @path.
 * See also __gtk_icon_view_set_tooltip_column() for a simpler alternative.
 * See also __gtk_tooltip_set_tip_area().
 * 
 * Since: 2.12
 */
void 
__gtk_icon_view_set_tooltip_item (GtkIconView     *icon_view,
                                GtkTooltip      *tooltip,
                                GtkTreePath     *path)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  g_return_if_fail (GTK_IS_TOOLTIP (tooltip));

  __gtk_icon_view_set_tooltip_cell (icon_view, tooltip, path, NULL);
}

/**
 * __gtk_icon_view_set_tooltip_cell:
 * @icon_view: a #GtkIconView
 * @tooltip: a #GtkTooltip
 * @path: a #GtkTreePath
 * @cell: (allow-none): a #GtkCellRenderer or %NULL
 *
 * Sets the tip area of @tooltip to the area which @cell occupies in
 * the item pointed to by @path. See also __gtk_tooltip_set_tip_area().
 *
 * See also __gtk_icon_view_set_tooltip_column() for a simpler alternative.
 *
 * Since: 2.12
 */
void
__gtk_icon_view_set_tooltip_cell (GtkIconView     *icon_view,
                                GtkTooltip      *tooltip,
                                GtkTreePath     *path,
                                GtkCellRenderer *cell)
{
  GdkRectangle rect;
  GtkIconViewItem *item = NULL;
  GtkIconViewCellInfo *info = NULL;
  gint x, y;
 
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  g_return_if_fail (GTK_IS_TOOLTIP (tooltip));
  g_return_if_fail (cell == NULL || GTK_IS_CELL_RENDERER (cell));

  if (__gtk_tree_path_get_depth (path) > 0)
    item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items,
                            __gtk_tree_path_get_indices(path)[0]);
 
  if (!item)
    return;

  if (cell)
    {
      info = gtk_icon_view_get_cell_info (icon_view, cell);
      gtk_icon_view_get_cell_area (icon_view, item, info, &rect);
    }
  else
    {
      rect.x = item->x;
      rect.y = item->y;
      rect.width = item->width;
      rect.height = item->height;
    }
  
  if (gtk_icon_view_get_props (icon_view)->priv->bin_window)
    {
      __gdk_window_get_position (gtk_icon_view_get_props (icon_view)->priv->bin_window, &x, &y);
      rect.x += x;
      rect.y += y; 
    }

  __gtk_tooltip_set_tip_area (tooltip, &rect); 
}


/**
 * __gtk_icon_view_get_tooltip_context:
 * @icon_view: an #GtkIconView
 * @x: (inout): the x coordinate (relative to widget coordinates)
 * @y: (inout): the y coordinate (relative to widget coordinates)
 * @keyboard_tip: whether this is a keyboard tooltip or not
 * @model: (out) (allow-none): a pointer to receive a #GtkTreeModel or %NULL
 * @path: (out) (allow-none): a pointer to receive a #GtkTreePath or %NULL
 * @iter: (out) (allow-none): a pointer to receive a #GtkTreeIter or %NULL
 *
 * This function is supposed to be used in a #GtkWidget::query-tooltip
 * signal handler for #GtkIconView.  The @x, @y and @keyboard_tip values
 * which are received in the signal handler, should be passed to this
 * function without modification.
 *
 * The return value indicates whether there is an icon view item at the given
 * coordinates (%TRUE) or not (%FALSE) for mouse tooltips. For keyboard
 * tooltips the item returned will be the cursor item. When %TRUE, then any of
 * @model, @path and @iter which have been provided will be set to point to
 * that row and the corresponding model. @x and @y will always be converted
 * to be relative to @icon_view's bin_window if @keyboard_tooltip is %FALSE.
 *
 * Return value: whether or not the given tooltip context points to a item
 *
 * Since: 2.12
 */
gboolean
__gtk_icon_view_get_tooltip_context (GtkIconView   *icon_view,
                                   gint          *x,
                                   gint          *y,
                                   gboolean       keyboard_tip,
                                   GtkTreeModel **model,
                                   GtkTreePath  **path,
                                   GtkTreeIter   *iter)
{
  GtkTreePath *tmppath = NULL;

  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), FALSE);
  g_return_val_if_fail (x != NULL, FALSE);
  g_return_val_if_fail (y != NULL, FALSE);

  if (keyboard_tip)
    {
      __gtk_icon_view_get_cursor (icon_view, &tmppath, NULL);

      if (!tmppath)
        return FALSE;
    }
  else
    {
      __gtk_icon_view_convert_widget_to_bin_window_coords (icon_view, *x, *y,
                                                         x, y);

      if (!__gtk_icon_view_get_item_at_pos (icon_view, *x, *y, &tmppath, NULL))
        return FALSE;
    }

  if (model)
    *model = __gtk_icon_view_get_model (icon_view);

  if (iter)
    __gtk_tree_model_get_iter (__gtk_icon_view_get_model (icon_view),
                             iter, tmppath);

  if (path)
    *path = tmppath;
  else
    __gtk_tree_path_free (tmppath);

  return TRUE;
}

static gboolean
gtk_icon_view_set_tooltip_query_cb (GtkWidget  *widget,
                                    gint        x,
                                    gint        y,
                                    gboolean    keyboard_tip,
                                    GtkTooltip *tooltip,
                                    gpointer    data)
{
  gchar *str;
  GtkTreeIter iter;
  GtkTreePath *path;
  GtkTreeModel *model;
  GtkIconView *icon_view = GTK_ICON_VIEW (widget);

  if (!__gtk_icon_view_get_tooltip_context (GTK_ICON_VIEW (widget),
                                          &x, &y,
                                          keyboard_tip,
                                          &model, &path, &iter))
    return FALSE;

  __gtk_tree_model_get (model, &iter, gtk_icon_view_get_props (icon_view)->priv->tooltip_column, &str, -1);

  if (!str)
    {
      __gtk_tree_path_free (path);
      return FALSE;
    }

  __gtk_tooltip_set_markup (tooltip, str);
  __gtk_icon_view_set_tooltip_item (icon_view, tooltip, path);

  __gtk_tree_path_free (path);
  g_free (str);

  return TRUE;
}


/**
 * __gtk_icon_view_set_tooltip_column:
 * @icon_view: a #GtkIconView
 * @column: an integer, which is a valid column number for @icon_view's model
 *
 * If you only plan to have simple (text-only) tooltips on full items, you
 * can use this function to have #GtkIconView handle these automatically
 * for you. @column should be set to the column in @icon_view's model
 * containing the tooltip texts, or -1 to disable this feature.
 *
 * When enabled, #GtkWidget::has-tooltip will be set to %TRUE and
 * @icon_view will connect a #GtkWidget::query-tooltip signal handler.
 *
 * Since: 2.12
 */
void
__gtk_icon_view_set_tooltip_column (GtkIconView *icon_view,
                                  gint         column)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  if (column == gtk_icon_view_get_props (icon_view)->priv->tooltip_column)
    return;

  if (column == -1)
    {
      g_signal_handlers_disconnect_by_func (icon_view,
                                            gtk_icon_view_set_tooltip_query_cb,
                                            NULL);
      __gtk_widget_set_has_tooltip (GTK_WIDGET (icon_view), FALSE);
    }
  else
    {
      if (gtk_icon_view_get_props (icon_view)->priv->tooltip_column == -1)
        {
          g_signal_connect (icon_view, "query-tooltip",
                            G_CALLBACK (gtk_icon_view_set_tooltip_query_cb), NULL);
          __gtk_widget_set_has_tooltip (GTK_WIDGET (icon_view), TRUE);
        }
    }

  gtk_icon_view_get_props (icon_view)->priv->tooltip_column = column;
  g_object_notify (G_OBJECT (icon_view), "tooltip-column");
}

/** 
 * __gtk_icon_view_get_tooltip_column:
 * @icon_view: a #GtkIconView
 *
 * Returns the column of @icon_view's model which is being used for
 * displaying tooltips on @icon_view's rows.
 *
 * Return value: the index of the tooltip column that is currently being
 * used, or -1 if this is disabled.
 *
 * Since: 2.12
 */
gint
__gtk_icon_view_get_tooltip_column (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), 0);

  return gtk_icon_view_get_props (icon_view)->priv->tooltip_column;
}

/**
 * __gtk_icon_view_get_visible_range:
 * @icon_view: A #GtkIconView
 * @start_path: (allow-none): Return location for start of region, or %NULL
 * @end_path: (allow-none): Return location for end of region, or %NULL
 * 
 * Sets @start_path and @end_path to be the first and last visible path. 
 * Note that there may be invisible paths in between.
 * 
 * Both paths should be freed with __gtk_tree_path_free() after use.
 * 
 * Return value: %TRUE, if valid paths were placed in @start_path and @end_path
 *
 * Since: 2.8
 **/
gboolean
__gtk_icon_view_get_visible_range (GtkIconView  *icon_view,
				 GtkTreePath **start_path,
				 GtkTreePath **end_path)
{
  gint start_index = -1;
  gint end_index = -1;
  GList *icons;

  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), FALSE);

  if (gtk_icon_view_get_props (icon_view)->priv->hadjustment == NULL ||
      gtk_icon_view_get_props (icon_view)->priv->vadjustment == NULL)
    return FALSE;

  if (start_path == NULL && end_path == NULL)
    return FALSE;
  
  for (icons = gtk_icon_view_get_props (icon_view)->priv->items; icons; icons = icons->next) 
    {
      GtkIconViewItem *item = icons->data;

      if ((item->x + item->width >= (int)gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->hadjustment)->value) &&
	  (item->y + item->height >= (int)gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->value) &&
	  (item->x <= (int) (gtk_adjustment_get_props (gtk_icon_view_get_instance_private (icon_view)->hadjustment)->value + gtk_adjustment_get_props (gtk_icon_view_get_instance_private (icon_view)->hadjustment)->page_size)) &&
	  (item->y <= (int) (gtk_adjustment_get_props (gtk_icon_view_get_instance_private (icon_view)->vadjustment)->value + gtk_adjustment_get_props (gtk_icon_view_get_instance_private (icon_view)->vadjustment)->page_size)))
	{
	  if (start_index == -1)
	    start_index = item->index;
	  end_index = item->index;
	}
    }

  if (start_path && start_index != -1)
    *start_path = __gtk_tree_path_new_from_indices (start_index, -1);
  if (end_path && end_index != -1)
    *end_path = __gtk_tree_path_new_from_indices (end_index, -1);
  
  return start_index != -1;
}

/**
 * __gtk_icon_view_selected_foreach:
 * @icon_view: A #GtkIconView.
 * @func: (scope call): The function to call for each selected icon.
 * @data: User data to pass to the function.
 * 
 * Calls a function for each selected icon. Note that the model or
 * selection cannot be modified from within this function.
 *
 * Since: 2.6 
 **/
void
__gtk_icon_view_selected_foreach (GtkIconView           *icon_view,
				GtkIconViewForeachFunc func,
				gpointer               data)
{
  GList *list;
  
  for (list = gtk_icon_view_get_props (icon_view)->priv->items; list; list = list->next)
    {
      GtkIconViewItem *item = list->data;
      GtkTreePath *path = __gtk_tree_path_new_from_indices (item->index, -1);

      if (item->selected)
	(* func) (icon_view, path, data);

      __gtk_tree_path_free (path);
    }
}

/**
 * __gtk_icon_view_set_selection_mode:
 * @icon_view: A #GtkIconView.
 * @mode: The selection mode
 * 
 * Sets the selection mode of the @icon_view.
 *
 * Since: 2.6 
 **/
void
__gtk_icon_view_set_selection_mode (GtkIconView      *icon_view,
				  GtkSelectionMode  mode)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  if (mode == gtk_icon_view_get_props (icon_view)->priv->selection_mode)
    return;
  
  if (mode == GTK_SELECTION_NONE ||
      gtk_icon_view_get_props (icon_view)->priv->selection_mode == GTK_SELECTION_MULTIPLE)
    __gtk_icon_view_unselect_all (icon_view);
  
  gtk_icon_view_get_props (icon_view)->priv->selection_mode = mode;

  g_object_notify (G_OBJECT (icon_view), "selection-mode");
}

/**
 * __gtk_icon_view_get_selection_mode:
 * @icon_view: A #GtkIconView.
 * 
 * Gets the selection mode of the @icon_view.
 *
 * Return value: the current selection mode
 *
 * Since: 2.6 
 **/
GtkSelectionMode
__gtk_icon_view_get_selection_mode (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), GTK_SELECTION_SINGLE);

  return gtk_icon_view_get_props (icon_view)->priv->selection_mode;
}

/**
 * __gtk_icon_view_set_model:
 * @icon_view: A #GtkIconView.
 * @model: (allow-none): The model.
 *
 * Sets the model for a #GtkIconView.
 * If the @icon_view already has a model set, it will remove
 * it before setting the new model.  If @model is %NULL, then
 * it will unset the old model.
 *
 * Since: 2.6 
 **/
void
__gtk_icon_view_set_model (GtkIconView *icon_view,
			 GtkTreeModel *model)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  g_return_if_fail (model == NULL || GTK_IS_TREE_MODEL (model));
  
  if (gtk_icon_view_get_props (icon_view)->priv->model == model)
    return;

  if (gtk_icon_view_get_props (icon_view)->priv->scroll_to_path)
    {
      __gtk_tree_row_reference_free (gtk_icon_view_get_props (icon_view)->priv->scroll_to_path);
      gtk_icon_view_get_props (icon_view)->priv->scroll_to_path = NULL;
    }

  gtk_icon_view_stop_editing (icon_view, TRUE);

  if (model)
    {
      GType column_type;
      
      g_return_if_fail (__gtk_tree_model_get_flags (model) & GTK_TREE_MODEL_LIST_ONLY);

      if (gtk_icon_view_get_props (icon_view)->priv->pixbuf_column != -1)
	{
	  column_type = __gtk_tree_model_get_column_type (model,
							gtk_icon_view_get_props (icon_view)->priv->pixbuf_column);	  

	  g_return_if_fail (column_type == GDK_TYPE_PIXBUF);
	}

      if (gtk_icon_view_get_props (icon_view)->priv->text_column != -1)
	{
	  column_type = __gtk_tree_model_get_column_type (model,
							gtk_icon_view_get_props (icon_view)->priv->text_column);	  

	  g_return_if_fail (column_type == G_TYPE_STRING);
	}

      if (gtk_icon_view_get_props (icon_view)->priv->markup_column != -1)
	{
	  column_type = __gtk_tree_model_get_column_type (model,
							gtk_icon_view_get_props (icon_view)->priv->markup_column);	  

	  g_return_if_fail (column_type == G_TYPE_STRING);
	}
      
    }
  
  if (gtk_icon_view_get_props (icon_view)->priv->model)
    {
      g_signal_handlers_disconnect_by_func (gtk_icon_view_get_props (icon_view)->priv->model,
					    gtk_icon_view_row_changed,
					    icon_view);
      g_signal_handlers_disconnect_by_func (gtk_icon_view_get_props (icon_view)->priv->model,
					    gtk_icon_view_row_inserted,
					    icon_view);
      g_signal_handlers_disconnect_by_func (gtk_icon_view_get_props (icon_view)->priv->model,
					    gtk_icon_view_row_deleted,
					    icon_view);
      g_signal_handlers_disconnect_by_func (gtk_icon_view_get_props (icon_view)->priv->model,
					    gtk_icon_view_rows_reordered,
					    icon_view);

      g_object_unref (gtk_icon_view_get_props (icon_view)->priv->model);
      
      g_list_foreach (gtk_icon_view_get_props (icon_view)->priv->items, (GFunc)gtk_icon_view_item_free, NULL);
      g_list_free (gtk_icon_view_get_props (icon_view)->priv->items);
      gtk_icon_view_get_props (icon_view)->priv->items = NULL;
      gtk_icon_view_get_props (icon_view)->priv->anchor_item = NULL;
      gtk_icon_view_get_props (icon_view)->priv->cursor_item = NULL;
      gtk_icon_view_get_props (icon_view)->priv->last_single_clicked = NULL;
      gtk_icon_view_get_props (icon_view)->priv->width = 0;
      gtk_icon_view_get_props (icon_view)->priv->height = 0;
    }

  gtk_icon_view_get_props (icon_view)->priv->model = model;

  if (gtk_icon_view_get_props (icon_view)->priv->model)
    {
      g_object_ref (gtk_icon_view_get_props (icon_view)->priv->model);
      g_signal_connect (gtk_icon_view_get_props (icon_view)->priv->model,
			"row-changed",
			G_CALLBACK (gtk_icon_view_row_changed),
			icon_view);
      g_signal_connect (gtk_icon_view_get_props (icon_view)->priv->model,
			"row-inserted",
			G_CALLBACK (gtk_icon_view_row_inserted),
			icon_view);
      g_signal_connect (gtk_icon_view_get_props (icon_view)->priv->model,
			"row-deleted",
			G_CALLBACK (gtk_icon_view_row_deleted),
			icon_view);
      g_signal_connect (gtk_icon_view_get_props (icon_view)->priv->model,
			"rows-reordered",
			G_CALLBACK (gtk_icon_view_rows_reordered),
			icon_view);

      gtk_icon_view_build_items (icon_view);

      gtk_icon_view_queue_layout (icon_view);
    }

  g_object_notify (G_OBJECT (icon_view), "model");  

  if (__gtk_widget_get_realized (GTK_WIDGET (icon_view)))
    __gtk_widget_queue_resize (GTK_WIDGET (icon_view));
}

/**
 * __gtk_icon_view_get_model:
 * @icon_view: a #GtkIconView
 *
 * Returns the model the #GtkIconView is based on.  Returns %NULL if the
 * model is unset.
 *
 * Return value: (transfer none): A #GtkTreeModel, or %NULL if none is
 *     currently being used.
 *
 * Since: 2.6 
 **/
GtkTreeModel *
__gtk_icon_view_get_model (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), NULL);

  return gtk_icon_view_get_props (icon_view)->priv->model;
}

static void
update_text_cell (GtkIconView *icon_view)
{
  GtkIconViewCellInfo *info;
  GList *l;
  gint i;
	  
  if (gtk_icon_view_get_props (icon_view)->priv->text_column == -1 &&
      gtk_icon_view_get_props (icon_view)->priv->markup_column == -1)
    {
      if (gtk_icon_view_get_props (icon_view)->priv->text_cell != -1)
	{
	  if (gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell > gtk_icon_view_get_props (icon_view)->priv->text_cell)
	    gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell--;

	  info = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->cell_list, 
				  gtk_icon_view_get_props (icon_view)->priv->text_cell);
	  
	  gtk_icon_view_get_props (icon_view)->priv->cell_list = g_list_remove (gtk_icon_view_get_props (icon_view)->priv->cell_list, info);
	  
	  free_cell_info (info);
	  
	  gtk_icon_view_get_props (icon_view)->priv->n_cells--;
	  gtk_icon_view_get_props (icon_view)->priv->text_cell = -1;
	}
    }
  else 
    {
      if (gtk_icon_view_get_props (icon_view)->priv->text_cell == -1)
	{
	  GtkCellRenderer *cell = __gtk_cell_renderer_text_new ();
	  __gtk_cell_layout_pack_end (GTK_CELL_LAYOUT (icon_view), cell, FALSE);
	  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list, i = 0; l; l = l->next, i++)
	    {
	      info = l->data;
	      if (info->cell == cell)
		{
		  gtk_icon_view_get_props (icon_view)->priv->text_cell = i;
		  break;
		}
	    }
	}
      
      info = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->cell_list,
			      gtk_icon_view_get_props (icon_view)->priv->text_cell);

      if (gtk_icon_view_get_props (icon_view)->priv->markup_column != -1)
	__gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (icon_view),
					info->cell, 
					"markup", gtk_icon_view_get_props (icon_view)->priv->markup_column, 
					NULL);
      else
	__gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (icon_view),
					info->cell, 
					"text", gtk_icon_view_get_props (icon_view)->priv->text_column, 
					NULL);

      if (gtk_icon_view_get_props (icon_view)->priv->item_orientation == GTK_ORIENTATION_VERTICAL)
	g_object_set (info->cell,
                      "alignment", PANGO_ALIGN_CENTER,
		      "wrap-mode", PANGO_WRAP_WORD_CHAR,
		      "xalign", 0.0,
		      "yalign", 0.0,
		      NULL);
      else
	g_object_set (info->cell,
                      "alignment", PANGO_ALIGN_LEFT,
		      "wrap-mode", PANGO_WRAP_WORD_CHAR,
		      "xalign", 0.0,
		      "yalign", 0.0,
		      NULL);
    }
}

static void
update_pixbuf_cell (GtkIconView *icon_view)
{
  GtkIconViewCellInfo *info;
  GList *l;
  gint i;

  if (gtk_icon_view_get_props (icon_view)->priv->pixbuf_column == -1)
    {
      if (gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell != -1)
	{
	  if (gtk_icon_view_get_props (icon_view)->priv->text_cell > gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell)
	    gtk_icon_view_get_props (icon_view)->priv->text_cell--;

	  info = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->cell_list, 
				  gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell);
	  
	  gtk_icon_view_get_props (icon_view)->priv->cell_list = g_list_remove (gtk_icon_view_get_props (icon_view)->priv->cell_list, info);
	  
	  free_cell_info (info);
	  
	  gtk_icon_view_get_props (icon_view)->priv->n_cells--;
	  gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell = -1;
	}
    }
  else 
    {
      if (gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell == -1)
	{
	  GtkCellRenderer *cell = __gtk_cell_renderer_pixbuf_new ();
	  
	  __gtk_cell_layout_pack_start (GTK_CELL_LAYOUT (icon_view), cell, FALSE);
	  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list, i = 0; l; l = l->next, i++)
	    {
	      info = l->data;
	      if (info->cell == cell)
		{
		  gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell = i;
		  break;
		}
	    }
	}
      
	info = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->cell_list, 
				gtk_icon_view_get_props (icon_view)->priv->pixbuf_cell);
	
	__gtk_cell_layout_set_attributes (GTK_CELL_LAYOUT (icon_view),
					info->cell, 
					"pixbuf", gtk_icon_view_get_props (icon_view)->priv->pixbuf_column, 
					NULL);

	if (gtk_icon_view_get_props (icon_view)->priv->item_orientation == GTK_ORIENTATION_VERTICAL)
	  g_object_set (info->cell,
			"xalign", 0.5,
			"yalign", 1.0,
			NULL);
	else
	  g_object_set (info->cell,
			"xalign", 0.0,
			"yalign", 0.0,
			NULL);
    }
}

/**
 * __gtk_icon_view_set_text_column:
 * @icon_view: A #GtkIconView.
 * @column: A column in the currently used model, or -1 to display no text
 * 
 * Sets the column with text for @icon_view to be @column. The text
 * column must be of type #G_TYPE_STRING.
 *
 * Since: 2.6 
 **/
void
__gtk_icon_view_set_text_column (GtkIconView *icon_view,
			       gint          column)
{
  if (column == gtk_icon_view_get_props (icon_view)->priv->text_column)
    return;
  
  if (column == -1)
    gtk_icon_view_get_props (icon_view)->priv->text_column = -1;
  else
    {
      if (gtk_icon_view_get_props (icon_view)->priv->model != NULL)
	{
	  GType column_type;
	  
	  column_type = __gtk_tree_model_get_column_type (gtk_icon_view_get_props (icon_view)->priv->model, column);

	  g_return_if_fail (column_type == G_TYPE_STRING);
	}
      
      gtk_icon_view_get_props (icon_view)->priv->text_column = column;
    }

  gtk_icon_view_stop_editing (icon_view, TRUE);

  update_text_cell (icon_view);

  gtk_icon_view_invalidate_sizes (icon_view);
  gtk_icon_view_queue_layout (icon_view);
  
  g_object_notify (G_OBJECT (icon_view), "text-column");
}

/**
 * __gtk_icon_view_get_text_column:
 * @icon_view: A #GtkIconView.
 *
 * Returns the column with text for @icon_view.
 *
 * Returns: the text column, or -1 if it's unset.
 *
 * Since: 2.6
 */
gint
__gtk_icon_view_get_text_column (GtkIconView  *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), -1);

  return gtk_icon_view_get_props (icon_view)->priv->text_column;
}

/**
 * __gtk_icon_view_set_markup_column:
 * @icon_view: A #GtkIconView.
 * @column: A column in the currently used model, or -1 to display no text
 * 
 * Sets the column with markup information for @icon_view to be
 * @column. The markup column must be of type #G_TYPE_STRING.
 * If the markup column is set to something, it overrides
 * the text column set by __gtk_icon_view_set_text_column().
 *
 * Since: 2.6
 **/
void
__gtk_icon_view_set_markup_column (GtkIconView *icon_view,
				 gint         column)
{
  if (column == gtk_icon_view_get_props (icon_view)->priv->markup_column)
    return;
  
  if (column == -1)
    gtk_icon_view_get_props (icon_view)->priv->markup_column = -1;
  else
    {
      if (gtk_icon_view_get_props (icon_view)->priv->model != NULL)
	{
	  GType column_type;
	  
	  column_type = __gtk_tree_model_get_column_type (gtk_icon_view_get_props (icon_view)->priv->model, column);

	  g_return_if_fail (column_type == G_TYPE_STRING);
	}
      
      gtk_icon_view_get_props (icon_view)->priv->markup_column = column;
    }

  gtk_icon_view_stop_editing (icon_view, TRUE);

  update_text_cell (icon_view);

  gtk_icon_view_invalidate_sizes (icon_view);
  gtk_icon_view_queue_layout (icon_view);
  
  g_object_notify (G_OBJECT (icon_view), "markup-column");
}

/**
 * __gtk_icon_view_get_markup_column:
 * @icon_view: A #GtkIconView.
 *
 * Returns the column with markup text for @icon_view.
 *
 * Returns: the markup column, or -1 if it's unset.
 *
 * Since: 2.6
 */
gint
__gtk_icon_view_get_markup_column (GtkIconView  *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), -1);

  return gtk_icon_view_get_props (icon_view)->priv->markup_column;
}

/**
 * __gtk_icon_view_set_pixbuf_column:
 * @icon_view: A #GtkIconView.
 * @column: A column in the currently used model, or -1 to disable
 * 
 * Sets the column with pixbufs for @icon_view to be @column. The pixbuf
 * column must be of type #GDK_TYPE_PIXBUF
 *
 * Since: 2.6 
 **/
void
__gtk_icon_view_set_pixbuf_column (GtkIconView *icon_view,
				 gint         column)
{
  if (column == gtk_icon_view_get_props (icon_view)->priv->pixbuf_column)
    return;
  
  if (column == -1)
    gtk_icon_view_get_props (icon_view)->priv->pixbuf_column = -1;
  else
    {
      if (gtk_icon_view_get_props (icon_view)->priv->model != NULL)
	{
	  GType column_type;
	  
	  column_type = __gtk_tree_model_get_column_type (gtk_icon_view_get_props (icon_view)->priv->model, column);

	  g_return_if_fail (column_type == GDK_TYPE_PIXBUF);
	}
      
      gtk_icon_view_get_props (icon_view)->priv->pixbuf_column = column;
    }

  gtk_icon_view_stop_editing (icon_view, TRUE);

  update_pixbuf_cell (icon_view);

  gtk_icon_view_invalidate_sizes (icon_view);
  gtk_icon_view_queue_layout (icon_view);
  
  g_object_notify (G_OBJECT (icon_view), "pixbuf-column");
  
}

/**
 * __gtk_icon_view_get_pixbuf_column:
 * @icon_view: A #GtkIconView.
 *
 * Returns the column with pixbufs for @icon_view.
 *
 * Returns: the pixbuf column, or -1 if it's unset.
 *
 * Since: 2.6
 */
gint
__gtk_icon_view_get_pixbuf_column (GtkIconView  *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), -1);

  return gtk_icon_view_get_props (icon_view)->priv->pixbuf_column;
}

/**
 * __gtk_icon_view_select_path:
 * @icon_view: A #GtkIconView.
 * @path: The #GtkTreePath to be selected.
 * 
 * Selects the row at @path.
 *
 * Since: 2.6
 **/
void
__gtk_icon_view_select_path (GtkIconView *icon_view,
			   GtkTreePath *path)
{
  GtkIconViewItem *item = NULL;

  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  g_return_if_fail (gtk_icon_view_get_props (icon_view)->priv->model != NULL);
  g_return_if_fail (path != NULL);

  if (__gtk_tree_path_get_depth (path) > 0)
    item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items,
			    __gtk_tree_path_get_indices(path)[0]);

  if (item)
    gtk_icon_view_select_item (icon_view, item);
}

/**
 * __gtk_icon_view_unselect_path:
 * @icon_view: A #GtkIconView.
 * @path: The #GtkTreePath to be unselected.
 * 
 * Unselects the row at @path.
 *
 * Since: 2.6
 **/
void
__gtk_icon_view_unselect_path (GtkIconView *icon_view,
			     GtkTreePath *path)
{
  GtkIconViewItem *item;
  
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  g_return_if_fail (gtk_icon_view_get_props (icon_view)->priv->model != NULL);
  g_return_if_fail (path != NULL);

  item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items,
			  __gtk_tree_path_get_indices(path)[0]);

  if (!item)
    return;
  
  gtk_icon_view_unselect_item (icon_view, item);
}

/**
 * __gtk_icon_view_get_selected_items:
 * @icon_view: A #GtkIconView.
 *
 * Creates a list of paths of all selected items. Additionally, if you are
 * planning on modifying the model after calling this function, you may
 * want to convert the returned list into a list of #GtkTreeRowReference<!-- -->s.
 * To do this, you can use __gtk_tree_row_reference_new().
 *
 * To free the return value, use:
 * |[
 * g_list_foreach (list, (GFunc)__gtk_tree_path_free, NULL);
 * g_list_free (list);
 * ]|
 *
 * Return value: (element-type GtkTreePath) (transfer full): A #GList containing a #GtkTreePath for each selected row.
 *
 * Since: 2.6
 **/
GList *
__gtk_icon_view_get_selected_items (GtkIconView *icon_view)
{
  GList *list;
  GList *selected = NULL;
  
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), NULL);
  
  for (list = gtk_icon_view_get_props (icon_view)->priv->items; list != NULL; list = list->next)
    {
      GtkIconViewItem *item = list->data;

      if (item->selected)
	{
	  GtkTreePath *path = __gtk_tree_path_new_from_indices (item->index, -1);

	  selected = g_list_prepend (selected, path);
	}
    }

  return selected;
}

/**
 * __gtk_icon_view_select_all:
 * @icon_view: A #GtkIconView.
 * 
 * Selects all the icons. @icon_view must has its selection mode set
 * to #GTK_SELECTION_MULTIPLE.
 *
 * Since: 2.6
 **/
void
__gtk_icon_view_select_all (GtkIconView *icon_view)
{
  GList *items;
  gboolean dirty = FALSE;
  
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  if (gtk_icon_view_get_props (icon_view)->priv->selection_mode != GTK_SELECTION_MULTIPLE)
    return;

  for (items = gtk_icon_view_get_props (icon_view)->priv->items; items; items = items->next)
    {
      GtkIconViewItem *item = items->data;
      
      if (!item->selected)
	{
	  dirty = TRUE;
	  item->selected = TRUE;
	  gtk_icon_view_queue_draw_item (icon_view, item);
	}
    }

  if (dirty)
    g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0);
}

/**
 * __gtk_icon_view_unselect_all:
 * @icon_view: A #GtkIconView.
 * 
 * Unselects all the icons.
 *
 * Since: 2.6
 **/
void
__gtk_icon_view_unselect_all (GtkIconView *icon_view)
{
  gboolean dirty = FALSE;
  
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  if (gtk_icon_view_get_props (icon_view)->priv->selection_mode == GTK_SELECTION_BROWSE)
    return;

  dirty = __gtk_icon_view_unselect_all_internal (icon_view);

  if (dirty)
    g_signal_emit (icon_view, icon_view_signals[SELECTION_CHANGED], 0);
}

/**
 * __gtk_icon_view_path_is_selected:
 * @icon_view: A #GtkIconView.
 * @path: A #GtkTreePath to check selection on.
 * 
 * Returns %TRUE if the icon pointed to by @path is currently
 * selected. If @path does not point to a valid location, %FALSE is returned.
 * 
 * Return value: %TRUE if @path is selected.
 *
 * Since: 2.6
 **/
gboolean
__gtk_icon_view_path_is_selected (GtkIconView *icon_view,
				GtkTreePath *path)
{
  GtkIconViewItem *item;
  
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), FALSE);
  g_return_val_if_fail (gtk_icon_view_get_props (icon_view)->priv->model != NULL, FALSE);
  g_return_val_if_fail (path != NULL, FALSE);
  
  item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items,
			  __gtk_tree_path_get_indices(path)[0]);

  if (!item)
    return FALSE;
  
  return item->selected;
}

/**
 * __gtk_icon_view_get_item_row:
 * @icon_view: a #GtkIconView
 * @path: the #GtkTreePath of the item
 *
 * Gets the row in which the item @path is currently
 * displayed. Row numbers start at 0.
 *
 * Returns: The row in which the item is displayed
 *
 * Since: 2.22
 */
gint
__gtk_icon_view_get_item_row (GtkIconView *icon_view,
                            GtkTreePath *path)
{
  GtkIconViewItem *item;

  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), FALSE);
  g_return_val_if_fail (gtk_icon_view_get_props (icon_view)->priv->model != NULL, FALSE);
  g_return_val_if_fail (path != NULL, FALSE);

  item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items,
                          __gtk_tree_path_get_indices(path)[0]);

  if (!item)
    return -1;

  return item->row;
}

/**
 * __gtk_icon_view_get_item_column:
 * @icon_view: a #GtkIconView
 * @path: the #GtkTreePath of the item
 *
 * Gets the column in which the item @path is currently
 * displayed. Column numbers start at 0.
 *
 * Returns: The column in which the item is displayed
 *
 * Since: 2.22
 */
gint
__gtk_icon_view_get_item_column (GtkIconView *icon_view,
                               GtkTreePath *path)
{
  GtkIconViewItem *item;

  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), FALSE);
  g_return_val_if_fail (gtk_icon_view_get_props (icon_view)->priv->model != NULL, FALSE);
  g_return_val_if_fail (path != NULL, FALSE);

  item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items,
                          __gtk_tree_path_get_indices(path)[0]);

  if (!item)
    return -1;

  return item->col;
}

/**
 * __gtk_icon_view_item_activated:
 * @icon_view: A #GtkIconView
 * @path: The #GtkTreePath to be activated
 * 
 * Activates the item determined by @path.
 *
 * Since: 2.6
 **/
void
__gtk_icon_view_item_activated (GtkIconView      *icon_view,
			      GtkTreePath      *path)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  g_return_if_fail (path != NULL);
  
  g_signal_emit (icon_view, icon_view_signals[ITEM_ACTIVATED], 0, path);
}

/**
 * __gtk_icon_view_set_item_orientation:
 * @icon_view: a #GtkIconView
 * @orientation: the relative position of texts and icons 
 * 
 * Sets the ::item-orientation property which determines whether
 * the labels are drawn beside the icons instead of below.
 *
 * Since: 2.22
 **/
void 
__gtk_icon_view_set_item_orientation (GtkIconView    *icon_view,
                                    GtkOrientation  orientation)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  if (gtk_icon_view_get_props (icon_view)->priv->item_orientation != orientation)
    {
      gtk_icon_view_get_props (icon_view)->priv->item_orientation = orientation;

      gtk_icon_view_stop_editing (icon_view, TRUE);
      gtk_icon_view_invalidate_sizes (icon_view);
      gtk_icon_view_queue_layout (icon_view);

      update_text_cell (icon_view);
      update_pixbuf_cell (icon_view);
      
      g_object_notify (G_OBJECT (icon_view), "item-orientation");
      g_object_notify (G_OBJECT (icon_view), "orientation");
    }
}

/**
 * __gtk_icon_view_set_orientation:
 * @icon_view: a #GtkIconView
 * @orientation: the relative position of texts and icons 
 * 
 * Sets the ::orientation property which determines whether the labels 
 * are drawn beside the icons instead of below.
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use __gtk_icon_view_set_item_orientation()
 **/
void 
__gtk_icon_view_set_orientation (GtkIconView    *icon_view,
                               GtkOrientation  orientation)
{
  __gtk_icon_view_set_item_orientation (icon_view, orientation);
}

/**
 * __gtk_icon_view_get_item_orientation:
 * @icon_view: a #GtkIconView
 *
 * Returns the value of the ::item-orientation property which determines
 * whether the labels are drawn beside the icons instead of below.
 *
 * Return value: the relative position of texts and icons
 *
 * Since: 2.22
 */
GtkOrientation
__gtk_icon_view_get_item_orientation (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view),
			GTK_ORIENTATION_VERTICAL);

  return gtk_icon_view_get_props (icon_view)->priv->item_orientation;
}

/**
 * __gtk_icon_view_get_orientation:
 * @icon_view: a #GtkIconView
 * 
 * Returns the value of the ::orientation property which determines 
 * whether the labels are drawn beside the icons instead of below. 
 * 
 * Return value: the relative position of texts and icons 
 *
 * Since: 2.6
 *
 * Deprecated: 2.22: Use __gtk_icon_view_get_item_orientation()
 **/
GtkOrientation
__gtk_icon_view_get_orientation (GtkIconView *icon_view)
{
  return __gtk_icon_view_get_item_orientation (icon_view);
}

/**
 * __gtk_icon_view_set_columns:
 * @icon_view: a #GtkIconView
 * @columns: the number of columns
 * 
 * Sets the ::columns property which determines in how
 * many columns the icons are arranged. If @columns is
 * -1, the number of columns will be chosen automatically 
 * to fill the available area. 
 *
 * Since: 2.6
 */
void 
__gtk_icon_view_set_columns (GtkIconView *icon_view,
			   gint         columns)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  
  if (gtk_icon_view_get_props (icon_view)->priv->columns != columns)
    {
      gtk_icon_view_get_props (icon_view)->priv->columns = columns;

      gtk_icon_view_stop_editing (icon_view, TRUE);
      gtk_icon_view_queue_layout (icon_view);
      
      g_object_notify (G_OBJECT (icon_view), "columns");
    }  
}

/**
 * __gtk_icon_view_get_columns:
 * @icon_view: a #GtkIconView
 * 
 * Returns the value of the ::columns property.
 * 
 * Return value: the number of columns, or -1
 *
 * Since: 2.6
 */
gint
__gtk_icon_view_get_columns (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), -1);

  return gtk_icon_view_get_props (icon_view)->priv->columns;
}

/**
 * __gtk_icon_view_set_item_width:
 * @icon_view: a #GtkIconView
 * @item_width: the width for each item
 * 
 * Sets the ::item-width property which specifies the width 
 * to use for each item. If it is set to -1, the icon view will 
 * automatically determine a suitable item size.
 *
 * Since: 2.6
 */
void 
__gtk_icon_view_set_item_width (GtkIconView *icon_view,
			      gint         item_width)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  
  if (gtk_icon_view_get_props (icon_view)->priv->item_width != item_width)
    {
      gtk_icon_view_get_props (icon_view)->priv->item_width = item_width;
      
      gtk_icon_view_stop_editing (icon_view, TRUE);
      gtk_icon_view_invalidate_sizes (icon_view);
      gtk_icon_view_queue_layout (icon_view);
      
      update_text_cell (icon_view);

      g_object_notify (G_OBJECT (icon_view), "item-width");
    }  
}

/**
 * __gtk_icon_view_get_item_width:
 * @icon_view: a #GtkIconView
 * 
 * Returns the value of the ::item-width property.
 * 
 * Return value: the width of a single item, or -1
 *
 * Since: 2.6
 */
gint
__gtk_icon_view_get_item_width (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), -1);

  return gtk_icon_view_get_props (icon_view)->priv->item_width;
}


/**
 * __gtk_icon_view_set_spacing:
 * @icon_view: a #GtkIconView
 * @spacing: the spacing
 * 
 * Sets the ::spacing property which specifies the space 
 * which is inserted between the cells (i.e. the icon and 
 * the text) of an item.
 *
 * Since: 2.6
 */
void 
__gtk_icon_view_set_spacing (GtkIconView *icon_view,
			   gint         spacing)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  
  if (gtk_icon_view_get_props (icon_view)->priv->spacing != spacing)
    {
      gtk_icon_view_get_props (icon_view)->priv->spacing = spacing;

      gtk_icon_view_stop_editing (icon_view, TRUE);
      gtk_icon_view_invalidate_sizes (icon_view);
      gtk_icon_view_queue_layout (icon_view);
      
      g_object_notify (G_OBJECT (icon_view), "spacing");
    }  
}

/**
 * __gtk_icon_view_get_spacing:
 * @icon_view: a #GtkIconView
 * 
 * Returns the value of the ::spacing property.
 * 
 * Return value: the space between cells 
 *
 * Since: 2.6
 */
gint
__gtk_icon_view_get_spacing (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), -1);

  return gtk_icon_view_get_props (icon_view)->priv->spacing;
}

/**
 * __gtk_icon_view_set_row_spacing:
 * @icon_view: a #GtkIconView
 * @row_spacing: the row spacing
 * 
 * Sets the ::row-spacing property which specifies the space 
 * which is inserted between the rows of the icon view.
 *
 * Since: 2.6
 */
void 
__gtk_icon_view_set_row_spacing (GtkIconView *icon_view,
			       gint         row_spacing)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  
  if (gtk_icon_view_get_props (icon_view)->priv->row_spacing != row_spacing)
    {
      gtk_icon_view_get_props (icon_view)->priv->row_spacing = row_spacing;

      gtk_icon_view_stop_editing (icon_view, TRUE);
      gtk_icon_view_invalidate_sizes (icon_view);
      gtk_icon_view_queue_layout (icon_view);
      
      g_object_notify (G_OBJECT (icon_view), "row-spacing");
    }  
}

/**
 * __gtk_icon_view_get_row_spacing:
 * @icon_view: a #GtkIconView
 * 
 * Returns the value of the ::row-spacing property.
 * 
 * Return value: the space between rows
 *
 * Since: 2.6
 */
gint
__gtk_icon_view_get_row_spacing (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), -1);

  return gtk_icon_view_get_props (icon_view)->priv->row_spacing;
}

/**
 * __gtk_icon_view_set_column_spacing:
 * @icon_view: a #GtkIconView
 * @column_spacing: the column spacing
 * 
 * Sets the ::column-spacing property which specifies the space 
 * which is inserted between the columns of the icon view.
 *
 * Since: 2.6
 */
void 
__gtk_icon_view_set_column_spacing (GtkIconView *icon_view,
				  gint         column_spacing)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  
  if (gtk_icon_view_get_props (icon_view)->priv->column_spacing != column_spacing)
    {
      gtk_icon_view_get_props (icon_view)->priv->column_spacing = column_spacing;

      gtk_icon_view_stop_editing (icon_view, TRUE);
      gtk_icon_view_invalidate_sizes (icon_view);
      gtk_icon_view_queue_layout (icon_view);
      
      g_object_notify (G_OBJECT (icon_view), "column-spacing");
    }  
}

/**
 * __gtk_icon_view_get_column_spacing:
 * @icon_view: a #GtkIconView
 * 
 * Returns the value of the ::column-spacing property.
 * 
 * Return value: the space between columns
 *
 * Since: 2.6
 */
gint
__gtk_icon_view_get_column_spacing (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), -1);

  return gtk_icon_view_get_props (icon_view)->priv->column_spacing;
}

/**
 * __gtk_icon_view_set_margin:
 * @icon_view: a #GtkIconView
 * @margin: the margin
 * 
 * Sets the ::margin property which specifies the space 
 * which is inserted at the top, bottom, left and right 
 * of the icon view.
 *
 * Since: 2.6
 */
void 
__gtk_icon_view_set_margin (GtkIconView *icon_view,
			  gint         margin)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  
  if (gtk_icon_view_get_props (icon_view)->priv->margin != margin)
    {
      gtk_icon_view_get_props (icon_view)->priv->margin = margin;

      gtk_icon_view_stop_editing (icon_view, TRUE);
      gtk_icon_view_invalidate_sizes (icon_view);
      gtk_icon_view_queue_layout (icon_view);
      
      g_object_notify (G_OBJECT (icon_view), "margin");
    }  
}

/**
 * __gtk_icon_view_get_margin:
 * @icon_view: a #GtkIconView
 * 
 * Returns the value of the ::margin property.
 * 
 * Return value: the space at the borders 
 *
 * Since: 2.6
 */
gint
__gtk_icon_view_get_margin (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), -1);

  return gtk_icon_view_get_props (icon_view)->priv->margin;
}

/**
 * __gtk_icon_view_set_item_padding:
 * @icon_view: a #GtkIconView
 * @item_padding: the item padding
 *
 * Sets the #GtkIconView:item-padding property which specifies the padding
 * around each of the icon view's items.
 *
 * Since: 2.18
 */
void
__gtk_icon_view_set_item_padding (GtkIconView *icon_view,
				gint         item_padding)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));
  
  if (gtk_icon_view_get_props (icon_view)->priv->item_padding != item_padding)
    {
      gtk_icon_view_get_props (icon_view)->priv->item_padding = item_padding;

      gtk_icon_view_stop_editing (icon_view, TRUE);
      gtk_icon_view_invalidate_sizes (icon_view);
      gtk_icon_view_queue_layout (icon_view);
      
      g_object_notify (G_OBJECT (icon_view), "item-padding");
    }  
}

/**
 * __gtk_icon_view_get_item_padding:
 * @icon_view: a #GtkIconView
 * 
 * Returns the value of the ::item-padding property.
 * 
 * Return value: the padding around items
 *
 * Since: 2.18
 */
gint
__gtk_icon_view_get_item_padding (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), -1);

  return gtk_icon_view_get_props (icon_view)->priv->item_padding;
}

/* Get/set whether drag_motion requested the drag data and
 * drag_data_received should thus not actually insert the data,
 * since the data doesn't result from a drop.
 */
static void
set_status_pending (GdkDragContext *context,
                    GdkDragAction   suggested_action)
{
  g_object_set_data (G_OBJECT (context),
                     I_("gtk-icon-view-status-pending"),
                     GINT_TO_POINTER (suggested_action));
}

static GdkDragAction
get_status_pending (GdkDragContext *context)
{
  return GPOINTER_TO_INT (g_object_get_data (G_OBJECT (context),
                                             "gtk-icon-view-status-pending"));
}

static void
unset_reorderable (GtkIconView *icon_view)
{
  if (gtk_icon_view_get_props (icon_view)->priv->reorderable)
    {
      gtk_icon_view_get_props (icon_view)->priv->reorderable = FALSE;
      g_object_notify (G_OBJECT (icon_view), "reorderable");
    }
}

static void
set_source_row (GdkDragContext *context,
                GtkTreeModel   *model,
                GtkTreePath    *source_row)
{
  if (source_row)
    g_object_set_data_full (G_OBJECT (context),
			    I_("gtk-icon-view-source-row"),
			    __gtk_tree_row_reference_new (model, source_row),
			    (GDestroyNotify) __gtk_tree_row_reference_free);
  else
    g_object_set_data_full (G_OBJECT (context),
			    I_("gtk-icon-view-source-row"),
			    NULL, NULL);
}

static GtkTreePath*
get_source_row (GdkDragContext *context)
{
  GtkTreeRowReference *ref;

  ref = g_object_get_data (G_OBJECT (context), "gtk-icon-view-source-row");

  if (ref)
    return __gtk_tree_row_reference_get_path (ref);
  else
    return NULL;
}

typedef struct
{
  GtkTreeRowReference *dest_row;
  gboolean             empty_view_drop;
  gboolean             drop_append_mode;
} DestRow;

static void
dest_row_free (gpointer data)
{
  DestRow *dr = (DestRow *)data;

  __gtk_tree_row_reference_free (dr->dest_row);
  g_free (dr);
}

static void
set_dest_row (GdkDragContext *context,
	      GtkTreeModel   *model,
	      GtkTreePath    *dest_row,
	      gboolean        empty_view_drop,
	      gboolean        drop_append_mode)
{
  DestRow *dr;

  if (!dest_row)
    {
      g_object_set_data_full (G_OBJECT (context),
			      I_("gtk-icon-view-dest-row"),
			      NULL, NULL);
      return;
    }
  
  dr = g_new0 (DestRow, 1);
     
  dr->dest_row = __gtk_tree_row_reference_new (model, dest_row);
  dr->empty_view_drop = empty_view_drop;
  dr->drop_append_mode = drop_append_mode;
  g_object_set_data_full (G_OBJECT (context),
                          I_("gtk-icon-view-dest-row"),
                          dr, (GDestroyNotify) dest_row_free);
}

static GtkTreePath*
get_dest_row (GdkDragContext *context)
{
  DestRow *dr;

  dr = g_object_get_data (G_OBJECT (context), "gtk-icon-view-dest-row");

  if (dr)
    {
      GtkTreePath *path = NULL;
      
      if (dr->dest_row)
	path = __gtk_tree_row_reference_get_path (dr->dest_row);
      else if (dr->empty_view_drop)
	path = __gtk_tree_path_new_from_indices (0, -1);
      else
	path = NULL;

      if (path && dr->drop_append_mode)
	__gtk_tree_path_next (path);

      return path;
    }
  else
    return NULL;
}

static gboolean
check_model_dnd (GtkTreeModel *model,
                 GType         required_iface,
                 const gchar  *signal)
{
  if (model == NULL || !G_TYPE_CHECK_INSTANCE_TYPE ((model), required_iface))
    {
      g_warning ("You must override the default '%s' handler "
                 "on GtkIconView when using models that don't support "
                 "the %s interface and enabling drag-and-drop. The simplest way to do this "
                 "is to connect to '%s' and call "
                 "g_signal_stop_emission_by_name() in your signal handler to prevent "
                 "the default handler from running. Look at the source code "
                 "for the default handler in gtkiconview.c to get an idea what "
                 "your handler should do. (gtkiconview.c is in the GTK+ source "
                 "code.) If you're using GTK+ from a language other than C, "
                 "there may be a more natural way to override default handlers, e.g. via derivation.",
                 signal, g_type_name (required_iface), signal);
      return FALSE;
    }
  else
    return TRUE;
}

static void
remove_scroll_timeout (GtkIconView *icon_view)
{
  if (gtk_icon_view_get_props (icon_view)->priv->scroll_timeout_id != 0)
    {
      g_source_remove (gtk_icon_view_get_props (icon_view)->priv->scroll_timeout_id);

      gtk_icon_view_get_props (icon_view)->priv->scroll_timeout_id = 0;
    }
}

static void
gtk_icon_view_autoscroll (GtkIconView *icon_view)
{
  gint px, py, width, height;
  gint hoffset, voffset;
  gfloat value;

  __gdk_window_get_pointer (gtk_widget_get_props (GTK_WIDGET (icon_view))->window, &px, &py, NULL);
  __gdk_window_get_geometry (gtk_widget_get_props (GTK_WIDGET (icon_view))->window, NULL, NULL, &width, &height, NULL);

  /* see if we are near the edge. */
  voffset = py - 2 * SCROLL_EDGE_SIZE;
  if (voffset > 0)
    voffset = MAX (py - (height - 2 * SCROLL_EDGE_SIZE), 0);

  hoffset = px - 2 * SCROLL_EDGE_SIZE;
  if (hoffset > 0)
    hoffset = MAX (px - (width - 2 * SCROLL_EDGE_SIZE), 0);

  if (voffset != 0)
    {
      value = CLAMP (gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->value + voffset, 
		     gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->lower,
		     gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->upper - gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->page_size);
      __gtk_adjustment_set_value (gtk_icon_view_get_props (icon_view)->priv->vadjustment, value);
    }
  if (hoffset != 0)
    {
      value = CLAMP (gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->hadjustment)->value + hoffset, 
		     gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->hadjustment)->lower,
		     gtk_adjustment_get_props (gtk_icon_view_get_instance_private (icon_view)->hadjustment)->upper - gtk_adjustment_get_props (gtk_icon_view_get_instance_private (icon_view)->hadjustment)->page_size);
      __gtk_adjustment_set_value (gtk_icon_view_get_props (icon_view)->priv->hadjustment, value);
    }
}


static gboolean
drag_scroll_timeout (gpointer data)
{
  GtkIconView *icon_view = GTK_ICON_VIEW (data);

  gtk_icon_view_autoscroll (icon_view);

  return TRUE;
}


static gboolean
set_destination (GtkIconView    *icon_view,
		 GdkDragContext *context,
		 gint            x,
		 gint            y,
		 GdkDragAction  *suggested_action,
		 GdkAtom        *target)
{
  GtkWidget *widget;
  GtkTreePath *path = NULL;
  GtkIconViewDropPosition pos;
  GtkIconViewDropPosition old_pos;
  GtkTreePath *old_dest_path = NULL;
  gboolean can_drop = FALSE;

  widget = GTK_WIDGET (icon_view);

  *suggested_action = 0;
  *target = GDK_NONE;

  if (!gtk_icon_view_get_props (icon_view)->priv->dest_set)
    {
      /* someone unset us as a drag dest, note that if
       * we return FALSE drag_leave isn't called
       */

      __gtk_icon_view_set_drag_dest_item (icon_view,
					NULL,
					GTK_ICON_VIEW_DROP_LEFT);

      remove_scroll_timeout (GTK_ICON_VIEW (widget));

      return FALSE; /* no longer a drop site */
    }

  *target = __gtk_drag_dest_find_target (widget, context,
                                       __gtk_drag_dest_get_target_list (widget));
  if (*target == GDK_NONE)
    return FALSE;

  if (!__gtk_icon_view_get_dest_item_at_pos (icon_view, x, y, &path, &pos)) 
    {
      gint n_children;
      GtkTreeModel *model;
      
      /* the row got dropped on empty space, let's setup a special case
       */

      if (path)
	__gtk_tree_path_free (path);

      model = __gtk_icon_view_get_model (icon_view);

      n_children = __gtk_tree_model_iter_n_children (model, NULL);
      if (n_children)
        {
          pos = GTK_ICON_VIEW_DROP_BELOW;
          path = __gtk_tree_path_new_from_indices (n_children - 1, -1);
        }
      else
        {
          pos = GTK_ICON_VIEW_DROP_ABOVE;
          path = __gtk_tree_path_new_from_indices (0, -1);
        }

      can_drop = TRUE;

      goto out;
    }

  g_assert (path);

  __gtk_icon_view_get_drag_dest_item (icon_view,
				    &old_dest_path,
				    &old_pos);
  
  if (old_dest_path)
    __gtk_tree_path_free (old_dest_path);
  
  if (TRUE /* FIXME if the location droppable predicate */)
    {
      can_drop = TRUE;
    }

out:
  if (can_drop)
    {
      GtkWidget *source_widget;

      *suggested_action = __gdk_drag_context_get_suggested_action (context);
      source_widget = __gtk_drag_get_source_widget (context);

      if (source_widget == widget)
        {
          /* Default to MOVE, unless the user has
           * pressed ctrl or shift to affect available actions
           */
          if ((__gdk_drag_context_get_actions (context) & GDK_ACTION_MOVE) != 0)
            *suggested_action = GDK_ACTION_MOVE;
        }

      __gtk_icon_view_set_drag_dest_item (GTK_ICON_VIEW (widget),
					path, pos);
    }
  else
    {
      /* can't drop here */
      __gtk_icon_view_set_drag_dest_item (GTK_ICON_VIEW (widget),
					NULL,
					GTK_ICON_VIEW_DROP_LEFT);
    }
  
  if (path)
    __gtk_tree_path_free (path);
  
  return TRUE;
}

static GtkTreePath*
get_logical_destination (GtkIconView *icon_view,
			 gboolean    *drop_append_mode)
{
  /* adjust path to point to the row the drop goes in front of */
  GtkTreePath *path = NULL;
  GtkIconViewDropPosition pos;
  
  *drop_append_mode = FALSE;

  __gtk_icon_view_get_drag_dest_item (icon_view, &path, &pos);

  if (path == NULL)
    return NULL;

  if (pos == GTK_ICON_VIEW_DROP_RIGHT || 
      pos == GTK_ICON_VIEW_DROP_BELOW)
    {
      GtkTreeIter iter;
      GtkTreeModel *model = gtk_icon_view_get_props (icon_view)->priv->model;

      if (!__gtk_tree_model_get_iter (model, &iter, path) ||
          !__gtk_tree_model_iter_next (model, &iter))
        *drop_append_mode = TRUE;
      else
        {
          *drop_append_mode = FALSE;
          __gtk_tree_path_next (path);
        }      
    }

  return path;
}

static gboolean
gtk_icon_view_maybe_begin_drag (GtkIconView    *icon_view,
				GdkEventMotion *event)
{
  GtkWidget *widget = GTK_WIDGET (icon_view);
  GdkDragContext *context;
  GtkTreePath *path = NULL;
  gint button;
  GtkTreeModel *model;
  gboolean retval = FALSE;

  if (!gtk_icon_view_get_props (icon_view)->priv->source_set)
    goto out;

  if (gtk_icon_view_get_props (icon_view)->priv->pressed_button < 0)
    goto out;

  if (!__gtk_drag_check_threshold (GTK_WIDGET (icon_view),
                                 gtk_icon_view_get_props (icon_view)->priv->press_start_x,
                                 gtk_icon_view_get_props (icon_view)->priv->press_start_y,
                                 event->x, event->y))
    goto out;

  model = __gtk_icon_view_get_model (icon_view);

  if (model == NULL)
    goto out;

  button = gtk_icon_view_get_props (icon_view)->priv->pressed_button;
  gtk_icon_view_get_props (icon_view)->priv->pressed_button = -1;

  path = __gtk_icon_view_get_path_at_pos (icon_view,
					gtk_icon_view_get_props (icon_view)->priv->press_start_x,
					gtk_icon_view_get_props (icon_view)->priv->press_start_y);

  if (path == NULL)
    goto out;

  if (!GTK_IS_TREE_DRAG_SOURCE (model) ||
      !__gtk_tree_drag_source_row_draggable (GTK_TREE_DRAG_SOURCE (model),
					   path))
    goto out;

  /* FIXME Check whether we're a start button, if not return FALSE and
   * free path
   */

  /* Now we can begin the drag */
  
  retval = TRUE;

  context = __gtk_drag_begin (widget,
                            __gtk_drag_source_get_target_list (widget),
                            gtk_icon_view_get_props (icon_view)->priv->source_actions,
                            button,
                            (GdkEvent*)event);

  set_source_row (context, model, path);
  
 out:
  if (path)
    __gtk_tree_path_free (path);

  return retval;
}

/* Source side drag signals */
static void 
gtk_icon_view_drag_begin (GtkWidget      *widget,
			  GdkDragContext *context)
{
  GtkIconView *icon_view;
  GtkIconViewItem *item;
  GdkPixmap *icon;
  gint x, y;
  GtkTreePath *path;

  icon_view = GTK_ICON_VIEW (widget);

  /* if the user uses a custom DnD impl, we don't set the icon here */
  if (!gtk_icon_view_get_props (icon_view)->priv->dest_set && !gtk_icon_view_get_props (icon_view)->priv->source_set)
    return;

  item = gtk_icon_view_get_item_at_coords (icon_view,
					   gtk_icon_view_get_props (icon_view)->priv->press_start_x,
					   gtk_icon_view_get_props (icon_view)->priv->press_start_y,
					   TRUE,
					   NULL);

  g_return_if_fail (item != NULL);

  x = gtk_icon_view_get_props (icon_view)->priv->press_start_x - item->x + 1;
  y = gtk_icon_view_get_props (icon_view)->priv->press_start_y - item->y + 1;
  
  path = __gtk_tree_path_new_from_indices (item->index, -1);
  icon = __gtk_icon_view_create_drag_icon (icon_view, path);
  __gtk_tree_path_free (path);

  __gtk_drag_set_icon_pixmap (context, 
			    __gdk_drawable_get_colormap ((GdkDrawable *) icon),
			    icon, 
			    NULL, 
			    x, y);

  g_object_unref (icon);
}

static void 
gtk_icon_view_drag_end (GtkWidget      *widget,
			GdkDragContext *context)
{
  /* do nothing */
}

static void 
gtk_icon_view_drag_data_get (GtkWidget        *widget,
			     GdkDragContext   *context,
			     GtkSelectionData *selection_data,
			     guint             info,
			     guint             time)
{
  GtkIconView *icon_view;
  GtkTreeModel *model;
  GtkTreePath *source_row;

  icon_view = GTK_ICON_VIEW (widget);
  model = __gtk_icon_view_get_model (icon_view);

  if (model == NULL)
    return;

  if (!gtk_icon_view_get_props (icon_view)->priv->source_set)
    return;

  source_row = get_source_row (context);

  if (source_row == NULL)
    return;

  /* We can implement the GTK_TREE_MODEL_ROW target generically for
   * any model; for DragSource models there are some other targets
   * we also support.
   */

  if (GTK_IS_TREE_DRAG_SOURCE (model) &&
      __gtk_tree_drag_source_drag_data_get (GTK_TREE_DRAG_SOURCE (model),
                                          source_row,
                                          selection_data))
    goto done;

  /* If drag_data_get does nothing, try providing row data. */
  if (selection_data->target == __gdk_atom_intern_static_string ("GTK_TREE_MODEL_ROW"))
    __gtk_tree_set_row_drag_data (selection_data,
				model,
				source_row);

 done:
  __gtk_tree_path_free (source_row);
}

static void 
gtk_icon_view_drag_data_delete (GtkWidget      *widget,
				GdkDragContext *context)
{
  GtkTreeModel *model;
  GtkIconView *icon_view;
  GtkTreePath *source_row;

  icon_view = GTK_ICON_VIEW (widget);
  model = __gtk_icon_view_get_model (icon_view);

  if (!check_model_dnd (model, GTK_TYPE_TREE_DRAG_SOURCE, "drag-data-delete"))
    return;

  if (!gtk_icon_view_get_props (icon_view)->priv->source_set)
    return;

  source_row = get_source_row (context);

  if (source_row == NULL)
    return;

  __gtk_tree_drag_source_drag_data_delete (GTK_TREE_DRAG_SOURCE (model),
                                         source_row);

  __gtk_tree_path_free (source_row);

  set_source_row (context, NULL, NULL);
}

/* Target side drag signals */
static void
gtk_icon_view_drag_leave (GtkWidget      *widget,
			  GdkDragContext *context,
			  guint           time)
{
  GtkIconView *icon_view;

  icon_view = GTK_ICON_VIEW (widget);

  /* unset any highlight row */
  __gtk_icon_view_set_drag_dest_item (icon_view,
				    NULL,
				    GTK_ICON_VIEW_DROP_LEFT);

  remove_scroll_timeout (icon_view);
}

static gboolean 
gtk_icon_view_drag_motion (GtkWidget      *widget,
			   GdkDragContext *context,
			   gint            x,
			   gint            y,
			   guint           time)
{
  GtkTreePath *path = NULL;
  GtkIconViewDropPosition pos;
  GtkIconView *icon_view;
  GdkDragAction suggested_action = 0;
  GdkAtom target;
  gboolean empty;

  icon_view = GTK_ICON_VIEW (widget);

  if (!set_destination (icon_view, context, x, y, &suggested_action, &target))
    return FALSE;

  __gtk_icon_view_get_drag_dest_item (icon_view, &path, &pos);

  /* we only know this *after* set_desination_row */
  empty = gtk_icon_view_get_props (icon_view)->priv->empty_view_drop;

  if (path == NULL && !empty)
    {
      /* Can't drop here. */
      __gdk_drag_status (context, 0, time);
    }
  else
    {
      if (gtk_icon_view_get_props (icon_view)->priv->scroll_timeout_id == 0)
	{
	  gtk_icon_view_get_props (icon_view)->priv->scroll_timeout_id =
	    __gdk_threads_add_timeout (50, drag_scroll_timeout, icon_view);
	}

      if (target == __gdk_atom_intern_static_string ("GTK_TREE_MODEL_ROW"))
        {
          /* Request data so we can use the source row when
           * determining whether to accept the drop
           */
          set_status_pending (context, suggested_action);
          __gtk_drag_get_data (widget, context, target, time);
        }
      else
        {
          set_status_pending (context, 0);
          __gdk_drag_status (context, suggested_action, time);
        }
    }

  if (path)
    __gtk_tree_path_free (path);

  return TRUE;
}

static gboolean 
gtk_icon_view_drag_drop (GtkWidget      *widget,
			 GdkDragContext *context,
			 gint            x,
			 gint            y,
			 guint           time)
{
  GtkIconView *icon_view;
  GtkTreePath *path;
  GdkDragAction suggested_action = 0;
  GdkAtom target = GDK_NONE;
  GtkTreeModel *model;
  gboolean drop_append_mode;

  icon_view = GTK_ICON_VIEW (widget);
  model = __gtk_icon_view_get_model (icon_view);

  remove_scroll_timeout (GTK_ICON_VIEW (widget));

  if (!gtk_icon_view_get_props (icon_view)->priv->dest_set)
    return FALSE;

  if (!check_model_dnd (model, GTK_TYPE_TREE_DRAG_DEST, "drag-drop"))
    return FALSE;

  if (!set_destination (icon_view, context, x, y, &suggested_action, &target))
    return FALSE;
  
  path = get_logical_destination (icon_view, &drop_append_mode);

  if (target != GDK_NONE && path != NULL)
    {
      /* in case a motion had requested drag data, change things so we
       * treat drag data receives as a drop.
       */
      set_status_pending (context, 0);
      set_dest_row (context, model, path, 
		    gtk_icon_view_get_props (icon_view)->priv->empty_view_drop, drop_append_mode);
    }

  if (path)
    __gtk_tree_path_free (path);

  /* Unset this thing */
  __gtk_icon_view_set_drag_dest_item (icon_view, NULL, GTK_ICON_VIEW_DROP_LEFT);

  if (target != GDK_NONE)
    {
      __gtk_drag_get_data (widget, context, target, time);
      return TRUE;
    }
  else
    return FALSE;
}

static void
gtk_icon_view_drag_data_received (GtkWidget        *widget,
				  GdkDragContext   *context,
				  gint              x,
				  gint              y,
				  GtkSelectionData *selection_data,
				  guint             info,
				  guint             time)
{
  GtkTreePath *path;
  gboolean accepted = FALSE;
  GtkTreeModel *model;
  GtkIconView *icon_view;
  GtkTreePath *dest_row;
  GdkDragAction suggested_action;
  gboolean drop_append_mode;
  
  icon_view = GTK_ICON_VIEW (widget);  
  model = __gtk_icon_view_get_model (icon_view);

  if (!check_model_dnd (model, GTK_TYPE_TREE_DRAG_DEST, "drag-data-received"))
    return;

  if (!gtk_icon_view_get_props (icon_view)->priv->dest_set)
    return;

  suggested_action = get_status_pending (context);

  if (suggested_action)
    {
      /* We are getting this data due to a request in drag_motion,
       * rather than due to a request in drag_drop, so we are just
       * supposed to call drag_status, not actually paste in the
       * data.
       */
      path = get_logical_destination (icon_view, &drop_append_mode);

      if (path == NULL)
        suggested_action = 0;

      if (suggested_action)
        {
	  if (!__gtk_tree_drag_dest_row_drop_possible (GTK_TREE_DRAG_DEST (model),
						     path,
						     selection_data))
	    suggested_action = 0;
        }

      __gdk_drag_status (context, suggested_action, time);

      if (path)
        __gtk_tree_path_free (path);

      /* If you can't drop, remove user drop indicator until the next motion */
      if (suggested_action == 0)
        __gtk_icon_view_set_drag_dest_item (icon_view,
					  NULL,
					  GTK_ICON_VIEW_DROP_LEFT);
      return;
    }
  

  dest_row = get_dest_row (context);

  if (dest_row == NULL)
    return;

  if (selection_data->length >= 0)
    {
      if (__gtk_tree_drag_dest_drag_data_received (GTK_TREE_DRAG_DEST (model),
                                                 dest_row,
                                                 selection_data))
        accepted = TRUE;
    }

  __gtk_drag_finish (context,
                   accepted,
                   (__gdk_drag_context_get_selected_action (context) == GDK_ACTION_MOVE),
                   time);

  __gtk_tree_path_free (dest_row);

  /* drop dest_row */
  set_dest_row (context, NULL, NULL, FALSE, FALSE);
}

/* Drag-and-Drop support */
/**
 * __gtk_icon_view_enable_model_drag_source:
 * @icon_view: a #GtkIconTreeView
 * @start_button_mask: Mask of allowed buttons to start drag
 * @targets: the table of targets that the drag will support
 * @n_targets: the number of items in @targets
 * @actions: the bitmask of possible actions for a drag from this
 *    widget
 *
 * Turns @icon_view into a drag source for automatic DND. Calling this
 * method sets #GtkIconView:reorderable to %FALSE.
 *
 * Since: 2.8
 **/
void
__gtk_icon_view_enable_model_drag_source (GtkIconView              *icon_view,
					GdkModifierType           start_button_mask,
					const GtkTargetEntry     *targets,
					gint                      n_targets,
					GdkDragAction             actions)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  __gtk_drag_source_set (GTK_WIDGET (icon_view), 0, targets, n_targets, actions);

  gtk_icon_view_get_props (icon_view)->priv->start_button_mask = start_button_mask;
  gtk_icon_view_get_props (icon_view)->priv->source_actions = actions;

  gtk_icon_view_get_props (icon_view)->priv->source_set = TRUE;

  unset_reorderable (icon_view);
}

/**
 * __gtk_icon_view_enable_model_drag_dest:
 * @icon_view: a #GtkIconView
 * @targets: the table of targets that the drag will support
 * @n_targets: the number of items in @targets
 * @actions: the bitmask of possible actions for a drag to this
 *    widget
 *
 * Turns @icon_view into a drop destination for automatic DND. Calling this
 * method sets #GtkIconView:reorderable to %FALSE.
 *
 * Since: 2.8
 **/
void 
__gtk_icon_view_enable_model_drag_dest (GtkIconView          *icon_view,
				      const GtkTargetEntry *targets,
				      gint                  n_targets,
				      GdkDragAction         actions)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  __gtk_drag_dest_set (GTK_WIDGET (icon_view), 0, targets, n_targets, actions);

  gtk_icon_view_get_props (icon_view)->priv->dest_actions = actions;

  gtk_icon_view_get_props (icon_view)->priv->dest_set = TRUE;

  unset_reorderable (icon_view);  
}

/**
 * __gtk_icon_view_unset_model_drag_source:
 * @icon_view: a #GtkIconView
 * 
 * Undoes the effect of __gtk_icon_view_enable_model_drag_source(). Calling this
 * method sets #GtkIconView:reorderable to %FALSE.
 *
 * Since: 2.8
 **/
void
__gtk_icon_view_unset_model_drag_source (GtkIconView *icon_view)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  if (gtk_icon_view_get_props (icon_view)->priv->source_set)
    {
      __gtk_drag_source_unset (GTK_WIDGET (icon_view));
      gtk_icon_view_get_props (icon_view)->priv->source_set = FALSE;
    }

  unset_reorderable (icon_view);
}

/**
 * __gtk_icon_view_unset_model_drag_dest:
 * @icon_view: a #GtkIconView
 * 
 * Undoes the effect of __gtk_icon_view_enable_model_drag_dest(). Calling this
 * method sets #GtkIconView:reorderable to %FALSE.
 *
 * Since: 2.8
 **/
void
__gtk_icon_view_unset_model_drag_dest (GtkIconView *icon_view)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  if (gtk_icon_view_get_props (icon_view)->priv->dest_set)
    {
      __gtk_drag_dest_unset (GTK_WIDGET (icon_view));
      gtk_icon_view_get_props (icon_view)->priv->dest_set = FALSE;
    }

  unset_reorderable (icon_view);
}

/* These are useful to implement your own custom stuff. */
/**
 * __gtk_icon_view_set_drag_dest_item:
 * @icon_view: a #GtkIconView
 * @path: (allow-none): The path of the item to highlight, or %NULL.
 * @pos: Specifies where to drop, relative to the item
 *
 * Sets the item that is highlighted for feedback.
 *
 * Since: 2.8
 */
void
__gtk_icon_view_set_drag_dest_item (GtkIconView              *icon_view,
				  GtkTreePath              *path,
				  GtkIconViewDropPosition   pos)
{
  /* Note; this function is exported to allow a custom DND
   * implementation, so it can't touch TreeViewDragInfo
   */

  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  if (gtk_icon_view_get_props (icon_view)->priv->dest_item)
    {
      GtkTreePath *current_path;
      current_path = __gtk_tree_row_reference_get_path (gtk_icon_view_get_props (icon_view)->priv->dest_item);
      __gtk_tree_row_reference_free (gtk_icon_view_get_props (icon_view)->priv->dest_item);
      gtk_icon_view_get_props (icon_view)->priv->dest_item = NULL;      

      gtk_icon_view_queue_draw_path (icon_view, current_path);
      __gtk_tree_path_free (current_path);
    }
  
  /* special case a drop on an empty model */
  gtk_icon_view_get_props (icon_view)->priv->empty_view_drop = FALSE;
  if (pos == GTK_ICON_VIEW_DROP_ABOVE && path
      && __gtk_tree_path_get_depth (path) == 1
      && __gtk_tree_path_get_indices (path)[0] == 0)
    {
      gint n_children;

      n_children = __gtk_tree_model_iter_n_children (gtk_icon_view_get_props (icon_view)->priv->model,
                                                   NULL);

      if (n_children == 0)
        gtk_icon_view_get_props (icon_view)->priv->empty_view_drop = TRUE;
    }

  gtk_icon_view_get_props (icon_view)->priv->dest_pos = pos;

  if (path)
    {
      gtk_icon_view_get_props (icon_view)->priv->dest_item =
        __gtk_tree_row_reference_new_proxy (G_OBJECT (icon_view), 
					  gtk_icon_view_get_props (icon_view)->priv->model, path);
      
      gtk_icon_view_queue_draw_path (icon_view, path);
    }
}

/**
 * __gtk_icon_view_get_drag_dest_item:
 * @icon_view: a #GtkIconView
 * @path: (allow-none): Return location for the path of the highlighted item, or %NULL.
 * @pos: (allow-none): Return location for the drop position, or %NULL
 * 
 * Gets information about the item that is highlighted for feedback.
 *
 * Since: 2.8
 **/
void
__gtk_icon_view_get_drag_dest_item (GtkIconView              *icon_view,
				  GtkTreePath             **path,
				  GtkIconViewDropPosition  *pos)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  if (path)
    {
      if (gtk_icon_view_get_props (icon_view)->priv->dest_item)
        *path = __gtk_tree_row_reference_get_path (gtk_icon_view_get_props (icon_view)->priv->dest_item);
      else
	*path = NULL;
    }

  if (pos)
    *pos = gtk_icon_view_get_props (icon_view)->priv->dest_pos;
}

/**
 * __gtk_icon_view_get_dest_item_at_pos:
 * @icon_view: a #GtkIconView
 * @drag_x: the position to determine the destination item for
 * @drag_y: the position to determine the destination item for
 * @path: (allow-none): Return location for the path of the item, or %NULL.
 * @pos: (allow-none): Return location for the drop position, or %NULL
 * 
 * Determines the destination item for a given position.
 * 
 * Return value: whether there is an item at the given position.
 *
 * Since: 2.8
 **/
gboolean
__gtk_icon_view_get_dest_item_at_pos (GtkIconView              *icon_view,
				    gint                      drag_x,
				    gint                      drag_y,
				    GtkTreePath             **path,
				    GtkIconViewDropPosition  *pos)
{
  GtkIconViewItem *item;

  /* Note; this function is exported to allow a custom DND
   * implementation, so it can't touch TreeViewDragInfo
   */

  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), FALSE);
  g_return_val_if_fail (drag_x >= 0, FALSE);
  g_return_val_if_fail (drag_y >= 0, FALSE);
  g_return_val_if_fail (gtk_icon_view_get_props (icon_view)->priv->bin_window != NULL, FALSE);


  if (path)
    *path = NULL;

  item = gtk_icon_view_get_item_at_coords (icon_view, 
					   drag_x + gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->hadjustment)->value, 
					   drag_y + gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->value,
					   FALSE, NULL);

  if (item == NULL)
    return FALSE;

  if (path)
    *path = __gtk_tree_path_new_from_indices (item->index, -1);

  if (pos)
    {
      if (drag_x < item->x + item->width / 4)
	*pos = GTK_ICON_VIEW_DROP_LEFT;
      else if (drag_x > item->x + item->width * 3 / 4)
	*pos = GTK_ICON_VIEW_DROP_RIGHT;
      else if (drag_y < item->y + item->height / 4)
	*pos = GTK_ICON_VIEW_DROP_ABOVE;
      else if (drag_y > item->y + item->height * 3 / 4)
	*pos = GTK_ICON_VIEW_DROP_BELOW;
      else
	*pos = GTK_ICON_VIEW_DROP_INTO;
    }

  return TRUE;
}

/**
 * __gtk_icon_view_create_drag_icon:
 * @icon_view: a #GtkIconView
 * @path: a #GtkTreePath in @icon_view
 *
 * Creates a #GdkPixmap representation of the item at @path.
 * This image is used for a drag icon.
 *
 * Return value: (transfer full): a newly-allocated pixmap of the drag icon.
 * 
 * Since: 2.8
 **/
GdkPixmap *
__gtk_icon_view_create_drag_icon (GtkIconView *icon_view,
				GtkTreePath *path)
{
  GtkWidget *widget;
  cairo_t *cr;
  GdkPixmap *drawable;
  GList *l;
  gint index;
  GdkRectangle area;

  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), NULL);
  g_return_val_if_fail (path != NULL, NULL);

  widget = GTK_WIDGET (icon_view);

  if (!__gtk_widget_get_realized (widget))
    return NULL;

  index = __gtk_tree_path_get_indices (path)[0];

  for (l = gtk_icon_view_get_props (icon_view)->priv->items; l; l = l->next) 
    {
      GtkIconViewItem *item = l->data;
      
      if (index == item->index)
	{
	  drawable = __gdk_pixmap_new ((GdkDrawable *) gtk_icon_view_get_props (icon_view)->priv->bin_window,
				     item->width + 2,
				     item->height + 2,
				     -1);

	  cr = __gdk_cairo_create ((GdkDrawable *) (drawable));
	  cairo_set_line_width (cr, 1.);

	  __gdk_cairo_set_source_color
	    (cr, &gtk_widget_get_props (widget)->style->base[__gtk_widget_get_state (widget)]);
	  cairo_rectangle (cr, 0, 0, item->width + 2, item->height + 2);
	  cairo_fill (cr);

	  area.x = 0;
	  area.y = 0;
	  area.width = item->width;
	  area.height = item->height;

	  gtk_icon_view_paint_item (icon_view, cr, item, &area, 
 				    drawable, 1, 1, FALSE); 

	  cairo_set_source_rgb (cr, 0.0, 0.0, 0.0); /* black */
	  cairo_rectangle (cr, 0.5, 0.5, item->width + 1, item->height + 1);
	  cairo_stroke (cr);

	  cairo_destroy (cr);

	  return drawable;
	}
    }
  
  return NULL;
}

/**
 * __gtk_icon_view_get_reorderable:
 * @icon_view: a #GtkIconView
 *
 * Retrieves whether the user can reorder the list via drag-and-drop. 
 * See __gtk_icon_view_set_reorderable().
 *
 * Return value: %TRUE if the list can be reordered.
 *
 * Since: 2.8
 **/
gboolean
__gtk_icon_view_get_reorderable (GtkIconView *icon_view)
{
  g_return_val_if_fail (GTK_IS_ICON_VIEW (icon_view), FALSE);

  return gtk_icon_view_get_props (icon_view)->priv->reorderable;
}

static const GtkTargetEntry item_targets[] = {
  { "GTK_TREE_MODEL_ROW", GTK_TARGET_SAME_WIDGET, 0 }
};


/**
 * __gtk_icon_view_set_reorderable:
 * @icon_view: A #GtkIconView.
 * @reorderable: %TRUE, if the list of items can be reordered.
 *
 * This function is a convenience function to allow you to reorder models that
 * support the #GtkTreeDragSourceIface and the #GtkTreeDragDestIface.  Both
 * #GtkTreeStore and #GtkListStore support these.  If @reorderable is %TRUE, then
 * the user can reorder the model by dragging and dropping rows.  The
 * developer can listen to these changes by connecting to the model's
 * row_inserted and row_deleted signals. The reordering is implemented by setting up
 * the icon view as a drag source and destination. Therefore, drag and
 * drop can not be used in a reorderable view for any other purpose.
 *
 * This function does not give you any degree of control over the order -- any
 * reordering is allowed.  If more control is needed, you should probably
 * handle drag and drop manually.
 *
 * Since: 2.8
 **/
void
__gtk_icon_view_set_reorderable (GtkIconView *icon_view,
			       gboolean     reorderable)
{
  g_return_if_fail (GTK_IS_ICON_VIEW (icon_view));

  reorderable = reorderable != FALSE;

  if (gtk_icon_view_get_props (icon_view)->priv->reorderable == reorderable)
    return;

  if (reorderable)
    {
      __gtk_icon_view_enable_model_drag_source (icon_view,
					      GDK_BUTTON1_MASK,
					      item_targets,
					      G_N_ELEMENTS (item_targets),
					      GDK_ACTION_MOVE);
      __gtk_icon_view_enable_model_drag_dest (icon_view,
					    item_targets,
					    G_N_ELEMENTS (item_targets),
					    GDK_ACTION_MOVE);
    }
  else
    {
      __gtk_icon_view_unset_model_drag_source (icon_view);
      __gtk_icon_view_unset_model_drag_dest (icon_view);
    }

  gtk_icon_view_get_props (icon_view)->priv->reorderable = reorderable;

  g_object_notify (G_OBJECT (icon_view), "reorderable");
}


/* Accessibility Support */

static gpointer accessible_parent_class;
static gpointer accessible_item_parent_class;
static GQuark accessible_private_data_quark = 0;

#define GTK_TYPE_ICON_VIEW_ITEM_ACCESSIBLE      (gtk_icon_view_item_accessible_get_type ())
#define GTK_ICON_VIEW_ITEM_ACCESSIBLE(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_ICON_VIEW_ITEM_ACCESSIBLE, GtkIconViewItemAccessible))
#define GTK_IS_ICON_VIEW_ITEM_ACCESSIBLE(obj)   (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_ICON_VIEW_ITEM_ACCESSIBLE))

static GType gtk_icon_view_item_accessible_get_type (void);

enum {
    ACTION_ACTIVATE,
    LAST_ACTION
};

typedef struct
{
  AtkObject parent;

  GtkIconViewItem *item;

  GtkWidget *widget;

  AtkStateSet *state_set;

  gchar *text;

  GtkTextBuffer *text_buffer;

  gchar *action_descriptions[LAST_ACTION];
  gchar *image_description;
  guint action_idle_handler;
} GtkIconViewItemAccessible;

static const gchar *const gtk_icon_view_item_accessible_action_names[] = 
{
  "activate",
  NULL
};

static const gchar *const gtk_icon_view_item_accessible_action_descriptions[] =
{
  "Activate item",
  NULL
};
typedef struct _GtkIconViewItemAccessibleClass
{
  AtkObjectClass parent_class;

} GtkIconViewItemAccessibleClass;

static gboolean gtk_icon_view_item_accessible_is_showing (GtkIconViewItemAccessible *item);

static gboolean
gtk_icon_view_item_accessible_idle_do_action (gpointer data)
{
  GtkIconViewItemAccessible *item;
  GtkIconView *icon_view;
  GtkTreePath *path;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (data);
  item->action_idle_handler = 0;

  if (item->widget != NULL)
    {
      icon_view = GTK_ICON_VIEW (item->widget);
      path = __gtk_tree_path_new_from_indices (item->item->index, -1);
      __gtk_icon_view_item_activated (icon_view, path);
      __gtk_tree_path_free (path);
    }

  return FALSE;
}

static gboolean
gtk_icon_view_item_accessible_action_do_action (AtkAction *action,
                                                gint       i)
{
  GtkIconViewItemAccessible *item;

  if (i < 0 || i >= LAST_ACTION) 
    return FALSE;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (action);

  if (!GTK_IS_ICON_VIEW (item->widget))
    return FALSE;

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return FALSE;

  switch (i)
    {
    case ACTION_ACTIVATE:
      if (!item->action_idle_handler)
        item->action_idle_handler = __gdk_threads_add_idle (gtk_icon_view_item_accessible_idle_do_action, item);
      break;
    default:
      g_assert_not_reached ();
      return FALSE;

    }        
  return TRUE;
}

static gint
gtk_icon_view_item_accessible_action_get_n_actions (AtkAction *action)
{
        return LAST_ACTION;
}

static const gchar *
gtk_icon_view_item_accessible_action_get_description (AtkAction *action,
                                                      gint       i)
{
  GtkIconViewItemAccessible *item;

  if (i < 0 || i >= LAST_ACTION) 
    return NULL;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (action);

  if (item->action_descriptions[i])
    return item->action_descriptions[i];
  else
    return gtk_icon_view_item_accessible_action_descriptions[i];
}

static const gchar *
gtk_icon_view_item_accessible_action_get_name (AtkAction *action,
                                               gint       i)
{
  if (i < 0 || i >= LAST_ACTION) 
    return NULL;

  return gtk_icon_view_item_accessible_action_names[i];
}

static gboolean
gtk_icon_view_item_accessible_action_set_description (AtkAction   *action,
                                                      gint         i,
                                                      const gchar *description)
{
  GtkIconViewItemAccessible *item;

  if (i < 0 || i >= LAST_ACTION) 
    return FALSE;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (action);

  g_free (item->action_descriptions[i]);

  item->action_descriptions[i] = g_strdup (description);

  return TRUE;
}

static void
atk_action_item_interface_init (AtkActionIface *iface)
{
  iface->do_action = gtk_icon_view_item_accessible_action_do_action;
  iface->get_n_actions = gtk_icon_view_item_accessible_action_get_n_actions;
  iface->get_description = gtk_icon_view_item_accessible_action_get_description;
  iface->get_name = gtk_icon_view_item_accessible_action_get_name;
  iface->set_description = gtk_icon_view_item_accessible_action_set_description;
}

static const gchar *
gtk_icon_view_item_accessible_image_get_image_description (AtkImage *image)
{
  GtkIconViewItemAccessible *item;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (image);

  return item->image_description;
}

static gboolean
gtk_icon_view_item_accessible_image_set_image_description (AtkImage    *image,
                                                           const gchar *description)
{
  GtkIconViewItemAccessible *item;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (image);

  g_free (item->image_description);
  item->image_description = g_strdup (description);

  return TRUE;
}

static gboolean
get_pixbuf_box (GtkIconView     *icon_view,
		GtkIconViewItem *item,
		GdkRectangle    *box)
{
  GList *l;

  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list; l; l = l->next)
    {
      GtkIconViewCellInfo *info = l->data;
      
      if (GTK_IS_CELL_RENDERER_PIXBUF (info->cell))
	{
	  gtk_icon_view_get_cell_box (icon_view, item, info, box);

	  return TRUE;
	}
    }

  return FALSE;
}

static gchar *
get_text (GtkIconView     *icon_view,
	  GtkIconViewItem *item)
{
  GList *l;
  gchar *text;

  for (l = gtk_icon_view_get_props (icon_view)->priv->cell_list; l; l = l->next)
    {
      GtkIconViewCellInfo *info = l->data;
      
      if (GTK_IS_CELL_RENDERER_TEXT (info->cell))
	{
	  g_object_get (info->cell, "text", &text, NULL);
	  
	  return text;
	}
    }

  return NULL;
}

static void
gtk_icon_view_item_accessible_image_get_image_size (AtkImage *image,
                                                    gint     *width,
                                                    gint     *height)
{
  GtkIconViewItemAccessible *item;
  GdkRectangle box;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (image);

  if (!GTK_IS_ICON_VIEW (item->widget))
    return;

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return;

  if (get_pixbuf_box (GTK_ICON_VIEW (item->widget), item->item, &box))
    {
      *width = box.width;
      *height = box.height;  
    }
}

static void
gtk_icon_view_item_accessible_image_get_image_position (AtkImage    *image,
                                                        gint        *x,
                                                        gint        *y,
                                                        AtkCoordType coord_type)
{
  GtkIconViewItemAccessible *item;
  GdkRectangle box;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (image);

  if (!GTK_IS_ICON_VIEW (item->widget))
    return;

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return;

  atk_component_get_position (ATK_COMPONENT (image), x, y, coord_type);

  if (get_pixbuf_box (GTK_ICON_VIEW (item->widget), item->item, &box))
    {
      *x+= box.x - item->item->x;
      *y+= box.y - item->item->y;
    }

}

static void
atk_image_item_interface_init (AtkImageIface *iface)
{
  iface->get_image_description = gtk_icon_view_item_accessible_image_get_image_description;
  iface->set_image_description = gtk_icon_view_item_accessible_image_set_image_description;
  iface->get_image_size = gtk_icon_view_item_accessible_image_get_image_size;
  iface->get_image_position = gtk_icon_view_item_accessible_image_get_image_position;
}

static gchar *
gtk_icon_view_item_accessible_text_get_text (AtkText *text,
                                             gint     start_pos,
                                             gint     end_pos)
{
  GtkIconViewItemAccessible *item;
  GtkTextIter start, end;
  GtkTextBuffer *buffer;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (text);

  if (!GTK_IS_ICON_VIEW (item->widget))
    return NULL;

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return NULL;

  buffer = item->text_buffer;
  __gtk_text_buffer_get_iter_at_offset (buffer, &start, start_pos);
  if (end_pos < 0)
    __gtk_text_buffer_get_end_iter (buffer, &end);
  else
    __gtk_text_buffer_get_iter_at_offset (buffer, &end, end_pos);

  return __gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
}

static gunichar
gtk_icon_view_item_accessible_text_get_character_at_offset (AtkText *text,
                                                            gint     offset)
{
  GtkIconViewItemAccessible *item;
  GtkTextIter start, end;
  GtkTextBuffer *buffer;
  gchar *string;
  gunichar unichar;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (text);

  if (!GTK_IS_ICON_VIEW (item->widget))
    return '\0';

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return '\0';

  buffer = item->text_buffer;
  if (offset >= __gtk_text_buffer_get_char_count (buffer))
    return '\0';

  __gtk_text_buffer_get_iter_at_offset (buffer, &start, offset);
  end = start;
  __gtk_text_iter_forward_char (&end);
  string = __gtk_text_buffer_get_slice (buffer, &start, &end, FALSE);
  unichar = g_utf8_get_char (string);
  g_free(string);

  return unichar;
}

#if 0
static void
get_pango_text_offsets (PangoLayout     *layout,
                        GtkTextBuffer   *buffer,
                        gint             function,
                        AtkTextBoundary  boundary_type,
                        gint             offset,
                        gint            *start_offset,
                        gint            *end_offset,
                        GtkTextIter     *start_iter,
                        GtkTextIter     *end_iter)
{
  PangoLayoutIter *iter;
  PangoLayoutLine *line, *prev_line = NULL, *prev_prev_line = NULL;
  gint index, start_index, end_index;
  const gchar *text;
  gboolean found = FALSE;

  text = pango_layout_get_text (layout);
  index = g_utf8_offset_to_pointer (text, offset) - text;
  iter = pango_layout_get_iter (layout);
  do
    {
      line = pango_layout_iter_get_line_readonly (iter);
      start_index = line->start_index;
      end_index = start_index + line->length;

      if (index >= start_index && index <= end_index)
        {
          /*
           * Found line for offset
           */
          switch (function)
            {
            case 0:
                  /*
                   * We want the previous line
                   */
              if (prev_line)
                {
                  switch (boundary_type)
                    {
                    case ATK_TEXT_BOUNDARY_LINE_START:
                      end_index = start_index;
                      start_index = prev_line->start_index;
                      break;
                    case ATK_TEXT_BOUNDARY_LINE_END:
                      if (prev_prev_line)
                        start_index = prev_prev_line->start_index + 
                                  prev_prev_line->length;
                      end_index = prev_line->start_index + prev_line->length;
                      break;
                    default:
                      g_assert_not_reached();
                    }
                }
              else
                start_index = end_index = 0;
              break;
            case 1:
              switch (boundary_type)
                {
                case ATK_TEXT_BOUNDARY_LINE_START:
                  if (pango_layout_iter_next_line (iter))
                    end_index = pango_layout_iter_get_line_readonly (iter)->start_index;
                  break;
                case ATK_TEXT_BOUNDARY_LINE_END:
                  if (prev_line)
                    start_index = prev_line->start_index + 
                                  prev_line->length;
                  break;
                default:
                  g_assert_not_reached();
                }
              break;
            case 2:
               /*
                * We want the next line
                */
              if (pango_layout_iter_next_line (iter))
                {
                  line = pango_layout_iter_get_line_readonly (iter);
                  switch (boundary_type)
                    {
                    case ATK_TEXT_BOUNDARY_LINE_START:
                      start_index = line->start_index;
                      if (pango_layout_iter_next_line (iter))
                        end_index = pango_layout_iter_get_line_readonly (iter)->start_index;
                      else
                        end_index = start_index + line->length;
                      break;
                    case ATK_TEXT_BOUNDARY_LINE_END:
                      start_index = end_index;
                      end_index = line->start_index + line->length;
                      break;
                    default:
                      g_assert_not_reached();
                    }
                }
              else
                start_index = end_index;
              break;
            }
          found = TRUE;
          break;
        }
      prev_prev_line = prev_line; 
      prev_line = line; 
    }
  while (pango_layout_iter_next_line (iter));

  if (!found)
    {
      start_index = prev_line->start_index + prev_line->length;
      end_index = start_index;
    }
  pango_layout_iter_free (iter);
  *start_offset = g_utf8_pointer_to_offset (text, text + start_index);
  *end_offset = g_utf8_pointer_to_offset (text, text + end_index);
 
  __gtk_text_buffer_get_iter_at_offset (buffer, start_iter, *start_offset);
  __gtk_text_buffer_get_iter_at_offset (buffer, end_iter, *end_offset);
}
#endif

static gchar*
gtk_icon_view_item_accessible_text_get_text_before_offset (AtkText         *text,
                                                           gint            offset,
                                                           AtkTextBoundary boundary_type,
                                                           gint            *start_offset,
                                                           gint            *end_offset)
{
  GtkIconViewItemAccessible *item;
  GtkTextIter start, end;
  GtkTextBuffer *buffer;
#if 0
  GtkIconView *icon_view;
#endif

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (text);

  if (!GTK_IS_ICON_VIEW (item->widget))
    return NULL;

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return NULL;

  buffer = item->text_buffer;

  if (!__gtk_text_buffer_get_char_count (buffer))
    {
      *start_offset = 0;
      *end_offset = 0;
      return g_strdup ("");
    }
  __gtk_text_buffer_get_iter_at_offset (buffer, &start, offset);
   
  end = start;

  switch (boundary_type)
    {
    case ATK_TEXT_BOUNDARY_CHAR:
      __gtk_text_iter_backward_char(&start);
      break;
    case ATK_TEXT_BOUNDARY_WORD_START:
      if (!__gtk_text_iter_starts_word (&start))
        __gtk_text_iter_backward_word_start (&start);
      end = start;
      __gtk_text_iter_backward_word_start(&start);
      break;
    case ATK_TEXT_BOUNDARY_WORD_END:
      if (__gtk_text_iter_inside_word (&start) &&
          !__gtk_text_iter_starts_word (&start))
        __gtk_text_iter_backward_word_start (&start);
      while (!__gtk_text_iter_ends_word (&start))
        {
          if (!__gtk_text_iter_backward_char (&start))
            break;
        }
      end = start;
      __gtk_text_iter_backward_word_start(&start);
      while (!__gtk_text_iter_ends_word (&start))
        {
          if (!__gtk_text_iter_backward_char (&start))
            break;
        }
      break;
    case ATK_TEXT_BOUNDARY_SENTENCE_START:
      if (!__gtk_text_iter_starts_sentence (&start))
        __gtk_text_iter_backward_sentence_start (&start);
      end = start;
      __gtk_text_iter_backward_sentence_start (&start);
      break;
    case ATK_TEXT_BOUNDARY_SENTENCE_END:
      if (__gtk_text_iter_inside_sentence (&start) &&
          !__gtk_text_iter_starts_sentence (&start))
        __gtk_text_iter_backward_sentence_start (&start);
      while (!__gtk_text_iter_ends_sentence (&start))
        {
          if (!__gtk_text_iter_backward_char (&start))
            break;
        }
      end = start;
      __gtk_text_iter_backward_sentence_start (&start);
      while (!__gtk_text_iter_ends_sentence (&start))
        {
          if (!__gtk_text_iter_backward_char (&start))
            break;
        }
      break;
   case ATK_TEXT_BOUNDARY_LINE_START:
   case ATK_TEXT_BOUNDARY_LINE_END:
#if 0
      icon_view = GTK_ICON_VIEW (item->widget);
      /* FIXME we probably have to use GailTextCell to salvage this */
      gtk_icon_view_update_item_text (icon_view, item->item);
      get_pango_text_offsets (gtk_icon_view_get_props (icon_view)->priv->layout,
                              buffer,
                              0,
                              boundary_type,
                              offset,
                              start_offset,
                              end_offset,
                              &start,
                              &end);
#endif
      break;
    }

  *start_offset = __gtk_text_iter_get_offset (&start);
  *end_offset = __gtk_text_iter_get_offset (&end);

  return __gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
}

static gchar*
gtk_icon_view_item_accessible_text_get_text_at_offset (AtkText         *text,
                                                       gint            offset,
                                                       AtkTextBoundary boundary_type,
                                                       gint            *start_offset,
                                                       gint            *end_offset)
{
  GtkIconViewItemAccessible *item;
  GtkTextIter start, end;
  GtkTextBuffer *buffer;
#if 0
  GtkIconView *icon_view;
#endif

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (text);

  if (!GTK_IS_ICON_VIEW (item->widget))
    return NULL;

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return NULL;

  buffer = item->text_buffer;

  if (!__gtk_text_buffer_get_char_count (buffer))
    {
      *start_offset = 0;
      *end_offset = 0;
      return g_strdup ("");
    }
  __gtk_text_buffer_get_iter_at_offset (buffer, &start, offset);
   
  end = start;

  switch (boundary_type)
    {
    case ATK_TEXT_BOUNDARY_CHAR:
      __gtk_text_iter_forward_char (&end);
      break;
    case ATK_TEXT_BOUNDARY_WORD_START:
      if (!__gtk_text_iter_starts_word (&start))
        __gtk_text_iter_backward_word_start (&start);
      if (__gtk_text_iter_inside_word (&end))
        __gtk_text_iter_forward_word_end (&end);
      while (!__gtk_text_iter_starts_word (&end))
        {
          if (!__gtk_text_iter_forward_char (&end))
            break;
        }
      break;
    case ATK_TEXT_BOUNDARY_WORD_END:
      if (__gtk_text_iter_inside_word (&start) &&
          !__gtk_text_iter_starts_word (&start))
        __gtk_text_iter_backward_word_start (&start);
      while (!__gtk_text_iter_ends_word (&start))
        {
          if (!__gtk_text_iter_backward_char (&start))
            break;
        }
      __gtk_text_iter_forward_word_end (&end);
      break;
    case ATK_TEXT_BOUNDARY_SENTENCE_START:
      if (!__gtk_text_iter_starts_sentence (&start))
        __gtk_text_iter_backward_sentence_start (&start);
      if (__gtk_text_iter_inside_sentence (&end))
        __gtk_text_iter_forward_sentence_end (&end);
      while (!__gtk_text_iter_starts_sentence (&end))
        {
          if (!__gtk_text_iter_forward_char (&end))
            break;
        }
      break;
    case ATK_TEXT_BOUNDARY_SENTENCE_END:
      if (__gtk_text_iter_inside_sentence (&start) &&
          !__gtk_text_iter_starts_sentence (&start))
        __gtk_text_iter_backward_sentence_start (&start);
      while (!__gtk_text_iter_ends_sentence (&start))
        {
          if (!__gtk_text_iter_backward_char (&start))
            break;
        }
      __gtk_text_iter_forward_sentence_end (&end);
      break;
   case ATK_TEXT_BOUNDARY_LINE_START:
   case ATK_TEXT_BOUNDARY_LINE_END:
#if 0
      icon_view = GTK_ICON_VIEW (item->widget);
      /* FIXME we probably have to use GailTextCell to salvage this */
      gtk_icon_view_update_item_text (icon_view, item->item);
      get_pango_text_offsets (gtk_icon_view_get_props (icon_view)->priv->layout,
                              buffer,
                              1,
                              boundary_type,
                              offset,
                              start_offset,
                              end_offset,
                              &start,
                              &end);
#endif
      break;
    }


  *start_offset = __gtk_text_iter_get_offset (&start);
  *end_offset = __gtk_text_iter_get_offset (&end);

  return __gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
}

static gchar*
gtk_icon_view_item_accessible_text_get_text_after_offset (AtkText         *text,
                                                          gint            offset,
                                                          AtkTextBoundary boundary_type,
                                                          gint            *start_offset,
                                                          gint            *end_offset)
{
  GtkIconViewItemAccessible *item;
  GtkTextIter start, end;
  GtkTextBuffer *buffer;
#if 0
  GtkIconView *icon_view;
#endif

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (text);

  if (!GTK_IS_ICON_VIEW (item->widget))
    return NULL;

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return NULL;

  buffer = item->text_buffer;

  if (!__gtk_text_buffer_get_char_count (buffer))
    {
      *start_offset = 0;
      *end_offset = 0;
      return g_strdup ("");
    }
  __gtk_text_buffer_get_iter_at_offset (buffer, &start, offset);
   
  end = start;

  switch (boundary_type)
    {
    case ATK_TEXT_BOUNDARY_CHAR:
      __gtk_text_iter_forward_char(&start);
      __gtk_text_iter_forward_chars(&end, 2);
      break;
    case ATK_TEXT_BOUNDARY_WORD_START:
      if (__gtk_text_iter_inside_word (&end))
        __gtk_text_iter_forward_word_end (&end);
      while (!__gtk_text_iter_starts_word (&end))
        {
          if (!__gtk_text_iter_forward_char (&end))
            break;
        }
      start = end;
      if (!__gtk_text_iter_is_end (&end))
        {
          __gtk_text_iter_forward_word_end (&end);
          while (!__gtk_text_iter_starts_word (&end))
            {
              if (!__gtk_text_iter_forward_char (&end))
                break;
            }
        }
      break;
    case ATK_TEXT_BOUNDARY_WORD_END:
      __gtk_text_iter_forward_word_end (&end);
      start = end;
      if (!__gtk_text_iter_is_end (&end))
        __gtk_text_iter_forward_word_end (&end);
      break;
    case ATK_TEXT_BOUNDARY_SENTENCE_START:
      if (__gtk_text_iter_inside_sentence (&end))
        __gtk_text_iter_forward_sentence_end (&end);
      while (!__gtk_text_iter_starts_sentence (&end))
        {
          if (!__gtk_text_iter_forward_char (&end))
            break;
        }
      start = end;
      if (!__gtk_text_iter_is_end (&end))
        {
          __gtk_text_iter_forward_sentence_end (&end);
          while (!__gtk_text_iter_starts_sentence (&end))
            {
              if (!__gtk_text_iter_forward_char (&end))
                break;
            }
        }
      break;
    case ATK_TEXT_BOUNDARY_SENTENCE_END:
      __gtk_text_iter_forward_sentence_end (&end);
      start = end;
      if (!__gtk_text_iter_is_end (&end))
        __gtk_text_iter_forward_sentence_end (&end);
      break;
   case ATK_TEXT_BOUNDARY_LINE_START:
   case ATK_TEXT_BOUNDARY_LINE_END:
#if 0
      icon_view = GTK_ICON_VIEW (item->widget);
      /* FIXME we probably have to use GailTextCell to salvage this */
      gtk_icon_view_update_item_text (icon_view, item->item);
      get_pango_text_offsets (gtk_icon_view_get_props (icon_view)->priv->layout,
                              buffer,
                              2,
                              boundary_type,
                              offset,
                              start_offset,
                              end_offset,
                              &start,
                              &end);
#endif
      break;
    }
  *start_offset = __gtk_text_iter_get_offset (&start);
  *end_offset = __gtk_text_iter_get_offset (&end);

  return __gtk_text_buffer_get_text (buffer, &start, &end, FALSE);
}

static gint
gtk_icon_view_item_accessible_text_get_character_count (AtkText *text)
{
  GtkIconViewItemAccessible *item;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (text);

  if (!GTK_IS_ICON_VIEW (item->widget))
    return 0;

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return 0;

  return __gtk_text_buffer_get_char_count (item->text_buffer);
}

static void
gtk_icon_view_item_accessible_text_get_character_extents (AtkText      *text,
                                                          gint         offset,
                                                          gint         *x,
                                                          gint         *y,
                                                          gint         *width,
                                                          gint         *height,
                                                          AtkCoordType coord_type)
{
  GtkIconViewItemAccessible *item;
#if 0
  GtkIconView *icon_view;
  PangoRectangle char_rect;
  const gchar *item_text;
  gint index;
#endif

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (text);

  if (!GTK_IS_ICON_VIEW (item->widget))
    return;

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return;

#if 0
  icon_view = GTK_ICON_VIEW (item->widget);
      /* FIXME we probably have to use GailTextCell to salvage this */
  gtk_icon_view_update_item_text (icon_view, item->item);
  item_text = pango_layout_get_text (gtk_icon_view_get_props (icon_view)->priv->layout);
  index = g_utf8_offset_to_pointer (item_text, offset) - item_text;
  pango_layout_index_to_pos (gtk_icon_view_get_props (icon_view)->priv->layout, index, &char_rect);

  atk_component_get_position (ATK_COMPONENT (text), x, y, coord_type);
  *x += item->item->layout_x - item->item->x + char_rect.x / PANGO_SCALE;
  /* Look at gtk_icon_view_paint_item() to see where the text is. */
  *x -=  ((item->item->width - item->item->layout_width) / 2) + (MAX (item->item->pixbuf_width, gtk_icon_view_get_props (icon_view)->priv->item_width) - item->item->width) / 2,
  *y += item->item->layout_y - item->item->y + char_rect.y / PANGO_SCALE;
  *width = char_rect.width / PANGO_SCALE;
  *height = char_rect.height / PANGO_SCALE;
#endif
}

static gint
gtk_icon_view_item_accessible_text_get_offset_at_point (AtkText      *text,
                                                        gint          x,
                                                        gint          y,
                                                        AtkCoordType coord_type)
{
  GtkIconViewItemAccessible *item;
  gint offset = 0;
#if 0
  GtkIconView *icon_view;
  const gchar *item_text;
  gint index;
  gint l_x, l_y;
#endif

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (text);

  if (!GTK_IS_ICON_VIEW (item->widget))
    return -1;

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return -1;

#if 0
  icon_view = GTK_ICON_VIEW (item->widget);
      /* FIXME we probably have to use GailTextCell to salvage this */
  gtk_icon_view_update_item_text (icon_view, item->item);
  atk_component_get_position (ATK_COMPONENT (text), &l_x, &l_y, coord_type);
  x -= l_x + item->item->layout_x - item->item->x;
  x +=  ((item->item->width - item->item->layout_width) / 2) + (MAX (item->item->pixbuf_width, gtk_icon_view_get_props (icon_view)->priv->item_width) - item->item->width) / 2,
  y -= l_y + item->item->layout_y - item->item->y;
  item_text = pango_layout_get_text (gtk_icon_view_get_props (icon_view)->priv->layout);
  if (!pango_layout_xy_to_index (gtk_icon_view_get_props (icon_view)->priv->layout, 
                                x * PANGO_SCALE,
                                y * PANGO_SCALE,
                                &index, NULL))
    {
      if (x < 0 || y < 0)
        index = 0;
      else
        index = -1;
    } 
  if (index == -1)
    offset = g_utf8_strlen (item_text, -1);
  else
    offset = g_utf8_pointer_to_offset (item_text, item_text + index);
#endif
  return offset;
}

static void
atk_text_item_interface_init (AtkTextIface *iface)
{
  iface->get_text = gtk_icon_view_item_accessible_text_get_text;
  iface->get_character_at_offset = gtk_icon_view_item_accessible_text_get_character_at_offset;
  iface->get_text_before_offset = gtk_icon_view_item_accessible_text_get_text_before_offset;
  iface->get_text_at_offset = gtk_icon_view_item_accessible_text_get_text_at_offset;
  iface->get_text_after_offset = gtk_icon_view_item_accessible_text_get_text_after_offset;
  iface->get_character_count = gtk_icon_view_item_accessible_text_get_character_count;
  iface->get_character_extents = gtk_icon_view_item_accessible_text_get_character_extents;
  iface->get_offset_at_point = gtk_icon_view_item_accessible_text_get_offset_at_point;
}

static void
gtk_icon_view_item_accessible_get_extents (AtkComponent *component,
                                           gint         *x,
                                           gint         *y,
                                           gint         *width,
                                           gint         *height,
                                           AtkCoordType  coord_type)
{
  GtkIconViewItemAccessible *item;
  AtkObject *parent_obj;
  gint l_x, l_y;

  g_return_if_fail (GTK_IS_ICON_VIEW_ITEM_ACCESSIBLE (component));

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (component);
  if (!GTK_IS_WIDGET (item->widget))
    return;

  if (atk_state_set_contains_state (item->state_set, ATK_STATE_DEFUNCT))
    return;

  *width = item->item->width;
  *height = item->item->height;
  if (gtk_icon_view_item_accessible_is_showing (item))
    {
      parent_obj = __gtk_widget_get_accessible (item->widget);
      atk_component_get_position (ATK_COMPONENT (parent_obj), &l_x, &l_y, coord_type);
      *x = l_x + item->item->x;
      *y = l_y + item->item->y;
    }
  else
    {
      *x = G_MININT;
      *y = G_MININT;
    }
}

static gboolean
gtk_icon_view_item_accessible_grab_focus (AtkComponent *component)
{
  GtkIconViewItemAccessible *item;
  GtkWidget *toplevel;

  g_return_val_if_fail (GTK_IS_ICON_VIEW_ITEM_ACCESSIBLE (component), FALSE);

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (component);
  if (!GTK_IS_WIDGET (item->widget))
    return FALSE;

  __gtk_widget_grab_focus (item->widget);
  __gtk_icon_view_set_cursor_item (GTK_ICON_VIEW (item->widget), item->item, -1);
  toplevel = __gtk_widget_get_toplevel (GTK_WIDGET (item->widget));
  if (__gtk_widget_is_toplevel (toplevel))
    __gtk_window_present (GTK_WINDOW (toplevel));

  return TRUE;
}

static void
atk_component_item_interface_init (AtkComponentIface *iface)
{
  iface->get_extents = gtk_icon_view_item_accessible_get_extents;
  iface->grab_focus = gtk_icon_view_item_accessible_grab_focus;
}

static gboolean
gtk_icon_view_item_accessible_add_state (GtkIconViewItemAccessible *item,
                                         AtkStateType               state_type,
                                         gboolean                   emit_signal)
{
  gboolean rc;

  rc = atk_state_set_add_state (item->state_set, state_type);
  /*
   * The signal should only be generated if the value changed,
   * not when the item is set up.  So states that are set
   * initially should pass FALSE as the emit_signal argument.
   */

  if (emit_signal)
    {
      atk_object_notify_state_change (ATK_OBJECT (item), state_type, TRUE);
      /* If state_type is ATK_STATE_VISIBLE, additional notification */
      if (state_type == ATK_STATE_VISIBLE)
        g_signal_emit_by_name (item, "visible-data-changed");
    }

  return rc;
}

static gboolean
gtk_icon_view_item_accessible_remove_state (GtkIconViewItemAccessible *item,
                                            AtkStateType               state_type,
                                            gboolean                   emit_signal)
{
  if (atk_state_set_contains_state (item->state_set, state_type))
    {
      gboolean rc;

      rc = atk_state_set_remove_state (item->state_set, state_type);
      /*
       * The signal should only be generated if the value changed,
       * not when the item is set up.  So states that are set
       * initially should pass FALSE as the emit_signal argument.
       */

      if (emit_signal)
        {
          atk_object_notify_state_change (ATK_OBJECT (item), state_type, FALSE);
          /* If state_type is ATK_STATE_VISIBLE, additional notification */
          if (state_type == ATK_STATE_VISIBLE)
            g_signal_emit_by_name (item, "visible-data-changed");
        }

      return rc;
    }
  else
    return FALSE;
}

static gboolean
gtk_icon_view_item_accessible_is_showing (GtkIconViewItemAccessible *item)
{
  GtkIconView *icon_view;
  GdkRectangle visible_rect;
  gboolean is_showing;

  /*
   * An item is considered "SHOWING" if any part of the item is in the
   * visible rectangle.
   */

  if (!GTK_IS_ICON_VIEW (item->widget))
    return FALSE;

  if (item->item == NULL)
    return FALSE;

  icon_view = GTK_ICON_VIEW (item->widget);
  visible_rect.x = 0;
  if (gtk_icon_view_get_props (icon_view)->priv->hadjustment)
    visible_rect.x += gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->hadjustment)->value;
  visible_rect.y = 0;
  if (gtk_icon_view_get_props (icon_view)->priv->hadjustment)
    visible_rect.y += gtk_adjustment_get_props (gtk_icon_view_get_props (icon_view)->priv->vadjustment)->value;
  visible_rect.width = gtk_widget_get_props (item->widget)->allocation.width;
  visible_rect.height = gtk_widget_get_props (item->widget)->allocation.height;

  if (((item->item->x + item->item->width) < visible_rect.x) ||
     ((item->item->y + item->item->height) < (visible_rect.y)) ||
     (item->item->x > (visible_rect.x + visible_rect.width)) ||
     (item->item->y > (visible_rect.y + visible_rect.height)))
    is_showing =  FALSE;
  else
    is_showing = TRUE;

  return is_showing;
}

static gboolean
gtk_icon_view_item_accessible_set_visibility (GtkIconViewItemAccessible *item,
                                              gboolean                   emit_signal)
{
  if (gtk_icon_view_item_accessible_is_showing (item))
    return gtk_icon_view_item_accessible_add_state (item, ATK_STATE_SHOWING,
						    emit_signal);
  else
    return gtk_icon_view_item_accessible_remove_state (item, ATK_STATE_SHOWING,
						       emit_signal);
}

static void
gtk_icon_view_item_accessible_object_init (GtkIconViewItemAccessible *item)
{
  gint i;

  item->state_set = atk_state_set_new ();

  atk_state_set_add_state (item->state_set, ATK_STATE_ENABLED);
  atk_state_set_add_state (item->state_set, ATK_STATE_FOCUSABLE);
  atk_state_set_add_state (item->state_set, ATK_STATE_SENSITIVE);
  atk_state_set_add_state (item->state_set, ATK_STATE_SELECTABLE);
  atk_state_set_add_state (item->state_set, ATK_STATE_VISIBLE);

  for (i = 0; i < LAST_ACTION; i++)
    item->action_descriptions[i] = NULL;

  item->image_description = NULL;

  item->action_idle_handler = 0;
}

static void
gtk_icon_view_item_accessible_finalize (GObject *object)
{
  GtkIconViewItemAccessible *item;
  gint i;

  g_return_if_fail (GTK_IS_ICON_VIEW_ITEM_ACCESSIBLE (object));

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (object);

  if (item->widget)
    g_object_remove_weak_pointer (G_OBJECT (item->widget), (gpointer) &item->widget);

  if (item->state_set)
    g_object_unref (item->state_set);

  if (item->text_buffer)
     g_object_unref (item->text_buffer);

  for (i = 0; i < LAST_ACTION; i++)
    g_free (item->action_descriptions[i]);

  g_free (item->image_description);

  if (item->action_idle_handler)
    {
      g_source_remove (item->action_idle_handler);
      item->action_idle_handler = 0;
    }

  G_OBJECT_CLASS (accessible_item_parent_class)->finalize (object);
}

static AtkObject*
gtk_icon_view_item_accessible_get_parent (AtkObject *obj)
{
  GtkIconViewItemAccessible *item;

  g_return_val_if_fail (GTK_IS_ICON_VIEW_ITEM_ACCESSIBLE (obj), NULL);
  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (obj);

  if (item->widget)
    return __gtk_widget_get_accessible (item->widget);
  else
    return NULL;
}

static gint
gtk_icon_view_item_accessible_get_index_in_parent (AtkObject *obj)
{
  GtkIconViewItemAccessible *item;

  g_return_val_if_fail (GTK_IS_ICON_VIEW_ITEM_ACCESSIBLE (obj), 0);
  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (obj);

  return item->item->index; 
}

static AtkStateSet *
gtk_icon_view_item_accessible_ref_state_set (AtkObject *obj)
{
  GtkIconViewItemAccessible *item;
  GtkIconView *icon_view;

  item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (obj);
  g_return_val_if_fail (item->state_set, NULL);

  if (!item->widget)
    return NULL;

  icon_view = GTK_ICON_VIEW (item->widget);
  if (gtk_icon_view_get_props (icon_view)->priv->cursor_item == item->item)
    atk_state_set_add_state (item->state_set, ATK_STATE_FOCUSED);
  else
    atk_state_set_remove_state (item->state_set, ATK_STATE_FOCUSED);
  if (item->item->selected)
    atk_state_set_add_state (item->state_set, ATK_STATE_SELECTED);
  else
    atk_state_set_remove_state (item->state_set, ATK_STATE_SELECTED);

  return g_object_ref (item->state_set);
}

static void
gtk_icon_view_item_accessible_class_init (AtkObjectClass *klass)
{
  GObjectClass *gobject_class;

  accessible_item_parent_class = g_type_class_peek_parent (klass);

  gobject_class = (GObjectClass *)klass;

  gobject_class->finalize = gtk_icon_view_item_accessible_finalize;

  klass->get_index_in_parent = gtk_icon_view_item_accessible_get_index_in_parent;
  klass->get_parent = gtk_icon_view_item_accessible_get_parent;
  klass->ref_state_set = gtk_icon_view_item_accessible_ref_state_set;
}

static GType
gtk_icon_view_item_accessible_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      const GTypeInfo tinfo =
      {
        sizeof (GtkIconViewItemAccessibleClass),
        (GBaseInitFunc) NULL, /* base init */
        (GBaseFinalizeFunc) NULL, /* base finalize */
        (GClassInitFunc) gtk_icon_view_item_accessible_class_init, /* class init */
        (GClassFinalizeFunc) NULL, /* class finalize */
        NULL, /* class data */
        sizeof (GtkIconViewItemAccessible), /* instance size */
        0, /* nb preallocs */
        (GInstanceInitFunc) gtk_icon_view_item_accessible_object_init, /* instance init */
        NULL /* value table */
      };

      const GInterfaceInfo atk_component_info =
      {
        (GInterfaceInitFunc) atk_component_item_interface_init,
        (GInterfaceFinalizeFunc) NULL,
        NULL
      };
      const GInterfaceInfo atk_action_info =
      {
        (GInterfaceInitFunc) atk_action_item_interface_init,
        (GInterfaceFinalizeFunc) NULL,
        NULL
      };
      const GInterfaceInfo atk_image_info =
      {
        (GInterfaceInitFunc) atk_image_item_interface_init,
        (GInterfaceFinalizeFunc) NULL,
        NULL
      };
      const GInterfaceInfo atk_text_info =
      {
        (GInterfaceInitFunc) atk_text_item_interface_init,
        (GInterfaceFinalizeFunc) NULL,
        NULL
      };

      type = g_type_register_static (ATK_TYPE_OBJECT,
                                     I_("GtkIconViewItemAccessible"), &tinfo, 0);
      g_type_add_interface_static (type, ATK_TYPE_COMPONENT,
                                   &atk_component_info);
      g_type_add_interface_static (type, ATK_TYPE_ACTION,
                                   &atk_action_info);
      g_type_add_interface_static (type, ATK_TYPE_IMAGE,
                                   &atk_image_info);
      g_type_add_interface_static (type, ATK_TYPE_TEXT,
                                   &atk_text_info);
    }

  return type;
}

#define GTK_TYPE_ICON_VIEW_ACCESSIBLE      (gtk_icon_view_accessible_get_type ())
#define GTK_ICON_VIEW_ACCESSIBLE(obj)      (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_ICON_VIEW_ACCESSIBLE, GtkIconViewAccessible))
#define GTK_IS_ICON_VIEW_ACCESSIBLE(obj)   (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_ICON_VIEW_ACCESSIBLE))

static GType gtk_icon_view_accessible_get_type (void);

typedef struct
{
   AtkObject parent;
} GtkIconViewAccessible;

typedef struct
{
  AtkObject *item;
  gint       index;
} GtkIconViewItemAccessibleInfo;

typedef struct
{
  GList *items;

  GtkAdjustment *old_hadj;
  GtkAdjustment *old_vadj;

  GtkTreeModel *model;

} GtkIconViewAccessiblePrivate;

static GtkIconViewAccessiblePrivate *
gtk_icon_view_accessible_get_priv (AtkObject *accessible)
{
  return g_object_get_qdata (G_OBJECT (accessible),
                             accessible_private_data_quark);
}

static void
gtk_icon_view_item_accessible_info_new (AtkObject *accessible,
                                        AtkObject *item,
                                        gint       index)
{
  GtkIconViewItemAccessibleInfo *info;
  GtkIconViewItemAccessibleInfo *tmp_info;
  GtkIconViewAccessiblePrivate *priv;
  GList *items;

  info = g_new (GtkIconViewItemAccessibleInfo, 1);
  info->item = item;
  info->index = index;

  priv = gtk_icon_view_accessible_get_priv (accessible);
  items = priv->items;
  while (items)
    {
      tmp_info = items->data;
      if (tmp_info->index > index)
        break;
      items = items->next;
    }
  priv->items = g_list_insert_before (priv->items, items, info);
  priv->old_hadj = NULL;
  priv->old_vadj = NULL;
}

static gint
gtk_icon_view_accessible_get_n_children (AtkObject *accessible)
{
  GtkIconView *icon_view;
  GtkWidget *widget;

  widget = GTK_ACCESSIBLE (accessible)->widget;
  if (!widget)
      return 0;

  icon_view = GTK_ICON_VIEW (widget);

  return g_list_length (gtk_icon_view_get_props (icon_view)->priv->items);
}

static AtkObject *
gtk_icon_view_accessible_find_child (AtkObject *accessible,
                                     gint       index)
{
  GtkIconViewAccessiblePrivate *priv;
  GtkIconViewItemAccessibleInfo *info;
  GList *items;

  priv = gtk_icon_view_accessible_get_priv (accessible);
  items = priv->items;

  while (items)
    {
      info = items->data;
      if (info->index == index)
        return info->item;
      items = items->next; 
    }
  return NULL;
}

static AtkObject *
gtk_icon_view_accessible_ref_child (AtkObject *accessible,
                                    gint       index)
{
  GtkIconView *icon_view;
  GtkWidget *widget;
  GList *icons;
  AtkObject *obj;
  GtkIconViewItemAccessible *a11y_item;

  widget = GTK_ACCESSIBLE (accessible)->widget;
  if (!widget)
    return NULL;

  icon_view = GTK_ICON_VIEW (widget);
  icons = g_list_nth (gtk_icon_view_get_props (icon_view)->priv->items, index);
  obj = NULL;
  if (icons)
    {
      GtkIconViewItem *item = icons->data;
   
      g_return_val_if_fail (item->index == index, NULL);
      obj = gtk_icon_view_accessible_find_child (accessible, index);
      if (!obj)
        {
          gchar *text;

          obj = g_object_new (gtk_icon_view_item_accessible_get_type (), NULL);
          gtk_icon_view_item_accessible_info_new (accessible,
                                                  obj,
                                                  index);
          obj->role = ATK_ROLE_ICON;
          a11y_item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (obj);
          a11y_item->item = item;
          a11y_item->widget = widget;
          a11y_item->text_buffer = __gtk_text_buffer_new (NULL);

	  gtk_icon_view_set_cell_data (icon_view, item);
          text = get_text (icon_view, item);
          if (text)
            {
              __gtk_text_buffer_set_text (a11y_item->text_buffer, text, -1);
              g_free (text);
            } 

          gtk_icon_view_item_accessible_set_visibility (a11y_item, FALSE);
          g_object_add_weak_pointer (G_OBJECT (widget), (gpointer) &(a11y_item->widget));
       }
      g_object_ref (obj);
    }
  return obj;
}

static void
gtk_icon_view_accessible_traverse_items (GtkIconViewAccessible *view,
                                         GList                 *list)
{
  GtkIconViewAccessiblePrivate *priv;
  GtkIconViewItemAccessibleInfo *info;
  GtkIconViewItemAccessible *item;
  GList *items;
  
  priv =  gtk_icon_view_accessible_get_priv (ATK_OBJECT (view));
  if (priv->items)
    {
      GtkWidget *widget;
      gboolean act_on_item;

      widget = GTK_ACCESSIBLE (view)->widget;
      if (widget == NULL)
        return;

      items = priv->items;

      act_on_item = (list == NULL);

      while (items)
        {

          info = (GtkIconViewItemAccessibleInfo *)items->data;
          item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (info->item);

          if (act_on_item == FALSE && list == items)
            act_on_item = TRUE;

          if (act_on_item)
	    gtk_icon_view_item_accessible_set_visibility (item, TRUE);

          items = items->next;
       }
   }
}

static void
gtk_icon_view_accessible_adjustment_changed (GtkAdjustment *adjustment,
                                             GtkIconView   *icon_view)
{
  AtkObject *obj;
  GtkIconViewAccessible *view;

  /*
   * The scrollbars have changed
   */
  obj = __gtk_widget_get_accessible (GTK_WIDGET (icon_view));
  view = GTK_ICON_VIEW_ACCESSIBLE (obj);

  gtk_icon_view_accessible_traverse_items (view, NULL);
}

static void
gtk_icon_view_accessible_set_scroll_adjustments (GtkWidget      *widget,
                                                 GtkAdjustment *hadj,
                                                 GtkAdjustment *vadj)
{
  AtkObject *atk_obj;
  GtkIconViewAccessiblePrivate *priv;

  atk_obj = __gtk_widget_get_accessible (widget);
  priv = gtk_icon_view_accessible_get_priv (atk_obj);

  if (priv->old_hadj != hadj)
    {
      if (priv->old_hadj)
        {
          g_object_remove_weak_pointer (G_OBJECT (priv->old_hadj),
                                        (gpointer *)&priv->old_hadj);
          
          g_signal_handlers_disconnect_by_func (priv->old_hadj,
                                                (gpointer) gtk_icon_view_accessible_adjustment_changed,
                                                widget);
        }
      priv->old_hadj = hadj;
      if (priv->old_hadj)
        {
          g_object_add_weak_pointer (G_OBJECT (priv->old_hadj),
                                     (gpointer *)&priv->old_hadj);
          g_signal_connect (hadj,
                            "value-changed",
                            G_CALLBACK (gtk_icon_view_accessible_adjustment_changed),
                            widget);
        }
    }
  if (priv->old_vadj != vadj)
    {
      if (priv->old_vadj)
        {
          g_object_remove_weak_pointer (G_OBJECT (priv->old_vadj),
                                        (gpointer *)&priv->old_vadj);
          
          g_signal_handlers_disconnect_by_func (priv->old_vadj,
                                                (gpointer) gtk_icon_view_accessible_adjustment_changed,
                                                widget);
        }
      priv->old_vadj = vadj;
      if (priv->old_vadj)
        {
          g_object_add_weak_pointer (G_OBJECT (priv->old_vadj),
                                     (gpointer *)&priv->old_vadj);
          g_signal_connect (vadj,
                            "value-changed",
                            G_CALLBACK (gtk_icon_view_accessible_adjustment_changed),
                            widget);
        }
    }
}

static void
gtk_icon_view_accessible_model_row_changed (GtkTreeModel *tree_model,
                                            GtkTreePath  *path,
                                            GtkTreeIter  *iter,
                                            gpointer      user_data)
{
  AtkObject *atk_obj;
  gint index;
  GtkWidget *widget;
  GtkIconView *icon_view;
  GtkIconViewItem *item;
  GtkIconViewAccessible *a11y_view;
  GtkIconViewItemAccessible *a11y_item;
  const gchar *name;
  gchar *text;

  atk_obj = __gtk_widget_get_accessible (GTK_WIDGET (user_data));
  a11y_view = GTK_ICON_VIEW_ACCESSIBLE (atk_obj);
  index = __gtk_tree_path_get_indices(path)[0];
  a11y_item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (
      gtk_icon_view_accessible_find_child (atk_obj, index));

  if (a11y_item)
    {
      widget = GTK_ACCESSIBLE (atk_obj)->widget;
      icon_view = GTK_ICON_VIEW (widget);
      item = a11y_item->item;

      name = atk_object_get_name (ATK_OBJECT (a11y_item));

      if (!name || strcmp (name, "") == 0)
        {
          gtk_icon_view_set_cell_data (icon_view, item);
          text = get_text (icon_view, item);
          if (text)
            {
              __gtk_text_buffer_set_text (a11y_item->text_buffer, text, -1);
              g_free (text);
            }
        }
    }

  g_signal_emit_by_name (atk_obj, "visible-data-changed");

  return;
}

static void
gtk_icon_view_accessible_model_row_inserted (GtkTreeModel *tree_model,
                                             GtkTreePath  *path,
                                             GtkTreeIter  *iter,
                                             gpointer     user_data)
{
  GtkIconViewAccessiblePrivate *priv;
  GtkIconViewItemAccessibleInfo *info;
  GtkIconViewAccessible *view;
  GtkIconViewItemAccessible *item;
  GList *items;
  GList *tmp_list;
  AtkObject *atk_obj;
  gint index;

  index = __gtk_tree_path_get_indices(path)[0];
  atk_obj = __gtk_widget_get_accessible (GTK_WIDGET (user_data));
  view = GTK_ICON_VIEW_ACCESSIBLE (atk_obj);
  priv = gtk_icon_view_accessible_get_priv (atk_obj);

  items = priv->items;
  tmp_list = NULL;
  while (items)
    {
      info = items->data;
      item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (info->item);
      if (info->index != item->item->index)
        {
          if (info->index < index)
            g_warning ("Unexpected index value on insertion %d %d", index, info->index);
 
          if (tmp_list == NULL)
            tmp_list = items;
   
          info->index = item->item->index;
        }

      items = items->next;
    }
  gtk_icon_view_accessible_traverse_items (view, tmp_list);
  g_signal_emit_by_name (atk_obj, "children-changed::add",
                         index, NULL, NULL);
  return;
}

static void
gtk_icon_view_accessible_model_row_deleted (GtkTreeModel *tree_model,
                                            GtkTreePath  *path,
                                            gpointer     user_data)
{
  GtkIconViewAccessiblePrivate *priv;
  GtkIconViewItemAccessibleInfo *info;
  GtkIconViewAccessible *view;
  GtkIconViewItemAccessible *item;
  GList *items;
  GList *tmp_list;
  GList *deleted_item;
  AtkObject *atk_obj;
  gint index;

  index = __gtk_tree_path_get_indices(path)[0];
  atk_obj = __gtk_widget_get_accessible (GTK_WIDGET (user_data));
  view = GTK_ICON_VIEW_ACCESSIBLE (atk_obj);
  priv = gtk_icon_view_accessible_get_priv (atk_obj);

  items = priv->items;
  tmp_list = NULL;
  deleted_item = NULL;
  info = NULL;
  while (items)
    {
      info = items->data;
      item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (info->item);
      if (info->index == index)
        {
          deleted_item = items;
        }
      if (info->index != item->item->index)
        {
          if (tmp_list == NULL)
            tmp_list = items;
            
          info->index = item->item->index;
        }

      items = items->next;
    }
  gtk_icon_view_accessible_traverse_items (view, tmp_list);
  if (deleted_item)
    {
      info = deleted_item->data;
      gtk_icon_view_item_accessible_add_state (GTK_ICON_VIEW_ITEM_ACCESSIBLE (info->item), ATK_STATE_DEFUNCT, TRUE);
      g_signal_emit_by_name (atk_obj, "children-changed::remove",
                             index, NULL, NULL);
      priv->items = g_list_remove_link (priv->items, deleted_item);
      g_free (info);
    }

  return;
}

static gint
gtk_icon_view_accessible_item_compare (GtkIconViewItemAccessibleInfo *i1,
                                       GtkIconViewItemAccessibleInfo *i2)
{
  return i1->index - i2->index;
}

static void
gtk_icon_view_accessible_model_rows_reordered (GtkTreeModel *tree_model,
                                               GtkTreePath  *path,
                                               GtkTreeIter  *iter,
                                               gint         *new_order,
                                               gpointer     user_data)
{
  GtkIconViewAccessiblePrivate *priv;
  GtkIconViewItemAccessibleInfo *info;
  GtkIconView *icon_view;
  GtkIconViewItemAccessible *item;
  GList *items;
  AtkObject *atk_obj;
  gint *order;
  gint length, i;

  atk_obj = __gtk_widget_get_accessible (GTK_WIDGET (user_data));
  icon_view = GTK_ICON_VIEW (user_data);
  priv = gtk_icon_view_accessible_get_priv (atk_obj);

  length = __gtk_tree_model_iter_n_children (tree_model, NULL);

  order = g_new (gint, length);
  for (i = 0; i < length; i++)
    order [new_order[i]] = i;

  items = priv->items;
  while (items)
    {
      info = items->data;
      item = GTK_ICON_VIEW_ITEM_ACCESSIBLE (info->item);
      info->index = order[info->index];
      item->item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items, info->index);
      items = items->next;
    }
  g_free (order);
  priv->items = g_list_sort (priv->items, 
                             (GCompareFunc)gtk_icon_view_accessible_item_compare);

  return;
}

static void
gtk_icon_view_accessible_disconnect_model_signals (GtkTreeModel *model,
                                                   GtkWidget *widget)
{
  GObject *obj;

  obj = G_OBJECT (model);
  g_signal_handlers_disconnect_by_func (obj, (gpointer) gtk_icon_view_accessible_model_row_changed, widget);
  g_signal_handlers_disconnect_by_func (obj, (gpointer) gtk_icon_view_accessible_model_row_inserted, widget);
  g_signal_handlers_disconnect_by_func (obj, (gpointer) gtk_icon_view_accessible_model_row_deleted, widget);
  g_signal_handlers_disconnect_by_func (obj, (gpointer) gtk_icon_view_accessible_model_rows_reordered, widget);
}

static void
gtk_icon_view_accessible_connect_model_signals (GtkIconView *icon_view)
{
  GObject *obj;

  obj = G_OBJECT (gtk_icon_view_get_props (icon_view)->priv->model);
  g_signal_connect_data (obj, "row-changed",
                         (GCallback) gtk_icon_view_accessible_model_row_changed,
                         icon_view, NULL, 0);
  g_signal_connect_data (obj, "row-inserted",
                         (GCallback) gtk_icon_view_accessible_model_row_inserted, 
                         icon_view, NULL, G_CONNECT_AFTER);
  g_signal_connect_data (obj, "row-deleted",
                         (GCallback) gtk_icon_view_accessible_model_row_deleted, 
                         icon_view, NULL, G_CONNECT_AFTER);
  g_signal_connect_data (obj, "rows-reordered",
                         (GCallback) gtk_icon_view_accessible_model_rows_reordered, 
                         icon_view, NULL, G_CONNECT_AFTER);
}

static void
gtk_icon_view_accessible_clear_cache (GtkIconViewAccessiblePrivate *priv)
{
  GtkIconViewItemAccessibleInfo *info;
  GList *items;

  items = priv->items;
  while (items)
    {
      info = (GtkIconViewItemAccessibleInfo *) items->data;
      g_object_unref (info->item);
      g_free (items->data);
      items = items->next;
    }
  g_list_free (priv->items);
  priv->items = NULL;
}

static void
gtk_icon_view_accessible_notify_gtk (GObject *obj,
                                     GParamSpec *pspec)
{
  GtkIconView *icon_view;
  GtkWidget *widget;
  AtkObject *atk_obj;
  GtkIconViewAccessiblePrivate *priv;

  if (strcmp (pspec->name, "model") == 0)
    {
      widget = GTK_WIDGET (obj); 
      atk_obj = __gtk_widget_get_accessible (widget);
      priv = gtk_icon_view_accessible_get_priv (atk_obj);
      if (priv->model)
        {
          g_object_remove_weak_pointer (G_OBJECT (priv->model),
                                        (gpointer *)&priv->model);
          gtk_icon_view_accessible_disconnect_model_signals (priv->model, widget);
        }
      gtk_icon_view_accessible_clear_cache (priv);

      icon_view = GTK_ICON_VIEW (obj);
      priv->model = gtk_icon_view_get_props (icon_view)->priv->model;
      /* If there is no model the GtkIconView is probably being destroyed */
      if (priv->model)
        {
          g_object_add_weak_pointer (G_OBJECT (priv->model), (gpointer *)&priv->model);
          gtk_icon_view_accessible_connect_model_signals (icon_view);
        }
    }

  return;
}

static void
gtk_icon_view_accessible_initialize (AtkObject *accessible,
                                     gpointer   data)
{
  GtkIconViewAccessiblePrivate *priv;
  GtkIconView *icon_view;

  if (ATK_OBJECT_CLASS (accessible_parent_class)->initialize)
    ATK_OBJECT_CLASS (accessible_parent_class)->initialize (accessible, data);

  priv = g_new0 (GtkIconViewAccessiblePrivate, 1);
  g_object_set_qdata (G_OBJECT (accessible),
                      accessible_private_data_quark,
                      priv);

  icon_view = GTK_ICON_VIEW (data);
  if (gtk_icon_view_get_props (icon_view)->priv->hadjustment)
    {
      priv->old_hadj = gtk_icon_view_get_props (icon_view)->priv->hadjustment;
      g_object_add_weak_pointer (G_OBJECT (priv->old_hadj), (gpointer *)&priv->old_hadj);
      g_signal_connect (gtk_icon_view_get_props (icon_view)->priv->hadjustment,
                        "value-changed",
                        G_CALLBACK (gtk_icon_view_accessible_adjustment_changed),
                        icon_view);
    } 
  if (gtk_icon_view_get_props (icon_view)->priv->vadjustment)
    {
      priv->old_vadj = gtk_icon_view_get_props (icon_view)->priv->vadjustment;
      g_object_add_weak_pointer (G_OBJECT (priv->old_vadj), (gpointer *)&priv->old_vadj);
      g_signal_connect (gtk_icon_view_get_props (icon_view)->priv->vadjustment,
                        "value-changed",
                        G_CALLBACK (gtk_icon_view_accessible_adjustment_changed),
                        icon_view);
    }
  g_signal_connect_after (data,
                          "set-scroll-adjustments",
                          G_CALLBACK (gtk_icon_view_accessible_set_scroll_adjustments),
                          NULL);
  g_signal_connect (data,
                    "notify",
                    G_CALLBACK (gtk_icon_view_accessible_notify_gtk),
                    NULL);

  priv->model = gtk_icon_view_get_props (icon_view)->priv->model;
  if (priv->model)
    {
      g_object_add_weak_pointer (G_OBJECT (priv->model), (gpointer *)&priv->model);
      gtk_icon_view_accessible_connect_model_signals (icon_view);
    }
                          
  accessible->role = ATK_ROLE_LAYERED_PANE;
}

static void
gtk_icon_view_accessible_finalize (GObject *object)
{
  GtkIconViewAccessiblePrivate *priv;

  priv = gtk_icon_view_accessible_get_priv (ATK_OBJECT (object));
  gtk_icon_view_accessible_clear_cache (priv);

  g_free (priv);

  G_OBJECT_CLASS (accessible_parent_class)->finalize (object);
}

static void
gtk_icon_view_accessible_destroyed (GtkWidget *widget,
                                    GtkAccessible *accessible)
{
  AtkObject *atk_obj;
  GtkIconViewAccessiblePrivate *priv;

  atk_obj = ATK_OBJECT (accessible);
  priv = gtk_icon_view_accessible_get_priv (atk_obj);
  if (priv->old_hadj)
    {
      g_object_remove_weak_pointer (G_OBJECT (priv->old_hadj),
                                    (gpointer *)&priv->old_hadj);
          
      g_signal_handlers_disconnect_by_func (priv->old_hadj,
                                            (gpointer) gtk_icon_view_accessible_adjustment_changed,
                                            widget);
      priv->old_hadj = NULL;
    }
  if (priv->old_vadj)
    {
      g_object_remove_weak_pointer (G_OBJECT (priv->old_vadj),
                                    (gpointer *)&priv->old_vadj);
          
      g_signal_handlers_disconnect_by_func (priv->old_vadj,
                                            (gpointer) gtk_icon_view_accessible_adjustment_changed,
                                            widget);
      priv->old_vadj = NULL;
    }
}

static void
gtk_icon_view_accessible_connect_widget_destroyed (GtkAccessible *accessible)
{
  if (accessible->widget)
    {
      g_signal_connect_after (accessible->widget,
                              "destroy",
                              G_CALLBACK (gtk_icon_view_accessible_destroyed),
                              accessible);
    }
  GTK_ACCESSIBLE_CLASS (accessible_parent_class)->connect_widget_destroyed (accessible);
}

static void
gtk_icon_view_accessible_class_init (AtkObjectClass *klass)
{
  GObjectClass *gobject_class;
  GtkAccessibleClass *accessible_class;

  accessible_parent_class = g_type_class_peek_parent (klass);

  gobject_class = (GObjectClass *)klass;
  accessible_class = (GtkAccessibleClass *)klass;

  gobject_class->finalize = gtk_icon_view_accessible_finalize;

  klass->get_n_children = gtk_icon_view_accessible_get_n_children;
  klass->ref_child = gtk_icon_view_accessible_ref_child;
  klass->initialize = gtk_icon_view_accessible_initialize;

  accessible_class->connect_widget_destroyed = gtk_icon_view_accessible_connect_widget_destroyed;

  accessible_private_data_quark = g_quark_from_static_string ("icon_view-accessible-private-data");
}

static AtkObject*
gtk_icon_view_accessible_ref_accessible_at_point (AtkComponent *component,
                                                  gint          x,
                                                  gint          y,
                                                  AtkCoordType  coord_type)
{
  GtkWidget *widget;
  GtkIconView *icon_view;
  GtkIconViewItem *item;
  gint x_pos, y_pos;

  widget = GTK_ACCESSIBLE (component)->widget;
  if (widget == NULL)
  /* State is defunct */
    return NULL;

  icon_view = GTK_ICON_VIEW (widget);
  atk_component_get_extents (component, &x_pos, &y_pos, NULL, NULL, coord_type);
  item = gtk_icon_view_get_item_at_coords (icon_view, x - x_pos, y - y_pos, TRUE, NULL);
  if (item)
    return gtk_icon_view_accessible_ref_child (ATK_OBJECT (component), item->index);

  return NULL;
}

static void
atk_component_interface_init (AtkComponentIface *iface)
{
  iface->ref_accessible_at_point = gtk_icon_view_accessible_ref_accessible_at_point;
}

static gboolean
gtk_icon_view_accessible_add_selection (AtkSelection *selection,
                                        gint i)
{
  GtkWidget *widget;
  GtkIconView *icon_view;
  GtkIconViewItem *item;

  widget = GTK_ACCESSIBLE (selection)->widget;
  if (widget == NULL)
    return FALSE;

  icon_view = GTK_ICON_VIEW (widget);

  item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items, i);

  if (!item)
    return FALSE;

  gtk_icon_view_select_item (icon_view, item);

  return TRUE;
}

static gboolean
gtk_icon_view_accessible_clear_selection (AtkSelection *selection)
{
  GtkWidget *widget;
  GtkIconView *icon_view;

  widget = GTK_ACCESSIBLE (selection)->widget;
  if (widget == NULL)
    return FALSE;

  icon_view = GTK_ICON_VIEW (widget);
  __gtk_icon_view_unselect_all (icon_view);

  return TRUE;
}

static AtkObject*
gtk_icon_view_accessible_ref_selection (AtkSelection *selection,
                                        gint          i)
{
  GList *l;
  GtkWidget *widget;
  GtkIconView *icon_view;
  GtkIconViewItem *item;

  widget = GTK_ACCESSIBLE (selection)->widget;
  if (widget == NULL)
    return NULL;

  icon_view = GTK_ICON_VIEW (widget);

  l = gtk_icon_view_get_props (icon_view)->priv->items;
  while (l)
    {
      item = l->data;
      if (item->selected)
        {
          if (i == 0)
	    return atk_object_ref_accessible_child (__gtk_widget_get_accessible (widget), item->index);
          else
            i--;
        }
      l = l->next;
    }

  return NULL;
}

static gint
gtk_icon_view_accessible_get_selection_count (AtkSelection *selection)
{
  GtkWidget *widget;
  GtkIconView *icon_view;
  GtkIconViewItem *item;
  GList *l;
  gint count;

  widget = GTK_ACCESSIBLE (selection)->widget;
  if (widget == NULL)
    return 0;

  icon_view = GTK_ICON_VIEW (widget);

  l = gtk_icon_view_get_props (icon_view)->priv->items;
  count = 0;
  while (l)
    {
      item = l->data;

      if (item->selected)
	count++;

      l = l->next;
    }

  return count;
}

static gboolean
gtk_icon_view_accessible_is_child_selected (AtkSelection *selection,
                                            gint          i)
{
  GtkWidget *widget;
  GtkIconView *icon_view;
  GtkIconViewItem *item;

  widget = GTK_ACCESSIBLE (selection)->widget;
  if (widget == NULL)
    return FALSE;

  icon_view = GTK_ICON_VIEW (widget);

  item = g_list_nth_data (gtk_icon_view_get_props (icon_view)->priv->items, i);
  if (!item)
    return FALSE;

  return item->selected;
}

static gboolean
gtk_icon_view_accessible_remove_selection (AtkSelection *selection,
                                           gint          i)
{
  GtkWidget *widget;
  GtkIconView *icon_view;
  GtkIconViewItem *item;
  GList *l;
  gint count;

  widget = GTK_ACCESSIBLE (selection)->widget;
  if (widget == NULL)
    return FALSE;

  icon_view = GTK_ICON_VIEW (widget);
  l = gtk_icon_view_get_props (icon_view)->priv->items;
  count = 0;
  while (l)
    {
      item = l->data;
      if (item->selected)
        {
          if (count == i)
            {
              gtk_icon_view_unselect_item (icon_view, item);
              return TRUE;
            }
          count++;
        }
      l = l->next;
    }

  return FALSE;
}
 
static gboolean
gtk_icon_view_accessible_select_all_selection (AtkSelection *selection)
{
  GtkWidget *widget;
  GtkIconView *icon_view;

  widget = GTK_ACCESSIBLE (selection)->widget;
  if (widget == NULL)
    return FALSE;

  icon_view = GTK_ICON_VIEW (widget);
  __gtk_icon_view_select_all (icon_view);
  return TRUE;
}

static void
gtk_icon_view_accessible_selection_interface_init (AtkSelectionIface *iface)
{
  iface->add_selection = gtk_icon_view_accessible_add_selection;
  iface->clear_selection = gtk_icon_view_accessible_clear_selection;
  iface->ref_selection = gtk_icon_view_accessible_ref_selection;
  iface->get_selection_count = gtk_icon_view_accessible_get_selection_count;
  iface->is_child_selected = gtk_icon_view_accessible_is_child_selected;
  iface->remove_selection = gtk_icon_view_accessible_remove_selection;
  iface->select_all_selection = gtk_icon_view_accessible_select_all_selection;
}

static GType
gtk_icon_view_accessible_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      GTypeInfo tinfo =
      {
        0, /* class size */
        (GBaseInitFunc) NULL, /* base init */
        (GBaseFinalizeFunc) NULL, /* base finalize */
        (GClassInitFunc) gtk_icon_view_accessible_class_init,
        (GClassFinalizeFunc) NULL, /* class finalize */
        NULL, /* class data */
        0, /* instance size */
        0, /* nb preallocs */
        (GInstanceInitFunc) NULL, /* instance init */
        NULL /* value table */
      };
      const GInterfaceInfo atk_component_info =
      {
        (GInterfaceInitFunc) atk_component_interface_init,
        (GInterfaceFinalizeFunc) NULL,
        NULL
      };
      const GInterfaceInfo atk_selection_info =
      {
        (GInterfaceInitFunc) gtk_icon_view_accessible_selection_interface_init,
        (GInterfaceFinalizeFunc) NULL,
        NULL
      };

      /*
       * Figure out the size of the class and instance
       * we are deriving from
       */
      AtkObjectFactory *factory;
      GType derived_type;
      GTypeQuery query;
      GType derived_atk_type;

      derived_type = g_type_parent (GTK_TYPE_ICON_VIEW);
      factory = atk_registry_get_factory (atk_get_default_registry (), 
                                          derived_type);
      derived_atk_type = atk_object_factory_get_accessible_type (factory);
      g_type_query (derived_atk_type, &query);
      tinfo.class_size = query.class_size;
      tinfo.instance_size = query.instance_size;
 
      type = g_type_register_static (derived_atk_type, 
                                     I_("GtkIconViewAccessible"), 
                                     &tinfo, 0);
      g_type_add_interface_static (type, ATK_TYPE_COMPONENT,
                                   &atk_component_info);
      g_type_add_interface_static (type, ATK_TYPE_SELECTION,
                                   &atk_selection_info);
    }
  return type;
}

static AtkObject *
gtk_icon_view_accessible_new (GObject *obj)
{
  AtkObject *accessible;

  g_return_val_if_fail (GTK_IS_WIDGET (obj), NULL);

  accessible = g_object_new (gtk_icon_view_accessible_get_type (), NULL);
  atk_object_initialize (accessible, obj);

  return accessible;
}

static GType
gtk_icon_view_accessible_factory_get_accessible_type (void)
{
  return gtk_icon_view_accessible_get_type ();
}

static AtkObject*
gtk_icon_view_accessible_factory_create_accessible (GObject *obj)
{
  return gtk_icon_view_accessible_new (obj);
}

static void
gtk_icon_view_accessible_factory_class_init (AtkObjectFactoryClass *klass)
{
  klass->create_accessible = gtk_icon_view_accessible_factory_create_accessible;
  klass->get_accessible_type = gtk_icon_view_accessible_factory_get_accessible_type;
}

static GType
gtk_icon_view_accessible_factory_get_type (void)
{
  static GType type = 0;

  if (!type)
    {
      const GTypeInfo tinfo =
      {
        sizeof (AtkObjectFactoryClass),
        NULL,           /* base_init */
        NULL,           /* base_finalize */
        (GClassInitFunc) gtk_icon_view_accessible_factory_class_init,
        NULL,           /* class_finalize */
        NULL,           /* class_data */
        sizeof (AtkObjectFactory),
        0,             /* n_preallocs */
        NULL, NULL
      };

      type = g_type_register_static (ATK_TYPE_OBJECT_FACTORY, 
                                    I_("GtkIconViewAccessibleFactory"),
                                    &tinfo, 0);
    }
  return type;
}


static AtkObject *
gtk_icon_view_get_accessible (GtkWidget *widget)
{
  static gboolean first_time = TRUE;

  if (first_time)
    {
      AtkObjectFactory *factory;
      AtkRegistry *registry;
      GType derived_type; 
      GType derived_atk_type; 

      /*
       * Figure out whether accessibility is enabled by looking at the
       * type of the accessible object which would be created for
       * the parent type of GtkIconView.
       */
      derived_type = g_type_parent (GTK_TYPE_ICON_VIEW);

      registry = atk_get_default_registry ();
      factory = atk_registry_get_factory (registry,
                                          derived_type);
      derived_atk_type = atk_object_factory_get_accessible_type (factory);
      if (g_type_is_a (derived_atk_type, GTK_TYPE_ACCESSIBLE)) 
	atk_registry_set_factory_type (registry, 
				       GTK_TYPE_ICON_VIEW,
				       gtk_icon_view_accessible_factory_get_type ());
      first_time = FALSE;
    } 
  return GTK_WIDGET_CLASS (gtk_icon_view_parent_class)->get_accessible (widget);
}

static gboolean
gtk_icon_view_buildable_custom_tag_start (GtkBuildable  *buildable,
					  GtkBuilder    *builder,
					  GObject       *child,
					  const gchar   *tagname,
					  GMarkupParser *parser,
					  gpointer      *data)
{
  if (parent_buildable_iface->custom_tag_start (buildable, builder, child,
						tagname, parser, data))
    return TRUE;

  return ___gtk_cell_layout_buildable_custom_tag_start (buildable, builder, child,
						      tagname, parser, data);
}

static void
gtk_icon_view_buildable_custom_tag_end (GtkBuildable *buildable,
					GtkBuilder   *builder,
					GObject      *child,
					const gchar  *tagname,
					gpointer     *data)
{
  if (strcmp (tagname, "attributes") == 0)
    ___gtk_cell_layout_buildable_custom_tag_end (buildable, builder, child, tagname,
					       data);
  else
    parent_buildable_iface->custom_tag_end (buildable, builder, child, tagname,
					    data);
}
