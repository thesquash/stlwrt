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

#ifndef __GTK_EDITABLE_H__
#define __GTK_EDITABLE_H__

#include <gtkwidget.h>


G_BEGIN_DECLS

#define GTK_TYPE_EDITABLE             (SF(gtk_editable_get_type) ())
#define GTK_EDITABLE(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_EDITABLE, GtkEditable))
#define GTK_EDITABLE_CLASS(vtable)    (G_TYPE_CHECK_CLASS_CAST ((vtable), GTK_TYPE_EDITABLE, GtkEditableClass))
#define GTK_IS_EDITABLE(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_EDITABLE))
#define GTK_IS_EDITABLE_CLASS(vtable) (G_TYPE_CHECK_CLASS_TYPE ((vtable), GTK_TYPE_EDITABLE))
#define GTK_EDITABLE_GET_CLASS(inst)  (G_TYPE_INSTANCE_GET_INTERFACE ((inst), GTK_TYPE_EDITABLE, GtkEditableClass))

typedef struct _GtkEditableClass  GtkEditableClass;

struct _GtkEditableClass
{
  GTypeInterface		   base_iface;

  /* signals */
  void (* insert_text)              (GtkEditable    *editable,
				     const gchar    *text,
				     gint            length,
				     gint           *position);
  void (* delete_text)              (GtkEditable    *editable,
				     gint            start_pos,
				     gint            end_pos);
  void (* changed)                  (GtkEditable    *editable);

  /* vtable */
  void (* do_insert_text)           (GtkEditable    *editable,
				     const gchar    *text,
				     gint            length,
				     gint           *position);
  void (* do_delete_text)           (GtkEditable    *editable,
				     gint            start_pos,
				     gint            end_pos);

  gchar* (* get_chars)              (GtkEditable    *editable,
				     gint            start_pos,
				     gint            end_pos);
  void (* set_selection_bounds)     (GtkEditable    *editable,
				     gint            start_pos,
				     gint            end_pos);
  gboolean (* get_selection_bounds) (GtkEditable    *editable,
				     gint           *start_pos,
				     gint           *end_pos);
  void (* set_position)             (GtkEditable    *editable,
				     gint            position);
  gint (* get_position)             (GtkEditable    *editable);
};


STLWRT_DECLARE_GET_FTYPE_FUNCTIONS(gtk_editable)
void     SF(gtk_editable_select_region)        (GtkEditable *editable,
					    gint         start_pos,
					    gint         end_pos);
gboolean SF(gtk_editable_get_selection_bounds) (GtkEditable *editable,
					    gint        *start_pos,
					    gint        *end_pos);
void     SF(gtk_editable_insert_text)          (GtkEditable *editable,
					    const gchar *new_text,
					    gint         new_text_length,
					    gint        *position);
void     SF(gtk_editable_delete_text)          (GtkEditable *editable,
					    gint         start_pos,
					    gint         end_pos);
gchar*   SF(gtk_editable_get_chars)            (GtkEditable *editable,
					    gint         start_pos,
					    gint         end_pos);
void     SF(gtk_editable_cut_clipboard)        (GtkEditable *editable);
void     SF(gtk_editable_copy_clipboard)       (GtkEditable *editable);
void     SF(gtk_editable_paste_clipboard)      (GtkEditable *editable);
void     SF(gtk_editable_delete_selection)     (GtkEditable *editable);
void     SF(gtk_editable_set_position)         (GtkEditable *editable,
					    gint         position);
gint     SF(gtk_editable_get_position)         (GtkEditable *editable);
void     SF(gtk_editable_set_editable)         (GtkEditable *editable,
					    gboolean     is_editable);
gboolean SF(gtk_editable_get_editable)         (GtkEditable *editable);

G_END_DECLS

#endif /* __GTK_EDITABLE_H__ */
