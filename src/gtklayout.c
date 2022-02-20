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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 *
 * GtkLayout: Widget for scrolling of arbitrary-sized areas.
 *
 * Copyright Owen Taylor, 1998
 */



#include "config.h"

#include <stlwrt.h>
#include <gdkconfig.h>

#include <gtklayout.h>
#include <gtkprivate.h>
#include <gtkintl.h>



typedef struct _GtkLayoutChild   GtkLayoutChild;

struct _GtkLayoutChild {
  GtkWidget *widget;
  gint x;
  gint y;
};

enum {
   PROP_0,
   PROP_HADJUSTMENT,
   PROP_VADJUSTMENT,
   PROP_WIDTH,
   PROP_HEIGHT
};

enum {
  CHILD_PROP_0,
  CHILD_PROP_X,
  CHILD_PROP_Y
};

static void gtk_layout_get_property       (GObject        *object,
                                           guint           prop_id,
                                           GValue         *value,
                                           GParamSpec     *pspec);
static void gtk_layout_set_property       (GObject        *object,
                                           guint           prop_id,
                                           const GValue   *value,
                                           GParamSpec     *pspec);
static GObject *gtk_layout_constructor    (GType                  type,
					   guint                  n_properties,
					   GObjectConstructParam *properties);
static void gtk_layout_finalize           (GObject        *object);
static void gtk_layout_realize            (GtkWidget      *widget);
static void gtk_layout_unrealize          (GtkWidget      *widget);
static void gtk_layout_map                (GtkWidget      *widget);
static void gtk_layout_size_request       (GtkWidget      *widget,
                                           GtkRequisition *requisition);
static void gtk_layout_size_allocate      (GtkWidget      *widget,
                                           GtkAllocation  *allocation);
static gint gtk_layout_expose             (GtkWidget      *widget,
                                           GdkEventExpose *event);
static void gtk_layout_add                (GtkContainer   *container,
					   GtkWidget      *widget);
static void gtk_layout_remove             (GtkContainer   *container,
                                           GtkWidget      *widget);
static void gtk_layout_forall             (GtkContainer   *container,
                                           gboolean        include_internals,
                                           GtkCallback     callback,
                                           gpointer        callback_data);
static void gtk_layout_set_adjustments    (GtkLayout      *layout,
                                           GtkAdjustment  *hadj,
                                           GtkAdjustment  *vadj);
static void gtk_layout_set_child_property (GtkContainer   *container,
                                           GtkWidget      *child,
                                           guint           property_id,
                                           const GValue   *value,
                                           GParamSpec     *pspec);
static void gtk_layout_get_child_property (GtkContainer   *container,
                                           GtkWidget      *child,
                                           guint           property_id,
                                           GValue         *value,
                                           GParamSpec     *pspec);
static void gtk_layout_allocate_child     (GtkLayout      *layout,
                                           GtkLayoutChild *child);
static void gtk_layout_adjustment_changed (GtkAdjustment  *adjustment,
                                           GtkLayout      *layout);
static void gtk_layout_style_set          (GtkWidget      *widget,
					   GtkStyle       *old_style);

static void gtk_layout_set_adjustment_upper (GtkAdjustment *adj,
					     gdouble        upper,
					     gboolean       always_emit_changed);

STLWRT_DEFINE_VTYPE (GtkLayout, gtk_layout, GTK_TYPE_CONTAINER, G_TYPE_FLAG_NONE, ;)

/* Public interface
 */
/**
 * __gtk_layout_new:
 * @hadjustment: (allow-none): horizontal scroll adjustment, or %NULL
 * @vadjustment: (allow-none): vertical scroll adjustment, or %NULL
 * 
 * Creates a new #GtkLayout. Unless you have a specific adjustment
 * you'd like the layout to use for scrolling, pass %NULL for
 * @hadjustment and @vadjustment.
 * 
 * Return value: a new #GtkLayout
 **/
  
GtkWidget*    
__gtk_layout_new (GtkAdjustment *hadjustment,
		GtkAdjustment *vadjustment)
{
  GtkLayout *layout;

  layout = g_object_new (GTK_TYPE_LAYOUT,
			 "hadjustment", hadjustment,
			 "vadjustment", vadjustment,
			 NULL);

  return GTK_WIDGET (layout);
}

/**
 * __gtk_layout_get_bin_window:
 * @layout: a #GtkLayout
 *
 * Retrieve the bin window of the layout used for drawing operations.
 *
 * Return value: (transfer none): a #GdkWindow
 *
 * Since: 2.14
 **/
