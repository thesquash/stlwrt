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

#ifndef __GDK_REGION_H__
#define __GDK_REGION_H__

#include <stlwrt.h>


#include <gdktypes.h>

G_BEGIN_DECLS

#ifndef GDK_DISABLE_DEPRECATED
/* GC fill rule for polygons
 *  EvenOddRule
 *  WindingRule
 */
typedef enum
{
  GDK_EVEN_ODD_RULE,
  GDK_WINDING_RULE
} GdkFillRule;
#endif

/* Types of overlapping between a rectangle and a region
 * GDK_OVERLAP_RECTANGLE_IN: rectangle is in region
 * GDK_OVERLAP_RECTANGLE_OUT: rectangle in not in region
 * GDK_OVERLAP_RECTANGLE_PART: rectangle in partially in region
 */
typedef enum
{
  GDK_OVERLAP_RECTANGLE_IN,
  GDK_OVERLAP_RECTANGLE_OUT,
  GDK_OVERLAP_RECTANGLE_PART
} GdkOverlapType;

#ifndef GDK_DISABLE_DEPRECATED
typedef void (* GdkSpanFunc) (GdkSpan *span,
                              gpointer data);
#endif

GdkRegion    * SF(gdk_region_new)             (void);
#ifndef GDK_DISABLE_DEPRECATED
GdkRegion    * SF(gdk_region_polygon)         (const GdkPoint     *points,
                                           gint                n_points,
                                           GdkFillRule         fill_rule);
#endif
GdkRegion    * SF(gdk_region_copy)            (const GdkRegion    *region);
GdkRegion    * SF(gdk_region_rectangle)       (const GdkRectangle *rectangle);
void           SF(gdk_region_destroy)         (GdkRegion          *region);

void	       SF(gdk_region_get_clipbox)     (const GdkRegion    *region,
                                           GdkRectangle       *rectangle);
void           SF(gdk_region_get_rectangles)  (const GdkRegion    *region,
                                           GdkRectangle      **rectangles,
                                           gint               *n_rectangles);

gboolean       SF(gdk_region_empty)           (const GdkRegion    *region);
gboolean       SF(gdk_region_equal)           (const GdkRegion    *region1,
                                           const GdkRegion    *region2);
#ifndef GDK_DISABLE_DEPRECATED
gboolean       SF(gdk_region_rect_equal)      (const GdkRegion    *region,
                                           const GdkRectangle *rectangle);
#endif
gboolean       SF(gdk_region_point_in)        (const GdkRegion    *region,
                                           int                 x,
                                           int                 y);
GdkOverlapType SF(gdk_region_rect_in)         (const GdkRegion    *region,
                                           const GdkRectangle *rectangle);

void           SF(gdk_region_offset)          (GdkRegion          *region,
                                           gint                dx,
                                           gint                dy);
#ifndef GDK_DISABLE_DEPRECATED
void           SF(gdk_region_shrink)          (GdkRegion          *region,
                                           gint                dx,
                                           gint                dy);
#endif
void           SF(gdk_region_union_with_rect) (GdkRegion          *region,
                                           const GdkRectangle *rect);
void           SF(gdk_region_intersect)       (GdkRegion          *source1,
                                           const GdkRegion    *source2);
void           SF(gdk_region_union)           (GdkRegion          *source1,
                                           const GdkRegion    *source2);
void           SF(gdk_region_subtract)        (GdkRegion          *source1,
                                           const GdkRegion    *source2);
void           SF(gdk_region_xor)             (GdkRegion          *source1,
                                           const GdkRegion    *source2);

#ifndef GDK_DISABLE_DEPRECATED
void   SF(gdk_region_spans_intersect_foreach) (GdkRegion          *region,
                                           const GdkSpan      *spans,
                                           int                 n_spans,
                                           gboolean            sorted,
                                           GdkSpanFunc         function,
                                           gpointer            data);
#endif

G_END_DECLS

#endif /* __GDK_REGION_H__ */
