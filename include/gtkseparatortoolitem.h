/* gtktoggletoolbutton.h
 *
 * Copyright (C) 2002 Anders Carlsson <andersca@gnome.org>
 * Copyright (C) 2002 James Henstridge <james@daa.com.au>
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

#ifndef __GTK_SEPARATOR_TOOL_ITEM_H__
#define __GTK_SEPARATOR_TOOL_ITEM_H__

#include <stlwrt.h>


#include <gtktoolitem.h>

G_BEGIN_DECLS

#define GTK_TYPE_SEPARATOR_TOOL_ITEM            (gtk_separator_tool_item_get_type ())
#define GTK_SEPARATOR_TOOL_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SEPARATOR_TOOL_ITEM, GtkSeparatorToolItem))
#define GTK_SEPARATOR_TOOL_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SEPARATOR_TOOL_ITEM, GtkSeparatorToolItemClass))
#define GTK_IS_SEPARATOR_TOOL_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SEPARATOR_TOOL_ITEM))
#define GTK_IS_SEPARATOR_TOOL_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SEPARATOR_TOOL_ITEM))
#define GTK_SEPARATOR_TOOL_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_SEPARATOR_TOOL_ITEM, GtkSeparatorToolItemClass))

typedef struct _GtkSeparatorToolItemClass   GtkSeparatorToolItemClass;
typedef struct _GtkSeparatorToolItemPrivate GtkSeparatorToolItemPrivate;

struct _GtkSeparatorToolItemPrivate
{
  guint draw : 1;
};

STLWRT_DECLARE_FTYPE_VPARENT(GtkSeparatorToolItem, gtk_separator_tool_item, GtkToolItem,
  /*< private >*/
  GtkSeparatorToolItemPrivate * (priv);
)

struct _GtkSeparatorToolItemClass
{
  GtkToolItemClass parent_class;

  /* Padding for future expansion */
  void (* _gtk_reserved1) (void);
  void (* _gtk_reserved2) (void);
  void (* _gtk_reserved3) (void);
  void (* _gtk_reserved4) (void);
};


GtkToolItem *SF(gtk_separator_tool_item_new)      (void);

gboolean     SF(gtk_separator_tool_item_get_draw) (GtkSeparatorToolItem *item);
void         SF(gtk_separator_tool_item_set_draw) (GtkSeparatorToolItem *item,
					       gboolean              draw);

G_END_DECLS

#endif /* __GTK_SEPARATOR_TOOL_ITEM_H__ */
