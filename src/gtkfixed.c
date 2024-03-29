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
#include <gtkfixed.h>
#include <gtkprivate.h>
#include <gtkintl.h>

enum {
  CHILD_PROP_0,
  CHILD_PROP_X,
  CHILD_PROP_Y
};

static void gtk_fixed_realize       (GtkWidget        *widget);
static void gtk_fixed_size_request  (GtkWidget        *widget,
				     GtkRequisition   *requisition);
static void gtk_fixed_size_allocate (GtkWidget        *widget,
				     GtkAllocation    *allocation);
static void gtk_fixed_add           (GtkContainer     *container,
				     GtkWidget        *widget);
static void gtk_fixed_remove        (GtkContainer     *container,
				     GtkWidget        *widget);
static void gtk_fixed_forall        (GtkContainer     *container,
				     gboolean 	       include_internals,
				     GtkCallback       callback,
				     gpointer          callback_data);
static GType gtk_fixed_child_type   (GtkContainer     *container);

static void gtk_fixed_set_child_property (GtkContainer *container,
                                          GtkWidget    *child,
                                          guint         property_id,
                                          const GValue *value,
                                          GParamSpec   *pspec);
static void gtk_fixed_get_child_property (GtkContainer *container,
                                          GtkWidget    *child,
                                          guint         property_id,
                                          GValue       *value,
                                          GParamSpec   *pspec);

STLWRT_DEFINE_FTYPE_VPARENT (GtkFixed, gtk_fixed, GTK_TYPE_CONTAINER, G_TYPE_FLAG_NONE, ;)

static void
gtk_fixed_class_init (GtkFixedClass *class)
{
  GtkWidgetClass *widget_class;
  GtkContainerClass *container_class;

  widget_class = (GtkWidgetClass*) class;
  container_class = (GtkContainerClass*) class;

  widget_class->realize = gtk_fixed_realize;
  widget_class->size_request = gtk_fixed_size_request;
  widget_class->size_allocate = gtk_fixed_size_allocate;

  container_class->add = gtk_fixed_add;
  container_class->remove = gtk_fixed_remove;
  container_class->forall = gtk_fixed_forall;
  container_class->child_type = gtk_fixed_child_type;

  container_class->set_child_property = gtk_fixed_set_child_property;
  container_class->get_child_property = gtk_fixed_get_child_property;

  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_X,
					      g_param_spec_int ("x",
                                                                P_("X position"),
                                                                P_("X position of child widget"),
                                                                G_MININT,
                                                                G_MAXINT,
                                                                0,
                                                                GTK_PARAM_READWRITE));

  __gtk_container_class_install_child_property (container_class,
					      CHILD_PROP_Y,
					      g_param_spec_int ("y",
                                                                P_("Y position"),
                                                                P_("Y position of child widget"),
                                                                G_MININT,
                                                                G_MAXINT,
                                                                0,
                                                                GTK_PARAM_READWRITE));
}

static GType
gtk_fixed_child_type (GtkContainer     *container)
{
  return GTK_TYPE_WIDGET;
}

static void
gtk_fixed_init (GtkFixed *fixed)
{
  __gtk_widget_set_has_window (GTK_WIDGET (fixed), FALSE);

  gtk_fixed_get_props (fixed)->children = NULL;
}

GtkWidget*
__gtk_fixed_new (void)
{
  return g_object_new (GTK_TYPE_FIXED, NULL);
}

static GtkFixedChild*
get_child (GtkFixed  *fixed,
           GtkWidget *widget)
{
  GList *children;
  
  children = gtk_fixed_get_props (fixed)->children;
  while (children)
    {
      GtkFixedChild *child;
      
      child = children->data;
      children = children->next;

      if (child->widget == widget)
        return child;
    }

  return NULL;
}

void
__gtk_fixed_put (GtkFixed       *fixed,
               GtkWidget      *widget,
               gint            x,
               gint            y)
{
  GtkFixedChild *child_info;

  g_return_if_fail (GTK_IS_FIXED (fixed));
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (__gtk_widget_get_parent (widget) == NULL);

  child_info = g_new (GtkFixedChild, 1);
  child_info->widget = widget;
  child_info->x = x;
  child_info->y = y;

  __gtk_widget_set_parent (widget, GTK_WIDGET (fixed));

  gtk_fixed_get_props (fixed)->children = g_list_append (gtk_fixed_get_props (fixed)->children, child_info);
}

