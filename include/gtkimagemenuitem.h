/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) Red Hat, Inc.
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

#ifndef __GTK_IMAGE_MENU_ITEM_H__
#define __GTK_IMAGE_MENU_ITEM_H__

#include <stlwrt.h>
#include <stlwrt-typedefs.h>


#include <gtkmenuitem.h>


G_BEGIN_DECLS

#define GTK_TYPE_IMAGE_MENU_ITEM            (gtk_image_menu_item_get_type ())
#define GTK_IMAGE_MENU_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_IMAGE_MENU_ITEM, GtkImageMenuItem))
#define GTK_IMAGE_MENU_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_IMAGE_MENU_ITEM, GtkImageMenuItemClass))
#define GTK_IS_IMAGE_MENU_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_IMAGE_MENU_ITEM))
#define GTK_IS_IMAGE_MENU_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_IMAGE_MENU_ITEM))
#define GTK_IMAGE_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_IMAGE_MENU_ITEM, GtkImageMenuItemClass))

typedef struct _GtkImageMenuItemClass  GtkImageMenuItemClass;

STLWRT_DECLARE_VTYPE(GtkImageMenuItem, gtk_image_menu_item, GtkMenuItem,
  /*< private >*/
  GtkWidget      * (image);
)

struct _GtkImageMenuItemClass
{
  GtkMenuItemClass parent_class;
};


GtkWidget* SF(gtk_image_menu_item_new)               (void);
GtkWidget* SF(gtk_image_menu_item_new_with_label)    (const gchar      *label);
GtkWidget* SF(gtk_image_menu_item_new_with_mnemonic) (const gchar      *label);
GtkWidget* SF(gtk_image_menu_item_new_from_stock)    (const gchar      *stock_id,
                                                  GtkAccelGroup    *accel_group);
void       SF(gtk_image_menu_item_set_always_show_image) (GtkImageMenuItem *image_menu_item,
                                                      gboolean          always_show);
gboolean   SF(gtk_image_menu_item_get_always_show_image) (GtkImageMenuItem *image_menu_item);
void       SF(gtk_image_menu_item_set_image)         (GtkImageMenuItem *image_menu_item,
                                                  GtkWidget        *image);
GtkWidget* SF(gtk_image_menu_item_get_image)         (GtkImageMenuItem *image_menu_item);
void       SF(gtk_image_menu_item_set_use_stock)     (GtkImageMenuItem *image_menu_item,
						  gboolean          use_stock);
gboolean   SF(gtk_image_menu_item_get_use_stock)     (GtkImageMenuItem *image_menu_item);
void       SF(gtk_image_menu_item_set_accel_group)   (GtkImageMenuItem *image_menu_item, 
						  GtkAccelGroup    *accel_group);

G_END_DECLS

#endif /* __GTK_IMAGE_MENU_ITEM_H__ */
