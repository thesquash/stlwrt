
/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2005 Red Hat, Inc. 
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

#ifndef __GDK_CAIRO_H__
#define __GDK_CAIRO_H__

#include <gdkcolor.h>
#include <gdkpixbuf.h>
#include <pango/pangocairo.h>

G_BEGIN_DECLS

cairo_t *__gdk_cairo_create            (GdkDrawable        *drawable);
void     __gdk_cairo_reset_clip        (cairo_t            *cr,
				      GdkDrawable        *drawable);

void     __gdk_cairo_set_source_color  (cairo_t            *cr,
                                      const GdkColor     *color);
void     __gdk_cairo_set_source_pixbuf (cairo_t            *cr,
                                      const GdkPixbuf    *pixbuf,
                                      double              pixbuf_x,
                                      double              pixbuf_y);
void     __gdk_cairo_set_source_pixmap (cairo_t            *cr,
                                      GdkPixmap          *pixmap,
                                      double              pixmap_x,
                                      double              pixmap_y);
void     __gdk_cairo_set_source_window (cairo_t            *cr,
                                      GdkWindow          *window,
                                      double              x,
                                      double              y);

void     __gdk_cairo_rectangle         (cairo_t            *cr,
                                      const GdkRectangle *rectangle);
void     __gdk_cairo_region            (cairo_t            *cr,
                                      const GdkRegion    *region);

G_END_DECLS

#endif /* __GDK_CAIRO_H__ */