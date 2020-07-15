
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
 *
 * Modified by the GTK+ Team and others 2003.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/.
 */

#ifndef __GTK_TOGGLE_ACTION_H__
#define __GTK_TOGGLE_ACTION_H__

#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtkaction.h>

G_BEGIN_DECLS

#define GTK_TYPE_TOGGLE_ACTION            (gtk_toggle_action_get_type ())
#define GTK_TOGGLE_ACTION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TOGGLE_ACTION, GtkToggleAction))
#define GTK_TOGGLE_ACTION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TOGGLE_ACTION, GtkToggleActionClass))
#define GTK_IS_TOGGLE_ACTION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TOGGLE_ACTION))
#define GTK_IS_TOGGLE_ACTION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TOGGLE_ACTION))
#define GTK_TOGGLE_ACTION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), GTK_TYPE_TOGGLE_ACTION, GtkToggleActionClass))

typedef struct _GtkToggleAction        GtkToggleActionFat;
typedef struct _GtkToggleAction        GtkToggleActionThin;
typedef struct _GtkToggleActionPrivate GtkToggleActionPrivate;
typedef struct _GtkToggleActionClass   GtkToggleActionClassFat;
typedef struct _GtkToggleActionClass   GtkToggleActionClassThin;

struct _GtkToggleAction
{
  GtkAction parent;

  /*< private >*/

  GtkToggleActionPrivate * (private_data);
};

struct _GtkToggleActionClass
{
  GtkActionClass parent_class;

  void (* toggled) (GtkToggleAction *action);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

GType            _T2_gtk_toggle_action_get_type          (void) G_GNUC_CONST;
GType            _3T_gtk_toggle_action_get_type          (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType            gtk_toggle_action_get_type          (void) G_GNUC_CONST;
GtkToggleAction *__gtk_toggle_action_new               (const gchar     *name,
                                                      const gchar     *label,
                                                      const gchar     *tooltip,
                                                      const gchar     *stock_id);
void             __gtk_toggle_action_toggled           (GtkToggleAction *action);
void             __gtk_toggle_action_set_active        (GtkToggleAction *action,
                                                      gboolean         is_active);
gboolean         __gtk_toggle_action_get_active        (GtkToggleAction *action);
void             __gtk_toggle_action_set_draw_as_radio (GtkToggleAction *action,
                                                      gboolean         draw_as_radio);
gboolean         __gtk_toggle_action_get_draw_as_radio (GtkToggleAction *action);


G_END_DECLS

#endif  /* __GTK_TOGGLE_ACTION_H__ */
