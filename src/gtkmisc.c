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



#include "config.h"

#include <stlwrt.h>
#include <gtkcontainer.h>
#include <gtkmisc.h>
#include <gtkintl.h>
#include <gtkprivate.h>



enum {
  PROP_0,
  PROP_XALIGN,
  PROP_YALIGN,
  PROP_XPAD,
  PROP_YPAD
};

static void gtk_misc_realize      (GtkWidget    *widget);
static void gtk_misc_set_property (GObject         *object,
				   guint            prop_id,
				   const GValue    *value,
				   GParamSpec      *pspec);
static void gtk_misc_get_property (GObject         *object,
				   guint            prop_id,
				   GValue          *value,
				   GParamSpec      *pspec);


STLWRT_DEFINE_VTYPE (GtkMisc, gtk_misc, GTK_TYPE_WIDGET, G_TYPE_FLAG_ABSTRACT, ;)

static void
gtk_misc_class_init (GtkMiscClass *class)
{
  GObjectClass   *gobject_class;
  GtkWidgetClass *widget_class;

  gobject_class = G_OBJECT_CLASS (class);
  widget_class = (GtkWidgetClass*) class;

  gobject_class->set_property = gtk_misc_set_property;
  gobject_class->get_property = gtk_misc_get_property;
  
  widget_class->realize = gtk_misc_realize;

  g_object_class_install_property (gobject_class,
                                   PROP_XALIGN,
                                   g_param_spec_float ("xalign",
						       P_("X align"),
						       P_("The horizontal alignment, from 0 (left) to 1 (right). Reversed for RTL layouts."),
						       0.0,
						       1.0,
						       0.5,
						       GTK_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
                                   PROP_YALIGN,
                                   g_param_spec_float ("yalign",
						       P_("Y align"),
						       P_("The vertical alignment, from 0 (top) to 1 (bottom)"),
						       0.0,
						       1.0,
						       0.5,
						       GTK_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
                                   PROP_XPAD,
                                   g_param_spec_int ("xpad",
						     P_("X pad"),
						     P_("The amount of space to add on the left and right of the widget, in pixels"),
						     0,
						     G_MAXINT,
						     0,
						     GTK_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
                                   PROP_YPAD,
                                   g_param_spec_int ("ypad",
						     P_("Y pad"),
						     P_("The amount of space to add on the top and bottom of the widget, in pixels"),
						     0,
						     G_MAXINT,
						     0,
						     GTK_PARAM_READWRITE));
}

static void
gtk_misc_init (GtkMisc *misc)
{
  gtk_misc_get_props (misc)->xalign = 0.5;
  gtk_misc_get_props (misc)->yalign = 0.5;
  gtk_misc_get_props (misc)->xpad = 0;
  gtk_misc_get_props (misc)->ypad = 0;
}

static void
gtk_misc_set_property (GObject      *object,
		       guint         prop_id,
		       const GValue *value,
		       GParamSpec   *pspec)
{
  GtkMisc *misc;

  misc = GTK_MISC (object);

  switch (prop_id)
    {
    case PROP_XALIGN:
      __gtk_misc_set_alignment (gtk_misc_get_props (misc), g_value_get_float (value), gtk_misc_get_props (misc)->yalign);
      break;
    case PROP_YALIGN:
      __gtk_misc_set_alignment (gtk_misc_get_props (misc), gtk_misc_get_props (misc)->xalign, g_value_get_float (value));
      break;
    case PROP_XPAD:
      __gtk_misc_set_padding (gtk_misc_get_props (misc), g_value_get_int (value), gtk_misc_get_props (misc)->ypad);
      break;
    case PROP_YPAD:
      __gtk_misc_set_padding (gtk_misc_get_props (misc), gtk_misc_get_props (misc)->xpad, g_value_get_int (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_misc_get_property (GObject      *object,
		       guint         prop_id,
		       GValue       *value,
		       GParamSpec   *pspec)
{
  GtkMisc *misc;

  misc = GTK_MISC (object);

  switch (prop_id)
    {
    case PROP_XALIGN:
      g_value_set_float (value, gtk_misc_get_props (misc)->xalign);
      break;
    case PROP_YALIGN:
      g_value_set_float (value, gtk_misc_get_props (misc)->yalign);
      break;
    case PROP_XPAD:
      g_value_set_int (value, gtk_misc_get_props (misc)->xpad);
      break;
    case PROP_YPAD:
      g_value_set_int (value, gtk_misc_get_props (misc)->ypad);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

void
__gtk_misc_set_alignment (GtkMisc *misc,
			gfloat   xalign,
			gfloat   yalign)
{
  GtkWidget *widget;

  g_return_if_fail (GTK_IS_MISC (misc));

  if (xalign < 0.0)
    xalign = 0.0;
  else if (xalign > 1.0)
    xalign = 1.0;

  if (yalign < 0.0)
    yalign = 0.0;
  else if (yalign > 1.0)
    yalign = 1.0;

  if ((xalign != gtk_misc_get_props (misc)->xalign) || (yalign != gtk_misc_get_props (misc)->yalign))
    {
      g_object_freeze_notify (G_OBJECT (misc));
      if (xalign != gtk_misc_get_props (misc)->xalign)
	g_object_notify (G_OBJECT (misc), "xalign");

      if (yalign != gtk_misc_get_props (misc)->yalign)
	g_object_notify (G_OBJECT (misc), "yalign");

      gtk_misc_get_props (misc)->xalign = xalign;
      gtk_misc_get_props (misc)->yalign = yalign;
      
      /* clear the area that was allocated before the change
       */
      widget = GTK_WIDGET (misc);
      if (__gtk_widget_is_drawable (widget))
        __gtk_widget_queue_draw (widget);

      g_object_thaw_notify (G_OBJECT (misc));
    }
}

/**
 * __gtk_misc_get_alignment:
 * @misc: a #GtkMisc
 * @xalign: (out) (allow-none): location to store X alignment of @misc, or %NULL
 * @yalign: (out) (allow-none): location to store Y alignment of @misc, or %NULL
 *
 * Gets the X and Y alignment of the widget within its allocation. 
 * See __gtk_misc_set_alignment().
 **/
void
__gtk_misc_get_alignment (GtkMisc *misc,
		        gfloat  *xalign,
			gfloat  *yalign)
{
  g_return_if_fail (GTK_IS_MISC (misc));

  if (xalign)
    *xalign = gtk_misc_get_props (misc)->xalign;
  if (yalign)
    *yalign = gtk_misc_get_props (misc)->yalign;
}

void
__gtk_misc_set_padding (GtkMisc *misc,
		      gint     xpad,
		      gint     ypad)
{
  GtkRequisition *requisition;
  
  g_return_if_fail (GTK_IS_MISC (misc));
  
  if (xpad < 0)
    xpad = 0;
  if (ypad < 0)
    ypad = 0;
  
  if ((xpad != gtk_misc_get_props (misc)->xpad) || (ypad != gtk_misc_get_props (misc)->ypad))
    {
      g_object_freeze_notify (G_OBJECT (misc));
      if (xpad != gtk_misc_get_props (misc)->xpad)
	g_object_notify (G_OBJECT (misc), "xpad");

      if (ypad != gtk_misc_get_props (misc)->ypad)
	g_object_notify (G_OBJECT (misc), "ypad");

      requisition = &(gtk_widget_get_props (GTK_WIDGET (misc))->requisition);
      requisition->width -= gtk_misc_get_props (misc)->xpad * 2;
      requisition->height -= gtk_misc_get_props (misc)->ypad * 2;
      
      gtk_misc_get_props (misc)->xpad = xpad;
      gtk_misc_get_props (misc)->ypad = ypad;
      
      requisition->width += gtk_misc_get_props (misc)->xpad * 2;
      requisition->height += gtk_misc_get_props (misc)->ypad * 2;
      
      if (__gtk_widget_is_drawable (GTK_WIDGET (misc)))
	__gtk_widget_queue_resize (GTK_WIDGET (misc));

      g_object_thaw_notify (G_OBJECT (misc));
    }
}

/**
 * __gtk_misc_get_padding:
 * @misc: a #GtkMisc
 * @xpad: (out) (allow-none): location to store padding in the X
 *        direction, or %NULL
 * @ypad: (out) (allow-none): location to store padding in the Y
 *        direction, or %NULL
 *
 * Gets the padding in the X and Y directions of the widget. 
 * See __gtk_misc_set_padding().
 **/
void
__gtk_misc_get_padding (GtkMisc *misc,
		      gint    *xpad,
		      gint    *ypad)
{
  g_return_if_fail (GTK_IS_MISC (misc));

  if (xpad)
    *xpad = gtk_misc_get_props (misc)->xpad;
  if (ypad)
    *ypad = gtk_misc_get_props (misc)->ypad;
}

static void
gtk_misc_realize (GtkWidget *widget)
{
  GdkWindowAttr attributes;
  gint attributes_mask;

  __gtk_widget_set_realized (widget, TRUE);

  if (!__gtk_widget_get_has_window (widget))
    {
      gtk_widget_get_props (widget)->window = __gtk_widget_get_parent_window (gtk_widget_get_props (widget));
      g_object_ref (gtk_widget_get_props (widget)->window);
      gtk_widget_get_props (widget)->style = __gtk_style_attach (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window);
    }
  else
    {
      attributes.window_type = GDK_WINDOW_CHILD;
      attributes.x = gtk_widget_get_props (widget)->allocation.x;
      attributes.y = gtk_widget_get_props (widget)->allocation.y;
      attributes.width = gtk_widget_get_props (widget)->allocation.width;
      attributes.height = gtk_widget_get_props (widget)->allocation.height;
      attributes.wclass = GDK_INPUT_OUTPUT;
      attributes.visual = __gtk_widget_get_visual (widget);
      attributes.colormap = __gtk_widget_get_colormap (widget);
      attributes.event_mask = __gtk_widget_get_events (widget) | GDK_EXPOSURE_MASK;
      attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

      gtk_widget_get_props (widget)->window = __gdk_window_new (__gtk_widget_get_parent_window (gtk_widget_get_props (widget)), &attributes, attributes_mask);
      __gdk_window_set_user_data (gtk_widget_get_props (widget)->window, gtk_widget_get_props (widget));

      gtk_widget_get_props (widget)->style = __gtk_style_attach (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window);
      __gdk_window_set_back_pixmap (gtk_widget_get_props (widget)->window, NULL, TRUE);
    }
}