GdkWindow*
__gtk_layout_get_bin_window (GtkLayout *layout)
{
  g_return_val_if_fail (GTK_IS_LAYOUT (layout), NULL);

  return gtk_layout_get_props (layout)->bin_window;
}

/**
 * __gtk_layout_get_hadjustment:
 * @layout: a #GtkLayout
 *
 * This function should only be called after the layout has been
 * placed in a #GtkScrolledWindow or otherwise configured for
 * scrolling. It returns the #GtkAdjustment used for communication
 * between the horizontal scrollbar and @layout.
 *
 * See #GtkScrolledWindow, #GtkScrollbar, #GtkAdjustment for details.
 *
 * Return value: (transfer none): horizontal scroll adjustment
 **/
GtkAdjustment*
__gtk_layout_get_hadjustment (GtkLayout *layout)
{
  g_return_val_if_fail (GTK_IS_LAYOUT (layout), NULL);

  return gtk_layout_get_props (layout)->hadjustment;
}
/**
 * __gtk_layout_get_vadjustment:
 * @layout: a #GtkLayout
 *
 * This function should only be called after the layout has been
 * placed in a #GtkScrolledWindow or otherwise configured for
 * scrolling. It returns the #GtkAdjustment used for communication
 * between the vertical scrollbar and @layout.
 *
 * See #GtkScrolledWindow, #GtkScrollbar, #GtkAdjustment for details.
 *
 * Return value: (transfer none): vertical scroll adjustment
 **/
GtkAdjustment*
__gtk_layout_get_vadjustment (GtkLayout *layout)
{
  g_return_val_if_fail (GTK_IS_LAYOUT (layout), NULL);

  return gtk_layout_get_props (layout)->vadjustment;
}

static GtkAdjustment *
new_default_adjustment (void)
{
  return GTK_ADJUSTMENT (__gtk_adjustment_new (0.0, 0.0, 0.0, 0.0, 0.0, 0.0));
}

static void           
gtk_layout_set_adjustments (GtkLayout     *layout,
			    GtkAdjustment *hadj,
			    GtkAdjustment *vadj)
{
  gboolean need_adjust = FALSE;

  g_return_if_fail (GTK_IS_LAYOUT (layout));

  if (hadj)
    g_return_if_fail (GTK_IS_ADJUSTMENT (hadj));
  else if (gtk_layout_get_props (layout)->hadjustment)
    hadj = new_default_adjustment ();
  if (vadj)
    g_return_if_fail (GTK_IS_ADJUSTMENT (vadj));
  else if (gtk_layout_get_props (layout)->vadjustment)
    vadj = new_default_adjustment ();
  
  if (gtk_layout_get_props (layout)->hadjustment && (gtk_layout_get_props (layout)->hadjustment != hadj))
    {
      g_signal_handlers_disconnect_by_func (gtk_layout_get_props (layout)->hadjustment,
					    gtk_layout_adjustment_changed,
					    layout);
      g_object_unref (gtk_layout_get_props (layout)->hadjustment);
    }
  
  if (gtk_layout_get_props (layout)->vadjustment && (gtk_layout_get_props (layout)->vadjustment != vadj))
    {
      g_signal_handlers_disconnect_by_func (gtk_layout_get_props (layout)->vadjustment,
					    gtk_layout_adjustment_changed,
					    layout);
      g_object_unref (gtk_layout_get_props (layout)->vadjustment);
    }
  
  if (gtk_layout_get_props (layout)->hadjustment != hadj)
    {
      gtk_layout_get_props (layout)->hadjustment = hadj;
      g_object_ref_sink (gtk_layout_get_props (layout)->hadjustment);
      gtk_layout_set_adjustment_upper (gtk_layout_get_props (layout)->hadjustment, gtk_layout_get_props (layout)->width, FALSE);
      
      g_signal_connect (gtk_layout_get_props (layout)->hadjustment, "value-changed",
			G_CALLBACK (gtk_layout_adjustment_changed),
			layout);
      need_adjust = TRUE;
    }
  
  if (gtk_layout_get_props (layout)->vadjustment != vadj)
    {
      gtk_layout_get_props (layout)->vadjustment = vadj;
      g_object_ref_sink (gtk_layout_get_props (layout)->vadjustment);
      gtk_layout_set_adjustment_upper (gtk_layout_get_props (layout)->vadjustment, gtk_layout_get_props (layout)->height, FALSE);
      
      g_signal_connect (gtk_layout_get_props (layout)->vadjustment, "value-changed",
			G_CALLBACK (gtk_layout_adjustment_changed),
			layout);
      need_adjust = TRUE;
    }

  /* vadj or hadj can be NULL while constructing; don't emit a signal
     then */
  if (need_adjust && vadj && hadj)
    gtk_layout_adjustment_changed (NULL, layout);
}

