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


#include <gtkframe.h>


G_BEGIN_DECLS

#define GTK_TYPE_ASPECT_FRAME            (SF(gtk_aspect_frame_get_type) ())
#define GTK_ASPECT_FRAME(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_ASPECT_FRAME, GtkAspectFrame))
#define GTK_ASPECT_FRAME_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_ASPECT_FRAME, GtkAspectFrameClass))
#define GTK_IS_ASPECT_FRAME(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_ASPECT_FRAME))
#define GTK_IS_ASPECT_FRAME_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_ASPECT_FRAME))
#define GTK_ASPECT_FRAME_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_ASPECT_FRAME, GtkAspectFrameClass))


typedef struct _GtkAspectFrame       GtkAspectFrameFat;
typedef struct _GtkAspectFrame       GtkAspectFrameThin;

typedef struct _GtkAspectFrameClass  GtkAspectFrameClass;

/********************************************************************/
struct _GtkAspectFrameProps
{


  gfloat    (xalign);
  gfloat    (yalign);
  gfloat    (ratio);
  gboolean  (obey_child);

  GtkAllocation  (center_allocation);
};

struct _GtkAspectFrameFat
{
  GtkFrameFat   frame;

  struct _GtkAspectFrameProps instance_properties;
};

struct _GtkAspectFrameThin
{
  GtkFrameThin  frame;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkAspectFrameFat   fat_instance;
  struct _GtkAspectFrameThin  thin_instance;
}   GtkAspectFrame;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkAspectFrameFat GtkAspectFrame;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkAspectFrameThin GtkAspectFrame;
#endif
/********************************************************************/



struct _GtkAspectFrameClass
{
  GtkFrameClass parent_class;
};


GType      SF(_T2_gtk_aspect_frame_get_type)   (void) G_GNUC_CONST;
GType      SF(_3T_gtk_aspect_frame_get_type)   (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType      SF(gtk_aspect_frame_get_type)   (void) G_GNUC_CONST;
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
