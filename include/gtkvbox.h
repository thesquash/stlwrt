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

#ifndef __GTK_VBOX_H__
#define __GTK_VBOX_H__

#include <stlwrt.h>


#include <gtkbox.h>

G_BEGIN_DECLS

#define GTK_TYPE_VBOX		 (gtk_vbox_get_type ())
#define GTK_VBOX(obj)		 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_VBOX, GtkVBox))
#define GTK_VBOX_CLASS(klass)	 (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_VBOX, GtkVBoxClass))
#define GTK_IS_VBOX(obj)	 (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_VBOX))
#define GTK_IS_VBOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_VBOX))
#define GTK_VBOX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_VBOX, GtkVBoxClass))

typedef struct _GtkVBoxClass  GtkVBoxClass;

STLWRT_DECLARE_FTYPE_VPARENT(GtkVBox, gtk_vbox, GtkBox)

struct _GtkVBoxClass
{
  GtkBoxClass parent_class;
};


GtkWidget * SF(gtk_vbox_new)      (gboolean homogeneous,
                               gint     spacing);

G_END_DECLS

#endif /* __GTK_VBOX_H__ */
