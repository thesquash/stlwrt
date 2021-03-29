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
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

/* By Owen Taylor <otaylor@gtk.org>              98/4/4 */



#include "config.h"

#include <stlwrt.h>

#include <gtkmain.h>

#include <gtkplug.h>
#include <gtkintl.h>
#include <gtkprivate.h>
#include <gtkplugprivate.h>



/**
 * SECTION:gtkplug
 * @Short_description: Toplevel for embedding into other processes
 * @Title: GtkPlug
 * @See_also: #GtkSocket
 *
 * Together with #GtkSocket, #GtkPlug provides the ability
 * to embed widgets from one process into another process
 * in a fashion that is transparent to the user. One
 * process creates a #GtkSocket widget and passes the
 * ID of that widget's window to the other process,
 * which then creates a #GtkPlug with that window ID.
 * Any widgets contained in the #GtkPlug then will appear
 * inside the first application's window.
 *
 * <note>
 * The #GtkPlug and #GtkSocket widgets are currently not available
 * on all platforms supported by GTK+.
 * </note>
 */

static void            gtk_plug_get_property          (GObject     *object,
						       guint        prop_id,
						       GValue      *value,
						       GParamSpec  *pspec);
static void            gtk_plug_finalize              (GObject          *object);
static void            gtk_plug_realize               (GtkWidget        *widget);
static void            gtk_plug_unrealize             (GtkWidget        *widget);
static void            gtk_plug_show                  (GtkWidget        *widget);
static void            gtk_plug_hide                  (GtkWidget        *widget);
static void            gtk_plug_map                   (GtkWidget        *widget);
static void            gtk_plug_unmap                 (GtkWidget        *widget);
static void            gtk_plug_size_allocate         (GtkWidget        *widget,
						       GtkAllocation    *allocation);
static gboolean        gtk_plug_key_press_event       (GtkWidget        *widget,
						       GdkEventKey      *event);
static gboolean        gtk_plug_focus_event           (GtkWidget        *widget,
						       GdkEventFocus    *event);
static void            gtk_plug_set_focus             (GtkWindow        *window,
						       GtkWidget        *focus);
static gboolean        gtk_plug_focus                 (GtkWidget        *widget,
						       GtkDirectionType  direction);
static void            gtk_plug_check_resize          (GtkContainer     *container);
static void            gtk_plug_keys_changed          (GtkWindow        *window);

static GtkBinClass *bin_class = NULL;

typedef struct
{
  guint			 accelerator_key;
  GdkModifierType	 accelerator_mods;
} GrabbedKey;

enum {
  PROP_0,
  PROP_EMBEDDED,
  PROP_SOCKET_WINDOW
};

enum {
  EMBEDDED,
  LAST_SIGNAL
}; 

static guint plug_signals[LAST_SIGNAL] = { 0 };

STLWRT_DEFINE_VTYPE (GtkPlug, gtk_plug, GTK_TYPE_WINDOW, G_TYPE_FLAG_NONE, ;)

