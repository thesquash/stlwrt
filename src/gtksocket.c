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
#include <string.h>

#include <gdkkeysyms.h>
#include <gtkmain.h>

#include <gtkwindow.h>
#include <gtkplug.h>
#include <gtkprivate.h>
#include <gtksocket.h>
#include <gtksocketprivate.h>
#include <gtkdnd.h>
#include <gtkintl.h>



/**
 * SECTION:gtksocket
 * @Short_description: Container for widgets from other processes
 * @Title: GtkSocket
 * @See_also: #GtkPlug, <ulink url="http://www.freedesktop.org/Standards/xembed-spec">XEmbed</ulink>
 *
 * Together with #GtkPlug, #GtkSocket provides the ability
 * to embed widgets from one process into another process
 * in a fashion that is transparent to the user. One
 * process creates a #GtkSocket widget and passes
 * that widget's window ID to the other process,
 * which then creates a #GtkPlug with that window ID.
 * Any widgets contained in the #GtkPlug then will appear
 * inside the first application's window.
 *
 * The socket's window ID is obtained by using
 * __gtk_socket_get_id(). Before using this function,
 * the socket must have been realized, and for hence,
 * have been added to its parent.
 *
 * <example>
 * <title>Obtaining the window ID of a socket.</title>
 * <programlisting>
 * GtkWidget *socket = __gtk_socket_new (<!-- -->);
 * __gtk_widget_show (socket);
 * __gtk_container_add (GTK_CONTAINER (parent), socket);
 *
 * /<!---->* The following call is only necessary if one of
 *  * the ancestors of the socket is not yet visible.
 *  *<!---->/
 * __gtk_widget_realize (socket);
 * g_print ("The ID of the sockets window is %#x\n",
 *          __gtk_socket_get_id (socket));
 * </programlisting>
 * </example>
 *
 * Note that if you pass the window ID of the socket to another
 * process that will create a plug in the socket, you
 * must make sure that the socket widget is not destroyed
 * until that plug is created. Violating this rule will
 * cause unpredictable consequences, the most likely
 * consequence being that the plug will appear as a
 * separate toplevel window. You can check if the plug
 * has been created by using __gtk_socket_get_plug_window(). If
 * it returns a non-%NULL value, then the plug has been
 * successfully created inside of the socket.
 *
 * When GTK+ is notified that the embedded window has been
 * destroyed, then it will destroy the socket as well. You
 * should always, therefore, be prepared for your sockets
 * to be destroyed at any time when the main event loop
 * is running. To prevent this from happening, you can
 * connect to the #GtkSocket::plug-removed signal.
 *
 * The communication between a #GtkSocket and a #GtkPlug follows the
 * <ulink url="http://www.freedesktop.org/Standards/xembed-spec">XEmbed</ulink>
 * protocol. This protocol has also been implemented in other toolkits, e.g.
 * <application>Qt</application>, allowing the same level of integration
 * when embedding a <application>Qt</application> widget in GTK or vice versa.
 *
 * A socket can also be used to swallow arbitrary
 * pre-existing top-level windows using __gtk_socket_steal(),
 * though the integration when this is done will not be as close
 * as between a #GtkPlug and a #GtkSocket.
 *
 * <note>
 * The #GtkPlug and #GtkSocket widgets are currently not available
 * on all platforms supported by GTK+.
 * </note>
 */

/* Forward declararations */

static void     gtk_socket_finalize             (GObject          *object);
static void     gtk_socket_notify               (GObject          *object,
						 GParamSpec       *pspec);
static void     gtk_socket_realize              (GtkWidget        *widget);
static void     gtk_socket_unrealize            (GtkWidget        *widget);
static void     gtk_socket_size_request         (GtkWidget        *widget,
						 GtkRequisition   *requisition);
static void     gtk_socket_size_allocate        (GtkWidget        *widget,
						 GtkAllocation    *allocation);
static void     gtk_socket_hierarchy_changed    (GtkWidget        *widget,
						 GtkWidget        *old_toplevel);
static void     gtk_socket_grab_notify          (GtkWidget        *widget,
						 gboolean          was_grabbed);
static gboolean gtk_socket_key_event            (GtkWidget        *widget,
						 GdkEventKey      *event);
static gboolean gtk_socket_focus                (GtkWidget        *widget,
						 GtkDirectionType  direction);
static void     gtk_socket_remove               (GtkContainer     *container,
						 GtkWidget        *widget);
