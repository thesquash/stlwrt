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
#include <gtkeventbox.h>
#include <gtkprivate.h>
#include <gtkintl.h>


struct _GtkEventBoxPrivate
{
  gboolean above_child;
  GdkWindow *event_window;
};

enum {
  PROP_0,
  PROP_VISIBLE_WINDOW,
  PROP_ABOVE_CHILD
};




static void     gtk_event_box_realize       (GtkWidget        *widget);
static void     gtk_event_box_unrealize     (GtkWidget        *widget);
static void     gtk_event_box_map           (GtkWidget        *widget);
static void     gtk_event_box_unmap         (GtkWidget        *widget);
static void     gtk_event_box_size_request  (GtkWidget        *widget,
                                             GtkRequisition   *requisition);
static void     gtk_event_box_size_allocate (GtkWidget        *widget,
                                             GtkAllocation    *allocation);
static void     gtk_event_box_paint         (GtkWidget        *widget,
                                             GdkRectangle     *area);
static gboolean gtk_event_box_expose        (GtkWidget        *widget,
                                             GdkEventExpose   *event);
static void     gtk_event_box_set_property  (GObject          *object,
                                             guint             prop_id,
                                             const GValue     *value,
                                             GParamSpec       *pspec);
static void     gtk_event_box_get_property  (GObject          *object,
                                             guint             prop_id,
                                             GValue           *value,
                                             GParamSpec       *pspec);

STLWRT_DEFINE_FTYPE_VPARENT (GtkEventBox, gtk_event_box, GTK_TYPE_BIN, G_TYPE_FLAG_NONE,
                             G_ADD_PRIVATE (GtkEventBox))

static void
gtk_event_box_class_init (GtkEventBoxClass *class)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (class);

  gobject_class->set_property = gtk_event_box_set_property;
  gobject_class->get_property = gtk_event_box_get_property;
  
  widget_class->realize = gtk_event_box_realize;
  widget_class->unrealize = gtk_event_box_unrealize;
  widget_class->map = gtk_event_box_map;
  widget_class->unmap = gtk_event_box_unmap;
  widget_class->size_request = gtk_event_box_size_request;
  widget_class->size_allocate = gtk_event_box_size_allocate;
  widget_class->expose_event = gtk_event_box_expose;

  g_object_class_install_property (gobject_class,
                                   PROP_VISIBLE_WINDOW,
                                   g_param_spec_boolean ("visible-window",
                                                        P_("Visible Window"),
                                                        P_("Whether the event box is visible, as opposed to invisible and only used to trap events."),
                                                        TRUE,
                                                        GTK_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_ABOVE_CHILD,
                                   g_param_spec_boolean ("above-child",
                                                        P_("Above child"),
                                                        P_("Whether the event-trapping window of the eventbox is above the window of the child widget as opposed to below it."),
                                                        FALSE,
                                                        GTK_PARAM_READWRITE));
}

static void
gtk_event_box_init (GtkEventBox *event_box)
{
  GtkEventBoxPrivate *priv;

  __gtk_widget_set_has_window (GTK_WIDGET (event_box), TRUE);
 
  priv = gtk_event_box_get_instance_private (event_box);
  priv->above_child = FALSE;
}

GtkWidget*
__gtk_event_box_new (void)
{
  return g_object_new (GTK_TYPE_EVENT_BOX, NULL);
}

