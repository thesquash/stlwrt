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

#ifndef __GTK_VIEWPORT_H__
#define __GTK_VIEWPORT_H__

#include <stlwrt.h>


#include <gtkadjustment.h>

#include <gtkbin.h>

G_BEGIN_DECLS

#define GTK_TYPE_VIEWPORT            (gtk_viewport_get_type ())
#define GTK_VIEWPORT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_VIEWPORT, GtkViewport))
#define GTK_VIEWPORT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_VIEWPORT, GtkViewportClass))
#define GTK_IS_VIEWPORT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_VIEWPORT))
#define GTK_IS_VIEWPORT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_VIEWPORT))
#define GTK_VIEWPORT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_VIEWPORT, GtkViewportClass))

typedef struct _GtkViewportClass  GtkViewportClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkViewport, gtk_viewport, GtkBin,
  GtkShadowType  (shadow_type);
  GdkWindow * (view_window);
  GdkWindow * (bin_window);
  GtkAdjustment * (hadjustment);
  GtkAdjustment * (vadjustment);
)

struct _GtkViewportClass
{
  GtkBinClass parent_class;

  void	(*set_scroll_adjustments)	(GtkViewport	*viewport,
					 GtkAdjustment	*hadjustment,
					 GtkAdjustment	*vadjustment);
};


GtkWidget*     SF(gtk_viewport_new)             (GtkAdjustment *hadjustment,
					     GtkAdjustment *vadjustment);
GtkAdjustment* SF(gtk_viewport_get_hadjustment) (GtkViewport   *viewport);
GtkAdjustment* SF(gtk_viewport_get_vadjustment) (GtkViewport   *viewport);
void           SF(gtk_viewport_set_hadjustment) (GtkViewport   *viewport,
					     GtkAdjustment *adjustment);
void           SF(gtk_viewport_set_vadjustment) (GtkViewport   *viewport,
					     GtkAdjustment *adjustment);
void           SF(gtk_viewport_set_shadow_type) (GtkViewport   *viewport,
					     GtkShadowType  type);
GtkShadowType  SF(gtk_viewport_get_shadow_type) (GtkViewport   *viewport);
GdkWindow*     SF(gtk_viewport_get_bin_window)  (GtkViewport   *viewport);
GdkWindow*     SF(gtk_viewport_get_view_window) (GtkViewport   *viewport);

G_END_DECLS

#endif /* __GTK_VIEWPORT_H__ */
