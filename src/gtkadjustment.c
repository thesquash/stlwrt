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
#include <gtkadjustment.h>

#include <gtkprivate.h>
#include <gtkintl.h>


enum
{
  PROP_0,
  PROP_VALUE,
  PROP_LOWER,
  PROP_UPPER,
  PROP_STEP_INCREMENT,
  PROP_PAGE_INCREMENT,
  PROP_PAGE_SIZE
};

enum
{
  CHANGED,
  VALUE_CHANGED,
  LAST_SIGNAL
};


static void gtk_adjustment_get_property                (GObject      *object,
                                                        guint         prop_id,
                                                        GValue       *value,
                                                        GParamSpec   *pspec);
static void gtk_adjustment_set_property                (GObject      *object,
                                                        guint         prop_id,
                                                        const GValue *value,
                                                        GParamSpec   *pspec);
static void gtk_adjustment_dispatch_properties_changed (GObject      *object,
                                                        guint         n_pspecs,
                                                        GParamSpec  **pspecs);

static guint adjustment_signals[LAST_SIGNAL] = { 0 };

static guint64 adjustment_changed_stamp = 0; /* protected by global gdk lock */

STLWRT_DEFINE_VTYPE (GtkAdjustment, gtk_adjustment, G_TYPE_OBJECT, G_TYPE_FLAG_NONE, ;)

static void
gtk_adjustment_class_init (GtkAdjustmentClass *class)
{
  GObjectClass *gobject_class = G_OBJECT_CLASS (class);

  gobject_class->set_property                = gtk_adjustment_set_property;
  gobject_class->get_property                = gtk_adjustment_get_property;
  gobject_class->dispatch_properties_changed = gtk_adjustment_dispatch_properties_changed;

  class->changed = NULL;
  class->value_changed = NULL;

  /**
   * GtkAdjustment:value:
   * 
   * The value of the adjustment.
   * 
   * Since: 2.4
   */
  g_object_class_install_property (gobject_class,
                                   PROP_VALUE,
                                   g_param_spec_double ("value",
							P_("Value"),
							P_("The value of the adjustment"),
							-G_MAXDOUBLE, 
							G_MAXDOUBLE, 
							0.0, 
							GTK_PARAM_READWRITE));
  
  /**
   * GtkAdjustment:lower:
   * 
   * The minimum value of the adjustment.
   * 
   * Since: 2.4
   */
  g_object_class_install_property (gobject_class,
                                   PROP_LOWER,
                                   g_param_spec_double ("lower",
							P_("Minimum Value"),
							P_("The minimum value of the adjustment"),
							-G_MAXDOUBLE, 
							G_MAXDOUBLE, 
							0.0,
							GTK_PARAM_READWRITE));
  
  /**
   * GtkAdjustment:upper:
   * 
   * The maximum value of the adjustment. 
   * Note that values will be restricted by 
   * <literal>upper - page-size</literal> if the page-size 
   * property is nonzero.
   *
   * Since: 2.4
   */
  g_object_class_install_property (gobject_class,
                                   PROP_UPPER,
                                   g_param_spec_double ("upper",
							P_("Maximum Value"),
							P_("The maximum value of the adjustment"),
							-G_MAXDOUBLE, 
							G_MAXDOUBLE, 
							0.0, 
							GTK_PARAM_READWRITE));
  
  /**
   * GtkAdjustment:step-increment:
   * 
   * The step increment of the adjustment.
   * 
   * Since: 2.4
   */
  g_object_class_install_property (gobject_class,
                                   PROP_STEP_INCREMENT,
                                   g_param_spec_double ("step-increment",
							P_("Step Increment"),
							P_("The step increment of the adjustment"),
							-G_MAXDOUBLE, 
							G_MAXDOUBLE, 
							0.0, 
							GTK_PARAM_READWRITE));
  
  /**
   * GtkAdjustment:page-increment:
   * 
   * The page increment of the adjustment.
   * 
   * Since: 2.4
   */
  g_object_class_install_property (gobject_class,
                                   PROP_PAGE_INCREMENT,
                                   g_param_spec_double ("page-increment",
							P_("Page Increment"),
							P_("The page increment of the adjustment"),
							-G_MAXDOUBLE, 
							G_MAXDOUBLE, 
							0.0, 
							GTK_PARAM_READWRITE));
  
  /**
   * GtkAdjustment:page-size:
   * 
   * The page size of the adjustment. 
   * Note that the page-size is irrelevant and should be set to zero
   * if the adjustment is used for a simple scalar value, e.g. in a 
   * #GtkSpinButton.
   * 
   * Since: 2.4
   */
  g_object_class_install_property (gobject_class,
                                   PROP_PAGE_SIZE,
                                   g_param_spec_double ("page-size",
							P_("Page Size"),
							P_("The page size of the adjustment"),
							-G_MAXDOUBLE, 
							G_MAXDOUBLE, 
							0.0, 
							GTK_PARAM_READWRITE));

  adjustment_signals[CHANGED] =
    g_signal_new (I_("changed"),
		  G_OBJECT_CLASS_TYPE (class),
		  G_SIGNAL_RUN_FIRST | G_SIGNAL_NO_RECURSE,
		  G_STRUCT_OFFSET (GtkAdjustmentClass, changed),
		  NULL, NULL,
		  NULL,
		  G_TYPE_NONE, 0);

  adjustment_signals[VALUE_CHANGED] =
    g_signal_new (I_("value-changed"),
		  G_OBJECT_CLASS_TYPE (class),
		  G_SIGNAL_RUN_FIRST | G_SIGNAL_NO_RECURSE,
		  G_STRUCT_OFFSET (GtkAdjustmentClass, value_changed),
		  NULL, NULL,
		  NULL,
		  G_TYPE_NONE, 0);
}

