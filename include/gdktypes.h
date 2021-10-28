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

#ifndef __GDK_TYPES_H__
#define __GDK_TYPES_H__

#include <stlwrt.h>


#include <glib.h>

#include <pango/pango.h>

#include <glib-object.h>

#ifdef G_OS_WIN32
#  ifdef STLWRT_COMPILATION
#    define GDKVAR __declspec(dllexport)
#  else
#    define GDKVAR extern __declspec(dllimport)
#  endif
#else
#  define GDKVAR extern
#endif

/* The system specific file gdkconfig.h contains such configuration
 * settings that are needed not only when compiling GDK (or GTK)
 * itself, but also occasionally when compiling programs that use GDK
 * (or GTK). One such setting is what windowing API backend is in use.
 */

#include <gdkconfig.h>

/* some common magic values */
#define GDK_CURRENT_TIME     0L
#define GDK_PARENT_RELATIVE  1L



G_BEGIN_DECLS

#define GDK_TYPE_RECTANGLE (gdk_rectangle_get_type ())

/* Type definitions for the basic structures.
 */
typedef struct _GdkPoint	      GdkPoint;
typedef struct _GdkRectangle	      GdkRectangle;
typedef struct _GdkSegment	      GdkSegment;
typedef struct _GdkSpan	              GdkSpan;

/*
 * Note that on some platforms the wchar_t type
 * is not the same as GdkWChar. For instance
 * on Win32, wchar_t is unsigned short.
 */
typedef guint32			    GdkWChar;

typedef struct _GdkAtom            *GdkAtom;

#define GDK_ATOM_TO_POINTER(atom) (atom)
#define GDK_POINTER_TO_ATOM(ptr)  ((GdkAtom)(ptr))

#ifdef GDK_NATIVE_WINDOW_POINTER
#define GDK_GPOINTER_TO_NATIVE_WINDOW(p) ((GdkNativeWindow) (p))
#else
#define GDK_GPOINTER_TO_NATIVE_WINDOW(p) GPOINTER_TO_UINT(p)
#endif

#define _GDK_MAKE_ATOM(val) ((GdkAtom)GUINT_TO_POINTER(val))
#define GDK_NONE            _GDK_MAKE_ATOM (0)

#ifdef GDK_NATIVE_WINDOW_POINTER
typedef gpointer GdkNativeWindow;
#else
typedef guint32 GdkNativeWindow;
#endif

typedef void (*GdkInputFunction) (gpointer	    data,
				  gint		    source,
				  GdkInputCondition condition);

#ifndef GDK_DISABLE_DEPRECATED

typedef void (*GdkDestroyNotify) (gpointer data);

#endif /* GDK_DISABLE_DEPRECATED */

struct _GdkPoint
{
  gint x;
  gint y;
};

STLWRT_DECLARE_BOXED_TYPE(GdkRectangle, gdk_rectangle,
  gint x;
  gint y;
  gint width;
  gint height;
)

struct _GdkSegment
{
  gint x1;
  gint y1;
  gint x2;
  gint y2;
};

struct _GdkSpan
{
  gint x;
  gint y;
  gint width;
};


/* Rectangle utilities
 */

gboolean SF(gdk_rectangle_intersect) (const GdkRectangle *src1,
				  const GdkRectangle *src2,
				  GdkRectangle       *dest);
void     SF(gdk_rectangle_union)     (const GdkRectangle *src1,
				  const GdkRectangle *src2,
				  GdkRectangle       *dest);

G_END_DECLS


#endif /* __GDK_TYPES_H__ */
