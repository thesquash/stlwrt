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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_TEXT_TAG_TABLE_H__
#define __GTK_TEXT_TAG_TABLE_H__

#include <stlwrt.h>


#include <gtktexttag.h>

G_BEGIN_DECLS

typedef void (* GtkTextTagTableForeach) (GtkTextTag *tag, gpointer data);

#define GTK_TYPE_TEXT_TAG_TABLE            (gtk_text_tag_table_get_type ())
#define GTK_TEXT_TAG_TABLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TEXT_TAG_TABLE, GtkTextTagTable))
#define GTK_TEXT_TAG_TABLE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TEXT_TAG_TABLE, GtkTextTagTableClass))
#define GTK_IS_TEXT_TAG_TABLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TEXT_TAG_TABLE))
#define GTK_IS_TEXT_TAG_TABLE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TEXT_TAG_TABLE))
#define GTK_TEXT_TAG_TABLE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TEXT_TAG_TABLE, GtkTextTagTableClass))

typedef struct _GtkTextTagTableClass GtkTextTagTableClass;

STLWRT_DECLARE_VTYPE_FPARENT(GtkTextTagTable, gtk_text_tag_table, GObject,
  GHashTable * (hash);
  GSList * (anonymous);
  gint  (anon_count);

  GSList * (buffers);
)

struct _GtkTextTagTableClass
{
  GObjectClass parent_class;

  void (* tag_changed) (GtkTextTagTable *table, GtkTextTag *tag, gboolean size_changed);
  void (* tag_added) (GtkTextTagTable *table, GtkTextTag *tag);
  void (* tag_removed) (GtkTextTagTable *table, GtkTextTag *tag);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GtkTextTagTable *SF(gtk_text_tag_table_new)      (void);
void             SF(gtk_text_tag_table_add)      (GtkTextTagTable        *table,
                                              GtkTextTag             *tag);
void             SF(gtk_text_tag_table_remove)   (GtkTextTagTable        *table,
                                              GtkTextTag             *tag);
GtkTextTag      *SF(gtk_text_tag_table_lookup)   (GtkTextTagTable        *table,
                                              const gchar            *name);
void             SF(gtk_text_tag_table_foreach)  (GtkTextTagTable        *table,
                                              GtkTextTagTableForeach  func,
                                              gpointer                data);
gint             SF(gtk_text_tag_table_get_size) (GtkTextTagTable        *table);


/* INTERNAL private stuff - not even exported from the library on
 * many platforms
 */
void SF(_gtk_text_tag_table_add_buffer)    (GtkTextTagTable *table,
                                        gpointer         buffer);
void SF(_gtk_text_tag_table_remove_buffer) (GtkTextTagTable *table,
                                        gpointer         buffer);

G_END_DECLS

#endif