static void
gtk_adjustment_init (GtkAdjustment *adjustment)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  adjustment_props->value = 0.0;
  adjustment_props->lower = 0.0;
  adjustment_props->upper = 0.0;
  adjustment_props->step_increment = 0.0;
  adjustment_props->page_increment = 0.0;
  adjustment_props->page_size = 0.0;
}

static void
gtk_adjustment_get_property (GObject    *object,
                             guint       prop_id,
                             GValue     *value,
                             GParamSpec *pspec)
{
  GtkAdjustment *adjustment = GTK_ADJUSTMENT (object);
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  switch (prop_id)
    {
    case PROP_VALUE:
      g_value_set_double (value, adjustment_props->value);
      break;
    case PROP_LOWER:
      g_value_set_double (value, adjustment_props->lower);
      break;
    case PROP_UPPER:
      g_value_set_double (value, adjustment_props->upper);
      break;
    case PROP_STEP_INCREMENT:
      g_value_set_double (value, adjustment_props->step_increment);
      break;
    case PROP_PAGE_INCREMENT:
      g_value_set_double (value, adjustment_props->page_increment);
      break;
    case PROP_PAGE_SIZE:
      g_value_set_double (value, adjustment_props->page_size);
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_adjustment_set_property (GObject      *object,
                             guint         prop_id,
                             const GValue *value,
                             GParamSpec   *pspec)
{
  GtkAdjustment *adjustment = GTK_ADJUSTMENT (object);
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);
  gdouble double_value = g_value_get_double (value);

  switch (prop_id)
    {
    case PROP_VALUE:
      __gtk_adjustment_set_value (adjustment, double_value);
      break;
    case PROP_LOWER:
      adjustment_props->lower = double_value;
      break;
    case PROP_UPPER:
      adjustment_props->upper = double_value;
      break;
    case PROP_STEP_INCREMENT:
      adjustment_props->step_increment = double_value;
      break;
    case PROP_PAGE_INCREMENT:
      adjustment_props->page_increment = double_value;
      break;
    case PROP_PAGE_SIZE:
      adjustment_props->page_size = double_value;
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
      break;
    }
}

static void
gtk_adjustment_dispatch_properties_changed (GObject     *object,
                                            guint        n_pspecs,
                                            GParamSpec **pspecs)
{
  gboolean changed = FALSE;
  gint i;

  G_OBJECT_CLASS (gtk_adjustment_parent_class)->dispatch_properties_changed (object, n_pspecs, pspecs);

  for (i = 0; i < n_pspecs; i++)
    switch (pspecs[i]->param_id)
      {
      case PROP_LOWER:
      case PROP_UPPER:
      case PROP_STEP_INCREMENT:
      case PROP_PAGE_INCREMENT:
      case PROP_PAGE_SIZE:
        changed = TRUE;
        break;
      default:
        break;
      }

  if (changed)
    {
      adjustment_changed_stamp++;
      __gtk_adjustment_changed (GTK_ADJUSTMENT (object));
    }
}

GObject *
__gtk_adjustment_new (gdouble value,
		    gdouble lower,
		    gdouble upper,
		    gdouble step_increment,
		    gdouble page_increment,
		    gdouble page_size)
{
  return g_object_new (GTK_TYPE_ADJUSTMENT,
		       "lower", lower,
		       "upper", upper,
		       "step-increment", step_increment,
		       "page-increment", page_increment,
		       "page-size", page_size,
		       "value", value,
		       NULL);
}

/**
 * __gtk_adjustment_get_value:
 * @adjustment: a #GtkAdjustment
 *
 * Gets the current value of the adjustment. See
 * __gtk_adjustment_set_value ().
 *
 * Return value: The current value of the adjustment.
 **/
gdouble
__gtk_adjustment_get_value (GtkAdjustment *adjustment)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_val_if_fail (GTK_IS_ADJUSTMENT (adjustment), 0.0);

  return adjustment_props->value;
}

