/* GtkPrintJob
 * Copyright (C) 2006 Red Hat,Inc.
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
 */

#ifndef __GTK_PRINT_JOB_H__
#define __GTK_PRINT_JOB_H__

#include <stlwrt.h>


#include <cairo.h>


#include <gtk.h>

#include <gtkprinter.h>

G_BEGIN_DECLS

#define GTK_TYPE_PRINT_JOB                  (gtk_print_job_get_type ())
#define GTK_PRINT_JOB(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PRINT_JOB, GtkPrintJob))
#define GTK_PRINT_JOB_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_PRINT_JOB, GtkPrintJobClass))
#define GTK_IS_PRINT_JOB(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PRINT_JOB))
#define GTK_IS_PRINT_JOB_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_PRINT_JOB))
#define GTK_PRINT_JOB_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_PRINT_JOB, GtkPrintJobClass))

typedef struct _GtkPrintJobClass     GtkPrintJobClass;
typedef struct _GtkPrintJobPrivate   GtkPrintJobPrivate;

typedef void (*GtkPrintJobCompleteFunc) (GtkPrintJob *print_job,
                                         gpointer     user_data,
                                         GError      *error);

STLWRT_DECLARE_VTYPE_FPARENT(GtkPrintJob, gtk_print_job, GObject,
  GtkPrintJobPrivate * (priv);

  /* Settings the client has to implement:
   * (These are read-only, set at initialization)
   */
  GtkPrintPages  (print_pages);
  GtkPageRange * (page_ranges);
  gint  (num_page_ranges);
  GtkPageSet  (page_set);
  gint  (num_copies);
  gdouble  (scale);
  guint  (rotate_to_orientation) : 1;
  guint  (collate)               : 1;
  guint  (reverse)               : 1;
  guint  (number_up);
  GtkNumberUpLayout  (number_up_layout);
)

struct _GtkPrintJobClass
{
  GObjectClass parent_class;

  void (*status_changed) (GtkPrintJob *job);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
  void (*_gtk_reserved5) (void);
  void (*_gtk_reserved6) (void);
  void (*_gtk_reserved7) (void);
};


GtkPrintJob             *gtk_print_job_new                    (const gchar              *title,
							       GtkPrinter               *printer,
							       GtkPrintSettings         *settings,
							       GtkPageSetup             *page_setup);
GtkPrintSettings        *gtk_print_job_get_settings           (GtkPrintJob              *job);
GtkPrinter              *gtk_print_job_get_printer            (GtkPrintJob              *job);
const gchar *            gtk_print_job_get_title              (GtkPrintJob              *job);
GtkPrintStatus           gtk_print_job_get_status             (GtkPrintJob              *job);
gboolean                 gtk_print_job_set_source_file        (GtkPrintJob              *job,
							       const gchar              *filename,
							       GError                  **error);
cairo_surface_t         *gtk_print_job_get_surface            (GtkPrintJob              *job,
							       GError                  **error);
void                     gtk_print_job_set_track_print_status (GtkPrintJob              *job,
							       gboolean                  track_status);
gboolean                 gtk_print_job_get_track_print_status (GtkPrintJob              *job);
void                     gtk_print_job_send                   (GtkPrintJob              *job,
							       GtkPrintJobCompleteFunc   callback,
							       gpointer                  user_data,
							       GDestroyNotify            dnotify);

G_END_DECLS

#endif /* __GTK_PRINT_JOB_H__ */
