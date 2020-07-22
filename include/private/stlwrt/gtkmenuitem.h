
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

#ifndef __GTK_MENU_ITEM_H__
#define __GTK_MENU_ITEM_H__

#include <gtkbin.h>


G_BEGIN_DECLS

#define	GTK_TYPE_MENU_ITEM		(gtk_menu_item_get_type ())
#define GTK_MENU_ITEM(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_MENU_ITEM, GtkMenuItem))
#define GTK_MENU_ITEM_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_MENU_ITEM, GtkMenuItemClass))
#define GTK_IS_MENU_ITEM(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_MENU_ITEM))
#define GTK_IS_MENU_ITEM_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_MENU_ITEM))
#define GTK_MENU_ITEM_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_MENU_ITEM, GtkMenuItemClass))


typedef struct _GtkMenuItem	  GtkMenuItemFat;
typedef struct _GtkMenuItem	  GtkMenuItemThin;

typedef struct _GtkMenuItemClass  GtkMenuItemClass;

/********************************************************************/
struct _GtkMenuItemInstanceProps
{


  GtkWidget * (submenu);
  GdkWindow * (event_window);

  guint16  (toggle_size);
  guint16  (accelerator_width);
  gchar  * (accel_path);

  guint  (show_submenu_indicator) : 1;
  guint  (submenu_placement) : 1;
  guint  (submenu_direction) : 1;
  guint  (right_justify): 1;
  guint  (timer_from_keypress) : 1;
  guint  (from_menubar) : 1;
  guint  (timer);
};

struct _GtkMenuItemFat
{
  GtkBinFat   bin;

  struct _GtkMenuItemInstanceProps instance_properties;
};

struct _GtkMenuItemThin
{
  GtkBinThin  bin;

  gpointer reserved;
};


typedef union
{
  struct _GtkMenuItemFat   fat_instance;
  struct _GtkMenuItemThin  thin_instance;
}   GtkMenuItem;
/********************************************************************/



struct _GtkMenuItemClass
{
  GtkBinClass parent_class;
  
  /* If the following flag is true, then we should always hide
   * the menu when the MenuItem is activated. Otherwise, the 
   * it is up to the caller. For instance, when navigating
   * a menu with the keyboard, <Space> doesn't hide, but
   * <Return> does.
   */
  guint hide_on_activate : 1;
  
  void (* select)               (GtkMenuItem *item);
  void (* deselect)             (GtkMenuItem *item);
  
  void (* activate)             (GtkMenuItem *menu_item);
  void (* activate_item)        (GtkMenuItem *menu_item);
  void (* toggle_size_request)  (GtkMenuItem *menu_item,
				 gint        *requisition);
  void (* toggle_size_allocate) (GtkMenuItem *menu_item,
				 gint         allocation);
  void (* set_label)            (GtkMenuItem *menu_item,
				 const gchar *label);
  const gchar *(* get_label) (GtkMenuItem *menu_item);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
};


GType	   _T2_gtk_menu_item_get_type	      (void) G_GNUC_CONST;
GType	   _3T_gtk_menu_item_get_type	      (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType	   gtk_menu_item_get_type	      (void) G_GNUC_CONST;
GtkWidget* __gtk_menu_item_new                  (void);
GtkWidget* __gtk_menu_item_new_with_label       (const gchar         *label);
GtkWidget* __gtk_menu_item_new_with_mnemonic    (const gchar         *label);
void       __gtk_menu_item_set_submenu          (GtkMenuItem         *menu_item,
					       GtkWidget           *submenu);
GtkWidget* __gtk_menu_item_get_submenu          (GtkMenuItem         *menu_item);
void       __gtk_menu_item_select               (GtkMenuItem         *menu_item);
void       __gtk_menu_item_deselect             (GtkMenuItem         *menu_item);
void       __gtk_menu_item_activate             (GtkMenuItem         *menu_item);
void       __gtk_menu_item_toggle_size_request  (GtkMenuItem         *menu_item,
					       gint                *requisition);
void       __gtk_menu_item_toggle_size_allocate (GtkMenuItem         *menu_item,
					       gint                 allocation);
void       __gtk_menu_item_set_right_justified  (GtkMenuItem         *menu_item,
					       gboolean             right_justified);
gboolean   __gtk_menu_item_get_right_justified  (GtkMenuItem         *menu_item);
void	   __gtk_menu_item_set_accel_path	      (GtkMenuItem	   *menu_item,
					       const gchar	   *accel_path);
const gchar* __gtk_menu_item_get_accel_path     (GtkMenuItem    *menu_item);

void       __gtk_menu_item_set_label            (GtkMenuItem         *menu_item,
 					       const gchar         *label);
const gchar *__gtk_menu_item_get_label          (GtkMenuItem         *menu_item);

void       __gtk_menu_item_set_use_underline    (GtkMenuItem         *menu_item,
 					       gboolean             setting);
gboolean   __gtk_menu_item_get_use_underline    (GtkMenuItem         *menu_item);

/* private */
void	  ___gtk_menu_item_refresh_accel_path   (GtkMenuItem	   *menu_item,
					       const gchar	   *prefix,
					       GtkAccelGroup	   *accel_group,
					       gboolean		    group_changed);
gboolean  ___gtk_menu_item_is_selectable        (GtkWidget           *menu_item);
void      ___gtk_menu_item_popup_submenu        (GtkWidget           *menu_item,
                                               gboolean             with_delay);
void      ___gtk_menu_item_popdown_submenu      (GtkWidget           *menu_item);

#ifndef GTK_DISABLE_DEPRECATED
void       __gtk_menu_item_remove_submenu       (GtkMenuItem         *menu_item);
#define __gtk_menu_item_right_justify(menu_item) __gtk_menu_item_set_right_justified ((menu_item), TRUE)
#endif /* GTK_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GTK_MENU_ITEM_H__ */