static void     gtk_socket_forall               (GtkContainer     *container,
						 gboolean          include_internals,
						 GtkCallback       callback,
						 gpointer          callback_data);


/* Local data */

typedef struct
{
  guint			 accel_key;
  GdkModifierType	 accel_mods;
} GrabbedKey;

enum {
  PLUG_ADDED,
  PLUG_REMOVED,
  LAST_SIGNAL
}; 

static guint socket_signals[LAST_SIGNAL] = { 0 };

STLWRT_DEFINE_VTYPE (GtkSocket, gtk_socket, GTK_TYPE_CONTAINER, G_TYPE_FLAG_NONE,
                     G_ADD_PRIVATE (GtkSocket))

static void
gtk_socket_finalize (GObject *object)
{
  GtkSocket *socket = GTK_SOCKET (object);
  
  g_object_unref (gtk_socket_get_props (socket)->accel_group);
  gtk_socket_get_props (socket)->accel_group = NULL;

  G_OBJECT_CLASS (gtk_socket_parent_class)->finalize (object);
}

static void
gtk_socket_class_init (GtkSocketClass *class)
{
  GtkWidgetClass *widget_class;
  GtkContainerClass *container_class;
  GObjectClass *gobject_class;

  gobject_class = (GObjectClass *) class;
  widget_class = (GtkWidgetClass*) class;
  container_class = (GtkContainerClass*) class;

  gobject_class->finalize = gtk_socket_finalize;
  gobject_class->notify = gtk_socket_notify;

  widget_class->realize = gtk_socket_realize;
  widget_class->unrealize = gtk_socket_unrealize;
  widget_class->size_request = gtk_socket_size_request;
  widget_class->size_allocate = gtk_socket_size_allocate;
  widget_class->hierarchy_changed = gtk_socket_hierarchy_changed;
  widget_class->grab_notify = gtk_socket_grab_notify;
  widget_class->key_press_event = gtk_socket_key_event;
  widget_class->key_release_event = gtk_socket_key_event;
  widget_class->focus = gtk_socket_focus;

  /* We don't want to show_all/hide_all the in-process
   * plug, if any.
   */
  widget_class->show_all = __gtk_widget_show;
  widget_class->hide_all = __gtk_widget_hide;
  
  container_class->remove = gtk_socket_remove;
  container_class->forall = gtk_socket_forall;

  /**
   * GtkSocket::plug-added:
   * @socket_: the object which received the signal
   *
   * This signal is emitted when a client is successfully
   * added to the socket. 
   */
  socket_signals[PLUG_ADDED] =
    g_signal_new (I_("plug-added"),
		  G_OBJECT_CLASS_TYPE (class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GtkSocketClass, plug_added),
		  NULL, NULL,
		  NULL,
		  G_TYPE_NONE, 0);

  /**
   * GtkSocket::plug-removed:
   * @socket_: the object which received the signal
   *
   * This signal is emitted when a client is removed from the socket. 
   * The default action is to destroy the #GtkSocket widget, so if you 
   * want to reuse it you must add a signal handler that returns %TRUE. 
   *
   * Return value: %TRUE to stop other handlers from being invoked.
   */
  socket_signals[PLUG_REMOVED] =
    g_signal_new (I_("plug-removed"),
		  G_OBJECT_CLASS_TYPE (class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GtkSocketClass, plug_removed),
                  ___gtk_boolean_handled_accumulator, NULL,
		  NULL,
		  G_TYPE_BOOLEAN, 0);
}

static void
gtk_socket_init (GtkSocket *socket)
{
  gtk_socket_get_props (socket)->request_width = 0;
  gtk_socket_get_props (socket)->request_height = 0;
  gtk_socket_get_props (socket)->current_width = 0;
  gtk_socket_get_props (socket)->current_height = 0;
  
  gtk_socket_get_props (socket)->plug_window = NULL;
  gtk_socket_get_props (socket)->plug_widget = NULL;
  gtk_socket_get_props (socket)->focus_in = FALSE;
  gtk_socket_get_props (socket)->have_size = FALSE;
  gtk_socket_get_props (socket)->need_map = FALSE;
  gtk_socket_get_props (socket)->active = FALSE;

  gtk_socket_get_props (socket)->accel_group = __gtk_accel_group_new ();
  g_object_set_data (G_OBJECT (gtk_socket_get_props (socket)->accel_group), I_("gtk-gtk_socket_get_props (socket)"), gtk_socket_get_props (socket));
}

