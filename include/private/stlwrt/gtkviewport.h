
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

#include <gtkadjustment.h>
#include <gtkbin.h>


G_BEGIN_DECLS


#define GTK_TYPE_VIEWPORT            (gtk_viewport_get_type ())
#define GTK_VIEWPORT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_VIEWPORT, GtkViewport))
#define GTK_VIEWPORT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_VIEWPORT, GtkViewportClass))
#define GTK_IS_VIEWPORT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_VIEWPORT))
#define GTK_IS_VIEWPORT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_VIEWPORT))
#define GTK_VIEWPORT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_VIEWPORT, GtkViewportClass))


typedef struct _GtkViewport       GtkViewportFat;
typedef struct _GtkViewport       GtkViewportThin;

typedef struct _GtkViewportClass  GtkViewportClass;

/********************************************************************/
struct _GtkViewportInstanceProps
{


  GtkShadowType  (shadow_type);
  GdkWindow * (view_window);
  GdkWindow * (bin_window);
  GtkAdjustment * (hadjustment);
  GtkAdjustment * (vadjustment);
};

struct _GtkViewportFat
{
  GtkBinFat   bin;

  struct _GtkViewportInstanceProps instance_properties;
};

struct _GtkViewportThin
{
  GtkBinThin  bin;

  gpointer reserved;
};


typedef union
{
  struct _GtkViewportFat   fat_instance;
  struct _GtkViewportThin  thin_instance;
}   GtkViewport;
/********************************************************************/



struct _GtkViewportClass
{
  GtkBinClass parent_class;

  void	(*set_scroll_adjustments)	(GtkViewport	*viewport,
					 GtkAdjustment	*hadjustment,
					 GtkAdjustment	*vadjustment);
};


GType          _T2_gtk_viewport_get_type        (void) G_GNUC_CONST;
GType          _3T_gtk_viewport_get_type        (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType          gtk_viewport_get_type        (void) G_GNUC_CONST;
GtkWidget*     __gtk_viewport_new             (GtkAdjustment *hadjustment,
					     GtkAdjustment *vadjustment);
GtkAdjustment* __gtk_viewport_get_hadjustment (GtkViewport   *viewport);
GtkAdjustment* __gtk_viewport_get_vadjustment (GtkViewport   *viewport);
void           __gtk_viewport_set_hadjustment (GtkViewport   *viewport,
					     GtkAdjustment *adjustment);
void           __gtk_viewport_set_vadjustment (GtkViewport   *viewport,
					     GtkAdjustment *adjustment);
void           __gtk_viewport_set_shadow_type (GtkViewport   *viewport,
					     GtkShadowType  type);
GtkShadowType  __gtk_viewport_get_shadow_type (GtkViewport   *viewport);
GdkWindow*     __gtk_viewport_get_bin_window  (GtkViewport   *viewport);
GdkWindow*     __gtk_viewport_get_view_window (GtkViewport   *viewport);


G_END_DECLS


#endif /* __GTK_VIEWPORT_H__ */
