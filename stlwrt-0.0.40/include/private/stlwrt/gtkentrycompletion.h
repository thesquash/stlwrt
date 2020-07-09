/* gtkentrycompletion.h
 * Copyright (C) 2003  Kristian Rietveld  <kris@gtk.org>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_ENTRY_COMPLETION_H__
#define __GTK_ENTRY_COMPLETION_H__

#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtktreemodel.h>
#include <gtkliststore.h>
#include <gtktreeviewcolumn.h>
#include <gtktreemodelfilter.h>

G_BEGIN_DECLS

#define GTK_TYPE_ENTRY_COMPLETION            (gtk_entry_completion_get_type ())
#define GTK_ENTRY_COMPLETION(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_ENTRY_COMPLETION, GtkEntryCompletion))
#define GTK_ENTRY_COMPLETION_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_ENTRY_COMPLETION, GtkEntryCompletionClass))
#define GTK_IS_ENTRY_COMPLETION(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_ENTRY_COMPLETION))
#define GTK_IS_ENTRY_COMPLETION_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_ENTRY_COMPLETION))
#define GTK_ENTRY_COMPLETION_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_ENTRY_COMPLETION, GtkEntryCompletionClass))

typedef struct _GtkEntryCompletion            GtkEntryCompletion;
typedef struct _GtkEntryCompletionClass       GtkEntryCompletionClass;
typedef struct _GtkEntryCompletionPrivate     GtkEntryCompletionPrivate;

typedef gboolean (* GtkEntryCompletionMatchFunc) (GtkEntryCompletion *completion,
                                                  const gchar        *key,
                                                  GtkTreeIter        *iter,
                                                  gpointer            user_data);


struct _GtkEntryCompletion
{
  GObject parent_instance;

  /*< private >*/
  GtkEntryCompletionPrivate * (priv);
};

struct _GtkEntryCompletionClass
{
  GObjectClass parent_class;

  gboolean (* match_selected)   (GtkEntryCompletion *completion,
                                 GtkTreeModel       *model,
                                 GtkTreeIter        *iter);
  void     (* action_activated) (GtkEntryCompletion *completion,
                                 gint                index_);
  gboolean (* insert_prefix)    (GtkEntryCompletion *completion,
				 const gchar        *prefix);
  gboolean (* cursor_on_match)  (GtkEntryCompletion *completion,
				 GtkTreeModel       *model,
				 GtkTreeIter        *iter);

  /* Padding for future expansion */
  void (*_gtk_reserved0) (void);
  void (*_gtk_reserved1) (void);
};

/* core */
GType               gtk_entry_completion_get_type               (void) G_GNUC_CONST;
GtkEntryCompletion *__gtk_entry_completion_new                    (void);

GtkWidget          *__gtk_entry_completion_get_entry              (GtkEntryCompletion          *completion);

void                __gtk_entry_completion_set_model              (GtkEntryCompletion          *completion,
                                                                 GtkTreeModel                *model);
GtkTreeModel       *__gtk_entry_completion_get_model              (GtkEntryCompletion          *completion);

void                __gtk_entry_completion_set_match_func         (GtkEntryCompletion          *completion,
                                                                 GtkEntryCompletionMatchFunc  func,
                                                                 gpointer                     func_data,
                                                                 GDestroyNotify               func_notify);
void                __gtk_entry_completion_set_minimum_key_length (GtkEntryCompletion          *completion,
                                                                 gint                         length);
gint                __gtk_entry_completion_get_minimum_key_length (GtkEntryCompletion          *completion);
void                __gtk_entry_completion_complete               (GtkEntryCompletion          *completion);
void                __gtk_entry_completion_insert_prefix          (GtkEntryCompletion          *completion);

void                __gtk_entry_completion_insert_action_text     (GtkEntryCompletion          *completion,
                                                                 gint                         index_,
                                                                 const gchar                 *text);
void                __gtk_entry_completion_insert_action_markup   (GtkEntryCompletion          *completion,
                                                                 gint                         index_,
                                                                 const gchar                 *markup);
void                __gtk_entry_completion_delete_action          (GtkEntryCompletion          *completion,
                                                                 gint                         index_);

void                __gtk_entry_completion_set_inline_completion  (GtkEntryCompletion          *completion,
                                                                 gboolean                     inline_completion);
gboolean            __gtk_entry_completion_get_inline_completion  (GtkEntryCompletion          *completion);
void                __gtk_entry_completion_set_inline_selection  (GtkEntryCompletion          *completion,
                                                                 gboolean                     inline_selection);
gboolean            __gtk_entry_completion_get_inline_selection  (GtkEntryCompletion          *completion);
void                __gtk_entry_completion_set_popup_completion   (GtkEntryCompletion          *completion,
                                                                 gboolean                     popup_completion);
gboolean            __gtk_entry_completion_get_popup_completion   (GtkEntryCompletion          *completion);
void                __gtk_entry_completion_set_popup_set_width    (GtkEntryCompletion          *completion,
                                                                 gboolean                     popup_set_width);
gboolean            __gtk_entry_completion_get_popup_set_width    (GtkEntryCompletion          *completion);
void                __gtk_entry_completion_set_popup_single_match (GtkEntryCompletion          *completion,
                                                                 gboolean                     popup_single_match);
gboolean            __gtk_entry_completion_get_popup_single_match (GtkEntryCompletion          *completion);

const gchar         *__gtk_entry_completion_get_completion_prefix (GtkEntryCompletion *completion);
/* convenience */
void                __gtk_entry_completion_set_text_column        (GtkEntryCompletion          *completion,
                                                                 gint                         column);
gint                __gtk_entry_completion_get_text_column        (GtkEntryCompletion          *completion);

G_END_DECLS

#endif /* __GTK_ENTRY_COMPLETION_H__ */
