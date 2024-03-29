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
#include <gtkcheckbutton.h>
#include <gtklabel.h>
#include <gtkprivate.h>
#include <gtkintl.h>



#define INDICATOR_SIZE     13
#define INDICATOR_SPACING  2


static void gtk_check_button_size_request        (GtkWidget           *widget,
						  GtkRequisition      *requisition);
static void gtk_check_button_size_allocate       (GtkWidget           *widget,
						  GtkAllocation       *allocation);
static gint gtk_check_button_expose              (GtkWidget           *widget,
						  GdkEventExpose      *event);
static void gtk_check_button_paint               (GtkWidget           *widget,
						  GdkRectangle        *area);
static void gtk_check_button_draw_indicator      (GtkCheckButton      *check_button,
						  GdkRectangle        *area);
static void gtk_real_check_button_draw_indicator (GtkCheckButton      *check_button,
						  GdkRectangle        *area);

STLWRT_DEFINE_FTYPE_VPARENT (GtkCheckButton, gtk_check_button, GTK_TYPE_TOGGLE_BUTTON,
                             G_TYPE_FLAG_NONE, ;)

static void
gtk_check_button_class_init (GtkCheckButtonClass *class)
{
  GtkWidgetClass *widget_class;
  
  widget_class = (GtkWidgetClass*) class;
  
  widget_class->size_request = gtk_check_button_size_request;
  widget_class->size_allocate = gtk_check_button_size_allocate;
  widget_class->expose_event = gtk_check_button_expose;

  class->draw_indicator = gtk_real_check_button_draw_indicator;

  __gtk_widget_class_install_style_property (widget_class,
					   g_param_spec_int ("indicator-size",
							     P_("Indicator Size"),
							     P_("Size of check or radio indicator"),
							     0,
							     G_MAXINT,
							     INDICATOR_SIZE,
							     GTK_PARAM_READABLE));
  __gtk_widget_class_install_style_property (widget_class,
					   g_param_spec_int ("indicator-spacing",
							     P_("Indicator Spacing"),
							     P_("Spacing around check or radio indicator"),
							     0,
							     G_MAXINT,
							     INDICATOR_SPACING,
							     GTK_PARAM_READABLE));
}

static void
gtk_check_button_init (GtkCheckButton *check_button)
{
  __gtk_widget_set_has_window (GTK_WIDGET (check_button), FALSE);
  __gtk_widget_set_receives_default (GTK_WIDGET (check_button), FALSE);
  gtk_toggle_button_get_props (GTK_TOGGLE_BUTTON (check_button))->draw_indicator = TRUE;
  gtk_button_get_props (GTK_BUTTON (check_button))->depress_on_activate = FALSE;
}

GtkWidget*
__gtk_check_button_new (void)
{
  return g_object_new (GTK_TYPE_CHECK_BUTTON, NULL);
}


GtkWidget*
__gtk_check_button_new_with_label (const gchar *label)
{
  return g_object_new (GTK_TYPE_CHECK_BUTTON, "label", label, NULL);
}

/**
 * __gtk_check_button_new_with_mnemonic:
 * @label: The text of the button, with an underscore in front of the
 *         mnemonic character
 * @returns: a new #GtkCheckButton
 *
 * Creates a new #GtkCheckButton containing a label. The label
 * will be created using __gtk_label_new_with_mnemonic(), so underscores
 * in @label indicate the mnemonic for the check button.
 **/
GtkWidget*
__gtk_check_button_new_with_mnemonic (const gchar *label)
{
  return g_object_new (GTK_TYPE_CHECK_BUTTON, 
                       "label", label, 
                       "use-underline", TRUE, 
                       NULL);
}


/* This should only be called when toggle_button->draw_indicator
 * is true.
 */
