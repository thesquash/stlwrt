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

#include <stlwrt.h>
#include <stlwrt-typedefs.h>


#include <gtkaction.h>

#include <gtkrecentmanager.h>

G_BEGIN_DECLS

#define GTK_TYPE_RECENT_ACTION                  (gtk_recent_action_get_type ())
#define GTK_RECENT_ACTION(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_RECENT_ACTION, GtkRecentAction))
#define GTK_IS_RECENT_ACTION(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_RECENT_ACTION))
#define GTK_RECENT_ACTION_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_RECENT_ACTION, GtkRecentActionClass))
#define GTK_IS_RECENT_ACTION_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_RECENT_ACTION))
#define GTK_RECENT_ACTION_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_RECENT_ACTION, GtkRecentActionClass))

typedef struct _GtkRecentActionPrivate  GtkRecentActionPrivate;
typedef struct _GtkRecentActionClass    GtkRecentActionClass;


struct _GtkRecentActionPrivate
{
  GtkRecentManager *manager;

  guint show_numbers   : 1;

  /* RecentChooser properties */
  guint show_private   : 1;
  guint show_not_found : 1;
  guint show_tips      : 1;
  guint show_icons     : 1;
  guint local_only     : 1;

  gint limit;

  GtkRecentSortType sort_type;
  GtkRecentSortFunc sort_func;
  gpointer          sort_data;
  GDestroyNotify    data_destroy;

  GtkRecentFilter *current_filter;

  GSList *choosers;
  GtkRecentChooser *current_chooser;
};

STLWRT_DECLARE_FTYPE_FPARENT(GtkRecentAction, gtk_recent_action, GtkAction,
  /*< private >*/
  GtkRecentActionPrivate * (priv);
)

struct _GtkRecentActionClass
{
  GtkActionClass parent_class;
};


GtkAction *SF(gtk_recent_action_new)              (const gchar      *name,
                                               const gchar      *label,
                                               const gchar      *tooltip,
                                               const gchar      *stock_id);
GtkAction *SF(gtk_recent_action_new_for_manager)  (const gchar      *name,
                                               const gchar      *label,
                                               const gchar      *tooltip,
                                               const gchar      *stock_id,
                                               GtkRecentManager *manager);
gboolean   SF(gtk_recent_action_get_show_numbers) (GtkRecentAction  *action);
void       SF(gtk_recent_action_set_show_numbers) (GtkRecentAction  *action,
                                               gboolean          show_numbers);

G_END_DECLS

#endif /* __GTK_RECENT_ACTION_H__ */
