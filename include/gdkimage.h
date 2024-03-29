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

#ifndef __GDK_IMAGE_H__
#define __GDK_IMAGE_H__

#include <stlwrt.h>


#include <gdktypes.h>

G_BEGIN_DECLS


typedef struct _GdkImageClass GdkImageClass;

#define GDK_TYPE_IMAGE              (gdk_image_get_type ())
#define GDK_IMAGE(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_IMAGE, GdkImage))
#define GDK_IMAGE_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_IMAGE, GdkImageClass))
#define GDK_IS_IMAGE(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_IMAGE))
#define GDK_IS_IMAGE_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_IMAGE))
#define GDK_IMAGE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_IMAGE, GdkImageClass))

/*
 * GTK+ 3 doesn't even have GdkImage anymore, so why should we make this
 * a variable-size type?  No real GTK+ 3 application can use this anyway.
 */
STLWRT_DECLARE_FTYPE_FPARENT(GdkImage, gdk_image, GObject,
  /*< public >*/
  
  GdkImageType	 (type); /* read only. */
  GdkVisual    * (visual);	    /* read only. visual used to create the image */
  GdkByteOrder	 (byte_order); /* read only. */
  gint		 (width);  /* read only. */
  gint		 (height); /* read only. */
  guint16	 (depth);  /* read only. */
  guint16	 (bpp);    /* read only. bytes per pixel */
  guint16	 (bpl);    /* read only. bytes per line */
  guint16        (bits_per_pixel); /* read only. bits per pixel */
  gpointer	 (mem);

  GdkColormap  * (colormap); /* read only. */

  /*< private >*/
  gpointer  (windowing_data); /* read only. */
)

struct _GdkImageClass
{
  GObjectClass parent_class;
};


#ifndef GDK_DISABLE_DEPRECATED
GdkImage*  SF(gdk_image_new)       (GdkImageType  type,
				GdkVisual    *visual,
				gint	      width,
				gint	      height);

GdkImage*  SF(gdk_image_get)       (GdkDrawable  *drawable,
				gint	      x,
				gint	      y,
				gint	      width,
				gint	      height);

GdkImage * SF(gdk_image_ref)       (GdkImage     *image);
void       SF(gdk_image_unref)     (GdkImage     *image);

void	   SF(gdk_image_put_pixel) (GdkImage     *image,
				gint	      x,
				gint	      y,
				guint32	      pixel);
guint32	   SF(gdk_image_get_pixel) (GdkImage     *image,
				gint	      x,
				gint	      y);

void       SF(gdk_image_set_colormap) (GdkImage    *image,
                                   GdkColormap *colormap);
GdkColormap* SF(gdk_image_get_colormap) (GdkImage    *image);

GdkImageType  SF(gdk_image_get_image_type)     (GdkImage *image);
GdkVisual    *SF(gdk_image_get_visual)         (GdkImage *image);
GdkByteOrder  SF(gdk_image_get_byte_order)     (GdkImage *image);
gint          SF(gdk_image_get_width)          (GdkImage *image);
gint          SF(gdk_image_get_height)         (GdkImage *image);
guint16       SF(gdk_image_get_depth)          (GdkImage *image);
guint16       SF(gdk_image_get_bytes_per_pixel)(GdkImage *image);
guint16       SF(gdk_image_get_bytes_per_line) (GdkImage *image);
guint16       SF(gdk_image_get_bits_per_pixel) (GdkImage *image);
gpointer      SF(gdk_image_get_pixels)         (GdkImage *image);

#ifdef GDK_ENABLE_BROKEN
GdkImage* SF(gdk_image_new_bitmap) (GdkVisual     *visual,
				gpointer      data,
				gint          width,
				gint          height);
#endif /* GDK_ENABLE_BROKEN */

#define gdk_image_destroy              g_object_unref
#endif /* GDK_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GDK_IMAGE_H__ */
