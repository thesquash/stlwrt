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

#include <config.h>

#include <stlwrtobject.h>


#define STLWRT_OBJECT_GET_PRIVATE(obj) (G_TYPE_INSTANCE_GET_PRIVATE ((obj), STLWRT_TYPE_OBJECT, StlwrtObjectPrivate))

struct _StlwrtObjectPrivate
{
  StlwrtObject*	fat_instance;
  
  StlwrtObject*	skinny_instance;
};

G_DEFINE_ABSTRACT_TYPE_WITH_CODE (StlwrtObject, stlwrt_object,
				  G_TYPE_INITIALLY_UNOWNED,
				  G_ADD_PRIVATE (StlwrtObject))

static void stlwrt_object_finalize     (GObject *object);


static void
stlwrt_object_class_init (StlwrtObjectClass *class)
{
  GObjectClass *gobject_class;

  gobject_class = G_OBJECT_CLASS (class);

  gobject_class->finalize     = stlwrt_object_finalize;

  stlwrt_object_class_register_type (class, STLWRT_TYPE_OBJECT);
}



StlwrtObject * _stlwrt_object_get_fat_instance (StlwrtObject *object)
{
  StlwrtObjectPrivate *private;
  g_return_val_if_fail (STLWRT_IS_OBJECT(object), NULL);
  
  private = G_TYPE_INSTANCE_GET_PRIVATE (object, STLWRT_TYPE_OBJECT, StlwrtObject);
  
  g_return_val_if_fail (private != NULL, NULL);
  
  return private->fat_instance;
}


StlwrtObject * _stlwrt_object_get_skinny_instance (StlwrtObject *object)
{
  StlwrtObjectPrivate *private;
  g_return_val_if_fail (STLWRT_IS_OBJECT(object), NULL);
  
  private = G_TYPE_INSTANCE_GET_PRIVATE (object, STLWRT_TYPE_OBJECT, StlwrtObject);
  
  g_return_val_if_fail (private != NULL, NULL);
  
  return private->skinny_instance;
}

#define __GTK_ACTION_C__