static void
gtk_check_button_paint (GtkWidget    *widget,
			GdkRectangle *area)
{
  GtkCheckButton *check_button = GTK_CHECK_BUTTON (widget);
  
  if (__gtk_widget_is_drawable (widget))
    {
      gint border_width;
      gint interior_focus;
      gint focus_width;
      gint focus_pad;
	  
      __gtk_widget_style_get (widget,
			    "interior-focus", &interior_focus,
			    "focus-line-width", &focus_width,
			    "focus-padding", &focus_pad,
			    NULL);

      gtk_check_button_draw_indicator (check_button, area);
      
      border_width = gtk_container_get_props (GTK_CONTAINER (widget))->border_width;
      if (__gtk_widget_has_focus (widget))
	{
	  GtkWidget *child = gtk_bin_get_props (GTK_BIN (widget))->child;
	  
	  if (interior_focus && child && __gtk_widget_get_visible (child))
	    __gtk_paint_focus (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window, __gtk_widget_get_state (widget),
			     area, widget, "checkbutton",
			     gtk_widget_get_props (child)->allocation.x - focus_width - focus_pad,
			     gtk_widget_get_props (child)->allocation.y - focus_width - focus_pad,
			     gtk_widget_get_props (child)->allocation.width + 2 * (focus_width + focus_pad),
			     gtk_widget_get_props (child)->allocation.height + 2 * (focus_width + focus_pad));
	  else
	    __gtk_paint_focus (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window, __gtk_widget_get_state (widget),
			     area, widget, "checkbutton",
			     border_width + gtk_widget_get_props (widget)->allocation.x,
			     border_width + gtk_widget_get_props (widget)->allocation.y,
			     gtk_widget_get_props (widget)->allocation.width - 2 * border_width,
			     gtk_widget_get_props (widget)->allocation.height - 2 * border_width);
	}
    }
}

void
___gtk_check_button_get_props (GtkCheckButton *check_button,
			     gint           *indicator_size,
			     gint           *indicator_spacing)
{
  GtkWidget *widget =  GTK_WIDGET (check_button);

  if (indicator_size)
    __gtk_widget_style_get (widget, "indicator-size", indicator_size, NULL);

  if (indicator_spacing)
    __gtk_widget_style_get (widget, "indicator-spacing", indicator_spacing, NULL);
}

static void
gtk_check_button_size_request (GtkWidget      *widget,
			       GtkRequisition *requisition)
{
  GtkToggleButton *toggle_button = GTK_TOGGLE_BUTTON (widget);
  
  if (gtk_toggle_button_get_props (toggle_button)->draw_indicator)
    {
      GtkWidget *child;
      gint temp;
      gint indicator_size;
      gint indicator_spacing;
      gint border_width = gtk_container_get_props (GTK_CONTAINER (widget))->border_width;
      gint focus_width;
      gint focus_pad;

      __gtk_widget_style_get (GTK_WIDGET (widget),
			    "focus-line-width", &focus_width,
			    "focus-padding", &focus_pad,
			    NULL);
 
      requisition->width = border_width * 2;
      requisition->height = border_width * 2;

      ___gtk_check_button_get_props (GTK_CHECK_BUTTON (widget),
 				   &indicator_size, &indicator_spacing);
      
      child = gtk_bin_get_props (GTK_BIN (widget))->child;
      if (child && __gtk_widget_get_visible (child))
	{
	  GtkRequisition child_requisition;
	  
	  __gtk_widget_size_request (child, &child_requisition);

	  requisition->width += child_requisition.width + indicator_spacing;
	  requisition->height += child_requisition.height;
	}
      
      requisition->width += (indicator_size + indicator_spacing * 2 + 2 * (focus_width + focus_pad));
      
      temp = indicator_size + indicator_spacing * 2;
      requisition->height = MAX (requisition->height, temp) + 2 * (focus_width + focus_pad);
    }
  else
    GTK_WIDGET_CLASS (gtk_check_button_parent_class)->size_request (widget, requisition);
}

