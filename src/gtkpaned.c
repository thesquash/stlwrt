/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/.
 */

#include "config.h"

#include <stlwrt.h>

#include <gdkkeysyms.h>
#include <gtkbindings.h>
#include <gtkmain.h>

#include <gtkorientable.h>
#include <gtkpaned.h>
#include <gtkwindow.h>
#include <gtkprivate.h>
#include <gtkintl.h>


enum {
  PROP_0,
  PROP_ORIENTATION,
  PROP_POSITION,
  PROP_POSITION_SET,
  PROP_MIN_POSITION,
  PROP_MAX_POSITION
};

enum {
  CHILD_PROP_0,
  CHILD_PROP_RESIZE,
  CHILD_PROP_SHRINK
};

enum {
  CYCLE_CHILD_FOCUS,
  TOGGLE_HANDLE_FOCUS,
  MOVE_HANDLE,
  CYCLE_HANDLE_FOCUS,
  ACCEPT_POSITION,
  CANCEL_POSITION,
  LAST_SIGNAL
};

static void     gtk_paned_set_property          (GObject          *object,
						 guint             prop_id,
						 const GValue     *value,
						 GParamSpec       *pspec);
static void     gtk_paned_get_property          (GObject          *object,
						 guint             prop_id,
						 GValue           *value,
						 GParamSpec       *pspec);
static void     gtk_paned_set_child_property    (GtkContainer     *container,
                                                 GtkWidget        *child,
                                                 guint             property_id,
                                                 const GValue     *value,
                                                 GParamSpec       *pspec);
static void     gtk_paned_get_child_property    (GtkContainer     *container,
                                                 GtkWidget        *child,
                                                 guint             property_id,
                                                 GValue           *value,
                                                 GParamSpec       *pspec);
static void     gtk_paned_finalize              (GObject          *object);

static void     gtk_paned_size_request          (GtkWidget        *widget,
                                                 GtkRequisition   *requisition);
static void     gtk_paned_size_allocate         (GtkWidget        *widget,
                                                 GtkAllocation    *allocation);
static void     gtk_paned_realize               (GtkWidget        *widget);
static void     gtk_paned_unrealize             (GtkWidget        *widget);
static void     gtk_paned_map                   (GtkWidget        *widget);
static void     gtk_paned_unmap                 (GtkWidget        *widget);
static void     gtk_paned_state_changed         (GtkWidget        *widget,
                                                 GtkStateType      previous_state);
static gboolean gtk_paned_expose                (GtkWidget        *widget,
						 GdkEventExpose   *event);
static gboolean gtk_paned_enter                 (GtkWidget        *widget,
						 GdkEventCrossing *event);
static gboolean gtk_paned_leave                 (GtkWidget        *widget,
						 GdkEventCrossing *event);
static gboolean gtk_paned_button_press          (GtkWidget        *widget,
						 GdkEventButton   *event);
static gboolean gtk_paned_button_release        (GtkWidget        *widget,
						 GdkEventButton   *event);
static gboolean gtk_paned_motion                (GtkWidget        *widget,
						 GdkEventMotion   *event);
static gboolean gtk_paned_focus                 (GtkWidget        *widget,
						 GtkDirectionType  direction);
static gboolean gtk_paned_grab_broken           (GtkWidget          *widget,
						 GdkEventGrabBroken *event);
static void     gtk_paned_add                   (GtkContainer     *container,
						 GtkWidget        *widget);
static void     gtk_paned_remove                (GtkContainer     *container,
						 GtkWidget        *widget);
static void     gtk_paned_forall                (GtkContainer     *container,
						 gboolean          include_internals,
						 GtkCallback       callback,
						 gpointer          callback_data);
static void     gtk_paned_calc_position         (GtkPaned         *paned,
                                                 gint              allocation,
                                                 gint              child1_req,
                                                 gint              child2_req);
static void     gtk_paned_set_focus_child       (GtkContainer     *container,
						 GtkWidget        *child);
static void     gtk_paned_set_saved_focus       (GtkPaned         *paned,
						 GtkWidget        *widget);
static void     gtk_paned_set_first_paned       (GtkPaned         *paned,
						 GtkPaned         *first_paned);
static void     gtk_paned_set_last_child1_focus (GtkPaned         *paned,
						 GtkWidget        *widget);
static void     gtk_paned_set_last_child2_focus (GtkPaned         *paned,
						 GtkWidget        *widget);
static gboolean gtk_paned_cycle_child_focus     (GtkPaned         *paned,
						 gboolean          reverse);
static gboolean gtk_paned_cycle_handle_focus    (GtkPaned         *paned,
						 gboolean          reverse);
static gboolean gtk_paned_move_handle           (GtkPaned         *paned,
						 GtkScrollType     scroll);
static gboolean gtk_paned_accept_position       (GtkPaned         *paned);
static gboolean gtk_paned_cancel_position       (GtkPaned         *paned);
static gboolean gtk_paned_toggle_handle_focus   (GtkPaned         *paned);

static GType    gtk_paned_child_type            (GtkContainer     *container);
static void     gtk_paned_grab_notify           (GtkWidget        *widget,
		                                 gboolean          was_grabbed);



STLWRT_DEFINE_VTYPE (GtkPaned, gtk_paned, GTK_TYPE_CONTAINER, G_TYPE_FLAG_NONE,
                     G_IMPLEMENT_INTERFACE (GTK_TYPE_ORIENTABLE, NULL))

static guint signals[LAST_SIGNAL] = { 0 };


static void
add_tab_bindings (GtkBindingSet    *binding_set,
		  GdkModifierType   modifiers)
{
  __gtk_binding_entry_add_signal (binding_set, GDK_Tab, modifiers,
                                "toggle-handle-focus", 0);
  __gtk_binding_entry_add_signal (binding_set, GDK_KP_Tab, modifiers,
				"toggle-handle-focus", 0);
}

static void
add_move_binding (GtkBindingSet   *binding_set,
		  guint            keyval,
		  GdkModifierType  mask,
		  GtkScrollType    scroll)
{
  __gtk_binding_entry_add_signal (binding_set, keyval, mask,
				"move-handle", 1,
				GTK_TYPE_SCROLL_TYPE, scroll);
}

