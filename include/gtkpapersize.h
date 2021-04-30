/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtkpapersize.h: Paper Size
 * Copyright (C) 2006, Red Hat, Inc.
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

#ifndef __GTK_PAPER_SIZE_H__
#define __GTK_PAPER_SIZE_H__

#include <stlwrt.h>


#include <gtkenums.h>


G_BEGIN_DECLS

#define GTK_TYPE_PAPER_SIZE    (gtk_paper_size_get_type ())

STLWRT_DECLARE_OPAQUE_TYPE(GtkPaperSize, gtk_paper_size)

/* Common names, from PWG 5101.1-2002 PWG: Standard for Media Standardized Names */
#define GTK_PAPER_NAME_A3 "iso_a3"
#define GTK_PAPER_NAME_A4 "iso_a4"
#define GTK_PAPER_NAME_A5 "iso_a5"
#define GTK_PAPER_NAME_B5 "iso_b5"
#define GTK_PAPER_NAME_LETTER "na_letter"
#define GTK_PAPER_NAME_EXECUTIVE "na_executive"
#define GTK_PAPER_NAME_LEGAL "na_legal"

GtkPaperSize *SF(gtk_paper_size_new)          (const gchar  *name);
GtkPaperSize *SF(gtk_paper_size_new_from_ppd) (const gchar  *ppd_name,
					   const gchar  *ppd_display_name,
					   gdouble       width,
					   gdouble       height);
GtkPaperSize *SF(gtk_paper_size_new_custom)   (const gchar  *name,
					   const gchar  *display_name,
					   gdouble       width,
					   gdouble       height,
					   GtkUnit       unit);
GtkPaperSize *SF(gtk_paper_size_copy)         (GtkPaperSize *other);
void          SF(gtk_paper_size_free)         (GtkPaperSize *size);
gboolean      SF(gtk_paper_size_is_equal)     (GtkPaperSize *size1,
					   GtkPaperSize *size2);

GList        *SF(gtk_paper_size_get_paper_sizes) (gboolean include_custom);

/* The width is always the shortest side, measure in mm */
const gchar *SF(gtk_paper_size_get_name)         (GtkPaperSize *size);
const gchar *SF(gtk_paper_size_get_display_name) (GtkPaperSize *size);
const gchar *SF(gtk_paper_size_get_ppd_name)     (GtkPaperSize *size);

gdouble  SF(gtk_paper_size_get_width)        (GtkPaperSize *size, GtkUnit unit);
gdouble  SF(gtk_paper_size_get_height)       (GtkPaperSize *size, GtkUnit unit);
gboolean SF(gtk_paper_size_is_custom)        (GtkPaperSize *size);

/* Only for custom sizes: */
void    SF(gtk_paper_size_set_size)                  (GtkPaperSize *size, 
                                                  gdouble       width, 
                                                  gdouble       height, 
                                                  GtkUnit       unit);

gdouble SF(gtk_paper_size_get_default_top_margin)    (GtkPaperSize *size,
						  GtkUnit       unit);
gdouble SF(gtk_paper_size_get_default_bottom_margin) (GtkPaperSize *size,
						  GtkUnit       unit);
gdouble SF(gtk_paper_size_get_default_left_margin)   (GtkPaperSize *size,
						  GtkUnit       unit);
gdouble SF(gtk_paper_size_get_default_right_margin)  (GtkPaperSize *size,
						  GtkUnit       unit);

const gchar *SF(gtk_paper_size_get_default) (void);

GtkPaperSize *SF(gtk_paper_size_new_from_key_file) (GKeyFile    *key_file,
					        const gchar *group_name,
					        GError     **error);
void     SF(gtk_paper_size_to_key_file)            (GtkPaperSize *size,
					        GKeyFile     *key_file,
					        const gchar  *group_name);

G_END_DECLS

#endif /* __GTK_PAPER_SIZE_H__ */