/**
 * __gtk_socket_new:
 * 
 * Create a new empty #GtkSocket.
 * 
 * Return value:  the new #GtkSocket.
 **/
GtkWidget*
__gtk_socket_new (void)
{
  GtkSocket *socket;

  socket = g_object_new (GTK_TYPE_SOCKET, NULL);

  return GTK_WIDGET (socket);
}

/**
 * __gtk_socket_steal:
 * @socket_: a #GtkSocket
 * @wid: the window ID of an existing toplevel window.
 * 
 * Reparents a pre-existing toplevel window into a #GtkSocket. This is
 * meant to embed clients that do not know about embedding into a
 * #GtkSocket, however doing so is inherently unreliable, and using
 * this function is not recommended.
 *
 * The #GtkSocket must have already be added into a toplevel window
 *  before you can make this call.
 **/
void           
__gtk_socket_steal (GtkSocket      *socket,
		  GdkNativeWindow wid)
{
  g_return_if_fail (GTK_IS_SOCKET (socket));
  g_return_if_fail (GTK_WIDGET_ANCHORED (socket));

  if (!__gtk_widget_get_realized (GTK_WIDGET (socket)))
    __gtk_widget_realize (GTK_WIDGET (socket));

  _gtk_socket_add_window (socket, wid, TRUE);
}

/**
 * __gtk_socket_add_id:
 * @socket_: a #GtkSocket
 * @window_id: the window ID of a client participating in the XEMBED protocol.
 *
 * Adds an XEMBED client, such as a #GtkPlug, to the #GtkSocket.  The
 * client may be in the same process or in a different process. 
 * 
 * To embed a #GtkPlug in a #GtkSocket, you can either create the
 * #GtkPlug with <literal>__gtk_plug_new (0)</literal>, call 
 * __gtk_plug_get_id() to get the window ID of the plug, and then pass that to the
 * __gtk_socket_add_id(), or you can call __gtk_socket_get_id() to get the
 * window ID for the socket, and call __gtk_plug_new() passing in that
 * ID.
 *
 * The #GtkSocket must have already be added into a toplevel window
 *  before you can make this call.
 **/
void           
__gtk_socket_add_id (GtkSocket      *socket,
		   GdkNativeWindow window_id)
{
  g_return_if_fail (GTK_IS_SOCKET (socket));
  g_return_if_fail (GTK_WIDGET_ANCHORED (socket));

  if (!__gtk_widget_get_realized (GTK_WIDGET (socket)))
    __gtk_widget_realize (GTK_WIDGET (socket));

  _gtk_socket_add_window (socket, window_id, TRUE);
}

/**
 * __gtk_socket_get_id:
 * @socket_: a #GtkSocket.
 * 
 * Gets the window ID of a #GtkSocket widget, which can then
 * be used to create a client embedded inside the socket, for
 * instance with __gtk_plug_new(). 
 *
 * The #GtkSocket must have already be added into a toplevel window 
 * before you can make this call.
 * 
 * Return value: the window ID for the socket
 **/
GdkNativeWindow
__gtk_socket_get_id (GtkSocket *socket)
{
  g_return_val_if_fail (GTK_IS_SOCKET (socket), 0);
  g_return_val_if_fail (GTK_WIDGET_ANCHORED (socket), 0);

  if (!__gtk_widget_get_realized (GTK_WIDGET (socket)))
    __gtk_widget_realize (GTK_WIDGET (socket));

  return _gtk_socket_windowing_get_id (socket);
}

/**
 * __gtk_socket_get_plug_window:
 * @socket_: a #GtkSocket.
 *
 * Retrieves the window of the plug. Use this to check if the plug has
 * been created inside of the socket.
 *
 * Return value: (transfer none): the window of the plug if available, or %NULL
 *
 * Since:  2.14
 **/
GdkWindow*
__gtk_socket_get_plug_window (GtkSocket *socket)
{
  g_return_val_if_fail (GTK_IS_SOCKET (socket), NULL);

  return gtk_socket_get_props (socket)->plug_window;
}

