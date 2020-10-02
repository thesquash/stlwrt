/* GTK+: gtkfilechooserbutton.h
 *
 * Copyright (c) 2004 James M. Cape <jcape@ignore-your.tv>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_FILE_CHOOSER_BUTTON_H__
#define __GTK_FILE_CHOOSER_BUTTON_H__

#include <stlwrt.h>


#include <gtkhbox.h>

#include <gtkfilechooser.h>

G_BEGIN_DECLS

#define GTK_TYPE_FILE_CHOOSER_BUTTON            (gtk_file_chooser_button_get_type ())
#define GTK_FILE_CHOOSER_BUTTON(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_FILE_CHOOSER_BUTTON, GtkFileChooserButton))
#define GTK_FILE_CHOOSER_BUTTON_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_FILE_CHOOSER_BUTTON, GtkFileChooserButtonClass))
#define GTK_IS_FILE_CHOOSER_BUTTON(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_FILE_CHOOSER_BUTTON))
#define GTK_IS_FILE_CHOOSER_BUTTON_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_FILE_CHOOSER_BUTTON))
#define GTK_FILE_CHOOSER_BUTTON_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_FILE_CHOOSER_BUTTON, GtkFileChooserButtonClass))

typedef struct _GtkFileChooserButtonPrivate GtkFileChooserButtonPrivate;
typedef struct _GtkFileChooserButtonClass   GtkFileChooserButtonClass;


struct _GtkFileChooserButtonPrivate
{
  GtkWidget *dialog;
  GtkWidget *button;
  GtkWidget *image;
  GtkWidget *label;
  GtkWidget *combo_box;
  GtkCellRenderer *icon_cell;
  GtkCellRenderer *name_cell;

  GtkTreeModel *model;
  GtkTreeModel *filter_model;

  GtkFileSystem *fs;
  GFile *selection_while_inactive;
  GFile *current_folder_while_inactive;

  gulong combo_box_changed_id;
  gulong fs_volumes_changed_id;
  gulong fs_bookmarks_changed_id;

  GCancellable *dnd_select_folder_cancellable;
  GCancellable *update_button_cancellable;
  GSList *change_icon_theme_cancellables;

  gint icon_size;

  guint8 n_special;
  guint8 n_volumes;
  guint8 n_shortcuts;
  guint8 n_bookmarks;
  guint  has_bookmark_separator       : 1;
  guint  has_current_folder_separator : 1;
  guint  has_current_folder           : 1;
  guint  has_other_separator          : 1;

  /* Used for hiding/showing the dialog when the button is hidden */
  guint  active                       : 1;

  guint  focus_on_click               : 1;

  /* Whether the next async callback from GIO should emit the "selection-changed" signal */
  guint  is_changing_selection        : 1;
};

STLWRT_DECLARE_FTYPE_VPARENT(GtkFileChooserButton, gtk_file_chooser_button, GtkHBox,
  /*< private >*/
  GtkHBox parent;

  GtkFileChooserButtonPrivate * (priv);
)

struct _GtkFileChooserButtonClass
{
  /*< private >*/
  GtkHBoxClass parent_class;

  void (* file_set) (GtkFileChooserButton *fc);

  void (*SF(gtk_reserved1));
  void (*SF(gtk_reserved2));
  void (*SF(gtk_reserved3));
  void (*SF(gtk_reserved4));
  void (*SF(gtk_reserved5));
  void (*SF(gtk_reserved6));
  void (*SF(gtk_reserved7));
};


GtkWidget *           SF(gtk_file_chooser_button_new)              (const gchar          *title,
								GtkFileChooserAction  action);

#ifndef GTK_DISABLE_DEPRECATED
GtkWidget *           SF(gtk_file_chooser_button_new_with_backend) (const gchar          *title,
								GtkFileChooserAction  action,
								const gchar          *backend);
#endif /* GTK_DISABLE_DEPRECATED */

GtkWidget *           SF(gtk_file_chooser_button_new_with_dialog)  (GtkWidget            *dialog);
const gchar *         SF(gtk_file_chooser_button_get_title)        (GtkFileChooserButton *button);
void                  SF(gtk_file_chooser_button_set_title)        (GtkFileChooserButton *button,
								const gchar          *title);
gint                  SF(gtk_file_chooser_button_get_width_chars)  (GtkFileChooserButton *button);
void                  SF(gtk_file_chooser_button_set_width_chars)  (GtkFileChooserButton *button,
								gint                  n_chars);
gboolean              SF(gtk_file_chooser_button_get_focus_on_click) (GtkFileChooserButton *button);
void                  SF(gtk_file_chooser_button_set_focus_on_click) (GtkFileChooserButton *button,
                                                                  gboolean              focus_on_click);

G_END_DECLS

#endif /* !__GTK_FILE_CHOOSER_BUTTON_H__ */
