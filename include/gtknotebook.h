/* -*- Mode: C; c-file-style: "gnu"; tab-width: 8 -*- */
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

#ifndef __GTK_NOTEBOOK_H__
#define __GTK_NOTEBOOK_H__

#include <stlwrt.h>


#include <gtkcontainer.h>


G_BEGIN_DECLS

#define GTK_TYPE_NOTEBOOK                  (gtk_notebook_get_type ())
#define GTK_NOTEBOOK(obj)                  (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_NOTEBOOK, GtkNotebook))
#define GTK_NOTEBOOK_CLASS(klass)          (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_NOTEBOOK, GtkNotebookClass))
#define GTK_IS_NOTEBOOK(obj)               (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_NOTEBOOK))
#define GTK_IS_NOTEBOOK_CLASS(klass)       (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_NOTEBOOK))
#define GTK_NOTEBOOK_GET_CLASS(obj)        (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_NOTEBOOK, GtkNotebookClass))


typedef enum 
{
  GTK_NOTEBOOK_TAB_FIRST,
  GTK_NOTEBOOK_TAB_LAST
} GtkNotebookTab;

typedef struct _GtkNotebookClass  GtkNotebookClass;
#if !defined (GTK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
typedef struct _GtkNotebookPage   GtkNotebookPage;
#endif

STLWRT_DECLARE_VTYPE_VPARENT(GtkNotebook, gtk_notebook, GtkContainer,
#if !defined (GTK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
  GtkNotebookPage * (cur_page);
#else
  gpointer  (cur_page);
#endif
  GList * (children);
  GList * (first_tab);		/* The first tab visible (for scrolling notebooks) */
  GList * (focus_tab);
  
  GtkWidget * (menu);
  GdkWindow * (event_window);
  
  guint32  (timer);
  
  guint16  (tab_hborder);
  guint16  (tab_vborder);
  
  guint  (show_tabs)          : 1;
  guint  (homogeneous)        : 1;
  guint  (show_border)        : 1;
  guint  (tab_pos)            : 2;
  guint  (scrollable)         : 1;
  guint  (in_child)           : 3;
  guint  (click_child)        : 3;
  guint  (button)             : 2;
  guint  (need_timer)         : 1;
  guint  (child_has_focus)    : 1;
  guint  (have_visible_child) : 1;
  guint  (focus_out)          : 1;	/* Flag used by ::move-focus-out implementation */

  guint  (has_before_previous) : 1;
  guint  (has_before_next)     : 1;
  guint  (has_after_previous)  : 1;
  guint  (has_after_next)      : 1;
)

struct _GtkNotebookClass
{
  GtkContainerClass parent_class;

  void (* switch_page)       (GtkNotebook     *notebook,
#if !defined (GTK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
                              GtkNotebookPage *page,
#else
                              gpointer         page,
#endif
			      guint            page_num);

  /* Action signals for keybindings */
  gboolean (* select_page)     (GtkNotebook       *notebook,
                                gboolean           move_focus);
  gboolean (* focus_tab)       (GtkNotebook       *notebook,
                                GtkNotebookTab     type);
  gboolean (* change_current_page) (GtkNotebook   *notebook,
                                gint               offset);
  void (* move_focus_out)      (GtkNotebook       *notebook,
				GtkDirectionType   direction);
  gboolean (* reorder_tab)     (GtkNotebook       *notebook,
				GtkDirectionType   direction,
				gboolean           move_to_last);

  /* More vfuncs */
  gint (* insert_page)         (GtkNotebook       *notebook,
			        GtkWidget         *child,
				GtkWidget         *tab_label,
				GtkWidget         *menu_label,
				gint               position);

  GtkNotebook * (* create_window) (GtkNotebook       *notebook,
                                   GtkWidget         *page,
                                   gint               x,
                                   gint               y);

  void (*_gtk_reserved1) (void);
};

typedef GtkNotebook* (*GtkNotebookWindowCreationFunc) (GtkNotebook *source,
                                                       GtkWidget   *page,
                                                       gint         x,
                                                       gint         y,
                                                       gpointer     data);

/***********************************************************
 *           Creation, insertion, deletion                 *
 ***********************************************************/