static void
gtk_socket_realize (GtkWidget *widget)
{
  GtkSocket *socket = GTK_SOCKET (widget);
  GdkWindowAttr attributes;
  gint attributes_mask;

  __gtk_widget_set_realized (widget, TRUE);

  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.x = gtk_widget_get_props (widget)->allocation.x;
  attributes.y = gtk_widget_get_props (widget)->allocation.y;
  attributes.width = gtk_widget_get_props (widget)->allocation.width;
  attributes.height = gtk_widget_get_props (widget)->allocation.height;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = __gtk_widget_get_visual (widget);
  attributes.colormap = __gtk_widget_get_colormap (widget);
  attributes.event_mask = GDK_FOCUS_CHANGE_MASK;

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  gtk_widget_get_props (widget)->window = __gdk_window_new (__gtk_widget_get_parent_window (widget), 
				   &attributes, attributes_mask);
  __gdk_window_set_user_data (gtk_widget_get_props (widget)->window, socket);

  gtk_widget_get_props (widget)->style = __gtk_style_attach (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window);
  __gtk_style_set_background (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window, GTK_STATE_NORMAL);

  _gtk_socket_windowing_realize_window (socket);

  __gdk_window_add_filter (gtk_widget_get_props (widget)->window,
			 _gtk_socket_windowing_filter_func,
			 widget);

  /* We sync here so that we make sure that if the XID for
   * our window is passed to another application, SubstructureRedirectMask
   * will be set by the time the other app creates its window.
   */
  __gdk_display_sync (__gtk_widget_get_display (widget));
}

/**
 * _gtk_socket_end_embedding:
 *
 * @socket: a #GtkSocket
 *
 * Called to end the embedding of a plug in the socket.
 */
void
_gtk_socket_end_embedding (GtkSocket *socket)
{
  GtkSocketPrivate *private = gtk_socket_get_instance_private (socket);
  GtkWidget *toplevel = __gtk_widget_get_toplevel (GTK_WIDGET (socket));
  
  if (GTK_IS_WINDOW (toplevel))
    _gtk_socket_windowing_end_embedding_toplevel (socket);

  g_object_unref (gtk_socket_get_props (socket)->plug_window);
  gtk_socket_get_props (socket)->plug_window = NULL;
  gtk_socket_get_props (socket)->current_width = 0;
  gtk_socket_get_props (socket)->current_height = 0;
  private->resize_count = 0;

  __gtk_accel_group_disconnect (gtk_socket_get_props (socket)->accel_group, NULL);
}

static void
gtk_socket_unrealize (GtkWidget *widget)
{
  GtkSocket *socket = GTK_SOCKET (widget);

  __gtk_widget_set_realized (widget, FALSE);

  if (gtk_socket_get_props (socket)->plug_widget)
    {
      ___gtk_plug_remove_from_socket (GTK_PLUG (gtk_socket_get_props (socket)->plug_widget), gtk_socket_get_props (socket));
    }
  else if (gtk_socket_get_props (socket)->plug_window)
    {
      _gtk_socket_end_embedding (socket);
    }

  GTK_WIDGET_CLASS (gtk_socket_parent_class)->unrealize (widget);
}

static void
gtk_socket_size_request (GtkWidget      *widget,
			 GtkRequisition *requisition)
{
  GtkSocket *socket = GTK_SOCKET (widget);

  if (gtk_socket_get_props (socket)->plug_widget)
    {
      __gtk_widget_size_request (gtk_socket_get_props (socket)->plug_widget, requisition);
    }
  else
    {
      if (gtk_socket_get_props (socket)->is_mapped && !gtk_socket_get_props (socket)->have_size && gtk_socket_get_props (socket)->plug_window)
	_gtk_socket_windowing_size_request (socket);

      if (gtk_socket_get_props (socket)->is_mapped && gtk_socket_get_props (socket)->have_size)
	{
	  requisition->width = MAX (gtk_socket_get_props (socket)->request_width, 1);
	  requisition->height = MAX (gtk_socket_get_props (socket)->request_height, 1);
	}
      else
	{
	  requisition->width = 1;
	  requisition->height = 1;
	}
    }
}

