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



/**
 * SECTION:gtkalignment
 * @Short_description: A widget which controls the alignment and size of its child
 * @Title: GtkAlignment
 *
 * The #GtkAlignment widget controls the alignment and size of its child widget.
 * It has four settings: xscale, yscale, xalign, and yalign.
 *
 * The scale settings are used to specify how much the child widget should
 * expand to fill the space allocated to the #GtkAlignment.
 * The values can range from 0 (meaning the child doesn't expand at all) to
 * 1 (meaning the child expands to fill all of the available space).
 *
 * The align settings are used to place the child widget within the available
 * area. The values range from 0 (top or left) to 1 (bottom or right).
 * Of course, if the scale settings are both set to 1, the alignment settings
 * have no effect.
 */

#include "config.h"

#include <stlwrt.h>
#include <gtkalignment.h>
#include <gtkprivate.h>
#include <gtkintl.h>


enum {
  PROP_0,

  PROP_XALIGN,
  PROP_YALIGN,
  PROP_XSCALE,
  PROP_YSCALE,

  PROP_TOP_PADDING,
  PROP_BOTTOM_PADDING,
  PROP_LEFT_PADDING,
  PROP_RIGHT_PADDING
};

static void gtk_alignment_size_request  (GtkWidget         *widget,
					 GtkRequisition    *requisition);
static void gtk_alignment_size_allocate (GtkWidget         *widget,
					 GtkAllocation     *allocation);
static void gtk_alignment_set_property (GObject         *object,
                                        guint            prop_id,
                                        const GValue    *value,
                                        GParamSpec      *pspec);
static void gtk_alignment_get_property (GObject         *object,
                                        guint            prop_id,
                                        GValue          *value,
                                        GParamSpec      *pspec);

STLWRT_DEFINE_VTYPE (GtkAlignment, gtk_alignment, GTK_TYPE_BIN,
                     G_TYPE_FLAG_NONE,
                     G_ADD_PRIVATE (GtkAlignment))

