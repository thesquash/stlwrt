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

#ifndef __GTK_DIALOG_H__
#define __GTK_DIALOG_H__

#include <stlwrt.h>


#include <gtkwindow.h>


G_BEGIN_DECLS

/* Parameters for dialog construction */
typedef enum 
{
  GTK_DIALOG_MODAL               = 1 << 0, /* call SF(gtk_window_set_modal) (win, TRUE) */
  GTK_DIALOG_DESTROY_WITH_PARENT = 1 << 1, /* call SF(gtk_window_set_destroy_with_parent) () */
  GTK_DIALOG_NO_SEPARATOR        = 1 << 2  /* no separator bar above buttons */
} GtkDialogFlags;

/* Convenience enum to use for response_id's.  Positive values are
 * totally user-interpreted. GTK will sometimes return
 * GTK_RESPONSE_NONE if no response_id is available.
 *
 *  Typical usage is:
 *     if (SF(gtk_dialog_run)(dialog) == GTK_RESPONSE_ACCEPT)
 *       blah();
 */
typedef enum 
{
  /* GTK returns this if a response widget has no response_id,
   * or if the dialog gets programmatically hidden or destroyed.
   */
  GTK_RESPONSE_NONE = -1,

  /* GTK won't return these unless you pass them in
   * as the response for an action widget. They are
   * for your convenience.
   */
  GTK_RESPONSE_REJECT = -2,
  GTK_RESPONSE_ACCEPT = -3,

  /* If the dialog is deleted. */
  GTK_RESPONSE_DELETE_EVENT = -4,

  /* These are returned from GTK dialogs, and you can also use them
   * yourself if you like.
   */
  GTK_RESPONSE_OK     = -5,
  GTK_RESPONSE_CANCEL = -6,
  GTK_RESPONSE_CLOSE  = -7,
  GTK_RESPONSE_YES    = -8,
  GTK_RESPONSE_NO     = -9,
  GTK_RESPONSE_APPLY  = -10,
  GTK_RESPONSE_HELP   = -11
} GtkResponseType;


#define GTK_TYPE_DIALOG                  (gtk_dialog_get_type ())
#define GTK_DIALOG(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_DIALOG, GtkDialog))
#define GTK_DIALOG_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_DIALOG, GtkDialogClass))
#define GTK_IS_DIALOG(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_DIALOG))
#define GTK_IS_DIALOG_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_DIALOG))
#define GTK_DIALOG_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_DIALOG, GtkDialogClass))

typedef struct _GtkDialogClass   GtkDialogClass;


STLWRT_DECLARE_VTYPE_VPARENT(GtkDialog, gtk_dialog, GtkWindow,
  /*< public >*/
  GtkWidget * (vbox);
  GtkWidget * (action_area);

  /*< private >*/
  GtkWidget * (separator);
)

struct _GtkDialogClass
{
  GtkWindowClass parent_class;

  void (* response) (GtkDialog *dialog, gint response_id);

  /* Keybinding signals */

  void (* close)    (GtkDialog *dialog);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GtkWidget* SF(gtk_dialog_new)      (void);

GtkWidget* SF(gtk_dialog_new_with_buttons) (const gchar     *title,
                                        GtkWindow       *parent,
                                        GtkDialogFlags   flags,
                                        const gchar     *first_button_text,
                                        ...);

void       SF(gtk_dialog_add_action_widget) (GtkDialog   *dialog,
                                         GtkWidget   *child,
                                         gint         response_id);
GtkWidget* SF(gtk_dialog_add_button)        (GtkDialog   *dialog,
                                         const gchar *button_text,
                                         gint         response_id);
void       SF(gtk_dialog_add_buttons)       (GtkDialog   *dialog,
                                         const gchar *first_button_text,
                                         ...) G_GNUC_NULL_TERMINATED;

void SF(gtk_dialog_set_response_sensitive) (GtkDialog *dialog,
                                        gint       response_id,
                                        gboolean   setting);
void SF(gtk_dialog_set_default_response)   (GtkDialog *dialog,
                                        gint       response_id);
GtkWidget* SF(gtk_dialog_get_widget_for_response) (GtkDialog *dialog,
                                               gint       response_id);
gint SF(gtk_dialog_get_response_for_widget) (GtkDialog *dialog,
					 GtkWidget *widget);

#if !defined (GTK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
void     SF(gtk_dialog_set_has_separator) (GtkDialog *dialog,
                                       gboolean   setting);
gboolean SF(gtk_dialog_get_has_separator) (GtkDialog *dialog);
#endif

gboolean SF(gtk_alternative_dialog_button_order) (GdkScreen *screen);
void     SF(gtk_dialog_set_alternative_button_order) (GtkDialog *dialog,
						  gint       first_response_id,
						  ...);
void     SF(gtk_dialog_set_alternative_button_order_from_array) (GtkDialog *dialog,
                                                             gint       n_params,
                                                             gint      *new_order);

/* Emit response signal */
void SF(gtk_dialog_response)           (GtkDialog *dialog,
                                    gint       response_id);

/* Returns response_id */
gint SF(gtk_dialog_run)                (GtkDialog *dialog);

GtkWidget * SF(gtk_dialog_get_action_area)  (GtkDialog *dialog);
GtkWidget * SF(gtk_dialog_get_content_area) (GtkDialog *dialog);

/* For private use only */
void SF(_gtk_dialog_set_ignore_separator) (GtkDialog *dialog,
				       gboolean   ignore_separator);

G_END_DECLS

#endif /* __GTK_DIALOG_H__ */