static void
gtk_socket_size_allocate (GtkWidget     *widget,
			  GtkAllocation *allocation)
{
  GtkSocket *socket = GTK_SOCKET (widget);

  gtk_widget_get_props (widget)->allocation = *allocation;
  if (__gtk_widget_get_realized (widget))
    {
      __gdk_window_move_resize (gtk_widget_get_props (widget)->window,
			      allocation->x, allocation->y,
			      allocation->width, allocation->height);

      if (gtk_socket_get_props (socket)->plug_widget)
	{
	  GtkAllocation child_allocation;

	  child_allocation.x = 0;
	  child_allocation.y = 0;
	  child_allocation.width = allocation->width;
	  child_allocation.height = allocation->height;

	  __gtk_widget_size_allocate (gtk_socket_get_props (socket)->plug_widget, &child_allocation);
	}
      else if (gtk_socket_get_props (socket)->plug_window)
	{
	  GtkSocketPrivate *private = gtk_socket_get_instance_private (socket);
	  
	  __gdk_error_trap_push ();
	  
	  if (allocation->width != gtk_socket_get_props (socket)->current_width ||
	      allocation->height != gtk_socket_get_props (socket)->current_height)
	    {
	      __gdk_window_move_resize (gtk_socket_get_props (socket)->plug_window,
				      0, 0,
				      allocation->width, allocation->height);
	      if (private->resize_count)
		private->resize_count--;

	      gtk_socket_get_props (socket)->current_width = allocation->width;
	      gtk_socket_get_props (socket)->current_height = allocation->height;
	    }

	  if (gtk_socket_get_props (socket)->need_map)
	    {
	      __gdk_window_show (gtk_socket_get_props (socket)->plug_window);
	      gtk_socket_get_props (socket)->need_map = FALSE;
	    }

	  while (private->resize_count)
 	    {
 	      _gtk_socket_windowing_send_configure_event (socket);
 	      private->resize_count--;
 	    }
	  
	  __gdk_display_sync (__gtk_widget_get_display (widget));
	  __gdk_error_trap_pop ();
	}
    }
}

static gboolean
activate_key (GtkAccelGroup  *accel_group,
	      GObject        *acceleratable,
	      guint           accel_key,
	      GdkModifierType accel_mods,
	      GrabbedKey     *grabbed_key)
{
  GdkEvent *gdk_event = __gtk_get_current_event ();
  
  GtkSocket *socket = g_object_get_data (G_OBJECT (accel_group), "gtk-socket");
  gboolean retval = FALSE;

  if (gdk_event && gdk_event->type == GDK_KEY_PRESS && gtk_socket_get_props (socket)->plug_window)
    {
      _gtk_socket_windowing_send_key_event (socket, gdk_event, TRUE);
      retval = TRUE;
    }

  if (gdk_event)
    __gdk_event_free (gdk_event);

  return retval;
}

static gboolean
find_accel_key (GtkAccelKey *key,
		GClosure    *closure,
		gpointer     data)
{
  GrabbedKey *grabbed_key = data;
  
  return (key->accel_key == grabbed_key->accel_key &&
	  key->accel_mods == grabbed_key->accel_mods);
}

/**
 * _gtk_socket_add_grabbed_key:
 *
 * @socket: a #GtkSocket
 * @keyval: a key
 * @modifiers: modifiers for the key
 *
 * Called from the GtkSocket platform-specific backend when the
 * corresponding plug has told the socket to grab a key.
 */
void
_gtk_socket_add_grabbed_key (GtkSocket       *socket,
			     guint            keyval,
			     GdkModifierType  modifiers)
{
  GClosure *closure;
  GrabbedKey *grabbed_key;

  grabbed_key = g_new (GrabbedKey, 1);
  
  grabbed_key->accel_key = keyval;
  grabbed_key->accel_mods = modifiers;

  if (__gtk_accel_group_find (gtk_socket_get_props (socket)->accel_group,
			    find_accel_key,
			    &grabbed_key))
    {
      g_warning ("GtkSocket: request to add already present grabbed key %u,%#x\n",
		 keyval, modifiers);
      g_free (grabbed_key);
      return;
    }

  closure = g_cclosure_new (G_CALLBACK (activate_key), grabbed_key, (GClosureNotify)g_free);

  __gtk_accel_group_connect (gtk_socket_get_props (socket)->accel_group, keyval, modifiers, GTK_ACCEL_LOCKED,
			   closure);
}

/**
 * _gtk_socket_remove_grabbed_key:
 *
 * @socket: a #GtkSocket
 * @keyval: a key
 * @modifiers: modifiers for the key
 *
 * Called from the GtkSocket backend when the corresponding plug has
 * told the socket to remove a key grab.
 */
void
_gtk_socket_remove_grabbed_key (GtkSocket      *socket,
				guint           keyval,
				GdkModifierType modifiers)
{
  if (!__gtk_accel_group_disconnect_key (gtk_socket_get_props (socket)->accel_group, keyval, modifiers))
    g_warning ("GtkSocket: request to remove non-present grabbed key %u,%#x\n",
	       keyval, modifiers);
}

