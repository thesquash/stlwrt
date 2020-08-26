/* gtkliststore.h
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

#ifndef __GTK_LIST_STORE_H__
#define __GTK_LIST_STORE_H__

#include <gdkconfig.h>
#include <gtktreemodel.h>
#include <gtktreesortable.h>


G_BEGIN_DECLS


#define GTK_TYPE_LIST_STORE	       (gtk_list_store_get_type ())
#define GTK_LIST_STORE(obj)	       (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_LIST_STORE, GtkListStore))
#define GTK_LIST_STORE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_LIST_STORE, GtkListStoreClass))
#define GTK_IS_LIST_STORE(obj)	       (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_LIST_STORE))
#define GTK_IS_LIST_STORE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_LIST_STORE))
#define GTK_LIST_STORE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_LIST_STORE, GtkListStoreClass))

typedef struct _GtkListStore       GtkListStoreFat;
typedef struct _GtkListStore       GtkListStoreThin;

typedef struct _GtkListStoreClass  GtkListStoreClass;

/********************************************************************/
struct _GtkListStoreProps
{


  /*< private >*/
  gint  (stamp);
  gpointer  (seq);		/* head of the list */
  gpointer  (_gtk_reserved1);
  GList * (sort_list);
  gint  (n_columns);
  gint  (sort_column_id);
  GtkSortType  (order);
  GType * (column_headers);
  gint  (length);
  GtkTreeIterCompareFunc  (default_sort_func);
  gpointer  (default_sort_data);
  GDestroyNotify  (default_sort_destroy);
  guint  (columns_dirty) : 1;
};

struct _GtkListStoreFat
{
  GObject   parent;

  struct _GtkListStoreProps instance_properties;
};

struct _GtkListStoreThin
{
  GObject  parent;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkListStoreFat   fat_instance;
  struct _GtkListStoreThin  thin_instance;
}   GtkListStore;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkListStoreFat GtkListStore;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkListStoreThin GtkListStore;
#endif
/********************************************************************/



struct _GtkListStoreClass
{
  GObjectClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GType         _T2_gtk_list_store_get_type         (void) G_GNUC_CONST;
GType         _3T_gtk_list_store_get_type         (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType         gtk_list_store_get_type         (void) G_GNUC_CONST;
GtkListStore *SF(gtk_list_store_new)              (gint          n_columns,
					       ...);
GtkListStore *SF(gtk_list_store_newv)             (gint          n_columns,
					       GType        *types);
void          SF(gtk_list_store_set_column_types) (GtkListStore *list_store,
					       gint          n_columns,
					       GType        *types);

/* NOTE: use gtk_tree_model_get to get values from a GtkListStore */

void          SF(gtk_list_store_set_value)        (GtkListStore *list_store,
					       GtkTreeIter  *iter,
					       gint          column,
					       GValue       *value);
void          SF(gtk_list_store_set)              (GtkListStore *list_store,
					       GtkTreeIter  *iter,
					       ...);
void          SF(gtk_list_store_set_valuesv)      (GtkListStore *list_store,
					       GtkTreeIter  *iter,
					       gint         *columns,
					       GValue       *values,
					       gint          n_values);
void          SF(gtk_list_store_set_valist)       (GtkListStore *list_store,
					       GtkTreeIter  *iter,
					       va_list       var_args);
gboolean      SF(gtk_list_store_remove)           (GtkListStore *list_store,
					       GtkTreeIter  *iter);
void          SF(gtk_list_store_insert)           (GtkListStore *list_store,
					       GtkTreeIter  *iter,
					       gint          position);
void          SF(gtk_list_store_insert_before)    (GtkListStore *list_store,
					       GtkTreeIter  *iter,
					       GtkTreeIter  *sibling);
void          SF(gtk_list_store_insert_after)     (GtkListStore *list_store,
					       GtkTreeIter  *iter,
					       GtkTreeIter  *sibling);
void          SF(gtk_list_store_insert_with_values)  (GtkListStore *list_store,
						  GtkTreeIter  *iter,
						  gint          position,
						  ...);
void          SF(gtk_list_store_insert_with_valuesv) (GtkListStore *list_store,
						  GtkTreeIter  *iter,
						  gint          position,
						  gint         *columns,
						  GValue       *values,
						  gint          n_values);
void          SF(gtk_list_store_prepend)          (GtkListStore *list_store,
					       GtkTreeIter  *iter);
void          SF(gtk_list_store_append)           (GtkListStore *list_store,
					       GtkTreeIter  *iter);
void          SF(gtk_list_store_clear)            (GtkListStore *list_store);
gboolean      SF(gtk_list_store_iter_is_valid)    (GtkListStore *list_store,
                                               GtkTreeIter  *iter);
void          SF(gtk_list_store_reorder)          (GtkListStore *store,
                                               gint         *new_order);
void          SF(gtk_list_store_swap)             (GtkListStore *store,
                                               GtkTreeIter  *a,
                                               GtkTreeIter  *b);
void          SF(gtk_list_store_move_after)       (GtkListStore *store,
                                               GtkTreeIter  *iter,
                                               GtkTreeIter  *position);
void          SF(gtk_list_store_move_before)      (GtkListStore *store,
                                               GtkTreeIter  *iter,
                                               GtkTreeIter  *position);


G_END_DECLS


#endif /* __GTK_LIST_STORE_H__ */