static void
gtk_paned_class_init (GtkPanedClass *class)
{
  GObjectClass *object_class;
  GtkWidgetClass *widget_class;
  GtkContainerClass *container_class;
  GtkPanedClass *paned_class;
  GtkBindingSet *binding_set;

  object_class = (GObjectClass *) class;
  widget_class = (GtkWidgetClass *) class;
  container_class = (GtkContainerClass *) class;
  paned_class = (GtkPanedClass *) class;

  object_class->set_property = gtk_paned_set_property;
  object_class->get_property = gtk_paned_get_property;
  object_class->finalize = gtk_paned_finalize;

  widget_class->size_request = gtk_paned_size_request;
  widget_class->size_allocate = gtk_paned_size_allocate;
  widget_class->realize = gtk_paned_realize;
  widget_class->unrealize = gtk_paned_unrealize;
  widget_class->map = gtk_paned_map;
  widget_class->unmap = gtk_paned_unmap;
  widget_class->expose_event = gtk_paned_expose;
  widget_class->focus = gtk_paned_focus;
  widget_class->enter_notify_event = gtk_paned_enter;
  widget_class->leave_notify_event = gtk_paned_leave;
  widget_class->button_press_event = gtk_paned_button_press;
  widget_class->button_release_event = gtk_paned_button_release;
  widget_class->motion_notify_event = gtk_paned_motion;
  widget_class->grab_broken_event = gtk_paned_grab_broken;
  widget_class->grab_notify = gtk_paned_grab_notify;
  widget_class->state_changed = gtk_paned_state_changed;

  container_class->add = gtk_paned_add;
  container_class->remove = gtk_paned_remove;
  container_class->forall = gtk_paned_forall;
  container_class->child_type = gtk_paned_child_type;
  container_class->set_focus_child = gtk_paned_set_focus_child;
  container_class->set_child_property = gtk_paned_set_child_property;
  container_class->get_child_property = gtk_paned_get_child_property;

  paned_class->cycle_child_focus = gtk_paned_cycle_child_focus;
  paned_class->toggle_handle_focus = gtk_paned_toggle_handle_focus;
  paned_class->move_handle = gtk_paned_move_handle;
  paned_class->cycle_handle_focus = gtk_paned_cycle_handle_focus;
  paned_class->accept_position = gtk_paned_accept_position;
  paned_class->cancel_position = gtk_paned_cancel_position;

  g_object_class_override_property (object_class,
                                    PROP_ORIENTATION,
                                    "orientation");

  g_object_class_install_property (object_class,
				   PROP_POSITION,
				   g_param_spec_int ("position",
						     P_("Position"),
						     P_("Position of paned separator in pixels (0 means all the way to the left/top)"),
						     0,
						     G_MAXINT,
						     0,
						     GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_POSITION_SET,
				   g_param_spec_boolean ("position-set",
							 P_("Position Set"),
							 P_("TRUE if the Position property should be used"),
							 FALSE,
							 GTK_PARAM_READWRITE));

  __gtk_widget_class_install_style_property (widget_class,
					   g_param_spec_int ("handle-size",
							     P_("Handle Size"),
							     P_("Width of handle"),
							     0,
							     G_MAXINT,
							     5,
							     GTK_PARAM_READABLE));
  /**
   * GtkPaned:min-position:
   *
   * The smallest possible value for the position property. This property is derived from the
   * size and shrinkability of the widget's children.
   *
   * Since: 2.4
   */
  g_object_class_install_property (object_class,
				   PROP_MIN_POSITION,
				   g_param_spec_int ("min-position",
						     P_("Minimal Position"),
						     P_("Smallest possible value for the \"position\" property"),
						     0,
						     G_MAXINT,
						     0,
						     GTK_PARAM_READABLE));

  /**
   * GtkPaned:max-position:
   *
   * The largest possible value for the position property. This property is derived from the
   * size and shrinkability of the widget's children.
   *
   * Since: 2.4
   */
  g_object_class_install_property (object_class,
				   PROP_MAX_POSITION,
				   g_param_spec_int ("max-position",
						     P_("Maximal Position"),
						     P_("Largest possible value for the \"position\" property"),
						     0,
						     G_MAXINT,
						     G_MAXINT,
						     GTK_PARAM_READABLE));

  /**
   * GtkPaned:resize:
   *
   * The "resize" child property determines whether the child expands and
   * shrinks along with the paned widget.
   *
   * Since: 2.4
   */
  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_RESIZE,
					      g_param_spec_boolean ("resize", 
								    P_("Resize"),
								    P_("If TRUE, the child expands and shrinks along with the paned widget"),
								    TRUE,
								    GTK_PARAM_READWRITE));

  /**
   * GtkPaned:shrink:
   *
   * The "shrink" child property determines whether the child can be made
   * smaller than its requisition.
   *
   * Since: 2.4
   */
  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_SHRINK,
					      g_param_spec_boolean ("shrink", 
								    P_("Shrink"),
								    P_("If TRUE, the child can be made smaller than its requisition"),
								    TRUE,
								    GTK_PARAM_READWRITE));

  /**
   * GtkPaned::cycle-child-focus:
   * @widget: the object that received the signal
   * @reversed: whether cycling backward or forward
   *
   * The ::cycle-child-focus signal is a 
   * <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted to cycle the focus between the children of the paned.
   *
   * The default binding is f6.
   *
   * Since: 2.0
   */
  signals [CYCLE_CHILD_FOCUS] =
    g_signal_new (I_("cycle-child-focus"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkPanedClass, cycle_child_focus),
		  NULL, NULL,
		  NULL,
		  G_TYPE_BOOLEAN, 1,
		  G_TYPE_BOOLEAN);

  /**
   * GtkPaned::toggle-handle-focus:
   * @widget: the object that received the signal
   *
   * The ::toggle-handle-focus is a 
   * <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted to accept the current position of the handle and then 
   * move focus to the next widget in the focus chain.
   *
   * The default binding is Tab.
   *
   * Since: 2.0
   */
  signals [TOGGLE_HANDLE_FOCUS] =
    g_signal_new (I_("toggle-handle-focus"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkPanedClass, toggle_handle_focus),
		  NULL, NULL,
		  NULL,
		  G_TYPE_BOOLEAN, 0);

  /**
   * GtkPaned::move-handle:
   * @widget: the object that received the signal
   * @scroll_type: a #GtkScrollType
   *
   * The ::move-handle signal is a 
   * <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted to move the handle when the user is using key bindings 
   * to move it.
   *
   * Since: 2.0
   */
  signals[MOVE_HANDLE] =
    g_signal_new (I_("move-handle"),
		  G_TYPE_FROM_CLASS (object_class),
                  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
                  G_STRUCT_OFFSET (GtkPanedClass, move_handle),
                  NULL, NULL,
                  NULL,
                  G_TYPE_BOOLEAN, 1,
                  GTK_TYPE_SCROLL_TYPE);

  /**
   * GtkPaned::cycle-handle-focus:
   * @widget: the object that received the signal
   * @reversed: whether cycling backward or forward
   *
   * The ::cycle-handle-focus signal is a 
   * <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted to cycle whether the paned should grab focus to allow
   * the user to change position of the handle by using key bindings.
   *
   * The default binding for this signal is f8.
   *
   * Since: 2.0
   */
  signals [CYCLE_HANDLE_FOCUS] =
    g_signal_new (I_("cycle-handle-focus"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkPanedClass, cycle_handle_focus),
		  NULL, NULL,
		  NULL,
		  G_TYPE_BOOLEAN, 1,
		  G_TYPE_BOOLEAN);

  /**
   * GtkPaned::accept-position:
   * @widget: the object that received the signal
   *
   * The ::accept-position signal is a 
   * <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted to accept the current position of the handle when 
   * moving it using key bindings.
   *
   * The default binding for this signal is Return or Space.
   *
   * Since: 2.0
   */
  signals [ACCEPT_POSITION] =
    g_signal_new (I_("accept-position"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkPanedClass, accept_position),
		  NULL, NULL,
		  NULL,
		  G_TYPE_BOOLEAN, 0);

  /**
   * GtkPaned::cancel-position:
   * @widget: the object that received the signal
   *
   * The ::cancel-position signal is a 
   * <link linkend="keybinding-signals">keybinding signal</link>
   * which gets emitted to cancel moving the position of the handle using key 
   * bindings. The position of the handle will be reset to the value prior to 
   * moving it.
   *
   * The default binding for this signal is Escape.
   *
   * Since: 2.0
   */
  signals [CANCEL_POSITION] =
    g_signal_new (I_("cancel-position"),
		  G_TYPE_FROM_CLASS (object_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkPanedClass, cancel_position),
		  NULL, NULL,
		  NULL,
		  G_TYPE_BOOLEAN, 0);

  binding_set = __gtk_binding_set_by_class (class);

  /* F6 and friends */
  __gtk_binding_entry_add_signal (binding_set,
                                GDK_F6, 0,
                                "cycle-child-focus", 1, 
                                G_TYPE_BOOLEAN, FALSE);
  __gtk_binding_entry_add_signal (binding_set,
				GDK_F6, GDK_SHIFT_MASK,
				"cycle-child-focus", 1,
				G_TYPE_BOOLEAN, TRUE);

  /* F8 and friends */
  __gtk_binding_entry_add_signal (binding_set,
				GDK_F8, 0,
				"cycle-handle-focus", 1,
				G_TYPE_BOOLEAN, FALSE);
 
  __gtk_binding_entry_add_signal (binding_set,
				GDK_F8, GDK_SHIFT_MASK,
				"cycle-handle-focus", 1,
				G_TYPE_BOOLEAN, TRUE);
 
  add_tab_bindings (binding_set, 0);
  add_tab_bindings (binding_set, GDK_CONTROL_MASK);
  add_tab_bindings (binding_set, GDK_SHIFT_MASK);
  add_tab_bindings (binding_set, GDK_CONTROL_MASK | GDK_SHIFT_MASK);

  /* accept and cancel positions */
  __gtk_binding_entry_add_signal (binding_set,
				GDK_Escape, 0,
				"cancel-position", 0);

  __gtk_binding_entry_add_signal (binding_set,
				GDK_Return, 0,
				"accept-position", 0);
  __gtk_binding_entry_add_signal (binding_set,
				GDK_ISO_Enter, 0,
				"accept-position", 0);
  __gtk_binding_entry_add_signal (binding_set,
				GDK_KP_Enter, 0,
				"accept-position", 0);
  __gtk_binding_entry_add_signal (binding_set,
				GDK_space, 0,
				"accept-position", 0);
  __gtk_binding_entry_add_signal (binding_set,
				GDK_KP_Space, 0,
				"accept-position", 0);

  /* move handle */
  add_move_binding (binding_set, GDK_Left, 0, GTK_SCROLL_STEP_LEFT);
  add_move_binding (binding_set, GDK_KP_Left, 0, GTK_SCROLL_STEP_LEFT);
  add_move_binding (binding_set, GDK_Left, GDK_CONTROL_MASK, GTK_SCROLL_PAGE_LEFT);
  add_move_binding (binding_set, GDK_KP_Left, GDK_CONTROL_MASK, GTK_SCROLL_PAGE_LEFT);

  add_move_binding (binding_set, GDK_Right, 0, GTK_SCROLL_STEP_RIGHT);
  add_move_binding (binding_set, GDK_Right, GDK_CONTROL_MASK, GTK_SCROLL_PAGE_RIGHT);
  add_move_binding (binding_set, GDK_KP_Right, 0, GTK_SCROLL_STEP_RIGHT);
  add_move_binding (binding_set, GDK_KP_Right, GDK_CONTROL_MASK, GTK_SCROLL_PAGE_RIGHT);

  add_move_binding (binding_set, GDK_Up, 0, GTK_SCROLL_STEP_UP);
  add_move_binding (binding_set, GDK_Up, GDK_CONTROL_MASK, GTK_SCROLL_PAGE_UP);
  add_move_binding (binding_set, GDK_KP_Up, 0, GTK_SCROLL_STEP_UP);
  add_move_binding (binding_set, GDK_KP_Up, GDK_CONTROL_MASK, GTK_SCROLL_PAGE_UP);
  add_move_binding (binding_set, GDK_Page_Up, 0, GTK_SCROLL_PAGE_UP);
  add_move_binding (binding_set, GDK_KP_Page_Up, 0, GTK_SCROLL_PAGE_UP);

  add_move_binding (binding_set, GDK_Down, 0, GTK_SCROLL_STEP_DOWN);
  add_move_binding (binding_set, GDK_Down, GDK_CONTROL_MASK, GTK_SCROLL_PAGE_DOWN);
  add_move_binding (binding_set, GDK_KP_Down, 0, GTK_SCROLL_STEP_DOWN);
  add_move_binding (binding_set, GDK_KP_Down, GDK_CONTROL_MASK, GTK_SCROLL_PAGE_DOWN);
  add_move_binding (binding_set, GDK_Page_Down, 0, GTK_SCROLL_PAGE_RIGHT);
  add_move_binding (binding_set, GDK_KP_Page_Down, 0, GTK_SCROLL_PAGE_RIGHT);

  add_move_binding (binding_set, GDK_Home, 0, GTK_SCROLL_START);
  add_move_binding (binding_set, GDK_KP_Home, 0, GTK_SCROLL_START);
  add_move_binding (binding_set, GDK_End, 0, GTK_SCROLL_END);
  add_move_binding (binding_set, GDK_KP_End, 0, GTK_SCROLL_END);

  g_type_class_add_private (object_class, sizeof (GtkPanedPrivate));
}

static GType
gtk_paned_child_type (GtkContainer *container)
{
  if (!GTK_PANED (container)->child1 || !GTK_PANED (container)->child2)
    return GTK_TYPE_WIDGET;
  else
    return G_TYPE_NONE;
}

