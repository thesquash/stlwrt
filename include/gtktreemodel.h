
/* gtktreemodel.h
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

#ifndef __GTK_TREE_MODEL_H__
#define __GTK_TREE_MODEL_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define GTK_TYPE_TREE_MODEL            (gtk_tree_model_get_type ())
#define GTK_TREE_MODEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TREE_MODEL, GtkTreeModel))
#define GTK_IS_TREE_MODEL(obj)	       (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TREE_MODEL))
#define GTK_TREE_MODEL_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), GTK_TYPE_TREE_MODEL, GtkTreeModelIface))

#define GTK_TYPE_TREE_ITER             (gtk_tree_iter_get_type ())
#define GTK_TYPE_TREE_PATH             (gtk_tree_path_get_type ())
#define GTK_TYPE_TREE_ROW_REFERENCE    (gtk_tree_row_reference_get_type ())

typedef struct _GtkTreeIter         GtkTreeIter;
typedef struct _GtkTreePath         GtkTreePath;
typedef struct _GtkTreeRowReference GtkTreeRowReference;
typedef struct _GtkTreeModel        GtkTreeModel; /* Dummy typedef */
typedef struct _GtkTreeModelIface   GtkTreeModelIface;
typedef gboolean (* GtkTreeModelForeachFunc) (GtkTreeModel *model, GtkTreePath *path, GtkTreeIter *iter, gpointer data);


typedef enum
{
  GTK_TREE_MODEL_ITERS_PERSIST = 1 << 0,
  GTK_TREE_MODEL_LIST_ONLY = 1 << 1
} GtkTreeModelFlags;

struct _GtkTreeIter
{
  gint stamp;
  gpointer user_data;
  gpointer user_data2;
  gpointer user_data3;
};

struct _GtkTreeModelIface
{
  GTypeInterface g_iface;

  /* Signals */
  void         (* row_changed)           (GtkTreeModel *tree_model,
					  GtkTreePath  *path,
					  GtkTreeIter  *iter);
  void         (* row_inserted)          (GtkTreeModel *tree_model,
					  GtkTreePath  *path,
					  GtkTreeIter  *iter);
  void         (* row_has_child_toggled) (GtkTreeModel *tree_model,
					  GtkTreePath  *path,
					  GtkTreeIter  *iter);
  void         (* row_deleted)           (GtkTreeModel *tree_model,
					  GtkTreePath  *path);
  void         (* rows_reordered)        (GtkTreeModel *tree_model,
					  GtkTreePath  *path,
					  GtkTreeIter  *iter,
					  gint         *new_order);

  /* Virtual Table */
  GtkTreeModelFlags (* get_flags)  (GtkTreeModel *tree_model);

  gint         (* get_n_columns)   (GtkTreeModel *tree_model);
  GType        (* get_column_type) (GtkTreeModel *tree_model,
				    gint          index_);
  gboolean     (* get_iter)        (GtkTreeModel *tree_model,
				    GtkTreeIter  *iter,
				    GtkTreePath  *path);
  GtkTreePath *(* get_path)        (GtkTreeModel *tree_model,
				    GtkTreeIter  *iter);
  void         (* get_value)       (GtkTreeModel *tree_model,
				    GtkTreeIter  *iter,
				    gint          column,
				    GValue       *value);
  gboolean     (* iter_next)       (GtkTreeModel *tree_model,
				    GtkTreeIter  *iter);
  gboolean     (* iter_children)   (GtkTreeModel *tree_model,
				    GtkTreeIter  *iter,
				    GtkTreeIter  *parent);
  gboolean     (* iter_has_child)  (GtkTreeModel *tree_model,
				    GtkTreeIter  *iter);
  gint         (* iter_n_children) (GtkTreeModel *tree_model,
				    GtkTreeIter  *iter);
  gboolean     (* iter_nth_child)  (GtkTreeModel *tree_model,
				    GtkTreeIter  *iter,
				    GtkTreeIter  *parent,
				    gint          n);
  gboolean     (* iter_parent)     (GtkTreeModel *tree_model,
				    GtkTreeIter  *iter,
				    GtkTreeIter  *child);
  void         (* ref_node)        (GtkTreeModel *tree_model,
				    GtkTreeIter  *iter);
  void         (* unref_node)      (GtkTreeModel *tree_model,
				    GtkTreeIter  *iter);
};