static void
gtk_check_button_size_allocate (GtkWidget     *widget,
				GtkAllocation *allocation)
{
  GtkCheckButton *check_button;
  GtkToggleButton *toggle_button;
  GtkButton *button;
  GtkAllocation child_allocation;

  button = GTK_BUTTON (widget);
  check_button = GTK_CHECK_BUTTON (widget);
  toggle_button = GTK_TOGGLE_BUTTON (widget);

  if (gtk_toggle_button_get_props (toggle_button)->draw_indicator)
    {
      gint indicator_size;
      gint indicator_spacing;
      gint focus_width;
      gint focus_pad;
      
      ___gtk_check_button_get_props (check_button, &indicator_size, &indicator_spacing);
      __gtk_widget_style_get (widget,
			    "focus-line-width", &focus_width,
			    "focus-padding", &focus_pad,
			    NULL);
						    
      gtk_widget_get_props (widget)->allocation = *allocation;
      if (__gtk_widget_get_realized (widget))
	__gdk_window_move_resize (gtk_button_get_props (button)->event_window,
				allocation->x, allocation->y,
				allocation->width, allocation->height);
      
      if (gtk_bin_get_props (GTK_BIN (button))->child && __gtk_widget_get_visible (gtk_bin_get_props (GTK_BIN (button))->child))
	{
	  GtkRequisition child_requisition;
 	  gint border_width = gtk_container_get_props (GTK_CONTAINER (widget))->border_width;

	  __gtk_widget_get_child_requisition (gtk_bin_get_props (GTK_BIN (button))->child, &child_requisition);
 
	  child_allocation.width = MIN (child_requisition.width,
					allocation->width -
					((border_width + focus_width + focus_pad) * 2
					 + indicator_size + indicator_spacing * 3));
	  child_allocation.width = MAX (child_allocation.width, 1);

	  child_allocation.height = MIN (child_requisition.height,
					 allocation->height - (border_width + focus_width + focus_pad) * 2);
	  child_allocation.height = MAX (child_allocation.height, 1);
	  
	  child_allocation.x = (border_width + indicator_size + indicator_spacing * 3 +
				gtk_widget_get_props (widget)->allocation.x + focus_width + focus_pad);
	  child_allocation.y = gtk_widget_get_props (widget)->allocation.y +
		  (allocation->height - child_allocation.height) / 2;

	  if (__gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL)
	    child_allocation.x = allocation->x + allocation->width
	      - (child_allocation.x - allocation->x + child_allocation.width);
	  
	  __gtk_widget_size_allocate (gtk_bin_get_props (GTK_BIN (button))->child, &child_allocation);
	}
    }
  else
    GTK_WIDGET_CLASS (gtk_check_button_parent_class)->size_allocate (widget, allocation);
}

static gint
gtk_check_button_expose (GtkWidget      *widget,
			 GdkEventExpose *event)
{
  GtkToggleButton *toggle_button;
  GtkBin *bin;
  
  toggle_button = GTK_TOGGLE_BUTTON (widget);
  bin = GTK_BIN (widget);
  
  if (__gtk_widget_is_drawable (widget))
    {
      if (gtk_toggle_button_get_props (toggle_button)->draw_indicator)
	{
	  gtk_check_button_paint (widget, &event->area);
	  
	  if (gtk_bin_get_props (bin)->child)
	    __gtk_container_propagate_expose (GTK_CONTAINER (widget),
					    gtk_bin_get_props (bin)->child,
					    event);
	}
      else if (GTK_WIDGET_CLASS (gtk_check_button_parent_class)->expose_event)
	GTK_WIDGET_CLASS (gtk_check_button_parent_class)->expose_event (widget, event);
    }
  
  return FALSE;
}


static void
gtk_check_button_draw_indicator (GtkCheckButton *check_button,
				 GdkRectangle   *area)
{
  GtkCheckButtonClass *class;
  
  g_return_if_fail (GTK_IS_CHECK_BUTTON (check_button));
  
  class = GTK_CHECK_BUTTON_GET_CLASS (check_button);

  if (class->draw_indicator)
    class->draw_indicator (check_button, area);
}

