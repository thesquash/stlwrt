/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 *
 * Copyright (C) 2010 Christian Dywan
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __GTK_COMBO_BOX_TEXT_H__
#define __GTK_COMBO_BOX_TEXT_H__

#include <stlwrt.h>


#include <gtkcombobox.h>

G_BEGIN_DECLS

#define GTK_TYPE_COMBO_BOX_TEXT                 (gtk_combo_box_text_get_type ())
#define GTK_COMBO_BOX_TEXT(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_COMBO_BOX_TEXT, GtkComboBoxText))
#define GTK_COMBO_BOX_TEXT_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_COMBO_BOX_TEXT, GtkComboBoxTextClass))
#define GTK_IS_COMBO_BOX_TEXT(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_COMBO_BOX_TEXT))
#define GTK_IS_COMBO_BOX_TEXT_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_COMBO_BOX_TEXT))
#define GTK_COMBO_BOX_TEXT_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_COMBO_BOX_TEXT, GtkComboBoxTextClass))

typedef struct _GtkComboBoxTextPrivate      GtkComboBoxTextPrivate;
typedef struct _GtkComboBoxTextClass        GtkComboBoxTextClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkComboBoxText, gtk_combo_box_text, GtkComboBox,
  GtkComboBoxTextPrivate *priv;
)

struct _GtkComboBoxTextClass
{
  GtkComboBoxClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GtkWidget*    SF(gtk_combo_box_text_new)             (void);
GtkWidget*    SF(gtk_combo_box_text_new_with_entry)  (void);
void          SF(gtk_combo_box_text_append_text)     (GtkComboBoxText     *combo_box,
                                                  const gchar         *text);
void          SF(gtk_combo_box_text_insert_text)     (GtkComboBoxText     *combo_box,
                                                  gint                 position,
                                                  const gchar         *text);
void          SF(gtk_combo_box_text_prepend_text)    (GtkComboBoxText     *combo_box,
                                                  const gchar         *text);
void          SF(gtk_combo_box_text_remove)          (GtkComboBoxText     *combo_box,
                                                  gint                 position);
gchar        *SF(gtk_combo_box_text_get_active_text) (GtkComboBoxText     *combo_box);


G_END_DECLS

#endif /* __GTK_COMBO_BOX_TEXT_H__ */
