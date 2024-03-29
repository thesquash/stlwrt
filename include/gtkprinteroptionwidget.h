/* GtkPrinterOptionWidget 
 * Copyright (C) 2006 John (J5) Palmieri <johnp@redhat.com>
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
#ifndef __GTK_PRINTER_OPTION_WIDGET_H__
#define __GTK_PRINTER_OPTION_WIDGET_H__

#include <stlwrt.h>


#include "gtkprinteroption.h"

#include "gtkhbox.h"

G_BEGIN_DECLS

#define GTK_TYPE_PRINTER_OPTION_WIDGET                  (gtk_printer_option_widget_get_type ())
#define GTK_PRINTER_OPTION_WIDGET(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_PRINTER_OPTION_WIDGET, GtkPrinterOptionWidget))
#define GTK_PRINTER_OPTION_WIDGET_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_PRINTER_OPTION_WIDGET, GtkPrinterOptionWidgetClass))
#define GTK_IS_PRINTER_OPTION_WIDGET(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_PRINTER_OPTION_WIDGET))
#define GTK_IS_PRINTER_OPTION_WIDGET_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_PRINTER_OPTION_WIDGET))
#define GTK_PRINTER_OPTION_WIDGET_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_PRINTER_OPTION_WIDGET, GtkPrinterOptionWidgetClass))

typedef struct _GtkPrinterOptionWidgetClass    GtkPrinterOptionWidgetClass;
typedef struct _GtkPrinterOptionWidgetPrivate   GtkPrinterOptionWidgetPrivate;

STLWRT_DECLARE_VTYPE_VPARENT(GtkPrinterOptionWidget, gtk_printer_option_widget, GtkHBox,
  GtkPrinterOptionWidgetPrivate *priv;
)

struct _GtkPrinterOptionWidgetClass
{
  GtkHBoxClass parent_class;

  void (*changed) (GtkPrinterOptionWidget *widget);
};


GtkWidget   *gtk_printer_option_widget_new                (GtkPrinterOption       *source);
void         gtk_printer_option_widget_set_source         (GtkPrinterOptionWidget *setting,
		 					   GtkPrinterOption       *source);
gboolean     gtk_printer_option_widget_has_external_label (GtkPrinterOptionWidget *setting);
GtkWidget   *gtk_printer_option_widget_get_external_label (GtkPrinterOptionWidget *setting);
const gchar *gtk_printer_option_widget_get_value          (GtkPrinterOptionWidget *setting);

G_END_DECLS

#endif /* __GTK_PRINTER_OPTION_WIDGET_H__ */
