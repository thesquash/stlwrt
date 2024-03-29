/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_CHECK_MENU_ITEM_H__
#define __GTK_CHECK_MENU_ITEM_H__

#include <stlwrt.h>


#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif


#include <gtkmenuitem.h>


G_BEGIN_DECLS

#define GTK_TYPE_CHECK_MENU_ITEM            (gtk_check_menu_item_get_type ())
#define GTK_CHECK_MENU_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CHECK_MENU_ITEM, GtkCheckMenuItem))
#define GTK_CHECK_MENU_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CHECK_MENU_ITEM, GtkCheckMenuItemClass))
#define GTK_IS_CHECK_MENU_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CHECK_MENU_ITEM))
#define GTK_IS_CHECK_MENU_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CHECK_MENU_ITEM))
#define GTK_CHECK_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CHECK_MENU_ITEM, GtkCheckMenuItemClass))

typedef struct _GtkCheckMenuItemClass  GtkCheckMenuItemClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkCheckMenuItem, gtk_check_menu_item, GtkMenuItem,
  guint  (active) : 1;
  guint  (always_show_toggle) : 1;
  guint  (inconsistent) : 1;
  guint  (draw_as_radio) : 1;
)

struct _GtkCheckMenuItemClass
{
  GtkMenuItemClass parent_class;

  void (* toggled)	  (GtkCheckMenuItem *check_menu_item);
  void (* draw_indicator) (GtkCheckMenuItem *check_menu_item,
			   GdkRectangle	    *area);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GtkWidget* SF(gtk_check_menu_item_new)               (void);
GtkWidget* SF(gtk_check_menu_item_new_with_label)    (const gchar      *label);
GtkWidget* SF(gtk_check_menu_item_new_with_mnemonic) (const gchar      *label);
void       SF(gtk_check_menu_item_set_active)        (GtkCheckMenuItem *check_menu_item,
						  gboolean          is_active);
gboolean   SF(gtk_check_menu_item_get_active)        (GtkCheckMenuItem *check_menu_item);
void       SF(gtk_check_menu_item_toggled)           (GtkCheckMenuItem *check_menu_item);
void       SF(gtk_check_menu_item_set_inconsistent)  (GtkCheckMenuItem *check_menu_item,
						  gboolean          setting);
gboolean   SF(gtk_check_menu_item_get_inconsistent)  (GtkCheckMenuItem *check_menu_item);
void       SF(gtk_check_menu_item_set_draw_as_radio) (GtkCheckMenuItem *check_menu_item,
						  gboolean          draw_as_radio);
gboolean   SF(gtk_check_menu_item_get_draw_as_radio) (GtkCheckMenuItem *check_menu_item);


#ifndef GTK_DISABLE_DEPRECATED
void	   SF(gtk_check_menu_item_set_show_toggle) (GtkCheckMenuItem *menu_item,
						gboolean	  always);
#define	gtk_check_menu_item_set_state		gtk_check_menu_item_set_active
#endif

G_END_DECLS

#endif /* __GTK_CHECK_MENU_ITEM_H__ */