/* GtkTreePath operations */
GtkTreePath *SF(gtk_tree_path_new)              (void);
GtkTreePath *SF(gtk_tree_path_new_from_string)  (const gchar       *path);
GtkTreePath *SF(gtk_tree_path_new_from_indices) (gint               first_index,
					     ...);
gchar       *SF(gtk_tree_path_to_string)        (GtkTreePath       *path);
GtkTreePath *SF(gtk_tree_path_new_first)        (void);
void         SF(gtk_tree_path_append_index)     (GtkTreePath       *path,
					     gint               index_);
void         SF(gtk_tree_path_prepend_index)    (GtkTreePath       *path,
					     gint               index_);
gint         SF(gtk_tree_path_get_depth)        (GtkTreePath       *path);
gint        *SF(gtk_tree_path_get_indices)      (GtkTreePath       *path);
gint        *SF(gtk_tree_path_get_indices_with_depth) (GtkTreePath *path,
                                                   gint        *depth);
void         SF(gtk_tree_path_free)             (GtkTreePath       *path);
GtkTreePath *SF(gtk_tree_path_copy)             (const GtkTreePath *path);
GType        _T2_gtk_tree_path_get_type         (void) G_GNUC_CONST;
GType        _3T_gtk_tree_path_get_type         (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType        gtk_tree_path_get_type         (void) G_GNUC_CONST;
gint         SF(gtk_tree_path_compare)          (const GtkTreePath *a,
					     const GtkTreePath *b);
void         SF(gtk_tree_path_next)             (GtkTreePath       *path);
gboolean     SF(gtk_tree_path_prev)             (GtkTreePath       *path);
gboolean     SF(gtk_tree_path_up)               (GtkTreePath       *path);
void         SF(gtk_tree_path_down)             (GtkTreePath       *path);

gboolean     SF(gtk_tree_path_is_ancestor)      (GtkTreePath       *path,
                                             GtkTreePath       *descendant);
gboolean     SF(gtk_tree_path_is_descendant)    (GtkTreePath       *path,
                                             GtkTreePath       *ancestor);

#ifndef GTK_DISABLE_DEPRECATED
#define SF(gtk_tree_path_new_root)() SF(gtk_tree_path_new_first)()
#endif /* !GTK_DISABLE_DEPRECATED */

/* Row reference (an object that tracks model changes so it refers to the same
 * row always; a path refers to a position, not a fixed row).  You almost always
 * want to call gtk_tree_row_reference_new.
 */

GType                _T2_gtk_tree_row_reference_get_type (void) G_GNUC_CONST;
GType                _3T_gtk_tree_row_reference_get_type (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType                gtk_tree_row_reference_get_type (void) G_GNUC_CONST;
GtkTreeRowReference *SF(gtk_tree_row_reference_new)       (GtkTreeModel        *model,
						       GtkTreePath         *path);
GtkTreeRowReference *SF(gtk_tree_row_reference_new_proxy) (GObject             *proxy,
						       GtkTreeModel        *model,
						       GtkTreePath         *path);
GtkTreePath         *SF(gtk_tree_row_reference_get_path)  (GtkTreeRowReference *reference);
GtkTreeModel        *SF(gtk_tree_row_reference_get_model) (GtkTreeRowReference *reference);
gboolean             SF(gtk_tree_row_reference_valid)     (GtkTreeRowReference *reference);
GtkTreeRowReference *SF(gtk_tree_row_reference_copy)      (GtkTreeRowReference *reference);
void                 SF(gtk_tree_row_reference_free)      (GtkTreeRowReference *reference);
/* These two functions are only needed if you created the row reference with a
 * proxy object */
void                 SF(gtk_tree_row_reference_inserted)  (GObject     *proxy,
						       GtkTreePath *path);
void                 SF(gtk_tree_row_reference_deleted)   (GObject     *proxy,
						       GtkTreePath *path);
void                 SF(gtk_tree_row_reference_reordered) (GObject     *proxy,
						       GtkTreePath *path,
						       GtkTreeIter *iter,
						       gint        *new_order);

/* GtkTreeIter operations */
GtkTreeIter *     SF(gtk_tree_iter_copy)             (GtkTreeIter  *iter);
void              SF(gtk_tree_iter_free)             (GtkTreeIter  *iter);
GType             _T2_gtk_tree_iter_get_type         (void) G_GNUC_CONST;
GType             _3T_gtk_tree_iter_get_type         (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType             gtk_tree_iter_get_type         (void) G_GNUC_CONST;

GType             _T2_gtk_tree_model_get_type        (void) G_GNUC_CONST;
GType             _3T_gtk_tree_model_get_type        (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType             gtk_tree_model_get_type        (void) G_GNUC_CONST;
GtkTreeModelFlags SF(gtk_tree_model_get_flags)       (GtkTreeModel *tree_model);
gint              SF(gtk_tree_model_get_n_columns)   (GtkTreeModel *tree_model);
GType             SF(gtk_tree_model_get_column_type) (GtkTreeModel *tree_model,
						  gint          index_);


/* Iterator movement */
gboolean          SF(gtk_tree_model_get_iter)        (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter,
						  GtkTreePath  *path);
gboolean          SF(gtk_tree_model_get_iter_from_string) (GtkTreeModel *tree_model,
						       GtkTreeIter  *iter,
						       const gchar  *path_string);
gchar *           SF(gtk_tree_model_get_string_from_iter) (GtkTreeModel *tree_model,
                                                       GtkTreeIter  *iter);
gboolean          SF(gtk_tree_model_get_iter_first)  (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter);
GtkTreePath *     SF(gtk_tree_model_get_path)        (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter);
void              SF(gtk_tree_model_get_value)       (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter,
						  gint          column,
						  GValue       *value);
gboolean          SF(gtk_tree_model_iter_next)       (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter);
gboolean          SF(gtk_tree_model_iter_children)   (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter,
						  GtkTreeIter  *parent);
gboolean          SF(gtk_tree_model_iter_has_child)  (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter);
gint              SF(gtk_tree_model_iter_n_children) (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter);
gboolean          SF(gtk_tree_model_iter_nth_child)  (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter,
						  GtkTreeIter  *parent,
						  gint          n);
gboolean          SF(gtk_tree_model_iter_parent)     (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter,
						  GtkTreeIter  *child);
void              SF(gtk_tree_model_ref_node)        (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter);
void              SF(gtk_tree_model_unref_node)      (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter);
void              SF(gtk_tree_model_get)             (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter,
						  ...);
void              SF(gtk_tree_model_get_valist)      (GtkTreeModel *tree_model,
						  GtkTreeIter  *iter,
						  va_list       var_args);


void              SF(gtk_tree_model_foreach)         (GtkTreeModel            *model,
						  GtkTreeModelForeachFunc  func,
						  gpointer                 user_data);


#ifndef GTK_DISABLE_DEPRECATED
#define SF(gtk_tree_model_get_iter_root)(tree_model, iter) SF(gtk_tree_model_get_iter_first)(tree_model, iter)
#endif /* !GTK_DISABLE_DEPRECATED */

/* Signals */
void SF(gtk_tree_model_row_changed)           (GtkTreeModel *tree_model,
					   GtkTreePath  *path,
					   GtkTreeIter  *iter);
void SF(gtk_tree_model_row_inserted)          (GtkTreeModel *tree_model,
					   GtkTreePath  *path,
					   GtkTreeIter  *iter);
void SF(gtk_tree_model_row_has_child_toggled) (GtkTreeModel *tree_model,
					   GtkTreePath  *path,
					   GtkTreeIter  *iter);
void SF(gtk_tree_model_row_deleted)           (GtkTreeModel *tree_model,
					   GtkTreePath  *path);
void SF(gtk_tree_model_rows_reordered)        (GtkTreeModel *tree_model,
					   GtkTreePath  *path,
					   GtkTreeIter  *iter,
					   gint         *new_order);

G_END_DECLS

#endif /* __GTK_TREE_MODEL_H__ */
