
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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_CHECK_BUTTON_H__
#define __GTK_CHECK_BUTTON_H__


#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtktogglebutton.h>


G_BEGIN_DECLS

#define GTK_TYPE_CHECK_BUTTON                  (gtk_check_button_get_type ())
#define GTK_CHECK_BUTTON(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CHECK_BUTTON, GtkCheckButton))
#define GTK_CHECK_BUTTON_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CHECK_BUTTON, GtkCheckButtonClass))
#define GTK_IS_CHECK_BUTTON(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CHECK_BUTTON))
#define GTK_IS_CHECK_BUTTON_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CHECK_BUTTON))
#define GTK_CHECK_BUTTON_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CHECK_BUTTON, GtkCheckButtonClass))


typedef struct _GtkCheckButton       GtkCheckButton;

typedef struct _GtkCheckButtonClass  GtkCheckButtonClass;

struct _GtkCheckButton
{
  GtkToggleButton toggle_button;
};

struct _GtkCheckButtonClass
{
  GtkToggleButtonClass parent_class;

  void (* draw_indicator) (GtkCheckButton *check_button,
			   GdkRectangle   *area);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GType      gtk_check_button_get_type       (void) G_GNUC_CONST;
GtkWidget* __gtk_check_button_new               (void);
GtkWidget* __gtk_check_button_new_with_label    (const gchar *label);
GtkWidget* __gtk_check_button_new_with_mnemonic (const gchar *label);

void ___gtk_check_button_get_props (GtkCheckButton *check_button,
				  gint           *indicator_size,
				  gint           *indicator_spacing);

G_END_DECLS

#endif /* __GTK_CHECK_BUTTON_H__ */