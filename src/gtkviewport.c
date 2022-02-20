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
#include <gtkviewport.h>
#include <gtkintl.h>

#include <gtkprivate.h>


/**
 * SECTION:gtkviewport
 * @Short_description: An adapter which makes widgets scrollable
 * @Title: GtkViewport
 * @See_also:#GtkScrolledWindow, #GtkAdjustment
 *
 * The #GtkViewport widget acts as an adaptor class, implementing
 * scrollability for child widgets that lack their own scrolling
 * capabilities. Use #GtkViewport to scroll child widgets such as
 * #GtkTable, #GtkBox, and so on.
 *
 * If a widget has native scrolling abilities, such as #GtkTextView,
 * #GtkTreeView or #GtkIconview, it can be added to a #GtkScrolledWindow
 * with __gtk_container_add(). If a widget does not, you must first add the
 * widget to a #GtkViewport, then add the viewport to the scrolled window.
 * The convenience function __gtk_scrolled_window_add_with_viewport() does
 * exactly this, so you can ignore the presence of the viewport.
 */

enum {
  PROP_0,
  PROP_HADJUSTMENT,
  PROP_VADJUSTMENT,
  PROP_SHADOW_TYPE
};


static void gtk_viewport_finalize                 (GObject          *object);
static void gtk_viewport_set_property             (GObject         *object,
						   guint            prop_id,
						   const GValue    *value,
						   GParamSpec      *pspec);
static void gtk_viewport_get_property             (GObject         *object,
						   guint            prop_id,
						   GValue          *value,
						   GParamSpec      *pspec);
static void gtk_viewport_set_scroll_adjustments	  (GtkViewport	    *viewport,
						   GtkAdjustment    *hadjustment,
						   GtkAdjustment    *vadjustment);
static void gtk_viewport_realize                  (GtkWidget        *widget);
static void gtk_viewport_unrealize                (GtkWidget        *widget);
static void gtk_viewport_paint                    (GtkWidget        *widget,
						   GdkRectangle     *area);
static gint gtk_viewport_expose                   (GtkWidget        *widget,
						   GdkEventExpose   *event);
static void gtk_viewport_add                      (GtkContainer     *container,
						   GtkWidget        *widget);
static void gtk_viewport_size_request             (GtkWidget        *widget,
						   GtkRequisition   *requisition);
static void gtk_viewport_size_allocate            (GtkWidget        *widget,
						   GtkAllocation    *allocation);
static void gtk_viewport_adjustment_value_changed (GtkAdjustment    *adjustment,
						   gpointer          data);
static void gtk_viewport_style_set                (GtkWidget *widget,
			                           GtkStyle  *previous_style);

STLWRT_DEFINE_VTYPE (GtkViewport, gtk_viewport, GTK_TYPE_BIN, G_TYPE_FLAG_NONE, ;)