void
__gtk_adjustment_set_value (GtkAdjustment *adjustment,
			  gdouble        value)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  value = CLAMP (value, adjustment_props->lower, adjustment_props->upper);

  if (value != adjustment_props->value)
    {
      adjustment_props->value = value;

      __gtk_adjustment_value_changed (adjustment);
    }
}

/**
 * __gtk_adjustment_get_lower:
 * @adjustment: a #GtkAdjustment
 *
 * Retrieves the minimum value of the adjustment.
 *
 * Return value: The current minimum value of the adjustment.
 *
 * Since: 2.14
 **/
gdouble
__gtk_adjustment_get_lower (GtkAdjustment *adjustment)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_val_if_fail (GTK_IS_ADJUSTMENT (adjustment), 0.0);

  return adjustment_props->lower;
}

/**
 * __gtk_adjustment_set_lower:
 * @adjustment: a #GtkAdjustment
 * @lower: the new minimum value
 *
 * Sets the minimum value of the adjustment.
 *
 * When setting multiple adjustment properties via their individual
 * setters, multiple "changed" signals will be emitted. However, since
 * the emission of the "changed" signal is tied to the emission of the
 * "GObject::notify" signals of the changed properties, it's possible
 * to compress the "changed" signals into one by calling
 * g_object_freeze_notify() and g_object_thaw_notify() around the
 * calls to the individual setters.
 *
 * Alternatively, using a single g_object_set() for all the properties
 * to change, or using __gtk_adjustment_configure() has the same effect
 * of compressing "changed" emissions.
 *
 * Since: 2.14
 **/
void
__gtk_adjustment_set_lower (GtkAdjustment *adjustment,
                          gdouble        lower)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  if (lower != adjustment_props->lower)
    g_object_set (adjustment, "lower", lower, NULL);
}

/**
 * __gtk_adjustment_get_upper:
 * @adjustment: a #GtkAdjustment
 *
 * Retrieves the maximum value of the adjustment.
 *
 * Return value: The current maximum value of the adjustment.
 *
 * Since: 2.14
 **/
gdouble
__gtk_adjustment_get_upper (GtkAdjustment *adjustment)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_val_if_fail (GTK_IS_ADJUSTMENT (adjustment), 0.0);

  return adjustment_props->upper;
}

/**
 * __gtk_adjustment_set_upper:
 * @adjustment: a #GtkAdjustment
 * @upper: the new maximum value
 *
 * Sets the maximum value of the adjustment.
 *
 * Note that values will be restricted by
 * <literal>upper - page-size</literal> if the page-size
 * property is nonzero.
 *
 * See __gtk_adjustment_set_lower() about how to compress multiple
 * emissions of the "changed" signal when setting multiple adjustment
 * properties.
 *
 * Since: 2.14
 **/
void
__gtk_adjustment_set_upper (GtkAdjustment *adjustment,
                          gdouble        upper)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  if (upper != adjustment_props->upper)
    g_object_set (adjustment, "upper", upper, NULL);
}

/**
 * __gtk_adjustment_get_step_increment:
 * @adjustment: a #GtkAdjustment
 *
 * Retrieves the step increment of the adjustment.
 *
 * Return value: The current step increment of the adjustment.
 *
 * Since: 2.14
 **/
gdouble
__gtk_adjustment_get_step_increment (GtkAdjustment *adjustment)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_val_if_fail (GTK_IS_ADJUSTMENT (adjustment), 0.0);

  return adjustment_props->step_increment;
}

/**
 * __gtk_adjustment_set_step_increment:
 * @adjustment: a #GtkAdjustment
 * @step_increment: the new step increment
 *
 * Sets the step increment of the adjustment.
 *
 * See __gtk_adjustment_set_lower() about how to compress multiple
 * emissions of the "changed" signal when setting multiple adjustment
 * properties.
 *
 * Since: 2.14
 **/
void
__gtk_adjustment_set_step_increment (GtkAdjustment *adjustment,
                                   gdouble        step_increment)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  if (step_increment != adjustment_props->step_increment)
    g_object_set (adjustment, "step-increment", step_increment, NULL);
}

/**
 * __gtk_adjustment_get_page_increment:
 * @adjustment: a #GtkAdjustment
 *
 * Retrieves the page increment of the adjustment.
 *
 * Return value: The current page increment of the adjustment.
 *
 * Since: 2.14
 **/
gdouble
__gtk_adjustment_get_page_increment (GtkAdjustment *adjustment)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_val_if_fail (GTK_IS_ADJUSTMENT (adjustment), 0.0);

  return adjustment_props->page_increment;
}

