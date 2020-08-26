/*
 * STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1998, 1999 Red Hat, Inc.
 * All rights reserved.
 *
 * This Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the Gnome Library; see the file COPYING.LIB.  If not,
 * write to the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

/*
 * Author: James Henstridge <james@daa.com.au>
 */

#ifndef __GTK_ACTION_H__
#define __GTK_ACTION_H__


#include <gtkwidget.h>

G_BEGIN_DECLS

#define GTK_TYPE_ACTION            (gtk_action_get_type ())
#define GTK_ACTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_ACTION, GtkAction))
#define GTK_ACTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_ACTION, GtkActionClass))
#define GTK_IS_ACTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_ACTION))
#define GTK_IS_ACTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_ACTION))
#define GTK_ACTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_ACTION, GtkActionClass))

typedef struct _GtkAction      GtkAction;

typedef struct _GtkActionClass GtkActionClass;
typedef struct _GtkActionPrivate GtkActionPrivate;


struct _GtkActionPrivate 
{
  const gchar *name; /* interned */
  gchar *label;
  gchar *short_label;
  gchar *tooltip;
  gchar *stock_id; /* stock icon */
  gchar *icon_name; /* themed icon */
  GIcon *gicon;

  guint sensitive          : 1;
  guint visible            : 1;
  guint label_set          : 1; /* these two used so we can set label */
  guint short_label_set    : 1; /* based on stock id */
  guint visible_horizontal : 1;
  guint visible_vertical   : 1;
  guint is_important       : 1;
  guint hide_if_empty      : 1;
  guint visible_overflown  : 1;
  guint always_show_image  : 1;
  guint recursion_guard    : 1;
  guint activate_blocked   : 1;

  /* accelerator */
  guint          accel_count;
  GtkAccelGroup *accel_group;
  GClosure      *accel_closure;
  GQuark         accel_quark;

  GtkActionGroup *action_group;

  /* list of proxy widgets */
  GSList *proxies;
};

struct _GtkAction
{
  GObject object;

  /*< private >*/

  GtkActionPrivate * (private_data);
};

struct _GtkActionClass
{
  GObjectClass parent_class;

  /* activation signal */
  void       (* activate)           (GtkAction    *action);

  GType      menu_item_type;
  GType      toolbar_item_type;

  /* widget creation routines (not signals) */
  GtkWidget *(* create_menu_item)   (GtkAction *action);
  GtkWidget *(* create_tool_item)   (GtkAction *action);
  void       (* connect_proxy)      (GtkAction *action,
				     GtkWidget *proxy);
  void       (* disconnect_proxy)   (GtkAction *action,
				     GtkWidget *proxy);

  GtkWidget *(* create_menu)        (GtkAction *action);

