
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

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/.
 */

#ifndef __GTK_TOOLBAR_H__
#define __GTK_TOOLBAR_H__


#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtkcontainer.h>
#include <gtktooltips.h>
#include <gtktoolitem.h>

#ifndef GTK_DISABLE_DEPRECATED

/* Not needed, retained for compatibility -Yosh */
#include <gtkpixmap.h>
#include <gtksignal.h>

#endif /* GTK_DISABLE_DEPRECATED */

G_BEGIN_DECLS

#define GTK_TYPE_TOOLBAR            (__gtk_toolbar_get_type ())
#define GTK_TOOLBAR(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TOOLBAR, GtkToolbar))
#define GTK_TOOLBAR_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TOOLBAR, GtkToolbarClass))
#define GTK_IS_TOOLBAR(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TOOLBAR))
#define GTK_IS_TOOLBAR_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TOOLBAR))
#define GTK_TOOLBAR_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TOOLBAR, GtkToolbarClass))

#ifndef GTK_DISABLE_DEPRECATED
typedef enum
{
  GTK_TOOLBAR_CHILD_SPACE,
  GTK_TOOLBAR_CHILD_BUTTON,
  GTK_TOOLBAR_CHILD_TOGGLEBUTTON,
  GTK_TOOLBAR_CHILD_RADIOBUTTON,
  GTK_TOOLBAR_CHILD_WIDGET
} GtkToolbarChildType;

typedef struct _GtkToolbarChild	     GtkToolbarChild;

struct _GtkToolbarChild
{
  GtkToolbarChildType type;
  GtkWidget *widget;
  GtkWidget *icon;
  GtkWidget *label;
};

#endif /* GTK_DISABLE_DEPRECATED */

typedef enum
{
  GTK_TOOLBAR_SPACE_EMPTY,
  GTK_TOOLBAR_SPACE_LINE
} GtkToolbarSpaceStyle;

typedef struct _GtkToolbar           GtkToolbar;
typedef struct _GtkToolbarClass      GtkToolbarClass;
typedef struct _GtkToolbarPrivate    GtkToolbarPrivate;

struct _GtkToolbar
{
  GtkContainer container;

  /*< public >*/
  gint              (num_children);
  GList           * (children);
  GtkOrientation    (orientation);
  GtkToolbarStyle   (style);
  GtkIconSize       (icon_size);

#ifndef GTK_DISABLE_DEPRECATED
  GtkTooltips     * (tooltips);
#else
  gpointer          (_tooltips);
#endif

  /*< private >*/
  gint              (button_maxw);		/* maximum width of homogeneous children */
  gint              (button_maxh);		/* maximum height of homogeneous children */

  guint            _gtk_reserved1;
  guint            _gtk_reserved2;

  guint             (style_set) : 1;
  guint             (icon_size_set) : 1;
};

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

GType           __gtk_toolbar_get_type                (void) G_GNUC_CONST;
GtkWidget *     __gtk_toolbar_new                     (void);

void            __gtk_toolbar_insert                  (GtkToolbar      *toolbar,
						     GtkToolItem     *item,
						     gint             pos);

gint            __gtk_toolbar_get_item_index          (GtkToolbar      *toolbar,
						     GtkToolItem     *item);
gint            __gtk_toolbar_get_n_items             (GtkToolbar      *toolbar);
GtkToolItem *   __gtk_toolbar_get_nth_item            (GtkToolbar      *toolbar,
						     gint             n);

gboolean        __gtk_toolbar_get_show_arrow          (GtkToolbar      *toolbar);
void            __gtk_toolbar_set_show_arrow          (GtkToolbar      *toolbar,
						     gboolean         show_arrow);

GtkToolbarStyle __gtk_toolbar_get_style               (GtkToolbar      *toolbar);
void            __gtk_toolbar_set_style               (GtkToolbar      *toolbar,
						     GtkToolbarStyle  style);
void            __gtk_toolbar_unset_style             (GtkToolbar      *toolbar);

GtkIconSize     __gtk_toolbar_get_icon_size           (GtkToolbar      *toolbar);
void            __gtk_toolbar_set_icon_size           (GtkToolbar      *toolbar,
                                                     GtkIconSize      icon_size);
void            __gtk_toolbar_unset_icon_size         (GtkToolbar      *toolbar);

GtkReliefStyle  __gtk_toolbar_get_relief_style        (GtkToolbar      *toolbar);
gint            __gtk_toolbar_get_drop_index          (GtkToolbar      *toolbar,
						     gint             x,
						     gint             y);
