
/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2000 Red Hat, Inc.
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

#ifndef __GDK_DISPLAY_MANAGER_H__
#define __GDK_DISPLAY_MANAGER_H__

#include <gdktypes.h>
#include <gdkdisplay.h>

G_BEGIN_DECLS

typedef struct _GdkDisplayManager      GdkDisplayManagerFat;
typedef struct _GdkDisplayManager      GdkDisplayManagerThin;

typedef struct _GdkDisplayManagerClass GdkDisplayManagerClass;

#define GDK_TYPE_DISPLAY_MANAGER              (gdk_display_manager_get_type ())
#define GDK_DISPLAY_MANAGER(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_DISPLAY_MANAGER, GdkDisplayManager))
#define GDK_DISPLAY_MANAGER_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_DISPLAY_MANAGER, GdkDisplayManagerClass))
#define GDK_IS_DISPLAY_MANAGER(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_DISPLAY_MANAGER))
#define GDK_IS_DISPLAY_MANAGER_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_DISPLAY_MANAGER))
#define GDK_DISPLAY_MANAGER_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_DISPLAY_MANAGER, GdkDisplayManagerClass))

struct _GdkDisplayManagerClass
{
  GObjectClass parent_class;

  void (*display_opened) (GdkDisplayManager *display_manager,
			  GdkDisplay *display);
};

GType _T2_gdk_display_manager_get_type (void) G_GNUC_CONST;
GType _3T_gdk_display_manager_get_type (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType gdk_display_manager_get_type (void) G_GNUC_CONST;

GdkDisplayManager *SF(gdk_display_manager_get)                 (void);
GdkDisplay *       SF(gdk_display_manager_get_default_display) (GdkDisplayManager *display_manager);
void               SF(gdk_display_manager_set_default_display) (GdkDisplayManager *display_manager,
							    GdkDisplay        *display);
GSList     *       SF(gdk_display_manager_list_displays)       (GdkDisplayManager *display_manager);


void SF(gdk_set_allowed_backends) (const gchar *backends);

G_END_DECLS

#endif /* __GDK_DISPLAY_MANAGER_H__ */
