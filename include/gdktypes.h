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

typedef enum
{
  GDK_LSB_FIRST,
  GDK_MSB_FIRST
} GdkByteOrder;

/* Types of modifiers.
 */
typedef enum
{
  GDK_SHIFT_MASK    = 1 << 0,
  GDK_LOCK_MASK	    = 1 << 1,
  GDK_CONTROL_MASK  = 1 << 2,
  GDK_MOD1_MASK	    = 1 << 3,
  GDK_MOD2_MASK	    = 1 << 4,
  GDK_MOD3_MASK	    = 1 << 5,
  GDK_MOD4_MASK	    = 1 << 6,
  GDK_MOD5_MASK	    = 1 << 7,
  GDK_BUTTON1_MASK  = 1 << 8,
  GDK_BUTTON2_MASK  = 1 << 9,
  GDK_BUTTON3_MASK  = 1 << 10,
  GDK_BUTTON4_MASK  = 1 << 11,
  GDK_BUTTON5_MASK  = 1 << 12,

  /* The next few modifiers are used by XKB, so we skip to the end.
   * Bits 15 - 25 are currently unused. Bit 29 is used internally.
   */
  
  GDK_SUPER_MASK    = 1 << 26,
  GDK_HYPER_MASK    = 1 << 27,
  GDK_META_MASK     = 1 << 28,
  
  GDK_RELEASE_MASK  = 1 << 30,

  GDK_MODIFIER_MASK = 0x5c001fff
} GdkModifierType;

typedef enum
{
  GDK_INPUT_READ       = 1 << 0,
  GDK_INPUT_WRITE      = 1 << 1,
  GDK_INPUT_EXCEPTION  = 1 << 2
} GdkInputCondition;

typedef enum
{
  GDK_OK	  = 0,
  GDK_ERROR	  = -1,
  GDK_ERROR_PARAM = -2,
  GDK_ERROR_FILE  = -3,
  GDK_ERROR_MEM	  = -4
} GdkStatus;

/* We define specific numeric values for these constants,
 * since old application code may depend on them matching the X values
 * We don't actually depend on the matchup ourselves.
 */
typedef enum
{
  GDK_GRAB_SUCCESS         = 0,
  GDK_GRAB_ALREADY_GRABBED = 1,
  GDK_GRAB_INVALID_TIME    = 2,
  GDK_GRAB_NOT_VIEWABLE    = 3,
  GDK_GRAB_FROZEN          = 4
} GdkGrabStatus;

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

struct _GdkRectangle
{
  gint x;
  gint y;
  gint width;
  gint height;
};

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

STLWRT_DECLARE_GET_FTYPE_FUNCTIONS (gdk_rectangle_get_type)

gboolean SF(gdk_rectangle_intersect) (const GdkRectangle *src1,
				  const GdkRectangle *src2,
				  GdkRectangle       *dest);
void     SF(gdk_rectangle_union)     (const GdkRectangle *src1,
				  const GdkRectangle *src2,
				  GdkRectangle       *dest);

G_END_DECLS


#endif /* __GDK_TYPES_H__ */
