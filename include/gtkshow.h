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

#ifndef __GTK_SHOW_H__
#define __GTK_SHOW_H__

#include <stlwrt.h>

G_BEGIN_DECLS

gboolean SF(gtk_show_uri)  (GdkScreen   *screen,
                        const gchar *uri,
                        guint32      timestamp,
                        GError     **error);

G_END_DECLS

#endif /* __GTK_SHOW_H__ */
