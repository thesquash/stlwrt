/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtkrecentchooser.h - Abstract interface for recent file selectors GUIs
 *
 * Copyright (C) 2006, Emmanuele Bassi
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

#ifndef __GTK_RECENT_CHOOSER_H__
#define __GTK_RECENT_CHOOSER_H__

#include <stlwrt.h>


#include <gtkwidget.h>

#include <gtkrecentmanager.h>

#include <gtkrecentfilter.h>

G_BEGIN_DECLS

#define GTK_TYPE_RECENT_CHOOSER			(gtk_recent_chooser_get_type ())
#define GTK_RECENT_CHOOSER(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_RECENT_CHOOSER, GtkRecentChooser))
#define GTK_IS_RECENT_CHOOSER(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_RECENT_CHOOSER))
#define GTK_RECENT_CHOOSER_GET_IFACE(inst)	(G_TYPE_INSTANCE_GET_INTERFACE ((inst), GTK_TYPE_RECENT_CHOOSER, GtkRecentChooserIface))

/**
 * GtkRecentSortType:
 * @GTK_RECENT_SORT_NONE: Do not sort the returned list of recently used
 *   resources.
 * @GTK_RECENT_SORT_MRU: Sort the returned list with the most recently used
 *   items first.
 * @GTK_RECENT_SORT_LRU: Sort the returned list with the least recently used
 *   items first.
 * @GTK_RECENT_SORT_CUSTOM: Sort the returned list using a custom sorting
 *   function passed using SF(gtk_recent_manager_set_sort_func)().
 *
 * Used to specify the sorting method to be applyed to the recently
 * used resource list.
 **/
typedef enum 
{
  GTK_RECENT_SORT_NONE = 0,
  GTK_RECENT_SORT_MRU,
  GTK_RECENT_SORT_LRU,
  GTK_RECENT_SORT_CUSTOM
} GtkRecentSortType;

typedef gint (*GtkRecentSortFunc) (GtkRecentInfo *a,
				   GtkRecentInfo *b,
				   gpointer       user_data);

#define GTK_RECENT_CHOOSER_ERROR	(SF(gtk_recent_chooser_error_quark) ())

typedef enum 
{
  GTK_RECENT_CHOOSER_ERROR_NOT_FOUND,
  GTK_RECENT_CHOOSER_ERROR_INVALID_URI
} GtkRecentChooserError;

GQuark  SF(gtk_recent_chooser_error_quark) (void);


STLWRT_DECLARE_INTERFACE(GtkRecentChooser, gtk_recent_chooser,
  /*
   * Methods
   */
  gboolean          (* set_current_uri)    (GtkRecentChooser  *chooser,
  					    const gchar       *uri,
  					    GError           **error);
  gchar *           (* get_current_uri)    (GtkRecentChooser  *chooser);
  gboolean          (* select_uri)         (GtkRecentChooser  *chooser,
  					    const gchar       *uri,
  					    GError           **error);
  void              (* unselect_uri)       (GtkRecentChooser  *chooser,
                                            const gchar       *uri);
  void              (* select_all)         (GtkRecentChooser  *chooser);
  void              (* unselect_all)       (GtkRecentChooser  *chooser);
  GList *           (* get_items)          (GtkRecentChooser  *chooser);
  GtkRecentManager *(* get_recent_manager) (GtkRecentChooser  *chooser);
  void              (* add_filter)         (GtkRecentChooser  *chooser,
  					    GtkRecentFilter   *filter);
  void              (* remove_filter)      (GtkRecentChooser  *chooser,
  					    GtkRecentFilter   *filter);
  GSList *          (* list_filters)       (GtkRecentChooser  *chooser);
  void              (* set_sort_func)      (GtkRecentChooser  *chooser,
  					    GtkRecentSortFunc  sort_func,
  					    gpointer           data,
  					    GDestroyNotify     destroy);

  /*
   * Signals
   */
  void		    (* item_activated)     (GtkRecentChooser  *chooser);
  void		    (* selection_changed)  (GtkRecentChooser  *chooser);
)


/*
 * Configuration
 */
void              SF(gtk_recent_chooser_set_show_private)    (GtkRecentChooser  *chooser,
							  gboolean           show_private);