static void
socket_update_focus_in (GtkSocket *socket)
{
  gboolean focus_in = FALSE;

  if (gtk_socket_get_props (socket)->plug_window)
    {
      GtkWidget *toplevel = __gtk_widget_get_toplevel (GTK_WIDGET (socket));

      if (__gtk_widget_is_toplevel (toplevel) &&
	  gtk_window_get_props (GTK_WINDOW (toplevel))->has_toplevel_focus &&
	  __gtk_widget_is_focus (GTK_WIDGET (socket)))
	focus_in = TRUE;
    }

  if (focus_in != gtk_socket_get_props (socket)->focus_in)
    {
      gtk_socket_get_props (socket)->focus_in = focus_in;

      _gtk_socket_windowing_focus_change (socket, focus_in);
    }
}

static void
socket_update_active (GtkSocket *socket)
{
  gboolean active = FALSE;

  if (gtk_socket_get_props (socket)->plug_window)
    {
      GtkWidget *toplevel = __gtk_widget_get_toplevel (GTK_WIDGET (socket));

      if (__gtk_widget_is_toplevel (toplevel) &&
	  gtk_window_get_props (GTK_WINDOW (toplevel))->is_active)
	active = TRUE;
    }

  if (active != gtk_socket_get_props (socket)->active)
    {
      gtk_socket_get_props (socket)->active = active;

      _gtk_socket_windowing_update_active (socket, active);
    }
}

static void
gtk_socket_hierarchy_changed (GtkWidget *widget,
			      GtkWidget *old_toplevel)
{
  GtkSocket *socket = GTK_SOCKET (widget);
  GtkWidget *toplevel = __gtk_widget_get_toplevel (widget);

  if (toplevel && !GTK_IS_WINDOW (toplevel))
    toplevel = NULL;

  if (toplevel != gtk_socket_get_props (socket)->toplevel)
    {
      if (gtk_socket_get_props (socket)->toplevel)
	{
	  __gtk_window_remove_accel_group (GTK_WINDOW (gtk_socket_get_props (socket)->toplevel), gtk_socket_get_props (socket)->accel_group);
	  g_signal_handlers_disconnect_by_func (gtk_socket_get_props (socket)->toplevel,
						socket_update_focus_in,
						socket);
	  g_signal_handlers_disconnect_by_func (gtk_socket_get_props (socket)->toplevel,
						socket_update_active,
						socket);
	}

      gtk_socket_get_props (socket)->toplevel = toplevel;

      if (toplevel)
	{
	  __gtk_window_add_accel_group (GTK_WINDOW (gtk_socket_get_props (socket)->toplevel), gtk_socket_get_props (socket)->accel_group);
	  g_signal_connect_swapped (gtk_socket_get_props (socket)->toplevel, "notify::has-toplevel-focus",
				    G_CALLBACK (socket_update_focus_in), socket);
	  g_signal_connect_swapped (gtk_socket_get_props (socket)->toplevel, "notify::is-active",
				    G_CALLBACK (socket_update_active), socket);
	}

      socket_update_focus_in (socket);
      socket_update_active (socket);
    }
}

static void
gtk_socket_grab_notify (GtkWidget *widget,
			gboolean   was_grabbed)
{
  GtkSocket *socket = GTK_SOCKET (widget);

  if (!gtk_socket_get_props (socket)->same_app)
    _gtk_socket_windowing_update_modality (socket, !was_grabbed);
}

static gboolean
gtk_socket_key_event (GtkWidget   *widget,
                      GdkEventKey *event)
{
  GtkSocket *socket = GTK_SOCKET (widget);
  
  if (__gtk_widget_has_focus (widget) && gtk_socket_get_props (socket)->plug_window && !gtk_socket_get_props (socket)->plug_widget)
    {
      _gtk_socket_windowing_send_key_event (socket, (GdkEvent *) event, FALSE);

      return TRUE;
    }
  else
    return FALSE;
}

static void
gtk_socket_notify (GObject    *object,
		   GParamSpec *pspec)
{
  if (!strcmp (pspec->name, "is-focus"))
    return;
  socket_update_focus_in (GTK_SOCKET (object));
}

/**
 * _gtk_socket_claim_focus:
 *
 * @socket: a #GtkSocket
 * @send_event: huh?
 *
 * Claims focus for the socket. XXX send_event?
 */
