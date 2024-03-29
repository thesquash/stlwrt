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



#include <gtkmain.h>

#include <gtkplug.h>
#include <gtkprivate.h>
#include <gtkplugprivate.h>

#include "gdkx.h"

#include <gtkxembed.h>


static void xembed_set_info            (GdkWindow     *window,
					unsigned long  flags);

GdkNativeWindow
_gtk_plug_windowing_get_id (GtkPlug *plug)
{
  return GDK_WINDOW_XWINDOW (gtk_widget_get_props (plug)->window);
}

void
_gtk_plug_windowing_realize_toplevel (GtkPlug *plug)
{
  xembed_set_info (gtk_widget_get_props (plug)->window, 0);
}

void
_gtk_plug_windowing_map_toplevel (GtkPlug *plug)
{
  xembed_set_info (gtk_widget_get_props (plug)->window, XEMBED_MAPPED);
}

void
_gtk_plug_windowing_unmap_toplevel (GtkPlug *plug)
{
  xembed_set_info (gtk_widget_get_props (plug)->window, 0);
}

void
_gtk_plug_windowing_set_focus (GtkPlug *plug)
{
  _gtk_xembed_send_message (gtk_plug_get_props (plug)->socket_window,
			    XEMBED_REQUEST_FOCUS, 0, 0, 0);
}

void
_gtk_plug_windowing_add_grabbed_key (GtkPlug        *plug,
				     guint           accelerator_key,
				     GdkModifierType accelerator_mods)
{
  _gtk_xembed_send_message (gtk_plug_get_props (plug)->socket_window, XEMBED_GTK_GRAB_KEY, 0, 
			    accelerator_key, accelerator_mods);
}

void
_gtk_plug_windowing_remove_grabbed_key (GtkPlug        *plug,
					guint           accelerator_key,
					GdkModifierType accelerator_mods)
{
  _gtk_xembed_send_message (gtk_plug_get_props (plug)->socket_window, XEMBED_GTK_UNGRAB_KEY, 0, 
			    accelerator_key, accelerator_mods);
}

void
_gtk_plug_windowing_focus_to_parent (GtkPlug         *plug,
				     GtkDirectionType direction)
{
  XEmbedMessageType message = XEMBED_FOCUS_PREV; /* Quiet GCC */
  
  switch (direction)
    {
    case GTK_DIR_UP:
    case GTK_DIR_LEFT:
    case GTK_DIR_TAB_BACKWARD:
      message = XEMBED_FOCUS_PREV;
      break;
    case GTK_DIR_DOWN:
    case GTK_DIR_RIGHT:
    case GTK_DIR_TAB_FORWARD:
      message = XEMBED_FOCUS_NEXT;
      break;
    }
  
  _gtk_xembed_send_focus_message (gtk_plug_get_props (plug)->socket_window, message, 0);
}

static void
xembed_set_info (GdkWindow     *window,
		 unsigned long  flags)
{
  GdkDisplay *display = __gdk_window_get_display (window);
  unsigned long buffer[2];

  Atom xembed_info_atom = __gdk_x11_get_xatom_by_name_for_display (display, "_XEMBED_INFO");

  buffer[0] = GTK_XEMBED_PROTOCOL_VERSION;
  buffer[1] = flags;

  XChangeProperty (GDK_DISPLAY_XDISPLAY (display),
		   GDK_WINDOW_XWINDOW (window),
		   xembed_info_atom, xembed_info_atom, 32,
		   PropModeReplace,
		   (unsigned char *)buffer, 2);
}

static void
handle_xembed_message (GtkPlug           *plug,
		       XEmbedMessageType  message,
		       glong              detail,
		       glong              data1,
		       glong              data2,
		       guint32            time)
{
  GtkWindow *window = GTK_WINDOW (plug);
  
  switch (message)
    {
    case XEMBED_EMBEDDED_NOTIFY:
      break;
    case XEMBED_WINDOW_ACTIVATE:
      ___gtk_window_set_is_active (window, TRUE);
      break;
    case XEMBED_WINDOW_DEACTIVATE:
      ___gtk_window_set_is_active (window, FALSE);
      break;
      
    case XEMBED_MODALITY_ON:
      _gtk_plug_handle_modality_on (plug);
      break;
    case XEMBED_MODALITY_OFF:
      _gtk_plug_handle_modality_off (plug);
      break;

    case XEMBED_FOCUS_IN:
      ___gtk_window_set_has_toplevel_focus (window, TRUE);
      switch (detail)
	{
	case XEMBED_FOCUS_FIRST:
	  _gtk_plug_focus_first_last (plug, GTK_DIR_TAB_FORWARD);
	  break;
	case XEMBED_FOCUS_LAST:
	  _gtk_plug_focus_first_last (plug, GTK_DIR_TAB_BACKWARD);
	  break;
	case XEMBED_FOCUS_CURRENT:
	  break;
	}
      break;

    case XEMBED_FOCUS_OUT:
      ___gtk_window_set_has_toplevel_focus (window, FALSE);
      break;
      
    case XEMBED_GRAB_KEY:
    case XEMBED_UNGRAB_KEY:
    case XEMBED_GTK_GRAB_KEY:
    case XEMBED_GTK_UNGRAB_KEY:
    case XEMBED_REQUEST_FOCUS:
    case XEMBED_FOCUS_NEXT:
    case XEMBED_FOCUS_PREV:
      g_warning ("GtkPlug: Invalid _XEMBED message %s received", _gtk_xembed_message_name (message));
      break;
    }
}

