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

#ifndef __GTK_PROGRESS_BAR_H__
#define __GTK_PROGRESS_BAR_H__

#include <stlwrt.h>

G_BEGIN_DECLS

#define GTK_TYPE_PROGRESS_BAR            (gtk_progress_bar_get_type ())
#define GTK_PROGRESS_BAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PROGRESS_BAR, GtkProgressBar))
#define GTK_PROGRESS_BAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_PROGRESS_BAR, GtkProgressBarClass))
#define GTK_IS_PROGRESS_BAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PROGRESS_BAR))
#define GTK_IS_PROGRESS_BAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_PROGRESS_BAR))
#define GTK_PROGRESS_BAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_PROGRESS_BAR, GtkProgressBarClass))

typedef struct _GtkProgressBarClass  GtkProgressBarClass;

typedef enum 
{
  GTK_PROGRESS_CONTINUOUS,
  GTK_PROGRESS_DISCRETE
} GtkProgressBarStyle;

typedef enum 
{
  GTK_PROGRESS_LEFT_TO_RIGHT,
  GTK_PROGRESS_RIGHT_TO_LEFT,
  GTK_PROGRESS_BOTTOM_TO_TOP,
  GTK_PROGRESS_TOP_TO_BOTTOM
} GtkProgressBarOrientation;

STLWRT_DECLARE_VTYPE_VPARENT(GtkProgressBar, gtk_progress_bar, GtkWidget,
  /* From gtkprogress.h */
  GtkAdjustment *adjustment;
  GdkPixmap     *offscreen_pixmap;
  gchar         *format;
  gfloat         x_align;
  gfloat         y_align;

  guint          show_text : 1;
  guint          activity_mode : 1;
  guint          use_text_format : 1;

  /* From gtkprogressbar.h */
  GtkProgressBarStyle        (bar_style);
  GtkProgressBarOrientation  (orientation);

  guint  (blocks);
  gint   (in_block);

  gint   (activity_pos);
  guint  (activity_step);
  guint  (activity_blocks);

  gdouble  (pulse_fraction);

  guint  (activity_dir) : 1;
  guint  (ellipsize) : 3;
  guint  (dirty) : 1;
)

struct _GtkProgressBarClass
{
  GtkWidgetClass parent_class;

  void (* paint)            (GtkProgressBar *progress);
  void (* update)           (GtkProgressBar *progress);
  void (* act_mode_enter)   (GtkProgressBar *progress);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);

  /*
   * I don't think any GTK+ program that anybody still wants to run subclassed
   * GtkProgress or GtkProgressBar -- for the time being, delete the extra
   * padding from GtkProgressBar.
   */
};


GtkWidget* SF(gtk_progress_bar_new)                  (void);

/*
 * Long story short, GTK+ 2 still contained deprecated functions for
 * manipulating progress bars, and I finally removed the old, unuseful
 * functions from STLWRT.  All that remains is the "new" GTK+ 2 and
 * later interface for progress bars.
 */
void       SF(gtk_progress_bar_pulse)                (GtkProgressBar *pbar);
void       SF(gtk_progress_bar_set_text)             (GtkProgressBar *pbar,
                                                  const gchar    *text);
void       SF(gtk_progress_bar_set_fraction)         (GtkProgressBar *pbar,
                                                  gdouble         fraction);

void       SF(gtk_progress_bar_set_pulse_step)       (GtkProgressBar *pbar,
                                                  gdouble         fraction);
void       SF(gtk_progress_bar_set_orientation)      (GtkProgressBar *pbar,
						  GtkProgressBarOrientation orientation);

const gchar*          SF(gtk_progress_bar_get_text)       (GtkProgressBar *pbar);
gdouble               SF(gtk_progress_bar_get_fraction)   (GtkProgressBar *pbar);
gdouble               SF(gtk_progress_bar_get_pulse_step) (GtkProgressBar *pbar);

GtkProgressBarOrientation SF(gtk_progress_bar_get_orientation) (GtkProgressBar *pbar);
void               SF(gtk_progress_bar_set_ellipsize) (GtkProgressBar     *pbar,
						   PangoEllipsizeMode  mode);
PangoEllipsizeMode SF(gtk_progress_bar_get_ellipsize) (GtkProgressBar     *pbar);

G_END_DECLS

#endif /* __GTK_PROGRESS_BAR_H__ */
