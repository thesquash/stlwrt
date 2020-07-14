
/* gtkstatusicon.h:
 *
 * Copyright (C) 2003 Sun Microsystems, Inc.
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
 *
 * Authors:
 *      Mark McLoughlin <mark@skynet.ie>
 */

#ifndef __GTK_STATUS_ICON_H__
#define __GTK_STATUS_ICON_H__

#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtkimage.h>
#include <gtkmenu.h>

G_BEGIN_DECLS

#define GTK_TYPE_STATUS_ICON         (__gtk_status_icon_get_type ())
#define GTK_STATUS_ICON(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GTK_TYPE_STATUS_ICON, GtkStatusIcon))
#define GTK_STATUS_ICON_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST ((k), GTK_TYPE_STATUS_ICON, GtkStatusIconClass))
#define GTK_IS_STATUS_ICON(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GTK_TYPE_STATUS_ICON))
#define GTK_IS_STATUS_ICON_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), GTK_TYPE_STATUS_ICON))
#define GTK_STATUS_ICON_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GTK_TYPE_STATUS_ICON, GtkStatusIconClass))

typedef struct _GtkStatusIcon	     GtkStatusIconFat;
typedef struct _GtkStatusIcon	     GtkStatusIconSkinny;
typedef struct _GtkStatusIconClass   GtkStatusIconClassFat;
typedef struct _GtkStatusIconClass   GtkStatusIconClassSkinny;
typedef struct _GtkStatusIconPrivate GtkStatusIconPrivate;

struct _GtkStatusIcon
{
  GObject               parent_instance;

  GtkStatusIconPrivate * (priv);
};

struct _GtkStatusIconClass
{
  GObjectClass parent_class;

  void     (* activate)             (GtkStatusIcon  *status_icon);
  void     (* popup_menu)           (GtkStatusIcon  *status_icon,
                                     guint           button,
                                     guint32         activate_time);
  gboolean (* size_changed)         (GtkStatusIcon  *status_icon,
                                     gint            size);
  gboolean (* button_press_event)   (GtkStatusIcon  *status_icon,
                                     GdkEventButton *event);
  gboolean (* button_release_event) (GtkStatusIcon  *status_icon,
                                     GdkEventButton *event);
  gboolean (* scroll_event)         (GtkStatusIcon  *status_icon,
                                     GdkEventScroll *event);
  gboolean (* query_tooltip)        (GtkStatusIcon  *status_icon,
                                     gint            x,
                                     gint            y,
                                     gboolean        keyboard_mode,
                                     GtkTooltip     *tooltip);

  void (*__gtk_reserved1);
  void (*__gtk_reserved2);
};

GType                 __gtk_status_icon_get_type           (void) G_GNUC_CONST;

GtkStatusIcon        *__gtk_status_icon_new                (void);
GtkStatusIcon        *__gtk_status_icon_new_from_pixbuf    (GdkPixbuf          *pixbuf);
GtkStatusIcon        *__gtk_status_icon_new_from_file      (const gchar        *filename);
GtkStatusIcon        *__gtk_status_icon_new_from_stock     (const gchar        *stock_id);
GtkStatusIcon        *__gtk_status_icon_new_from_icon_name (const gchar        *icon_name);
GtkStatusIcon        *__gtk_status_icon_new_from_gicon     (GIcon              *icon);

void                  __gtk_status_icon_set_from_pixbuf    (GtkStatusIcon      *status_icon,
							  GdkPixbuf          *pixbuf);
void                  __gtk_status_icon_set_from_file      (GtkStatusIcon      *status_icon,
							  const gchar        *filename);
void                  __gtk_status_icon_set_from_stock     (GtkStatusIcon      *status_icon,
							  const gchar        *stock_id);
void                  __gtk_status_icon_set_from_icon_name (GtkStatusIcon      *status_icon,
							  const gchar        *icon_name);
void                  __gtk_status_icon_set_from_gicon     (GtkStatusIcon      *status_icon,
                                                          GIcon              *icon);

GtkImageType          __gtk_status_icon_get_storage_type   (GtkStatusIcon      *status_icon);

GdkPixbuf            *__gtk_status_icon_get_pixbuf         (GtkStatusIcon      *status_icon);
const gchar *         __gtk_status_icon_get_stock          (GtkStatusIcon      *status_icon);
const gchar *         __gtk_status_icon_get_icon_name      (GtkStatusIcon      *status_icon);
GIcon                *__gtk_status_icon_get_gicon          (GtkStatusIcon      *status_icon);

gint                  __gtk_status_icon_get_size           (GtkStatusIcon      *status_icon);

void                  __gtk_status_icon_set_screen         (GtkStatusIcon      *status_icon,
                                                          GdkScreen          *screen);
GdkScreen            *__gtk_status_icon_get_screen         (GtkStatusIcon      *status_icon);

#ifndef GTK_DISABLE_DEPRECATED
void                  __gtk_status_icon_set_tooltip        (GtkStatusIcon      *status_icon,
                                                          const gchar        *tooltip_text);
#endif
void                  __gtk_status_icon_set_has_tooltip    (GtkStatusIcon      *status_icon,
                                                          gboolean            has_tooltip);
void                  __gtk_status_icon_set_tooltip_text   (GtkStatusIcon      *status_icon,
                                                          const gchar        *text);
void                  __gtk_status_icon_set_tooltip_markup (GtkStatusIcon      *status_icon,
                                                          const gchar        *markup);
void                  __gtk_status_icon_set_title          (GtkStatusIcon      *status_icon,
                                                          const gchar        *title);
const gchar *         __gtk_status_icon_get_title          (GtkStatusIcon      *status_icon);
void                  __gtk_status_icon_set_name           (GtkStatusIcon      *status_icon,
                                                          const gchar        *name);
void                  __gtk_status_icon_set_visible        (GtkStatusIcon      *status_icon,
							  gboolean            visible);
gboolean              __gtk_status_icon_get_visible        (GtkStatusIcon      *status_icon);

#if !defined (GTK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
void                  __gtk_status_icon_set_blinking       (GtkStatusIcon      *status_icon,
							  gboolean            blinking);
gboolean              __gtk_status_icon_get_blinking       (GtkStatusIcon      *status_icon);
#endif

gboolean              __gtk_status_icon_is_embedded        (GtkStatusIcon      *status_icon);

void                  __gtk_status_icon_position_menu      (GtkMenu            *menu,
							  gint               *x,
							  gint               *y,
							  gboolean           *push_in,
							  gpointer            user_data);
gboolean              __gtk_status_icon_get_geometry       (GtkStatusIcon      *status_icon,
							  GdkScreen         **screen,
							  GdkRectangle       *area,
							  GtkOrientation     *orientation);
gboolean              __gtk_status_icon_get_has_tooltip    (GtkStatusIcon      *status_icon);
gchar                *__gtk_status_icon_get_tooltip_text   (GtkStatusIcon      *status_icon);
gchar                *__gtk_status_icon_get_tooltip_markup (GtkStatusIcon      *status_icon);

guint32               __gtk_status_icon_get_x11_window_id  (GtkStatusIcon      *status_icon);

G_END_DECLS

#endif /* __GTK_STATUS_ICON_H__ */
