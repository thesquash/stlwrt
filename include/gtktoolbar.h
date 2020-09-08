/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 * GtkToolbar copyright (C) Federico Mena
 *
 * Copyright (C) 2002 Anders Carlsson <andersca@gnome.org>
 * Copyright (C) 2002 James Henstridge <james@daa.com.au>
 * Copyright (C) 2003 Soeren Sandmann <sandmann@daimi.au.dk>
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

#ifndef __GTK_TOOLBAR_H__
#define __GTK_TOOLBAR_H__

#include <gtkcontainer.h>
#include <gtktooltips.h>
#include <gtktoolitem.h>

#ifndef GTK_DISABLE_DEPRECATED

/* Not needed, retained for compatibility -Yosh */
#include <gtkpixmap.h>
#include <gtksignal.h>

#endif /* GTK_DISABLE_DEPRECATED */

G_BEGIN_DECLS

#define GTK_TYPE_TOOLBAR            (gtk_toolbar_get_type ())
#define GTK_TOOLBAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TOOLBAR, GtkToolbar))
#define GTK_TOOLBAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TOOLBAR, GtkToolbarClass))
#define GTK_IS_TOOLBAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TOOLBAR))
#define GTK_IS_TOOLBAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TOOLBAR))
#define GTK_TOOLBAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TOOLBAR, GtkToolbarClass))

typedef enum
{
  GTK_TOOLBAR_SPACE_EMPTY,
  GTK_TOOLBAR_SPACE_LINE
} GtkToolbarSpaceStyle;

typedef struct _GtkToolbarClass      GtkToolbarClass;
typedef struct _GtkToolbarPrivate    GtkToolbarPrivate;

struct _GtkToolbarPrivate
{
  GList	*	content;
  
  GtkWidget *	arrow;
  GtkWidget *	arrow_button;
  GtkMenu *	menu;
  
  GdkWindow *	event_window;
  ApiMode	api_mode;
  GtkSettings *	settings;
  int		idle_id;
  GtkToolItem *	highlight_tool_item;
  gint		max_homogeneous_pixels;
  
  GTimer *	timer;

  gulong        settings_connection;

  guint         show_arrow : 1;
  guint         need_sync : 1;
  guint         is_sliding : 1;
  guint         need_rebuild : 1;  /* whether the overflow menu should be regenerated */
  guint         animation : 1;
};

STLWRT_DECLARE_VTYPE_VPARENT(GtkToolbar, gtk_toolbar, gtk_container,
  /*< public >*/
  gint              (num_children);
  GList           * (children);
  GtkOrientation    (orientation);
  GtkToolbarStyle   (style);
  GtkIconSize       (icon_size);

  gpointer          (_tooltips);

  /*< private >*/
  gint              (button_maxw);		/* maximum width of homogeneous children */
  gint              (button_maxh);		/* maximum height of homogeneous children */

  guint            _gtk_reserved1;
  guint            _gtk_reserved2;

  guint             (style_set) : 1;
  guint             (icon_size_set) : 1;
)

struct _GtkToolbarClass
{
  GtkContainerClass parent_class;

  /* signals */
  void     (* orientation_changed) (GtkToolbar       *toolbar,
				    GtkOrientation    orientation);
  void     (* style_changed)       (GtkToolbar       *toolbar,
				    GtkToolbarStyle   style);
  gboolean (* popup_context_menu)  (GtkToolbar       *toolbar,
				    gint              x,
				    gint              y,
				    gint              button_number);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
};


GtkWidget *     SF(gtk_toolbar_new)                     (void);

void            SF(gtk_toolbar_insert)                  (GtkToolbar      *toolbar,
						     GtkToolItem     *item,
						     gint             pos);

gint            SF(gtk_toolbar_get_item_index)          (GtkToolbar      *toolbar,
						     GtkToolItem     *item);
gint            SF(gtk_toolbar_get_n_items)             (GtkToolbar      *toolbar);
GtkToolItem *   SF(gtk_toolbar_get_nth_item)            (GtkToolbar      *toolbar,
						     gint             n);

gboolean        SF(gtk_toolbar_get_show_arrow)          (GtkToolbar      *toolbar);
void            SF(gtk_toolbar_set_show_arrow)          (GtkToolbar      *toolbar,
						     gboolean         show_arrow);

GtkToolbarStyle SF(gtk_toolbar_get_style)               (GtkToolbar      *toolbar);
void            SF(gtk_toolbar_set_style)               (GtkToolbar      *toolbar,
						     GtkToolbarStyle  style);
void            SF(gtk_toolbar_unset_style)             (GtkToolbar      *toolbar);

GtkIconSize     SF(gtk_toolbar_get_icon_size)           (GtkToolbar      *toolbar);
void            SF(gtk_toolbar_set_icon_size)           (GtkToolbar      *toolbar,
                                                     GtkIconSize      icon_size);
void            SF(gtk_toolbar_unset_icon_size)         (GtkToolbar      *toolbar);

GtkReliefStyle  SF(gtk_toolbar_get_relief_style)        (GtkToolbar      *toolbar);
gint            SF(gtk_toolbar_get_drop_index)          (GtkToolbar      *toolbar,
						     gint             x,
						     gint             y);
void            SF(gtk_toolbar_set_drop_highlight_item) (GtkToolbar      *toolbar,
						     GtkToolItem     *tool_item,
						     gint             index_);


/* internal functions */
gchar *         SF(_gtk_toolbar_elide_underscores)      (const gchar         *original);
void            SF(_gtk_toolbar_paint_space_line)       (GtkWidget           *widget,
						     GtkToolbar          *toolbar,
						     const GdkRectangle  *area,
						     const GtkAllocation *allocation);
gint            SF(_gtk_toolbar_get_default_space_size) (void);


G_END_DECLS

#endif /* __GTK_TOOLBAR_H__ */