static void
gtk_paned_init (GtkPaned *paned)
{
  __gtk_widget_set_has_window (GTK_WIDGET (paned), FALSE);
  __gtk_widget_set_can_focus (GTK_WIDGET (paned), TRUE);

  /* We only need to redraw when the handle position moves, which is
   * independent of the overall allocation of the GtkPaned
   */
  __gtk_widget_set_redraw_on_allocate (GTK_WIDGET (paned), FALSE);

  gtk_paned_get_props (paned)->priv = G_TYPE_INSTANCE_GET_PRIVATE (gtk_paned_get_props (paned), GTK_TYPE_PANED, GtkPanedPrivate);

  gtk_paned_get_props (paned)->priv->orientation = GTK_ORIENTATION_HORIZONTAL;
  gtk_paned_get_props (paned)->cursor_type = GDK_SB_H_DOUBLE_ARROW;

  gtk_paned_get_props (paned)->child1 = NULL;
  gtk_paned_get_props (paned)->child2 = NULL;
  gtk_paned_get_props (paned)->handle = NULL;
  gtk_paned_get_props (paned)->xor_gc = NULL;
  gtk_paned_get_props (paned)->cursor_type = GDK_CROSS;
  
  gtk_paned_get_props (paned)->handle_pos.width = 5;
  gtk_paned_get_props (paned)->handle_pos.height = 5;
  gtk_paned_get_props (paned)->position_set = FALSE;
  gtk_paned_get_props (paned)->last_allocation = -1;
  gtk_paned_get_props (paned)->in_drag = FALSE;

  gtk_paned_get_props (paned)->last_child1_focus = NULL;
  gtk_paned_get_props (paned)->last_child2_focus = NULL;
  gtk_paned_get_props (paned)->in_recursion = FALSE;
  gtk_paned_get_props (paned)->handle_prelit = FALSE;
  gtk_paned_get_props (paned)->original_position = -1;
  
  gtk_paned_get_props (paned)->handle_pos.x = -1;
  gtk_paned_get_props (paned)->handle_pos.y = -1;

  gtk_paned_get_props (paned)->drag_pos = -1;
}

static void
gtk_paned_set_property (GObject        *object,
			guint           prop_id,
			const GValue   *value,
			GParamSpec     *pspec)
{
  GtkPaned *paned = GTK_PANED (object);

  switch (prop_id)
    {
    case PROP_ORIENTATION:
      gtk_paned_get_props (paned)->priv->orientation = g_value_get_enum (value);
      gtk_paned_get_props (paned)->orientation = !gtk_paned_get_props (paned)->priv->orientation;

      if (gtk_paned_get_props (paned)->priv->orientation == GTK_ORIENTATION_HORIZONTAL)
        gtk_paned_get_props (paned)->cursor_type = GDK_SB_H_DOUBLE_ARROW;
      else
        gtk_paned_get_props (paned)->cursor_type = GDK_SB_V_DOUBLE_ARROW;

      /* state_changed updates the cursor */
      gtk_paned_state_changed (GTK_WIDGET (paned), GTK_WIDGET (paned)->state);
      __gtk_widget_queue_resize (GTK_WIDGET (paned));
      break;
    case PROP_POSITION:
      __gtk_paned_set_position (paned, g_value_get_int (value));
      break;
    case PROP_POSITION_SET:
      gtk_paned_get_props (paned)->position_set = g_value_get_boolean (value);
      __gtk_widget_queue_resize_no_redraw (GTK_WIDGET (paned));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_paned_get_property (GObject        *object,
			guint           prop_id,
			GValue         *value,
			GParamSpec     *pspec)
{
  GtkPaned *paned = GTK_PANED (object);

  switch (prop_id)
    {
    case PROP_ORIENTATION:
      g_value_set_enum (value, gtk_paned_get_props (paned)->priv->orientation);
      break;
    case PROP_POSITION:
      g_value_set_int (value, gtk_paned_get_props (paned)->child1_size);
      break;
    case PROP_POSITION_SET:
      g_value_set_boolean (value, gtk_paned_get_props (paned)->position_set);
      break;
    case PROP_MIN_POSITION:
      g_value_set_int (value, gtk_paned_get_props (paned)->min_position);
      break;
    case PROP_MAX_POSITION:
      g_value_set_int (value, gtk_paned_get_props (paned)->max_position);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_paned_set_child_property (GtkContainer    *container,
			      GtkWidget       *child,
			      guint            property_id,
			      const GValue    *value,
			      GParamSpec      *pspec)
{
  GtkPaned *paned = GTK_PANED (container);
  gboolean old_value, new_value;

  g_assert (child == gtk_paned_get_props (paned)->child1 || child == gtk_paned_get_props (paned)->child2);

  new_value = g_value_get_boolean (value);
  switch (property_id)
    {
    case CHILD_PROP_RESIZE:
      if (child == gtk_paned_get_props (paned)->child1)
	{
	  old_value = gtk_paned_get_props (paned)->child1_resize;
	  gtk_paned_get_props (paned)->child1_resize = new_value;
	}
      else
	{
	  old_value = gtk_paned_get_props (paned)->child2_resize;
	  gtk_paned_get_props (paned)->child2_resize = new_value;
	}
      break;
    case CHILD_PROP_SHRINK:
      if (child == gtk_paned_get_props (paned)->child1)
	{
	  old_value = gtk_paned_get_props (paned)->child1_shrink;
	  gtk_paned_get_props (paned)->child1_shrink = new_value;
	}
      else
	{
	  old_value = gtk_paned_get_props (paned)->child2_shrink;
	  gtk_paned_get_props (paned)->child2_shrink = new_value;
	}
      break;
    default:
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID (container, property_id, pspec);
      old_value = -1; /* quiet gcc */
      break;
    }
  if (old_value != new_value)
    __gtk_widget_queue_resize_no_redraw (GTK_WIDGET (container));
}

static void
gtk_paned_get_child_property (GtkContainer *container,
			      GtkWidget    *child,
			      guint         property_id,
			      GValue       *value,
			      GParamSpec   *pspec)
{
  GtkPaned *paned = GTK_PANED (container);

  g_assert (child == gtk_paned_get_props (paned)->child1 || child == gtk_paned_get_props (paned)->child2);
  
  switch (property_id)
    {
    case CHILD_PROP_RESIZE:
      if (child == gtk_paned_get_props (paned)->child1)
	g_value_set_boolean (value, gtk_paned_get_props (paned)->child1_resize);
      else
	g_value_set_boolean (value, gtk_paned_get_props (paned)->child2_resize);
      break;
    case CHILD_PROP_SHRINK:
      if (child == gtk_paned_get_props (paned)->child1)
	g_value_set_boolean (value, gtk_paned_get_props (paned)->child1_shrink);
      else
	g_value_set_boolean (value, gtk_paned_get_props (paned)->child2_shrink);
      break;
    default:
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID (container, property_id, pspec);
      break;
    }
}

static void
gtk_paned_finalize (GObject *object)
{
  GtkPaned *paned = GTK_PANED (object);
  
  gtk_paned_set_saved_focus (paned, NULL);
  gtk_paned_set_first_paned (paned, NULL);

  G_OBJECT_CLASS (gtk_paned_parent_class)->finalize (object);
}

static void
gtk_paned_size_request (GtkWidget      *widget,
                        GtkRequisition *requisition)
{
  GtkPaned *paned = GTK_PANED (widget);
  GtkRequisition child_requisition;

  requisition->width = 0;
  requisition->height = 0;

  if (gtk_paned_get_props (paned)->child1 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child1))
    {
      __gtk_widget_size_request (gtk_paned_get_props (paned)->child1, &child_requisition);

      requisition->height = child_requisition.height;
      requisition->width = child_requisition.width;
    }

  if (gtk_paned_get_props (paned)->child2 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child2))
    {
      __gtk_widget_size_request (gtk_paned_get_props (paned)->child2, &child_requisition);

      if (gtk_paned_get_props (paned)->priv->orientation == GTK_ORIENTATION_HORIZONTAL)
        {
          requisition->height = MAX (requisition->height,
                                     child_requisition.height);
          requisition->width += child_requisition.width;
        }
      else
        {
          requisition->width = MAX (requisition->width,
                                    child_requisition.width);
          requisition->height += child_requisition.height;
        }
    }

  requisition->width += GTK_CONTAINER (paned)->border_width * 2;
  requisition->height += GTK_CONTAINER (paned)->border_width * 2;

  if (gtk_paned_get_props (paned)->child1 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child1) &&
      gtk_paned_get_props (paned)->child2 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child2))
    {
      gint handle_size;

      __gtk_widget_style_get (widget, "handle-size", &handle_size, NULL);

      if (gtk_paned_get_props (paned)->priv->orientation == GTK_ORIENTATION_HORIZONTAL)
        requisition->width += handle_size;
      else
        requisition->height += handle_size;
    }
}

static void
flip_child (GtkWidget     *widget,
            GtkAllocation *child_pos)
{
  gint x     = gtk_widget_get_props (widget)->allocation.x;
  gint width = gtk_widget_get_props (widget)->allocation.width;

  child_pos->x = 2 * x + width - child_pos->x - child_pos->width;
}

