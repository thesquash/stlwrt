/* gtkcellrenderertoggle.c
 * Copyright (C) 2000  Red Hat, Inc.,  Jonathan Blandford <jrb@redhat.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#include "config.h"

#include <stlwrt.h>
#include <stdlib.h>
#include <gtkcellrenderertoggle.h>
#include <gtkintl.h>

#include <gtkprivate.h>
#include <gtktreeprivate.h>


static void gtk_cell_renderer_toggle_get_property  (GObject                    *object,
						    guint                       param_id,
						    GValue                     *value,
						    GParamSpec                 *pspec);
static void gtk_cell_renderer_toggle_set_property  (GObject                    *object,
						    guint                       param_id,
						    const GValue               *value,
						    GParamSpec                 *pspec);
static void gtk_cell_renderer_toggle_get_size   (GtkCellRenderer            *cell,
						 GtkWidget                  *widget,
 						 GdkRectangle               *cell_area,
						 gint                       *x_offset,
						 gint                       *y_offset,
						 gint                       *width,
						 gint                       *height);
static void gtk_cell_renderer_toggle_render     (GtkCellRenderer            *cell,
						 GdkDrawable                *window,
						 GtkWidget                  *widget,
						 GdkRectangle               *background_area,
						 GdkRectangle               *cell_area,
						 GdkRectangle               *expose_area,
						 GtkCellRendererState        flags);
static gboolean gtk_cell_renderer_toggle_activate  (GtkCellRenderer            *cell,
						    GdkEvent                   *event,
						    GtkWidget                  *widget,
						    const gchar                *path,
						    GdkRectangle               *background_area,
						    GdkRectangle               *cell_area,
						    GtkCellRendererState        flags);


enum {
  TOGGLED,
  LAST_SIGNAL
};

enum {
  PROP_0,
  PROP_ACTIVATABLE,
  PROP_ACTIVE,
  PROP_RADIO,
  PROP_INCONSISTENT,
  PROP_INDICATOR_SIZE
};

#define TOGGLE_WIDTH 13

static guint toggle_cell_signals[LAST_SIGNAL] = { 0 };



typedef struct _GtkCellRendererTogglePrivate GtkCellRendererTogglePrivate;

struct _GtkCellRendererTogglePrivate
{
  gint indicator_size;

  guint inconsistent : 1;
};


STLWRT_DEFINE_VTYPE (GtkCellRendererToggle, gtk_cell_renderer_toggle, GTK_TYPE_CELL_RENDERER,
                     G_TYPE_FLAG_NONE,
                     G_ADD_PRIVATE (GtkCellRendererToggle))

static void
gtk_cell_renderer_toggle_init (GtkCellRendererToggle *celltoggle)
{
  GtkCellRendererTogglePrivate *priv;

  priv = gtk_cell_renderer_toggle_get_instance_private (celltoggle);

  gtk_cell_renderer_toggle_get_props (celltoggle)->activatable = TRUE;
  gtk_cell_renderer_toggle_get_props (celltoggle)->active = FALSE;
  gtk_cell_renderer_toggle_get_props (celltoggle)->radio = FALSE;

  gtk_cell_renderer_get_props (GTK_CELL_RENDERER (celltoggle))->mode = GTK_CELL_RENDERER_MODE_ACTIVATABLE;
  gtk_cell_renderer_get_props (GTK_CELL_RENDERER (celltoggle))->xpad = 2;
  gtk_cell_renderer_get_props (GTK_CELL_RENDERER (celltoggle))->ypad = 2;

  priv->indicator_size = TOGGLE_WIDTH;
  priv->inconsistent = FALSE;
}

static void
gtk_cell_renderer_toggle_class_init (GtkCellRendererToggleClass *class)
{
  GObjectClass *object_class = G_OBJECT_CLASS (class);
  GtkCellRendererClass *cell_class = GTK_CELL_RENDERER_CLASS (class);

  object_class->get_property = gtk_cell_renderer_toggle_get_property;
  object_class->set_property = gtk_cell_renderer_toggle_set_property;

  cell_class->get_size = gtk_cell_renderer_toggle_get_size;
  cell_class->render = gtk_cell_renderer_toggle_render;
  cell_class->activate = gtk_cell_renderer_toggle_activate;
  
  g_object_class_install_property (object_class,
				   PROP_ACTIVE,
				   g_param_spec_boolean ("active",
							 P_("Toggle state"),
							 P_("The toggle state of the button"),
							 FALSE,
							 GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
		                   PROP_INCONSISTENT,
				   g_param_spec_boolean ("inconsistent",
					                 P_("Inconsistent state"),
							 P_("The inconsistent state of the button"),
							 FALSE,
							 GTK_PARAM_READWRITE));
  
  g_object_class_install_property (object_class,
				   PROP_ACTIVATABLE,
				   g_param_spec_boolean ("activatable",
							 P_("Activatable"),
							 P_("The toggle button can be activated"),
							 TRUE,
							 GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_RADIO,
				   g_param_spec_boolean ("radio",
							 P_("Radio state"),
							 P_("Draw the toggle button as a radio button"),
							 FALSE,
							 GTK_PARAM_READWRITE));

  g_object_class_install_property (object_class,
				   PROP_INDICATOR_SIZE,
				   g_param_spec_int ("indicator-size",
						     P_("Indicator size"),
						     P_("Size of check or radio indicator"),
						     0,
						     G_MAXINT,
						     TOGGLE_WIDTH,
						     GTK_PARAM_READWRITE));

  
  /**
   * GtkCellRendererToggle::toggled:
   * @cell_renderer: the object which received the signal
   * @path: string representation of #GtkTreePath describing the 
   *        event location
   *
   * The ::toggled signal is emitted when the cell is toggled. 
   **/
  toggle_cell_signals[TOGGLED] =
    g_signal_new (I_("toggled"),
		  G_OBJECT_CLASS_TYPE (object_class),
		  G_SIGNAL_RUN_LAST,
		  G_STRUCT_OFFSET (GtkCellRendererToggleClass, toggled),
		  NULL, NULL,
		  NULL,
		  G_TYPE_NONE, 1,
		  G_TYPE_STRING);
}

