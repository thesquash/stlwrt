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

#ifndef __GDK_CURSOR_H__
#define __GDK_CURSOR_H__

#include <stlwrt.h>


#include <gdktypes.h>

#include <gdk-pixbuf/gdk-pixbuf.h>

G_BEGIN_DECLS

#define GDK_TYPE_CURSOR (gdk_cursor_get_type ())

STLWRT_DECLARE_BOXED_TYPE(GdkCursor, gdk_cursor,
  GdkCursorType  (type);
  /*< private >*/
  guint  (ref_count);
)

/* Cursors
 */

GdkCursor* SF(gdk_cursor_new_for_display)	 (GdkDisplay      *display,
					  GdkCursorType    cursor_type);
#ifndef GDK_MULTIHEAD_SAFE
GdkCursor* SF(gdk_cursor_new)		 (GdkCursorType	   cursor_type);
#endif
GdkCursor* SF(gdk_cursor_new_from_pixmap)	 (GdkPixmap	  *source,
					  GdkPixmap	  *mask,
					  const GdkColor  *fg,
					  const GdkColor  *bg,
					  gint		   x,
					  gint		   y);
GdkCursor* SF(gdk_cursor_new_from_pixbuf)	 (GdkDisplay      *display,
					  GdkPixbuf       *pixbuf,
					  gint             x,
					  gint             y);
GdkDisplay* SF(gdk_cursor_get_display)	 (GdkCursor	  *cursor);
GdkCursor*  SF(gdk_cursor_ref)               (GdkCursor       *cursor);
void        SF(gdk_cursor_unref)             (GdkCursor       *cursor);
GdkCursor*  SF(gdk_cursor_new_from_name)	 (GdkDisplay      *display,
					  const gchar     *name);
GdkPixbuf*  SF(gdk_cursor_get_image)         (GdkCursor       *cursor);
GdkCursorType SF(gdk_cursor_get_cursor_type) (GdkCursor       *cursor);

#ifndef GDK_DISABLE_DEPRECATED
#define gdk_cursor_destroy             gdk_cursor_unref
#endif /* GDK_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GDK_CURSOR_H__ */
