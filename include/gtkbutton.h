/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2020 Gordon N. Squash.
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

#ifndef __GTK_BUTTON_H__
#define __GTK_BUTTON_H__


#include <gtkbin.h>
#include <gtkimage.h>


G_BEGIN_DECLS

#define GTK_TYPE_BUTTON                 (SF(gtk_button_get_type) ())
#define GTK_BUTTON(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_BUTTON, GtkButton))
#define GTK_BUTTON_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_BUTTON, GtkButtonClass))
#define GTK_IS_BUTTON(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_BUTTON))
#define GTK_IS_BUTTON_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_BUTTON))
#define GTK_BUTTON_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_BUTTON, GtkButtonClass))

typedef struct _GtkButtonClass   GtkButtonClass;

STLWRT_DECLARE_VTYPE_VPARENT (GtkButton, gtk_button, GtkBin,
  GdkWindow * (event_window);

  gchar * (label_text);

  guint  (activate_timeout);

  guint  (constructed) : 1;
  guint  (in_button) : 1;
  guint  (button_down) : 1;
  guint  (relief) : 2;
  guint  (use_underline) : 1;
  guint  (use_stock) : 1;
  guint  (depressed) : 1;
  guint  (depress_on_activate) : 1;
  guint  (focus_on_click) : 1;
)


struct _GtkButtonPrivate
{
  gfloat          xalign;
  gfloat          yalign;
  GtkWidget      *image;
  guint           align_set             : 1;
  guint           image_is_stock        : 1;
  guint           has_grab              : 1;
  guint           use_action_appearance : 1;
  guint32         grab_time;
  GtkPositionType image_position;
  GtkAction      *action;
};


struct _GtkButtonClass
{
  GtkBinClass        parent_class;
  
  void (* pressed)  (GtkButton *button);
  void (* released) (GtkButton *button);
  void (* clicked)  (GtkButton *button);
  void (* enter)    (GtkButton *button);
  void (* leave)    (GtkButton *button);
  void (* activate) (GtkButton *button);
  
  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GtkWidget*     SF(gtk_button_new)               (void);
GtkWidget*     SF(gtk_button_new_with_label)    (const gchar    *label);
GtkWidget*     SF(gtk_button_new_from_stock)    (const gchar    *stock_id);
GtkWidget*     SF(gtk_button_new_with_mnemonic) (const gchar    *label);
#ifndef GTK_DISABLE_DEPRECATED
void           SF(gtk_button_pressed)           (GtkButton      *button);
void           SF(gtk_button_released)          (GtkButton      *button);
#endif
void           SF(gtk_button_clicked)           (GtkButton      *button);
#ifndef GTK_DISABLE_DEPRECATED
void           SF(gtk_button_enter)             (GtkButton      *button);
void           SF(gtk_button_leave)             (GtkButton      *button);
#endif

void                  SF(gtk_button_set_relief)         (GtkButton      *button,
                                                         GtkReliefStyle  newstyle);
GtkReliefStyle        SF(gtk_button_get_relief)         (GtkButton      *button);
void                  SF(gtk_button_set_label)          (GtkButton      *button,
                                                         const gchar    *label);
const gchar *         SF(gtk_button_get_label)          (GtkButton      *button);
void                  SF(gtk_button_set_use_underline)  (GtkButton      *button,
                                                         gboolean        use_underline);
gboolean              SF(gtk_button_get_use_underline)  (GtkButton      *button);
void                  SF(gtk_button_set_use_stock)      (GtkButton      *button,
                                                         gboolean        use_stock);
gboolean              SF(gtk_button_get_use_stock)      (GtkButton      *button);
void                  SF(gtk_button_set_focus_on_click) (GtkButton      *button,
                                                         gboolean        focus_on_click);
gboolean              SF(gtk_button_get_focus_on_click) (GtkButton      *button);
void                  SF(gtk_button_set_alignment)      (GtkButton      *button,
                                                         gfloat          xalign,
                                                         gfloat          yalign);
void                  SF(gtk_button_get_alignment)      (GtkButton      *button,
                                                         gfloat         *xalign,
                                                         gfloat         *yalign);
void                  SF(gtk_button_set_image)          (GtkButton      *button,
                                                         GtkWidget      *image);
GtkWidget*            SF(gtk_button_get_image)          (GtkButton      *button);
void                  SF(gtk_button_set_image_position) (GtkButton      *button,
                                                         GtkPositionType position);
GtkPositionType       SF(gtk_button_get_image_position) (GtkButton      *button);

GdkWindow*            SF(gtk_button_get_event_window)   (GtkButton      *button);

void SF(_gtk_button_set_depressed)             (GtkButton          *button,
                                                gboolean            depressed);
void SF(_gtk_button_paint)                     (GtkButton          *button,
                                                const GdkRectangle *area,
                                                GtkStateType        state_type,
                                                GtkShadowType       shadow_type,
                                                const gchar        *main_detail,
                                                const gchar        *default_detail);

G_END_DECLS

#endif /* __GTK_BUTTON_H__ */