void
_gtk_socket_claim_focus (GtkSocket *socket,
			 gboolean   send_event)
{
  GtkWidget *widget = GTK_WIDGET (socket);

  if (!send_event)
    gtk_socket_get_props (socket)->focus_in = TRUE;	/* Otherwise, our notify handler will send FOCUS_IN  */
      
  /* Oh, the trickery... */
  
  __gtk_widget_set_can_focus (widget, TRUE);
  __gtk_widget_grab_focus (widget);
  __gtk_widget_set_can_focus (widget, FALSE);
}

static gboolean
gtk_socket_focus (GtkWidget       *widget,
		  GtkDirectionType direction)
{
  GtkSocket *socket = GTK_SOCKET (widget);

  if (gtk_socket_get_props (socket)->plug_widget)
    return __gtk_widget_child_focus (gtk_socket_get_props (socket)->plug_widget, direction);

  if (!__gtk_widget_is_focus (widget))
    {
      _gtk_socket_windowing_focus (socket, direction);
      _gtk_socket_claim_focus (socket, FALSE);
 
      return TRUE;
    }
  else
    return FALSE;
}

static void
gtk_socket_remove (GtkContainer *container,
		   GtkWidget    *child)
{
  GtkSocket *socket = GTK_SOCKET (container);

  g_return_if_fail (child == gtk_socket_get_props (socket)->plug_widget);

  ___gtk_plug_remove_from_socket (GTK_PLUG (gtk_socket_get_props (socket)->plug_widget), gtk_socket_get_props (socket));
}

static void
gtk_socket_forall (GtkContainer *container,
		   gboolean      include_internals,
		   GtkCallback   callback,
		   gpointer      callback_data)
{
  GtkSocket *socket = GTK_SOCKET (container);

  if (gtk_socket_get_props (socket)->plug_widget)
    (* callback) (gtk_socket_get_props (socket)->plug_widget, callback_data);
}

/**
 * _gtk_socket_add_window:
 *
 * @socket: a #GtkSocket
 * @xid: the native identifier for a window
 * @need_reparent: whether the socket's plug's window needs to be
 *		   reparented to the socket
 *
 * Adds a window to a GtkSocket.
 */
void
_gtk_socket_add_window (GtkSocket       *socket,
			GdkNativeWindow  xid,
			gboolean         need_reparent)
{
  GtkWidget *widget = GTK_WIDGET (socket);
  GdkDisplay *display = __gtk_widget_get_display (widget);
  gpointer user_data = NULL;
  
  gtk_socket_get_props (socket)->plug_window = __gdk_window_lookup_for_display (display, xid);

  if (gtk_socket_get_props (socket)->plug_window)
    {
      g_object_ref (gtk_socket_get_props (socket)->plug_window);
      __gdk_window_get_user_data (gtk_socket_get_props (socket)->plug_window, &user_data);
    }

  if (user_data)		/* A widget's window in this process */
    {
      GtkWidget *child_widget = user_data;

      if (!GTK_IS_PLUG (child_widget))
	{
	  g_warning (G_STRLOC ": Can't add non-GtkPlug to GtkSocket");
	  gtk_socket_get_props (socket)->plug_window = NULL;
	  __gdk_error_trap_pop ();
	  
	  return;
	}

      ___gtk_plug_add_to_socket (GTK_PLUG (child_widget), socket);
    }
  else				/* A foreign window */
    {
      GtkWidget *toplevel;
      GdkDragProtocol protocol;

      __gdk_error_trap_push ();

      if (!gtk_socket_get_props (socket)->plug_window)
	{  
	  gtk_socket_get_props (socket)->plug_window = __gdk_window_foreign_new_for_display (display, xid);
	  if (!gtk_socket_get_props (socket)->plug_window) /* was deleted before we could get it */
	    {
	      __gdk_error_trap_pop ();
	      return;
	    }
	}
	
      _gtk_socket_windowing_select_plug_window_input (socket);

      if (__gdk_error_trap_pop ())
	{
	  g_object_unref (gtk_socket_get_props (socket)->plug_window);
	  gtk_socket_get_props (socket)->plug_window = NULL;
	  return;
	}
      
      /* OK, we now will reliably get destroy notification on gtk_socket_get_props (socket)->plug_window */

      __gdk_error_trap_push ();

      if (need_reparent)
	{
	  __gdk_window_hide (gtk_socket_get_props (socket)->plug_window); /* Shouldn't actually be necessary for XEMBED, but just in case */
	  __gdk_window_reparent (gtk_socket_get_props (socket)->plug_window, gtk_widget_get_props (widget)->window, 0, 0);
	}

      gtk_socket_get_props (socket)->have_size = FALSE;

      _gtk_socket_windowing_embed_get_info (socket);

      gtk_socket_get_props (socket)->need_map = gtk_socket_get_props (socket)->is_mapped;

      if (__gdk_drag_get_protocol_for_display (display, xid, &protocol))
	__gtk_drag_dest_set_proxy (GTK_WIDGET (socket), gtk_socket_get_props (socket)->plug_window, 
				 protocol, TRUE);

      __gdk_display_sync (display);
      __gdk_error_trap_pop ();

      __gdk_window_add_filter (gtk_socket_get_props (socket)->plug_window,
			     _gtk_socket_windowing_filter_func,
			     socket);

      /* Add a pointer to the socket on our toplevel window */

      toplevel = __gtk_widget_get_toplevel (GTK_WIDGET (socket));
      if (GTK_IS_WINDOW (toplevel))
	__gtk_window_add_embedded_xid (GTK_WINDOW (toplevel), xid);

      _gtk_socket_windowing_embed_notify (socket);

      socket_update_active (socket);
      socket_update_focus_in (socket);

      __gtk_widget_queue_resize (GTK_WIDGET (socket));
    }

  if (gtk_socket_get_props (socket)->plug_window)
    g_signal_emit (socket, socket_signals[PLUG_ADDED], 0);
}