static void
gtk_paned_size_allocate (GtkWidget     *widget,
                         GtkAllocation *allocation)
{
  GtkPaned *paned = GTK_PANED (widget);
  gint border_width = GTK_CONTAINER (paned)->border_width;

  gtk_widget_get_props (widget)->allocation = *allocation;

  if (gtk_paned_get_props (paned)->child1 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child1) &&
      gtk_paned_get_props (paned)->child2 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child2))
    {
      GtkRequisition child1_requisition;
      GtkRequisition child2_requisition;
      GtkAllocation child1_allocation;
      GtkAllocation child2_allocation;
      GdkRectangle old_handle_pos;
      gint handle_size;

      __gtk_widget_style_get (widget, "handle-size", &handle_size, NULL);

      __gtk_widget_get_child_requisition (gtk_paned_get_props (paned)->child1, &child1_requisition);
      __gtk_widget_get_child_requisition (gtk_paned_get_props (paned)->child2, &child2_requisition);

      old_handle_pos = gtk_paned_get_props (paned)->handle_pos;

      if (gtk_paned_get_props (paned)->priv->orientation == GTK_ORIENTATION_HORIZONTAL)
        {
          gtk_paned_calc_position (paned,
                                   MAX (1, gtk_widget_get_props (widget)->allocation.width
                                        - handle_size
                                        - 2 * border_width),
                                   child1_requisition.width,
                                   child2_requisition.width);

          gtk_paned_get_props (gtk_paned_get_props (paned))->handle_pos.x = gtk_widget_get_props (widget)->allocation.x + gtk_paned_get_props (gtk_paned_get_props (paned))->child1_size + border_width;
          gtk_paned_get_props (paned)->handle_pos.y = gtk_widget_get_props (widget)->allocation.y + border_width;
          gtk_paned_get_props (paned)->handle_pos.width = handle_size;
          gtk_paned_get_props (paned)->handle_pos.height = MAX (1, gtk_widget_get_props (widget)->allocation.height - 2 * border_width);

          child1_allocation.height = child2_allocation.height = MAX (1, (gint) allocation->height - border_width * 2);
          child1_allocation.width = MAX (1, gtk_paned_get_props (paned)->child1_size);
          child1_allocation.x = gtk_widget_get_props (widget)->allocation.x + border_width;
          child1_allocation.y = child2_allocation.y = gtk_widget_get_props (widget)->allocation.y + border_width;

          child2_allocation.x = child1_allocation.x + gtk_paned_get_props (paned)->child1_size + gtk_paned_get_props (paned)->handle_pos.width;
          child2_allocation.width = MAX (1, gtk_widget_get_props (widget)->allocation.x + gtk_widget_get_props (widget)->allocation.width - child2_allocation.x - border_width);

          if (__gtk_widget_get_direction (GTK_WIDGET (widget)) == GTK_TEXT_DIR_RTL)
            {
              flip_child (widget, &(child2_allocation));
              flip_child (widget, &(child1_allocation));
              flip_child (widget, &(gtk_paned_get_props (paned)->handle_pos));
            }
        }
      else
        {
          gtk_paned_calc_position (paned,
                                   MAX (1, gtk_widget_get_props (widget)->allocation.height
                                        - handle_size
                                        - 2 * border_width),
                                   child1_requisition.height,
                                   child2_requisition.height);

          gtk_paned_get_props (paned)->handle_pos.x = gtk_widget_get_props (widget)->allocation.x + border_width;
          gtk_paned_get_props (gtk_paned_get_props (paned))->handle_pos.y = gtk_widget_get_props (widget)->allocation.y + gtk_paned_get_props (gtk_paned_get_props (paned))->child1_size + border_width;
          gtk_paned_get_props (paned)->handle_pos.width = MAX (1, (gint) gtk_widget_get_props (widget)->allocation.width - 2 * border_width);
          gtk_paned_get_props (paned)->handle_pos.height = handle_size;

          child1_allocation.width = child2_allocation.width = MAX (1, (gint) allocation->width - border_width * 2);
          child1_allocation.height = MAX (1, gtk_paned_get_props (paned)->child1_size);
          child1_allocation.x = child2_allocation.x = gtk_widget_get_props (widget)->allocation.x + border_width;
          child1_allocation.y = gtk_widget_get_props (widget)->allocation.y + border_width;

          child2_allocation.y = child1_allocation.y + gtk_paned_get_props (paned)->child1_size + gtk_paned_get_props (paned)->handle_pos.height;
          child2_allocation.height = MAX (1, gtk_widget_get_props (widget)->allocation.y + gtk_widget_get_props (widget)->allocation.height - child2_allocation.y - border_width);
        }

      if (__gtk_widget_get_mapped (widget) &&
          (old_handle_pos.x != gtk_paned_get_props (paned)->handle_pos.x ||
           old_handle_pos.y != gtk_paned_get_props (paned)->handle_pos.y ||
           old_handle_pos.width != gtk_paned_get_props (paned)->handle_pos.width ||
           old_handle_pos.height != gtk_paned_get_props (paned)->handle_pos.height))
        {
          __gdk_window_invalidate_rect (gtk_widget_get_props (widget)->window, &old_handle_pos, FALSE);
          __gdk_window_invalidate_rect (gtk_widget_get_props (widget)->window, &gtk_paned_get_props (paned)->handle_pos, FALSE);
        }

      if (__gtk_widget_get_realized (widget))
	{
	  if (__gtk_widget_get_mapped (widget))
	    __gdk_window_show (gtk_paned_get_props (paned)->handle);

          if (gtk_paned_get_props (paned)->priv->orientation == GTK_ORIENTATION_HORIZONTAL)
            {
              __gdk_window_move_resize (gtk_paned_get_props (paned)->handle,
                                      gtk_paned_get_props (paned)->handle_pos.x,
                                      gtk_paned_get_props (paned)->handle_pos.y,
                                      handle_size,
                                      gtk_paned_get_props (paned)->handle_pos.height);
            }
          else
            {
              __gdk_window_move_resize (gtk_paned_get_props (paned)->handle,
                                      gtk_paned_get_props (paned)->handle_pos.x,
                                      gtk_paned_get_props (paned)->handle_pos.y,
                                      gtk_paned_get_props (paned)->handle_pos.width,
                                      handle_size);
            }
	}

      /* Now allocate the childen, making sure, when resizing not to
       * overlap the windows
       */
      if (__gtk_widget_get_mapped (widget) &&

          ((gtk_paned_get_props (paned)->priv->orientation == GTK_ORIENTATION_HORIZONTAL &&
            gtk_paned_get_props (paned)->child1->allocation.width < child1_allocation.width) ||

           (gtk_paned_get_props (paned)->priv->orientation == GTK_ORIENTATION_VERTICAL &&
            gtk_paned_get_props (paned)->child1->allocation.height < child1_allocation.height)))
	{
	  __gtk_widget_size_allocate (gtk_paned_get_props (paned)->child2, &child2_allocation);
	  __gtk_widget_size_allocate (gtk_paned_get_props (paned)->child1, &child1_allocation);
	}
      else
	{
	  __gtk_widget_size_allocate (gtk_paned_get_props (paned)->child1, &child1_allocation);
	  __gtk_widget_size_allocate (gtk_paned_get_props (paned)->child2, &child2_allocation);
	}
    }
  else
    {
      GtkAllocation child_allocation;

      if (__gtk_widget_get_realized (widget))
	__gdk_window_hide (gtk_paned_get_props (paned)->handle);

      if (gtk_paned_get_props (paned)->child1)
	__gtk_widget_set_child_visible (gtk_paned_get_props (paned)->child1, TRUE);
      if (gtk_paned_get_props (paned)->child2)
	__gtk_widget_set_child_visible (gtk_paned_get_props (paned)->child2, TRUE);

      child_allocation.x = gtk_widget_get_props (widget)->allocation.x + border_width;
      child_allocation.y = gtk_widget_get_props (widget)->allocation.y + border_width;
      child_allocation.width = MAX (1, allocation->width - 2 * border_width);
      child_allocation.height = MAX (1, allocation->height - 2 * border_width);

      if (gtk_paned_get_props (paned)->child1 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child1))
	__gtk_widget_size_allocate (gtk_paned_get_props (paned)->child1, &child_allocation);
      else if (gtk_paned_get_props (paned)->child2 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child2))
	__gtk_widget_size_allocate (gtk_paned_get_props (paned)->child2, &child_allocation);
    }
}

static void
gtk_paned_realize (GtkWidget *widget)
{
  GtkPaned *paned;
  GdkWindowAttr attributes;
  gint attributes_mask;

  __gtk_widget_set_realized (widget, TRUE);
  paned = GTK_PANED (widget);

  gtk_widget_get_props (widget)->window = __gtk_widget_get_parent_window (gtk_widget_get_props (widget));
  g_object_ref (gtk_widget_get_props (widget)->window);
  
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.wclass = GDK_INPUT_ONLY;
  attributes.x = gtk_paned_get_props (paned)->handle_pos.x;
  attributes.y = gtk_paned_get_props (paned)->handle_pos.y;
  attributes.width = gtk_paned_get_props (paned)->handle_pos.width;
  attributes.height = gtk_paned_get_props (paned)->handle_pos.height;
  attributes.event_mask = __gtk_widget_get_events (widget);
  attributes.event_mask |= (GDK_BUTTON_PRESS_MASK |
			    GDK_BUTTON_RELEASE_MASK |
			    GDK_ENTER_NOTIFY_MASK |
			    GDK_LEAVE_NOTIFY_MASK |
			    GDK_POINTER_MOTION_MASK |
			    GDK_POINTER_MOTION_HINT_MASK);
  attributes_mask = GDK_WA_X | GDK_WA_Y;
  if (__gtk_widget_is_sensitive (widget))
    {
      attributes.cursor = __gdk_cursor_new_for_display (__gtk_widget_get_display (widget),
						      gtk_paned_get_props (paned)->cursor_type);
      attributes_mask |= GDK_WA_CURSOR;
    }

  gtk_paned_get_props (paned)->handle = __gdk_window_new (gtk_widget_get_props (widget)->window,
				  &attributes, attributes_mask);
  __gdk_window_set_user_data (gtk_paned_get_props (paned)->handle, gtk_paned_get_props (paned));
  if (attributes_mask & GDK_WA_CURSOR)
    __gdk_cursor_unref (attributes.cursor);

  gtk_widget_get_props (widget)->style = __gtk_style_attach (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window);

  if (gtk_paned_get_props (paned)->child1 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child1) &&
      gtk_paned_get_props (paned)->child2 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child2))
    __gdk_window_show (gtk_paned_get_props (paned)->handle);
}

