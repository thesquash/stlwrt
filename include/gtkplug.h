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
 * License along with this library; if not, write to the Free
 * Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __GTK_PLUG_H__
#define __GTK_PLUG_H__

#include <stlwrt.h>


#include <gtksocket.h>

#include <gtkwindow.h>


G_BEGIN_DECLS

#define GTK_TYPE_PLUG            (gtk_plug_get_type ())
#define GTK_PLUG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PLUG, GtkPlug))
#define GTK_PLUG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_PLUG, GtkPlugClass))
#define GTK_IS_PLUG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PLUG))
#define GTK_IS_PLUG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_PLUG))
#define GTK_PLUG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_PLUG, GtkPlugClass))

typedef struct _GtkPlugClass   GtkPlugClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkPlug, gtk_plug, GtkWindow,
  GdkWindow * (socket_window);
  GtkWidget * (modality_window);
  GtkWindowGroup * (modality_group);
  GHashTable * (grabbed_keys);

  guint  (same_app) : 1;
)

struct _GtkPlugClass
{
  GtkWindowClass parent_class;

  void (*embedded) (GtkPlug *plug);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


#ifndef GDK_MULTIHEAD_SAFE
void       SF(gtk_plug_construct) (GtkPlug         *plug,
			       GdkNativeWindow  socket_id);
GtkWidget* SF(gtk_plug_new)       (GdkNativeWindow  socket_id);
#endif

void       SF(gtk_plug_construct_for_display) (GtkPlug         *plug,
					   GdkDisplay      *display,
					   GdkNativeWindow  socket_id);
GtkWidget* SF(gtk_plug_new_for_display)       (GdkDisplay      *display,
					   GdkNativeWindow  socket_id);

GdkNativeWindow SF(gtk_plug_get_id) (GtkPlug         *plug);

gboolean  SF(gtk_plug_get_embedded) (GtkPlug         *plug);

GdkWindow *SF(gtk_plug_get_socket_window) (GtkPlug   *plug);

void SF(_gtk_plug_add_to_socket)      (GtkPlug   *plug,
				   GtkSocket *socket_);
void SF(_gtk_plug_remove_from_socket) (GtkPlug   *plug,
				   GtkSocket *socket_);

G_END_DECLS

#endif /* __GTK_PLUG_H__ */
