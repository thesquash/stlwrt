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


#ifndef __GTK_ASPECT_FRAME_H__
#define __GTK_ASPECT_FRAME_H__

#include <stlwrt.h>



#include <gtkframe.h>


G_BEGIN_DECLS

#define GTK_TYPE_ASPECT_FRAME            (gtk_aspect_frame_get_type ())
#define GTK_ASPECT_FRAME(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_ASPECT_FRAME, GtkAspectFrame))
#define GTK_ASPECT_FRAME_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_ASPECT_FRAME, GtkAspectFrameClass))
#define GTK_IS_ASPECT_FRAME(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_ASPECT_FRAME))
#define GTK_IS_ASPECT_FRAME_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_ASPECT_FRAME))
#define GTK_ASPECT_FRAME_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_ASPECT_FRAME, GtkAspectFrameClass))

typedef struct _GtkAspectFrameClass  GtkAspectFrameClass;

/********************************************************************/
STLWRT_DECLARE_VTYPE_VPARENT(GtkAspectFrame, gtk_aspect_frame, GtkFrame,
  gfloat    (xalign);
  gfloat    (yalign);
  gfloat    (ratio);
  gboolean  (obey_child);

  GtkAllocation  (center_allocation);
)

struct _GtkAspectFrameClass
{
  GtkFrameClass parent_class;
};


GtkWidget* SF(gtk_aspect_frame_new)        (const gchar     *label,
					gfloat           xalign,
					gfloat           yalign,
					gfloat           ratio,
					gboolean         obey_child);
void       SF(gtk_aspect_frame_set)        (GtkAspectFrame  *aspect_frame,
					gfloat           xalign,
					gfloat           yalign,
					gfloat           ratio,
					gboolean         obey_child);

G_END_DECLS

#endif /* __GTK_ASPECT_FRAME_H__ */
