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

#ifndef __GDK_GC_H__
#define __GDK_GC_H__

#include <stlwrt.h>

#include <gdkcolor.h>
#include <gdktypes.h>

G_BEGIN_DECLS

typedef struct _GdkGCClass	      GdkGCClass;


STLWRT_DECLARE_STRUCTURE(GdkGCValues,
  GdkColor  	    foreground;
  GdkColor	    background;
  GdkFont	   *font;
  GdkFunction	    function;
  GdkFill	    fill;
  GdkPixmap	   *tile;
  GdkPixmap	   *stipple;
  GdkPixmap	   *clip_mask;
  GdkSubwindowMode  subwindow_mode;
  gint		    ts_x_origin;
  gint		    ts_y_origin;
  gint		    clip_x_origin;
  gint		    clip_y_origin;
  gint		    graphics_exposures;
  gint		    line_width;
  GdkLineStyle	    line_style;
  GdkCapStyle	    cap_style;
  GdkJoinStyle	    join_style;
)

#define GDK_TYPE_GC              (gdk_gc_get_type ())
#define GDK_GC(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GDK_TYPE_GC, GdkGC))
#define GDK_GC_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GDK_TYPE_GC, GdkGCClass))
#define GDK_IS_GC(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GDK_TYPE_GC))
#define GDK_IS_GC_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GDK_TYPE_GC))
#define GDK_GC_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GDK_TYPE_GC, GdkGCClass))


STLWRT_DECLARE_FTYPE_FPARENT(GdkGC, gdk_gc, GObject,
  gint  (clip_x_origin);
  gint  (clip_y_origin);
  gint  (ts_x_origin);
  gint  (ts_y_origin);

  GdkColormap * (colormap);
)


struct _GdkGCClass 
{
  GObjectClass parent_class;
  
  void (*get_values)     (GdkGC          *gc,
			  GdkGCValues    *values);
  void (*set_values)     (GdkGC          *gc,
			  GdkGCValues    *values,
			  GdkGCValuesMask mask);
  void (*set_dashes)     (GdkGC          *gc,
			  gint	          dash_offset,
			  gint8           dash_list[],
			  gint            n);
  
  /* Padding for future expansion */
  void         (*_gdk_reserved1)  (void);
  void         (*_gdk_reserved2)  (void);
  void         (*_gdk_reserved3)  (void);
  void         (*_gdk_reserved4)  (void);
};


#ifndef GDK_DISABLE_DEPRECATED
GdkGC *SF(gdk_gc_new)		  (GdkDrawable	    *drawable);
GdkGC *SF(gdk_gc_new_with_values)	  (GdkDrawable	    *drawable,
				   GdkGCValues	    *values,
				   GdkGCValuesMask   values_mask);

GdkGC *SF(gdk_gc_ref)		  (GdkGC	    *gc);
void   SF(gdk_gc_unref)		  (GdkGC	    *gc);

void   SF(gdk_gc_get_values)	  (GdkGC	    *gc,
				   GdkGCValues	    *values);
void   SF(gdk_gc_set_values)          (GdkGC           *gc,
                                   GdkGCValues	   *values,
                                   GdkGCValuesMask  values_mask);
void   SF(gdk_gc_set_foreground)	  (GdkGC	    *gc,
				   const GdkColor   *color);
void   SF(gdk_gc_set_background)	  (GdkGC	    *gc,
				   const GdkColor   *color);
void   SF(gdk_gc_set_font)		  (GdkGC	    *gc,
				   GdkFont	    *font);
void   SF(gdk_gc_set_function)	  (GdkGC	    *gc,
				   GdkFunction	     function);
void   SF(gdk_gc_set_fill)		  (GdkGC	    *gc,
				   GdkFill	     fill);
void   SF(gdk_gc_set_tile)		  (GdkGC	    *gc,
				   GdkPixmap	    *tile);
void   SF(gdk_gc_set_stipple)	  (GdkGC	    *gc,
				   GdkPixmap	    *stipple);
void   SF(gdk_gc_set_ts_origin)	  (GdkGC	    *gc,
				   gint		     x,
				   gint		     y);
void   SF(gdk_gc_set_clip_origin)	  (GdkGC	    *gc,
				   gint		     x,
				   gint		     y);
void   SF(gdk_gc_set_clip_mask)	  (GdkGC	    *gc,
				   GdkBitmap	    *mask);
void   SF(gdk_gc_set_clip_rectangle)  (GdkGC	    *gc,
				   const GdkRectangle *rectangle);
void   SF(gdk_gc_set_clip_region)	  (GdkGC	    *gc,
				   const GdkRegion  *region);
void   SF(gdk_gc_set_subwindow)	  (GdkGC	    *gc,
				   GdkSubwindowMode  mode);
void   SF(gdk_gc_set_exposures)	  (GdkGC	    *gc,
				   gboolean	     exposures);
void   SF(gdk_gc_set_line_attributes) (GdkGC	    *gc,
				   gint		     line_width,
				   GdkLineStyle	     line_style,
				   GdkCapStyle	     cap_style,
				   GdkJoinStyle	     join_style);
void   SF(gdk_gc_set_dashes)          (GdkGC            *gc,
				   gint	             dash_offset,
				   gint8             dash_list[],
				   gint              n);
void   SF(gdk_gc_offset)              (GdkGC            *gc,
				   gint              x_offset,
				   gint              y_offset);
void   SF(gdk_gc_copy)		  (GdkGC	    *dst_gc,
				   GdkGC	    *src_gc);


void         SF(gdk_gc_set_colormap)     (GdkGC          *gc,
				      GdkColormap    *colormap);
GdkColormap *SF(gdk_gc_get_colormap)     (GdkGC          *gc);
void         SF(gdk_gc_set_rgb_fg_color) (GdkGC          *gc,
				      const GdkColor *color);
void         SF(gdk_gc_set_rgb_bg_color) (GdkGC          *gc,
				      const GdkColor *color);
GdkScreen *  SF(gdk_gc_get_screen)	     (GdkGC          *gc);

#define gdk_gc_destroy                 g_object_unref
#endif /* GDK_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GDK_DRAWABLE_H__ */
