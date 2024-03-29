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

#ifndef __GTK_PANED_H__
#define __GTK_PANED_H__

#include <stlwrt.h>


#include <gtkcontainer.h>

G_BEGIN_DECLS

#define GTK_TYPE_PANED                  (gtk_paned_get_type ())
#define GTK_PANED(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PANED, GtkPaned))
#define GTK_PANED_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_PANED, GtkPanedClass))
#define GTK_IS_PANED(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PANED))
#define GTK_IS_PANED_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_PANED))
#define GTK_PANED_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_PANED, GtkPanedClass))

typedef struct _GtkPanedClass   GtkPanedClass;
typedef struct _GtkPanedPrivate GtkPanedPrivate;

STLWRT_DECLARE_VTYPE_VPARENT(GtkPaned, gtk_paned, GtkContainer,
  GtkWidget * (child1);
  GtkWidget * (child2);

  GdkWindow * (handle);
  GdkGC * (xor_gc);
  GdkCursorType  (cursor_type);

  /*< private >*/
  GdkRectangle  (handle_pos);

  gint  (child1_size);
  gint  (last_allocation);
  gint  (min_position);
  gint  (max_position);

  guint  (position_set) : 1;
  guint  (in_drag) : 1;
  guint  (child1_shrink) : 1;
  guint  (child1_resize) : 1;
  guint  (child2_shrink) : 1;
  guint  (child2_resize) : 1;
  guint  (orientation) : 1;
  guint  (in_recursion) : 1;
  guint  (handle_prelit) : 1;

  GtkWidget * (last_child1_focus);
  GtkWidget * (last_child2_focus);
  GtkPanedPrivate * (priv);

  gint  (drag_pos);
  gint  (original_position);
)

struct _GtkPanedClass
{
  GtkContainerClass parent_class;

  gboolean (* cycle_child_focus)   (GtkPaned      *paned,
				    gboolean       reverse);
  gboolean (* toggle_handle_focus) (GtkPaned      *paned);
  gboolean (* move_handle)         (GtkPaned      *paned,
				    GtkScrollType  scroll);
  gboolean (* cycle_handle_focus)  (GtkPaned      *paned,
				    gboolean       reverse);
  gboolean (* accept_position)     (GtkPaned	  *paned);
  gboolean (* cancel_position)     (GtkPaned	  *paned);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


void        SF(gtk_paned_add1)         (GtkPaned       *paned,
                                    GtkWidget      *child);
void        SF(gtk_paned_add2)         (GtkPaned       *paned,
                                    GtkWidget      *child);
void        SF(gtk_paned_pack1)        (GtkPaned       *paned,
                                    GtkWidget      *child,
                                    gboolean        resize,
                                    gboolean        shrink);
void        SF(gtk_paned_pack2)        (GtkPaned       *paned,
                                    GtkWidget      *child,
                                    gboolean        resize,
                                    gboolean        shrink);

gint        SF(gtk_paned_get_position) (GtkPaned       *paned);
void        SF(gtk_paned_set_position) (GtkPaned       *paned,
                                    gint            position);

GtkWidget * SF(gtk_paned_get_child1)   (GtkPaned       *paned);
GtkWidget * SF(gtk_paned_get_child2)   (GtkPaned       *paned);

GdkWindow * SF(gtk_paned_get_handle_window) (GtkPaned  *paned);

#ifndef GTK_DISABLE_DEPRECATED
/* Internal function */
void    SF(gtk_paned_compute_position) (GtkPaned  *paned,
                                    gint       allocation,
                                    gint       child1_req,
                                    gint       child2_req);
#define	gtk_paned_gutter_size(p,s)		(void) 0
#define	gtk_paned_set_gutter_size(p,s)		(void) 0
#endif /* GTK_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GTK_PANED_H__ */
