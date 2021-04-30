/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 * Copyright (C) 1998 Elliot Lee
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

/* The GtkHandleBox is to allow widgets to be dragged in and out of
 * their parents.
 */

#ifndef __GTK_HANDLE_BOX_H__
#define __GTK_HANDLE_BOX_H__

#include <stlwrt.h>


#include <gtkbin.h>


G_BEGIN_DECLS

#define GTK_TYPE_HANDLE_BOX            (gtk_handle_box_get_type ())
#define GTK_HANDLE_BOX(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_HANDLE_BOX, GtkHandleBox))
#define GTK_HANDLE_BOX_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_HANDLE_BOX, GtkHandleBoxClass))
#define GTK_IS_HANDLE_BOX(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_HANDLE_BOX))
#define GTK_IS_HANDLE_BOX_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_HANDLE_BOX))
#define GTK_HANDLE_BOX_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_HANDLE_BOX, GtkHandleBoxClass))

typedef struct _GtkHandleBoxClass  GtkHandleBoxClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkHandleBox, gtk_handle_box, GtkBin,
  GdkWindow      * (bin_window);	/* parent window for children */
  GdkWindow      * (float_window);
  GtkShadowType    (shadow_type);
  guint            (handle_position) : 2;
  guint            (float_window_mapped) : 1;
  guint            (child_detached) : 1;
  guint            (in_drag) : 1;
  guint            (shrink_on_detach) : 1;

  signed int       (snap_edge : 3); /* -1 == unset */

  /* Variables used during a drag
   */
  gint             (deskoff_x); /* Offset between root relative coords */
  gint             (deskoff_y); /* and deskrelative coords             */

  GtkAllocation    (attach_allocation);
  GtkAllocation    (float_allocation);
)

struct _GtkHandleBoxClass
{
  GtkBinClass parent_class;

  void	(*child_attached)	(GtkHandleBox	*handle_box,
				 GtkWidget	*child);
  void	(*child_detached)	(GtkHandleBox	*handle_box,
				 GtkWidget	*child);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GtkWidget*    SF(gtk_handle_box_new)                  (void);
void          SF(gtk_handle_box_set_shadow_type)      (GtkHandleBox    *handle_box,
                                                   GtkShadowType    type);
GtkShadowType SF(gtk_handle_box_get_shadow_type)      (GtkHandleBox    *handle_box);
void          SF(gtk_handle_box_set_handle_position)  (GtkHandleBox    *handle_box,
					           GtkPositionType  position);
GtkPositionType SF(gtk_handle_box_get_handle_position)(GtkHandleBox    *handle_box);
void          SF(gtk_handle_box_set_snap_edge)        (GtkHandleBox    *handle_box,
						   GtkPositionType  edge);
GtkPositionType SF(gtk_handle_box_get_snap_edge)      (GtkHandleBox    *handle_box);
gboolean      SF(gtk_handle_box_get_child_detached)   (GtkHandleBox    *handle_box);

G_END_DECLS

#endif /* __GTK_HANDLE_BOX_H__ */