static void
gtk_paned_unrealize (GtkWidget *widget)
{
  GtkPaned *paned = GTK_PANED (widget);

  if (gtk_paned_get_props (paned)->xor_gc)
    {
      g_object_unref (gtk_paned_get_props (paned)->xor_gc);
      gtk_paned_get_props (paned)->xor_gc = NULL;
    }

  if (gtk_paned_get_props (paned)->handle)
    {
      __gdk_window_set_user_data (gtk_paned_get_props (paned)->handle, NULL);
      __gdk_window_destroy (gtk_paned_get_props (paned)->handle);
      gtk_paned_get_props (paned)->handle = NULL;
    }

  gtk_paned_set_last_child1_focus (paned, NULL);
  gtk_paned_set_last_child2_focus (paned, NULL);
  gtk_paned_set_saved_focus (paned, NULL);
  gtk_paned_set_first_paned (paned, NULL);

  GTK_WIDGET_CLASS (gtk_paned_parent_class)->unrealize (widget);
}

static void
gtk_paned_map (GtkWidget *widget)
{
  GtkPaned *paned = GTK_PANED (widget);

  __gdk_window_show (gtk_paned_get_props (paned)->handle);

  GTK_WIDGET_CLASS (gtk_paned_parent_class)->map (widget);
}

static void
gtk_paned_unmap (GtkWidget *widget)
{
  GtkPaned *paned = GTK_PANED (widget);
    
  __gdk_window_hide (gtk_paned_get_props (paned)->handle);

  GTK_WIDGET_CLASS (gtk_paned_parent_class)->unmap (widget);
}

static gboolean
gtk_paned_expose (GtkWidget      *widget,
		  GdkEventExpose *event)
{
  GtkPaned *paned = GTK_PANED (widget);

  if (__gtk_widget_get_visible (widget) && __gtk_widget_get_mapped (widget) &&
      gtk_paned_get_props (paned)->child1 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child1) &&
      gtk_paned_get_props (paned)->child2 && __gtk_widget_get_visible (gtk_paned_get_props (paned)->child2))
    {
      GtkStateType state;
      
      if (__gtk_widget_is_focus (widget))
	state = GTK_STATE_SELECTED;
      else if (gtk_paned_get_props (paned)->handle_prelit)
	state = GTK_STATE_PRELIGHT;
      else
	state = __gtk_widget_get_state (widget);
      
      __gtk_paint_handle (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window,
			state, GTK_SHADOW_NONE,
			&gtk_paned_get_props (paned)->handle_pos, widget, "gtk_paned_get_props (paned)",
			gtk_paned_get_props (paned)->handle_pos.x, gtk_paned_get_props (paned)->handle_pos.y,
			gtk_paned_get_props (paned)->handle_pos.width, gtk_paned_get_props (paned)->handle_pos.height,
			!gtk_paned_get_props (paned)->priv->orientation);
    }

  /* Chain up to draw children */
  GTK_WIDGET_CLASS (gtk_paned_parent_class)->expose_event (widget, event);
  
  return FALSE;
}

static gboolean
is_rtl (GtkPaned *paned)
{
  if (gtk_paned_get_props (paned)->priv->orientation == GTK_ORIENTATION_HORIZONTAL &&
      __gtk_widget_get_direction (GTK_WIDGET (paned)) == GTK_TEXT_DIR_RTL)
    {
      return TRUE;
    }

  return FALSE;
}

static void
update_drag (GtkPaned *paned)
{
  gint pos;
  gint handle_size;
  gint size;

  if (gtk_paned_get_props (paned)->priv->orientation == GTK_ORIENTATION_HORIZONTAL)
    __gtk_widget_get_pointer (GTK_WIDGET (paned), &pos, NULL);
  else
    __gtk_widget_get_pointer (GTK_WIDGET (paned), NULL, &pos);

  pos -= gtk_paned_get_props (paned)->drag_pos;

  if (is_rtl (paned))
    {
      __gtk_widget_style_get (GTK_WIDGET (paned),
			    "handle-size", &handle_size,
			    NULL);
      
      size = GTK_WIDGET (paned)->allocation.width - pos - handle_size;
    }
  else
    {
      size = pos;
    }

  size -= GTK_CONTAINER (paned)->border_width;
  
  size = CLAMP (size, gtk_paned_get_props (paned)->min_position, gtk_paned_get_props (paned)->max_position);

  if (size != gtk_paned_get_props (paned)->child1_size)
    __gtk_paned_set_position (paned, size);
}

static gboolean
gtk_paned_enter (GtkWidget        *widget,
		 GdkEventCrossing *event)
{
  GtkPaned *paned = GTK_PANED (widget);
  
  if (gtk_paned_get_props (paned)->in_drag)
    update_drag (paned);
  else
    {
      gtk_paned_get_props (paned)->handle_prelit = TRUE;
      __gtk_widget_queue_draw_area (widget,
				  gtk_paned_get_props (paned)->handle_pos.x,
				  gtk_paned_get_props (paned)->handle_pos.y,
				  gtk_paned_get_props (paned)->handle_pos.width,
				  gtk_paned_get_props (paned)->handle_pos.height);
    }
  
  return TRUE;
}

static gboolean
gtk_paned_leave (GtkWidget        *widget,
		 GdkEventCrossing *event)
{
  GtkPaned *paned = GTK_PANED (widget);
  
  if (gtk_paned_get_props (paned)->in_drag)
    update_drag (paned);
  else
    {
      gtk_paned_get_props (paned)->handle_prelit = FALSE;
      __gtk_widget_queue_draw_area (widget,
				  gtk_paned_get_props (paned)->handle_pos.x,
				  gtk_paned_get_props (paned)->handle_pos.y,
				  gtk_paned_get_props (paned)->handle_pos.width,
				  gtk_paned_get_props (paned)->handle_pos.height);
    }

  return TRUE;
}

static gboolean
gtk_paned_focus (GtkWidget        *widget,
		 GtkDirectionType  direction)

{
  gboolean retval;
  
  /* This is a hack, but how can this be done without
   * excessive cut-and-paste from gtkcontainer.c?
   */

  __gtk_widget_set_can_focus (widget, FALSE);
  retval = GTK_WIDGET_CLASS (gtk_paned_parent_class)->focus (widget, direction);
  __gtk_widget_set_can_focus (widget, TRUE);

  return retval;
}

static gboolean
gtk_paned_button_press (GtkWidget      *widget,
			GdkEventButton *event)
{
  GtkPaned *paned = GTK_PANED (widget);

  if (!gtk_paned_get_props (paned)->in_drag &&
      (event->window == gtk_paned_get_props (paned)->handle) && (event->button == 1))
    {
      /* We need a server grab here, not __gtk_grab_add(), since
       * we don't want to pass events on to the widget's children */
      if (__gdk_pointer_grab (gtk_paned_get_props (paned)->handle, FALSE,
			    GDK_POINTER_MOTION_HINT_MASK
			    | GDK_BUTTON1_MOTION_MASK
			    | GDK_BUTTON_RELEASE_MASK
			    | GDK_ENTER_NOTIFY_MASK
			    | GDK_LEAVE_NOTIFY_MASK,
			    NULL, NULL,
			    event->time) != GDK_GRAB_SUCCESS)
	return FALSE;

      gtk_paned_get_props (paned)->in_drag = TRUE;
      gtk_paned_get_props (paned)->priv->grab_time = event->time;

      if (gtk_paned_get_props (paned)->priv->orientation == GTK_ORIENTATION_HORIZONTAL)
	gtk_paned_get_props (paned)->drag_pos = event->x;
      else
	gtk_paned_get_props (paned)->drag_pos = event->y;

      return TRUE;
    }

  return FALSE;
}

static gboolean
gtk_paned_grab_broken (GtkWidget          *widget,
		       GdkEventGrabBroken *event)
{
  GtkPaned *paned = GTK_PANED (widget);

  gtk_paned_get_props (paned)->in_drag = FALSE;
  gtk_paned_get_props (paned)->drag_pos = -1;
  gtk_paned_get_props (paned)->position_set = TRUE;

  return TRUE;
}

static void
stop_drag (GtkPaned *paned)
{
  gtk_paned_get_props (paned)->in_drag = FALSE;
  gtk_paned_get_props (paned)->drag_pos = -1;
  gtk_paned_get_props (paned)->position_set = TRUE;
  __gdk_display_pointer_ungrab (__gtk_widget_get_display (GTK_WIDGET (paned)),
			      gtk_paned_get_props (paned)->priv->grab_time);
}

static void
gtk_paned_grab_notify (GtkWidget *widget,
		       gboolean   was_grabbed)
{
  GtkPaned *paned = GTK_PANED (widget);

  if (!was_grabbed && gtk_paned_get_props (paned)->in_drag)
    stop_drag (paned);
}

static void
gtk_paned_state_changed (GtkWidget    *widget,
                         GtkStateType  previous_state)
{
  GtkPaned *paned = GTK_PANED (widget);
  GdkCursor *cursor;

  if (__gtk_widget_get_realized (widget))
    {
      if (__gtk_widget_is_sensitive (widget))
        cursor = __gdk_cursor_new_for_display (__gtk_widget_get_display (widget),
                                             gtk_paned_get_props (paned)->cursor_type); 
      else
        cursor = NULL;

      __gdk_window_set_cursor (gtk_paned_get_props (paned)->handle, cursor);

      if (cursor)
        __gdk_cursor_unref (cursor);
    }
}

static gboolean
gtk_paned_button_release (GtkWidget      *widget,
			  GdkEventButton *event)
{
  GtkPaned *paned = GTK_PANED (widget);

  if (gtk_paned_get_props (paned)->in_drag && (event->button == 1))
    {
      stop_drag (paned);

      return TRUE;
    }

  return FALSE;
}

static gboolean
gtk_paned_motion (GtkWidget      *widget,
		  GdkEventMotion *event)
{
  GtkPaned *paned = GTK_PANED (widget);
  
  if (gtk_paned_get_props (paned)->in_drag)
    {
      update_drag (paned);
      return TRUE;
    }
  
  return FALSE;
}

/**
 * __gtk_paned_new:
 * @orientation: the paned's orientation.
 *
 * Creates a new #GtkPaned widget.
 *
 * Return value: a new #GtkPaned.
 **/
GtkWidget *
__gtk_paned_new (GtkOrientation orientation)
{
  return g_object_new (GTK_TYPE_PANED,
                       "orientation", orientation,
                       NULL);
}

