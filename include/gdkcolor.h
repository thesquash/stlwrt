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

#ifndef __GDK_COLOR_H__
#define __GDK_COLOR_H__

#include <stlwrt.h>


#include <cairo.h>

#include <gdkvisual.h>

#include <gdktypes.h>

G_BEGIN_DECLS

/* The color type.
 *   A color consists of red, green and blue values in the
 *    range 0-65535 and a pixel value. The pixel value is highly
 *    dependent on the depth and colormap which this color will
 *    be used to draw into. Therefore, sharing colors between
 *    colormaps is a bad idea.
 */
STLWRT_DECLARE_BOXED_TYPE(GdkColor, gdk_color,
  guint32 pixel;
  guint16 red;
  guint16 green;
  guint16 blue;
)

/* The colormap type.
 */


typedef struct _GdkColormapClass GdkColormapClass;

#define GDK_TYPE_COLORMAP              (gdk_colormap_get_type ())
#define GDK_COLORMAP(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_COLORMAP, GdkColormap))
#define GDK_COLORMAP_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_COLORMAP, GdkColormapClass))
#define GDK_IS_COLORMAP(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_COLORMAP))
#define GDK_IS_COLORMAP_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_COLORMAP))
#define GDK_COLORMAP_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_COLORMAP, GdkColormapClass))

#define GDK_TYPE_COLOR                 (gdk_color_get_type ())

STLWRT_DECLARE_VTYPE_FPARENT (GdkColormap, gdk_colormap, GObject,

  /*< public >*/
  gint       (size);
  GdkColor * (colors);

  /*< private >*/
  GdkVisual * (visual);

  gpointer  (windowing_data);

)

struct _GdkColormapClass
{
  GObjectClass parent_class;

};


GdkColormap* SF(gdk_colormap_new)	  (GdkVisual   *visual,
				   gboolean	allocate);

#ifndef GDK_DISABLE_DEPRECATED
GdkColormap* SF(gdk_colormap_ref)	  (GdkColormap *cmap);
void	     SF(gdk_colormap_unref)	  (GdkColormap *cmap);
#endif 

#ifndef GDK_MULTIHEAD_SAFE
GdkColormap* SF(gdk_colormap_get_system)	        (void);
#endif

GdkScreen *SF(gdk_colormap_get_screen) (GdkColormap *cmap);

#ifndef GDK_DISABLE_DEPRECATED
gint SF(gdk_colormap_get_system_size)  (void);
#endif

#if !defined (GDK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
/* Used by SF(gdk_colors_store) () */
void SF(gdk_colormap_change) (GdkColormap	*colormap,
			  gint		 ncolors);
#endif 

gint  SF(gdk_colormap_alloc_colors)   (GdkColormap    *colormap,
				   GdkColor       *colors,
				   gint            n_colors,
				   gboolean        writeable,
				   gboolean        best_match,
				   gboolean       *success);
gboolean SF(gdk_colormap_alloc_color) (GdkColormap    *colormap,
				   GdkColor       *color,
				   gboolean        writeable,
				   gboolean        best_match);
void     SF(gdk_colormap_free_colors) (GdkColormap    *colormap,
				   const GdkColor *colors,
				   gint            n_colors);
void     SF(gdk_colormap_query_color) (GdkColormap    *colormap,
				   gulong          pixel,
				   GdkColor       *result);

GdkVisual *SF(gdk_colormap_get_visual) (GdkColormap *colormap);

GdkColor *SF(gdk_color_copy)      (const GdkColor *color);
void      SF(gdk_color_free)      (GdkColor       *color);
gboolean  SF(gdk_color_parse)     (const gchar    *spec,
			       GdkColor       *color);
guint     SF(gdk_color_hash)      (const GdkColor *colora);
gboolean  SF(gdk_color_equal)     (const GdkColor *colora,
			       const GdkColor *colorb);
gchar *   SF(gdk_color_to_string) (const GdkColor *color);

/* The following functions are deprecated */
#ifndef GDK_DISABLE_DEPRECATED
void SF(gdk_colors_store)	 (GdkColormap	*colormap,
			  GdkColor	*colors,
			  gint		 ncolors);
gint SF(gdk_color_white)	 (GdkColormap	*colormap,
			  GdkColor	*color);
gint SF(gdk_color_black)	 (GdkColormap	*colormap,
			  GdkColor	*color);
gint SF(gdk_color_alloc)	 (GdkColormap	*colormap,
			  GdkColor	*color);
gint SF(gdk_color_change)	 (GdkColormap	*colormap,
			  GdkColor	*color);
#endif /* GDK_DISABLE_DEPRECATED */

#if !defined (GDK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
/* Used by SF(gdk_rgb_try_colormap) () */
gint SF(gdk_colors_alloc)	 (GdkColormap	*colormap,
			  gboolean	 contiguous,
			  gulong	*planes,
			  gint		 nplanes,
			  gulong	*pixels,
			  gint		 npixels);
void SF(gdk_colors_free)	 (GdkColormap	*colormap,
			  gulong	*pixels,
			  gint		 npixels,
			  gulong	 planes);
#endif /* !GDK_DISABLE_DEPRECATED || STLWRT_COMPILATION */

G_END_DECLS

#endif /* __GDK_COLOR_H__ */
