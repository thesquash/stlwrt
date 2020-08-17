
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

#ifndef __GTK_TEAROFF_MENU_ITEM_H__
#define __GTK_TEAROFF_MENU_ITEM_H__

#include <gtkmenuitem.h>


G_BEGIN_DECLS

#define GTK_TYPE_TEAROFF_MENU_ITEM	      (gtk_tearoff_menu_item_get_type ())
#define GTK_TEAROFF_MENU_ITEM(obj)	      (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TEAROFF_MENU_ITEM, GtkTearoffMenuItem))
#define GTK_TEAROFF_MENU_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TEAROFF_MENU_ITEM, GtkTearoffMenuItemClass))
#define GTK_IS_TEAROFF_MENU_ITEM(obj)	      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TEAROFF_MENU_ITEM))
#define GTK_IS_TEAROFF_MENU_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TEAROFF_MENU_ITEM))
#define GTK_TEAROFF_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TEAROFF_MENU_ITEM, GtkTearoffMenuItemClass))


typedef struct _GtkTearoffMenuItem       GtkTearoffMenuItemFat;
typedef struct _GtkTearoffMenuItem       GtkTearoffMenuItemThin;

typedef struct _GtkTearoffMenuItemClass  GtkTearoffMenuItemClass;

/********************************************************************/
struct _GtkTearoffMenuItemProps
{


  guint  (torn_off) : 1;
};

struct _GtkTearoffMenuItemFat
{
  GtkMenuItemFat   menu_item;

  struct _GtkTearoffMenuItemProps instance_properties;
};

struct _GtkTearoffMenuItemThin
{
  GtkMenuItemThin  menu_item;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkTearoffMenuItemFat   fat_instance;
  struct _GtkTearoffMenuItemThin  thin_instance;
}   GtkTearoffMenuItem;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkTearoffMenuItemFat GtkTearoffMenuItem;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkTearoffMenuItemThin GtkTearoffMenuItem;
#endif
/********************************************************************/



struct _GtkTearoffMenuItemClass
{
  GtkMenuItemClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GType	   _T2_gtk_tearoff_menu_item_get_type     (void) G_GNUC_CONST;
GType	   _3T_gtk_tearoff_menu_item_get_type     (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType	   gtk_tearoff_menu_item_get_type     (void) G_GNUC_CONST;
GtkWidget* __gtk_tearoff_menu_item_new	      (void);

G_END_DECLS

#endif /* __GTK_TEAROFF_MENU_ITEM_H__ */
