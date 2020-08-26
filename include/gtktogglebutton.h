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
#ifndef __GTK_TOGGLE_BUTTON_H__
#define __GTK_TOGGLE_BUTTON_H__

#include <gtkbutton.h>


G_BEGIN_DECLS

#define GTK_TYPE_TOGGLE_BUTTON                  (gtk_toggle_button_get_type ())
#define GTK_TOGGLE_BUTTON(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TOGGLE_BUTTON, GtkToggleButton))
#define GTK_TOGGLE_BUTTON_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TOGGLE_BUTTON, GtkToggleButtonClass))
#define GTK_IS_TOGGLE_BUTTON(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TOGGLE_BUTTON))
#define GTK_IS_TOGGLE_BUTTON_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TOGGLE_BUTTON))
#define GTK_TOGGLE_BUTTON_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TOGGLE_BUTTON, GtkToggleButtonClass))


typedef struct _GtkToggleButton       GtkToggleButtonFat;
typedef struct _GtkToggleButton       GtkToggleButtonThin;

typedef struct _GtkToggleButtonClass  GtkToggleButtonClass;

/********************************************************************/
struct _GtkToggleButtonProps
{


  guint  (active) : 1;
  guint  (draw_indicator) : 1;
  guint  (inconsistent) : 1;
};

struct _GtkToggleButtonFat
{
  GtkButtonFat   button;

  struct _GtkToggleButtonProps instance_properties;
};

struct _GtkToggleButtonThin
{
  GtkButtonThin  button;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkToggleButtonFat   fat_instance;
  struct _GtkToggleButtonThin  thin_instance;
}   GtkToggleButton;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkToggleButtonFat GtkToggleButton;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkToggleButtonThin GtkToggleButton;
#endif
/********************************************************************/



struct _GtkToggleButtonClass
{
  GtkButtonClass parent_class;

  void (* toggled) (GtkToggleButton *toggle_button);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GType      _T2_gtk_toggle_button_get_type          (void) G_GNUC_CONST;
GType      _3T_gtk_toggle_button_get_type          (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType      gtk_toggle_button_get_type          (void) G_GNUC_CONST;

GtkWidget* SF(gtk_toggle_button_new)               (void);
GtkWidget* SF(gtk_toggle_button_new_with_label)    (const gchar     *label);
GtkWidget* SF(gtk_toggle_button_new_with_mnemonic) (const gchar     *label);
void       SF(gtk_toggle_button_set_mode)          (GtkToggleButton *toggle_button,
                                                gboolean         draw_indicator);
gboolean   SF(gtk_toggle_button_get_mode)          (GtkToggleButton *toggle_button);
void       SF(gtk_toggle_button_set_active)        (GtkToggleButton *toggle_button,
                                                gboolean         is_active);
gboolean   SF(gtk_toggle_button_get_active)        (GtkToggleButton *toggle_button);
void       SF(gtk_toggle_button_toggled)           (GtkToggleButton *toggle_button);
void       SF(gtk_toggle_button_set_inconsistent)  (GtkToggleButton *toggle_button,
                                                gboolean         setting);
gboolean   SF(gtk_toggle_button_get_inconsistent)  (GtkToggleButton *toggle_button);


#ifndef GTK_DISABLE_DEPRECATED
#define	gtk_toggle_button_set_state		gtk_toggle_button_set_active
#endif /* GTK_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GTK_TOGGLE_BUTTON_H__ */
