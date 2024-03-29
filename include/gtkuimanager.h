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

#ifndef __GTK_UI_MANAGER_H__
#define __GTK_UI_MANAGER_H__

#include <stlwrt.h>


#include <gtkaccelgroup.h>

#include <gtkwidget.h>

#include <gtkaction.h>

#include <gtkactiongroup.h>

G_BEGIN_DECLS

#define GTK_TYPE_UI_MANAGER            (gtk_ui_manager_get_type ())
#define GTK_UI_MANAGER(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_UI_MANAGER, GtkUIManager))
#define GTK_UI_MANAGER_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_UI_MANAGER, GtkUIManagerClass))
#define GTK_IS_UI_MANAGER(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_UI_MANAGER))
#define GTK_IS_UI_MANAGER_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_UI_MANAGER))
#define GTK_UI_MANAGER_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_UI_MANAGER, GtkUIManagerClass))

typedef struct _GtkUIManagerClass GtkUIManagerClass;
typedef struct _GtkUIManagerPrivate GtkUIManagerPrivate;

STLWRT_DECLARE_FTYPE_FPARENT(GtkUIManager, gtk_ui_manager, GObject,
  /*< private >*/

  GtkUIManagerPrivate * (private_data);
)

struct _GtkUIManagerClass {
  GObjectClass parent_class;

  /* Signals */
  void (* add_widget)       (GtkUIManager *merge,
                             GtkWidget    *widget);
  void (* actions_changed)  (GtkUIManager *merge);
  void (* connect_proxy)    (GtkUIManager *merge,
			     GtkAction    *action,
			     GtkWidget    *proxy);
  void (* disconnect_proxy) (GtkUIManager *merge,
			     GtkAction    *action,
			     GtkWidget    *proxy);
  void (* pre_activate)     (GtkUIManager *merge,
			     GtkAction    *action);
  void (* post_activate)    (GtkUIManager *merge,
			     GtkAction    *action);

  /* Virtual functions */
  GtkWidget * (* get_widget) (GtkUIManager *manager,
                              const gchar  *path);
  GtkAction * (* get_action) (GtkUIManager *manager,
                              const gchar  *path);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
};

typedef enum  {
  GTK_UI_MANAGER_AUTO              = 0,
  GTK_UI_MANAGER_MENUBAR           = 1 << 0,
  GTK_UI_MANAGER_MENU              = 1 << 1,
  GTK_UI_MANAGER_TOOLBAR           = 1 << 2,
  GTK_UI_MANAGER_PLACEHOLDER       = 1 << 3,
  GTK_UI_MANAGER_POPUP             = 1 << 4,
  GTK_UI_MANAGER_MENUITEM          = 1 << 5,
  GTK_UI_MANAGER_TOOLITEM          = 1 << 6,
  GTK_UI_MANAGER_SEPARATOR         = 1 << 7,
  GTK_UI_MANAGER_ACCELERATOR       = 1 << 8,
  GTK_UI_MANAGER_POPUP_WITH_ACCELS = 1 << 9
} GtkUIManagerItemType;

#ifdef G_OS_WIN32
/* Reserve old name for DLL ABI backward compatibility */
#define gtk_ui_manager_add_ui_from_file gtk_ui_manager_add_ui_from_file_utf8
#endif


GtkUIManager  *SF(gtk_ui_manager_new)                 (void);
void           SF(gtk_ui_manager_set_add_tearoffs)    (GtkUIManager          *self,
						   gboolean               add_tearoffs);
gboolean       SF(gtk_ui_manager_get_add_tearoffs)    (GtkUIManager          *self);
void           SF(gtk_ui_manager_insert_action_group) (GtkUIManager          *self,
						   GtkActionGroup        *action_group,
						   gint                   pos);
void           SF(gtk_ui_manager_remove_action_group) (GtkUIManager          *self,
						   GtkActionGroup        *action_group);
GList         *SF(gtk_ui_manager_get_action_groups)   (GtkUIManager          *self);
GtkAccelGroup *SF(gtk_ui_manager_get_accel_group)     (GtkUIManager          *self);
GtkWidget     *SF(gtk_ui_manager_get_widget)          (GtkUIManager          *self,
						   const gchar           *path);
GSList        *SF(gtk_ui_manager_get_toplevels)       (GtkUIManager          *self,
						   GtkUIManagerItemType   types);
GtkAction     *SF(gtk_ui_manager_get_action)          (GtkUIManager          *self,
						   const gchar           *path);
guint          SF(gtk_ui_manager_add_ui_from_string)  (GtkUIManager          *self,
						   const gchar           *buffer,
						   gssize                 length,
						   GError               **error);
guint          SF(gtk_ui_manager_add_ui_from_file)    (GtkUIManager          *self,
						   const gchar           *filename,
						   GError               **error);
void           SF(gtk_ui_manager_add_ui)              (GtkUIManager          *self,
						   guint                  merge_id,
						   const gchar           *path,
						   const gchar           *name,
						   const gchar           *action,
						   GtkUIManagerItemType   type,
						   gboolean               top);
void           SF(gtk_ui_manager_remove_ui)           (GtkUIManager          *self,
						   guint                  merge_id);
gchar         *SF(gtk_ui_manager_get_ui)              (GtkUIManager          *self);
void           SF(gtk_ui_manager_ensure_update)       (GtkUIManager          *self);
guint          SF(gtk_ui_manager_new_merge_id)        (GtkUIManager          *self);

G_END_DECLS

#endif /* __GTK_UI_MANAGER_H__ */