static void
gtk_cell_renderer_toggle_get_property (GObject     *object,
				       guint        param_id,
				       GValue      *value,
				       GParamSpec  *pspec)
{
  GtkCellRendererToggle *celltoggle = GTK_CELL_RENDERER_TOGGLE (object);
  GtkCellRendererTogglePrivate *priv;

  priv = gtk_cell_renderer_toggle_get_instance_private (object);
  
  switch (param_id)
    {
    case PROP_ACTIVE:
      g_value_set_boolean (value, gtk_cell_renderer_toggle_get_props (celltoggle)->active);
      break;
    case PROP_INCONSISTENT:
      g_value_set_boolean (value, priv->inconsistent);
      break;
    case PROP_ACTIVATABLE:
      g_value_set_boolean (value, gtk_cell_renderer_toggle_get_props (celltoggle)->activatable);
      break;
    case PROP_RADIO:
      g_value_set_boolean (value, gtk_cell_renderer_toggle_get_props (celltoggle)->radio);
      break;
    case PROP_INDICATOR_SIZE:
      g_value_set_int (value, priv->indicator_size);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
      break;
    }
}


static void
gtk_cell_renderer_toggle_set_property (GObject      *object,
				       guint         param_id,
				       const GValue *value,
				       GParamSpec   *pspec)
{
  GtkCellRendererToggle *celltoggle = GTK_CELL_RENDERER_TOGGLE (object);
  GtkCellRendererTogglePrivate *priv;

  priv = gtk_cell_renderer_toggle_get_instance_private (object);

  switch (param_id)
    {
    case PROP_ACTIVE:
      gtk_cell_renderer_toggle_get_props (celltoggle)->active = g_value_get_boolean (value);
      break;
    case PROP_INCONSISTENT:
      priv->inconsistent = g_value_get_boolean (value);
      break;
    case PROP_ACTIVATABLE:
      gtk_cell_renderer_toggle_get_props (celltoggle)->activatable = g_value_get_boolean (value);
      break;
    case PROP_RADIO:
      gtk_cell_renderer_toggle_get_props (celltoggle)->radio = g_value_get_boolean (value);
      break;
    case PROP_INDICATOR_SIZE:
      priv->indicator_size = g_value_get_int (value);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, param_id, pspec);
      break;
    }
}

/**
 * __gtk_cell_renderer_toggle_new:
 *
 * Creates a new #GtkCellRendererToggle. Adjust rendering
 * parameters using object properties. Object properties can be set
 * globally (with g_object_set()). Also, with #GtkTreeViewColumn, you
 * can bind a property to a value in a #GtkTreeModel. For example, you
 * can bind the "active" property on the cell renderer to a boolean value
 * in the model, thus causing the check button to reflect the state of
 * the model.
 *
 * Return value: the new cell renderer
 **/
GtkCellRenderer *
__gtk_cell_renderer_toggle_new (void)
{
  return g_object_new (GTK_TYPE_CELL_RENDERER_TOGGLE, NULL);
}

