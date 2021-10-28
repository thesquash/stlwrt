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

typedef GdkSegment GdkRegionBox;

/* 
 *   clip region
 */

STLWRT_DECLARE_STRUCTURE(GdkRegion,
  long size;
  long numRects;
  GdkRegionBox *rects;
  GdkRegionBox extents;
)

/*  1 if two BOXs overlap.
 *  0 if two BOXs do not overlap.
 *  Remember, x2 and y2 are not in the region 
 */
#define EXTENTCHECK(r1, r2) \
	((r1)->x2 > (r2)->x1 && \
	 (r1)->x1 < (r2)->x2 && \
	 (r1)->y2 > (r2)->y1 && \
	 (r1)->y1 < (r2)->y2)

/*
 *  update region extents
 */
#define EXTENTS(r,idRect){\
            if((r)->x1 < (idRect)->extents.x1)\
              (idRect)->extents.x1 = (r)->x1;\
            if((r)->y1 < (idRect)->extents.y1)\
              (idRect)->extents.y1 = (r)->y1;\
            if((r)->x2 > (idRect)->extents.x2)\
              (idRect)->extents.x2 = (r)->x2;\
            if((r)->y2 > (idRect)->extents.y2)\
              (idRect)->extents.y2 = (r)->y2;\
        }

#define GROWREGION(reg, nRects) {  					   \
	  if ((nRects) == 0) {						   \
            if ((reg)->rects != &(reg)->extents) {			   \
	      g_free ((reg)->rects);					   \
              (reg)->rects = &(reg)->extents;				   \
	    }								   \
	  }  								   \
	  else if ((reg)->rects == &(reg)->extents) {                      \
            (reg)->rects = g_new (GdkRegionBox, (nRects));		   \
            (reg)->rects[0] = (reg)->extents;                              \
          }                                                                \
          else                                                             \
            (reg)->rects = g_renew (GdkRegionBox, (reg)->rects, (nRects)); \
	  (reg)->size = (nRects);                                          \
       }				 

/*
 *   Check to see if there is enough memory in the present region.
 */
#define MEMCHECK(reg, rect, firstrect){					  	 \
        if ((reg)->numRects >= ((reg)->size - 1)) {			 	 \
          GROWREGION(reg,2*(reg)->size);                                         \
          (rect) = &(firstrect)[(reg)->numRects];				 \
         }									 \
       }

/*  this routine checks to see if the previous rectangle is the same
 *  or subsumes the new rectangle to add.
 */

#define CHECK_PREVIOUS(Reg, R, Rx1, Ry1, Rx2, Ry2)\
               (!(((Reg)->numRects > 0)&&\
                  ((R-1)->y1 == (Ry1)) &&\
                  ((R-1)->y2 == (Ry2)) &&\
                  ((R-1)->x1 <= (Rx1)) &&\
                  ((R-1)->x2 >= (Rx2))))

/*  add a rectangle to the given Region */
#define ADDRECT(reg, r, rx1, ry1, rx2, ry2){\
    if (((rx1) < (rx2)) && ((ry1) < (ry2)) &&\
        CHECK_PREVIOUS((reg), (r), (rx1), (ry1), (rx2), (ry2))){\
              (r)->x1 = (rx1);\
              (r)->y1 = (ry1);\
              (r)->x2 = (rx2);\
              (r)->y2 = (ry2);\
              EXTENTS((r), (reg));\
              (reg)->numRects++;\
              (r)++;\
            }\
        }



/*  add a rectangle to the given Region */
#define ADDRECTNOX(reg, r, rx1, ry1, rx2, ry2){\
            if ((rx1 < rx2) && (ry1 < ry2) &&\
                CHECK_PREVIOUS((reg), (r), (rx1), (ry1), (rx2), (ry2))){\
              (r)->x1 = (rx1);\
              (r)->y1 = (ry1);\
              (r)->x2 = (rx2);\
              (r)->y2 = (ry2);\
              (reg)->numRects++;\
              (r)++;\
            }\
        }

#define EMPTY_REGION(pReg) pReg->numRects = 0

#define REGION_NOT_EMPTY(pReg) pReg->numRects

#define INBOX(r, x, y) \
      ( ( ((r).x2 >  x)) && \
        ( ((r).x1 <= x)) && \
        ( ((r).y2 >  y)) && \
        ( ((r).y1 <= y)) )

/*
 * number of points to buffer before sending them off
 * to scanlines() :  Must be an even number
 */
#define NUMPTSTOBUFFER 200

/*
 * used to allocate buffers for points and link
 * the buffers together
 */
typedef struct _POINTBLOCK {
  GdkPoint pts[NUMPTSTOBUFFER];
  struct _POINTBLOCK *next;
} POINTBLOCK;

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