static void
__gtk_fixed_move_internal (GtkFixed       *fixed,
                         GtkWidget      *widget,
                         gboolean        change_x,
                         gint            x,
                         gboolean        change_y,
                         gint            y)
{
  GtkFixedChild *child;
  
  g_return_if_fail (GTK_IS_FIXED (fixed));
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (gtk_widget_get_props (widget)->parent == GTK_WIDGET (fixed));  
  
  child = get_child (fixed, widget);

  g_assert (child);

  __gtk_widget_freeze_child_notify (widget);
  
  if (change_x)
    {
      child->x = x;
      __gtk_widget_child_notify (widget, "x");
    }

  if (change_y)
    {
      child->y = y;
      __gtk_widget_child_notify (widget, "y");
    }

  __gtk_widget_thaw_child_notify (widget);
  
  if (__gtk_widget_get_visible (widget) &&
      __gtk_widget_get_visible (GTK_WIDGET (fixed)))
    __gtk_widget_queue_resize (GTK_WIDGET (fixed));
}

void
__gtk_fixed_move (GtkFixed       *fixed,
                GtkWidget      *widget,
                gint            x,
                gint            y)
{
  __gtk_fixed_move_internal (fixed, widget, TRUE, x, TRUE, y);
}

static void
gtk_fixed_set_child_property (GtkContainer    *container,
                              GtkWidget       *child,
                              guint            property_id,
                              const GValue    *value,
                              GParamSpec      *pspec)
{
  switch (property_id)
    {
    case CHILD_PROP_X:
      __gtk_fixed_move_internal (GTK_FIXED (container),
                               child,
                               TRUE, g_value_get_int (value),
                               FALSE, 0);
      break;
    case CHILD_PROP_Y:
      __gtk_fixed_move_internal (GTK_FIXED (container),
                               child,
                               FALSE, 0,
                               TRUE, g_value_get_int (value));
      break;
    default:
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID (container, property_id, pspec);
      break;
    }
}

static void
gtk_fixed_get_child_property (GtkContainer *container,
                              GtkWidget    *child,
                              guint         property_id,
                              GValue       *value,
                              GParamSpec   *pspec)
{
  GtkFixedChild *fixed_child;

  fixed_child = get_child (GTK_FIXED (container), child);
  
  switch (property_id)
    {
    case CHILD_PROP_X:
      g_value_set_int (value, fixed_child->x);
      break;
    case CHILD_PROP_Y:
      g_value_set_int (value, fixed_child->y);
      break;
    default:
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID (container, property_id, pspec);
      break;
    }
}

static void
gtk_fixed_realize (GtkWidget *widget)
{
  GdkWindowAttr attributes;
  gint attributes_mask;

  if (!__gtk_widget_get_has_window (widget))
    GTK_WIDGET_CLASS (gtk_fixed_parent_class)->realize (widget);
  else
    {
      __gtk_widget_set_realized (widget, TRUE);

      attributes.window_type = GDK_WINDOW_CHILD;
      attributes.x = gtk_widget_get_props (widget)->allocation.x;
      attributes.y = gtk_widget_get_props (widget)->allocation.y;
      attributes.width = gtk_widget_get_props (widget)->allocation.width;
      attributes.height = gtk_widget_get_props (widget)->allocation.height;
      attributes.wclass = GDK_INPUT_OUTPUT;
      attributes.visual = __gtk_widget_get_visual (widget);
      attributes.colormap = __gtk_widget_get_colormap (widget);
      attributes.event_mask = __gtk_widget_get_events (widget);
      attributes.event_mask |= GDK_EXPOSURE_MASK | GDK_BUTTON_PRESS_MASK;
      
      attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;
      
      gtk_widget_get_props (widget)->window = __gdk_window_new (__gtk_widget_get_parent_window (widget), &attributes, 
				       attributes_mask);
      __gdk_window_set_user_data (gtk_widget_get_props (widget)->window, widget);
      
      gtk_widget_get_props (widget)->style = __gtk_style_attach (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window);
      __gtk_style_set_background (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window, GTK_STATE_NORMAL);
    }
}

static void
gtk_fixed_size_request (GtkWidget      *widget,
			GtkRequisition *requisition)
{
  GtkFixed *fixed;  
  GtkFixedChild *child;
  GList *children;
  GtkRequisition child_requisition;

  fixed = GTK_FIXED (widget);
  requisition->width = 0;
  requisition->height = 0;

  children = gtk_fixed_get_props (fixed)->children;
  while (children)
    {
      child = children->data;
      children = children->next;

      if (__gtk_widget_get_visible (child->widget))
	{
          __gtk_widget_size_request (child->widget, &child_requisition);

          requisition->height = MAX (requisition->height,
                                     child->y +
                                     child_requisition.height);
          requisition->width = MAX (requisition->width,
                                    child->x +
                                    child_requisition.width);
	}
    }

  requisition->height += gtk_container_get_props (GTK_CONTAINER (fixed))->border_width * 2;
  requisition->width += gtk_container_get_props (GTK_CONTAINER (fixed))->border_width * 2;
}

