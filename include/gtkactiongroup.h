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

#ifndef __GTK_ACTION_GROUP_H__
#define __GTK_ACTION_GROUP_H__

#include <stlwrt.h>



#include <gtkaction.h>

#include <gtktypeutils.h> /* for GtkTranslateFunc */

G_BEGIN_DECLS

#define GTK_TYPE_ACTION_GROUP              (gtk_action_group_get_type ())
#define GTK_ACTION_GROUP(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_ACTION_GROUP, GtkActionGroup))
#define GTK_ACTION_GROUP_CLASS(vtable)     (G_TYPE_CHECK_CLASS_CAST ((vtable), GTK_TYPE_ACTION_GROUP, GtkActionGroupClass))
#define GTK_IS_ACTION_GROUP(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_ACTION_GROUP))
#define GTK_IS_ACTION_GROUP_CLASS(vtable)  (G_TYPE_CHECK_CLASS_TYPE ((vtable), GTK_TYPE_ACTION_GROUP))
#define GTK_ACTION_GROUP_GET_CLASS(inst)   (G_TYPE_INSTANCE_GET_CLASS ((inst), GTK_TYPE_ACTION_GROUP, GtkActionGroupClass))

typedef struct _GtkActionGroupPrivate GtkActionGroupPrivate;
typedef struct _GtkActionGroupClass   GtkActionGroupClass;
typedef struct _GtkActionEntry        GtkActionEntry;
typedef struct _GtkToggleActionEntry  GtkToggleActionEntry;
typedef struct _GtkRadioActionEntry   GtkRadioActionEntry;


STLWRT_DECLARE_FTYPE_FPARENT(GtkActionGroup, gtk_action_group, GObject,
  /*< private >*/

  GtkActionGroupPrivate * (private_data);
)

struct _GtkActionGroupClass
{
  GObjectClass parent_class;

  GtkAction *(* get_action) (GtkActionGroup *action_group,
                             const gchar    *action_name);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

struct _GtkActionEntry 
{
  const gchar     *name;
  const gchar     *stock_id;
  const gchar     *label;
  const gchar     *accelerator;
  const gchar     *tooltip;
  GCallback  callback;
};

struct _GtkToggleActionEntry 
{
  const gchar     *name;
  const gchar     *stock_id;
  const gchar     *label;
  const gchar     *accelerator;
  const gchar     *tooltip;
  GCallback  callback;
  gboolean   is_active;
};

struct _GtkRadioActionEntry 
{
  const gchar *name;
  const gchar *stock_id;
  const gchar *label;
  const gchar *accelerator;
  const gchar *tooltip;
  gint   value; 
};


GtkActionGroup *SF(gtk_action_group_new)                     (const gchar                *name);
const gchar *SF(gtk_action_group_get_name)          (GtkActionGroup             *action_group);
gboolean        SF(gtk_action_group_get_sensitive)           (GtkActionGroup             *action_group);
void            SF(gtk_action_group_set_sensitive)           (GtkActionGroup             *action_group,
							  gboolean                    sensitive);
gboolean        SF(gtk_action_group_get_visible)             (GtkActionGroup             *action_group);
void            SF(gtk_action_group_set_visible)             (GtkActionGroup             *action_group,
							  gboolean                    visible);
GtkAction      *SF(gtk_action_group_get_action)              (GtkActionGroup             *action_group,
							  const gchar                *action_name);
GList          *SF(gtk_action_group_list_actions)            (GtkActionGroup             *action_group);
void            SF(gtk_action_group_add_action)              (GtkActionGroup             *action_group,
							  GtkAction                  *action);
void            SF(gtk_action_group_add_action_with_accel)   (GtkActionGroup             *action_group,
							  GtkAction                  *action,
							  const gchar                *accelerator);
void            SF(gtk_action_group_remove_action)           (GtkActionGroup             *action_group,
							  GtkAction                  *action);
void            SF(gtk_action_group_add_actions)             (GtkActionGroup             *action_group,
							  const GtkActionEntry       *entries,
							  guint                       n_entries,
							  gpointer                    user_data);
void            SF(gtk_action_group_add_toggle_actions)      (GtkActionGroup             *action_group,
							  const GtkToggleActionEntry *entries,
							  guint                       n_entries,
							  gpointer                    user_data);
void            SF(gtk_action_group_add_radio_actions)       (GtkActionGroup             *action_group,
							  const GtkRadioActionEntry  *entries,
							  guint                       n_entries,
							  gint                        value,
							  GCallback                   on_change,
							  gpointer                    user_data);
void            SF(gtk_action_group_add_actions_full)        (GtkActionGroup             *action_group,
							  const GtkActionEntry       *entries,
							  guint                       n_entries,
							  gpointer                    user_data,
							  GDestroyNotify              destroy);
void            SF(gtk_action_group_add_toggle_actions_full) (GtkActionGroup             *action_group,
							  const GtkToggleActionEntry *entries,
							  guint                       n_entries,
							  gpointer                    user_data,
							  GDestroyNotify              destroy);
void            SF(gtk_action_group_add_radio_actions_full)  (GtkActionGroup             *action_group,
							  const GtkRadioActionEntry  *entries,
							  guint                       n_entries,
							  gint                        value,
							  GCallback                   on_change,
							  gpointer                    user_data,
							  GDestroyNotify              destroy);
void            SF(gtk_action_group_set_translate_func)      (GtkActionGroup             *action_group,
							  GtkTranslateFunc            func,
							  gpointer                    data,
							  GDestroyNotify              notify);
void            SF(gtk_action_group_set_translation_domain)  (GtkActionGroup             *action_group,
							  const gchar                *domain);
const gchar *SF(gtk_action_group_translate_string)  (GtkActionGroup             *action_group,
  	                                                  const gchar                *string);

/* Protected for use by GtkAction */
void SF(_gtk_action_group_emit_connect_proxy)    (GtkActionGroup *action_group,
                                              GtkAction      *action,
                                              GtkWidget      *proxy);
void SF(_gtk_action_group_emit_disconnect_proxy) (GtkActionGroup *action_group,
                                              GtkAction      *action,
                                              GtkWidget      *proxy);
void SF(_gtk_action_group_emit_pre_activate)     (GtkActionGroup *action_group,
                                              GtkAction      *action);
void SF(_gtk_action_group_emit_post_activate)    (GtkActionGroup *action_group,
                                              GtkAction      *action);

G_END_DECLS

#endif  /* __GTK_ACTION_GROUP_H__ */
