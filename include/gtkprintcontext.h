/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtkprintcontext.h: Print Context
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

#ifndef __GTK_PRINT_CONTEXT_H__
#define __GTK_PRINT_CONTEXT_H__

#include <pango/pango.h>
#include <gtkpagesetup.h>


G_BEGIN_DECLS

typedef struct _GtkPrintContext GtkPrintContextFat;
typedef struct _GtkPrintContext GtkPrintContextThin;

#define GTK_TYPE_PRINT_CONTEXT    (SF(gtk_print_context_get_type) ())
#define GTK_PRINT_CONTEXT(obj)    (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PRINT_CONTEXT, GtkPrintContext))
#define GTK_IS_PRINT_CONTEXT(obj) (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PRINT_CONTEXT))

GType          SF(_T2_gtk_print_context_get_type) (void) G_GNUC_CONST;
GType          SF(_3T_gtk_print_context_get_type) (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType          SF(gtk_print_context_get_type) (void) G_GNUC_CONST;


/* Rendering */
cairo_t      *SF(gtk_print_context_get_cairo_context)    (GtkPrintContext *context);

GtkPageSetup *SF(gtk_print_context_get_page_setup)       (GtkPrintContext *context);
gdouble       SF(gtk_print_context_get_width)            (GtkPrintContext *context);
gdouble       SF(gtk_print_context_get_height)           (GtkPrintContext *context);
gdouble       SF(gtk_print_context_get_dpi_x)            (GtkPrintContext *context);
gdouble       SF(gtk_print_context_get_dpi_y)            (GtkPrintContext *context);
gboolean      SF(gtk_print_context_get_hard_margins)     (GtkPrintContext *context,
						      gdouble         *top,
						      gdouble         *bottom,
						      gdouble         *left,
						      gdouble         *right);

/* Fonts */
PangoFontMap *SF(gtk_print_context_get_pango_fontmap)    (GtkPrintContext *context);
PangoContext *SF(gtk_print_context_create_pango_context) (GtkPrintContext *context);
PangoLayout  *SF(gtk_print_context_create_pango_layout)  (GtkPrintContext *context);

/* Needed for preview implementations */
void         SF(gtk_print_context_set_cairo_context)     (GtkPrintContext *context,
						      cairo_t         *cr,
						      double           dpi_x,
						      double           dpi_y);

G_END_DECLS

#endif /* __GTK_PRINT_CONTEXT_H__ */
