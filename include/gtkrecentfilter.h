/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtkrecentfilter.h - Filter object for recently used resources
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

#ifndef __GTK_RECENT_FILTER_H__
#define __GTK_RECENT_FILTER_H__

#include <stlwrt.h>


#include <glib-object.h>

G_BEGIN_DECLS

#define GTK_TYPE_RECENT_FILTER		(gtk_recent_filter_get_type ())
#define GTK_RECENT_FILTER(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_RECENT_FILTER, GtkRecentFilter))
#define GTK_IS_RECENT_FILTER(obj)	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_RECENT_FILTER))

typedef struct _GtkRecentFilterInfo	GtkRecentFilterInfo;

typedef enum  {
  GTK_RECENT_FILTER_URI          = 1 << 0,
  GTK_RECENT_FILTER_DISPLAY_NAME = 1 << 1,
  GTK_RECENT_FILTER_MIME_TYPE    = 1 << 2,
  GTK_RECENT_FILTER_APPLICATION  = 1 << 3,
  GTK_RECENT_FILTER_GROUP        = 1 << 4,
  GTK_RECENT_FILTER_AGE          = 1 << 5
} GtkRecentFilterFlags;

typedef gboolean (*GtkRecentFilterFunc) (const GtkRecentFilterInfo *filter_info,
					 gpointer                   user_data);

struct _GtkRecentFilterInfo
{
  GtkRecentFilterFlags   contains;

  const gchar *uri;
  const gchar *display_name;
  const gchar *mime_type;
  const gchar **applications;
  const gchar **groups;

  gint age;
};

STLWRT_DECLARE_OPAQUE_TYPE(GtkRecentFilter, gtk_recent_filter)

GtkRecentFilter *     SF(gtk_recent_filter_new)      (void);
void                  SF(gtk_recent_filter_set_name) (GtkRecentFilter *filter,
						  const gchar     *name);
const gchar *         SF(gtk_recent_filter_get_name) (GtkRecentFilter *filter);

void SF(gtk_recent_filter_add_mime_type)      (GtkRecentFilter      *filter,
					   const gchar          *mime_type);
void SF(gtk_recent_filter_add_pattern)        (GtkRecentFilter      *filter,
					   const gchar          *pattern);
void SF(gtk_recent_filter_add_pixbuf_formats) (GtkRecentFilter      *filter);
void SF(gtk_recent_filter_add_application)    (GtkRecentFilter      *filter,
					   const gchar          *application);
void SF(gtk_recent_filter_add_group)          (GtkRecentFilter      *filter,
					   const gchar          *group);
void SF(gtk_recent_filter_add_age)            (GtkRecentFilter      *filter,
					   gint                  days);
void SF(gtk_recent_filter_add_custom)         (GtkRecentFilter      *filter,
					   GtkRecentFilterFlags  needed,
					   GtkRecentFilterFunc   func,
					   gpointer              data,
					   GDestroyNotify        data_destroy);

GtkRecentFilterFlags SF(gtk_recent_filter_get_needed) (GtkRecentFilter           *filter);
gboolean             SF(gtk_recent_filter_filter)     (GtkRecentFilter           *filter,
						   const GtkRecentFilterInfo *filter_info);

G_END_DECLS

#endif /* ! __GTK_RECENT_FILTER_H__ */
