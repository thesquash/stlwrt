/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtkpagesetup.h: Page Setup
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

#ifndef __GTK_PAGE_SETUP_H__
#define __GTK_PAGE_SETUP_H__

#include <stlwrt.h>


#include <gtkpapersize.h>


G_BEGIN_DECLS

#define GTK_TYPE_PAGE_SETUP    (gtk_page_setup_get_type ())
#define GTK_PAGE_SETUP(obj)    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PAGE_SETUP, GtkPageSetup))
#define GTK_IS_PAGE_SETUP(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PAGE_SETUP))

STLWRT_DECLARE_OPAQUE_TYPE(GtkPageSetup, gtk_page_setup)

GtkPageSetup *     SF(gtk_page_setup_new)               (void);
GtkPageSetup *     SF(gtk_page_setup_copy)              (GtkPageSetup       *other);
GtkPageOrientation SF(gtk_page_setup_get_orientation)   (GtkPageSetup       *setup);
void               SF(gtk_page_setup_set_orientation)   (GtkPageSetup       *setup,
						     GtkPageOrientation  orientation);
GtkPaperSize *     SF(gtk_page_setup_get_paper_size)    (GtkPageSetup       *setup);
void               SF(gtk_page_setup_set_paper_size)    (GtkPageSetup       *setup,
						     GtkPaperSize       *size);
gdouble            SF(gtk_page_setup_get_top_margin)    (GtkPageSetup       *setup,
						     GtkUnit             unit);
void               SF(gtk_page_setup_set_top_margin)    (GtkPageSetup       *setup,
						     gdouble             margin,
						     GtkUnit             unit);
gdouble            SF(gtk_page_setup_get_bottom_margin) (GtkPageSetup       *setup,
						     GtkUnit             unit);
void               SF(gtk_page_setup_set_bottom_margin) (GtkPageSetup       *setup,
						     gdouble             margin,
						     GtkUnit             unit);
gdouble            SF(gtk_page_setup_get_left_margin)   (GtkPageSetup       *setup,
						     GtkUnit             unit);
void               SF(gtk_page_setup_set_left_margin)   (GtkPageSetup       *setup,
						     gdouble             margin,
						     GtkUnit             unit);
gdouble            SF(gtk_page_setup_get_right_margin)  (GtkPageSetup       *setup,
						     GtkUnit             unit);
void               SF(gtk_page_setup_set_right_margin)  (GtkPageSetup       *setup,
						     gdouble             margin,
						     GtkUnit             unit);

void SF(gtk_page_setup_set_paper_size_and_default_margins) (GtkPageSetup    *setup,
							GtkPaperSize    *size);

/* These take orientation, but not margins into consideration */
gdouble            SF(gtk_page_setup_get_paper_width)   (GtkPageSetup       *setup,
						     GtkUnit             unit);
gdouble            SF(gtk_page_setup_get_paper_height)  (GtkPageSetup       *setup,
						     GtkUnit             unit);


/* These take orientation, and margins into consideration */
gdouble            SF(gtk_page_setup_get_page_width)    (GtkPageSetup       *setup,
						     GtkUnit             unit);
gdouble            SF(gtk_page_setup_get_page_height)   (GtkPageSetup       *setup,
						     GtkUnit             unit);

/* Saving and restoring page setup */
GtkPageSetup	  *SF(gtk_page_setup_new_from_file)	    (const gchar         *file_name,
						     GError              **error);
gboolean	   SF(gtk_page_setup_load_file)	    (GtkPageSetup        *setup,
						     const char          *file_name,
						     GError             **error);
gboolean	   SF(gtk_page_setup_to_file)	    (GtkPageSetup        *setup,
						     const char          *file_name,
						     GError             **error);
GtkPageSetup	  *SF(gtk_page_setup_new_from_key_file) (GKeyFile            *key_file,
						     const gchar         *group_name,
						     GError             **error);
gboolean           SF(gtk_page_setup_load_key_file)     (GtkPageSetup        *setup,
				                     GKeyFile            *key_file,
				                     const gchar         *group_name,
				                     GError             **error);
void		   SF(gtk_page_setup_to_key_file)	    (GtkPageSetup        *setup,
						     GKeyFile            *key_file,
						     const gchar         *group_name);

G_END_DECLS

#endif /* __GTK_PAGE_SETUP_H__ */
