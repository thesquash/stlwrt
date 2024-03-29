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

#ifndef __GTK_MISC_H__
#define __GTK_MISC_H__

#include <stlwrt.h>


#include <gtkwidget.h>


G_BEGIN_DECLS

#define GTK_TYPE_MISC		       (gtk_misc_get_type ())
#define GTK_MISC(obj)		       (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_MISC, GtkMisc))
#define GTK_MISC_CLASS(klass)	       (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_MISC, GtkMiscClass))
#define GTK_IS_MISC(obj)	       (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_MISC))
#define GTK_IS_MISC_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_MISC))
#define GTK_MISC_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_MISC, GtkMiscClass))

typedef struct _GtkMiscClass  GtkMiscClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkMisc, gtk_misc, GtkWidget,
  gfloat  (xalign);
  gfloat  (yalign);

  guint16  (xpad);
  guint16  (ypad);
)

struct _GtkMiscClass
{
  GtkWidgetClass parent_class;
};


void	SF(gtk_misc_set_alignment) (GtkMisc *misc,
				gfloat	 xalign,
				gfloat	 yalign);
void    SF(gtk_misc_get_alignment) (GtkMisc *misc,
				gfloat  *xalign,
				gfloat  *yalign);
void	SF(gtk_misc_set_padding)   (GtkMisc *misc,
				gint	 xpad,
				gint	 ypad);
void    SF(gtk_misc_get_padding)   (GtkMisc *misc,
				gint    *xpad,
				gint    *ypad);


G_END_DECLS

#endif /* __GTK_MISC_H__ */