GdkFilterReturn
_gtk_plug_windowing_filter_func (GdkXEvent *gdk_xevent,
				 GdkEvent  *event,
				 gpointer   data)
{
  GdkScreen *screen = __gdk_window_get_screen (event->any.window);
  GdkDisplay *display = __gdk_screen_get_display (screen);
  GtkPlug *plug = GTK_PLUG (data);
  XEvent *xevent = (XEvent *)gdk_xevent;

  GdkFilterReturn return_val;
  
  return_val = GDK_FILTER_CONTINUE;

  switch (xevent->type)
    {
    case ClientMessage:
      if (xevent->xclient.message_type == __gdk_x11_get_xatom_by_name_for_display (display, "_XEMBED"))
	{
	  _gtk_xembed_push_message (xevent);
	  handle_xembed_message (plug,
				 xevent->xclient.data.l[1],
				 xevent->xclient.data.l[2],
				 xevent->xclient.data.l[3],
				 xevent->xclient.data.l[4],
				 xevent->xclient.data.l[0]);
	  _gtk_xembed_pop_message ();

	  return_val = GDK_FILTER_REMOVE;
	}
      else if (xevent->xclient.message_type == __gdk_x11_get_xatom_by_name_for_display (display, "WM_DELETE_WINDOW"))
	{
	  /* We filter these out because we take being reparented back to the
	   * root window as the reliable end of the embedding protocol
	   */

	  return_val = GDK_FILTER_REMOVE;
	}
      break;
    case ReparentNotify:
      {
	XReparentEvent *xre = &xevent->xreparent;
	gboolean was_embedded = gtk_plug_get_props (plug)->socket_window != NULL;

	return_val = GDK_FILTER_REMOVE;
	
	g_object_ref (plug);
	
	if (was_embedded)
	  {
	    /* End of embedding protocol for previous socket */
	    /* FIXME: race if we remove from another socket and
	     * then add to a local window before we get notification
	     * Probably need check in ___gtk_plug_add_to_socket
	     */
	    
	    if (xre->parent != GDK_WINDOW_XWINDOW (gtk_plug_get_props (plug)->socket_window))
	      {
		GtkWidget *widget = GTK_WIDGET (plug);

		__gdk_window_set_user_data (gtk_plug_get_props (plug)->socket_window, NULL);
		g_object_unref (gtk_plug_get_props (plug)->socket_window);
		gtk_plug_get_props (plug)->socket_window = NULL;

		/* Emit a delete window, as if the user attempted
		 * to close the toplevel. Simple as to how we
		 * handle WM_DELETE_WINDOW, if it isn't handled
		 * we destroy the widget. BUt only do this if
		 * we are being reparented to the root window.
		 * Moving from one embedder to another should
		 * be invisible to the app.
		 */

		if (xre->parent == GDK_WINDOW_XWINDOW (__gdk_screen_get_root_window (screen)))
		  {
		    _gtk_plug_send_delete_event (widget);

		    g_object_notify (G_OBJECT (plug), "embedded");
		  }
	      }
	    else
	      goto done;
	  }

	if (xre->parent != GDK_WINDOW_XWINDOW (__gdk_screen_get_root_window (screen)))
	  {
	    /* Start of embedding protocol */

	    gtk_plug_get_props (plug)->socket_window = __gdk_window_lookup_for_display (display, xre->parent);
	    if (gtk_plug_get_props (plug)->socket_window)
	      {
		gpointer user_data = NULL;
		__gdk_window_get_user_data (gtk_plug_get_props (plug)->socket_window, &user_data);

		if (user_data)
		  {
		    g_warning (G_STRLOC "Plug reparented unexpectedly into window in the same process");
		    gtk_plug_get_props (plug)->socket_window = NULL;
		    break; /* FIXME: shouldn't this unref the plug? i.e. "goto done;" instead */
		  }

		g_object_ref (gtk_plug_get_props (plug)->socket_window);
	      }
	    else
	      {
		gtk_plug_get_props (plug)->socket_window = __gdk_window_foreign_new_for_display (display, xre->parent);
		if (!gtk_plug_get_props (plug)->socket_window) /* Already gone */
		  break; /* FIXME: shouldn't this unref the plug? i.e. "goto done;" instead */
	      }

	    _gtk_plug_add_all_grabbed_keys (plug);

	    if (!was_embedded)
	      g_signal_emit_by_name (plug, "embedded");

	    g_object_notify (G_OBJECT (plug), "embedded");
	  }

      done:
	g_object_unref (plug);
	
	break;
      }
    }

  return return_val;
}
