/* gtktreeselection.h
 * Copyright (C) 2000  Red Hat, Inc.,  Jonathan Blandford <jrb@redhat.com>
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

#ifndef __GTK_TREE_SELECTION_H__
#define __GTK_TREE_SELECTION_H__

#include <gtktreeview.h>

G_BEGIN_DECLS


#define GTK_TYPE_TREE_SELECTION			(gtk_tree_selection_get_type ())
#define GTK_TREE_SELECTION(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TREE_SELECTION, GtkTreeSelection))
#define GTK_TREE_SELECTION_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TREE_SELECTION, GtkTreeSelectionClass))
#define GTK_IS_TREE_SELECTION(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TREE_SELECTION))
#define GTK_IS_TREE_SELECTION_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TREE_SELECTION))
#define GTK_TREE_SELECTION_GET_CLASS(obj)	(G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TREE_SELECTION, GtkTreeSelectionClass))

typedef gboolean (* GtkTreeSelectionFunc)    (GtkTreeSelection  *selection,
					      GtkTreeModel      *model,
					      GtkTreePath       *path,
                                              gboolean           path_currently_selected,
					      gpointer           data);
typedef void (* GtkTreeSelectionForeachFunc) (GtkTreeModel      *model,
					      GtkTreePath       *path,
					      GtkTreeIter       *iter,
					      gpointer           data);

/********************************************************************/
struct _GtkTreeSelectionProps
{


  /*< private >*/

  GtkTreeView * (tree_view);
  GtkSelectionMode  (type);
  GtkTreeSelectionFunc  (user_func);
  gpointer  (user_data);
  GDestroyNotify  (destroy);
};

struct _GtkTreeSelectionFat
{
  GObject   parent;

  struct _GtkTreeSelectionProps instance_properties;
};

struct _GtkTreeSelectionThin
{
  GObject  parent;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkTreeSelectionFat   fat_instance;
  struct _GtkTreeSelectionThin  thin_instance;
}   GtkTreeSelection;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkTreeSelectionFat GtkTreeSelection;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkTreeSelectionThin GtkTreeSelection;
#endif
/********************************************************************/



struct _GtkTreeSelectionClass
{
  GObjectClass parent_class;

  void (* changed) (GtkTreeSelection *selection);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GType            _T2_gtk_tree_selection_get_type            (void) G_GNUC_CONST;
GType            _3T_gtk_tree_selection_get_type            (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType            gtk_tree_selection_get_type            (void) G_GNUC_CONST;

void             SF(gtk_tree_selection_set_mode)            (GtkTreeSelection            *selection,
							 GtkSelectionMode             type);
GtkSelectionMode SF(gtk_tree_selection_get_mode)        (GtkTreeSelection            *selection);
void             SF(gtk_tree_selection_set_select_function) (GtkTreeSelection            *selection,
							 GtkTreeSelectionFunc         func,
							 gpointer                     data,
							 GDestroyNotify               destroy);
gpointer         SF(gtk_tree_selection_get_user_data)       (GtkTreeSelection            *selection);
GtkTreeView*     SF(gtk_tree_selection_get_tree_view)       (GtkTreeSelection            *selection);

GtkTreeSelectionFunc SF(gtk_tree_selection_get_select_function) (GtkTreeSelection        *selection);

/* Only meaningful if GTK_SELECTION_SINGLE or GTK_SELECTION_BROWSE is set */
/* Use selected_foreach or get_selected_rows for GTK_SELECTION_MULTIPLE */
gboolean         SF(gtk_tree_selection_get_selected)        (GtkTreeSelection            *selection,
							 GtkTreeModel               **model,
							 GtkTreeIter                 *iter);
GList *          SF(gtk_tree_selection_get_selected_rows)   (GtkTreeSelection            *selection,
                                                         GtkTreeModel               **model);
gint             SF(gtk_tree_selection_count_selected_rows) (GtkTreeSelection            *selection);
void             SF(gtk_tree_selection_selected_foreach)    (GtkTreeSelection            *selection,
							 GtkTreeSelectionForeachFunc  func,
							 gpointer                     data);
void             SF(gtk_tree_selection_select_path)         (GtkTreeSelection            *selection,
							 GtkTreePath                 *path);
void             SF(gtk_tree_selection_unselect_path)       (GtkTreeSelection            *selection,
							 GtkTreePath                 *path);
void             SF(gtk_tree_selection_select_iter)         (GtkTreeSelection            *selection,
							 GtkTreeIter                 *iter);
void             SF(gtk_tree_selection_unselect_iter)       (GtkTreeSelection            *selection,
							 GtkTreeIter                 *iter);
gboolean         SF(gtk_tree_selection_path_is_selected)    (GtkTreeSelection            *selection,
							 GtkTreePath                 *path);
gboolean         SF(gtk_tree_selection_iter_is_selected)    (GtkTreeSelection            *selection,
							 GtkTreeIter                 *iter);
void             SF(gtk_tree_selection_select_all)          (GtkTreeSelection            *selection);
void             SF(gtk_tree_selection_unselect_all)        (GtkTreeSelection            *selection);
void             SF(gtk_tree_selection_select_range)        (GtkTreeSelection            *selection,
							 GtkTreePath                 *start_path,
							 GtkTreePath                 *end_path);
void             SF(gtk_tree_selection_unselect_range)      (GtkTreeSelection            *selection,
                                                         GtkTreePath                 *start_path,
							 GtkTreePath                 *end_path);


G_END_DECLS

#endif /* __GTK_TREE_SELECTION_H__ */