void
__gtk_paned_add1 (GtkPaned  *paned,
		GtkWidget *widget)
{
  __gtk_paned_pack1 (paned, widget, FALSE, TRUE);
}

void
__gtk_paned_add2 (GtkPaned  *paned,
		GtkWidget *widget)
{
  __gtk_paned_pack2 (paned, widget, TRUE, TRUE);
}

void
__gtk_paned_pack1 (GtkPaned  *paned,
		 GtkWidget *child,
		 gboolean   resize,
		 gboolean   shrink)
{
  g_return_if_fail (GTK_IS_PANED (paned));
  g_return_if_fail (GTK_IS_WIDGET (child));

  if (!gtk_paned_get_props (paned)->child1)
    {
      gtk_paned_get_props (paned)->child1 = child;
      gtk_paned_get_props (paned)->child1_resize = resize;
      gtk_paned_get_props (paned)->child1_shrink = shrink;

      __gtk_widget_set_parent (child, GTK_WIDGET (paned));
    }
}

void
__gtk_paned_pack2 (GtkPaned  *paned,
		 GtkWidget *child,
		 gboolean   resize,
		 gboolean   shrink)
{
  g_return_if_fail (GTK_IS_PANED (paned));
  g_return_if_fail (GTK_IS_WIDGET (child));

  if (!gtk_paned_get_props (paned)->child2)
    {
      gtk_paned_get_props (paned)->child2 = child;
      gtk_paned_get_props (paned)->child2_resize = resize;
      gtk_paned_get_props (paned)->child2_shrink = shrink;

      __gtk_widget_set_parent (child, GTK_WIDGET (paned));
    }
}


static void
gtk_paned_add (GtkContainer *container,
	       GtkWidget    *widget)
{
  GtkPaned *paned;

  g_return_if_fail (GTK_IS_PANED (container));

  paned = GTK_PANED (container);

  if (!gtk_paned_get_props (paned)->child1)
    __gtk_paned_add1 (paned, widget);
  else if (!gtk_paned_get_props (paned)->child2)
    __gtk_paned_add2 (paned, widget);
  else
    g_warning ("GtkPaned cannot have more than 2 children\n");
}

static void
gtk_paned_remove (GtkContainer *container,
		  GtkWidget    *widget)
{
  GtkPaned *paned;
  gboolean was_visible;

  paned = GTK_PANED (container);
  was_visible = __gtk_widget_get_visible (widget);

  if (gtk_paned_get_props (paned)->child1 == widget)
    {
      __gtk_widget_unparent (widget);

      gtk_paned_get_props (paned)->child1 = NULL;

      if (was_visible && __gtk_widget_get_visible (GTK_WIDGET (container)))
	__gtk_widget_queue_resize_no_redraw (GTK_WIDGET (container));
    }
  else if (gtk_paned_get_props (paned)->child2 == widget)
    {
      __gtk_widget_unparent (widget);

      gtk_paned_get_props (paned)->child2 = NULL;

      if (was_visible && __gtk_widget_get_visible (GTK_WIDGET (container)))
	__gtk_widget_queue_resize_no_redraw (GTK_WIDGET (container));
    }
}

static void
gtk_paned_forall (GtkContainer *container,
		  gboolean      include_internals,
		  GtkCallback   callback,
		  gpointer      callback_data)
{
  GtkPaned *paned;

  g_return_if_fail (callback != NULL);

  paned = GTK_PANED (container);

  if (gtk_paned_get_props (paned)->child1)
    (*callback) (gtk_paned_get_props (paned)->child1, callback_data);
  if (gtk_paned_get_props (paned)->child2)
    (*callback) (gtk_paned_get_props (paned)->child2, callback_data);
}

/**
 * __gtk_paned_get_position:
 * @paned: a #GtkPaned widget
 * 
 * Obtains the position of the divider between the two panes.
 * 
 * Return value: position of the divider
 **/
gint
__gtk_paned_get_position (GtkPaned  *paned)
{
  g_return_val_if_fail (GTK_IS_PANED (paned), 0);

  return gtk_paned_get_props (paned)->child1_size;
}

/**
 * __gtk_paned_set_position:
 * @paned: a #GtkPaned widget
 * @position: pixel position of divider, a negative value means that the position
 *            is unset.
 * 
 * Sets the position of the divider between the two panes.
 **/
void
__gtk_paned_set_position (GtkPaned *paned,
			gint      position)
{
  GObject *object;
  
  g_return_if_fail (GTK_IS_PANED (paned));

  if (gtk_paned_get_props (paned)->child1_size == position)
    return;

  object = G_OBJECT (paned);
  
  if (position >= 0)
    {
      /* We don't clamp here - the assumption is that
       * if the total allocation changes at the same time
       * as the position, the position set is with reference
       * to the new total size. If only the position changes,
       * then clamping will occur in gtk_paned_calc_position()
       */

      gtk_paned_get_props (paned)->child1_size = position;
      gtk_paned_get_props (paned)->position_set = TRUE;
    }
  else
    {
      gtk_paned_get_props (paned)->position_set = FALSE;
    }

  g_object_freeze_notify (object);
  g_object_notify (object, "position");
  g_object_notify (object, "position-set");
  g_object_thaw_notify (object);

  __gtk_widget_queue_resize_no_redraw (GTK_WIDGET (paned));

#ifdef G_OS_WIN32
  /* Hacky work-around for bug #144269 */
  if (gtk_paned_get_props (paned)->child2 != NULL)
    {
      __gtk_widget_queue_draw (gtk_paned_get_props (paned)->child2);
    }
#endif
}

/**
 * __gtk_paned_get_child1:
 * @paned: a #GtkPaned widget
 * 
 * Obtains the first child of the paned widget.
 * 
 * Return value: (transfer none): first child, or %NULL if it is not set.
 *
 * Since: 2.4
 **/
GtkWidget *
__gtk_paned_get_child1 (GtkPaned *paned)
{
  g_return_val_if_fail (GTK_IS_PANED (paned), NULL);

  return gtk_paned_get_props (paned)->child1;
}

/**
 * __gtk_paned_get_child2:
 * @paned: a #GtkPaned widget
 * 
 * Obtains the second child of the paned widget.
 * 
 * Return value: (transfer none): second child, or %NULL if it is not set.
 *
 * Since: 2.4
 **/
GtkWidget *
__gtk_paned_get_child2 (GtkPaned *paned)
{
  g_return_val_if_fail (GTK_IS_PANED (paned), NULL);

  return gtk_paned_get_props (paned)->child2;
}

void
__gtk_paned_compute_position (GtkPaned *paned,
			    gint      allocation,
			    gint      child1_req,
			    gint      child2_req)
{
  g_return_if_fail (GTK_IS_PANED (paned));

  gtk_paned_calc_position (paned, allocation, child1_req, child2_req);
}

static void
gtk_paned_calc_position (GtkPaned *paned,
                         gint      allocation,
                         gint      child1_req,
                         gint      child2_req)
{
  gint old_position;
  gint old_min_position;
  gint old_max_position;

  old_position = gtk_paned_get_props (paned)->child1_size;
  old_min_position = gtk_paned_get_props (paned)->min_position;
  old_max_position = gtk_paned_get_props (paned)->max_position;

  gtk_paned_get_props (paned)->min_position = gtk_paned_get_props (paned)->child1_shrink ? 0 : child1_req;

  gtk_paned_get_props (paned)->max_position = allocation;
  if (!gtk_paned_get_props (paned)->child2_shrink)
    gtk_paned_get_props (paned)->max_position = MAX (1, gtk_paned_get_props (paned)->max_position - child2_req);
  gtk_paned_get_props (paned)->max_position = MAX (gtk_paned_get_props (paned)->min_position, gtk_paned_get_props (paned)->max_position);

  if (!gtk_paned_get_props (paned)->position_set)
    {
      if (gtk_paned_get_props (paned)->child1_resize && !gtk_paned_get_props (paned)->child2_resize)
	gtk_paned_get_props (paned)->child1_size = MAX (0, allocation - child2_req);
      else if (!gtk_paned_get_props (paned)->child1_resize && gtk_paned_get_props (paned)->child2_resize)
	gtk_paned_get_props (paned)->child1_size = child1_req;
      else if (child1_req + child2_req != 0)
	gtk_paned_get_props (paned)->child1_size = allocation * ((gdouble)child1_req / (child1_req + child2_req)) + 0.5;
      else
	gtk_paned_get_props (paned)->child1_size = allocation * 0.5 + 0.5;
    }
  else
    {
      /* If the position was set before the initial allocation.
       * (gtk_paned_get_props (paned)->last_allocation <= 0) just clamp it and leave it.
       */
      if (gtk_paned_get_props (paned)->last_allocation > 0)
	{
	  if (gtk_paned_get_props (paned)->child1_resize && !gtk_paned_get_props (paned)->child2_resize)
	    gtk_paned_get_props (paned)->child1_size += allocation - gtk_paned_get_props (paned)->last_allocation;
	  else if (!(!gtk_paned_get_props (paned)->child1_resize && gtk_paned_get_props (paned)->child2_resize))
	    gtk_paned_get_props (paned)->child1_size = allocation * ((gdouble) gtk_paned_get_props (paned)->child1_size / (gtk_paned_get_props (paned)->last_allocation)) + 0.5;
	}
    }

  gtk_paned_get_props (paned)->child1_size = CLAMP (gtk_paned_get_props (paned)->child1_size,
			      gtk_paned_get_props (paned)->min_position,
			      gtk_paned_get_props (paned)->max_position);

  if (gtk_paned_get_props (paned)->child1)
    __gtk_widget_set_child_visible (gtk_paned_get_props (paned)->child1, gtk_paned_get_props (paned)->child1_size != 0);
  
  if (gtk_paned_get_props (paned)->child2)
    __gtk_widget_set_child_visible (gtk_paned_get_props (paned)->child2, gtk_paned_get_props (paned)->child1_size != allocation); 

  g_object_freeze_notify (G_OBJECT (paned));
  if (gtk_paned_get_props (paned)->child1_size != old_position)
    g_object_notify (G_OBJECT (paned), "position");
  if (gtk_paned_get_props (paned)->min_position != old_min_position)
    g_object_notify (G_OBJECT (paned), "min-position");
  if (gtk_paned_get_props (paned)->max_position != old_max_position)
    g_object_notify (G_OBJECT (paned), "max-position");
  g_object_thaw_notify (G_OBJECT (paned));

  gtk_paned_get_props (paned)->last_allocation = allocation;
}

