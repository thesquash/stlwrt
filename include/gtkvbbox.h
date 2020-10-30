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

#ifndef __GTK_VBBOX_H__
#define __GTK_VBBOX_H__

#include <stlwrt.h>
#include <stlwrt-typedefs.h>


#include <gtkbbox.h>

G_BEGIN_DECLS

#define GTK_TYPE_VBUTTON_BOX            (gtk_vbutton_box_get_type ())
#define GTK_VBUTTON_BOX(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_VBUTTON_BOX, GtkVButtonBox))
#define GTK_VBUTTON_BOX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_VBUTTON_BOX, GtkVButtonBoxClass))
#define GTK_IS_VBUTTON_BOX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_VBUTTON_BOX))
#define GTK_IS_VBUTTON_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_VBUTTON_BOX))
#define GTK_VBUTTON_BOX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_VBUTTON_BOX, GtkVButtonBoxClass))

typedef struct _GtkVButtonBoxClass  GtkVButtonBoxClass;

STLWRT_DECLARE_FTYPE_VPARENT(GtkVButtonBox, gtk_vbutton_box, GtkButtonBox)

struct _GtkVButtonBoxClass
{
  GtkButtonBoxClass parent_class;
};


GtkWidget *SF(gtk_vbutton_box_new)      (void);

/* buttons can be added by SF(gtk_container_add)() */

#ifndef GTK_DISABLE_DEPRECATED
gint SF(gtk_vbutton_box_get_spacing_default) (void);
void SF(gtk_vbutton_box_set_spacing_default) (gint spacing);

GtkButtonBoxStyle SF(gtk_vbutton_box_get_layout_default) (void);
void SF(gtk_vbutton_box_set_layout_default) (GtkButtonBoxStyle layout);
#endif

/* private API */
GtkButtonBoxStyle SF(_gtk_vbutton_box_get_layout_default) (void);

G_END_DECLS


#endif /* __GTK_VBBOX_H__ */
