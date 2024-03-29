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

#ifndef __GDK_RGB_H__
#define __GDK_RGB_H__

#include <stlwrt.h>


#include <gdktypes.h>

G_BEGIN_DECLS

typedef struct _GdkRgbCmap GdkRgbCmap;

#ifndef GDK_DISABLE_DEPRECATED

struct _GdkRgbCmap {
  guint32 colors[256];
  gint n_colors;

  /*< private >*/
  GSList *info_list;
};

void SF(gdk_rgb_init) (void);

gulong SF(gdk_rgb_xpixel_from_rgb)   (guint32      rgb) G_GNUC_CONST;
void   SF(gdk_rgb_gc_set_foreground) (GdkGC       *gc,
				  guint32      rgb);
void   SF(gdk_rgb_gc_set_background) (GdkGC       *gc,
				  guint32      rgb);
#define gdk_rgb_get_cmap               gdk_rgb_get_colormap

void   SF(gdk_rgb_find_color)        (GdkColormap *colormap,
				  GdkColor    *color);

void        SF(gdk_draw_rgb_image)              (GdkDrawable  *drawable,
					     GdkGC        *gc,
					     gint          x,
					     gint          y,
					     gint          width,
					     gint          height,
					     GdkRgbDither  dith,
					     const guchar *rgb_buf,
					     gint          rowstride);
void        SF(gdk_draw_rgb_image_dithalign)    (GdkDrawable  *drawable,
					     GdkGC        *gc,
					     gint          x,
					     gint          y,
					     gint          width,
					     gint          height,
					     GdkRgbDither  dith,
					     const guchar *rgb_buf,
					     gint          rowstride,
					     gint          xdith,
					     gint          ydith);
void        SF(gdk_draw_rgb_32_image)           (GdkDrawable  *drawable,
					     GdkGC        *gc,
					     gint          x,
					     gint          y,
					     gint          width,
					     gint          height,
					     GdkRgbDither  dith,
					     const guchar *buf,
					     gint          rowstride);
void        SF(gdk_draw_rgb_32_image_dithalign) (GdkDrawable  *drawable,
					     GdkGC        *gc,
					     gint          x,
					     gint          y,
					     gint          width,
					     gint          height,
					     GdkRgbDither  dith,
					     const guchar *buf,
					     gint          rowstride,
					     gint          xdith,
					     gint          ydith);
void        SF(gdk_draw_gray_image)             (GdkDrawable  *drawable,
					     GdkGC        *gc,
					     gint          x,
					     gint          y,
					     gint          width,
					     gint          height,
					     GdkRgbDither  dith,
					     const guchar *buf,
					     gint          rowstride);
void        SF(gdk_draw_indexed_image)          (GdkDrawable  *drawable,
					     GdkGC        *gc,
					     gint          x,
					     gint          y,
					     gint          width,
					     gint          height,
					     GdkRgbDither  dith,
					     const guchar *buf,
					     gint          rowstride,
					     GdkRgbCmap   *cmap);
GdkRgbCmap *SF(gdk_rgb_cmap_new)                (guint32      *colors,
					     gint          n_colors);
void        SF(gdk_rgb_cmap_free)               (GdkRgbCmap   *cmap);

void     SF(gdk_rgb_set_verbose) (gboolean verbose);

/* experimental colormap stuff */
void SF(gdk_rgb_set_install)    (gboolean install);
void SF(gdk_rgb_set_min_colors) (gint     min_colors);

#ifndef GDK_MULTIHEAD_SAFE
GdkColormap *SF(gdk_rgb_get_colormap) (void);
GdkVisual *  SF(gdk_rgb_get_visual)   (void);
gboolean     SF(gdk_rgb_ditherable)   (void);
gboolean     SF(gdk_rgb_colormap_ditherable) (GdkColormap *cmap);
#endif
#endif /* GDK_DISABLE_DEPRECATED */

G_END_DECLS


#endif /* __GDK_RGB_H__ */