static void
gtk_paned_set_saved_focus (GtkPaned *paned, GtkWidget *widget)
{
  if (gtk_paned_get_props (paned)->priv->saved_focus)
    g_object_remove_weak_pointer (G_OBJECT (gtk_paned_get_props (paned)->priv->saved_focus),
				  (gpointer *)&(gtk_paned_get_props (paned)->priv->saved_focus));

  gtk_paned_get_props (paned)->priv->saved_focus = widget;

  if (gtk_paned_get_props (paned)->priv->saved_focus)
    g_object_add_weak_pointer (G_OBJECT (gtk_paned_get_props (paned)->priv->saved_focus),
			       (gpointer *)&(gtk_paned_get_props (paned)->priv->saved_focus));
}

static void
gtk_paned_set_first_paned (GtkPaned *paned, GtkPaned *first_paned)
{
  if (gtk_paned_get_props (paned)->priv->first_paned)
    g_object_remove_weak_pointer (G_OBJECT (gtk_paned_get_props (paned)->priv->first_paned),
				  (gpointer *)&(gtk_paned_get_props (paned)->priv->first_paned));

  gtk_paned_get_props (paned)->priv->first_paned = first_paned;

  if (gtk_paned_get_props (paned)->priv->first_paned)
    g_object_add_weak_pointer (G_OBJECT (gtk_paned_get_props (paned)->priv->first_paned),
			       (gpointer *)&(gtk_paned_get_props (paned)->priv->first_paned));
}

static void
gtk_paned_set_last_child1_focus (GtkPaned *paned, GtkWidget *widget)
{
  if (gtk_paned_get_props (paned)->last_child1_focus)
    g_object_remove_weak_pointer (G_OBJECT (gtk_paned_get_props (paned)->last_child1_focus),
				  (gpointer *)&(gtk_paned_get_props (paned)->last_child1_focus));

  gtk_paned_get_props (paned)->last_child1_focus = widget;

  if (gtk_paned_get_props (paned)->last_child1_focus)
    g_object_add_weak_pointer (G_OBJECT (gtk_paned_get_props (paned)->last_child1_focus),
			       (gpointer *)&(gtk_paned_get_props (paned)->last_child1_focus));
}

static void
gtk_paned_set_last_child2_focus (GtkPaned *paned, GtkWidget *widget)
{
  if (gtk_paned_get_props (paned)->last_child2_focus)
    g_object_remove_weak_pointer (G_OBJECT (gtk_paned_get_props (paned)->last_child2_focus),
				  (gpointer *)&(gtk_paned_get_props (paned)->last_child2_focus));

  gtk_paned_get_props (paned)->last_child2_focus = widget;

  if (gtk_paned_get_props (paned)->last_child2_focus)
    g_object_add_weak_pointer (G_OBJECT (gtk_paned_get_props (paned)->last_child2_focus),
			       (gpointer *)&(gtk_paned_get_props (paned)->last_child2_focus));
}

static GtkWidget *
paned_get_focus_widget (GtkPaned *paned)
{
  GtkWidget *toplevel;

  toplevel = __gtk_widget_get_toplevel (GTK_WIDGET (paned));
  if (__gtk_widget_is_toplevel (toplevel))
    return GTK_WINDOW (toplevel)->focus_widget;

  return NULL;
}

static void
gtk_paned_set_focus_child (GtkContainer *container,
			   GtkWidget    *focus_child)
{
  GtkPaned *paned;
  
  g_return_if_fail (GTK_IS_PANED (container));

  paned = GTK_PANED (container);
 
  if (focus_child == NULL)
    {
      GtkWidget *last_focus;
      GtkWidget *w;
      
      last_focus = paned_get_focus_widget (paned);

      if (last_focus)
	{
	  /* If there is one or more paned widgets between us and the
	   * focus widget, we want the topmost of those as last_focus
	   */
	  for (gtk_widget_get_props (w) = last_focus; gtk_widget_get_props (w) != GTK_WIDGET (paned); gtk_widget_get_props (w) = gtk_widget_get_props (w)->parent)
	    if (GTK_IS_PANED (w))
	      last_focus = w;
	  
	  if (gtk_container_get_props (container)->focus_child == gtk_paned_get_props (paned)->child1)
	    gtk_paned_set_last_child1_focus (paned, last_focus);
	  else if (gtk_container_get_props (container)->focus_child == gtk_paned_get_props (paned)->child2)
	    gtk_paned_set_last_child2_focus (paned, last_focus);
	}
    }

  if (GTK_CONTAINER_CLASS (gtk_paned_parent_class)->set_focus_child)
    GTK_CONTAINER_CLASS (gtk_paned_parent_class)->set_focus_child (container, focus_child);
}

static void
gtk_paned_get_cycle_chain (GtkPaned          *paned,
			   GtkDirectionType   direction,
			   GList            **widgets)
{
  GtkContainer *container = GTK_CONTAINER (paned);
  GtkWidget *ancestor = NULL;
  GList *temp_list = NULL;
  GList *list;

  if (gtk_paned_get_props (paned)->in_recursion)
    return;

  g_assert (widgets != NULL);

  if (gtk_paned_get_props (paned)->last_child1_focus &&
      !__gtk_widget_is_ancestor (gtk_paned_get_props (paned)->last_child1_focus, GTK_WIDGET (gtk_paned_get_props (paned))))
    {
      gtk_paned_set_last_child1_focus (paned, NULL);
    }

  if (gtk_paned_get_props (paned)->last_child2_focus &&
      !__gtk_widget_is_ancestor (gtk_paned_get_props (paned)->last_child2_focus, GTK_WIDGET (gtk_paned_get_props (paned))))
    {
      gtk_paned_set_last_child2_focus (paned, NULL);
    }

  if (GTK_WIDGET (paned)->parent)
    ancestor = __gtk_widget_get_ancestor (GTK_WIDGET (paned)->parent, GTK_TYPE_PANED);

  /* The idea here is that temp_list is a list of widgets we want to cycle
   * to. The list is prioritized so that the first element is our first
   * choice, the next our second, and so on.
   *
   * We can't just use g_list_reverse(), because we want to try
   * gtk_paned_get_props (paned)->last_child?_focus before gtk_paned_get_props (paned)->child?, both when we
   * are going forward and backward.
   */
  if (direction == GTK_DIR_TAB_FORWARD)
    {
      if (gtk_container_get_props (container)->focus_child == gtk_paned_get_props (paned)->child1)
	{
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->last_child2_focus);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->child2);
	  temp_list = g_list_append (temp_list, ancestor);
	}
      else if (gtk_container_get_props (container)->focus_child == gtk_paned_get_props (paned)->child2)
	{
	  temp_list = g_list_append (temp_list, ancestor);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->last_child1_focus);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->child1);
	}
      else
	{
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->last_child1_focus);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->child1);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->last_child2_focus);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->child2);
	  temp_list = g_list_append (temp_list, ancestor);
	}
    }
  else
    {
      if (gtk_container_get_props (container)->focus_child == gtk_paned_get_props (paned)->child1)
	{
	  temp_list = g_list_append (temp_list, ancestor);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->last_child2_focus);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->child2);
	}
      else if (gtk_container_get_props (container)->focus_child == gtk_paned_get_props (paned)->child2)
	{
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->last_child1_focus);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->child1);
	  temp_list = g_list_append (temp_list, ancestor);
	}
      else
	{
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->last_child2_focus);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->child2);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->last_child1_focus);
	  temp_list = g_list_append (temp_list, gtk_paned_get_props (paned)->child1);
	  temp_list = g_list_append (temp_list, ancestor);
	}
    }

  /* Walk the list and expand all the paned widgets. */
  for (list = temp_list; list != NULL; list = list->next)
    {
      GtkWidget *widget = list->data;

      if (widget)
	{
	  if (GTK_IS_PANED (widget))
	    {
	      gtk_paned_get_props (paned)->in_recursion = TRUE;
	      gtk_paned_get_cycle_chain (GTK_PANED (widget), direction, widgets);
	      gtk_paned_get_props (paned)->in_recursion = FALSE;
	    }
	  else
	    {
	      *widgets = g_list_append (*widgets, widget);
	    }
	}
    }

  g_list_free (temp_list);
}

static gboolean
gtk_paned_cycle_child_focus (GtkPaned *paned,
			     gboolean  reversed)
{
  GList *cycle_chain = NULL;
  GList *list;
  
  GtkDirectionType direction = reversed? GTK_DIR_TAB_BACKWARD : GTK_DIR_TAB_FORWARD;

  /* ignore f6 if the handle is focused */
  if (__gtk_widget_is_focus (GTK_WIDGET (paned)))
    return TRUE;
  
  /* we can't just let the event propagate up the hierarchy,
   * because the paned will want to cycle focus _unless_ an
   * ancestor paned handles the event
   */
  gtk_paned_get_cycle_chain (paned, direction, &cycle_chain);

  for (list = cycle_chain; list != NULL; list = list->next)
    if (__gtk_widget_child_focus (GTK_WIDGET (list->data), direction))
      break;

  g_list_free (cycle_chain);
  
  return TRUE;
}

static void
get_child_panes (GtkWidget  *widget,
		 GList     **panes)
{
  if (!widget || !__gtk_widget_get_realized (widget))
    return;

  if (GTK_IS_PANED (widget))
    {
      GtkPaned *paned = GTK_PANED (widget);
      
      get_child_panes (gtk_paned_get_props (paned)->child1, panes);
      *panes = g_list_prepend (*panes, widget);
      get_child_panes (gtk_paned_get_props (paned)->child2, panes);
    }
  else if (GTK_IS_CONTAINER (widget))
    {
      __gtk_container_forall (GTK_CONTAINER (widget),
                            (GtkCallback)get_child_panes, panes);
    }
}