static void
gtk_fixed_size_allocate (GtkWidget     *widget,
			 GtkAllocation *allocation)
{
  GtkFixed *fixed;
  GtkFixedChild *child;
  GtkAllocation child_allocation;
  GtkRequisition child_requisition;
  GList *children;
  guint16 border_width;

  fixed = GTK_FIXED (widget);

  gtk_widget_get_props (widget)->allocation = *allocation;

  if (__gtk_widget_get_has_window (widget))
    {
      if (__gtk_widget_get_realized (widget))
	__gdk_window_move_resize (gtk_widget_get_props (widget)->window,
				allocation->x, 
				allocation->y,
				allocation->width, 
				allocation->height);
    }
      
  border_width = gtk_container_get_props (GTK_CONTAINER (fixed))->border_width;
  
  children = gtk_fixed_get_props (fixed)->children;
  while (children)
    {
      child = children->data;
      children = children->next;
      
      if (__gtk_widget_get_visible (child->widget))
	{
	  __gtk_widget_get_child_requisition (child->widget, &child_requisition);
	  child_allocation.x = child->x + border_width;
	  child_allocation.y = child->y + border_width;

	  if (!__gtk_widget_get_has_window (widget))
	    {
	      child_allocation.x += gtk_widget_get_props (widget)->allocation.x;
	      child_allocation.y += gtk_widget_get_props (widget)->allocation.y;
	    }
	  
	  child_allocation.width = child_requisition.width;
	  child_allocation.height = child_requisition.height;
	  __gtk_widget_size_allocate (child->widget, &child_allocation);
	}
    }
}

static void
gtk_fixed_add (GtkContainer *container,
	       GtkWidget    *widget)
{
  __gtk_fixed_put (GTK_FIXED (container), widget, 0, 0);
}

static void
gtk_fixed_remove (GtkContainer *container,
		  GtkWidget    *widget)
{
  GtkFixed *fixed;
  GtkFixedChild *child;
  GtkWidget *widget_container;
  GList *children;

  fixed = GTK_FIXED (container);
  widget_container = GTK_WIDGET (container);

  children = gtk_fixed_get_props (fixed)->children;
  while (children)
    {
      child = children->data;

      if (child->widget == widget)
	{
	  gboolean was_visible = __gtk_widget_get_visible (widget);
	  
	  __gtk_widget_unparent (widget);

	  gtk_fixed_get_props (fixed)->children = g_list_remove_link (gtk_fixed_get_props (fixed)->children, children);
	  g_list_free (children);
	  g_free (child);

	  if (was_visible && __gtk_widget_get_visible (widget_container))
	    __gtk_widget_queue_resize (widget_container);

	  break;
	}

      children = children->next;
    }
}

static void
gtk_fixed_forall (GtkContainer *container,
		  gboolean	include_internals,
		  GtkCallback   callback,
		  gpointer      callback_data)
{
  GtkFixed *fixed = GTK_FIXED (container);
  GtkFixedChild *child;
  GList *children;

  children = gtk_fixed_get_props (fixed)->children;
  while (children)
    {
      child = children->data;
      children = children->next;

      (* callback) (child->widget, callback_data);
    }
}

/**
 * __gtk_fixed_set_has_window:
 * @fixed: a #GtkFixed
 * @has_window: %TRUE if a separate window should be created
 * 
 * Sets whether a #GtkFixed widget is created with a separate
 * #GdkWindow for @gtk_widget_get_props (widget)->window or not. (By default, it will be
 * created with no separate #GdkWindow). This function must be called
 * while the #GtkFixed is not realized, for instance, immediately after the
 * window is created.
 * 
 * This function was added to provide an easy migration path for
 * older applications which may expect #GtkFixed to have a separate window.
 *
 * Deprecated: 2.20: Use __gtk_widget_set_has_window() instead.
 **/
void
__gtk_fixed_set_has_window (GtkFixed *fixed,
			  gboolean  has_window)
{
  g_return_if_fail (GTK_IS_FIXED (fixed));
  g_return_if_fail (!__gtk_widget_get_realized (GTK_WIDGET (fixed)));

  if (has_window != __gtk_widget_get_has_window (GTK_WIDGET (fixed)))
    {
      __gtk_widget_set_has_window (GTK_WIDGET (fixed), has_window);
    }
}

/**
 * __gtk_fixed_get_has_window:
 * @fixed: a #GtkWidget
 * 
 * Gets whether the #GtkFixed has its own #GdkWindow.
 * See __gtk_fixed_set_has_window().
 * 
 * Return value: %TRUE if @fixed has its own window.
 *
 * Deprecated: 2.20: Use __gtk_widget_get_has_window() instead.
 **/
gboolean
__gtk_fixed_get_has_window (GtkFixed *fixed)
{
  g_return_val_if_fail (GTK_IS_FIXED (fixed), FALSE);

  return __gtk_widget_get_has_window (GTK_WIDGET (fixed));
}