/**
 * __gtk_adjustment_set_page_increment:
 * @adjustment: a #GtkAdjustment
 * @page_increment: the new page increment
 *
 * Sets the page increment of the adjustment.
 *
 * See __gtk_adjustment_set_lower() about how to compress multiple
 * emissions of the "changed" signal when setting multiple adjustment
 * properties.
 *
 * Since: 2.14
 **/
void
__gtk_adjustment_set_page_increment (GtkAdjustment *adjustment,
                                   gdouble        page_increment)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  if (page_increment != adjustment_props->page_increment)
    g_object_set (adjustment, "page-increment", page_increment, NULL);
}

/**
 * __gtk_adjustment_get_page_size:
 * @adjustment: a #GtkAdjustment
 *
 * Retrieves the page size of the adjustment.
 *
 * Return value: The current page size of the adjustment.
 *
 * Since: 2.14
 **/
gdouble
__gtk_adjustment_get_page_size (GtkAdjustment *adjustment)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_val_if_fail (GTK_IS_ADJUSTMENT (adjustment), 0.0);

  return adjustment_props->page_size;
}

/**
 * __gtk_adjustment_set_page_size:
 * @adjustment: a #GtkAdjustment
 * @page_size: the new page size
 *
 * Sets the page size of the adjustment.
 *
 * See __gtk_adjustment_set_lower() about how to compress multiple
 * emissions of the "changed" signal when setting multiple adjustment
 * properties.
 *
 * Since: 2.14
 **/
void
__gtk_adjustment_set_page_size (GtkAdjustment *adjustment,
                              gdouble        page_size)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  if (page_size != adjustment_props->page_size)
    g_object_set (adjustment, "page-size", page_size, NULL);
}

/**
 * __gtk_adjustment_configure:
 * @adjustment: a #GtkAdjustment
 * @value: the new value
 * @lower: the new minimum value
 * @upper: the new maximum value
 * @step_increment: the new step increment
 * @page_increment: the new page increment
 * @page_size: the new page size
 *
 * Sets all properties of the adjustment at once.
 *
 * Use this function to avoid multiple emissions of the "changed"
 * signal. See __gtk_adjustment_set_lower() for an alternative way
 * of compressing multiple emissions of "changed" into one.
 *
 * Since: 2.14
 **/
void
__gtk_adjustment_configure (GtkAdjustment *adjustment,
                          gdouble        value,
                          gdouble        lower,
                          gdouble        upper,
                          gdouble        step_increment,
                          gdouble        page_increment,
                          gdouble        page_size)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  gboolean value_changed = FALSE;
  guint64 old_stamp = adjustment_changed_stamp;

  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  g_object_freeze_notify (G_OBJECT (adjustment));

  g_object_set (adjustment,
                "lower", lower,
                "upper", upper,
                "step-increment", step_increment,
                "page-increment", page_increment,
                "page-size", page_size,
                NULL);

  /* don't use CLAMP() so we don't end up below lower if upper - page_size
   * is smaller than lower
   */
  value = MIN (value, upper - page_size);
  value = MAX (value, lower);

  if (value != adjustment_props->value)
    {
      /* set value manually to make sure "changed" is emitted with the
       * new value in place and is emitted before "value-changed"
       */
      adjustment_props->value = value;
      value_changed = TRUE;
    }

  g_object_thaw_notify (G_OBJECT (adjustment));

  if (old_stamp == adjustment_changed_stamp)
    __gtk_adjustment_changed (adjustment); /* force emission before ::value-changed */

  if (value_changed)
    __gtk_adjustment_value_changed (adjustment);
}

void
__gtk_adjustment_changed (GtkAdjustment *adjustment)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  g_signal_emit (adjustment, adjustment_signals[CHANGED], 0);
}

void
__gtk_adjustment_value_changed (GtkAdjustment *adjustment)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);

  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  g_signal_emit (adjustment, adjustment_signals[VALUE_CHANGED], 0);
  g_object_notify (G_OBJECT (adjustment), "value");
}

void
__gtk_adjustment_clamp_page (GtkAdjustment *adjustment,
			   gdouble        lower,
			   gdouble        upper)
{
  GtkAdjustmentProps *adjustment_props = gtk_adjustment_get_props (adjustment);
  gboolean need_emission;

  g_return_if_fail (GTK_IS_ADJUSTMENT (adjustment));

  lower = CLAMP (lower, adjustment_props->lower, adjustment_props->upper);
  upper = CLAMP (upper, adjustment_props->lower, adjustment_props->upper);

  need_emission = FALSE;

  if (adjustment_props->value + adjustment_props->page_size < upper)
    {
      adjustment_props->value = upper - adjustment_props->page_size;
      need_emission = TRUE;
    }
  if (adjustment_props->value > lower)
    {
      adjustment_props->value = lower;
      need_emission = TRUE;
    }

  if (need_emission)
    __gtk_adjustment_value_changed (adjustment);
}