static void
gtk_viewport_class_init (GtkViewportClass *class)
{
  GObjectClass   *gobject_class;
  GtkWidgetClass *widget_class;
  GtkContainerClass *container_class;

  gobject_class = G_OBJECT_CLASS (class);
  widget_class = (GtkWidgetClass*) class;
  container_class = (GtkContainerClass*) class;

  gobject_class->finalize = gtk_viewport_finalize;
  gobject_class->set_property = gtk_viewport_set_property;
  gobject_class->get_property = gtk_viewport_get_property;
  
  widget_class->realize = gtk_viewport_realize;
  widget_class->unrealize = gtk_viewport_unrealize;
  widget_class->expose_event = gtk_viewport_expose;
  widget_class->size_request = gtk_viewport_size_request;
  widget_class->size_allocate = gtk_viewport_size_allocate;
  widget_class->style_set = gtk_viewport_style_set;
  
  container_class->add = gtk_viewport_add;

  class->set_scroll_adjustments = gtk_viewport_set_scroll_adjustments;

  g_object_class_install_property (gobject_class,
                                   PROP_HADJUSTMENT,
                                   g_param_spec_object ("hadjustment",
							P_("Horizontal adjustment"),
							P_("The GtkAdjustment that determines the values of the horizontal position for this viewport"),
                                                        GTK_TYPE_ADJUSTMENT,
                                                        GTK_PARAM_READWRITE | G_PARAM_CONSTRUCT));

  g_object_class_install_property (gobject_class,
                                   PROP_VADJUSTMENT,
                                   g_param_spec_object ("vadjustment",
							P_("Vertical adjustment"),
							P_("The GtkAdjustment that determines the values of the vertical position for this viewport"),
                                                        GTK_TYPE_ADJUSTMENT,
                                                        GTK_PARAM_READWRITE | G_PARAM_CONSTRUCT));

  g_object_class_install_property (gobject_class,
                                   PROP_SHADOW_TYPE,
                                   g_param_spec_enum ("shadow-type",
						      P_("Shadow type"),
						      P_("Determines how the shadowed box around the viewport is drawn"),
						      GTK_TYPE_SHADOW_TYPE,
						      GTK_SHADOW_IN,
						      GTK_PARAM_READWRITE));

  /**
   * GtkViewport::set-scroll-adjustments
   * @horizontal: the horizontal #GtkAdjustment
   * @vertical: the vertical #GtkAdjustment
   *
   * Set the scroll adjustments for the viewport. Usually scrolled containers
   * like #GtkScrolledWindow will emit this signal to connect two instances
   * of #GtkScrollbar to the scroll directions of the #GtkViewport.
   */
  widget_class->set_scroll_adjustments_signal =
    g_signal_new (I_("set-scroll-adjustments"),
		  G_OBJECT_CLASS_TYPE (gobject_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkViewportClass, set_scroll_adjustments),
		  NULL, NULL,
		  NULL,
		  G_TYPE_NONE, 2,
		  GTK_TYPE_ADJUSTMENT,
		  GTK_TYPE_ADJUSTMENT);
}

