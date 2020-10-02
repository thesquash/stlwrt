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

#ifndef __GTK_COLOR_SELECTION_DIALOG_H__
#define __GTK_COLOR_SELECTION_DIALOG_H__

#include <stlwrt.h>


#include <gtkdialog.h>

#include <gtkcolorsel.h>

#include <gtkvbox.h>

G_BEGIN_DECLS

#define GTK_TYPE_COLOR_SELECTION_DIALOG            (gtk_color_selection_dialog_get_type ())
#define GTK_COLOR_SELECTION_DIALOG(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_COLOR_SELECTION_DIALOG, GtkColorSelectionDialog))
#define GTK_COLOR_SELECTION_DIALOG_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_COLOR_SELECTION_DIALOG, GtkColorSelectionDialogClass))
#define GTK_IS_COLOR_SELECTION_DIALOG(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_COLOR_SELECTION_DIALOG))
#define GTK_IS_COLOR_SELECTION_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_COLOR_SELECTION_DIALOG))
#define GTK_COLOR_SELECTION_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_COLOR_SELECTION_DIALOG, GtkColorSelectionDialogClass))

typedef struct _GtkColorSelectionDialogClass  GtkColorSelectionDialogClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkColorSelectionDialog, gtk_color_selection_dialog, GtkDialog,
  GtkWidget * (colorsel);
  GtkWidget * (ok_button);
  GtkWidget * (cancel_button);
  GtkWidget * (help_button);
)

struct _GtkColorSelectionDialogClass
{
  GtkDialogClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


/* ColorSelectionDialog */
GtkWidget* SF(gtk_color_selection_dialog_new)                 (const gchar *title);
GtkWidget* SF(gtk_color_selection_dialog_get_color_selection) (GtkColorSelectionDialog *colorsel);


G_END_DECLS

#endif /* __GTK_COLOR_SELECTION_DIALOG_H__ */
