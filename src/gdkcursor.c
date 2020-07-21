
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



#include "config.h"

#include <stlwrtobject.h>
#include <gdkcursor.h>
#include <gdkdisplay.h>
#include <gdkinternals.h>


GType
gdk_cursor_get_type (void)
{
  static GType our_type = 0;
  
  if (our_type == 0)
    our_type = g_boxed_type_register_static (g_intern_static_string ("GdkCursor"),
					     (GBoxedCopyFunc)__gdk_cursor_ref,
					     (GBoxedFreeFunc)__gdk_cursor_unref);
  return our_type;
}

/**
 * __gdk_cursor_ref:
 * @cursor: a #GdkCursor
 * 
 * Adds a reference to @cursor.
 * 
 * Return value: (transfer full): Same @cursor that was passed in
 **/
GdkCursor*
__gdk_cursor_ref (GdkCursor *cursor)
{
  g_return_val_if_fail (cursor != NULL, NULL);
  g_return_val_if_fail (cursor->ref_count > 0, NULL);

  cursor->ref_count += 1;

  return cursor;
}

/**
 * __gdk_cursor_unref:
 * @cursor: a #GdkCursor
 *
 * Removes a reference from @cursor, deallocating the cursor
 * if no references remain.
 * 
 **/
void
__gdk_cursor_unref (GdkCursor *cursor)
{
  g_return_if_fail (cursor != NULL);
  g_return_if_fail (cursor->ref_count > 0);

  cursor->ref_count -= 1;

  if (cursor->ref_count == 0)
    _gdk_cursor_destroy (cursor);
}

/**
 * __gdk_cursor_new:
 * @cursor_type: cursor to create
 * 
 * Creates a new cursor from the set of builtin cursors for the default display.
 * See __gdk_cursor_new_for_display().
 *
 * To make the cursor invisible, use %GDK_BLANK_CURSOR.
 * 
 * Return value: a new #GdkCursor
 **/
GdkCursor*
__gdk_cursor_new (GdkCursorType cursor_type)
{
  return __gdk_cursor_new_for_display (__gdk_display_get_default(), cursor_type);
}

/**
 * __gdk_cursor_get_cursor_type:
 * @cursor:  a #GdkCursor
 *
 * Returns the cursor type for this cursor.
 *
 * Return value: a #GdkCursorType
 *
 * Since: 2.22
 **/
GdkCursorType
__gdk_cursor_get_cursor_type (GdkCursor *cursor)
{
  g_return_val_if_fail (cursor != NULL, GDK_BLANK_CURSOR);
  return cursor->type;
}

#define __GDK_CURSOR_C__