gboolean          SF(gtk_recent_chooser_get_show_private)    (GtkRecentChooser  *chooser);
void              SF(gtk_recent_chooser_set_show_not_found)  (GtkRecentChooser  *chooser,
							  gboolean           show_not_found);
gboolean          SF(gtk_recent_chooser_get_show_not_found)  (GtkRecentChooser  *chooser);
void              SF(gtk_recent_chooser_set_select_multiple) (GtkRecentChooser  *chooser,
							  gboolean           select_multiple);
gboolean          SF(gtk_recent_chooser_get_select_multiple) (GtkRecentChooser  *chooser);
void              SF(gtk_recent_chooser_set_limit)           (GtkRecentChooser  *chooser,
							  gint               limit);
gint              SF(gtk_recent_chooser_get_limit)           (GtkRecentChooser  *chooser);
void              SF(gtk_recent_chooser_set_local_only)      (GtkRecentChooser  *chooser,
							  gboolean           local_only);
gboolean          SF(gtk_recent_chooser_get_local_only)      (GtkRecentChooser  *chooser);
void              SF(gtk_recent_chooser_set_show_tips)       (GtkRecentChooser  *chooser,
							  gboolean           show_tips);
gboolean          SF(gtk_recent_chooser_get_show_tips)       (GtkRecentChooser  *chooser);
#ifndef GTK_DISABLE_DEPRECATED
void              SF(gtk_recent_chooser_set_show_numbers)    (GtkRecentChooser  *chooser,
							  gboolean           show_numbers);
gboolean          SF(gtk_recent_chooser_get_show_numbers)    (GtkRecentChooser  *chooser);
#endif /* GTK_DISABLE_DEPRECATED */
void              SF(gtk_recent_chooser_set_show_icons)      (GtkRecentChooser  *chooser,
							  gboolean           show_icons);
gboolean          SF(gtk_recent_chooser_get_show_icons)      (GtkRecentChooser  *chooser);
void              SF(gtk_recent_chooser_set_sort_type)       (GtkRecentChooser  *chooser,
							  GtkRecentSortType  sort_type);
GtkRecentSortType SF(gtk_recent_chooser_get_sort_type)       (GtkRecentChooser  *chooser);
void              SF(gtk_recent_chooser_set_sort_func)       (GtkRecentChooser  *chooser,
							  GtkRecentSortFunc  sort_func,
							  gpointer           sort_data,
							  GDestroyNotify     data_destroy);

/*
 * Items handling
 */
gboolean       SF(gtk_recent_chooser_set_current_uri)  (GtkRecentChooser  *chooser,
						    const gchar       *uri,
						    GError           **error);
gchar *        SF(gtk_recent_chooser_get_current_uri)  (GtkRecentChooser  *chooser);
GtkRecentInfo *SF(gtk_recent_chooser_get_current_item) (GtkRecentChooser  *chooser);
gboolean       SF(gtk_recent_chooser_select_uri)       (GtkRecentChooser  *chooser,
						    const gchar       *uri,
						    GError           **error);
void           SF(gtk_recent_chooser_unselect_uri)     (GtkRecentChooser  *chooser,
					            const gchar       *uri);
void           SF(gtk_recent_chooser_select_all)       (GtkRecentChooser  *chooser);
void           SF(gtk_recent_chooser_unselect_all)     (GtkRecentChooser  *chooser);
GList *        SF(gtk_recent_chooser_get_items)        (GtkRecentChooser  *chooser);
gchar **       SF(gtk_recent_chooser_get_uris)         (GtkRecentChooser  *chooser,
						    gsize             *length);

/*
 * Filters
 */
void 		 SF(gtk_recent_chooser_add_filter)    (GtkRecentChooser *chooser,
			 			   GtkRecentFilter  *filter);
void 		 SF(gtk_recent_chooser_remove_filter) (GtkRecentChooser *chooser,
						   GtkRecentFilter  *filter);
GSList * 	 SF(gtk_recent_chooser_list_filters)  (GtkRecentChooser *chooser);
void 		 SF(gtk_recent_chooser_set_filter)    (GtkRecentChooser *chooser,
						   GtkRecentFilter  *filter);
GtkRecentFilter *SF(gtk_recent_chooser_get_filter)    (GtkRecentChooser *chooser);


G_END_DECLS

#endif /* __GTK_RECENT_CHOOSER_H__ */
