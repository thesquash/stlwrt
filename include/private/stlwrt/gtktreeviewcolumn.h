
/* gtktreeviewcolumn.h
 * Copyright (C) 2000  Red Hat, Inc.,  Jonathan Blandford <jrb@redhat.com>
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

#ifndef __GTK_TREE_VIEW_COLUMN_H__
#define __GTK_TREE_VIEW_COLUMN_H__

#include <gtkcellrenderer.h>
#include <gtktreemodel.h>
#include <gtktreesortable.h>


G_BEGIN_DECLS


#define GTK_TYPE_TREE_VIEW_COLUMN	     (gtk_tree_view_column_get_type ())
#define GTK_TREE_VIEW_COLUMN(obj)	     (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TREE_VIEW_COLUMN, GtkTreeViewColumn))
#define GTK_TREE_VIEW_COLUMN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TREE_VIEW_COLUMN, GtkTreeViewColumnClass))
#define GTK_IS_TREE_VIEW_COLUMN(obj)	     (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TREE_VIEW_COLUMN))
#define GTK_IS_TREE_VIEW_COLUMN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TREE_VIEW_COLUMN))
#define GTK_TREE_VIEW_COLUMN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TREE_VIEW_COLUMN, GtkTreeViewColumnClass))

typedef enum
{
  GTK_TREE_VIEW_COLUMN_GROW_ONLY,
  GTK_TREE_VIEW_COLUMN_AUTOSIZE,
  GTK_TREE_VIEW_COLUMN_FIXED
} GtkTreeViewColumnSizing;

typedef struct _GtkTreeViewColumn      GtkTreeViewColumnFat;
typedef struct _GtkTreeViewColumn      GtkTreeViewColumnThin;

typedef struct _GtkTreeViewColumnClass GtkTreeViewColumnClass;

typedef void (* GtkTreeCellDataFunc) (GtkTreeViewColumn *tree_column,
				      GtkCellRenderer   *cell,
				      GtkTreeModel      *tree_model,
				      GtkTreeIter       *iter,
				      gpointer           data);


/********************************************************************/
struct _GtkTreeViewColumnInstanceProps
{


  GtkWidget * (tree_view);
  GtkWidget * (button);
  GtkWidget * (child);
  GtkWidget * (arrow);
  GtkWidget * (alignment);
  GdkWindow * (window);
  GtkCellEditable * (editable_widget);
  gfloat  (xalign);
  guint  (property_changed_signal);
  gint  (spacing);

  /* Sizing fields */
  /* see gtk+/doc/tree-column-sizing.txt for more information on them */
  GtkTreeViewColumnSizing  (column_type);
  gint  (requested_width);
  gint  (button_request);
  gint  (resized_width);
  gint  (width);
  gint  (fixed_width);
  gint  (min_width);
  gint  (max_width);

  /* dragging columns */
  gint  (drag_x);
  gint  (drag_y);

  gchar * (title);
  GList * (cell_list);

  /* Sorting */
  guint  (sort_clicked_signal);
  guint  (sort_column_changed_signal);
  gint  (sort_column_id);
  GtkSortType  (sort_order);

  /* Flags */
  guint  (visible)             : 1;
  guint  (resizable)           : 1;
  guint  (clickable)           : 1;
  guint  (dirty)               : 1;
  guint  (show_sort_indicator) : 1;
  guint  (maybe_reordered)     : 1;
  guint  (reorderable)         : 1;
  guint  (use_resized_width)   : 1;
  guint  (expand)              : 1;
};

struct _GtkTreeViewColumnFat
{
  GObjectFat   parent;

  struct _GtkTreeViewColumnInstanceProps instance_properties;
};

struct _GtkTreeViewColumnThin
{
  GObjectThin  parent;

  gpointer reserved;
};


typedef union
{
  struct _GtkTreeViewColumnFat   fat_instance;
  struct _GtkTreeViewColumnThin  thin_instance;
}   GtkTreeViewColumn;
/********************************************************************/



struct _GtkTreeViewColumnClass
{
  GObjectClass parent_class;

