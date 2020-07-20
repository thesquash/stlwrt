
/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Recent chooser action for GtkUIManager
 *
 * Copyright (C) 2007, Emmanuele Bassi
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

#ifndef __GTK_RECENT_ACTION_H__
#define __GTK_RECENT_ACTION_H__

#include <gtkaction.h>
#include <gtkrecentmanager.h>

G_BEGIN_DECLS

#define GTK_TYPE_RECENT_ACTION                  (gtk_recent_action_get_type ())
#define GTK_RECENT_ACTION(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_RECENT_ACTION, GtkRecentAction))
#define GTK_IS_RECENT_ACTION(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_RECENT_ACTION))
#define GTK_RECENT_ACTION_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_RECENT_ACTION, GtkRecentActionClass))
#define GTK_IS_RECENT_ACTION_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_RECENT_ACTION))
#define GTK_RECENT_ACTION_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_RECENT_ACTION, GtkRecentActionClass))

typedef struct _GtkRecentAction         GtkRecentActionFat;
typedef struct _GtkRecentAction         GtkRecentActionThin;
typedef struct _GtkRecentActionPrivate  GtkRecentActionPrivate;
typedef struct _GtkRecentActionClass    GtkRecentActionClassFat;
typedef struct _GtkRecentActionClass    GtkRecentActionClassThin;

struct _GtkRecentAction
{
  GtkAction parent_instance;

  /*< private >*/
  GtkRecentActionPrivate * (priv);
};

struct _GtkRecentActionClass
{
  GtkActionClass parent_class;
};

GType      _T2_gtk_recent_action_get_type         (void) G_GNUC_CONST;
GType      _3T_gtk_recent_action_get_type         (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType      gtk_recent_action_get_type         (void) G_GNUC_CONST;
GtkAction *__gtk_recent_action_new              (const gchar      *name,
                                               const gchar      *label,
                                               const gchar      *tooltip,
                                               const gchar      *stock_id);
GtkAction *__gtk_recent_action_new_for_manager  (const gchar      *name,
                                               const gchar      *label,
                                               const gchar      *tooltip,
                                               const gchar      *stock_id,
                                               GtkRecentManager *manager);
gboolean   __gtk_recent_action_get_show_numbers (GtkRecentAction  *action);
void       __gtk_recent_action_set_show_numbers (GtkRecentAction  *action,
                                               gboolean          show_numbers);

G_END_DECLS

#endif /* __GTK_RECENT_ACTION_H__ */
