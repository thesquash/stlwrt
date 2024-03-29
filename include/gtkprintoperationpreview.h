/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtkprintoperationpreview.h: Abstract print preview interface
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

#ifndef __GTK_PRINT_OPERATION_PREVIEW_H__
#define __GTK_PRINT_OPERATION_PREVIEW_H__

#include <stlwrt.h>


#include <cairo.h>

#include <gtkprintcontext.h>

G_BEGIN_DECLS

#define GTK_TYPE_PRINT_OPERATION_PREVIEW                  (gtk_print_operation_preview_get_type ())
#define GTK_PRINT_OPERATION_PREVIEW(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PRINT_OPERATION_PREVIEW, GtkPrintOperationPreview))
#define GTK_IS_PRINT_OPERATION_PREVIEW(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PRINT_OPERATION_PREVIEW))
#define GTK_PRINT_OPERATION_PREVIEW_GET_IFACE(obj)        (G_TYPE_INSTANCE_GET_INTERFACE ((obj), GTK_TYPE_PRINT_OPERATION_PREVIEW, GtkPrintOperationPreviewIface))

STLWRT_DECLARE_INTERFACE(GtkPrintOperationPreview, gtk_print_operation_preview,
  /* signals */
  void              (*ready)          (GtkPrintOperationPreview *preview,
				       GtkPrintContext          *context);
  void              (*got_page_size)  (GtkPrintOperationPreview *preview,
				       GtkPrintContext          *context,
				       GtkPageSetup             *page_setup);

  /* methods */
  void              (*render_page)    (GtkPrintOperationPreview *preview,
				       gint                      page_nr);
  gboolean          (*is_selected)    (GtkPrintOperationPreview *preview,
				       gint                      page_nr);
  void              (*end_preview)    (GtkPrintOperationPreview *preview);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
  void (*_gtk_reserved5) (void);
  void (*_gtk_reserved6) (void);
  void (*_gtk_reserved7) (void);
)

void     SF(gtk_print_operation_preview_render_page) (GtkPrintOperationPreview *preview,
						  gint                      page_nr);
void     SF(gtk_print_operation_preview_end_preview) (GtkPrintOperationPreview *preview);
gboolean SF(gtk_print_operation_preview_is_selected) (GtkPrintOperationPreview *preview,
						  gint                      page_nr);

G_END_DECLS

#endif /* __GTK_PRINT_OPERATION_PREVIEW_H__ */
