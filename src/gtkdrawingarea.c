
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

#include <stlwrtobject.h>
#include <gtkdrawingarea.h>
#include <gtkintl.h>



static void gtk_drawing_area_realize       (GtkWidget           *widget);
static void __gtk_drawing_area_size_allocate (GtkWidget           *widget,
					    GtkAllocation       *allocation);
static void gtk_drawing_area_send_configure (GtkDrawingArea     *darea);

STLWRT_DEFINE_TYPE (GtkDrawingArea, gtk_drawing_area, GTK_TYPE_WIDGET)

static void
gtk_drawing_area_class_init (GtkDrawingAreaClass *class)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

  widget_class->realize = gtk_drawing_area_realize;
  widget_class->size_allocate = __gtk_drawing_area_size_allocate;
}

static void
gtk_drawing_area_init (GtkDrawingArea *darea)
{
  darea->draw_data = NULL;
}


GtkWidget*
__gtk_drawing_area_new (void)
{
  return g_object_new (GTK_TYPE_DRAWING_AREA, NULL);
}

void
__gtk_drawing_area_size (GtkDrawingArea *darea,
		       gint            width,
		       gint            height)
{
  g_return_if_fail (GTK_IS_DRAWING_AREA (darea));

  GTK_WIDGET (darea)->requisition.width = width;
  GTK_WIDGET (darea)->requisition.height = height;

  __gtk_widget_queue_resize (GTK_WIDGET (darea));
}

static void
gtk_drawing_area_realize (GtkWidget *widget)
{
  GtkDrawingArea *darea = GTK_DRAWING_AREA (widget);
  GdkWindowAttr attributes;
  gint attributes_mask;

  if (!__gtk_widget_get_has_window (widget))
    {
      GTK_WIDGET_CLASS (gtk_drawing_area_parent_class)->realize (widget);
    }
  else
    {
      __gtk_widget_set_realized (widget, TRUE);

      attributes.window_type = GDK_WINDOW_CHILD;
      attributes.x = widget->allocation.x;
      attributes.y = widget->allocation.y;
      attributes.width = widget->allocation.width;
      attributes.height = widget->allocation.height;
      attributes.wclass = GDK_INPUT_OUTPUT;
      attributes.visual = __gtk_widget_get_visual (widget);
      attributes.colormap = __gtk_widget_get_colormap (widget);
      attributes.event_mask = __gtk_widget_get_events (widget) | GDK_EXPOSURE_MASK;

      attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

      widget->window = __gdk_window_new (__gtk_widget_get_parent_window (widget),
                                       &attributes, attributes_mask);
      __gdk_window_set_user_data (widget->window, darea);

      widget->style = __gtk_style_attach (widget->style, widget->window);
      __gtk_style_set_background (widget->style, widget->window, GTK_STATE_NORMAL);
    }

  gtk_drawing_area_send_configure (GTK_DRAWING_AREA (widget));
}

static void
__gtk_drawing_area_size_allocate (GtkWidget     *widget,
				GtkAllocation *allocation)
{
  g_return_if_fail (GTK_IS_DRAWING_AREA (widget));
  g_return_if_fail (allocation != NULL);

  widget->allocation = *allocation;

  if (__gtk_widget_get_realized (widget))
    {
      if (__gtk_widget_get_has_window (widget))
        __gdk_window_move_resize (widget->window,
                                allocation->x, allocation->y,
                                allocation->width, allocation->height);

      gtk_drawing_area_send_configure (GTK_DRAWING_AREA (widget));
    }
}

static void
gtk_drawing_area_send_configure (GtkDrawingArea *darea)
{
  GtkWidget *widget;
  GdkEvent *event = __gdk_event_new (GDK_CONFIGURE);

  widget = GTK_WIDGET (darea);

  event->configure.window = g_object_ref (widget->window);
  event->configure.send_event = TRUE;
  event->configure.x = widget->allocation.x;
  event->configure.y = widget->allocation.y;
  event->configure.width = widget->allocation.width;
  event->configure.height = widget->allocation.height;
  
  __gtk_widget_event (widget, event);
  __gdk_event_free (event);
}

#define __GTK_DRAWING_AREA_C__