static void
gtk_layout_finalize (GObject *object)
{
  GtkLayout *layout = GTK_LAYOUT (object);

  g_object_unref (gtk_layout_get_props (layout)->hadjustment);
  g_object_unref (gtk_layout_get_props (layout)->vadjustment);

  G_OBJECT_CLASS (gtk_layout_parent_class)->finalize (object);
}

/**
 * __gtk_layout_set_hadjustment:
 * @layout: a #GtkLayout
 * @adjustment: (allow-none): new scroll adjustment
 *
 * Sets the horizontal scroll adjustment for the layout.
 *
 * See #GtkScrolledWindow, #GtkScrollbar, #GtkAdjustment for details.
 * 
 **/
void           
__gtk_layout_set_hadjustment (GtkLayout     *layout,
			    GtkAdjustment *adjustment)
{
  g_return_if_fail (GTK_IS_LAYOUT (layout));

  gtk_layout_set_adjustments (layout, adjustment, gtk_layout_get_props (layout)->vadjustment);
  g_object_notify (G_OBJECT (layout), "hadjustment");
}
 
/**
 * __gtk_layout_set_vadjustment:
 * @layout: a #GtkLayout
 * @adjustment: (allow-none): new scroll adjustment
 *
 * Sets the vertical scroll adjustment for the layout.
 *
 * See #GtkScrolledWindow, #GtkScrollbar, #GtkAdjustment for details.
 * 
 **/
void           
__gtk_layout_set_vadjustment (GtkLayout     *layout,
			    GtkAdjustment *adjustment)
{
  g_return_if_fail (GTK_IS_LAYOUT (layout));
  
  gtk_layout_set_adjustments (layout, gtk_layout_get_props (layout)->hadjustment, adjustment);
  g_object_notify (G_OBJECT (layout), "vadjustment");
}

static GtkLayoutChild*
get_child (GtkLayout  *layout,
           GtkWidget  *widget)
{
  GList *children;
  
  children = gtk_layout_get_props (layout)->children;
  while (children)
    {
      GtkLayoutChild *child;
      
      child = children->data;
      children = children->next;

      if (child->widget == widget)
        return child;
    }

  return NULL;
}

/**
 * __gtk_layout_put:
 * @layout: a #GtkLayout
 * @child_widget: child widget
 * @x: X position of child widget
 * @y: Y position of child widget
 *
 * Adds @child_widget to @layout, at position (@x,@y).
 * @layout becomes the new parent container of @child_widget.
 * 
 **/
void           
__gtk_layout_put (GtkLayout     *layout, 
		GtkWidget     *child_widget, 
		gint           x, 
		gint           y)
{
  GtkLayoutChild *child;

  g_return_if_fail (GTK_IS_LAYOUT (layout));
  g_return_if_fail (GTK_IS_WIDGET (child_widget));
  
  child = g_new (GtkLayoutChild, 1);

  child->widget = child_widget;
  child->x = x;
  child->y = y;

  gtk_layout_get_props (layout)->children = g_list_append (gtk_layout_get_props (layout)->children, child);
  
  if (__gtk_widget_get_realized (GTK_WIDGET (layout)))
    __gtk_widget_set_parent_window (child->widget, gtk_layout_get_props (layout)->bin_window);

  __gtk_widget_set_parent (child_widget, GTK_WIDGET (layout));
}

static void
__gtk_layout_move_internal (GtkLayout       *layout,
                          GtkWidget       *widget,
                          gboolean         change_x,
                          gint             x,
                          gboolean         change_y,
                          gint             y)
{
  GtkLayoutChild *child;

  child = get_child (layout, widget);

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
      __gtk_widget_get_visible (GTK_WIDGET (layout)))
    __gtk_widget_queue_resize (widget);
}

/**
 * __gtk_layout_move:
 * @layout: a #GtkLayout
 * @child_widget: a current child of @layout
 * @x: X position to move to
 * @y: Y position to move to
 *
 * Moves a current child of @layout to a new position.
 * 
 **/
