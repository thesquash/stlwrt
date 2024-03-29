/* gtktrayicon.c
 * Copyright (C) 2002 Anders Carlsson <andersca@gnu.org>
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
 * This is an implementation of the freedesktop.org "system tray" spec,
 * http://www.freedesktop.org/wiki/Standards/systemtray-spec
 */

#include "config.h"

#include <stlwrt.h>
#include <string.h>

#include <gtkintl.h>
#include <gtkprivate.h>
#include <gtktrayicon.h>



#include "gdkx.h"
#include <X11/Xatom.h>

#define SYSTEM_TRAY_REQUEST_DOCK    0
#define SYSTEM_TRAY_BEGIN_MESSAGE   1
#define SYSTEM_TRAY_CANCEL_MESSAGE  2

#define SYSTEM_TRAY_ORIENTATION_HORZ 0
#define SYSTEM_TRAY_ORIENTATION_VERT 1

enum {
  PROP_0,
  PROP_ORIENTATION
};

struct _GtkTrayIconPrivate
{
  guint stamp;
  
  Atom selection_atom;
  Atom manager_atom;
  Atom system_tray_opcode_atom;
  Atom orientation_atom;
  Atom visual_atom;
  Window manager_window;
  GdkVisual *manager_visual;
  gboolean manager_visual_rgba;

  GtkOrientation orientation;
};

static void gtk_tray_icon_constructed   (GObject     *object);
static void gtk_tray_icon_dispose       (GObject     *object);

static void gtk_tray_icon_get_property  (GObject     *object,
				 	 guint        prop_id,
					 GValue      *value,
					 GParamSpec  *pspec);

static void     gtk_tray_icon_realize   (GtkWidget   *widget);
static void     gtk_tray_icon_style_set (GtkWidget   *widget,
					 GtkStyle    *previous_style);
static gboolean gtk_tray_icon_delete    (GtkWidget   *widget,
					 GdkEventAny *event);
static gboolean gtk_tray_icon_expose    (GtkWidget      *widget, 
					 GdkEventExpose *event);

static void gtk_tray_icon_clear_manager_window     (GtkTrayIcon *icon);
static void gtk_tray_icon_update_manager_window    (GtkTrayIcon *icon);
static void gtk_tray_icon_manager_window_destroyed (GtkTrayIcon *icon);

static GdkFilterReturn gtk_tray_icon_manager_filter (GdkXEvent *xevent,
						     GdkEvent  *event,
						     gpointer   user_data);


STLWRT_DEFINE_FTYPE (GtkTrayIcon, gtk_tray_icon, GTK_TYPE_PLUG,
                     G_TYPE_FLAG_NONE, G_ADD_PRIVATE (GtkTrayIcon))

static void
gtk_tray_icon_class_init (GtkTrayIconClass *class)
{
  GObjectClass *gobject_class = (GObjectClass *)class;
  GtkWidgetClass *widget_class = (GtkWidgetClass *)class;

  gobject_class->get_property = gtk_tray_icon_get_property;
  gobject_class->constructed = gtk_tray_icon_constructed;
  gobject_class->dispose = gtk_tray_icon_dispose;

  widget_class->realize = gtk_tray_icon_realize;
  widget_class->style_set = gtk_tray_icon_style_set;
  widget_class->delete_event = gtk_tray_icon_delete;
  widget_class->expose_event = gtk_tray_icon_expose;

  g_object_class_install_property (gobject_class,
				   PROP_ORIENTATION,
				   g_param_spec_enum ("orientation",
						      P_("Orientation"),
						      P_("The orientation of the tray"),
						      GTK_TYPE_ORIENTATION,
						      GTK_ORIENTATION_HORIZONTAL,
						      GTK_PARAM_READABLE));
}

static void
gtk_tray_icon_init (GtkTrayIcon *icon)
{
  gtk_tray_icon_get_props (icon)->priv = gtk_tray_icon_get_instance_private (icon);
  
  gtk_tray_icon_get_props (icon)->priv->stamp = 1;
  gtk_tray_icon_get_props (icon)->priv->orientation = GTK_ORIENTATION_HORIZONTAL;

  __gtk_widget_set_app_paintable (GTK_WIDGET (icon), TRUE);
  __gtk_widget_add_events (GTK_WIDGET (icon), GDK_PROPERTY_CHANGE_MASK);
}