/**
 * _gtk_socket_handle_map_request:
 *
 * @socket: a #GtkSocket
 *
 * Called from the GtkSocket backend when the plug has been mapped.
 */
void
_gtk_socket_handle_map_request (GtkSocket *socket)
{
  if (!gtk_socket_get_props (socket)->is_mapped)
    {
      gtk_socket_get_props (socket)->is_mapped = TRUE;
      gtk_socket_get_props (socket)->need_map = TRUE;

      __gtk_widget_queue_resize (GTK_WIDGET (socket));
    }
}

/**
 * _gtk_socket_unmap_notify:
 *
 * @socket: a #GtkSocket
 *
 * Called from the GtkSocket backend when the plug has been unmapped ???
 */
void
_gtk_socket_unmap_notify (GtkSocket *socket)
{
  if (gtk_socket_get_props (socket)->is_mapped)
    {
      gtk_socket_get_props (socket)->is_mapped = FALSE;
      __gtk_widget_queue_resize (GTK_WIDGET (socket));
    }
}

/**
 * _gtk_socket_advance_toplevel_focus:
 *
 * @socket: a #GtkSocket
 * @direction: a direction
 *
 * Called from the GtkSocket backend when the corresponding plug
 * has told the socket to move the focus.
 */
void
_gtk_socket_advance_toplevel_focus (GtkSocket        *socket,
				    GtkDirectionType  direction)
{
  GtkBin *bin;
  GtkWindow *window;
  GtkContainer *container;
  GtkWidget *toplevel;
  GtkWidget *old_focus_child;
  GtkWidget *parent;

  toplevel = __gtk_widget_get_toplevel (GTK_WIDGET (socket));
  if (!toplevel)
    return;

  if (!__gtk_widget_is_toplevel (toplevel) || GTK_IS_PLUG (toplevel))
    {
      __gtk_widget_child_focus (toplevel,direction);
      return;
    }

  container = GTK_CONTAINER (toplevel);
  window = GTK_WINDOW (toplevel);
  bin = GTK_BIN (toplevel);

  /* This is a copy of gtk_window_focus(), modified so that we
   * can detect wrap-around.
   */
  old_focus_child = gtk_container_get_props (container)->focus_child;
  
  if (old_focus_child)
    {
      if (__gtk_widget_child_focus (old_focus_child, direction))
	return;

      /* We are allowed exactly one wrap-around per sequence of focus
       * events
       */
      if (_gtk_socket_windowing_embed_get_focus_wrapped ())
	return;
      else
	_gtk_socket_windowing_embed_set_focus_wrapped ();
    }

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

  /* Now try to focus the first widget in the window */
  if (gtk_bin_get_props (bin)->child)
    {
      if (__gtk_widget_child_focus (gtk_bin_get_props (bin)->child, direction))
        return;
    }
}
