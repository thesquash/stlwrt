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
 *
 * GtkLayout: Widget for scrolling of arbitrary-sized areas.
 *
 * Copyright Owen Taylor, 1998
 */

#ifndef __GTK_LAYOUT_H__
#define __GTK_LAYOUT_H__

#include <stlwrt.h>


#include <gtkcontainer.h>

#include <gtkadjustment.h>


G_BEGIN_DECLS

#define GTK_TYPE_LAYOUT            (gtk_layout_get_type ())
#define GTK_LAYOUT(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_LAYOUT, GtkLayout))
#define GTK_LAYOUT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_LAYOUT, GtkLayoutClass))
#define GTK_IS_LAYOUT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_LAYOUT))
#define GTK_IS_LAYOUT_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_LAYOUT))
#define GTK_LAYOUT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_LAYOUT, GtkLayoutClass))

typedef struct _GtkLayoutClass   GtkLayoutClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkLayout, gtk_layout, GtkContainer,
  GList * (children);

  guint  (width);
  guint  (height);

  GtkAdjustment * (hadjustment);
  GtkAdjustment * (vadjustment);

  /*< public >*/
  GdkWindow * (bin_window);

  /*< private >*/
  GdkVisibilityState  (visibility);
  gint  (scroll_x);
  gint  (scroll_y);

  guint  (freeze_count);
)

struct _GtkLayoutClass
{
  GtkContainerClass parent_class;

  void  (*set_scroll_adjustments)   (GtkLayout	    *layout,
				     GtkAdjustment  *hadjustment,
				     GtkAdjustment  *vadjustment);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GtkWidget*     SF(gtk_layout_new)             (GtkAdjustment *hadjustment,
				           GtkAdjustment *vadjustment);
GdkWindow*     SF(gtk_layout_get_bin_window)  (GtkLayout     *layout);
void           SF(gtk_layout_put)             (GtkLayout     *layout,
		                           GtkWidget     *child_widget,
		                           gint           x,
		                           gint           y);

void           SF(gtk_layout_move)            (GtkLayout     *layout,
		                           GtkWidget     *child_widget,
		                           gint           x,
		                           gint           y);

void           SF(gtk_layout_set_size)        (GtkLayout     *layout,
			                   guint          width,
			                   guint          height);
void           SF(gtk_layout_get_size)        (GtkLayout     *layout,
					   guint         *width,
					   guint         *height);

GtkAdjustment* SF(gtk_layout_get_hadjustment) (GtkLayout     *layout);
GtkAdjustment* SF(gtk_layout_get_vadjustment) (GtkLayout     *layout);
void           SF(gtk_layout_set_hadjustment) (GtkLayout     *layout,
					   GtkAdjustment *adjustment);
void           SF(gtk_layout_set_vadjustment) (GtkLayout     *layout,
					   GtkAdjustment *adjustment);


#ifndef GTK_DISABLE_DEPRECATED
/* These disable and enable moving and repainting the scrolling window
 * of the GtkLayout, respectively.  If you want to update the layout's
 * offsets but do not want it to repaint itself, you should use these
 * functions.
 *
 * - I don't understand these are supposed to work, so I suspect
 * - they don't now.                    OWT 1/20/98
 */
void           SF(gtk_layout_freeze)          (GtkLayout     *layout);
void           SF(gtk_layout_thaw)            (GtkLayout     *layout);
#endif /* GTK_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GTK_LAYOUT_H__ */