void            __gtk_toolbar_set_drop_highlight_item (GtkToolbar      *toolbar,
						     GtkToolItem     *tool_item,
						     gint             index_);


/* internal functions */
gchar *         ___gtk_toolbar_elide_underscores      (const gchar         *original);
void            ___gtk_toolbar_paint_space_line       (GtkWidget           *widget,
						     GtkToolbar          *toolbar,
						     const GdkRectangle  *area,
						     const GtkAllocation *allocation);
gint            ___gtk_toolbar_get_default_space_size (void);



#ifndef GTK_DISABLE_DEPRECATED

GtkOrientation  __gtk_toolbar_get_orientation         (GtkToolbar      *toolbar);
void            __gtk_toolbar_set_orientation         (GtkToolbar      *toolbar,
						     GtkOrientation   orientation);
gboolean        __gtk_toolbar_get_tooltips            (GtkToolbar      *toolbar);
void            __gtk_toolbar_set_tooltips            (GtkToolbar      *toolbar,
						     gboolean         enable);

/* Simple button items */
GtkWidget* __gtk_toolbar_append_item   (GtkToolbar      *toolbar,
				      const char      *text,
				      const char      *tooltip_text,
				      const char      *tooltip_private_text,
				      GtkWidget       *icon,
				      GCallback        callback,
				      gpointer         user_data);
GtkWidget* __gtk_toolbar_prepend_item  (GtkToolbar      *toolbar,
				      const char      *text,
				      const char      *tooltip_text,
				      const char      *tooltip_private_text,
				      GtkWidget       *icon,
				      GCallback        callback,
				      gpointer         user_data);
GtkWidget* __gtk_toolbar_insert_item   (GtkToolbar      *toolbar,
				      const char      *text,
				      const char      *tooltip_text,
				      const char      *tooltip_private_text,
				      GtkWidget       *icon,
				      GCallback        callback,
				      gpointer         user_data,
				      gint             position);

/* Stock Items */
GtkWidget* __gtk_toolbar_insert_stock    (GtkToolbar      *toolbar,
					const gchar     *stock_id,
					const char      *tooltip_text,
					const char      *tooltip_private_text,
					GCallback        callback,
					gpointer         user_data,
					gint             position);

/* Space Items */
void       __gtk_toolbar_append_space    (GtkToolbar      *toolbar);
void       __gtk_toolbar_prepend_space   (GtkToolbar      *toolbar);
void       __gtk_toolbar_insert_space    (GtkToolbar      *toolbar,
					gint             position);
void       __gtk_toolbar_remove_space    (GtkToolbar      *toolbar,
                                        gint             position);
/* Any element type */
GtkWidget* __gtk_toolbar_append_element  (GtkToolbar      *toolbar,
					GtkToolbarChildType type,
					GtkWidget       *widget,
					const char      *text,
					const char      *tooltip_text,
					const char      *tooltip_private_text,
					GtkWidget       *icon,
					GCallback        callback,
					gpointer         user_data);

GtkWidget* __gtk_toolbar_prepend_element (GtkToolbar      *toolbar,
					GtkToolbarChildType type,
					GtkWidget       *widget,
					const char      *text,
					const char      *tooltip_text,
					const char      *tooltip_private_text,
					GtkWidget       *icon,
					GCallback        callback,
					gpointer         user_data);

GtkWidget* __gtk_toolbar_insert_element  (GtkToolbar      *toolbar,
					GtkToolbarChildType type,
					GtkWidget       *widget,
					const char      *text,
					const char      *tooltip_text,
					const char      *tooltip_private_text,
					GtkWidget       *icon,
					GCallback        callback,
					gpointer         user_data,
					gint             position);

/* Generic Widgets */
void       __gtk_toolbar_append_widget   (GtkToolbar      *toolbar,
					GtkWidget       *widget,
					const char      *tooltip_text,
					const char      *tooltip_private_text);
void       __gtk_toolbar_prepend_widget  (GtkToolbar      *toolbar,
					GtkWidget       *widget,
					const char      *tooltip_text,
					const char	*tooltip_private_text);
void       __gtk_toolbar_insert_widget   (GtkToolbar      *toolbar,
					GtkWidget       *widget,
					const char      *tooltip_text,
					const char      *tooltip_private_text,
					gint             position);

#endif /* GTK_DISABLE_DEPRECATED */


G_END_DECLS

#endif /* __GTK_TOOLBAR_H__ */