static void
gtk_alignment_class_init (GtkAlignmentClass *class)
{
  GObjectClass *gobject_class;
  GtkWidgetClass *widget_class;

  gobject_class = (GObjectClass*) class;
  widget_class = (GtkWidgetClass*) class;
  
  gobject_class->set_property = gtk_alignment_set_property;
  gobject_class->get_property = gtk_alignment_get_property;

  widget_class->size_request = gtk_alignment_size_request;
  widget_class->size_allocate = gtk_alignment_size_allocate;

  g_object_class_install_property (gobject_class,
                                   PROP_XALIGN,
                                   g_param_spec_float("xalign",
                                                      P_("Horizontal alignment"),
                                                      P_("Horizontal position of child in available space. 0.0 is left aligned, 1.0 is right aligned"),
                                                      0.0,
                                                      1.0,
                                                      0.5,
                                                      GTK_PARAM_READWRITE));
   
  g_object_class_install_property (gobject_class,
                                   PROP_YALIGN,
                                   g_param_spec_float("yalign",
                                                      P_("Vertical alignment"),
                                                      P_("Vertical position of child in available space. 0.0 is top aligned, 1.0 is bottom aligned"),
                                                      0.0,
                                                      1.0,
						      0.5,
                                                      GTK_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_XSCALE,
                                   g_param_spec_float("xscale",
                                                      P_("Horizontal scale"),
                                                      P_("If available horizontal space is bigger than needed for the child, how much of it to use for the child. 0.0 means none, 1.0 means all"),
                                                      0.0,
                                                      1.0,
                                                      1.0,
                                                      GTK_PARAM_READWRITE));
  g_object_class_install_property (gobject_class,
                                   PROP_YSCALE,
                                   g_param_spec_float("yscale",
                                                      P_("Vertical scale"),
                                                      P_("If available vertical space is bigger than needed for the child, how much of it to use for the child. 0.0 means none, 1.0 means all"),
                                                      0.0,
                                                      1.0,
                                                      1.0,
                                                      GTK_PARAM_READWRITE));


/**
 * GtkAlignment:top-padding:
 *
 * The padding to insert at the top of the widget.
 *
 * Since: 2.4
 */
  g_object_class_install_property (gobject_class,
                                   PROP_TOP_PADDING,
                                   g_param_spec_uint("top-padding",
                                                      P_("Top Padding"),
                                                      P_("The padding to insert at the top of the widget."),
                                                      0,
                                                      G_MAXINT,
                                                      0,
                                                      GTK_PARAM_READWRITE));

/**
 * GtkAlignment:bottom-padding:
 *
 * The padding to insert at the bottom of the widget.
 *
 * Since: 2.4
 */
  g_object_class_install_property (gobject_class,
                                   PROP_BOTTOM_PADDING,
                                   g_param_spec_uint("bottom-padding",
                                                      P_("Bottom Padding"),
                                                      P_("The padding to insert at the bottom of the widget."),
                                                      0,
                                                      G_MAXINT,
                                                      0,
                                                      GTK_PARAM_READWRITE));

/**
 * GtkAlignment:left-padding:
 *
 * The padding to insert at the left of the widget.
 *
 * Since: 2.4
 */
  g_object_class_install_property (gobject_class,
                                   PROP_LEFT_PADDING,
                                   g_param_spec_uint("left-padding",
                                                      P_("Left Padding"),
                                                      P_("The padding to insert at the left of the widget."),
                                                      0,
                                                      G_MAXINT,
                                                      0,
                                                      GTK_PARAM_READWRITE));

/**
 * GtkAlignment:right-padding:
 *
 * The padding to insert at the right of the widget.
 *
 * Since: 2.4
 */
  g_object_class_install_property (gobject_class,
                                   PROP_RIGHT_PADDING,
                                   g_param_spec_uint("right-padding",
                                                      P_("Right Padding"),
                                                      P_("The padding to insert at the right of the widget."),
                                                      0,
                                                      G_MAXINT,
                                                      0,
                                                      GTK_PARAM_READWRITE));
}

static void
gtk_alignment_init (GtkAlignment *alignment)
{
  GtkAlignmentProps *alignment_props = gtk_alignment_get_props (alignment);
  GtkAlignmentPrivate *priv;
  
  __gtk_widget_set_has_window (GTK_WIDGET (alignment), FALSE);
  __gtk_widget_set_redraw_on_allocate (GTK_WIDGET (alignment), FALSE);

  alignment_props->xalign = 0.5;
  alignment_props->yalign = 0.5;
  alignment_props->xscale = 1.0;
  alignment_props->yscale = 1.0;

  /* Initialize padding with default values: */
  priv = gtk_alignment_get_private (alignment);
  priv->padding_top = 0;
  priv->padding_bottom = 0;
  priv->padding_left = 0;
  priv->padding_right = 0;
}

/**
 * __gtk_alignment_new:
 * @xalign: the horizontal alignment of the child widget, from 0 (left) to 1
 *  (right).
 * @yalign: the vertical alignment of the child widget, from 0 (top) to 1
 *  (bottom).
 * @xscale: the amount that the child widget expands horizontally to fill up
 *  unused space, from 0 to 1.
 *  A value of 0 indicates that the child widget should never expand.
 *  A value of 1 indicates that the child widget will expand to fill all of the
 *  space allocated for the #GtkAlignment.
 * @yscale: the amount that the child widget expands vertically to fill up
 *  unused space, from 0 to 1. The values are similar to @xscale.
 *
 * Creates a new #GtkAlignment.
 *
 * Returns: the new #GtkAlignment.
 */
GtkWidget*
__gtk_alignment_new (gfloat xalign,
		   gfloat yalign,
		   gfloat xscale,
		   gfloat yscale)
{
  GtkAlignment *alignment;
  GtkAlignmentProps *alignment_props;

  alignment = g_object_new (GTK_TYPE_ALIGNMENT, NULL);
  alignment_props = gtk_alignment_get_props (alignment);

  alignment_props->xalign = CLAMP (xalign, 0.0, 1.0);
  alignment_props->yalign = CLAMP (yalign, 0.0, 1.0);
  alignment_props->xscale = CLAMP (xscale, 0.0, 1.0);
  alignment_props->yscale = CLAMP (yscale, 0.0, 1.0);

  return GTK_WIDGET (alignment);
}

static void
gtk_alignment_set_property (GObject         *object,
			    guint            prop_id,
			    const GValue    *value,
			    GParamSpec      *pspec)
{
  GtkAlignment *alignment;
  GtkAlignmentProps *alignment_props;
  GtkAlignmentPrivate *priv;
  
  alignment = GTK_ALIGNMENT (object);
  alignment_props = gtk_alignment_get_props (alignment);
  priv = gtk_alignment_get_private (alignment);
  
  switch (prop_id)
    {
    case PROP_XALIGN:
      __gtk_alignment_set (alignment,
			 g_value_get_float (value),
			 alignment_props->yalign,
			 alignment_props->xscale,
			 alignment_props->yscale);
      break;
    case PROP_YALIGN:
      __gtk_alignment_set (alignment,
			 alignment_props->xalign,
			 g_value_get_float (value),
			 alignment_props->xscale,
			 alignment_props->yscale);
      break;
    case PROP_XSCALE:
      __gtk_alignment_set (alignment,
			 alignment_props->xalign,
			 alignment_props->yalign,
			 g_value_get_float (value),
			 alignment_props->yscale);
      break;
    case PROP_YSCALE:
      __gtk_alignment_set (alignment,
			 alignment_props->xalign,
			 alignment_props->yalign,
			 alignment_props->xscale,
			 g_value_get_float (value));
      break;
      
    /* Padding: */
    case PROP_TOP_PADDING:
      __gtk_alignment_set_padding (alignment,
			 g_value_get_uint (value),
			 priv->padding_bottom,
			 priv->padding_left,
			 priv->padding_right);
      break;
    case PROP_BOTTOM_PADDING:
      __gtk_alignment_set_padding (alignment,
			 priv->padding_top,
			 g_value_get_uint (value),
			 priv->padding_left,
			 priv->padding_right);
      break;
    case PROP_LEFT_PADDING:
      __gtk_alignment_set_padding (alignment,
			 priv->padding_top,
			 priv->padding_bottom,
			 g_value_get_uint (value),
			 priv->padding_right);
      break;
    case PROP_RIGHT_PADDING:
      __gtk_alignment_set_padding (alignment,
			 priv->padding_top,
			 priv->padding_bottom,
			 priv->padding_left,
			 g_value_get_uint (value));
      break;
    
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_alignment_get_property (GObject         *object,
			    guint            prop_id,
			    GValue          *value,
			    GParamSpec      *pspec)
{
  GtkAlignment *alignment;
  GtkAlignmentProps *alignment_props;
  GtkAlignmentPrivate *priv;

  alignment = GTK_ALIGNMENT (object);
  alignment_props = gtk_alignment_get_props (alignment);
  priv = gtk_alignment_get_private (alignment);
   
  switch (prop_id)
    {
    case PROP_XALIGN:
      g_value_set_float(value, alignment_props->xalign);
      break;
    case PROP_YALIGN:
      g_value_set_float(value, alignment_props->yalign);
      break;
    case PROP_XSCALE:
      g_value_set_float(value, alignment_props->xscale);
      break;
    case PROP_YSCALE:
      g_value_set_float(value, alignment_props->yscale);
      break;

    /* Padding: */
    case PROP_TOP_PADDING:
      g_value_set_uint (value, priv->padding_top);
      break;
    case PROP_BOTTOM_PADDING:
      g_value_set_uint (value, priv->padding_bottom);
      break;
    case PROP_LEFT_PADDING:
      g_value_set_uint (value, priv->padding_left);
      break;
    case PROP_RIGHT_PADDING:
      g_value_set_uint (value, priv->padding_right);
      break;
      
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

/**
 * __gtk_alignment_set:
 * @alignment: a #GtkAlignment.
 * @xalign: the horizontal alignment of the child widget, from 0 (left) to 1
 *  (right).
 * @yalign: the vertical alignment of the child widget, from 0 (top) to 1
 *  (bottom).
 * @xscale: the amount that the child widget expands horizontally to fill up
 *  unused space, from 0 to 1.
 *  A value of 0 indicates that the child widget should never expand.
 *  A value of 1 indicates that the child widget will expand to fill all of the
 *  space allocated for the #GtkAlignment.
 * @yscale: the amount that the child widget expands vertically to fill up
 *  unused space, from 0 to 1. The values are similar to @xscale.
 *
 * Sets the #GtkAlignment values.
 */
void
__gtk_alignment_set (GtkAlignment *alignment,
		   gfloat        xalign,
		   gfloat        yalign,
		   gfloat        xscale,
		   gfloat        yscale)
{
  GtkAlignmentProps *alignment_props = gtk_alignment_get_props (alignment);

  g_return_if_fail (GTK_IS_ALIGNMENT (alignment));

  xalign = CLAMP (xalign, 0.0, 1.0);
  yalign = CLAMP (yalign, 0.0, 1.0);
  xscale = CLAMP (xscale, 0.0, 1.0);
  yscale = CLAMP (yscale, 0.0, 1.0);

  if (   (alignment_props->xalign != xalign)
      || (alignment_props->yalign != yalign)
      || (alignment_props->xscale != xscale)
      || (alignment_props->yscale != yscale))
    {
      g_object_freeze_notify (G_OBJECT (alignment));
      if (alignment_props->xalign != xalign)
        {
           alignment_props->xalign = xalign;
           g_object_notify (G_OBJECT (alignment), "xalign");
        }
      if (alignment_props->yalign != yalign)
        {
           alignment_props->yalign = yalign;
           g_object_notify (G_OBJECT (alignment), "yalign");
        }
      if (alignment_props->xscale != xscale)
        {
           alignment_props->xscale = xscale;
           g_object_notify (G_OBJECT (alignment), "xscale");
        }
      if (alignment_props->yscale != yscale)
        {
           alignment_props->yscale = yscale;
           g_object_notify (G_OBJECT (alignment), "yscale");
        }
      g_object_thaw_notify (G_OBJECT (alignment));

      if (GTK_BIN (alignment)->child)
        __gtk_widget_queue_resize (GTK_BIN (alignment)->child);
      __gtk_widget_queue_draw (GTK_WIDGET (alignment));
    }
}


static void
gtk_alignment_size_request (GtkWidget      *widget,
			    GtkRequisition *requisition)
{
  GtkBin *bin = GTK_BIN (widget);
  GtkBinProps *bin_props = gtk_bin_get_props (bin);
  GtkContainerProps *container_props = gtk_container_get_props (GTK_CONTAINER (widget));
  GtkAlignmentPrivate *priv = gtk_alignment_get_private (widget);

  requisition->width = container_props->border_width * 2;
  requisition->height = container_props->border_width * 2;

  if (bin_props->child && __gtk_widget_get_visible (bin_props->child))
    {
      GtkRequisition child_requisition;
      
      __gtk_widget_size_request (bin_props->child, &child_requisition);

      requisition->width += child_requisition.width;
      requisition->height += child_requisition.height;

      /* Request extra space for the padding: */
      requisition->width += (priv->padding_left + priv->padding_right);
      requisition->height += (priv->padding_top + priv->padding_bottom);
    }
}

static void
gtk_alignment_size_allocate (GtkWidget     *widget,
			     GtkAllocation *allocation)
{
  GtkWidget *widget_props = gtk_widget_get_props (widget);
  GtkAlignment *alignment;
  GtkAlignment *alignment_props = gtk_alignment_get_props (alignment);
  GtkBin *bin;
  GtkBin *bin_props = gtk_bin_get_props (bin);
  GtkAllocation child_allocation;
  GtkRequisition child_requisition;
  gint width, height;
  gint border_width;
  gint padding_horizontal, padding_vertical;
  GtkAlignmentPrivate *priv;

  padding_horizontal = 0;
  padding_vertical = 0;

  widget_props->allocation = *allocation;
  alignment = GTK_ALIGNMENT (widget);
  bin = GTK_BIN (widget);
  
  if (bin->child && __gtk_widget_get_visible (bin->child))
    {
      __gtk_widget_get_child_requisition (bin->child, &child_requisition);

      border_width = GTK_CONTAINER (alignment)->border_width;

      priv = gtk_alignment_get_private (widget);
      padding_horizontal = priv->padding_left + priv->padding_right;
      padding_vertical = priv->padding_top + priv->padding_bottom;

      width  = MAX (1, allocation->width - padding_horizontal - 2 * border_width);
      height = MAX (1, allocation->height - padding_vertical - 2 * border_width);
    
      if (width > child_requisition.width)
	child_allocation.width = (child_requisition.width *
				  (1.0 - alignment_props->xscale) +
				  width * alignment_props->xscale);
      else
	child_allocation.width = width;
      
      if (height > child_requisition.height)
	child_allocation.height = (child_requisition.height *
				   (1.0 - alignment_props->yscale) +
				   height * alignment_props->yscale);
      else
	child_allocation.height = height;

      if (__gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL)
	child_allocation.x = (1.0 - alignment_props->xalign) * (width - child_allocation.width) + allocation->x + border_width + priv->padding_right;
      else 
	child_allocation.x = alignment_props->xalign * (width - child_allocation.width) + allocation->x + border_width + priv->padding_left;

      child_allocation.y = alignment_props->yalign * (height - child_allocation.height) + allocation->y + border_width + priv->padding_top;

      __gtk_widget_size_allocate (bin->child, &child_allocation);
    }
}

/**
 * __gtk_alignment_set_padding:
 * @alignment: a #GtkAlignment
 * @padding_top: the padding at the top of the widget
 * @padding_bottom: the padding at the bottom of the widget
 * @padding_left: the padding at the left of the widget
 * @padding_right: the padding at the right of the widget.
 *
 * Sets the padding on the different sides of the widget.
 * The padding adds blank space to the sides of the widget. For instance,
 * this can be used to indent the child widget towards the right by adding
 * padding on the left.
 *
 * Since: 2.4
 */
void
__gtk_alignment_set_padding (GtkAlignment    *alignment,
			   guint            padding_top,
			   guint            padding_bottom,
			   guint            padding_left,
			   guint            padding_right)
{
  GtkAlignmentPrivate *priv;
  
  g_return_if_fail (GTK_IS_ALIGNMENT (alignment));

  priv = gtk_alignment_get_private (alignment);

  g_object_freeze_notify (G_OBJECT (alignment));

  if (priv->padding_top != padding_top)
    {
      priv->padding_top = padding_top;
      g_object_notify (G_OBJECT (alignment), "top-padding");
    }
  if (priv->padding_bottom != padding_bottom)
    {
      priv->padding_bottom = padding_bottom;
      g_object_notify (G_OBJECT (alignment), "bottom-padding");
    }
  if (priv->padding_left != padding_left)
    {
      priv->padding_left = padding_left;
      g_object_notify (G_OBJECT (alignment), "left-padding");
    }
  if (priv->padding_right != padding_right)
    {
      priv->padding_right = padding_right;
      g_object_notify (G_OBJECT (alignment), "right-padding");
    }

  g_object_thaw_notify (G_OBJECT (alignment));
  
  /* Make sure that the widget and children are redrawn with the new setting: */
  if (gtk_bin_get_props (GTK_BIN (alignment))->child)
    __gtk_widget_queue_resize (gtk_bin_get_props (GTK_BIN (alignment))->child);

  __gtk_widget_queue_draw (GTK_WIDGET (alignment));
}

/**
 * __gtk_alignment_get_padding:
 * @alignment: a #GtkAlignment
 * @padding_top: (out) (allow-none): location to store the padding for
 *     the top of the widget, or %NULL
 * @padding_bottom: (out) (allow-none): location to store the padding
 *     for the bottom of the widget, or %NULL
 * @padding_left: (out) (allow-none): location to store the padding
 *     for the left of the widget, or %NULL
 * @padding_right: (out) (allow-none): location to store the padding
 *     for the right of the widget, or %NULL
 *
 * Gets the padding on the different sides of the widget.
 * See __gtk_alignment_set_padding ().
 *
 * Since: 2.4
 */
void
__gtk_alignment_get_padding (GtkAlignment    *alignment,
			   guint           *padding_top,
			   guint           *padding_bottom,
			   guint           *padding_left,
			   guint           *padding_right)
{
  GtkAlignmentPrivate *priv;
 
  g_return_if_fail (GTK_IS_ALIGNMENT (alignment));

  priv = gtk_alignment_get_private (alignment);
  if(padding_top)
    *padding_top = priv->padding_top;
  if(padding_bottom)
    *padding_bottom = priv->padding_bottom;
  if(padding_left)
    *padding_left = priv->padding_left;
  if(padding_right)
    *padding_right = priv->padding_right;
}
