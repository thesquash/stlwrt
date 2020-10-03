/* GtkToolPalette -- A tool palette with categories and DnD support
 * Copyright (C) 2008  Openismus GmbH
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
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * Authors:
 *      Mathias Hasselmann
 */

#ifndef __GTK_TOOL_ITEM_GROUP_H__
#define __GTK_TOOL_ITEM_GROUP_H__

#include <stlwrt.h>
#include <stlwrt-typedefs.h>


#include <gtkcontainer.h>

#include <gtktoolitem.h>

G_BEGIN_DECLS

#define GTK_TYPE_TOOL_ITEM_GROUP           (gtk_tool_item_group_get_type ())
#define GTK_TOOL_ITEM_GROUP(obj)           (G_TYPE_CHECK_INSTANCE_CAST (obj, GTK_TYPE_TOOL_ITEM_GROUP, GtkToolItemGroup))
#define GTK_TOOL_ITEM_GROUP_CLASS(cls)     (G_TYPE_CHECK_CLASS_CAST (cls, GTK_TYPE_TOOL_ITEM_GROUP, GtkToolItemGroupClass))
#define GTK_IS_TOOL_ITEM_GROUP(obj)        (G_TYPE_CHECK_INSTANCE_TYPE (obj, GTK_TYPE_TOOL_ITEM_GROUP))
#define GTK_IS_TOOL_ITEM_GROUP_CLASS(obj)  (G_TYPE_CHECK_CLASS_TYPE (obj, GTK_TYPE_TOOL_ITEM_GROUP))
#define GTK_TOOL_ITEM_GROUP_GET_CLASS(obj) (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TOOL_ITEM_GROUP, GtkToolItemGroupClass))

typedef struct _GtkToolItemGroupClass   GtkToolItemGroupClass;
typedef struct _GtkToolItemGroupPrivate GtkToolItemGroupPrivate;

/**
 * GtkToolItemGroup:
 *
 * This should not be accessed directly. Use the accessor functions below.
 */

struct _GtkToolItemGroupPrivate
{
  GtkWidget         *header;
  GtkWidget         *label_widget;

  GList             *children;

  gboolean           animation;
  gint64             animation_start;
  GSource           *animation_timeout;
  GtkExpanderStyle   expander_style;
  gint               expander_size;
  gint               header_spacing;
  PangoEllipsizeMode ellipsize;

  gulong             focus_set_id;
  GtkWidget         *toplevel;

  GtkSettings       *settings;
  gulong             settings_connection;

  guint              collapsed : 1;
};

STLWRT_DECLARE_FTYPE_VPARENT(GtkToolItemGroup, gtk_tool_item_group, GtkContainer,
  GtkToolItemGroupPrivate *priv;
)

struct _GtkToolItemGroupClass
{
  GtkContainerClass parent_class;
};


GtkWidget*            SF(gtk_tool_item_group_new)               (const gchar        *label);

void                  SF(gtk_tool_item_group_set_label)         (GtkToolItemGroup   *group,
                                                             const gchar        *label);
void                  SF(gtk_tool_item_group_set_label_widget)  (GtkToolItemGroup   *group,
                                                             GtkWidget          *label_widget);
void                  SF(gtk_tool_item_group_set_collapsed)      (GtkToolItemGroup  *group,
                                                             gboolean            collapsed);
void                  SF(gtk_tool_item_group_set_ellipsize)     (GtkToolItemGroup   *group,
                                                             PangoEllipsizeMode  ellipsize);
void                  SF(gtk_tool_item_group_set_header_relief) (GtkToolItemGroup   *group,
                                                             GtkReliefStyle      style);

const gchar *         SF(gtk_tool_item_group_get_label)         (GtkToolItemGroup   *group);
GtkWidget            *SF(gtk_tool_item_group_get_label_widget)  (GtkToolItemGroup   *group);
gboolean              SF(gtk_tool_item_group_get_collapsed)     (GtkToolItemGroup   *group);
PangoEllipsizeMode    SF(gtk_tool_item_group_get_ellipsize)     (GtkToolItemGroup   *group);
GtkReliefStyle        SF(gtk_tool_item_group_get_header_relief) (GtkToolItemGroup   *group);

void                  SF(gtk_tool_item_group_insert)            (GtkToolItemGroup   *group,
                                                             GtkToolItem        *item,
                                                             gint                position);
void                  SF(gtk_tool_item_group_set_item_position) (GtkToolItemGroup   *group,
                                                             GtkToolItem        *item,
                                                             gint                position);
gint                  SF(gtk_tool_item_group_get_item_position) (GtkToolItemGroup   *group,
                                                             GtkToolItem        *item);

guint                 SF(gtk_tool_item_group_get_n_items)       (GtkToolItemGroup   *group);
GtkToolItem*          SF(gtk_tool_item_group_get_nth_item)      (GtkToolItemGroup   *group,
                                                             guint               index);
GtkToolItem*          SF(gtk_tool_item_group_get_drop_item)     (GtkToolItemGroup   *group,
                                                             gint                x,
                                                             gint                y);

G_END_DECLS

#endif /* __GTK_TOOL_ITEM_GROUP_H__ */