void           
__gtk_layout_move (GtkLayout     *layout, 
		 GtkWidget     *child_widget, 
		 gint           x, 
		 gint           y)
{
  g_return_if_fail (GTK_IS_LAYOUT (layout));
  g_return_if_fail (GTK_IS_WIDGET (child_widget));
  g_return_if_fail (gtk_widget_get_props (child_widget)->parent == GTK_WIDGET (layout));  

  __gtk_layout_move_internal (layout, child_widget, TRUE, x, TRUE, y);
}

static void
gtk_layout_set_adjustment_upper (GtkAdjustment *adj,
				 gdouble        upper,
				 gboolean       always_emit_changed)
{
  gboolean changed = FALSE;
  gboolean value_changed = FALSE;
  
  gdouble min = MAX (0., upper - gtk_adjustment_get_props (adj)->page_size);

  if (upper != gtk_adjustment_get_props (adj)->upper)
    {
      gtk_adjustment_get_props (adj)->upper = upper;
      changed = TRUE;
    }
      
  if (gtk_adjustment_get_props (adj)->value > min)
    {
      gtk_adjustment_get_props (adj)->value = min;
      value_changed = TRUE;
    }
  
  if (changed || always_emit_changed)
    __gtk_adjustment_changed (adj);
  if (value_changed)
    __gtk_adjustment_value_changed (adj);
}

/**
 * __gtk_layout_set_size:
 * @layout: a #GtkLayout
 * @width: width of entire scrollable area
 * @height: height of entire scrollable area
 *
 * Sets the size of the scrollable area of the layout.
 * 
 **/
void
__gtk_layout_set_size (GtkLayout     *layout, 
		     guint          width,
		     guint          height)
{
  GtkWidget *widget;
  
  g_return_if_fail (GTK_IS_LAYOUT (layout));
  
  widget = GTK_WIDGET (layout);
  
  g_object_freeze_notify (G_OBJECT (layout));
  if (width != gtk_layout_get_props (layout)->width)
     {
	gtk_layout_get_props (layout)->width = width;
	g_object_notify (G_OBJECT (layout), "width");
     }
  if (height != gtk_layout_get_props (layout)->height)
     {
	gtk_layout_get_props (layout)->height = height;
	g_object_notify (G_OBJECT (layout), "height");
     }
  g_object_thaw_notify (G_OBJECT (layout));

  if (gtk_layout_get_props (layout)->hadjustment)
    gtk_layout_set_adjustment_upper (gtk_layout_get_props (layout)->hadjustment, gtk_layout_get_props (layout)->width, FALSE);
  if (gtk_layout_get_props (layout)->vadjustment)
    gtk_layout_set_adjustment_upper (gtk_layout_get_props (layout)->vadjustment, gtk_layout_get_props (layout)->height, FALSE);

  if (__gtk_widget_get_realized (widget))
    {
      width = MAX (width, gtk_widget_get_props (widget)->allocation.width);
      height = MAX (height, gtk_widget_get_props (widget)->allocation.height);
      __gdk_window_resize (gtk_layout_get_props (layout)->bin_window, width, height);
    }
}

/**
 * __gtk_layout_get_size:
 * @layout: a #GtkLayout
 * @width: (out) (allow-none): location to store the width set on
 *     @layout, or %NULL
 * @height: (out) (allow-none): location to store the height set on
 *     @layout, or %NULL
 *
 * Gets the size that has been set on the layout, and that determines
 * the total extents of the layout's scrollbar area. See
 * __gtk_layout_set_size ().
 **/
void
__gtk_layout_get_size (GtkLayout *layout,
		     guint     *width,
		     guint     *height)
{
  g_return_if_fail (GTK_IS_LAYOUT (layout));

  if (width)
    *width = gtk_layout_get_props (layout)->width;
  if (height)
    *height = gtk_layout_get_props (layout)->height;
}

/**
 * __gtk_layout_freeze:
 * @layout: a #GtkLayout
 * 
 * This is a deprecated function, it doesn't do anything useful.
 **/
void
__gtk_layout_freeze (GtkLayout *layout)
{
  g_return_if_fail (GTK_IS_LAYOUT (layout));

  gtk_layout_get_props (layout)->freeze_count++;
}

/**
 * __gtk_layout_thaw:
 * @layout: a #GtkLayout
 * 
 * This is a deprecated function, it doesn't do anything useful.
 **/