static void
gtk_tray_icon_constructed (GObject *object)
{
  /* Do setup that depends on the screen; screen has been set at this point */

  GtkTrayIcon *icon = GTK_TRAY_ICON (object);
  GdkScreen *screen = __gtk_widget_get_screen (GTK_WIDGET (object));
  GdkWindow *root_window = __gdk_screen_get_root_window (screen);
  GdkDisplay *display = __gtk_widget_get_display (GTK_WIDGET (object));
  Display *xdisplay = __gdk_x11_display_get_xdisplay (display);
  char buffer[256];
  
  g_snprintf (buffer, sizeof (buffer),
	      "_NET_SYSTEM_TRAY_S%d",
	      __gdk_screen_get_number (screen));

  gtk_tray_icon_get_props (icon)->priv->selection_atom = XInternAtom (xdisplay, buffer, False);
  
  gtk_tray_icon_get_props (icon)->priv->manager_atom = XInternAtom (xdisplay, "MANAGER", False);
  
  gtk_tray_icon_get_props (icon)->priv->system_tray_opcode_atom = XInternAtom (xdisplay,
						     "_NET_SYSTEM_TRAY_OPCODE",
						     False);

  gtk_tray_icon_get_props (icon)->priv->orientation_atom = XInternAtom (xdisplay,
					      "_NET_SYSTEM_TRAY_ORIENTATION",
					      False);

  gtk_tray_icon_get_props (icon)->priv->visual_atom = XInternAtom (xdisplay,
					 "_NET_SYSTEM_TRAY_VISUAL",
					 False);

  /* Add a root window filter so that we get changes on MANAGER */
  __gdk_window_add_filter (root_window,
			 gtk_tray_icon_manager_filter, icon);

  gtk_tray_icon_update_manager_window (icon);
}

static void
gtk_tray_icon_clear_manager_window (GtkTrayIcon *icon)
{
  GdkDisplay *display = __gtk_widget_get_display (GTK_WIDGET (icon));

  if (gtk_tray_icon_get_props (icon)->priv->manager_window != None)
    {
      GdkWindow *gdkwin;

      gdkwin = __gdk_window_lookup_for_display (display,
                                              gtk_tray_icon_get_props (icon)->priv->manager_window);

      __gdk_window_remove_filter (gdkwin, gtk_tray_icon_manager_filter, icon);

      gtk_tray_icon_get_props (icon)->priv->manager_window = None;
      gtk_tray_icon_get_props (icon)->priv->manager_visual = NULL;
    }
}

static void
gtk_tray_icon_dispose (GObject *object)
{
  GtkTrayIcon *icon = GTK_TRAY_ICON (object);
  GtkWidget *widget = GTK_WIDGET (object);
  GdkWindow *root_window = __gdk_screen_get_root_window (__gtk_widget_get_screen (widget));

  gtk_tray_icon_clear_manager_window (icon);

  __gdk_window_remove_filter (root_window, gtk_tray_icon_manager_filter, icon);

  G_OBJECT_CLASS (gtk_tray_icon_parent_class)->dispose (object);
}

