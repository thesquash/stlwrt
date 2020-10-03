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

#ifndef __GTK_INVISIBLE_H__
#define __GTK_INVISIBLE_H__

#include <stlwrt.h>
#include <stlwrt-typedefs.h>


#include <gtkwidget.h>

G_BEGIN_DECLS

#define GTK_TYPE_INVISIBLE		(gtk_invisible_get_type ())
#define GTK_INVISIBLE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_INVISIBLE, GtkInvisible))
#define GTK_INVISIBLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_INVISIBLE, GtkInvisibleClass))
#define GTK_IS_INVISIBLE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_INVISIBLE))
#define GTK_IS_INVISIBLE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_INVISIBLE))
#define GTK_INVISIBLE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_INVISIBLE, GtkInvisibleClass))

typedef struct _GtkInvisibleClass  GtkInvisibleClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkInvisible, gtk_invisible, GtkWidget,
  gboolean    (has_user_ref_count);
  GdkScreen * (screen);
)

struct _GtkInvisibleClass
{
  GtkWidgetClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GtkWidget* SF(gtk_invisible_new)            (void);
GtkWidget* SF(gtk_invisible_new_for_screen) (GdkScreen    *screen);
void	   SF(gtk_invisible_set_screen)	(GtkInvisible *invisible,
					 GdkScreen    *screen);
GdkScreen* SF(gtk_invisible_get_screen)	(GtkInvisible *invisible);

G_END_DECLS

#endif /* __GTK_INVISIBLE_H__ */