static void
gtk_cell_renderer_toggle_get_size (GtkCellRenderer *cell,
				   GtkWidget       *widget,
				   GdkRectangle    *cell_area,
				   gint            *x_offset,
				   gint            *y_offset,
				   gint            *width,
				   gint            *height)
{
  gint calc_width;
  gint calc_height;
  GtkCellRendererTogglePrivate *priv;

  priv = gtk_cell_renderer_toggle_get_instance_private (cell);

  calc_width = (gint) gtk_cell_renderer_get_props (cell)->xpad * 2 + priv->indicator_size;
  calc_height = (gint) gtk_cell_renderer_get_props (cell)->ypad * 2 + priv->indicator_size;

  if (width)
    *width = calc_width;

  if (height)
    *height = calc_height;

  if (cell_area)
    {
      if (x_offset)
	{
	  *x_offset = ((__gtk_widget_get_direction (widget) == GTK_TEXT_DIR_RTL) ?
		       (1.0 - gtk_cell_renderer_get_props (cell)->xalign) : gtk_cell_renderer_get_props (cell)->xalign) * (cell_area->width - calc_width);
	  *x_offset = MAX (*x_offset, 0);
	}
      if (y_offset)
	{
	  *y_offset = gtk_cell_renderer_get_props (cell)->yalign * (cell_area->height - calc_height);
	  *y_offset = MAX (*y_offset, 0);
	}
    }
  else
    {
      if (x_offset) *x_offset = 0;
      if (y_offset) *y_offset = 0;
    }
}

static void
gtk_cell_renderer_toggle_render (GtkCellRenderer      *cell,
				 GdkDrawable          *window,
				 GtkWidget            *widget,
				 GdkRectangle         *background_area,
				 GdkRectangle         *cell_area,
				 GdkRectangle         *expose_area,
				 GtkCellRendererState  flags)
{
  GtkCellRendererToggle *celltoggle = (GtkCellRendererToggle *) cell;
  GtkCellRendererTogglePrivate *priv;
  gint width, height;
  gint x_offset, y_offset;
  GtkShadowType shadow;
  GtkStateType state = 0;

  priv = gtk_cell_renderer_toggle_get_instance_private (cell);

  gtk_cell_renderer_toggle_get_size (cell, widget, cell_area,
				     &x_offset, &y_offset,
				     &width, &height);
  width -= gtk_cell_renderer_get_props (cell)->xpad*2;
  height -= gtk_cell_renderer_get_props (cell)->ypad*2;

  if (width <= 0 || height <= 0)
    return;

  if (priv->inconsistent)
    shadow = GTK_SHADOW_ETCHED_IN;
  else
    shadow = gtk_cell_renderer_toggle_get_props (celltoggle)->active ? GTK_SHADOW_IN : GTK_SHADOW_OUT;

  if (__gtk_widget_get_state (widget) == GTK_STATE_INSENSITIVE || !gtk_cell_renderer_get_props (cell)->sensitive)
    {
      state = GTK_STATE_INSENSITIVE;
    }
  else if ((flags & GTK_CELL_RENDERER_SELECTED) == GTK_CELL_RENDERER_SELECTED)
    {
      if (__gtk_widget_has_focus (widget))
	state = GTK_STATE_SELECTED;
      else
	state = GTK_STATE_ACTIVE;
    }
  else
    {
      if (gtk_cell_renderer_toggle_get_props (celltoggle)->activatable)
        state = GTK_STATE_NORMAL;
      else
        state = GTK_STATE_INSENSITIVE;
    }

  if (gtk_cell_renderer_toggle_get_props (celltoggle)->radio)
    {
      __gtk_paint_option (gtk_widget_get_props (widget)->style,
                        window,
                        state, shadow,
                        expose_area, widget, "cellradio",
                        cell_area->x + x_offset + gtk_cell_renderer_get_props (cell)->xpad,
                        cell_area->y + y_offset + gtk_cell_renderer_get_props (cell)->ypad,
                        width, height);
    }
  else
    {
      __gtk_paint_check (gtk_widget_get_props (widget)->style,
                       window,
                       state, shadow,
                       expose_area, widget, "cellcheck",
                       cell_area->x + x_offset + gtk_cell_renderer_get_props (cell)->xpad,
                       cell_area->y + y_offset + gtk_cell_renderer_get_props (cell)->ypad,
                       width, height);
    }
}