static void
gtk_tray_icon_get_property (GObject    *object,
			    guint       prop_id,
			    GValue     *value,
			    GParamSpec *pspec)
{
  GtkTrayIcon *icon = GTK_TRAY_ICON (object);

  switch (prop_id)
    {
    case PROP_ORIENTATION:
      g_value_set_enum (value, gtk_tray_icon_get_props (icon)->priv->orientation);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static gboolean
gtk_tray_icon_expose (GtkWidget      *widget, 
		      GdkEventExpose *event)
{
  GtkTrayIcon *icon = GTK_TRAY_ICON (widget);
  GtkWidget *focus_child;
  gint border_width, x, y, width, height;
  gboolean retval = FALSE;

  if (gtk_tray_icon_get_props (icon)->priv->manager_visual_rgba)
    {
      /* Clear to transparent */
      cairo_t *cr = __gdk_cairo_create (gtk_widget_get_props (widget)->window);
      cairo_set_source_rgba (cr, 0, 0, 0, 0);
      cairo_set_operator (cr, CAIRO_OPERATOR_SOURCE);
      __gdk_cairo_region (cr, event->region);
      cairo_fill (cr);
      cairo_destroy (cr);
    }
  else
    {
      /* Clear to parent-relative pixmap */
      __gdk_window_clear_area (gtk_widget_get_props (widget)->window, event->area.x, event->area.y,
			     event->area.width, event->area.height);
    }

  if (GTK_WIDGET_CLASS (gtk_tray_icon_parent_class)->expose_event)
    retval = GTK_WIDGET_CLASS (gtk_tray_icon_parent_class)->expose_event (widget, event);

  focus_child = gtk_container_get_props (widget)->focus_child;
  if (focus_child && __gtk_widget_has_focus (focus_child))
    {
      border_width = gtk_container_get_props (widget)->border_width;

      x = gtk_widget_get_props (widget)->allocation.x + border_width;
      y = gtk_widget_get_props (widget)->allocation.y + border_width;

      width  = gtk_widget_get_props (widget)->allocation.width  - 2 * border_width;
      height = gtk_widget_get_props (widget)->allocation.height - 2 * border_width;

      __gtk_paint_focus (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window,
                       __gtk_widget_get_state (widget),
                       &event->area, widget, "tray_icon",
                       x, y, width, height);
    }

  return retval;
}

static void
gtk_tray_icon_get_orientation_property (GtkTrayIcon *icon)
{
  GdkScreen *screen = __gtk_widget_get_screen (GTK_WIDGET (icon));
  GdkDisplay *display = __gdk_screen_get_display (screen);
  Display *xdisplay = GDK_DISPLAY_XDISPLAY (display);

  Atom type;
  int format;
  union {
	gulong *prop;
	guchar *prop_ch;
  } prop = { NULL };
  gulong nitems;
  gulong bytes_after;
  int error, result;

  g_assert (gtk_tray_icon_get_props (icon)->priv->manager_window != None);
  
  __gdk_error_trap_push ();
  type = None;
  result = XGetWindowProperty (xdisplay,
			       gtk_tray_icon_get_props (icon)->priv->manager_window,
			       gtk_tray_icon_get_props (icon)->priv->orientation_atom,
			       0, G_MAXLONG, FALSE,
			       XA_CARDINAL,
			       &type, &format, &nitems,
			       &bytes_after, &(prop.prop_ch));
  error = __gdk_error_trap_pop ();

  if (error || result != Success)
    return;

  if (type == XA_CARDINAL && nitems == 1 && format == 32)
    {
      GtkOrientation orientation;

      orientation = (prop.prop [0] == SYSTEM_TRAY_ORIENTATION_HORZ) ?
					GTK_ORIENTATION_HORIZONTAL :
					GTK_ORIENTATION_VERTICAL;

      if (gtk_tray_icon_get_props (icon)->priv->orientation != orientation)
	{
	  gtk_tray_icon_get_props (icon)->priv->orientation = orientation;

	  g_object_notify (G_OBJECT (icon), "orientation");
	}
    }

  if (type != None)
    XFree (prop.prop);
}

static void
gtk_tray_icon_get_visual_property (GtkTrayIcon *icon)
{
  GdkScreen *screen = __gtk_widget_get_screen (GTK_WIDGET (icon));
  GdkDisplay *display = __gdk_screen_get_display (screen);
  Display *xdisplay = GDK_DISPLAY_XDISPLAY (display);

  Atom type;
  int format;
  union {
	gulong *prop;
	guchar *prop_ch;
  } prop = { NULL };
  gulong nitems;
  gulong bytes_after;
  int error, result;

  g_assert (gtk_tray_icon_get_props (icon)->priv->manager_window != None);

  __gdk_error_trap_push ();
  type = None;
  result = XGetWindowProperty (xdisplay,
			       gtk_tray_icon_get_props (icon)->priv->manager_window,
			       gtk_tray_icon_get_props (icon)->priv->visual_atom,
			       0, G_MAXLONG, FALSE,
			       XA_VISUALID,
			       &type, &format, &nitems,
			       &bytes_after, &(prop.prop_ch));
  error = __gdk_error_trap_pop ();

  if (!error && result == Success &&
      type == XA_VISUALID && nitems == 1 && format == 32)
    {
      VisualID visual_id;
      GdkVisual *visual;

      visual_id = prop.prop[0];
      visual = __gdk_x11_screen_lookup_visual (screen, visual_id);

      gtk_tray_icon_get_props (icon)->priv->manager_visual = visual;
      gtk_tray_icon_get_props (icon)->priv->manager_visual_rgba = visual != NULL &&
        (visual->red_prec + visual->blue_prec + visual->green_prec < visual->depth);
    }
  else
    {
      gtk_tray_icon_get_props (icon)->priv->manager_visual = NULL;
      gtk_tray_icon_get_props (icon)->priv->manager_visual_rgba = FALSE;
    }


  /* For the background-relative hack we use when we aren't using a real RGBA
   * visual, we can't be double-buffered */
  __gtk_widget_set_double_buffered (GTK_WIDGET (icon), gtk_tray_icon_get_props (icon)->priv->manager_visual_rgba);

  if (type != None)
    XFree (prop.prop);
}

static GdkFilterReturn
gtk_tray_icon_manager_filter (GdkXEvent *xevent, 
			      GdkEvent  *event, 
			      gpointer   user_data)
{
  GtkTrayIcon *icon = user_data;
  XEvent *xev = (XEvent *)xevent;

  if (xev->xany.type == ClientMessage &&
      xev->xclient.message_type == gtk_tray_icon_get_props (icon)->priv->manager_atom &&
      xev->xclient.data.l[1] == gtk_tray_icon_get_props (icon)->priv->selection_atom)
    {
      gtk_tray_icon_update_manager_window (icon);
    }
  else if (xev->xany.window == gtk_tray_icon_get_props (icon)->priv->manager_window)
    {
      if (xev->xany.type == PropertyNotify &&
	  xev->xproperty.atom == gtk_tray_icon_get_props (icon)->priv->orientation_atom)
	{
	  gtk_tray_icon_get_orientation_property (icon);
	}
      else if (xev->xany.type == DestroyNotify)
	{
	  gtk_tray_icon_manager_window_destroyed (icon);
	}
    }
  
  return GDK_FILTER_CONTINUE;
}

static void
gtk_tray_icon_send_manager_message (GtkTrayIcon *icon,
				    long         message,
				    Window       window,
				    long         data1,
				    long         data2,
				    long         data3)
{
  XClientMessageEvent ev;
  Display *display;
  
  memset (&ev, 0, sizeof (ev));
  ev.type = ClientMessage;
  ev.window = window;
  ev.message_type = gtk_tray_icon_get_props (icon)->priv->system_tray_opcode_atom;
  ev.format = 32;
  ev.data.l[0] = __gdk_x11_get_server_time (gtk_widget_get_props (icon)->window);
  ev.data.l[1] = message;
  ev.data.l[2] = data1;
  ev.data.l[3] = data2;
  ev.data.l[4] = data3;

  display = GDK_DISPLAY_XDISPLAY (__gtk_widget_get_display (GTK_WIDGET (icon)));
  
  __gdk_error_trap_push ();
  XSendEvent (display,
	      gtk_tray_icon_get_props (icon)->priv->manager_window, False, NoEventMask, (XEvent *)&ev);
  __gdk_display_sync (__gtk_widget_get_display (GTK_WIDGET (icon)));
  __gdk_error_trap_pop ();
}

static void
gtk_tray_icon_send_dock_request (GtkTrayIcon *icon)
{
  gtk_tray_icon_send_manager_message (icon,
				      SYSTEM_TRAY_REQUEST_DOCK,
				      gtk_tray_icon_get_props (icon)->priv->manager_window,
				      __gtk_plug_get_id (GTK_PLUG (icon)),
				      0, 0);
}

static void
gtk_tray_icon_update_manager_window (GtkTrayIcon *icon)
{
  GtkWidget *widget = GTK_WIDGET (icon);
  GdkScreen *screen = __gtk_widget_get_screen (widget);
  GdkDisplay *display = __gdk_screen_get_display (screen);
  Display *xdisplay = GDK_DISPLAY_XDISPLAY (display);

  if (gtk_tray_icon_get_props (icon)->priv->manager_window != None)
    return;

  XGrabServer (xdisplay);
  
  gtk_tray_icon_get_props (icon)->priv->manager_window = XGetSelectionOwner (xdisplay,
						   gtk_tray_icon_get_props (icon)->priv->selection_atom);

  if (gtk_tray_icon_get_props (icon)->priv->manager_window != None)
    XSelectInput (xdisplay,
		  gtk_tray_icon_get_props (icon)->priv->manager_window, StructureNotifyMask|PropertyChangeMask);

  XUngrabServer (xdisplay);
  XFlush (xdisplay);
  
  if (gtk_tray_icon_get_props (icon)->priv->manager_window != None)
    {
      GdkWindow *gdkwin;

      gdkwin = __gdk_window_lookup_for_display (display,
					      gtk_tray_icon_get_props (icon)->priv->manager_window);
      
      __gdk_window_add_filter (gdkwin, gtk_tray_icon_manager_filter, icon);

      gtk_tray_icon_get_orientation_property (icon);
      gtk_tray_icon_get_visual_property (icon);

      if (__gtk_widget_get_realized (GTK_WIDGET (icon)))
	{
	  if ((gtk_tray_icon_get_props (icon)->priv->manager_visual == NULL &&
	       __gtk_widget_get_visual (widget) == __gdk_screen_get_system_visual (screen)) ||
	      (gtk_tray_icon_get_props (icon)->priv->manager_visual == __gtk_widget_get_visual (widget)))
	    {
	      /* Already have the right visual, can just dock
	       */
	      gtk_tray_icon_send_dock_request (icon);
	    }
	  else
	    {
	      /* Need to re-realize the widget to get the right visual
	       */
	      __gtk_widget_hide (widget);
	      __gtk_widget_unrealize (widget);
	      __gtk_widget_show (widget);
	    }
	}
    }
}

static void
gtk_tray_icon_manager_window_destroyed (GtkTrayIcon *icon)
{
  g_return_if_fail (gtk_tray_icon_get_props (icon)->priv->manager_window != None);

  gtk_tray_icon_clear_manager_window (icon);
}

static gboolean
gtk_tray_icon_delete (GtkWidget   *widget,
		      GdkEventAny *event)
{
#ifdef G_ENABLE_DEBUG
  GtkTrayIcon *icon = GTK_TRAY_ICON (widget);
#endif

  /* A bug in X server versions up to x.org 1.5.0 means that:
   * XFixesChangeSaveSet(...., SaveSetRoot, SaveSetUnmap) doesn't work properly
   * and we'll left mapped in a separate toplevel window if the tray is destroyed.
   * For simplicity just get rid of our X window and start over.
   */
  __gtk_widget_hide (widget);
  __gtk_widget_unrealize (widget);
  __gtk_widget_show (widget);

  /* Handled it, don't destroy the tray icon */
  return TRUE;
}

static void
gtk_tray_icon_set_colormap (GtkTrayIcon *icon)
{
  GdkScreen *screen = __gtk_widget_get_screen (GTK_WIDGET (icon));
  GdkColormap *colormap;
  GdkVisual *visual = gtk_tray_icon_get_props (icon)->priv->manager_visual;
  gboolean new_colormap = FALSE;

  /* To avoid uncertainty about colormaps, _NET_SYSTEM_TRAY_VISUAL is supposed
   * to be either the screen default visual or a TrueColor visual; ignore it
   * if it is something else
   */
  if (visual && visual->type != GDK_VISUAL_TRUE_COLOR)
    visual = NULL;

  if (visual == NULL || visual == __gdk_screen_get_system_visual (screen))
    colormap = __gdk_screen_get_system_colormap (screen);
  else if (visual == __gdk_screen_get_rgb_visual (screen))
    colormap = __gdk_screen_get_rgb_colormap (screen);
  else if (visual == __gdk_screen_get_rgba_visual (screen))
    colormap = __gdk_screen_get_rgba_colormap (screen);
  else
    {
      colormap = __gdk_colormap_new (visual, FALSE);
      new_colormap = TRUE;
    }

  __gtk_widget_set_colormap (GTK_WIDGET (icon), colormap);

  if (new_colormap)
    g_object_unref (colormap);
}

static void
gtk_tray_icon_realize (GtkWidget *widget)
{
  GtkTrayIcon *icon = GTK_TRAY_ICON (widget);

  /* Set our colormap before realizing */
  gtk_tray_icon_set_colormap (icon);

  GTK_WIDGET_CLASS (gtk_tray_icon_parent_class)->realize (widget);
  if (gtk_tray_icon_get_props (icon)->priv->manager_visual_rgba)
    {
      /* Set a transparent background */
      GdkColor transparent = { 0, 0, 0, 0 }; /* Only pixel=0 matters */
      __gdk_window_set_background (gtk_widget_get_props (widget)->window, &transparent);
    }
  else
    {
      /* Set a parent-relative background pixmap */
      __gdk_window_set_back_pixmap (gtk_widget_get_props (widget)->window, NULL, TRUE);
    }

  if (gtk_tray_icon_get_props (icon)->priv->manager_window != None)
    gtk_tray_icon_send_dock_request (icon);
}

static void
gtk_tray_icon_style_set (GtkWidget   *widget,
			 GtkStyle    *previous_style)
{
  /* The default handler resets the background according to the style. We either
   * use a transparent background or a parent-relative background and ignore the
   * style background. So, just don't chain up.
   */
}

guint
_gtk_tray_icon_send_message (GtkTrayIcon *icon,
			     gint         timeout,
			     const gchar *message,
			     gint         len)
{
  guint stamp;
  Display *xdisplay;
 
  g_return_val_if_fail (GTK_IS_TRAY_ICON (icon), 0);
  g_return_val_if_fail (timeout >= 0, 0);
  g_return_val_if_fail (message != NULL, 0);

  if (gtk_tray_icon_get_props (icon)->priv->manager_window == None)
    return 0;

  if (len < 0)
    len = strlen (message);

  stamp = gtk_tray_icon_get_props (icon)->priv->stamp++;
  
  /* Get ready to send the message */
  gtk_tray_icon_send_manager_message (icon, SYSTEM_TRAY_BEGIN_MESSAGE,
				      (Window)__gtk_plug_get_id (GTK_PLUG (icon)),
				      timeout, len, stamp);

  /* Now to send the actual message */
  xdisplay = GDK_DISPLAY_XDISPLAY (__gtk_widget_get_display (GTK_WIDGET (icon)));
  __gdk_error_trap_push ();
  while (len > 0)
    {
      XClientMessageEvent ev;

      memset (&ev, 0, sizeof (ev));
      ev.type = ClientMessage;
      ev.window = (Window)__gtk_plug_get_id (GTK_PLUG (icon));
      ev.format = 8;
      ev.message_type = XInternAtom (xdisplay,
				     "_NET_SYSTEM_TRAY_MESSAGE_DATA", False);
      if (len > 20)
	{
	  memcpy (&ev.data, message, 20);
	  len -= 20;
	  message += 20;
	}
      else
	{
	  memcpy (&ev.data, message, len);
	  len = 0;
	}

      XSendEvent (xdisplay,
		  gtk_tray_icon_get_props (icon)->priv->manager_window, False,
		  StructureNotifyMask, (XEvent *)&ev);
    }
  __gdk_display_sync (__gtk_widget_get_display (GTK_WIDGET (icon)));
  __gdk_error_trap_pop ();

  return stamp;
}

void
_gtk_tray_icon_cancel_message (GtkTrayIcon *icon,
			       guint        id)
{
  g_return_if_fail (GTK_IS_TRAY_ICON (icon));
  g_return_if_fail (id > 0);
  
  gtk_tray_icon_send_manager_message (icon, SYSTEM_TRAY_CANCEL_MESSAGE,
				      (Window)__gtk_plug_get_id (GTK_PLUG (icon)),
				      id, 0, 0);
}

GtkTrayIcon *
_gtk_tray_icon_new_for_screen (GdkScreen  *screen, 
			       const gchar *name)
{
  g_return_val_if_fail (GDK_IS_SCREEN (screen), NULL);

  return g_object_new (GTK_TYPE_TRAY_ICON, 
		       "screen", screen, 
		       "title", name, 
		       NULL);
}

GtkTrayIcon*
_gtk_tray_icon_new (const gchar *name)
{
  return g_object_new (GTK_TYPE_TRAY_ICON, 
		       "title", name, 
		       NULL);
}

GtkOrientation
_gtk_tray_icon_get_orientation (GtkTrayIcon *icon)
{
  g_return_val_if_fail (GTK_IS_TRAY_ICON (icon), GTK_ORIENTATION_HORIZONTAL);

  return gtk_tray_icon_get_props (icon)->priv->orientation;
}


#define __GTK_TRAY_ICON_X11_C__
