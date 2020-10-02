/*
 * gdkscreen.h
 * 
 * Copyright 2001 Sun Microsystems Inc. 
 *
 * Erwann Chenede <erwann.chenede@sun.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */


#include <cairo.h>

#include <gdkgc.h>

#include "gdktypes.h"

#include "gdkdisplay.h"

G_BEGIN_DECLS


typedef struct _GdkScreenClass GdkScreenClass;

#define GDK_TYPE_SCREEN            (gdk_screen_get_type ())
#define GDK_SCREEN(object)         (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_SCREEN, GdkScreen))
#define GDK_SCREEN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_SCREEN, GdkScreenClass))
#define GDK_IS_SCREEN(object)      (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_SCREEN))
#define GDK_IS_SCREEN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_SCREEN))
#define GDK_SCREEN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_SCREEN, GdkScreenClass))

STLWRT_DECLARE_VTYPE_FPARENT(GdkScreen, gdk_screen, GObject,
  guint  (closed) : 1;

  GdkGC * (normal_gcs[32]);
  GdkGC * (exposure_gcs[32]);
  GdkGC * (subwindow_gcs[32]);

  cairo_font_options_t * (font_options);
  double  (resolution);	/* pixels/points scale factor for fonts */
)

struct _GdkScreenClass
{
  GObjectClass parent_class;

  void (*size_changed) (GdkScreen *screen);
  void (*composited_changed) (GdkScreen *screen);
  void (*monitors_changed) (GdkScreen *screen);
};


#include <gdkvisual.h>

GdkColormap *SF(gdk_screen_get_default_colormap)  (GdkScreen   *screen);
void         SF(gdk_screen_set_default_colormap)  (GdkScreen   *screen,
					       GdkColormap *colormap);
GdkColormap* SF(gdk_screen_get_system_colormap)   (GdkScreen   *screen);
GdkVisual*   SF(gdk_screen_get_system_visual)     (GdkScreen   *screen);
GdkColormap *SF(gdk_screen_get_rgb_colormap)      (GdkScreen   *screen);
GdkVisual *  SF(gdk_screen_get_rgb_visual)        (GdkScreen   *screen);
GdkColormap *SF(gdk_screen_get_rgba_colormap)     (GdkScreen   *screen);
GdkVisual *  SF(gdk_screen_get_rgba_visual)       (GdkScreen   *screen);
gboolean     SF(gdk_screen_is_composited)	      (GdkScreen   *screen);

GdkWindow *  SF(gdk_screen_get_root_window)       (GdkScreen   *screen);
GdkDisplay * SF(gdk_screen_get_display)           (GdkScreen   *screen);
gint         SF(gdk_screen_get_number)            (GdkScreen   *screen);
gint         SF(gdk_screen_get_width)             (GdkScreen   *screen);
gint         SF(gdk_screen_get_height)            (GdkScreen   *screen);
gint         SF(gdk_screen_get_width_mm)          (GdkScreen   *screen);
gint         SF(gdk_screen_get_height_mm)         (GdkScreen   *screen);

GList *      SF(gdk_screen_list_visuals)          (GdkScreen   *screen);
GList *      SF(gdk_screen_get_toplevel_windows)  (GdkScreen   *screen);
gchar *      SF(gdk_screen_make_display_name)     (GdkScreen   *screen);

gint          SF(gdk_screen_get_n_monitors)        (GdkScreen *screen);
gint          SF(gdk_screen_get_primary_monitor)   (GdkScreen *screen);
void          SF(gdk_screen_get_monitor_geometry)  (GdkScreen *screen,
						gint       monitor_num,
						GdkRectangle *dest);
gint          SF(gdk_screen_get_monitor_at_point)  (GdkScreen *screen,
						gint       x,
						gint       y);
gint          SF(gdk_screen_get_monitor_at_window) (GdkScreen *screen,
						GdkWindow *window);
gint          SF(gdk_screen_get_monitor_width_mm)  (GdkScreen *screen,
                                                gint       monitor_num);
gint          SF(gdk_screen_get_monitor_height_mm) (GdkScreen *screen,
                                                gint       monitor_num);
gchar *       SF(gdk_screen_get_monitor_plug_name) (GdkScreen *screen,
                                                gint       monitor_num);

void          SF(gdk_screen_broadcast_client_message)  (GdkScreen       *screen,
						    GdkEvent        *event);

GdkScreen *SF(gdk_screen_get_default) (void);

gboolean   SF(gdk_screen_get_setting) (GdkScreen   *screen,
				   const gchar *name,
				   GValue      *value);

void                        SF(gdk_screen_set_font_options) (GdkScreen                  *screen,
							 const cairo_font_options_t *options);
const cairo_font_options_t *SF(gdk_screen_get_font_options) (GdkScreen                  *screen);

void    SF(gdk_screen_set_resolution) (GdkScreen *screen,
				   gdouble    dpi);
gdouble SF(gdk_screen_get_resolution) (GdkScreen *screen);

GdkWindow *SF(gdk_screen_get_active_window) (GdkScreen *screen);
GList     *SF(gdk_screen_get_window_stack)  (GdkScreen *screen);

G_END_DECLS