static void
gtk_real_check_button_draw_indicator (GtkCheckButton *check_button,
				      GdkRectangle   *area)
{
  GtkWidget *widget;
  GtkWidget *child;
  GtkButton *button;
  GtkToggleButton *toggle_button;
  GtkStateType state_type;
  GtkShadowType shadow_type;
  gint x, y;
  gint indicator_size;
  gint indicator_spacing;
  gint focus_width;
  gint focus_pad;
  gboolean interior_focus;

  widget = GTK_WIDGET (check_button);

  if (__gtk_widget_is_drawable (widget))
    {
      button = GTK_BUTTON (check_button);
      toggle_button = GTK_TOGGLE_BUTTON (check_button);
  
      __gtk_widget_style_get (widget, 
			    "interior-focus", &interior_focus,
			    "focus-line-width", &focus_width, 
			    "focus-padding", &focus_pad, 
			    NULL);

      ___gtk_check_button_get_props (check_button, &indicator_size, &indicator_spacing);

      x = gtk_widget_get_props (widget)->allocation.x + indicator_spacing + gtk_container_get_props (GTK_CONTAINER (widget))->border_width;
      y = gtk_widget_get_props (widget)->allocation.y + (gtk_widget_get_props (widget)->allocation.height - indicator_size) / 2;

      child = gtk_bin_get_props (GTK_BIN (check_button))->child;
      if (!interior_focus || !(child && __gtk_widget_get_visible (child)))
	x += focus_width + focus_pad;      

      if (gtk_toggle_button_get_props (toggle_button)->inconsistent)
	shadow_type = GTK_SHADOW_ETCHED_IN;
      else if (gtk_toggle_button_get_props (toggle_button)->active)
	shadow_type = GTK_SHADOW_IN;
      else
	shadow_type = GTK_SHADOW_OUT;

      if (gtk_button_get_props (button)->activate_timeout || (gtk_button_get_props (button)->button_down && gtk_button_get_props (button)->in_button))
	state_type = GTK_STATE_ACTIVE;
      else if (gtk_button_get_props (button)->in_button)
	state_type = GTK_STATE_PRELIGHT;
      else if (!__gtk_widget_is_sensitive (widget))
	state_type = GTK_STATE_INSENSITIVE;
      else
	state_type = GTK_STATE_NORMAL;
      
      if (__gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL)
	x = gtk_widget_get_props (widget)->allocation.x + gtk_widget_get_props (widget)->allocation.width - (indicator_size + x - gtk_widget_get_props (widget)->allocation.x);

      if (__gtk_widget_get_state (widget) == GTK_STATE_PRELIGHT)
	{
	  GdkRectangle restrict_area;
	  GdkRectangle new_area;
	      
	  restrict_area.x = gtk_widget_get_props (widget)->allocation.x + gtk_container_get_props (GTK_CONTAINER (widget))->border_width;
	  restrict_area.y = gtk_widget_get_props (widget)->allocation.y + gtk_container_get_props (GTK_CONTAINER (widget))->border_width;
	  restrict_area.width = gtk_widget_get_props (widget)->allocation.width - (2 * gtk_container_get_props (GTK_CONTAINER (widget))->border_width);
	  restrict_area.height = gtk_widget_get_props (widget)->allocation.height - (2 * gtk_container_get_props (GTK_CONTAINER (widget))->border_width);
	  
	  if (__gdk_rectangle_intersect (area, &restrict_area, &new_area))
	    {
	      __gtk_paint_flat_box (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window, GTK_STATE_PRELIGHT,
				  GTK_SHADOW_ETCHED_OUT, 
				  area, widget, "checkbutton",
				  new_area.x, new_area.y,
				  new_area.width, new_area.height);
	    }
	}

      __gtk_paint_check (gtk_widget_get_props (widget)->style, gtk_widget_get_props (widget)->window,
		       state_type, shadow_type,
		       area, widget, "checkbutton",
		       x, y, indicator_size, indicator_size);
    }
}
