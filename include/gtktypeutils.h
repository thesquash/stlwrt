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

/*
 * TODO:
 *
 * Look into whether this header file is even necessary anymore!
 *
 */

#ifndef __GTK_TYPE_UTILS_H__
#define __GTK_TYPE_UTILS_H__

#include <stlwrt.h>

G_BEGIN_DECLS

/* urg */
#define GTK_TYPE_IDENTIFIER (gtk_identifier_get_type ())
STLWRT_DECLARE_OPAQUE_TYPE(GtkIdentifier, gtk_identifier)

/* --- typedefs --- */
/* here we come with some necessary forward declarations for structures and
 * provide some fundamental function signatures
 */
typedef struct _GtkArg	       	     GtkArg;
#if !defined (GTK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
typedef gboolean (*GtkFunction)	    (gpointer      data);
typedef void (*GtkCallbackMarshal)  (GObject      *object,
				     gpointer      data,
				     guint         n_args,
				     GtkArg       *args);
#endif

/* This used to be defined in gtkitemfactory.h, but moved over here after
 * the complete deprecation of that header
 */
typedef gchar * (*GtkTranslateFunc) (const gchar  *path,
				     gpointer      func_data);

/* Phooey.  I thought we could get rid of these darned marshaller types. */
#if !defined (GTK_DISABLE_DEPRECATED) || defined (GTK_COMPILATION)

/* Used by gtk_container_foreach_unmarshal () */

/* GtkArg, used to hold differently typed values */
struct _GtkArg
{
  GType type;
  gchar *name;

  /* this union only defines the required storage types for
   * the possibile values, thus there is no gint enum_data field,
   * because that would just be a mere alias for gint int_data.
   * use the GTK_VALUE_*() and GTK_RETLOC_*() macros to access
   * the discrete memebers.
   */
  union {
    /* flat values */
    gchar char_data;
    guchar uchar_data;
    gboolean bool_data;
    gint int_data;
    guint uint_data;
    glong long_data;
    gulong ulong_data;
    gfloat float_data;
    gdouble double_data;
    gchar *string_data;
    GObject *object_data;
    gpointer pointer_data;

    /* structured values */
    struct {
      GCallback f;
      gpointer d;
    } signal_data;
  } d;
};

/* argument value access macros, these must not contain casts,
 * to allow the usage of these macros in combination with the
 * address operator, e.g. &GTK_VALUE_CHAR (*arg)
 */
#define GTK_VALUE_CHAR(a)	((a).d.char_data)
#define GTK_VALUE_UCHAR(a)	((a).d.uchar_data)
#define GTK_VALUE_BOOL(a)	((a).d.bool_data)
#define GTK_VALUE_INT(a)	((a).d.int_data)
#define GTK_VALUE_UINT(a)	((a).d.uint_data)
#define GTK_VALUE_LONG(a)	((a).d.long_data)
#define GTK_VALUE_ULONG(a)	((a).d.ulong_data)
#define GTK_VALUE_FLOAT(a)	((a).d.float_data)
#define GTK_VALUE_DOUBLE(a)	((a).d.double_data)
#define GTK_VALUE_STRING(a)	((a).d.string_data)
#define GTK_VALUE_ENUM(a)	((a).d.int_data)
#define GTK_VALUE_FLAGS(a)	((a).d.uint_data)
#define GTK_VALUE_BOXED(a)	((a).d.pointer_data)
#define GTK_VALUE_OBJECT(a)	((a).d.object_data)
#define GTK_VALUE_POINTER(a)	((a).d.pointer_data)
#define GTK_VALUE_SIGNAL(a)	((a).d.signal_data)

#endif /* !GTK_DISABLE_DEPRECATED || GTK_COMPILATION */

G_END_DECLS

#endif /* __GTK_TYPE_UTILS_H__ */