GtkWidget * SF(gtk_notebook_new)        (void);
gint SF(gtk_notebook_append_page)       (GtkNotebook *notebook,
				     GtkWidget   *child,
				     GtkWidget   *tab_label);
gint SF(gtk_notebook_append_page_menu)  (GtkNotebook *notebook,
				     GtkWidget   *child,
				     GtkWidget   *tab_label,
				     GtkWidget   *menu_label);
gint SF(gtk_notebook_prepend_page)      (GtkNotebook *notebook,
				     GtkWidget   *child,
				     GtkWidget   *tab_label);
gint SF(gtk_notebook_prepend_page_menu) (GtkNotebook *notebook,
				     GtkWidget   *child,
				     GtkWidget   *tab_label,
				     GtkWidget   *menu_label);
gint SF(gtk_notebook_insert_page)       (GtkNotebook *notebook,
				     GtkWidget   *child,
				     GtkWidget   *tab_label,
				     gint         position);
gint SF(gtk_notebook_insert_page_menu)  (GtkNotebook *notebook,
				     GtkWidget   *child,
				     GtkWidget   *tab_label,
				     GtkWidget   *menu_label,
				     gint         position);
void SF(gtk_notebook_remove_page)       (GtkNotebook *notebook,
				     gint         page_num);

/***********************************************************
 *           Tabs drag and drop                            *
 ***********************************************************/

#ifndef GTK_DISABLE_DEPRECATED
void SF(gtk_notebook_set_window_creation_hook) (GtkNotebookWindowCreationFunc  func,
					    gpointer                       data,
                                            GDestroyNotify                 destroy);
void SF(gtk_notebook_set_group_id)             (GtkNotebook *notebook,
					    gint         group_id);
gint SF(gtk_notebook_get_group_id)             (GtkNotebook *notebook);

void SF(gtk_notebook_set_group)                (GtkNotebook *notebook,
					    gpointer     group);
gpointer SF(gtk_notebook_get_group)            (GtkNotebook *notebook);
#endif /* GTK_DISABLE_DEPRECATED */

void         SF(gtk_notebook_set_group_name)   (GtkNotebook *notebook,
                                            const gchar *group_name);
const gchar *SF(gtk_notebook_get_group_name)   (GtkNotebook *notebook);


/***********************************************************
 *            query, set current NotebookPage              *
 ***********************************************************/

gint       SF(gtk_notebook_get_current_page) (GtkNotebook *notebook);
GtkWidget* SF(gtk_notebook_get_nth_page)     (GtkNotebook *notebook,
					  gint         page_num);
gint       SF(gtk_notebook_get_n_pages)      (GtkNotebook *notebook);
gint       SF(gtk_notebook_page_num)         (GtkNotebook *notebook,
					  GtkWidget   *child);
void       SF(gtk_notebook_set_current_page) (GtkNotebook *notebook,
					  gint         page_num);
void       SF(gtk_notebook_next_page)        (GtkNotebook *notebook);
void       SF(gtk_notebook_prev_page)        (GtkNotebook *notebook);

/***********************************************************
 *            set Notebook, NotebookTab style              *
 ***********************************************************/

void     SF(gtk_notebook_set_show_border)      (GtkNotebook     *notebook,
					    gboolean         show_border);
gboolean SF(gtk_notebook_get_show_border)      (GtkNotebook     *notebook);
void     SF(gtk_notebook_set_show_tabs)        (GtkNotebook     *notebook,
					    gboolean         show_tabs);
gboolean SF(gtk_notebook_get_show_tabs)        (GtkNotebook     *notebook);
void     SF(gtk_notebook_set_tab_pos)          (GtkNotebook     *notebook,
				            GtkPositionType  pos);
GtkPositionType SF(gtk_notebook_get_tab_pos)   (GtkNotebook     *notebook);

#ifndef GTK_DISABLE_DEPRECATED
void     SF(gtk_notebook_set_homogeneous_tabs) (GtkNotebook     *notebook,
					    gboolean         homogeneous);
void     SF(gtk_notebook_set_tab_border)       (GtkNotebook     *notebook,
					    guint            border_width);