void
__gtk_layout_thaw (GtkLayout *layout)
{
  g_return_if_fail (GTK_IS_LAYOUT (layout));

  if (gtk_layout_get_props (layout)->freeze_count)
    {
      if (!(--gtk_layout_get_props (layout)->freeze_count))
	{
	  __gtk_widget_queue_draw (GTK_WIDGET (layout));
	  __gdk_window_process_updates (gtk_widget_get_props (GTK_WIDGET (layout))->window, TRUE);
	}
    }

}

/* Basic Object handling procedures
 */
static void
gtk_layout_class_init (GtkLayoutClass *class)
{
  GObjectClass *gobject_class;
  GtkWidgetClass *widget_class;
  GtkContainerClass *container_class;

  gobject_class = (GObjectClass*) class;
  widget_class = (GtkWidgetClass*) class;
  container_class = (GtkContainerClass*) class;

  gobject_class->set_property = gtk_layout_set_property;
  gobject_class->get_property = gtk_layout_get_property;
  gobject_class->finalize = gtk_layout_finalize;
  gobject_class->constructor = gtk_layout_constructor;

  container_class->set_child_property = gtk_layout_set_child_property;
  container_class->get_child_property = gtk_layout_get_child_property;

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
  
  g_object_class_install_property (gobject_class,
				   PROP_HADJUSTMENT,
				   g_param_spec_object ("hadjustment",
							P_("Horizontal adjustment"),
							P_("The GtkAdjustment for the horizontal position"),
							GTK_TYPE_ADJUSTMENT,
							GTK_PARAM_READWRITE));
  
  g_object_class_install_property (gobject_class,
				   PROP_VADJUSTMENT,
				   g_param_spec_object ("vadjustment",
							P_("Vertical adjustment"),
							P_("The GtkAdjustment for the vertical position"),
							GTK_TYPE_ADJUSTMENT,
							GTK_PARAM_READWRITE));

  g_object_class_install_property (gobject_class,
				   PROP_WIDTH,
				   g_param_spec_uint ("width",
						     P_("Width"),
						     P_("The width of the layout"),
						     0,
						     G_MAXINT,
						     100,
						     GTK_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
				   PROP_HEIGHT,
				   g_param_spec_uint ("height",
						     P_("Height"),
						     P_("The height of the layout"),
						     0,
						     G_MAXINT,
						     100,
						     GTK_PARAM_READWRITE));
  widget_class->realize = gtk_layout_realize;
  widget_class->unrealize = gtk_layout_unrealize;
  widget_class->map = gtk_layout_map;
  widget_class->size_request = gtk_layout_size_request;
  widget_class->size_allocate = gtk_layout_size_allocate;
  widget_class->expose_event = gtk_layout_expose;
  widget_class->style_set = gtk_layout_style_set;

  container_class->add = gtk_layout_add;
  container_class->remove = gtk_layout_remove;
  container_class->forall = gtk_layout_forall;

  class->set_scroll_adjustments = gtk_layout_set_adjustments;

  /**
   * GtkLayout::set-scroll-adjustments
   * @horizontal: the horizontal #GtkAdjustment
   * @vertical: the vertical #GtkAdjustment
   *
   * Set the scroll adjustments for the layout. Usually scrolled containers
   * like #GtkScrolledWindow will emit this signal to connect two instances
   * of #GtkScrollbar to the scroll directions of the #GtkLayout.
   */
  widget_class->set_scroll_adjustments_signal =
    g_signal_new (I_("set-scroll-adjustments"),
		  G_OBJECT_CLASS_TYPE (gobject_class),
		  G_SIGNAL_RUN_LAST | G_SIGNAL_ACTION,
		  G_STRUCT_OFFSET (GtkLayoutClass, set_scroll_adjustments),
		  NULL, NULL,
		  NULL,
		  G_TYPE_NONE, 2,
		  GTK_TYPE_ADJUSTMENT,
		  GTK_TYPE_ADJUSTMENT);
}

static void
gtk_layout_get_property (GObject     *object,
			 guint        prop_id,
			 GValue      *value,
			 GParamSpec  *pspec)
{
  GtkLayout *layout = GTK_LAYOUT (object);
  
  switch (prop_id)
    {
    case PROP_HADJUSTMENT:
      g_value_set_object (value, gtk_layout_get_props (layout)->hadjustment);
      break;
    case PROP_VADJUSTMENT:
      g_value_set_object (value, gtk_layout_get_props (layout)->vadjustment);
      break;
    case PROP_WIDTH:
      g_value_set_uint (value, gtk_layout_get_props (layout)->width);
      break;
    case PROP_HEIGHT:
      g_value_set_uint (value, gtk_layout_get_props (layout)->height);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_layout_set_property (GObject      *object,
			 guint         prop_id,
			 const GValue *value,
			 GParamSpec   *pspec)
{
  GtkLayout *layout = GTK_LAYOUT (object);
  
  switch (prop_id)
    {
    case PROP_HADJUSTMENT:
      __gtk_layout_set_hadjustment (layout, 
				  (GtkAdjustment*) g_value_get_object (value));
      break;
    case PROP_VADJUSTMENT:
      __gtk_layout_set_vadjustment (layout, 
				  (GtkAdjustment*) g_value_get_object (value));
      break;
    case PROP_WIDTH:
      __gtk_layout_set_size (layout, g_value_get_uint (value),
			   gtk_layout_get_props (layout)->height);
      break;
    case PROP_HEIGHT:
      __gtk_layout_set_size (layout, gtk_layout_get_props (layout)->width,
			   g_value_get_uint (value));
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_layout_set_child_property (GtkContainer    *container,
                               GtkWidget       *child,
                               guint            property_id,
                               const GValue    *value,
                               GParamSpec      *pspec)
{
  switch (property_id)
    {
    case CHILD_PROP_X:
      __gtk_layout_move_internal (GTK_LAYOUT (container),
                                child,
                                TRUE, g_value_get_int (value),
                                FALSE, 0);
      break;
    case CHILD_PROP_Y:
      __gtk_layout_move_internal (GTK_LAYOUT (container),
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
gtk_layout_get_child_property (GtkContainer *container,
                               GtkWidget    *child,
                               guint         property_id,
                               GValue       *value,
                               GParamSpec   *pspec)
{
  GtkLayoutChild *layout_child;

  layout_child = get_child (GTK_LAYOUT (container), child);
  
  switch (property_id)
    {
    case CHILD_PROP_X:
      g_value_set_int (value, layout_child->x);
      break;
    case CHILD_PROP_Y:
      g_value_set_int (value, layout_child->y);
      break;
    default:
      GTK_CONTAINER_WARN_INVALID_CHILD_PROPERTY_ID (container, property_id, pspec);
      break;
    }
}

static void
gtk_layout_init (GtkLayout *layout)
{
  gtk_layout_get_props (layout)->children = NULL;

  gtk_layout_get_props (layout)->width = 100;
  gtk_layout_get_props (layout)->height = 100;

  gtk_layout_get_props (layout)->hadjustment = NULL;
  gtk_layout_get_props (layout)->vadjustment = NULL;

  gtk_layout_get_props (layout)->bin_window = NULL;

  gtk_layout_get_props (layout)->scroll_x = 0;
  gtk_layout_get_props (layout)->scroll_y = 0;
  gtk_layout_get_props (layout)->visibility = GDK_VISIBILITY_PARTIAL;

  gtk_layout_get_props (layout)->freeze_count = 0;
}

static GObject *
gtk_layout_constructor (GType                  type,
			guint                  n_properties,
			GObjectConstructParam *properties)
{
  GtkLayout *layout;
  GObject *object;
  GtkAdjustment *hadj, *vadj;
  
  object = G_OBJECT_CLASS (gtk_layout_parent_class)->constructor (type,
								  n_properties,
								  properties);

  layout = GTK_LAYOUT (object);

  hadj = gtk_layout_get_props (layout)->hadjustment ? gtk_layout_get_props (layout)->hadjustment : new_default_adjustment ();
  vadj = gtk_layout_get_props (layout)->vadjustment ? gtk_layout_get_props (layout)->vadjustment : new_default_adjustment ();

  if (!gtk_layout_get_props (layout)->hadjustment || !gtk_layout_get_props (layout)->vadjustment)
    gtk_layout_set_adjustments (layout, hadj, vadj);

  return object;
}

/* Widget methods
 */

static void 
gtk_layout_realize (GtkWidget *widget)
{
  GtkLayout *layout = GTK_LAYOUT (widget);
  GList *tmp_list;
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
  attributes.event_mask = GDK_VISIBILITY_NOTIFY_MASK;

  attributes_mask = GDK_WA_X | GDK_WA_Y | GDK_WA_VISUAL | GDK_WA_COLORMAP;

  gtk_widget_get_props (widget)->window = __gdk_window_new (__gtk_widget_get_parent_window (widget),
				   &attributes, attributes_mask);
  __gdk_window_set_back_pixmap (gtk_widget_get_props (widget)->window, NULL, FALSE);
  __gdk_window_set_user_data (gtk_widget_get_props (widget)->window, widget);

  attributes.x = - gtk_adjustment_get_props (gtk_layout_get_props (layout)->hadjustment)->value,
  attributes.y = - gtk_adjustment_get_props (gtk_layout_get_props (layout)->vadjustment)->value;
  attributes.width = MAX (gtk_layout_get_props (layout)->width, gtk_widget_get_props (widget)->allocation.width);
  attributes.height = MAX (gtk_layout_get_props (layout)->height, gtk_widget_get_props (widget)->allocation.height);
  attributes.event_mask = GDK_EXPOSURE_MASK | GDK_SCROLL_MASK | 
                          __gtk_widget_get_events (widget);

  gtk_layout_get_props (layout)->bin_window = __gdk_window_new (gtk_widget_get_props (widget)->window,
					&attributes, attributes_mask);
  __gdk_window_set_user_data (gtk_layout_get_props (layout)->bin_window, widget);

  gtk_widget_get_props (widget)->style = __gtk_style_attach (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window);
  __gtk_style_set_background (gtk_widget_get_props (widget)->style, gtk_layout_get_props (layout)->bin_window, GTK_STATE_NORMAL);

  tmp_list = gtk_layout_get_props (layout)->children;
  while (tmp_list)
    {
      GtkLayoutChild *child = tmp_list->data;
      tmp_list = tmp_list->next;

      __gtk_widget_set_parent_window (child->widget, gtk_layout_get_props (layout)->bin_window);
    }
}

static void
gtk_layout_style_set (GtkWidget *widget,
                      GtkStyle  *old_style)
{
  GTK_WIDGET_CLASS (gtk_layout_parent_class)->style_set (widget, old_style);

  if (__gtk_widget_get_realized (widget))
    {
      __gtk_style_set_background (gtk_widget_get_props (widget)->style, gtk_layout_get_props (widget)->bin_window, GTK_STATE_NORMAL);
    }
}

static void
gtk_layout_map (GtkWidget *widget)
{
  GtkLayout *layout = GTK_LAYOUT (widget);
  GList *tmp_list;

  __gtk_widget_set_mapped (widget, TRUE);

  tmp_list = gtk_layout_get_props (layout)->children;
  while (tmp_list)
    {
      GtkLayoutChild *child = tmp_list->data;
      tmp_list = tmp_list->next;

      if (__gtk_widget_get_visible (child->widget))
	{
	  if (!__gtk_widget_get_mapped (child->widget))
	    __gtk_widget_map (child->widget);
	}
    }

  __gdk_window_show (gtk_layout_get_props (layout)->bin_window);
  __gdk_window_show (gtk_widget_get_props (widget)->window);
}

static void 
gtk_layout_unrealize (GtkWidget *widget)
{
  GtkLayout *layout = GTK_LAYOUT (widget);

  __gdk_window_set_user_data (gtk_layout_get_props (layout)->bin_window, NULL);
  __gdk_window_destroy (gtk_layout_get_props (layout)->bin_window);
  gtk_layout_get_props (layout)->bin_window = NULL;

  GTK_WIDGET_CLASS (gtk_layout_parent_class)->unrealize (widget);
}

static void     
gtk_layout_size_request (GtkWidget     *widget,
			 GtkRequisition *requisition)
{
  GtkLayout *layout = GTK_LAYOUT (widget);
  GList *tmp_list;

  requisition->width = 0;
  requisition->height = 0;

  tmp_list = gtk_layout_get_props (layout)->children;

  while (tmp_list)
    {
      GtkLayoutChild *child = tmp_list->data;
      GtkRequisition child_requisition;
      
      tmp_list = tmp_list->next;

      __gtk_widget_size_request (child->widget, &child_requisition);
    }
}

static void     
gtk_layout_size_allocate (GtkWidget     *widget,
			  GtkAllocation *allocation)
{
  GtkLayout *layout = GTK_LAYOUT (widget);
  GList *tmp_list;

  gtk_widget_get_props (widget)->allocation = *allocation;

  tmp_list = gtk_layout_get_props (layout)->children;

  while (tmp_list)
    {
      GtkLayoutChild *child = tmp_list->data;
      tmp_list = tmp_list->next;

      gtk_layout_allocate_child (layout, child);
    }

  if (__gtk_widget_get_realized (widget))
    {
      __gdk_window_move_resize (gtk_widget_get_props (widget)->window,
			      allocation->x, allocation->y,
			      allocation->width, allocation->height);

      __gdk_window_resize (gtk_layout_get_props (layout)->bin_window,
			 MAX (gtk_layout_get_props (layout)->width, allocation->width),
			 MAX (gtk_layout_get_props (layout)->height, allocation->height));
    }

  gtk_adjustment_get_props (gtk_layout_get_props (layout)->hadjustment)->page_size = allocation->width;
  gtk_adjustment_get_props (gtk_layout_get_props (layout)->hadjustment)->page_increment = allocation->width * 0.9;
  gtk_adjustment_get_props (gtk_layout_get_props (layout)->hadjustment)->lower = 0;
  /* set_adjustment_upper() emits ::changed */
  gtk_layout_set_adjustment_upper (gtk_layout_get_props (layout)->hadjustment, MAX (allocation->width, gtk_layout_get_props (layout)->width), TRUE);

  gtk_adjustment_get_props (gtk_layout_get_props (layout)->vadjustment)->page_size = allocation->height;
  gtk_adjustment_get_props (gtk_layout_get_props (layout)->vadjustment)->page_increment = allocation->height * 0.9;
  gtk_adjustment_get_props (gtk_layout_get_props (layout)->vadjustment)->lower = 0;
  gtk_adjustment_get_props (gtk_layout_get_props (layout)->vadjustment)->upper = MAX (allocation->height, gtk_layout_get_props (layout))->height);
  gtk_layout_set_adjustment_upper (gtk_layout_get_props (layout)->vadjustment, MAX (allocation->height, gtk_layout_get_props (layout)->height), TRUE);
}

static gint 
gtk_layout_expose (GtkWidget      *widget,
                   GdkEventExpose *event)
{
  GtkLayout *layout = GTK_LAYOUT (widget);

  if (event->window != gtk_layout_get_props (layout)->bin_window)
    return FALSE;

  GTK_WIDGET_CLASS (gtk_layout_parent_class)->expose_event (widget, event);

  return FALSE;
}

/* Container methods
 */
static void
gtk_layout_add (GtkContainer *container,
		GtkWidget    *widget)
{
  __gtk_layout_put (GTK_LAYOUT (container), widget, 0, 0);
}

static void
gtk_layout_remove (GtkContainer *container, 
		   GtkWidget    *widget)
{
  GtkLayout *layout = GTK_LAYOUT (container);
  GList *tmp_list;
  GtkLayoutChild *child = NULL;

  tmp_list = gtk_layout_get_props (layout)->children;
  while (tmp_list)
    {
      child = tmp_list->data;
      if (child->widget == widget)
	break;
      tmp_list = tmp_list->next;
    }

  if (tmp_list)
    {
      __gtk_widget_unparent (widget);

      gtk_layout_get_props (layout)->children = g_list_remove_link (gtk_layout_get_props (layout)->children, tmp_list);
      g_list_free_1 (tmp_list);
      g_free (child);
    }
}

static void
gtk_layout_forall (GtkContainer *container,
		   gboolean      include_internals,
		   GtkCallback   callback,
		   gpointer      callback_data)
{
  GtkLayout *layout = GTK_LAYOUT (container);
  GtkLayoutChild *child;
  GList *tmp_list;

  tmp_list = gtk_layout_get_props (layout)->children;
  while (tmp_list)
    {
      child = tmp_list->data;
      tmp_list = tmp_list->next;

      (* callback) (child->widget, callback_data);
    }
}

/* Operations on children
 */

static void
gtk_layout_allocate_child (GtkLayout      *layout,
			   GtkLayoutChild *child)
{
  GtkAllocation allocation;
  GtkRequisition requisition;

  allocation.x = child->x;
  allocation.y = child->y;
  __gtk_widget_get_child_requisition (child->widget, &requisition);
  allocation.width = requisition.width;
  allocation.height = requisition.height;
  
  __gtk_widget_size_allocate (child->widget, &allocation);
}

/* Callbacks */

static void
gtk_layout_adjustment_changed (GtkAdjustment *adjustment,
			       GtkLayout     *layout)
{
  if (gtk_layout_get_props (layout)->freeze_count)
    return;

  if (__gtk_widget_get_realized (GTK_WIDGET (layout)))
    {
      __gdk_window_move (gtk_layout_get_props (layout)->bin_window,
		       - gtk_adjustment_get_props (gtk_layout_get_props (layout)->hadjustment)->value,
		       - gtk_adjustment_get_props (gtk_layout_get_props (layout)->vadjustment)->value);
      
      __gdk_window_process_updates (gtk_layout_get_props (layout)->bin_window, TRUE);
    }
}