static void
gtk_viewport_set_property (GObject         *object,
			   guint            prop_id,
			   const GValue    *value,
			   GParamSpec      *pspec)
{
  GtkViewport *viewport;

  viewport = GTK_VIEWPORT (object);

  switch (prop_id)
    {
    case PROP_HADJUSTMENT:
      __gtk_viewport_set_hadjustment (viewport, g_value_get_object (value));
      break;
    case PROP_VADJUSTMENT:
      __gtk_viewport_set_vadjustment (viewport, g_value_get_object (value));
      break;
    case PROP_SHADOW_TYPE:
      __gtk_viewport_set_shadow_type (viewport, g_value_get_enum (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_viewport_get_property (GObject         *object,
			   guint            prop_id,
			   GValue          *value,
			   GParamSpec      *pspec)
{
  GtkViewport *viewport;

  viewport = GTK_VIEWPORT (object);

  switch (prop_id)
    {
    case PROP_HADJUSTMENT:
      g_value_set_object (value, gtk_viewport_get_props (viewport)->hadjustment);
      break;
    case PROP_VADJUSTMENT:
      g_value_set_object (value, gtk_viewport_get_props (viewport)->vadjustment);
      break;
    case PROP_SHADOW_TYPE:
      g_value_set_enum (value, gtk_viewport_get_props (viewport)->shadow_type);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_viewport_init (GtkViewport *viewport)
{
  __gtk_widget_set_has_window (GTK_WIDGET (viewport), TRUE);

  __gtk_widget_set_redraw_on_allocate (GTK_WIDGET (viewport), FALSE);
  __gtk_container_set_resize_mode (GTK_CONTAINER (viewport), GTK_RESIZE_QUEUE);
  
  gtk_viewport_get_props (viewport)->shadow_type = GTK_SHADOW_IN;
  gtk_viewport_get_props (viewport)->view_window = NULL;
  gtk_viewport_get_props (viewport)->bin_window = NULL;
  gtk_viewport_get_props (viewport)->hadjustment = NULL;
  gtk_viewport_get_props (viewport)->vadjustment = NULL;
}

/**
 * __gtk_viewport_new:
 * @hadjustment: horizontal adjustment.
 * @vadjustment: vertical adjustment.
 * @returns: a new #GtkViewport.
 *
 * Creates a new #GtkViewport with the given adjustments.
 *
 **/
GtkWidget*
__gtk_viewport_new (GtkAdjustment *hadjustment,
		  GtkAdjustment *vadjustment)
{
  GtkWidget *viewport;

  viewport = g_object_new (GTK_TYPE_VIEWPORT,
			     "hadjustment", hadjustment,
			     "vadjustment", vadjustment,
			     NULL);

  return viewport;
}

#define ADJUSTMENT_POINTER(viewport, orientation)         \
  (((orientation) == GTK_ORIENTATION_HORIZONTAL) ?        \
     &gtk_viewport_get_props (gtk_viewport_get_props ((viewport)))->hadjustment : &gtk_viewport_get_props (gtk_viewport_get_props ((viewport)))->vadjustment)

static void
viewport_disconnect_adjustment (GtkViewport    *viewport,
				GtkOrientation  orientation)
{
  GtkAdjustment **adjustmentp = ADJUSTMENT_POINTER (viewport, orientation);

  if (*adjustmentp)
    {
      g_signal_handlers_disconnect_by_func (*adjustmentp,
					    gtk_viewport_adjustment_value_changed,
					    viewport);
      g_object_unref (*adjustmentp);
      *adjustmentp = NULL;
    }
}

static void
gtk_viewport_finalize (GObject *object)
{
  GtkViewport *viewport = GTK_VIEWPORT (object);

  viewport_disconnect_adjustment (viewport, GTK_ORIENTATION_HORIZONTAL);
  viewport_disconnect_adjustment (viewport, GTK_ORIENTATION_VERTICAL);

  G_OBJECT_CLASS (gtk_viewport_parent_class)->finalize (object);
}

/**
 * __gtk_viewport_get_hadjustment:
 * @viewport: a #GtkViewport.
 *
 * Returns the horizontal adjustment of the viewport.
 *
 * Return value: (transfer none): the horizontal adjustment of @viewport.
 **/
GtkAdjustment*
__gtk_viewport_get_hadjustment (GtkViewport *viewport)
{
  g_return_val_if_fail (GTK_IS_VIEWPORT (viewport), NULL);

  if (!gtk_viewport_get_props (viewport)->hadjustment)
    __gtk_viewport_set_hadjustment (viewport, NULL);

  return gtk_viewport_get_props (viewport)->hadjustment;
}

/**
 * __gtk_viewport_get_vadjustment:
 * @viewport: a #GtkViewport.
 * 
 * Returns the vertical adjustment of the viewport.
 *
 * Return value: (transfer none): the vertical adjustment of @viewport.
 **/
GtkAdjustment*
__gtk_viewport_get_vadjustment (GtkViewport *viewport)
{
  g_return_val_if_fail (GTK_IS_VIEWPORT (viewport), NULL);

  if (!gtk_viewport_get_props (viewport)->vadjustment)
    __gtk_viewport_set_vadjustment (viewport, NULL);

  return gtk_viewport_get_props (viewport)->vadjustment;
}

static void
viewport_get_view_allocation (GtkViewport   *viewport,
			      GtkAllocation *view_allocation)
{
  GtkWidget *widget = GTK_WIDGET (viewport);
  GtkAllocation *allocation = &gtk_widget_get_props (widget)->allocation;
  gint border_width = gtk_container_get_props (GTK_CONTAINER (viewport))->border_width;
  
  view_allocation->x = 0;
  view_allocation->y = 0;

  if (gtk_viewport_get_props (viewport)->shadow_type != GTK_SHADOW_NONE)
    {
      view_allocation->x = gtk_widget_get_props (widget)->style->xthickness;
      view_allocation->y = gtk_widget_get_props (widget)->style->ythickness;
    }

  view_allocation->width = MAX (1, allocation->width - view_allocation->x * 2 - border_width * 2);
  view_allocation->height = MAX (1, allocation->height - view_allocation->y * 2 - border_width * 2);
}

static void
viewport_reclamp_adjustment (GtkAdjustment *adjustment,
			     gboolean      *value_changed)
{
  gdouble value = gtk_adjustment_get_props (adjustment)->value;
  
  value = CLAMP (value, 0, gtk_adjustment_get_props (adjustment)->upper - gtk_adjustment_get_props (adjustment)->page_size);
  if (value != gtk_adjustment_get_props (adjustment)->value)
    {
      gtk_adjustment_get_props (adjustment)->value = value;
      if (value_changed)
	*value_changed = TRUE;
    }
  else if (value_changed)
    *value_changed = FALSE;
}

static void
viewport_set_hadjustment_values (GtkViewport *viewport,
				 gboolean    *value_changed)
{
  GtkBin *bin = GTK_BIN (viewport);
  GtkAllocation view_allocation;
  GtkAdjustment *hadjustment = __gtk_viewport_get_hadjustment (viewport);
  gdouble old_page_size;
  gdouble old_upper;
  gdouble old_value;
  
  viewport_get_view_allocation (viewport, &view_allocation);  

  old_page_size = gtk_adjustment_get_props (hadjustment)->page_size;
  old_upper = gtk_adjustment_get_props (hadjustment)->upper;
  old_value = gtk_adjustment_get_props (hadjustment)->value;
  gtk_adjustment_get_props (hadjustment)->page_size = view_allocation.width;
  gtk_adjustment_get_props (hadjustment)->step_increment = view_allocation.width * 0.1;
  gtk_adjustment_get_props (hadjustment)->page_increment = view_allocation.width * 0.9;
  
  gtk_adjustment_get_props (hadjustment)->lower = 0;

  if (gtk_bin_get_props (bin)->child && __gtk_widget_get_visible (gtk_bin_get_props (bin)->child))
    {
      GtkRequisition child_requisition;
      
      __gtk_widget_get_child_requisition (gtk_bin_get_props (bin)->child, &child_requisition);
      gtk_adjustment_get_props (hadjustment)->upper = MAX (child_requisition.width, view_allocation.width);
    }
  else
    gtk_adjustment_get_props (hadjustment)->upper = view_allocation.width;

  if (__gtk_widget_get_direction (GTK_WIDGET (viewport)) == GTK_TEXT_DIR_RTL) 
    {
      gdouble dist = old_upper - (old_value + old_page_size);
      gtk_adjustment_get_props (hadjustment)->value = gtk_adjustment_get_props (hadjustment)->upper - dist - gtk_adjustment_get_props (hadjustment)->page_size;
      viewport_reclamp_adjustment (hadjustment, value_changed);
      *value_changed = (old_value != gtk_adjustment_get_props (hadjustment)->value);
    }
  else
    viewport_reclamp_adjustment (hadjustment, value_changed);
}

static void
viewport_set_vadjustment_values (GtkViewport *viewport,
				 gboolean    *value_changed)
{
  GtkBin *bin = GTK_BIN (viewport);
  GtkAllocation view_allocation;
  GtkAdjustment *vadjustment = __gtk_viewport_get_vadjustment (viewport);

  viewport_get_view_allocation (viewport, &view_allocation);  

  gtk_adjustment_get_props (vadjustment)->page_size = view_allocation.height;
  gtk_adjustment_get_props (vadjustment)->step_increment = view_allocation.height * 0.1;
  gtk_adjustment_get_props (vadjustment)->page_increment = view_allocation.height * 0.9;
  
  gtk_adjustment_get_props (vadjustment)->lower = 0;

  if (gtk_bin_get_props (bin)->child && __gtk_widget_get_visible (gtk_bin_get_props (bin)->child))
    {
      GtkRequisition child_requisition;
      
      __gtk_widget_get_child_requisition (gtk_bin_get_props (bin)->child, &child_requisition);
      gtk_adjustment_get_props (vadjustment)->upper = MAX (child_requisition.height, view_allocation.height);
    }
  else
    gtk_adjustment_get_props (vadjustment)->upper = view_allocation.height;

  viewport_reclamp_adjustment (vadjustment, value_changed);
}

static void
viewport_set_adjustment (GtkViewport    *viewport,
			 GtkOrientation  orientation,
			 GtkAdjustment  *adjustment)
{
  GtkAdjustment **adjustmentp = ADJUSTMENT_POINTER (viewport, orientation);
  gboolean value_changed;

  if (adjustment && adjustment == *adjustmentp)
    return;

  if (!adjustment)
    adjustment = GTK_ADJUSTMENT (__gtk_adjustment_new (0.0, 0.0, 0.0,
						     0.0, 0.0, 0.0));
  viewport_disconnect_adjustment (viewport, orientation);
  *adjustmentp = adjustment;
  g_object_ref_sink (adjustment);

  if (orientation == GTK_ORIENTATION_HORIZONTAL)
    viewport_set_hadjustment_values (viewport, &value_changed);
  else
    viewport_set_vadjustment_values (viewport, &value_changed);

  g_signal_connect (adjustment, "value-changed",
		    G_CALLBACK (gtk_viewport_adjustment_value_changed),
		    viewport);

  __gtk_adjustment_changed (adjustment);
  
  if (value_changed)
    __gtk_adjustment_value_changed (adjustment);
  else
    gtk_viewport_adjustment_value_changed (adjustment, viewport);
}

/**
 * __gtk_viewport_set_hadjustment:
 * @viewport: a #GtkViewport.
 * @adjustment: (allow-none): a #GtkAdjustment.
 *
 * Sets the horizontal adjustment of the viewport.
 **/
void
__gtk_viewport_set_hadjustment (GtkViewport   *viewport,
			      GtkAdjustment *adjustment)
{
  g_return_if_fail (GTK_IS_VIEWPORT (viewport));
  if (adjustment)
    g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  viewport_set_adjustment (viewport, GTK_ORIENTATION_HORIZONTAL, adjustment);

  g_object_notify (G_OBJECT (viewport), "hadjustment");
}

/**
 * __gtk_viewport_set_vadjustment:
 * @viewport: a #GtkViewport.
 * @adjustment: (allow-none): a #GtkAdjustment.
 *
 * Sets the vertical adjustment of the viewport.
 **/
void
__gtk_viewport_set_vadjustment (GtkViewport   *viewport,
			      GtkAdjustment *adjustment)
{
  g_return_if_fail (GTK_IS_VIEWPORT (viewport));
  if (adjustment)
    g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  viewport_set_adjustment (viewport, GTK_ORIENTATION_VERTICAL, adjustment);

  g_object_notify (G_OBJECT (viewport), "vadjustment");
}

static void
gtk_viewport_set_scroll_adjustments (GtkViewport      *viewport,
				     GtkAdjustment    *hadjustment,
				     GtkAdjustment    *vadjustment)
{
  __gtk_viewport_set_hadjustment (viewport, hadjustment);
  __gtk_viewport_set_vadjustment (viewport, vadjustment);
}

/** 
 * __gtk_viewport_set_shadow_type:
 * @viewport: a #GtkViewport.
 * @type: the new shadow type.
 *
 * Sets the shadow type of the viewport.
 **/ 
void
__gtk_viewport_set_shadow_type (GtkViewport   *viewport,
			      GtkShadowType  type)
{
  g_return_if_fail (GTK_IS_VIEWPORT (viewport));

  if ((GtkShadowType) gtk_viewport_get_props (viewport)->shadow_type != type)
    {
      gtk_viewport_get_props (viewport)->shadow_type = type;

      if (__gtk_widget_get_visible (GTK_WIDGET (viewport)))
	{
	  __gtk_widget_size_allocate (GTK_WIDGET (viewport), &(gtk_widget_get_props (GTK_WIDGET (viewport))->allocation));
	  __gtk_widget_queue_draw (GTK_WIDGET (viewport));
	}

      g_object_notify (G_OBJECT (viewport), "shadow-type");
    }
}

/**
 * __gtk_viewport_get_shadow_type:
 * @viewport: a #GtkViewport
 *
 * Gets the shadow type of the #GtkViewport. See
 * __gtk_viewport_set_shadow_type().
 
 * Return value: the shadow type 
 **/
GtkShadowType
__gtk_viewport_get_shadow_type (GtkViewport *viewport)
{
  g_return_val_if_fail (GTK_IS_VIEWPORT (viewport), GTK_SHADOW_NONE);

  return gtk_viewport_get_props (viewport)->shadow_type;
}

/**
 * __gtk_viewport_get_bin_window:
 * @viewport: a #GtkViewport
 *
 * Gets the bin window of the #GtkViewport.
 *
 * Return value: (transfer none): a #GdkWindow
 *
 * Since: 2.20
 **/
GdkWindow*
__gtk_viewport_get_bin_window (GtkViewport *viewport)
{
  g_return_val_if_fail (GTK_IS_VIEWPORT (viewport), NULL);

  return gtk_viewport_get_props (viewport)->bin_window;
}

/**
 * __gtk_viewport_get_view_window:
 * @viewport: a #GtkViewport
 *
 * Gets the view window of the #GtkViewport.
 *
 * Return value: (transfer none): a #GdkWindow
 *
 * Since: 2.22
 **/
GdkWindow*
__gtk_viewport_get_view_window (GtkViewport *viewport)
{
  g_return_val_if_fail (GTK_IS_VIEWPORT (viewport), NULL);

  return gtk_viewport_get_props (viewport)->view_window;
}

static void
gtk_viewport_realize (GtkWidget *widget)
{
  GtkViewport *viewport = GTK_VIEWPORT (widget);
  GtkBin *bin = GTK_BIN (widget);
  GtkAdjustment *hadjustment = __gtk_viewport_get_hadjustment (viewport);
  GtkAdjustment *vadjustment = __gtk_viewport_get_vadjustment (viewport);
  gint border_width = gtk_container_get_props (GTK_CONTAINER (widget))->border_width;
  
  GtkAllocation view_allocation;
  GdkWindowAttr attributes;
  gint attributes_mask;
  gint event_mask;

  __gtk_widget_set_realized (widget, TRUE);

  attributes.x = gtk_widget_get_props (widget)->allocation.x + border_width;
  attributes.y = gtk_widget_get_props (widget)->allocation.y + border_width;
  attributes.width = gtk_widget_get_props (widget)->allocation.width - border_width * 2;
  attributes.height = gtk_widget_get_props (widget)->allocation.height - border_width * 2;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.wclass = GDK_INPUT_OUTPUT;
  attributes.visual = __gtk_widget_get_visual (widget);
  attributes.colormap = __gtk_widget_get_colormap (widget);

  event_mask = __gtk_widget_get_events (widget) | GDK_EXPOSURE_MASK;
  /* We select on button_press_mask so that button 4-5 scrolls are trapped.
   */
  attributes.event_mask = event_mask | GDK_BUTTON_PRESS_MASK;

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  gtk_widget_get_props (widget)->window = __gdk_window_new (__gtk_widget_get_parent_window (widget),
				   &attributes, attributes_mask);
  __gdk_window_set_user_data (gtk_widget_get_props (widget)->window, viewport);

  viewport_get_view_allocation (viewport, &view_allocation);
  
  attributes.x = view_allocation.x;
  attributes.y = view_allocation.y;
  attributes.width = view_allocation.width;
  attributes.height = view_allocation.height;
  attributes.event_mask = 0;

  gtk_viewport_get_props (viewport)->view_window = __gdk_window_new (gtk_widget_get_props (widget)->window, &attributes, attributes_mask);
  __gdk_window_set_user_data (gtk_viewport_get_props (viewport)->view_window, viewport);

  __gdk_window_set_back_pixmap (gtk_viewport_get_props (viewport)->view_window, NULL, FALSE);
  
  attributes.x = - gtk_adjustment_get_props (hadjustment)->value;
  attributes.y = - gtk_adjustment_get_props (vadjustment)->value;
  attributes.width = gtk_adjustment_get_props (hadjustment)->upper;
  attributes.height = gtk_adjustment_get_props (vadjustment)->upper;
  
  attributes.event_mask = event_mask;

  gtk_viewport_get_props (viewport)->bin_window = __gdk_window_new (gtk_viewport_get_props (viewport)->view_window, &attributes, attributes_mask);
  __gdk_window_set_user_data (gtk_viewport_get_props (viewport)->bin_window, viewport);

  if (gtk_bin_get_props (bin)->child)
    __gtk_widget_set_parent_window (gtk_bin_get_props (bin)->child, gtk_viewport_get_props (viewport)->bin_window);

  gtk_widget_get_props (widget)->style = __gtk_style_attach (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window);
  __gtk_style_set_background (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window, GTK_STATE_NORMAL);
  __gtk_style_set_background (gtk_widget_get_props (widget)->style, gtk_viewport_get_props (viewport)->bin_window, GTK_STATE_NORMAL);

  /* Call paint here to allow a theme to set the background without flashing
   */
  __gtk_paint_flat_box(gtk_widget_get_props (widget)->style, gtk_viewport_get_props (viewport)->bin_window, GTK_STATE_NORMAL,
		     GTK_SHADOW_NONE,
		     NULL, widget, "viewportbin",
		     0, 0, -1, -1);
   
  __gdk_window_show (gtk_viewport_get_props (viewport)->bin_window);
  __gdk_window_show (gtk_viewport_get_props (viewport)->view_window);
}

static void
gtk_viewport_unrealize (GtkWidget *widget)
{
  GtkViewport *viewport = GTK_VIEWPORT (widget);

  __gdk_window_set_user_data (gtk_viewport_get_props (viewport)->view_window, NULL);
  __gdk_window_destroy (gtk_viewport_get_props (viewport)->view_window);
  gtk_viewport_get_props (viewport)->view_window = NULL;

  __gdk_window_set_user_data (gtk_viewport_get_props (viewport)->bin_window, NULL);
  __gdk_window_destroy (gtk_viewport_get_props (viewport)->bin_window);
  gtk_viewport_get_props (viewport)->bin_window = NULL;

  GTK_WIDGET_CLASS (gtk_viewport_parent_class)->unrealize (widget);
}

static void
gtk_viewport_paint (GtkWidget    *widget,
		    GdkRectangle *area)
{
  if (__gtk_widget_is_drawable (widget))
    {
      GtkViewport *viewport = GTK_VIEWPORT (widget);

      __gtk_paint_shadow (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window,
			GTK_STATE_NORMAL, gtk_viewport_get_props (viewport)->shadow_type,
			area, widget, "viewport",
			0, 0, -1, -1);
    }
}

static gint
gtk_viewport_expose (GtkWidget      *widget,
		     GdkEventExpose *event)
{
  GtkViewport *viewport;

  if (__gtk_widget_is_drawable (widget))
    {
      viewport = GTK_VIEWPORT (widget);

      if (event->window == gtk_widget_get_props (widget)->window)
	gtk_viewport_paint (widget, &event->area);
      else if (event->window == gtk_viewport_get_props (viewport)->bin_window)
	{
	  __gtk_paint_flat_box(gtk_widget_get_props (widget)->style, gtk_viewport_get_props (viewport)->bin_window, 
			     GTK_STATE_NORMAL, GTK_SHADOW_NONE,
			     &event->area, widget, "viewportbin",
			     0, 0, -1, -1);

	  GTK_WIDGET_CLASS (gtk_viewport_parent_class)->expose_event (widget, event);
	}
    }

  return FALSE;
}

static void
gtk_viewport_add (GtkContainer *container,
		  GtkWidget    *child)
{
  GtkBin *bin = GTK_BIN (container);

  g_return_if_fail (gtk_bin_get_props (bin)->child == NULL);

  __gtk_widget_set_parent_window (child, gtk_viewport_get_props (GTK_VIEWPORT (bin))->bin_window);

  GTK_CONTAINER_CLASS (gtk_viewport_parent_class)->add (container, child);
}

static void
gtk_viewport_size_request (GtkWidget      *widget,
			   GtkRequisition *requisition)
{
  GtkBin *bin = GTK_BIN (widget);
  GtkRequisition child_requisition;

  requisition->width = gtk_container_get_props (GTK_CONTAINER (widget))->border_width;

  requisition->height = gtk_container_get_props (GTK_CONTAINER (widget))->border_width;

  if (gtk_viewport_get_props (GTK_VIEWPORT (widget))->shadow_type != GTK_SHADOW_NONE)
    {
      requisition->width += 2 * gtk_widget_get_props (widget)->style->xthickness;
      requisition->height += 2 * gtk_widget_get_props (widget)->style->ythickness;
    }

  if (gtk_bin_get_props (bin)->child && __gtk_widget_get_visible (gtk_bin_get_props (bin)->child))
    {
      __gtk_widget_size_request (gtk_bin_get_props (bin)->child, &child_requisition);
      requisition->width += child_requisition.width;
      requisition->height += child_requisition.height;
    }
}

static void
gtk_viewport_size_allocate (GtkWidget     *widget,
			    GtkAllocation *allocation)
{
  GtkViewport *viewport = GTK_VIEWPORT (widget);
  GtkBin *bin = GTK_BIN (widget);
  gint border_width = gtk_container_get_props (GTK_CONTAINER (widget))->border_width;
  gboolean hadjustment_value_changed, vadjustment_value_changed;
  GtkAdjustment *hadjustment = __gtk_viewport_get_hadjustment (viewport);
  GtkAdjustment *vadjustment = __gtk_viewport_get_vadjustment (viewport);
  GtkAllocation child_allocation;

  /* If our size changed, and we have a shadow, queue a redraw on gtk_widget_get_props (widget)->window to
   * redraw the shadow correctly.
   */
  if (__gtk_widget_get_mapped (widget) &&
      gtk_viewport_get_props (viewport)->shadow_type != GTK_SHADOW_NONE &&
      (gtk_widget_get_props (widget)->allocation.width != allocation->width ||
       gtk_widget_get_props (widget)->allocation.height != allocation->height))
    __gdk_window_invalidate_rect (gtk_widget_get_props (widget)->window, NULL, FALSE);
  
  gtk_widget_get_props (widget)->allocation = *allocation;
  
  viewport_set_hadjustment_values (viewport, &hadjustment_value_changed);
  viewport_set_vadjustment_values (viewport, &vadjustment_value_changed);
  
  child_allocation.x = 0;
  child_allocation.y = 0;
  child_allocation.width = gtk_adjustment_get_props (hadjustment)->upper;
  child_allocation.height = gtk_adjustment_get_props (vadjustment)->upper;
  if (__gtk_widget_get_realized (widget))
    {
      GtkAllocation view_allocation;
      __gdk_window_move_resize (gtk_widget_get_props (widget)->window,
			      allocation->x + border_width,
			      allocation->y + border_width,
			      allocation->width - border_width * 2,
			      allocation->height - border_width * 2);
      
      viewport_get_view_allocation (viewport, &view_allocation);
      __gdk_window_move_resize (gtk_viewport_get_props (viewport)->view_window,
			      view_allocation.x,
			      view_allocation.y,
			      view_allocation.width,
			      view_allocation.height);
      __gdk_window_move_resize (gtk_viewport_get_props (viewport)->bin_window,
                              - gtk_adjustment_get_props (hadjustment)->value,
                              - gtk_adjustment_get_props (vadjustment)->value,
                              child_allocation.width,
                              child_allocation.height);
    }
  if (gtk_bin_get_props (bin)->child && __gtk_widget_get_visible (gtk_bin_get_props (bin)->child))
    __gtk_widget_size_allocate (gtk_bin_get_props (bin)->child, &child_allocation);

  __gtk_adjustment_changed (hadjustment);
  __gtk_adjustment_changed (vadjustment);
  if (hadjustment_value_changed)
    __gtk_adjustment_value_changed (hadjustment);
  if (vadjustment_value_changed)
    __gtk_adjustment_value_changed (vadjustment);
}

static void
gtk_viewport_adjustment_value_changed (GtkAdjustment *adjustment,
				       gpointer       data)
{
  GtkViewport *viewport = GTK_VIEWPORT (data);
  GtkBin *bin = GTK_BIN (data);

  if (gtk_bin_get_props (bin)->child && __gtk_widget_get_visible (gtk_bin_get_props (bin)->child) &&
      __gtk_widget_get_realized (GTK_WIDGET (viewport)))
    {
      GtkAdjustment *hadjustment = __gtk_viewport_get_hadjustment (viewport);
      GtkAdjustment *vadjustment = __gtk_viewport_get_vadjustment (viewport);
      gint old_x, old_y;
      gint new_x, new_y;

      __gdk_window_get_position (gtk_viewport_get_props (viewport)->bin_window, &old_x, &old_y);
      new_x = - gtk_adjustment_get_props (hadjustment)->value;
      new_y = - gtk_adjustment_get_props (vadjustment)->value;

      if (new_x != old_x || new_y != old_y)
	{
	  __gdk_window_move (gtk_viewport_get_props (viewport)->bin_window, new_x, new_y);
	  __gdk_window_process_updates (gtk_viewport_get_props (viewport)->bin_window, TRUE);
	}
    }
}

static void
gtk_viewport_style_set (GtkWidget *widget,
			GtkStyle  *previous_style)
{
   if (__gtk_widget_get_realized (widget) &&
       __gtk_widget_get_has_window (widget))
     {
	GtkViewport *viewport = GTK_VIEWPORT (widget);

	__gtk_style_set_background (gtk_widget_get_props (widget)->style, gtk_viewport_get_props (viewport)->bin_window, GTK_STATE_NORMAL);
	__gtk_style_set_background (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window, gtk_widget_get_props (widget)->state);
     }
}
