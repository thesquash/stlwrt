
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
 */

#ifndef __GTK_ADJUSTMENT_H__
#define __GTK_ADJUSTMENT_H__


#include <gdk.h>

G_BEGIN_DECLS

#define GTK_TYPE_ADJUSTMENT                  (gtk_adjustment_get_type ())
#define GTK_ADJUSTMENT(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_ADJUSTMENT, GtkAdjustment))
#define GTK_ADJUSTMENT_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_ADJUSTMENT, GtkAdjustmentClass))
#define GTK_IS_ADJUSTMENT(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_ADJUSTMENT))
#define GTK_IS_ADJUSTMENT_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_ADJUSTMENT))
#define GTK_ADJUSTMENT_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_ADJUSTMENT, GtkAdjustmentClass))


typedef struct _GtkAdjustment	    GtkAdjustmentFat;
typedef struct _GtkAdjustment	    GtkAdjustmentThin;
typedef struct _GtkAdjustmentClass  GtkAdjustmentClassFat;
typedef struct _GtkAdjustmentClass  GtkAdjustmentClassThin;

struct _GtkAdjustment
{
  GObject parent_instance;

  gdouble  (lower);
  gdouble  (upper);
  gdouble  (value);
  gdouble  (step_increment);
  gdouble  (page_increment);
  gdouble  (page_size);
};

struct _GtkAdjustmentClass
{
  GObjectClass parent_class;

  void (* changed)	 (GtkAdjustment *adjustment);
  void (* value_changed) (GtkAdjustment *adjustment);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GType	   _T2_gtk_adjustment_get_type		(void) G_GNUC_CONST;
GType	   _3T_gtk_adjustment_get_type		(void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType	   gtk_adjustment_get_type		(void) G_GNUC_CONST;
GObject* __gtk_adjustment_new			(gdouble	  value,
						 gdouble	  lower,
						 gdouble	  upper,
						 gdouble	  step_increment,
						 gdouble	  page_increment,
						 gdouble	  page_size);

void	   __gtk_adjustment_changed		(GtkAdjustment	 *adjustment);
void	   __gtk_adjustment_value_changed		(GtkAdjustment	 *adjustment);
void	   __gtk_adjustment_clamp_page		(GtkAdjustment	 *adjustment,
						 gdouble	  lower,
						 gdouble	  upper);

gdouble	   __gtk_adjustment_get_value		(GtkAdjustment   *adjustment);
void	   __gtk_adjustment_set_value		(GtkAdjustment	 *adjustment,
						 gdouble	  value);
gdouble    __gtk_adjustment_get_lower             (GtkAdjustment   *adjustment);
void       __gtk_adjustment_set_lower             (GtkAdjustment   *adjustment,
                                                 gdouble          lower);
gdouble    __gtk_adjustment_get_upper             (GtkAdjustment   *adjustment);
void       __gtk_adjustment_set_upper             (GtkAdjustment   *adjustment,
                                                 gdouble          upper);
gdouble    __gtk_adjustment_get_step_increment    (GtkAdjustment   *adjustment);
void       __gtk_adjustment_set_step_increment    (GtkAdjustment   *adjustment,
                                                 gdouble          step_increment);
gdouble    __gtk_adjustment_get_page_increment    (GtkAdjustment   *adjustment);
void       __gtk_adjustment_set_page_increment    (GtkAdjustment   *adjustment,
                                                 gdouble          page_increment);
gdouble    __gtk_adjustment_get_page_size         (GtkAdjustment   *adjustment);
void       __gtk_adjustment_set_page_size         (GtkAdjustment   *adjustment,
                                                 gdouble          page_size);

void       __gtk_adjustment_configure             (GtkAdjustment   *adjustment,
                                                 gdouble          value,
						 gdouble          lower,
						 gdouble          upper,
						 gdouble          step_increment,
						 gdouble          page_increment,
						 gdouble          page_size);

G_END_DECLS

#endif /* __GTK_ADJUSTMENT_H__ */