static gint
gtk_cell_renderer_toggle_activate (GtkCellRenderer      *cell,
				   GdkEvent             *event,
				   GtkWidget            *widget,
				   const gchar          *path,
				   GdkRectangle         *background_area,
				   GdkRectangle         *cell_area,
				   GtkCellRendererState  flags)
{
  GtkCellRendererToggle *celltoggle;
  
  celltoggle = GTK_CELL_RENDERER_TOGGLE (cell);
  if (gtk_cell_renderer_toggle_get_props (celltoggle)->activatable)
    {
      g_signal_emit (cell, toggle_cell_signals[TOGGLED], 0, path);
      return TRUE;
    }

  return FALSE;
}

/**
 * __gtk_cell_renderer_toggle_set_radio:
 * @toggle: a #GtkCellRendererToggle
 * @radio: %TRUE to make the toggle look like a radio button
 * 
 * If @radio is %TRUE, the cell renderer renders a radio toggle
 * (i.e. a toggle in a group of mutually-exclusive toggles).
 * If %FALSE, it renders a check toggle (a standalone boolean option).
 * This can be set globally for the cell renderer, or changed just
 * before rendering each cell in the model (for #GtkTreeView, you set
 * up a per-row setting using #GtkTreeViewColumn to associate model
 * columns with cell renderer properties).
 **/
void
__gtk_cell_renderer_toggle_set_radio (GtkCellRendererToggle *toggle,
				    gboolean               radio)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER_TOGGLE (toggle));

  gtk_cell_renderer_toggle_get_props (toggle)->radio = radio;
}

/**
 * __gtk_cell_renderer_toggle_get_radio:
 * @toggle: a #GtkCellRendererToggle
 *
 * Returns whether we're rendering radio toggles rather than checkboxes. 
 * 
 * Return value: %TRUE if we're rendering radio toggles rather than checkboxes
 **/
gboolean
__gtk_cell_renderer_toggle_get_radio (GtkCellRendererToggle *toggle)
{
  g_return_val_if_fail (GTK_IS_CELL_RENDERER_TOGGLE (toggle), FALSE);

  return gtk_cell_renderer_toggle_get_props (toggle)->radio;
}

/**
 * __gtk_cell_renderer_toggle_get_active:
 * @toggle: a #GtkCellRendererToggle
 *
 * Returns whether the cell renderer is active. See
 * __gtk_cell_renderer_toggle_set_active().
 *
 * Return value: %TRUE if the cell renderer is active.
 **/
gboolean
__gtk_cell_renderer_toggle_get_active (GtkCellRendererToggle *toggle)
{
  g_return_val_if_fail (GTK_IS_CELL_RENDERER_TOGGLE (toggle), FALSE);

  return gtk_cell_renderer_toggle_get_props (toggle)->active;
}

/**
 * __gtk_cell_renderer_toggle_set_active:
 * @toggle: a #GtkCellRendererToggle.
 * @setting: the value to set.
 *
 * Activates or deactivates a cell renderer.
 **/
void
__gtk_cell_renderer_toggle_set_active (GtkCellRendererToggle *toggle,
				     gboolean               setting)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER_TOGGLE (toggle));

  g_object_set (toggle, "active", setting ? TRUE : FALSE, NULL);
}

/**
 * __gtk_cell_renderer_toggle_get_activatable:
 * @toggle: a #GtkCellRendererToggle
 *
 * Returns whether the cell renderer is activatable. See
 * __gtk_cell_renderer_toggle_set_activatable().
 *
 * Return value: %TRUE if the cell renderer is activatable.
 *
 * Since: 2.18
 **/
gboolean
__gtk_cell_renderer_toggle_get_activatable (GtkCellRendererToggle *toggle)
{
  g_return_val_if_fail (GTK_IS_CELL_RENDERER_TOGGLE (toggle), FALSE);

  return gtk_cell_renderer_toggle_get_props (toggle)->activatable;
}

/**
 * __gtk_cell_renderer_toggle_set_activatable:
 * @toggle: a #GtkCellRendererToggle.
 * @setting: the value to set.
 *
 * Makes the cell renderer activatable.
 *
 * Since: 2.18
 **/
void
__gtk_cell_renderer_toggle_set_activatable (GtkCellRendererToggle *toggle,
                                          gboolean               setting)
{
  g_return_if_fail (GTK_IS_CELL_RENDERER_TOGGLE (toggle));

  if (gtk_cell_renderer_toggle_get_props (toggle)->activatable != setting)
    {
      gtk_cell_renderer_toggle_get_props (toggle)->activatable = setting ? TRUE : FALSE;
      g_object_notify (G_OBJECT (toggle), "activatable");
    }
}
