
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

#ifndef __GTK_SCALE_H__
#define __GTK_SCALE_H__

#include <gtkrange.h>


G_BEGIN_DECLS

#define GTK_TYPE_SCALE            (gtk_scale_get_type ())
#define GTK_SCALE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SCALE, GtkScale))
#define GTK_SCALE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SCALE, GtkScaleClass))
#define GTK_IS_SCALE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SCALE))
#define GTK_IS_SCALE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SCALE))
#define GTK_SCALE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_SCALE, GtkScaleClass))


typedef struct _GtkScale        GtkScaleFat;
typedef struct _GtkScale        GtkScaleThin;

typedef struct _GtkScaleClass   GtkScaleClass;


struct _GtkScalePrivate
{
  PangoLayout *layout;
  GSList      *marks;
};

struct _GtkScale
{
  GtkRange range;

  gint   (digits);
  guint  (draw_value) : 1;
  guint  (value_pos) : 2;
};

struct _GtkScaleClass
{
  GtkRangeClass parent_class;

  gchar* (* format_value) (GtkScale *scale,
                           gdouble   value);

  void (* draw_value) (GtkScale *scale);

  void (* get_layout_offsets) (GtkScale *scale,
                               gint     *x,
                               gint     *y);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
};

GType             _T2_gtk_scale_get_type           (void) G_GNUC_CONST;
GType             _3T_gtk_scale_get_type           (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType             gtk_scale_get_type           (void) G_GNUC_CONST;

GtkWidget*        __gtk_scale_new                (GtkOrientation orientation,
                                                GtkAdjustment *adjustment);
GtkWidget* 	  __gtk_scale_new_with_range     (GtkOrientation orientation,
                                                gdouble        min,
                                                gdouble        max,
                                                gdouble        step);

void              __gtk_scale_set_digits         (GtkScale        *scale,
                                                gint             digits);
gint              __gtk_scale_get_digits         (GtkScale        *scale);
void              __gtk_scale_set_draw_value     (GtkScale        *scale,
                                                gboolean         draw_value);
gboolean          __gtk_scale_get_draw_value     (GtkScale        *scale);
void              __gtk_scale_set_value_pos      (GtkScale        *scale,
                                                GtkPositionType  pos);
GtkPositionType   __gtk_scale_get_value_pos      (GtkScale        *scale);

PangoLayout     * __gtk_scale_get_layout         (GtkScale        *scale);
void              __gtk_scale_get_layout_offsets (GtkScale        *scale,
                                                gint            *x,
                                                gint            *y);

void              __gtk_scale_add_mark           (GtkScale        *scale,
                                                gdouble          value,
                                                GtkPositionType  position,
                                                const gchar     *markup);
void              __gtk_scale_clear_marks        (GtkScale        *scale);

/* internal API */
void              ___gtk_scale_clear_layout      (GtkScale        *scale);
void              ___gtk_scale_get_value_size    (GtkScale        *scale,
                                                gint            *width,
                                                gint            *height);
gchar           * ___gtk_scale_format_value      (GtkScale        *scale,
                                                gdouble          value);

G_END_DECLS

#endif /* __GTK_SCALE_H__ */