static void
gtk_plug_get_property (GObject    *object,
		       guint       prop_id,
		       GValue     *value,
		       GParamSpec *pspec)
{
  GtkPlug *plug = GTK_PLUG (object);

  switch (prop_id)
    {
    case PROP_EMBEDDED:
      g_value_set_boolean (value, gtk_plug_get_props (plug)->socket_window != NULL);
      break;
    case PROP_SOCKET_WINDOW:
      g_value_set_object (value, gtk_plug_get_props (plug)->socket_window);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_plug_class_init (GtkPlugClass *class)
{
  GObjectClass *gobject_class = (GObjectClass *)class;
  GtkWidgetClass *widget_class = (GtkWidgetClass *)class;
  GtkWindowClass *window_class = (GtkWindowClass *)class;
  GtkContainerClass *container_class = (GtkContainerClass *)class;

  bin_class = g_type_class_peek (GTK_TYPE_BIN);

  gobject_class->get_property = gtk_plug_get_property;
  gobject_class->finalize = gtk_plug_finalize;
  
  widget_class->realize = gtk_plug_realize;
  widget_class->unrealize = gtk_plug_unrealize;
  widget_class->key_press_event = gtk_plug_key_press_event;
  widget_class->focus_in_event = gtk_plug_focus_event;
  widget_class->focus_out_event = gtk_plug_focus_event;

  widget_class->show = gtk_plug_show;
  widget_class->hide = gtk_plug_hide;
  widget_class->map = gtk_plug_map;
  widget_class->unmap = gtk_plug_unmap;
  widget_class->size_allocate = gtk_plug_size_allocate;

  widget_class->focus = gtk_plug_focus;

  container_class->check_resize = gtk_plug_check_resize;

  window_class->set_focus = gtk_plug_set_focus;
  window_class->keys_changed = gtk_plug_keys_changed;

  /**
   * GtkPlug:embedded:
   *
   * %TRUE if the plug is embedded in a socket.
   *
   * Since: 2.12
   */
  g_object_class_install_property (gobject_class,
				   PROP_EMBEDDED,
				   g_param_spec_boolean ("embedded",
							 P_("Embedded"),
							 P_("Whether or not the plug is embedded"),
							 FALSE,
							 GTK_PARAM_READABLE));

  /**
   * GtkPlug:socket-window:
   *
   * The window of the socket the plug is embedded in.
   *
   * Since: 2.14
   */
  g_object_class_install_property (gobject_class,
				   PROP_SOCKET_WINDOW,
				   g_param_spec_object ("socket-window",
							P_("Socket Window"),
							P_("The window of the socket the plug is embedded in"),
							GDK_TYPE_WINDOW,
							GTK_PARAM_READABLE));

  /**
   * GtkPlug::embedded:
   * @plug: the object on which the signal was emitted
   *
   * Gets emitted when the plug becomes embedded in a socket.
   */ 
  plug_signals[EMBEDDED] =
    g_signal_new (I_("embedded"),
		  G_OBJECT_CLASS_TYPE (class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GtkPlugClass, embedded),
		  NULL, NULL,
		  NULL,
		  G_TYPE_NONE, 0);
}

static void
gtk_plug_init (GtkPlug *plug)
{
  GtkWindow *window;

  window = GTK_WINDOW (plug);

  gtk_window_get_props (window)->type = GTK_WINDOW_TOPLEVEL;
}

static void
gtk_plug_set_is_child (GtkPlug  *plug,
		       gboolean  is_child)
{
  g_assert (!gtk_widget_get_props (GTK_WIDGET (plug))->parent);
      
  if (is_child)
    {
      if (gtk_plug_get_props (plug)->modality_window)
	_gtk_plug_handle_modality_off (plug);

      if (gtk_plug_get_props (plug)->modality_group)
	{
	  __gtk_window_group_remove_window (gtk_plug_get_props (plug)->modality_group, GTK_WINDOW (plug));
	  g_object_unref (gtk_plug_get_props (plug)->modality_group);
	  gtk_plug_get_props (plug)->modality_group = NULL;
	}
      
      /* As a toplevel, the MAPPED flag doesn't correspond
       * to whether the widget->window is mapped; we unmap
       * here, but don't bother remapping -- we will get mapped
       * by __gtk_widget_set_parent ().
       */
      if (__gtk_widget_get_mapped (GTK_WIDGET (plug)))
	__gtk_widget_unmap (GTK_WIDGET (plug));
      
      ___gtk_window_set_is_toplevel (GTK_WINDOW (plug), FALSE);
      __gtk_container_set_resize_mode (GTK_CONTAINER (plug), GTK_RESIZE_PARENT);

      ___gtk_widget_propagate_hierarchy_changed (GTK_WIDGET (plug), GTK_WIDGET (plug));
    }
  else
    {
      if (gtk_window_get_props (GTK_WINDOW (plug))->focus_widget)
	__gtk_window_set_focus (GTK_WINDOW (plug), NULL);
      if (gtk_window_get_props (GTK_WINDOW (plug))->default_widget)
	__gtk_window_set_default (GTK_WINDOW (plug), NULL);
	  
      gtk_plug_get_props (plug)->modality_group = __gtk_window_group_new ();
      __gtk_window_group_add_window (gtk_plug_get_props (plug)->modality_group, GTK_WINDOW (plug));
      
      ___gtk_window_set_is_toplevel (GTK_WINDOW (plug), TRUE);
      __gtk_container_set_resize_mode (GTK_CONTAINER (plug), GTK_RESIZE_QUEUE);

      ___gtk_widget_propagate_hierarchy_changed (GTK_WIDGET (plug), NULL);
    }
}

/**
 * __gtk_plug_get_id:
 * @plug: a #GtkPlug.
 * 
 * Gets the window ID of a #GtkPlug widget, which can then
 * be used to embed this window inside another window, for
 * instance with __gtk_socket_add_id().
 * 
 * Return value: the window ID for the plug
 **/
GdkNativeWindow
__gtk_plug_get_id (GtkPlug *plug)
{
  g_return_val_if_fail (GTK_IS_PLUG (plug), 0);

  if (!__gtk_widget_get_realized (GTK_WIDGET (plug)))
    __gtk_widget_realize (GTK_WIDGET (plug));

  return _gtk_plug_windowing_get_id (plug);
}

/**
 * __gtk_plug_get_embedded:
 * @plug: a #GtkPlug
 *
 * Determines whether the plug is embedded in a socket.
 *
 * Return value: %TRUE if the plug is embedded in a socket
 *
 * Since: 2.14
 **/
gboolean
__gtk_plug_get_embedded (GtkPlug *plug)
{
  g_return_val_if_fail (GTK_IS_PLUG (plug), FALSE);

  return gtk_plug_get_props (plug)->socket_window != NULL;
}

/**
 * __gtk_plug_get_socket_window:
 * @plug: a #GtkPlug
 *
 * Retrieves the socket the plug is embedded in.
 *
 * Return value: (transfer none): the window of the socket, or %NULL
 *
 * Since: 2.14
 **/
GdkWindow *
__gtk_plug_get_socket_window (GtkPlug *plug)
{
  g_return_val_if_fail (GTK_IS_PLUG (plug), NULL);

  return gtk_plug_get_props (plug)->socket_window;
}

/**
 * ___gtk_plug_add_to_socket:
 * @plug: a #GtkPlug
 * @socket_: a #GtkSocket
 * 
 * Adds a plug to a socket within the same application.
 **/
void
___gtk_plug_add_to_socket (GtkPlug   *plug,
			 GtkSocket *socket_)
{
  GtkWidget *widget;
  gint w, h;
  
  g_return_if_fail (GTK_IS_PLUG (plug));
  g_return_if_fail (GTK_IS_SOCKET (socket_));
  g_return_if_fail (__gtk_widget_get_realized (GTK_WIDGET (socket_)));

  widget = GTK_WIDGET (plug);

  gtk_plug_set_is_child (plug, TRUE);
  gtk_plug_get_props (plug)->same_app = TRUE;
  gtk_socket_get_props (socket_)->same_app = TRUE;
  gtk_socket_get_props (socket_)->plug_widget = widget;

  gtk_plug_get_props (plug)->socket_window = gtk_widget_get_props (GTK_WIDGET (socket_))->window;
  g_object_ref (gtk_plug_get_props (plug)->socket_window);
  g_signal_emit (plug, plug_signals[EMBEDDED], 0);
  g_object_notify (G_OBJECT (plug), "embedded");

  if (__gtk_widget_get_realized (widget))
    {
      w = __gdk_window_get_width (gtk_widget_get_props (widget)->window);
      h = __gdk_window_get_height (gtk_widget_get_props (widget)->window);
      __gdk_window_reparent (gtk_widget_get_props (widget)->window, gtk_plug_get_props (plug)->socket_window, -w, -h);
    }

  __gtk_widget_set_parent (widget, GTK_WIDGET (socket_));

  g_signal_emit_by_name (socket_, "plug-added");
}

/**
 * _gtk_plug_send_delete_event:
 * @widget: a #GtkWidget
 *
 * Send a GDK_DELETE event to the @widget and destroy it if
 * necessary. Internal GTK function, called from this file or the
 * backend-specific GtkPlug implementation.
 */
void
_gtk_plug_send_delete_event (GtkWidget *widget)
{
  GdkEvent *event = __gdk_event_new (GDK_DELETE);

  event->any.window = g_object_ref (gtk_widget_get_props (widget)->window);
  event->any.send_event = FALSE;

  g_object_ref (widget);

  if (!__gtk_widget_event (widget, event))
    __gtk_widget_destroy (widget);

  g_object_unref (widget);

  __gdk_event_free (event);
}

/**
 * ___gtk_plug_remove_from_socket:
 * @plug: a #GtkPlug
 * @socket_: a #GtkSocket
 * 
 * Removes a plug from a socket within the same application.
 **/
void
___gtk_plug_remove_from_socket (GtkPlug   *plug,
			      GtkSocket *socket_)
{
  GtkWidget *widget;
  gboolean result;
  gboolean widget_was_visible;

  g_return_if_fail (GTK_IS_PLUG (plug));
  g_return_if_fail (GTK_IS_SOCKET (socket_));
  g_return_if_fail (__gtk_widget_get_realized (GTK_WIDGET (plug)));

  widget = GTK_WIDGET (plug);

  if (GTK_WIDGET_IN_REPARENT (widget))
    return;

  g_object_ref (plug);
  g_object_ref (socket_);

  widget_was_visible = __gtk_widget_get_visible (widget);
  
  __gdk_window_hide (gtk_widget_get_props (widget)->window);
  GTK_PRIVATE_SET_FLAG (plug, GTK_IN_REPARENT);
  __gdk_window_reparent (gtk_widget_get_props (widget)->window,
		       __gtk_widget_get_root_window (widget),
		       0, 0);
  __gtk_widget_unparent (GTK_WIDGET (plug));
  GTK_PRIVATE_UNSET_FLAG (plug, GTK_IN_REPARENT);
  
  gtk_socket_get_props (socket_)->plug_widget = NULL;
  if (gtk_socket_get_props (socket_)->plug_window != NULL)
    {
      g_object_unref (gtk_socket_get_props (socket_)->plug_window);
      gtk_socket_get_props (socket_)->plug_window = NULL;
    }
  
  gtk_socket_get_props (socket_)->same_app = FALSE;

  gtk_plug_get_props (plug)->same_app = FALSE;
  if (gtk_plug_get_props (plug)->socket_window != NULL)
    {
      g_object_unref (gtk_plug_get_props (plug)->socket_window);
      gtk_plug_get_props (plug)->socket_window = NULL;
    }
  gtk_plug_set_is_child (plug, FALSE);

  g_signal_emit_by_name (socket_, "plug-removed", &result);
  if (!result)
    __gtk_widget_destroy (GTK_WIDGET (socket_));

  if (gtk_widget_get_props (widget)->window)
    _gtk_plug_send_delete_event (widget);

  g_object_unref (plug);

  if (widget_was_visible && __gtk_widget_get_visible (GTK_WIDGET (socket_)))
    __gtk_widget_queue_resize (GTK_WIDGET (socket_));

  g_object_unref (socket_);
}

/**
 * __gtk_plug_construct:
 * @plug: a #GtkPlug.
 * @socket_id: the XID of the socket's window.
 *
 * Finish the initialization of @plug for a given #GtkSocket identified by
 * @socket_id. This function will generally only be used by classes deriving from #GtkPlug.
 **/
void
__gtk_plug_construct (GtkPlug         *plug,
		    GdkNativeWindow  socket_id)
{
  __gtk_plug_construct_for_display (plug, __gdk_display_get_default (), socket_id);
}

/**
 * __gtk_plug_construct_for_display:
 * @plug: a #GtkPlug.
 * @display: the #GdkDisplay associated with @socket_id's 
 *	     #GtkSocket.
 * @socket_id: the XID of the socket's window.
 *
 * Finish the initialization of @plug for a given #GtkSocket identified by
 * @socket_id which is currently displayed on @display.
 * This function will generally only be used by classes deriving from #GtkPlug.
 *
 * Since: 2.2
 **/
void
__gtk_plug_construct_for_display (GtkPlug         *plug,
				GdkDisplay	*display,
				GdkNativeWindow  socket_id)
{
  if (socket_id)
    {
      gpointer user_data = NULL;

      gtk_plug_get_props (plug)->socket_window = __gdk_window_lookup_for_display (display, socket_id);
      if (gtk_plug_get_props (plug)->socket_window)
	{
	  __gdk_window_get_user_data (gtk_plug_get_props (plug)->socket_window, &user_data);

	  if (user_data)
	    {
	      if (GTK_IS_SOCKET (user_data))
		___gtk_plug_add_to_socket (plug, user_data);
	      else
		{
		  g_warning (G_STRLOC "Can't create GtkPlug as child of non-GtkSocket");
		  gtk_plug_get_props (plug)->socket_window = NULL;
		}
	    }
	  else
	    g_object_ref (gtk_plug_get_props (plug)->socket_window);
	}
      else
	gtk_plug_get_props (plug)->socket_window = __gdk_window_foreign_new_for_display (display, socket_id);

      if (gtk_plug_get_props (plug)->socket_window) {
	g_signal_emit (plug, plug_signals[EMBEDDED], 0);

        g_object_notify (G_OBJECT (plug), "embedded");
      }
    }
}

/**
 * __gtk_plug_new:
 * @socket_id:  the window ID of the socket, or 0.
 * 
 * Creates a new plug widget inside the #GtkSocket identified
 * by @socket_id. If @socket_id is 0, the plug is left "unplugged" and
 * can later be plugged into a #GtkSocket by  __gtk_socket_add_id().
 * 
 * Return value: the new #GtkPlug widget.
 **/
GtkWidget*
__gtk_plug_new (GdkNativeWindow socket_id)
{
  return __gtk_plug_new_for_display (__gdk_display_get_default (), socket_id);
}

/**
 * __gtk_plug_new_for_display:
 * @display: the #GdkDisplay on which @socket_id is displayed
 * @socket_id: the XID of the socket's window.
 * 
 * Create a new plug widget inside the #GtkSocket identified by socket_id.
 *
 * Return value: the new #GtkPlug widget.
 *
 * Since: 2.2
 */
GtkWidget*
__gtk_plug_new_for_display (GdkDisplay	  *display,
			  GdkNativeWindow  socket_id)
{
  GtkPlug *plug;

  plug = g_object_new (GTK_TYPE_PLUG, NULL);
  __gtk_plug_construct_for_display (plug, display, socket_id);
  return GTK_WIDGET (plug);
}

static void
gtk_plug_finalize (GObject *object)
{
  GtkPlug *plug = GTK_PLUG (object);

  if (gtk_plug_get_props (plug)->grabbed_keys)
    {
      g_hash_table_destroy (gtk_plug_get_props (plug)->grabbed_keys);
      gtk_plug_get_props (plug)->grabbed_keys = NULL;
    }
  
  G_OBJECT_CLASS (gtk_plug_parent_class)->finalize (object);
}

static void
gtk_plug_unrealize (GtkWidget *widget)
{
  GtkPlug *plug = GTK_PLUG (widget);

  if (gtk_plug_get_props (plug)->socket_window != NULL)
    {
      __gdk_window_set_user_data (gtk_plug_get_props (plug)->socket_window, NULL);
      g_object_unref (gtk_plug_get_props (plug)->socket_window);
      gtk_plug_get_props (plug)->socket_window = NULL;

      g_object_notify (G_OBJECT (widget), "embedded");
    }

  if (!gtk_plug_get_props (plug)->same_app)
    {
      if (gtk_plug_get_props (plug)->modality_window)
	_gtk_plug_handle_modality_off (plug);

      __gtk_window_group_remove_window (gtk_plug_get_props (plug)->modality_group, GTK_WINDOW (plug));
      g_object_unref (gtk_plug_get_props (plug)->modality_group);
    }

  GTK_WIDGET_CLASS (gtk_plug_parent_class)->unrealize (widget);
}

static void
gtk_plug_realize (GtkWidget *widget)
{
  GtkWindow *window = GTK_WINDOW (widget);
  GtkPlug *plug = GTK_PLUG (widget);
  GdkWindowAttr attributes;
  gint attributes_mask;

  __gtk_widget_set_realized (widget, TRUE);

  attributes.window_type = GDK_WINDOW_CHILD;	/* XXX GDK_WINDOW_PLUG ? */
  attributes.title = gtk_window_get_props (window)->title;
  attributes.wmclass_name = gtk_window_get_props (window)->wmclass_name;
  attributes.wmclass_class = gtk_window_get_props (window)->wmclass_class;
  attributes.width = gtk_widget_get_props (widget)->allocation.width;
  attributes.height = gtk_widget_get_props (widget)->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;

  /* this isn't right - we should match our parent's visual/colormap.
   * though that will require handling "foreign" colormaps */
  attributes.visual = __gtk_widget_get_visual (widget);
  attributes.colormap = __gtk_widget_get_colormap (widget);
  attributes.event_mask = __gtk_widget_get_events (widget);
  attributes.event_mask |= (GDK_EXPOSURE_MASK |
			    GDK_KEY_PRESS_MASK |
			    GDK_KEY_RELEASE_MASK |
			    GDK_ENTER_NOTIFY_MASK |
			    GDK_LEAVE_NOTIFY_MASK |
			    GDK_STRUCTURE_MASK);

  attributes_mask = GDK_WA_VISUAL | GDK_WA_COLORMAP;
  attributes_mask |= (gtk_window_get_props (window)->title ? GDK_WA_TITLE : 0);
  attributes_mask |= (gtk_window_get_props (window)->wmclass_name ? GDK_WA_WMCLASS : 0);

  if (__gtk_widget_is_toplevel (widget))
    {
      attributes.window_type = GDK_WINDOW_TOPLEVEL;

      __gdk_error_trap_push ();
      if (gtk_plug_get_props (plug)->socket_window)
	gtk_widget_get_props (widget)->window = __gdk_window_new (gtk_plug_get_props (plug)->socket_window, 
					 &attributes, attributes_mask);
      else /* If it's a passive plug, we use the root window */
	gtk_widget_get_props (widget)->window = __gdk_window_new (__gtk_widget_get_root_window (gtk_widget_get_props (widget)),
					 &attributes, attributes_mask);

      __gdk_display_sync (__gtk_widget_get_display (widget));
      if (__gdk_error_trap_pop ()) /* Uh-oh */
	{
	  __gdk_error_trap_push ();
	  __gdk_window_destroy (gtk_widget_get_props (widget)->window);
	  __gdk_flush ();
	  __gdk_error_trap_pop ();
	  gtk_widget_get_props (widget)->window = __gdk_window_new (__gtk_widget_get_root_window (gtk_widget_get_props (widget)),
					   &attributes, attributes_mask);
	}
      
      __gdk_window_add_filter (gtk_widget_get_props (widget)->window,
			     _gtk_plug_windowing_filter_func,
			     widget);

      gtk_plug_get_props (plug)->modality_group = __gtk_window_group_new ();
      __gtk_window_group_add_window (gtk_plug_get_props (plug)->modality_group, window);
      
      _gtk_plug_windowing_realize_toplevel (plug);
    }
  else
    gtk_widget_get_props (widget)->window = __gdk_window_new (__gtk_widget_get_parent_window (gtk_widget_get_props (widget)), 
				     &attributes, attributes_mask);      
  
  __gdk_window_set_user_data (gtk_widget_get_props (widget)->window, window);

  gtk_widget_get_props (widget)->style = __gtk_style_attach (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window);
  __gtk_style_set_background (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window, GTK_STATE_NORMAL);

  __gdk_window_enable_synchronized_configure (gtk_widget_get_props (widget)->window);
}

static void
gtk_plug_show (GtkWidget *widget)
{
  if (__gtk_widget_is_toplevel (widget))
    GTK_WIDGET_CLASS (gtk_plug_parent_class)->show (widget);
  else
    GTK_WIDGET_CLASS (bin_class)->show (widget);
}

static void
gtk_plug_hide (GtkWidget *widget)
{
  if (__gtk_widget_is_toplevel (widget))
    GTK_WIDGET_CLASS (gtk_plug_parent_class)->hide (widget);
  else
    GTK_WIDGET_CLASS (bin_class)->hide (widget);
}

/* From gdkinternals.h */
void gdk_synthesize_window_state (GdkWindow     *window,
                                  GdkWindowState unset_flags,
                                  GdkWindowState set_flags);

static void
gtk_plug_map (GtkWidget *widget)
{
  if (__gtk_widget_is_toplevel (widget))
    {
      GtkBin *bin = GTK_BIN (widget);
      GtkPlug *plug = GTK_PLUG (widget);
      
      __gtk_widget_set_mapped (widget, TRUE);

      if (gtk_bin_get_props (bin)->child &&
	  __gtk_widget_get_visible (gtk_bin_get_props (bin)->child) &&
	  !__gtk_widget_get_mapped (gtk_bin_get_props (bin)->child))
	__gtk_widget_map (gtk_bin_get_props (bin)->child);

      _gtk_plug_windowing_map_toplevel (plug);
      
      gdk_synthesize_window_state (gtk_widget_get_props (widget)->window,
				   GDK_WINDOW_STATE_WITHDRAWN,
				   0);
    }
  else
    GTK_WIDGET_CLASS (bin_class)->map (widget);
}

static void
gtk_plug_unmap (GtkWidget *widget)
{
  if (__gtk_widget_is_toplevel (widget))
    {
      GtkPlug *plug = GTK_PLUG (widget);

      __gtk_widget_set_mapped (widget, FALSE);

      __gdk_window_hide (gtk_widget_get_props (widget)->window);

      _gtk_plug_windowing_unmap_toplevel (plug);
      
      gdk_synthesize_window_state (gtk_widget_get_props (widget)->window,
				   0,
				   GDK_WINDOW_STATE_WITHDRAWN);
    }
  else
    GTK_WIDGET_CLASS (bin_class)->unmap (widget);
}

static void
gtk_plug_size_allocate (GtkWidget     *widget,
			GtkAllocation *allocation)
{
  if (__gtk_widget_is_toplevel (widget))
    GTK_WIDGET_CLASS (gtk_plug_parent_class)->size_allocate (widget, allocation);
  else
    {
      GtkBin *bin = GTK_BIN (widget);

      gtk_widget_get_props (widget)->allocation = *allocation;

      if (__gtk_widget_get_realized (widget))
	__gdk_window_move_resize (gtk_widget_get_props (widget)->window,
				allocation->x, allocation->y,
				allocation->width, allocation->height);

      if (gtk_bin_get_props (bin)->child && __gtk_widget_get_visible (gtk_bin_get_props (bin)->child))
	{
	  GtkAllocation child_allocation;
	  
	  child_allocation.x = child_allocation.y = gtk_container_get_props (GTK_CONTAINER (widget))->border_width;
	  child_allocation.width =
	    MAX (1, (gint)allocation->width - child_allocation.x * 2);
	  child_allocation.height =
	    MAX (1, (gint)allocation->height - child_allocation.y * 2);
	  
	  __gtk_widget_size_allocate (gtk_bin_get_props (bin)->child, &child_allocation);
	}
      
    }
}

static gboolean
gtk_plug_key_press_event (GtkWidget   *widget,
			  GdkEventKey *event)
{
  if (__gtk_widget_is_toplevel (widget))
    return GTK_WIDGET_CLASS (gtk_plug_parent_class)->key_press_event (widget, event);
  else
    return FALSE;
}

static gboolean
gtk_plug_focus_event (GtkWidget      *widget,
		      GdkEventFocus  *event)
{
  /* We eat focus-in events and focus-out events, since they
   * can be generated by something like a keyboard grab on
   * a child of the plug.
   */
  return FALSE;
}

static void
gtk_plug_set_focus (GtkWindow *window,
		    GtkWidget *focus)
{
  GtkPlug *plug = GTK_PLUG (window);

  GTK_WINDOW_CLASS (gtk_plug_parent_class)->set_focus (window, focus);
  
  /* Ask for focus from embedder
   */

  if (focus && !gtk_window_get_props (window)->has_toplevel_focus)
    _gtk_plug_windowing_set_focus (plug);
}

static guint
grabbed_key_hash (gconstpointer a)
{
  const GrabbedKey *key = a;
  guint h;
  
  h = key->accelerator_key << 16;
  h ^= key->accelerator_key >> 16;
  h ^= key->accelerator_mods;

  return h;
}

static gboolean
grabbed_key_equal (gconstpointer a, gconstpointer b)
{
  const GrabbedKey *keya = a;
  const GrabbedKey *keyb = b;

  return (keya->accelerator_key == keyb->accelerator_key &&
	  keya->accelerator_mods == keyb->accelerator_mods);
}

static void
add_grabbed_key (gpointer key, gpointer val, gpointer data)
{
  GrabbedKey *grabbed_key = key;
  GtkPlug *plug = data;

  if (!gtk_plug_get_props (plug)->grabbed_keys ||
      !g_hash_table_lookup (gtk_plug_get_props (plug)->grabbed_keys, grabbed_key))
    {
      _gtk_plug_windowing_add_grabbed_key (plug,
					   grabbed_key->accelerator_key,
					   grabbed_key->accelerator_mods);
    }
}

static void
add_grabbed_key_always (gpointer key,
			gpointer val,
			gpointer data)
{
  GrabbedKey *grabbed_key = key;
  GtkPlug *plug = data;

  _gtk_plug_windowing_add_grabbed_key (plug,
				       grabbed_key->accelerator_key,
				       grabbed_key->accelerator_mods);
}

/**
 * _gtk_plug_add_all_grabbed_keys:
 *
 * @plug: a #GtkPlug
 *
 * Calls _gtk_plug_windowing_add_grabbed_key() on all the grabbed keys
 * in the @plug.
 */
void
_gtk_plug_add_all_grabbed_keys (GtkPlug *plug)
{
  if (gtk_plug_get_props (plug)->grabbed_keys)
    g_hash_table_foreach (gtk_plug_get_props (plug)->grabbed_keys, add_grabbed_key_always, gtk_plug_get_props (plug));
}

static void
remove_grabbed_key (gpointer key, gpointer val, gpointer data)
{
  GrabbedKey *grabbed_key = key;
  GtkPlug *plug = data;

  if (!gtk_plug_get_props (plug)->grabbed_keys ||
      !g_hash_table_lookup (gtk_plug_get_props (plug)->grabbed_keys, grabbed_key))
    {
      _gtk_plug_windowing_remove_grabbed_key (plug, 
					      grabbed_key->accelerator_key,
					      grabbed_key->accelerator_mods);
    }
}

static void
keys_foreach (GtkWindow      *window,
	      guint           keyval,
	      GdkModifierType modifiers,
	      gboolean        is_mnemonic,
	      gpointer        data)
{
  GHashTable *new_grabbed_keys = data;
  GrabbedKey *key = g_slice_new (GrabbedKey);

  key->accelerator_key = keyval;
  key->accelerator_mods = modifiers;
  
  g_hash_table_replace (new_grabbed_keys, key, key);
}

static void
grabbed_key_free (gpointer data)
{
  g_slice_free (GrabbedKey, data);
}

static void
gtk_plug_keys_changed (GtkWindow *window)
{
  GHashTable *new_grabbed_keys, *old_grabbed_keys;
  GtkPlug *plug = GTK_PLUG (window);

  new_grabbed_keys = g_hash_table_new_full (grabbed_key_hash, grabbed_key_equal, (GDestroyNotify)grabbed_key_free, NULL);
  ___gtk_window_keys_foreach (window, keys_foreach, new_grabbed_keys);

  if (gtk_plug_get_props (plug)->socket_window)
    g_hash_table_foreach (new_grabbed_keys, add_grabbed_key, plug);

  old_grabbed_keys = gtk_plug_get_props (plug)->grabbed_keys;
  gtk_plug_get_props (plug)->grabbed_keys = new_grabbed_keys;

  if (old_grabbed_keys)
    {
      if (gtk_plug_get_props (plug)->socket_window)
	g_hash_table_foreach (old_grabbed_keys, remove_grabbed_key, plug);
      g_hash_table_destroy (old_grabbed_keys);
    }
}

static gboolean
gtk_plug_focus (GtkWidget        *widget,
		GtkDirectionType  direction)
{
  GtkBin *bin = GTK_BIN (widget);
  GtkPlug *plug = GTK_PLUG (widget);
  GtkWindow *window = GTK_WINDOW (widget);
  GtkContainer *container = GTK_CONTAINER (widget);
  GtkWidget *old_focus_child = gtk_container_get_props (container)->focus_child;
  GtkWidget *parent;
  
  /* We override GtkWindow's behavior, since we don't want wrapping here.
   */
  if (old_focus_child)
    {
      if (__gtk_widget_child_focus (old_focus_child, direction))
	return TRUE;

      if (gtk_window_get_props (window)->focus_widget)
	{
	  /* Wrapped off the end, clear the focus setting for the toplevel */
	  parent = gtk_widget_get_props (gtk_window_get_props (window)->focus_widget)->parent;
	  while (parent)
	    {
	      __gtk_container_set_focus_child (GTK_CONTAINER (parent), NULL);
	      parent = gtk_widget_get_props (GTK_WIDGET (parent))->parent;
	    }
	  
	  __gtk_window_set_focus (GTK_WINDOW (container), NULL);
	}
    }
  else
    {
      /* Try to focus the first widget in the window */
      if (gtk_bin_get_props (bin)->child && __gtk_widget_child_focus (gtk_bin_get_props (bin)->child, direction))
        return TRUE;
    }

  if (!gtk_container_get_props (GTK_CONTAINER (window))->focus_child)
    _gtk_plug_windowing_focus_to_parent (plug, direction);

  return FALSE;
}

static void
gtk_plug_check_resize (GtkContainer *container)
{
  if (__gtk_widget_is_toplevel (GTK_WIDGET (container)))
    GTK_CONTAINER_CLASS (gtk_plug_parent_class)->check_resize (container);
  else
    GTK_CONTAINER_CLASS (bin_class)->check_resize (container);
}

/**
 * _gtk_plug_handle_modality_on:
 *
 * @plug: a #GtkPlug
 *
 * Called from the GtkPlug backend when the corresponding socket has
 * told the plug that it modality has toggled on.
 */
void
_gtk_plug_handle_modality_on (GtkPlug *plug)
{
  if (!gtk_plug_get_props (plug)->modality_window)
    {
      gtk_plug_get_props (plug)->modality_window = __gtk_window_new (GTK_WINDOW_POPUP);
      __gtk_window_set_screen (GTK_WINDOW (gtk_plug_get_props (plug)->modality_window),
			     __gtk_widget_get_screen (GTK_WIDGET (plug)));
      __gtk_widget_realize (gtk_plug_get_props (plug)->modality_window);
      __gtk_window_group_add_window (gtk_plug_get_props (plug)->modality_group, GTK_WINDOW (gtk_plug_get_props (plug)->modality_window));
      __gtk_grab_add (gtk_plug_get_props (plug)->modality_window);
    }
}

/**
 * _gtk_plug_handle_modality_off:
 *
 * @plug: a #GtkPlug
 *
 * Called from the GtkPlug backend when the corresponding socket has
 * told the plug that it modality has toggled off.
 */
void
_gtk_plug_handle_modality_off (GtkPlug *plug)
{
  if (gtk_plug_get_props (plug)->modality_window)
    {
      __gtk_widget_destroy (gtk_plug_get_props (plug)->modality_window);
      gtk_plug_get_props (plug)->modality_window = NULL;
    }
}

/**
 * _gtk_plug_focus_first_last:
 *
 * @plug: a #GtkPlug
 * @direction: a direction
 *
 * Called from the GtkPlug backend when the corresponding socket has
 * told the plug that it has received the focus.
 */
void
_gtk_plug_focus_first_last (GtkPlug          *plug,
			    GtkDirectionType  direction)
{
  GtkWindow *window = GTK_WINDOW (plug);
  GtkWidget *parent;

  if (gtk_window_get_props (window)->focus_widget)
    {
      parent = gtk_widget_get_props (gtk_window_get_props (window)->focus_widget)->parent;
      while (parent)
	{
	  __gtk_container_set_focus_child (GTK_CONTAINER (parent), NULL);
	  parent = gtk_widget_get_props (GTK_WIDGET (parent))->parent;
	}
      
      __gtk_window_set_focus (GTK_WINDOW (plug), NULL);
    }

  __gtk_widget_child_focus (GTK_WIDGET (plug), direction);
}
