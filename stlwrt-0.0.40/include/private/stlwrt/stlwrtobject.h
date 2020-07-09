/*
 * STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2020 Gordon N. Squash.
 *
 * This Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the Gnome Library; see the file COPYING.LIB.  If not,
 * write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __STLWRT_OBJECT_H__
#define __STLWRT_OBJECT_H__

#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk/gtk.h> can be included directly."
#endif

G_BEGIN_DECLS

#define STLWRT_TYPE_OBJECT            (stlwrt_object_get_type ())
#define STLWRT_OBJECT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), STLWRT_TYPE_OBJECT, StlwrtObject))
#define STLWRT_OBJECT_CLASS(class)    (G_TYPE_CHECK_CLASS_CAST ((class), STLWRT_TYPE_OBJECT, StlwrtObjectClass))
#define STLWRT_IS_OBJECT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), STLWRT_TYPE_OBJECT))
#define STLWRT_IS_OBJECT_CLASS(class) (G_TYPE_CHECK_CLASS_TYPE ((class), STLWRT_TYPE_OBJECT))
#define STLWRT_OBJECT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), STLWRT_TYPE_OBJECT, StlwrtObjectClass))

typedef struct _StlwrtObject        StlwrtObject;
typedef struct _StlwrtObjectClass   StlwrtObjectClass;
typedef struct _StlwrtObjectPrivate StlwrtObjectPrivate;

struct _StlwrtObject
{
  GInitiallyUnowned parent;
};

struct _StlwrtObjectClass
{
  GInitiallyUnownedClass parent_class;

  /*
   * The top-level type, directly or indirectly derived from StlwrtObject,
   * which is also an object that is part of the STLWRT library.  See the
   * documentation about StlwrtObject itself.
   */
  GType	top_level_stlwrt_type;
  

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

GType         stlwrt_object_get_type              (void) G_GNUC_CONST;

void          __stlwrt_object_class_register_type   (StlwrtObjectClass  *object,
						    GType     high_level_type);

StlwrtObject* __stlwrt_object_get_fat_instance      (StlwrtObjectClass  *object);

StlwrtObject* __stlwrt_object_get_skinny_instance   (StlwrtObjectClass  *object);


G_END_DECLS

#endif  /* __STLWRT_OBJECT_H__ */
