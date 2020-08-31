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

#ifndef __GTK_FRAME_H__
#define __GTK_FRAME_H__

#include <gtkbin.h>


G_BEGIN_DECLS


#define GTK_TYPE_FRAME                  (SF(gtk_frame_get_type) ())
#define GTK_FRAME(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_FRAME, GtkFrame))
#define GTK_FRAME_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_FRAME, GtkFrameClass))
#define GTK_IS_FRAME(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_FRAME))
#define GTK_IS_FRAME_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_FRAME))
#define GTK_FRAME_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_FRAME, GtkFrameClass))


typedef struct _GtkFrame       GtkFrameFat;
typedef struct _GtkFrame       GtkFrameThin;

typedef struct _GtkFrameClass  GtkFrameClass;

/********************************************************************/
struct _GtkFrameProps
{


  GtkWidget * (label_widget);
  gint16  (shadow_type);
  gfloat  (label_xalign);
  gfloat  (label_yalign);

  GtkAllocation  (child_allocation);
};

struct _GtkFrameFat
{
  GtkBinFat   bin;

  struct _GtkFrameProps instance_properties;
};

struct _GtkFrameThin
{
  GtkBinThin  bin;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkFrameFat   fat_instance;
  struct _GtkFrameThin  thin_instance;
}   GtkFrame;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkFrameFat GtkFrame;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkFrameThin GtkFrame;
#endif
/********************************************************************/



struct _GtkFrameClass
{
  GtkBinClass parent_class;

  void (*compute_child_allocation) (GtkFrame *frame, GtkAllocation *allocation);
};


GType      SF(_T2_gtk_frame_get_type)         (void) G_GNUC_CONST;
GType      SF(_3T_gtk_frame_get_type)         (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType      SF(gtk_frame_get_type)         (void) G_GNUC_CONST;
GtkWidget* SF(gtk_frame_new)              (const gchar   *label);

void                  SF(gtk_frame_set_label) (GtkFrame    *frame,
					   const gchar *label);
const gchar *SF(gtk_frame_get_label)      (GtkFrame    *frame);

void       SF(gtk_frame_set_label_widget) (GtkFrame      *frame,
				       GtkWidget     *label_widget);
GtkWidget *SF(gtk_frame_get_label_widget) (GtkFrame      *frame);
void       SF(gtk_frame_set_label_align)  (GtkFrame      *frame,
				       gfloat         xalign,
				       gfloat         yalign);
void       SF(gtk_frame_get_label_align)  (GtkFrame      *frame,
				       gfloat        *xalign,
				       gfloat        *yalign);
void       SF(gtk_frame_set_shadow_type)  (GtkFrame      *frame,
				       GtkShadowType  type);
GtkShadowType SF(gtk_frame_get_shadow_type) (GtkFrame    *frame);


G_END_DECLS


#endif /* __GTK_FRAME_H__ */
