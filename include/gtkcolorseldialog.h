
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


typedef struct _GtkColorSelectionDialog       GtkColorSelectionDialogFat;
typedef struct _GtkColorSelectionDialog       GtkColorSelectionDialogThin;

typedef struct _GtkColorSelectionDialogClass  GtkColorSelectionDialogClass;


/********************************************************************/
struct _GtkColorSelectionDialogProps
{


  GtkWidget * (colorsel);
  GtkWidget * (ok_button);
  GtkWidget * (cancel_button);
  GtkWidget * (help_button);
};

struct _GtkColorSelectionDialogFat
{
  GtkDialogFat   parent_instance;

  struct _GtkColorSelectionDialogProps instance_properties;
};

struct _GtkColorSelectionDialogThin
{
  GtkDialogThin  parent_instance;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkColorSelectionDialogFat   fat_instance;
  struct _GtkColorSelectionDialogThin  thin_instance;
}   GtkColorSelectionDialog;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkColorSelectionDialogFat GtkColorSelectionDialog;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkColorSelectionDialogThin GtkColorSelectionDialog;
#endif
/********************************************************************/



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
GType      _T2_gtk_color_selection_dialog_get_type            (void) G_GNUC_CONST;
GType      _3T_gtk_color_selection_dialog_get_type            (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType      gtk_color_selection_dialog_get_type            (void) G_GNUC_CONST;
GtkWidget* SF(gtk_color_selection_dialog_new)                 (const gchar *title);
GtkWidget* SF(gtk_color_selection_dialog_get_color_selection) (GtkColorSelectionDialog *colorsel);


G_END_DECLS

#endif /* __GTK_COLOR_SELECTION_DIALOG_H__ */
