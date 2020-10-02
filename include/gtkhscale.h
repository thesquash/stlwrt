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

#ifndef __GTK_HSCALE_H__
#define __GTK_HSCALE_H__

#include <stlwrt.h>



#include <gtkscale.h>


G_BEGIN_DECLS

#define GTK_TYPE_HSCALE            (gtk_hscale_get_type ())
#define GTK_HSCALE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_HSCALE, GtkHScale))
#define GTK_HSCALE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_HSCALE, GtkHScaleClass))
#define GTK_IS_HSCALE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_HSCALE))
#define GTK_IS_HSCALE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_HSCALE))
#define GTK_HSCALE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_HSCALE, GtkHScaleClass))

typedef struct _GtkHScaleClass  GtkHScaleClass;

STLWRT_DECLARE_FTYPE_VPARENT(GtkHScale, gtk_hscale, GtkScale, ;)

struct _GtkHScaleClass
{
  GtkScaleClass parent_class;
};


GtkWidget* SF(gtk_hscale_new)            (GtkAdjustment *adjustment);
GtkWidget* SF(gtk_hscale_new_with_range) (gdouble        min,
                                      gdouble        max,
                                      gdouble        step);


G_END_DECLS

#endif /* __GTK_HSCALE_H__ */