static void 
gtk_event_box_set_property (GObject      *object,
			    guint         prop_id,
			    const GValue *value,
			    GParamSpec   *pspec)
{
  GtkEventBox *event_box;
  
  event_box = GTK_EVENT_BOX (object);
  
  switch (prop_id)
    {
    case PROP_VISIBLE_WINDOW:
      __gtk_event_box_set_visible_window (event_box, g_value_get_boolean (value));
      break;	  
    case PROP_ABOVE_CHILD:
      __gtk_event_box_set_above_child (event_box, g_value_get_boolean (value));
      break;	  
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void 
gtk_event_box_get_property (GObject     *object,
			    guint        prop_id,
			    GValue      *value,
			    GParamSpec  *pspec)
{
  GtkEventBox *event_box;
  
  event_box = GTK_EVENT_BOX (object);
  
  switch (prop_id)
    {
    case PROP_VISIBLE_WINDOW:
      g_value_set_boolean (value, __gtk_event_box_get_visible_window (event_box));
      break;
    case PROP_ABOVE_CHILD:
      g_value_set_boolean (value, __gtk_event_box_get_above_child (event_box));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

/**
 * __gtk_event_box_get_visible_window:
 * @event_box: a #GtkEventBox
 *
 * Returns whether the event box has a visible window.
 * See __gtk_event_box_set_visible_window() for details.
 *
 * Return value: %TRUE if the event box window is visible
 *
 * Since: 2.4
 **/
gboolean
__gtk_event_box_get_visible_window (GtkEventBox *event_box)
{
  g_return_val_if_fail (GTK_IS_EVENT_BOX (event_box), FALSE);

  return __gtk_widget_get_has_window (GTK_WIDGET (event_box));
}

/**
 * __gtk_event_box_set_visible_window:
 * @event_box: a #GtkEventBox
 * @visible_window: boolean value
 *
 * Set whether the event box uses a visible or invisible child
 * window. The default is to use visible windows.
 *
 * In an invisible window event box, the window that the
 * event box creates is a %GDK_INPUT_ONLY window, which 
 * means that it is invisible and only serves to receive
 * events.
 * 
 * A visible window event box creates a visible (%GDK_INPUT_OUTPUT)
 * window that acts as the parent window for all the widgets  
 * contained in the event box.
 * 
 * You should generally make your event box invisible if
 * you just want to trap events. Creating a visible window
 * may cause artifacts that are visible to the user, especially
 * if the user is using a theme with gradients or pixmaps.
 * 
 * The main reason to create a non input-only event box is if
 * you want to set the background to a different color or
 * draw on it.
 *
 * <note><para>
 * There is one unexpected issue for an invisible event box that has its
 * window below the child. (See __gtk_event_box_set_above_child().)
 * Since the input-only window is not an ancestor window of any windows
 * that descendent widgets of the event box create, events on these 
 * windows aren't propagated up by the windowing system, but only by GTK+.
 * The practical effect of this is if an event isn't in the event
 * mask for the descendant window (see __gtk_widget_add_events()),  
 * it won't be received by the event box. 
 * </para><para>
 * This problem doesn't occur for visible event boxes, because in
 * that case, the event box window is actually the ancestor of the
 * descendant windows, not just at the same place on the screen.
 * </para></note>
 * 
 * Since: 2.4
 **/
void
__gtk_event_box_set_visible_window (GtkEventBox *event_box,
				  gboolean visible_window)
{
  GtkWidget *widget;

  g_return_if_fail (GTK_IS_EVENT_BOX (event_box));

  widget = GTK_WIDGET (event_box);

  visible_window = visible_window != FALSE;

  if (visible_window != __gtk_widget_get_has_window (widget))
    {
      if (__gtk_widget_get_realized (widget))
	{
	  gboolean visible = __gtk_widget_get_visible (widget);

	  if (visible)
	    __gtk_widget_hide (widget);

	  __gtk_widget_unrealize (widget);

          __gtk_widget_set_has_window (widget, visible_window);

	  __gtk_widget_realize (widget);

	  if (visible)
	    __gtk_widget_show (widget);
	}
      else
	{
          __gtk_widget_set_has_window (widget, visible_window);
	}

      if (__gtk_widget_get_visible (widget))
	__gtk_widget_queue_resize (widget);
      
      g_object_notify (G_OBJECT (event_box), "visible-window");
    }
}

/**
 * __gtk_event_box_get_above_child:
 * @event_box: a #GtkEventBox
 *
 * Returns whether the event box window is above or below the
 * windows of its child. See __gtk_event_box_set_above_child() for
 * details.
 *
 * Return value: %TRUE if the event box window is above the window
 * of its child.
 *
 * Since: 2.4
 **/
gboolean
__gtk_event_box_get_above_child (GtkEventBox *event_box)
{
  GtkEventBoxPrivate *priv;

  g_return_val_if_fail (GTK_IS_EVENT_BOX (event_box), FALSE);

  priv = gtk_event_box_get_instance_private (event_box);

  return priv->above_child;
}

/**
 * __gtk_event_box_set_above_child:
 * @event_box: a #GtkEventBox
 * @above_child: %TRUE if the event box window is above the windows of its child
 *
 * Set whether the event box window is positioned above the windows of its child,
 * as opposed to below it. If the window is above, all events inside the
 * event box will go to the event box. If the window is below, events
 * in windows of child widgets will first got to that widget, and then
 * to its parents.
 *
 * The default is to keep the window below the child.
 * 
 * Since: 2.4
 **/
void
__gtk_event_box_set_above_child (GtkEventBox *event_box,
			       gboolean above_child)
{
  GtkWidget *widget;
  GtkEventBoxPrivate *priv;

  g_return_if_fail (GTK_IS_EVENT_BOX (event_box));

  widget = GTK_WIDGET (event_box);
  priv = gtk_event_box_get_instance_private (event_box);

  above_child = above_child != FALSE;

  if (priv->above_child != above_child)
    {
      priv->above_child = above_child;

      if (__gtk_widget_get_realized (widget))
	{
	  if (!__gtk_widget_get_has_window (widget))
	    {
	      if (above_child)
		__gdk_window_raise (priv->event_window);
	      else
		__gdk_window_lower (priv->event_window);
	    }
	  else
	    {
	      gboolean visible = __gtk_widget_get_visible (widget);

	      if (visible)
		__gtk_widget_hide (widget);
	      
	      __gtk_widget_unrealize (widget);
	      
	      __gtk_widget_realize (widget);
	      
	      if (visible)
		__gtk_widget_show (widget);
	    }
	}

      if (__gtk_widget_get_visible (widget))
	__gtk_widget_queue_resize (widget);
      
      g_object_notify (G_OBJECT (event_box), "above-child");
    }
}


static void
gtk_event_box_realize (GtkWidget *widget)
{
  GdkWindowAttr attributes;
  gint attributes_mask;
  gint border_width;
  GtkEventBoxPrivate *priv;
  gboolean visible_window;

  __gtk_widget_set_realized (widget, TRUE);

  border_width = gtk_container_get_props (GTK_CONTAINER (widget))->border_width;
  
  attributes.x = gtk_widget_get_props (widget)->allocation.x + border_width;
  attributes.y = gtk_widget_get_props (widget)->allocation.y + border_width;
  attributes.width = gtk_widget_get_props (widget)->allocation.width - 2*border_width;
  attributes.height = gtk_widget_get_props (widget)->allocation.height - 2*border_width;
  attributes.window_type = GDK_WINDOW_CHILD;
  attributes.event_mask = __gtk_widget_get_events (widget)
			| GDK_BUTTON_MOTION_MASK
			| GDK_BUTTON_PRESS_MASK
			| GDK_BUTTON_RELEASE_MASK
			| GDK_EXPOSURE_MASK
			| GDK_ENTER_NOTIFY_MASK
			| GDK_LEAVE_NOTIFY_MASK;

  priv = gtk_event_box_get_instance_private (widget);

  visible_window = __gtk_widget_get_has_window (widget);
  if (visible_window)
    {
      attributes.visual = __gtk_widget_get_visual (widget);
      attributes.colormap = __gtk_widget_get_colormap (widget);
      attributes.wclass = GDK_INPUT_OUTPUT;
      
      attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
      
      gtk_widget_get_props (widget)->window = __gdk_window_new (__gtk_widget_get_parent_window (widget),
				       &attributes, attributes_mask);
      __gdk_window_set_user_data (gtk_widget_get_props (widget)->window, widget);
    }
  else
    {
      gtk_widget_get_props (widget)->window = __gtk_widget_get_parent_window (widget);
      g_object_ref (gtk_widget_get_props (widget)->window);
    }

  if (!visible_window || priv->above_child)
    {
      attributes.wclass = GDK_INPUT_ONLY;
      if (!visible_window)
        attributes_mask = GDK_WA_X | GDK_WA_Y;
      else
        attributes_mask = 0;

      priv->event_window = __gdk_window_new (gtk_widget_get_props (widget)->window,
					   &attributes, attributes_mask);
      __gdk_window_set_user_data (priv->event_window, widget);
    }


  gtk_widget_get_props (widget)->style = __gtk_style_attach (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window);
  
  if (visible_window)
    __gtk_style_set_background (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window, GTK_STATE_NORMAL);
}

static void
gtk_event_box_unrealize (GtkWidget *widget)
{
  GtkEventBoxPrivate *priv;
  
  priv = gtk_event_box_get_instance_private (widget);
  
  if (priv->event_window != NULL)
    {
      __gdk_window_set_user_data (priv->event_window, NULL);
      __gdk_window_destroy (priv->event_window);
      priv->event_window = NULL;
    }

  GTK_WIDGET_CLASS (gtk_event_box_parent_class)->unrealize (widget);
}

static void
gtk_event_box_map (GtkWidget *widget)
{
  GtkEventBoxPrivate *priv;

  priv = gtk_event_box_get_instance_private (widget);

  if (priv->event_window != NULL && !priv->above_child)
    __gdk_window_show (priv->event_window);

  GTK_WIDGET_CLASS (gtk_event_box_parent_class)->map (widget);

  if (priv->event_window != NULL && priv->above_child)
    __gdk_window_show (priv->event_window);
}

static void
gtk_event_box_unmap (GtkWidget *widget)
{
  GtkEventBoxPrivate *priv;

  priv = gtk_event_box_get_instance_private (widget);

  if (priv->event_window != NULL)
    __gdk_window_hide (priv->event_window);

  GTK_WIDGET_CLASS (gtk_event_box_parent_class)->unmap (widget);
}



static void
gtk_event_box_size_request (GtkWidget      *widget,
			    GtkRequisition *requisition)
{
  GtkBin *bin = GTK_BIN (widget);

  requisition->width = gtk_container_get_props (GTK_CONTAINER (widget))->border_width * 2;
  requisition->height = gtk_container_get_props (GTK_CONTAINER (widget))->border_width * 2;

  if (gtk_bin_get_props (bin)->child && __gtk_widget_get_visible (gtk_bin_get_props (bin)->child))
    {
      GtkRequisition child_requisition;
      
      __gtk_widget_size_request (gtk_bin_get_props (bin)->child, &child_requisition);

      requisition->width += child_requisition.width;
      requisition->height += child_requisition.height;
    }
}

static void
gtk_event_box_size_allocate (GtkWidget     *widget,
			     GtkAllocation *allocation)
{
  GtkBin *bin;
  GtkAllocation child_allocation;
  GtkEventBoxPrivate *priv;
  
  gtk_widget_get_props (widget)->allocation = *allocation;
  bin = GTK_BIN (widget);
  
  if (!__gtk_widget_get_has_window (widget))
    {
      child_allocation.x = allocation->x + gtk_container_get_props (GTK_CONTAINER (widget))->border_width;
      child_allocation.y = allocation->y + gtk_container_get_props (GTK_CONTAINER (widget))->border_width;
    }
  else
    {
      child_allocation.x = 0;
      child_allocation.y = 0;
    }
  child_allocation.width = MAX (allocation->width - gtk_container_get_props (GTK_CONTAINER (widget))->border_width * 2, 0);
  child_allocation.height = MAX (allocation->height - gtk_container_get_props (GTK_CONTAINER (widget))->border_width * 2, 0);

  if (__gtk_widget_get_realized (widget))
    {
      priv = gtk_event_box_get_instance_private (widget);

      if (priv->event_window != NULL)
	__gdk_window_move_resize (priv->event_window,
				child_allocation.x,
				child_allocation.y,
				child_allocation.width,
				child_allocation.height);
      
      if (__gtk_widget_get_has_window (widget))
	__gdk_window_move_resize (gtk_widget_get_props (widget)->window,
				allocation->x + gtk_container_get_props (GTK_CONTAINER (widget))->border_width,
				allocation->y + gtk_container_get_props (GTK_CONTAINER (widget))->border_width,
				child_allocation.width,
				child_allocation.height);
    }
  
  if (gtk_bin_get_props (bin)->child)
    __gtk_widget_size_allocate (gtk_bin_get_props (bin)->child, &child_allocation);
}

static void
gtk_event_box_paint (GtkWidget    *widget,
		     GdkRectangle *area)
{
  if (!__gtk_widget_get_app_paintable (widget))
    __gtk_paint_flat_box (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window,
			gtk_widget_get_props (widget)->state, GTK_SHADOW_NONE,
			area, widget, "eventbox",
			0, 0, -1, -1);
}

static gboolean
gtk_event_box_expose (GtkWidget      *widget,
		     GdkEventExpose *event)
{
  if (__gtk_widget_is_drawable (widget))
    {
      if (__gtk_widget_get_has_window (widget))
	gtk_event_box_paint (widget, &event->area);

      GTK_WIDGET_CLASS (gtk_event_box_parent_class)->expose_event (widget, event);
    }

  return FALSE;
}
