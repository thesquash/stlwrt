/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtkrecentchooserdialog.h: Recent files selector dialog
 * Copyright (C) 2006 Emmanuele Bassi
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

#ifndef __GTK_RECENT_CHOOSER_DIALOG_H__
#define __GTK_RECENT_CHOOSER_DIALOG_H__

#include <gtkdialog.h>
#include <gtkrecentchooser.h>

G_BEGIN_DECLS

#define GTK_TYPE_RECENT_CHOOSER_DIALOG		  (gtk_recent_chooser_dialog_get_type ())
#define GTK_RECENT_CHOOSER_DIALOG(obj)		  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_RECENT_CHOOSER_DIALOG, GtkRecentChooserDialog))
#define GTK_IS_RECENT_CHOOSER_DIALOG(obj)	  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_RECENT_CHOOSER_DIALOG))
#define GTK_RECENT_CHOOSER_DIALOG_CLASS(klass)	  (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_RECENT_CHOOSER_DIALOG, GtkRecentChooserDialogClass))
#define GTK_IS_RECENT_CHOOSER_DIALOG_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_RECENT_CHOOSER_DIALOG))
#define GTK_RECENT_CHOOSER_DIALOG_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_RECENT_CHOOSER_DIALOG, GtkRecentChooserDialogClass))

typedef struct _GtkRecentChooserDialog        GtkRecentChooserDialogFat;
typedef struct _GtkRecentChooserDialog        GtkRecentChooserDialogThin;

typedef struct _GtkRecentChooserDialogClass   GtkRecentChooserDialogClass;

typedef struct _GtkRecentChooserDialogPrivate GtkRecentChooserDialogPrivate;



struct _GtkRecentChooserDialogPrivate
{
  GtkRecentManager *manager;
  
  GtkWidget *chooser;
};

struct _GtkRecentChooserDialog
{
  /*< private >*/
  GtkDialog parent_instance;

  GtkRecentChooserDialogPrivate * (priv);
};

struct _GtkRecentChooserDialogClass
{
  GtkDialogClass parent_class;
};


GType      _T2_gtk_recent_chooser_dialog_get_type        (void) G_GNUC_CONST;
GType      _3T_gtk_recent_chooser_dialog_get_type        (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType      gtk_recent_chooser_dialog_get_type        (void) G_GNUC_CONST;

GtkWidget *SF(gtk_recent_chooser_dialog_new)             (const gchar      *title,
					              GtkWindow        *parent,
					              const gchar      *first_button_text,
					              ...) G_GNUC_NULL_TERMINATED;
GtkWidget *SF(gtk_recent_chooser_dialog_new_for_manager) (const gchar      *title,
						      GtkWindow        *parent,
						      GtkRecentManager *manager,
						      const gchar      *first_button_text,
						      ...) G_GNUC_NULL_TERMINATED;

G_END_DECLS

#endif /* __GTK_RECENT_CHOOSER_DIALOG_H__ */