static GList *
get_all_panes (GtkPaned *paned)
{
  GtkPaned *topmost = NULL;
  GList *result = NULL;
  GtkWidget *w;
  
  for (gtk_widget_get_props (w) = GTK_WIDGET (paned); gtk_widget_get_props (w) != NULL; gtk_widget_get_props (w) = gtk_widget_get_props (w)->parent)
    {
      if (GTK_IS_PANED (w))
	topmost = GTK_PANED (w);
    }

  g_assert (topmost);

  get_child_panes (GTK_WIDGET (topmost), &result);

  return g_list_reverse (result);
}

static void
gtk_paned_find_neighbours (GtkPaned  *paned,
			   GtkPaned **next,
			   GtkPaned **prev)
{
  GList *all_panes;
  GList *this_link;

  all_panes = get_all_panes (paned);
  g_assert (all_panes);

  this_link = g_list_find (all_panes, paned);

  g_assert (this_link);
  
  if (this_link->next)
    *next = this_link->next->data;
  else
    *next = all_panes->data;

  if (this_link->prev)
    *prev = this_link->prev->data;
  else
    *prev = g_list_last (all_panes)->data;

  g_list_free (all_panes);
}

static gboolean
gtk_paned_move_handle (GtkPaned      *paned,
		       GtkScrollType  scroll)
{
  if (__gtk_widget_is_focus (GTK_WIDGET (paned)))
    {
      gint old_position;
      gint new_position;
      gint increment;
      
      enum {
	SINGLE_STEP_SIZE = 1,
	PAGE_STEP_SIZE   = 75
      };
      
      new_position = old_position = __gtk_paned_get_position (paned);
      increment = 0;
      
      switch (scroll)
	{
	case GTK_SCROLL_STEP_LEFT:
	case GTK_SCROLL_STEP_UP:
	case GTK_SCROLL_STEP_BACKWARD:
	  increment = - SINGLE_STEP_SIZE;
	  break;
	  
	case GTK_SCROLL_STEP_RIGHT:
	case GTK_SCROLL_STEP_DOWN:
	case GTK_SCROLL_STEP_FORWARD:
	  increment = SINGLE_STEP_SIZE;
	  break;
	  
	case GTK_SCROLL_PAGE_LEFT:
	case GTK_SCROLL_PAGE_UP:
	case GTK_SCROLL_PAGE_BACKWARD:
	  increment = - PAGE_STEP_SIZE;
	  break;
	  
	case GTK_SCROLL_PAGE_RIGHT:
	case GTK_SCROLL_PAGE_DOWN:
	case GTK_SCROLL_PAGE_FORWARD:
	  increment = PAGE_STEP_SIZE;
	  break;
	  
	case GTK_SCROLL_START:
	  new_position = gtk_paned_get_props (paned)->min_position;
	  break;
	  
	case GTK_SCROLL_END:
	  new_position = gtk_paned_get_props (paned)->max_position;
	  break;

	default:
	  break;
	}

      if (increment)
	{
	  if (is_rtl (paned))
	    increment = -increment;
	  
	  new_position = old_position + increment;
	}
      
      new_position = CLAMP (new_position, gtk_paned_get_props (paned)->min_position, gtk_paned_get_props (paned)->max_position);
      
      if (old_position != new_position)
	__gtk_paned_set_position (paned, new_position);

      return TRUE;
    }

  return FALSE;
}

static void
gtk_paned_restore_focus (GtkPaned *paned)
{
  if (__gtk_widget_is_focus (GTK_WIDGET (paned)))
    {
      if (gtk_paned_get_props (paned)->priv->saved_focus &&
	  __gtk_widget_get_sensitive (gtk_paned_get_props (paned)->priv->saved_focus))
	{
	  __gtk_widget_grab_focus (gtk_paned_get_props (paned)->priv->saved_focus);
	}
      else
	{
	  /* the saved focus is somehow not available for focusing,
	   * try
	   *   1) tabbing into the paned widget
	   * if that didn't work,
	   *   2) unset focus for the window if there is one
	   */
	  
	  if (!__gtk_widget_child_focus (GTK_WIDGET (paned), GTK_DIR_TAB_FORWARD))
	    {
	      GtkWidget *toplevel = __gtk_widget_get_toplevel (GTK_WIDGET (paned));
	      
	      if (GTK_IS_WINDOW (toplevel))
		__gtk_window_set_focus (GTK_WINDOW (toplevel), NULL);
	    }
	}
      
      gtk_paned_set_saved_focus (paned, NULL);
      gtk_paned_set_first_paned (paned, NULL);
    }
}

static gboolean
gtk_paned_accept_position (GtkPaned *paned)
{
  if (__gtk_widget_is_focus (GTK_WIDGET (paned)))
    {
      gtk_paned_get_props (paned)->original_position = -1;
      gtk_paned_restore_focus (paned);

      return TRUE;
    }

  return FALSE;
}


static gboolean
gtk_paned_cancel_position (GtkPaned *paned)
{
  if (__gtk_widget_is_focus (GTK_WIDGET (paned)))
    {
      if (gtk_paned_get_props (paned)->original_position != -1)
	{
	  __gtk_paned_set_position (gtk_paned_get_props (paned), gtk_paned_get_props (paned)->original_position);
	  gtk_paned_get_props (paned)->original_position = -1;
	}

      gtk_paned_restore_focus (paned);
      return TRUE;
    }

  return FALSE;
}

static gboolean
gtk_paned_cycle_handle_focus (GtkPaned *paned,
			      gboolean  reversed)
{
  GtkPaned *next, *prev;
  
  if (__gtk_widget_is_focus (GTK_WIDGET (paned)))
    {
      GtkPaned *focus = NULL;

      if (!gtk_paned_get_props (paned)->priv->first_paned)
	{
	  /* The first_pane has disappeared. As an ad-hoc solution,
	   * we make the currently focused paned the first_paned. To the
	   * user this will seem like the paned cycling has been reset.
	   */
	  
	  gtk_paned_set_first_paned (paned, paned);
	}
      
      gtk_paned_find_neighbours (paned, &next, &prev);

      if (reversed && prev &&
	  prev != gtk_paned_get_props (paned) && gtk_paned_get_props (paned) != gtk_paned_get_props (paned)->priv->first_paned)
	{
	  focus = prev;
	}
      else if (!reversed && next &&
	       next != gtk_paned_get_props (paned) && next != gtk_paned_get_props (paned)->priv->first_paned)
	{
	  focus = next;
	}
      else
	{
	  gtk_paned_accept_position (paned);
	  return TRUE;
	}

      g_assert (focus);
      
      gtk_paned_set_saved_focus (focus, gtk_paned_get_props (paned)->priv->saved_focus);
      gtk_paned_set_first_paned (focus, gtk_paned_get_props (paned)->priv->first_paned);
      
      gtk_paned_set_saved_focus (paned, NULL);
      gtk_paned_set_first_paned (paned, NULL);
      
      __gtk_widget_grab_focus (GTK_WIDGET (focus));
      
      if (!__gtk_widget_is_focus (GTK_WIDGET (paned)))
	{
	  gtk_paned_get_props (paned)->original_position = -1;
	  gtk_paned_get_props (focus)->original_position = __gtk_paned_get_position (gtk_paned_get_props (focus));
	}
    }
  else
    {
      GtkContainer *container = GTK_CONTAINER (paned);
      GtkPaned *focus;
      GtkPaned *first;
      GtkPaned *prev, *next;
      GtkWidget *toplevel;

      gtk_paned_find_neighbours (paned, &next, &prev);

      if (gtk_container_get_props (container)->focus_child == gtk_paned_get_props (paned)->child1)
	{
	  if (reversed)
	    {
	      focus = prev;
	      first = paned;
	    }
	  else
	    {
	      focus = paned;
	      first = paned;
	    }
	}
      else if (gtk_container_get_props (container)->focus_child == gtk_paned_get_props (paned)->child2)
	{
	  if (reversed)
	    {
	      focus = paned;
	      first = next;
	    }
	  else
	    {
	      focus = next;
	      first = next;
	    }
	}
      else
	{
	  /* Focus is not inside this paned, and we don't have focus.
	   * Presumably this happened because the application wants us
	   * to start keyboard navigating.
	   */
	  focus = paned;

	  if (reversed)
	    first = paned;
	  else
	    first = next;
	}

      toplevel = __gtk_widget_get_toplevel (GTK_WIDGET (paned));

      if (GTK_IS_WINDOW (toplevel))
	gtk_paned_set_saved_focus (focus, GTK_WINDOW (toplevel)->focus_widget);
      gtk_paned_set_first_paned (focus, first);
      gtk_paned_get_props (focus)->original_position = __gtk_paned_get_position (gtk_paned_get_props (focus)); 

      __gtk_widget_grab_focus (GTK_WIDGET (focus));
   }
  
  return TRUE;
}

static gboolean
gtk_paned_toggle_handle_focus (GtkPaned *paned)
{
  /* This function/signal has the wrong name. It is called when you
   * press Tab or Shift-Tab and what we do is act as if
   * the user pressed Return and then Tab or Shift-Tab
   */
  if (__gtk_widget_is_focus (GTK_WIDGET (paned)))
    gtk_paned_accept_position (paned);

  return FALSE;
}

/**
 * __gtk_paned_get_handle_window:
 * @panede: a #GtkPaned
 *
 * Returns the #GdkWindow of the handle. This function is
 * useful when handling button or motion events because it
 * enables the callback to distinguish between the window
 * of the paned, a child and the handle.
 *
 * Return value: (transfer none): the paned's handle window.
 *
 * Since: 2.20
 **/
GdkWindow *
__gtk_paned_get_handle_window (GtkPaned *paned)
{
  g_return_val_if_fail (GTK_IS_PANED (paned), NULL);

  return gtk_paned_get_props (paned)->handle;
}
