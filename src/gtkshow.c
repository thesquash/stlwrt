/*
 * STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2008  Jaap Haitsma <jaap@haitsma.org>
 *
 * All rights reserved.
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

#include <stlwrt.h>

#include <gdk.h>

#include <gtkshow.h>




/**
 * __gtk_show_uri:
 * @screen: (allow-none): screen to show the uri on or %NULL for the default screen
 * @uri: the uri to show
 * @timestamp: a timestamp to prevent focus stealing.
 * @error: a #GError that is returned in case of errors
 *
 * This is a convenience function for launching the default application
 * to show the uri. The uri must be of a form understood by GIO (i.e. you
 * need to install gvfs to get support for uri schemes such as http://
 * or ftp://, as only local files are handled by GIO itself).
 * Typical examples are
 * <simplelist>
 *   <member><filename>file:///home/gnome/pict.jpg</filename></member>
 *   <member><filename>http://www.gnome.org</filename></member>
 *   <member><filename>mailto:me&commat;gnome.org</filename></member>
 * </simplelist>
 * Ideally the timestamp is taken from the event triggering
 * the __gtk_show_uri() call. If timestamp is not known you can take
 * %GDK_CURRENT_TIME.
 *
 * This function can be used as a replacement for gnome_vfs_url_show()
 * and gnome_url_show().
 *
 * Returns: %TRUE on success, %FALSE on error.
 *
 * Since: 2.14
 */
gboolean
__gtk_show_uri (GdkScreen    *screen,
              const gchar  *uri,
              guint32       timestamp,
              GError      **error)
{
  GdkAppLaunchContext *context;
  gboolean ret;

  g_return_val_if_fail (uri != NULL, FALSE);

  context = __gdk_app_launch_context_new ();
  __gdk_app_launch_context_set_screen (context, screen);
  __gdk_app_launch_context_set_timestamp (context, timestamp);

  ret = g_app_info_launch_default_for_uri (uri, (GAppLaunchContext*)context, error);
  g_object_unref (context);

  return ret;
}
