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



#ifndef __GDK_PIXMAP_X11_H__
#define __GDK_PIXMAP_X11_H__

#include <gdkdrawable-x11.h>
#include <gdkpixmap.h>

G_BEGIN_DECLS

/* Pixmap implementation for X11
 */

typedef struct _GdkPixmapImplX11Class GdkPixmapImplX11Class;

#define GDK_TYPE_PIXMAP_IMPL_X11              (gdk_pixmap_impl_x11_get_type ())
#define GDK_PIXMAP_IMPL_X11(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_PIXMAP_IMPL_X11, GdkPixmapImplX11))
#define GDK_PIXMAP_IMPL_X11_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_PIXMAP_IMPL_X11, GdkPixmapImplX11Class))
#define GDK_IS_PIXMAP_IMPL_X11(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_PIXMAP_IMPL_X11))
#define GDK_IS_PIXMAP_IMPL_X11_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_PIXMAP_IMPL_X11))
#define GDK_PIXMAP_IMPL_X11_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_PIXMAP_IMPL_X11, GdkPixmapImplX11Class))

STLWRT_DECLARE_FTYPE_FPARENT (GdkPixmapImplX11, gdk_pixmap_impl_x11, GdkDrawableImplX11,
  gint width;
  gint height;

  guint is_foreign : 1;
)
 
struct _GdkPixmapImplX11Class 
{
  GdkDrawableImplX11Class parent_class;

};

G_END_DECLS

#endif /* __GDK_PIXMAP_X11_H__ */
