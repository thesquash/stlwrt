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

/* Color picker button for GNOME
 *
 * Author: Federico Mena <federico@nuclecu.unam.mx>
 */

#ifndef __GTK_COLOR_BUTTON_H__
#define __GTK_COLOR_BUTTON_H__

#include <stlwrt.h>



#include <gtkbutton.h>

G_BEGIN_DECLS


/* The GtkColorButton widget is a simple color picker in a button.
 * The button displays a sample of the currently selected color.  When
 * the user clicks on the button, a color selection dialog pops up.
 * The color picker emits the "color_set" signal when the color is set.
 */

#define GTK_TYPE_COLOR_BUTTON             (gtk_color_button_get_type ())
#define GTK_COLOR_BUTTON(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_COLOR_BUTTON, GtkColorButton))
#define GTK_COLOR_BUTTON_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_COLOR_BUTTON, GtkColorButtonClass))
#define GTK_IS_COLOR_BUTTON(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_COLOR_BUTTON))
#define GTK_IS_COLOR_BUTTON_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_COLOR_BUTTON))
#define GTK_COLOR_BUTTON_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_COLOR_BUTTON, GtkColorButtonClass))

typedef struct _GtkColorButtonClass     GtkColorButtonClass;
typedef struct _GtkColorButtonPrivate   GtkColorButtonPrivate;


STLWRT_DECLARE_VTYPE_VPARENT(GtkColorButton, gtk_color_button, GtkButton,
  /*< private >*/

  GtkColorButtonPrivate * (priv);
)

struct _GtkColorButtonClass {
  GtkButtonClass parent_class;

  void (* color_set) (GtkColorButton *cp);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GtkWidget *SF(gtk_color_button_new)            (void);
GtkWidget *SF(gtk_color_button_new_with_color) (const GdkColor *color);
void       SF(gtk_color_button_set_color)      (GtkColorButton *color_button,
					    const GdkColor *color);
void       SF(gtk_color_button_set_alpha)      (GtkColorButton *color_button,
					    guint16         alpha);
void       SF(gtk_color_button_get_color)      (GtkColorButton *color_button,
					    GdkColor       *color);
guint16    SF(gtk_color_button_get_alpha)      (GtkColorButton *color_button);
void       SF(gtk_color_button_set_use_alpha)  (GtkColorButton *color_button,
					    gboolean        use_alpha);
gboolean   SF(gtk_color_button_get_use_alpha)  (GtkColorButton *color_button);
void       SF(gtk_color_button_set_title)      (GtkColorButton *color_button,
					    const gchar    *title);
const gchar *SF(gtk_color_button_get_title) (GtkColorButton *color_button);


G_END_DECLS

#endif  /* __GTK_COLOR_BUTTON_H__ */
