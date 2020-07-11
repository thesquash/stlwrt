
/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2000 Red Hat, Inc.
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
 *
 * Global clipboard abstraction.
 */

#ifndef __GTK_CLIPBOARD_H__
#define __GTK_CLIPBOARD_H__

#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtkselection.h>

G_BEGIN_DECLS

#define GTK_TYPE_CLIPBOARD            (__gtk_clipboard_get_type ())
#define GTK_CLIPBOARD(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CLIPBOARD, GtkClipboard))
#define GTK_IS_CLIPBOARD(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CLIPBOARD))

typedef void (* GtkClipboardReceivedFunc)         (GtkClipboard     *clipboard,
					           GtkSelectionData *selection_data,
					           gpointer          data);
typedef void (* GtkClipboardTextReceivedFunc)     (GtkClipboard     *clipboard,
					           const gchar      *text,
					           gpointer          data);
typedef void (* GtkClipboardRichTextReceivedFunc) (GtkClipboard     *clipboard,
                                                   GdkAtom           format,
					           const guint8     *text,
                                                   gsize             length,
					           gpointer          data);
typedef void (* GtkClipboardImageReceivedFunc)    (GtkClipboard     *clipboard,
						   GdkPixbuf        *pixbuf,
						   gpointer          data);
typedef void (* GtkClipboardURIReceivedFunc)      (GtkClipboard     *clipboard,
						   gchar           **uris,
						   gpointer          data);
typedef void (* GtkClipboardTargetsReceivedFunc)  (GtkClipboard     *clipboard,
					           GdkAtom          *atoms,
						   gint              n_atoms,
					           gpointer          data);

/* Should these functions have GtkClipboard *clipboard as the first argument?
 * right now for ClearFunc, you may have trouble determining _which_ clipboard
 * was cleared, if you reuse your ClearFunc for multiple clipboards.
 */
typedef void (* GtkClipboardGetFunc)          (GtkClipboard     *clipboard,
					       GtkSelectionData *selection_data,
					       guint             info,
					       gpointer          user_data_or_owner);
typedef void (* GtkClipboardClearFunc)        (GtkClipboard     *clipboard,
					       gpointer          user_data_or_owner);

GType         __gtk_clipboard_get_type (void) G_GNUC_CONST;

GtkClipboard *__gtk_clipboard_get_for_display (GdkDisplay   *display,
					     GdkAtom       selection);
#ifndef GDK_MULTIHEAD_SAFE
GtkClipboard *__gtk_clipboard_get             (GdkAtom       selection);
#endif

GdkDisplay   *__gtk_clipboard_get_display     (GtkClipboard *clipboard);


gboolean __gtk_clipboard_set_with_data  (GtkClipboard          *clipboard,
				       const GtkTargetEntry  *targets,
				       guint                  n_targets,
				       GtkClipboardGetFunc    get_func,
				       GtkClipboardClearFunc  clear_func,
				       gpointer               user_data);
gboolean __gtk_clipboard_set_with_owner (GtkClipboard          *clipboard,
				       const GtkTargetEntry  *targets,
				       guint                  n_targets,
				       GtkClipboardGetFunc    get_func,
				       GtkClipboardClearFunc  clear_func,
				       GObject               *owner);
GObject *__gtk_clipboard_get_owner      (GtkClipboard          *clipboard);
void     __gtk_clipboard_clear          (GtkClipboard          *clipboard);
void     __gtk_clipboard_set_text       (GtkClipboard          *clipboard,
				       const gchar           *text,
				       gint                   len);
void     __gtk_clipboard_set_image      (GtkClipboard          *clipboard,
				       GdkPixbuf             *pixbuf);

void __gtk_clipboard_request_contents  (GtkClipboard                     *clipboard,
                                      GdkAtom                           target,
                                      GtkClipboardReceivedFunc          callback,
                                      gpointer                          user_data);
void __gtk_clipboard_request_text      (GtkClipboard                     *clipboard,
                                      GtkClipboardTextReceivedFunc      callback,
                                      gpointer                          user_data);
void __gtk_clipboard_request_rich_text (GtkClipboard                     *clipboard,
                                      GtkTextBuffer                    *buffer,
                                      GtkClipboardRichTextReceivedFunc  callback,
                                      gpointer                          user_data);
void __gtk_clipboard_request_image     (GtkClipboard                     *clipboard,
                                      GtkClipboardImageReceivedFunc     callback,
                                      gpointer                          user_data);
void __gtk_clipboard_request_uris      (GtkClipboard                     *clipboard,
                                      GtkClipboardURIReceivedFunc       callback,
                                      gpointer                          user_data);
void __gtk_clipboard_request_targets   (GtkClipboard                     *clipboard,
                                      GtkClipboardTargetsReceivedFunc   callback,
                                      gpointer                          user_data);

GtkSelectionData *__gtk_clipboard_wait_for_contents  (GtkClipboard  *clipboard,
                                                    GdkAtom        target);
gchar *           __gtk_clipboard_wait_for_text      (GtkClipboard  *clipboard);
guint8 *          __gtk_clipboard_wait_for_rich_text (GtkClipboard  *clipboard,
                                                    GtkTextBuffer *buffer,
                                                    GdkAtom       *format,
                                                    gsize         *length);
GdkPixbuf *       __gtk_clipboard_wait_for_image     (GtkClipboard  *clipboard);
gchar **          __gtk_clipboard_wait_for_uris      (GtkClipboard  *clipboard);
gboolean          __gtk_clipboard_wait_for_targets   (GtkClipboard  *clipboard,
                                                    GdkAtom      **targets,
                                                    gint          *n_targets);

gboolean __gtk_clipboard_wait_is_text_available      (GtkClipboard  *clipboard);
gboolean __gtk_clipboard_wait_is_rich_text_available (GtkClipboard  *clipboard,
                                                    GtkTextBuffer *buffer);
gboolean __gtk_clipboard_wait_is_image_available     (GtkClipboard  *clipboard);
gboolean __gtk_clipboard_wait_is_uris_available      (GtkClipboard  *clipboard);
gboolean __gtk_clipboard_wait_is_target_available    (GtkClipboard  *clipboard,
                                                    GdkAtom        target);


void __gtk_clipboard_set_can_store (GtkClipboard         *clipboard,
				  const GtkTargetEntry *targets,
				  gint                  n_targets);

void __gtk_clipboard_store         (GtkClipboard   *clipboard);

/* private */
void     ___gtk_clipboard_handle_event    (GdkEventOwnerChange *event);

void     ___gtk_clipboard_store_all       (void);

G_END_DECLS

#endif /* __GTK_CLIPBOARD_H__ */
