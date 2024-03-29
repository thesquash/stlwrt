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

#ifndef __GTK_RANGE_H__
#define __GTK_RANGE_H__

#include <stlwrt.h>


#include <gtkadjustment.h>

#include <gtkwidget.h>


G_BEGIN_DECLS


#define GTK_TYPE_RANGE            (gtk_range_get_type ())
#define GTK_RANGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_RANGE, GtkRange))
#define GTK_RANGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_RANGE, GtkRangeClass))
#define GTK_IS_RANGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_RANGE))
#define GTK_IS_RANGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_RANGE))
#define GTK_RANGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_RANGE, GtkRangeClass))

typedef struct _GtkRangeLayout  GtkRangeLayout;
typedef struct _GtkRangeStepTimer GtkRangeStepTimer;

typedef struct _GtkRangeClass   GtkRangeClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkRange, gtk_range, GtkWidget,
  GtkAdjustment * (adjustment);
  GtkUpdateType  (update_policy);
  guint  (inverted) : 1;

  /*< protected >*/

  guint  (flippable) : 1;

  /* Steppers are: < > ---- < >
   *               a b      c d
   */

  guint  (has_stepper_a) : 1;
  guint  (has_stepper_b) : 1;
  guint  (has_stepper_c) : 1;
  guint  (has_stepper_d) : 1;

  guint  (need_recalc) : 1;

  guint  (slider_size_fixed) : 1;

  gint  (min_slider_size);

  GtkOrientation  (orientation);

  /* Area of entire stepper + trough assembly in widget->window coords */
  GdkRectangle  (range_rect);
  /* Slider range along the long dimension, in widget->window coords */
  gint  (slider_start);
  gint  (slider_end);

  /* Round off value to this many digits, -1 for no rounding */
  gint  (round_digits);

  /*< private >*/
  guint  (trough_click_forward) : 1;  /* trough click was on the forward side of slider */
  guint  (update_pending) : 1;        /* need to emit value_changed */
  GtkRangeLayout * (layout);
  GtkRangeStepTimer * (timer);
  gint  (slide_initial_slider_position);
  gint  (slide_initial_coordinate);
  guint  (update_timeout_id);
  GdkWindow * (event_window);
)

struct _GtkRangeClass
{
  GtkWidgetClass parent_class;

  /* what detail to pass to GTK drawing functions */
  gchar *slider_detail;
  gchar *stepper_detail;

  void (* value_changed)    (GtkRange     *range);
  void (* adjust_bounds)    (GtkRange     *range,
                             gdouble	   new_value);

  /* action signals for keybindings */
  void (* move_slider)      (GtkRange     *range,
                             GtkScrollType scroll);

  /* Virtual functions */
  void (* get_range_border) (GtkRange     *range,
                             GtkBorder    *border_);

  gboolean (* change_value) (GtkRange     *range,
                             GtkScrollType scroll,
                             gdouble       new_value);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
};


#ifndef GTK_DISABLE_DEPRECATED
void               SF(gtk_range_set_update_policy)             (GtkRange      *range,
                                                            GtkUpdateType  policy);
GtkUpdateType      SF(gtk_range_get_update_policy)             (GtkRange      *range);
#endif /* GTK_DISABLE_DEPRECATED */

void               SF(gtk_range_set_adjustment)                (GtkRange      *range,
                                                            GtkAdjustment *adjustment);
GtkAdjustment*     SF(gtk_range_get_adjustment)                (GtkRange      *range);

void               SF(gtk_range_set_inverted)                  (GtkRange      *range,
                                                            gboolean       setting);
gboolean           SF(gtk_range_get_inverted)                  (GtkRange      *range);

void               SF(gtk_range_set_flippable)                 (GtkRange      *range,
                                                            gboolean       flippable);
gboolean           SF(gtk_range_get_flippable)                 (GtkRange      *range);

void               SF(gtk_range_set_slider_size_fixed)         (GtkRange      *range,
                                                            gboolean       size_fixed);
gboolean           SF(gtk_range_get_slider_size_fixed)         (GtkRange      *range);

void               SF(gtk_range_set_min_slider_size)           (GtkRange      *range,
                                                            gboolean       min_size);
gint               SF(gtk_range_get_min_slider_size)           (GtkRange      *range);

void               SF(gtk_range_get_range_rect)                (GtkRange      *range,
                                                            GdkRectangle  *range_rect);
void               SF(gtk_range_get_slider_range)              (GtkRange      *range,
                                                            gint          *slider_start,
                                                            gint          *slider_end);

void               SF(gtk_range_set_lower_stepper_sensitivity) (GtkRange      *range,
                                                            GtkSensitivityType sensitivity);
GtkSensitivityType SF(gtk_range_get_lower_stepper_sensitivity) (GtkRange      *range);
void               SF(gtk_range_set_upper_stepper_sensitivity) (GtkRange      *range,
                                                            GtkSensitivityType sensitivity);
GtkSensitivityType SF(gtk_range_get_upper_stepper_sensitivity) (GtkRange      *range);

void               SF(gtk_range_set_increments)                (GtkRange      *range,
                                                            gdouble        step,
                                                            gdouble        page);
void               SF(gtk_range_set_range)                     (GtkRange      *range,
                                                            gdouble        min,
                                                            gdouble        max);
void               SF(gtk_range_set_value)                     (GtkRange      *range,
                                                            gdouble        value);
gdouble            SF(gtk_range_get_value)                     (GtkRange      *range);

void               SF(gtk_range_set_show_fill_level)           (GtkRange      *range,
                                                            gboolean       show_fill_level);
gboolean           SF(gtk_range_get_show_fill_level)           (GtkRange      *range);
void               SF(gtk_range_set_restrict_to_fill_level)    (GtkRange      *range,
                                                            gboolean       restrict_to_fill_level);
gboolean           SF(gtk_range_get_restrict_to_fill_level)    (GtkRange      *range);
void               SF(gtk_range_set_fill_level)                (GtkRange      *range,
                                                            gdouble        fill_level);
gdouble            SF(gtk_range_get_fill_level)                (GtkRange      *range);
void               SF(gtk_range_set_round_digits)              (GtkRange      *range,
                                                            gint           round_digits);
gint                SF(gtk_range_get_round_digits)             (GtkRange      *range);


/* internal API */
gdouble            SF(_gtk_range_get_wheel_delta)              (GtkRange      *range,
                                                            GdkScrollDirection direction);

void               SF(_gtk_range_set_stop_values)              (GtkRange      *range,
                                                            gdouble       *values,
                                                            gint           n_values);
gint               SF(_gtk_range_get_stop_positions)           (GtkRange      *range,
                                                            gint         **values);          


G_END_DECLS


#endif /* __GTK_RANGE_H__ */
