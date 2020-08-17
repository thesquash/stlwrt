
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

#ifndef __GTK_ALIGNMENT_H__
#define __GTK_ALIGNMENT_H__


#include <gtkbin.h>


G_BEGIN_DECLS

#define GTK_TYPE_ALIGNMENT                  (gtk_alignment_get_type ())
#define GTK_ALIGNMENT(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_ALIGNMENT, GtkAlignment))
#define GTK_ALIGNMENT_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_ALIGNMENT, GtkAlignmentClass))
#define GTK_IS_ALIGNMENT(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_ALIGNMENT))
#define GTK_IS_ALIGNMENT_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_ALIGNMENT))
#define GTK_ALIGNMENT_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_ALIGNMENT, GtkAlignmentClass))


typedef struct _GtkAlignment       GtkAlignmentFat;
typedef struct _GtkAlignment       GtkAlignmentThin;

typedef struct _GtkAlignmentClass  GtkAlignmentClass;
typedef struct _GtkAlignmentPrivate GtkAlignmentPrivate;

/********************************************************************/
struct _GtkAlignmentProps
{


  gfloat  (xalign);
  gfloat  (yalign);
  gfloat  (xscale);
  gfloat  (yscale);
};

struct _GtkAlignmentPrivate
{

  
  guint padding_top;
  guint padding_bottom;
  guint padding_left;
  guint padding_right;
};

struct _GtkAlignmentFat
{
  GtkBinFat   bin;

  struct _GtkAlignmentProps instance_properties;
};

struct _GtkAlignmentThin
{
  GtkBinThin  bin;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkAlignmentFat   fat_instance;
  struct _GtkAlignmentThin  thin_instance;
}   GtkAlignment;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkAlignmentFat GtkAlignment;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkAlignmentThin GtkAlignment;
#endif
/********************************************************************/



struct _GtkAlignmentClass
{
  GtkBinClass parent_class;
};


GType      _T2_gtk_alignment_get_type   (void) G_GNUC_CONST;
GType      _3T_gtk_alignment_get_type   (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType      gtk_alignment_get_type   (void) G_GNUC_CONST;
GtkWidget* __gtk_alignment_new        (gfloat             xalign,
				     gfloat             yalign,
				     gfloat             xscale,
				     gfloat             yscale);
void       __gtk_alignment_set        (GtkAlignment      *alignment,
				     gfloat             xalign,
				     gfloat             yalign,
				     gfloat             xscale,
				     gfloat             yscale);

void       __gtk_alignment_set_padding (GtkAlignment      *alignment,
				      guint              padding_top,
				      guint              padding_bottom,
				      guint              padding_left,
				      guint              padding_right);

void       __gtk_alignment_get_padding (GtkAlignment      *alignment,
				      guint             *padding_top,
				      guint             *padding_bottom,
				      guint             *padding_left,
				      guint             *padding_right);

G_END_DECLS


#endif /* __GTK_ALIGNMENT_H__ */