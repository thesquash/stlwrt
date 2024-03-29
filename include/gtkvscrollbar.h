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

#ifndef __GTK_VSCROLLBAR_H__
#define __GTK_VSCROLLBAR_H__

#include <stlwrt.h>


#include <gtkscrollbar.h>

G_BEGIN_DECLS

#define GTK_TYPE_VSCROLLBAR            (gtk_vscrollbar_get_type ())
#define GTK_VSCROLLBAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_VSCROLLBAR, GtkVScrollbar))
#define GTK_VSCROLLBAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_VSCROLLBAR, GtkVScrollbarClass))
#define GTK_IS_VSCROLLBAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_VSCROLLBAR))
#define GTK_IS_VSCROLLBAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_VSCROLLBAR))
#define GTK_VSCROLLBAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_VSCROLLBAR, GtkVScrollbarClass))

typedef struct _GtkVScrollbarClass  GtkVScrollbarClass;

/**
 * GtkVScrollbar:
 *
 * The #GtkVScrollbar struct contains private data and should be accessed
 * using the functions below.
 */
STLWRT_DECLARE_FTYPE_VPARENT(GtkVScrollbar, gtk_vscrollbar, GtkScrollbar)

struct _GtkVScrollbarClass
{
  GtkScrollbarClass parent_class;
};


GtkWidget* SF(gtk_vscrollbar_new)      (GtkAdjustment *adjustment);

G_END_DECLS

#endif /* __GTK_VSCROLLBAR_H__ */