  /* Padding for future expansion */
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GType        gtk_action_get_type               (void) G_GNUC_CONST;
GtkAction   *SF(gtk_action_new)                    (const gchar *name,
						const gchar *label,
						const gchar *tooltip,
						const gchar *stock_id);
const gchar* SF(gtk_action_get_name)               (GtkAction     *action);
gboolean     SF(gtk_action_is_sensitive)           (GtkAction     *action);
gboolean     SF(gtk_action_get_sensitive)          (GtkAction     *action);
void         SF(gtk_action_set_sensitive)          (GtkAction     *action,
						gboolean       sensitive);
gboolean     SF(gtk_action_is_visible)             (GtkAction     *action);
gboolean     SF(gtk_action_get_visible)            (GtkAction     *action);
void         SF(gtk_action_set_visible)            (GtkAction     *action,
						gboolean       visible);
void         SF(gtk_action_activate)               (GtkAction     *action);
GtkWidget *  SF(gtk_action_create_icon)            (GtkAction     *action,
						GtkIconSize    icon_size);
GtkWidget *  SF(gtk_action_create_menu_item)       (GtkAction     *action);
GtkWidget *  SF(gtk_action_create_tool_item)       (GtkAction     *action);
GtkWidget *  SF(gtk_action_create_menu)            (GtkAction     *action);
GSList *     SF(gtk_action_get_proxies)            (GtkAction     *action);
void         SF(gtk_action_connect_accelerator)    (GtkAction     *action);
void         SF(gtk_action_disconnect_accelerator) (GtkAction     *action);
const gchar *SF(gtk_action_get_accel_path)         (GtkAction     *action);
GClosure    *SF(gtk_action_get_accel_closure)      (GtkAction     *action);

#ifndef GTK_DISABLE_DEPRECATED
GtkAction   *SF(gtk_widget_get_action)             (GtkWidget     *widget);
void         SF(gtk_action_connect_proxy)          (GtkAction     *action,
						GtkWidget     *proxy);
void         SF(gtk_action_disconnect_proxy)       (GtkAction     *action,
						GtkWidget     *proxy);
void         SF(gtk_action_block_activate_from)    (GtkAction     *action,
						GtkWidget     *proxy);
void         SF(gtk_action_unblock_activate_from)  (GtkAction     *action,
						GtkWidget     *proxy);
#endif /* GTK_DISABLE_DEPRECATED */
void         SF(gtk_action_block_activate)         (GtkAction     *action);
void         SF(gtk_action_unblock_activate)       (GtkAction     *action);


void         SF(_gtk_action_add_to_proxy_list)     (GtkAction     *action,
						GtkWidget     *proxy);
void         SF(_gtk_action_remove_from_proxy_list)(GtkAction     *action,
						GtkWidget     *proxy);

/* protected ... for use by child actions */
void         SF(_gtk_action_emit_activate)         (GtkAction     *action);

/* protected ... for use by action groups */
void         SF(gtk_action_set_accel_path)         (GtkAction     *action,
						const gchar   *accel_path);
void         SF(gtk_action_set_accel_group)        (GtkAction     *action,
						GtkAccelGroup *accel_group);
void         SF(_gtk_action_sync_menu_visible)     (GtkAction     *action,
						GtkWidget     *proxy,
						gboolean       empty);

void                  SF(gtk_action_set_label)              (GtkAction   *action,
                                                         const gchar *label);
const gchar *         SF(gtk_action_get_label)              (GtkAction   *action);
void                  SF(gtk_action_set_short_label)        (GtkAction   *action,
                                                         const gchar *short_label);
const gchar *         SF(gtk_action_get_short_label)        (GtkAction   *action);
void                  SF(gtk_action_set_tooltip)            (GtkAction   *action,
                                                         const gchar *tooltip);
const gchar *         SF(gtk_action_get_tooltip)            (GtkAction   *action);
void                  SF(gtk_action_set_stock_id)           (GtkAction   *action,
                                                         const gchar *stock_id);
const gchar *         SF(gtk_action_get_stock_id)           (GtkAction   *action);
void                  SF(gtk_action_set_gicon)              (GtkAction   *action,
                                                         GIcon       *icon);
GIcon                *SF(gtk_action_get_gicon)              (GtkAction   *action);
void                  SF(gtk_action_set_icon_name)          (GtkAction   *action,
                                                         const gchar *icon_name);
const gchar *         SF(gtk_action_get_icon_name)          (GtkAction   *action);
void                  SF(gtk_action_set_visible_horizontal) (GtkAction   *action,
                                                         gboolean     visible_horizontal);
gboolean              SF(gtk_action_get_visible_horizontal) (GtkAction   *action);
void                  SF(gtk_action_set_visible_vertical)   (GtkAction   *action,
                                                         gboolean     visible_vertical);
gboolean              SF(gtk_action_get_visible_vertical)   (GtkAction   *action);
void                  SF(gtk_action_set_is_important)       (GtkAction   *action,
                                                         gboolean     is_important);
gboolean              SF(gtk_action_get_is_important)       (GtkAction   *action);
void                  SF(gtk_action_set_always_show_image)  (GtkAction   *action,
                                                         gboolean     always_show);
gboolean              SF(gtk_action_get_always_show_image)  (GtkAction   *action);


G_END_DECLS

#endif  /* __GTK_ACTION_H__ */