  void (*clicked) (GtkTreeViewColumn *tree_column);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

GType                   _T2_gtk_tree_view_column_get_type            (void) G_GNUC_CONST;
GType                   _3T_gtk_tree_view_column_get_type            (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType                   gtk_tree_view_column_get_type            (void) G_GNUC_CONST;
GtkTreeViewColumn      *__gtk_tree_view_column_new                 (void);
GtkTreeViewColumn      *__gtk_tree_view_column_new_with_attributes (const gchar             *title,
								  GtkCellRenderer         *cell,
								  ...) G_GNUC_NULL_TERMINATED;
void                    __gtk_tree_view_column_pack_start          (GtkTreeViewColumn       *tree_column,
								  GtkCellRenderer         *cell,
								  gboolean                 expand);
void                    __gtk_tree_view_column_pack_end            (GtkTreeViewColumn       *tree_column,
								  GtkCellRenderer         *cell,
								  gboolean                 expand);
void                    __gtk_tree_view_column_clear               (GtkTreeViewColumn       *tree_column);
#ifndef GTK_DISABLE_DEPRECATED
GList                  *__gtk_tree_view_column_get_cell_renderers  (GtkTreeViewColumn       *tree_column);
#endif
void                    __gtk_tree_view_column_add_attribute       (GtkTreeViewColumn       *tree_column,
								  GtkCellRenderer         *cell_renderer,
								  const gchar             *attribute,
								  gint                     column);
void                    __gtk_tree_view_column_set_attributes      (GtkTreeViewColumn       *tree_column,
								  GtkCellRenderer         *cell_renderer,
								  ...) G_GNUC_NULL_TERMINATED;
void                    __gtk_tree_view_column_set_cell_data_func  (GtkTreeViewColumn       *tree_column,
								  GtkCellRenderer         *cell_renderer,
								  GtkTreeCellDataFunc      func,
								  gpointer                 func_data,
								  GDestroyNotify           destroy);
void                    __gtk_tree_view_column_clear_attributes    (GtkTreeViewColumn       *tree_column,
								  GtkCellRenderer         *cell_renderer);
void                    __gtk_tree_view_column_set_spacing         (GtkTreeViewColumn       *tree_column,
								  gint                     spacing);
gint                    __gtk_tree_view_column_get_spacing         (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_visible         (GtkTreeViewColumn       *tree_column,
								  gboolean                 visible);
gboolean                __gtk_tree_view_column_get_visible         (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_resizable       (GtkTreeViewColumn       *tree_column,
								  gboolean                 resizable);
gboolean                __gtk_tree_view_column_get_resizable       (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_sizing          (GtkTreeViewColumn       *tree_column,
								  GtkTreeViewColumnSizing  type);
GtkTreeViewColumnSizing __gtk_tree_view_column_get_sizing          (GtkTreeViewColumn       *tree_column);
gint                    __gtk_tree_view_column_get_width           (GtkTreeViewColumn       *tree_column);
gint                    __gtk_tree_view_column_get_fixed_width     (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_fixed_width     (GtkTreeViewColumn       *tree_column,
								  gint                     fixed_width);
void                    __gtk_tree_view_column_set_min_width       (GtkTreeViewColumn       *tree_column,
								  gint                     min_width);
gint                    __gtk_tree_view_column_get_min_width       (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_max_width       (GtkTreeViewColumn       *tree_column,
								  gint                     max_width);
gint                    __gtk_tree_view_column_get_max_width       (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_clicked             (GtkTreeViewColumn       *tree_column);



/* Options for manipulating the column headers
 */
void                    __gtk_tree_view_column_set_title           (GtkTreeViewColumn       *tree_column,
								  const gchar             *title);
const gchar *           __gtk_tree_view_column_get_title           (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_expand          (GtkTreeViewColumn       *tree_column,
								  gboolean                 expand);
gboolean                __gtk_tree_view_column_get_expand          (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_clickable       (GtkTreeViewColumn       *tree_column,
								  gboolean                 clickable);
gboolean                __gtk_tree_view_column_get_clickable       (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_widget          (GtkTreeViewColumn       *tree_column,
								  GtkWidget               *widget);
GtkWidget              *__gtk_tree_view_column_get_widget          (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_alignment       (GtkTreeViewColumn       *tree_column,
								  gfloat                   xalign);
gfloat                  __gtk_tree_view_column_get_alignment       (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_reorderable     (GtkTreeViewColumn       *tree_column,
								  gboolean                 reorderable);
gboolean                __gtk_tree_view_column_get_reorderable     (GtkTreeViewColumn       *tree_column);



/* You probably only want to use gtk_tree_view_column_set_sort_column_id.  The
 * other sorting functions exist primarily to let others do their own custom sorting.
 */
void                    __gtk_tree_view_column_set_sort_column_id  (GtkTreeViewColumn       *tree_column,
								  gint                     sort_column_id);
gint                    __gtk_tree_view_column_get_sort_column_id  (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_sort_indicator  (GtkTreeViewColumn       *tree_column,
								  gboolean                 setting);
gboolean                __gtk_tree_view_column_get_sort_indicator  (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_set_sort_order      (GtkTreeViewColumn       *tree_column,
								  GtkSortType              order);
GtkSortType             __gtk_tree_view_column_get_sort_order      (GtkTreeViewColumn       *tree_column);


/* These functions are meant primarily for interaction between the GtkTreeView and the column.
 */
void                    __gtk_tree_view_column_cell_set_cell_data  (GtkTreeViewColumn       *tree_column,
								  GtkTreeModel            *tree_model,
								  GtkTreeIter             *iter,
								  gboolean                 is_expander,
								  gboolean                 is_expanded);
void                    __gtk_tree_view_column_cell_get_size       (GtkTreeViewColumn       *tree_column,
								  const GdkRectangle      *cell_area,
								  gint                    *x_offset,
								  gint                    *y_offset,
								  gint                    *width,
								  gint                    *height);
gboolean                __gtk_tree_view_column_cell_is_visible     (GtkTreeViewColumn       *tree_column);
void                    __gtk_tree_view_column_focus_cell          (GtkTreeViewColumn       *tree_column,
								  GtkCellRenderer         *cell);
gboolean                __gtk_tree_view_column_cell_get_position   (GtkTreeViewColumn       *tree_column,
					                          GtkCellRenderer         *cell_renderer,
					                          gint                    *start_pos,
					                          gint                    *width);
void                    __gtk_tree_view_column_queue_resize        (GtkTreeViewColumn       *tree_column);
GtkWidget              *__gtk_tree_view_column_get_tree_view       (GtkTreeViewColumn       *tree_column);


G_END_DECLS


#endif /* __GTK_TREE_VIEW_COLUMN_H__ */