void     SF(gtk_notebook_set_tab_hborder)      (GtkNotebook     *notebook,
					    guint            tab_hborder);
void     SF(gtk_notebook_set_tab_vborder)      (GtkNotebook     *notebook,
					    guint            tab_vborder);
#endif /* GTK_DISABLE_DEPRECATED */

void     SF(gtk_notebook_set_scrollable)       (GtkNotebook     *notebook,
					    gboolean         scrollable);
gboolean SF(gtk_notebook_get_scrollable)       (GtkNotebook     *notebook);
guint16  SF(gtk_notebook_get_tab_hborder)      (GtkNotebook     *notebook);
guint16  SF(gtk_notebook_get_tab_vborder)      (GtkNotebook     *notebook);

/***********************************************************
 *               enable/disable PopupMenu                  *
 ***********************************************************/

void SF(gtk_notebook_popup_enable)  (GtkNotebook *notebook);
void SF(gtk_notebook_popup_disable) (GtkNotebook *notebook);

/***********************************************************
 *             query/set NotebookPage Properties           *
 ***********************************************************/

GtkWidget * SF(gtk_notebook_get_tab_label)    (GtkNotebook *notebook,
					   GtkWidget   *child);
void SF(gtk_notebook_set_tab_label)           (GtkNotebook *notebook,
					   GtkWidget   *child,
					   GtkWidget   *tab_label);
void SF(gtk_notebook_set_tab_label_text)      (GtkNotebook *notebook,
					   GtkWidget   *child,
					   const gchar *tab_text);
const gchar *SF(gtk_notebook_get_tab_label_text) (GtkNotebook *notebook,
                                              GtkWidget   *child);
GtkWidget * SF(gtk_notebook_get_menu_label)   (GtkNotebook *notebook,
					   GtkWidget   *child);
void SF(gtk_notebook_set_menu_label)          (GtkNotebook *notebook,
					   GtkWidget   *child,
					   GtkWidget   *menu_label);
void SF(gtk_notebook_set_menu_label_text)     (GtkNotebook *notebook,
					   GtkWidget   *child,
					   const gchar *menu_text);
const gchar *SF(gtk_notebook_get_menu_label_text) (GtkNotebook *notebook,
                                               GtkWidget   *child);
#ifndef GTK_DISABLE_DEPRECATED
void SF(gtk_notebook_query_tab_label_packing) (GtkNotebook *notebook,
					   GtkWidget   *child,
					   gboolean    *expand,
					   gboolean    *fill,
					   GtkPackType *pack_type);
void SF(gtk_notebook_set_tab_label_packing)   (GtkNotebook *notebook,
					   GtkWidget   *child,
					   gboolean     expand,
					   gboolean     fill,
					   GtkPackType  pack_type);
#endif
void SF(gtk_notebook_reorder_child)           (GtkNotebook *notebook,
					   GtkWidget   *child,
					   gint         position);
gboolean SF(gtk_notebook_get_tab_reorderable) (GtkNotebook *notebook,
					   GtkWidget   *child);
void SF(gtk_notebook_set_tab_reorderable)     (GtkNotebook *notebook,
					   GtkWidget   *child,
					   gboolean     reorderable);
gboolean SF(gtk_notebook_get_tab_detachable)  (GtkNotebook *notebook,
					   GtkWidget   *child);
void SF(gtk_notebook_set_tab_detachable)      (GtkNotebook *notebook,
					   GtkWidget   *child,
					   gboolean     detachable);

GtkWidget* SF(gtk_notebook_get_action_widget) (GtkNotebook *notebook,
                                           GtkPackType  pack_type);
void       SF(gtk_notebook_set_action_widget) (GtkNotebook *notebook,
                                           GtkWidget   *widget,
                                           GtkPackType  pack_type);

#ifndef GTK_DISABLE_DEPRECATED
#define	gtk_notebook_current_page               gtk_notebook_get_current_page
#define gtk_notebook_set_page                   gtk_notebook_set_current_page
#endif /* GTK_DISABLE_DEPRECATED */

G_END_DECLS

#endif /* __GTK_NOTEBOOK_H__ */
