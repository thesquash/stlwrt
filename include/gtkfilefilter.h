/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtkfilefilter.h: Filters for selecting a file subset
 * Copyright (C) 2003, Red Hat, Inc.
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

#ifndef __GTK_FILE_FILTER_H__
#define __GTK_FILE_FILTER_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define GTK_TYPE_FILE_FILTER              (gtk_file_filter_get_type ())
#define GTK_FILE_FILTER(obj)              (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_FILE_FILTER, GtkFileFilter))
#define GTK_IS_FILE_FILTER(obj)           (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_FILE_FILTER))

typedef struct _GtkFileFilter     GtkFileFilterFat;
typedef struct _GtkFileFilter     GtkFileFilterThin;
typedef struct _GtkFileFilterInfo GtkFileFilterInfo;

typedef enum {
  GTK_FILE_FILTER_FILENAME     = 1 << 0,
  GTK_FILE_FILTER_URI          = 1 << 1,
  GTK_FILE_FILTER_DISPLAY_NAME = 1 << 2,
  GTK_FILE_FILTER_MIME_TYPE    = 1 << 3
} GtkFileFilterFlags;

typedef gboolean (*GtkFileFilterFunc) (const GtkFileFilterInfo *filter_info,
				       gpointer                 data);

/********************************************************************/
struct _GtkFileFilterInfoProps
{


  const gchar *filename;
  const gchar *uri;
  const gchar *display_name;
  const gchar *mime_type;
};

struct _GtkFileFilterInfoFat
{
  GtkFileFilterFlagsFat   contains;

  struct _GtkFileFilterInfoProps instance_properties;
};

struct _GtkFileFilterInfoThin
{
  GtkFileFilterFlagsThin  contains;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkFileFilterInfoFat   fat_instance;
  struct _GtkFileFilterInfoThin  thin_instance;
}   GtkFileFilterInfo;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkFileFilterInfoFat GtkFileFilterInfo;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkFileFilterInfoThin GtkFileFilterInfo;
#endif
/********************************************************************/



GType _T2_gtk_file_filter_get_type (void) G_GNUC_CONST;
GType _3T_gtk_file_filter_get_type (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType gtk_file_filter_get_type (void) G_GNUC_CONST;

GtkFileFilter *       SF(gtk_file_filter_new)      (void);
void                  SF(gtk_file_filter_set_name) (GtkFileFilter *filter,
						const gchar   *name);
const gchar *         SF(gtk_file_filter_get_name) (GtkFileFilter *filter);

void SF(gtk_file_filter_add_mime_type)      (GtkFileFilter      *filter,
					 const gchar        *mime_type);
void SF(gtk_file_filter_add_pattern)        (GtkFileFilter      *filter,
					 const gchar        *pattern);
void SF(gtk_file_filter_add_pixbuf_formats) (GtkFileFilter      *filter);
void SF(gtk_file_filter_add_custom)         (GtkFileFilter      *filter,
					 GtkFileFilterFlags  needed,
					 GtkFileFilterFunc   func,
					 gpointer            data,
					 GDestroyNotify      notify);

GtkFileFilterFlags SF(gtk_file_filter_get_needed) (GtkFileFilter           *filter);
gboolean           SF(gtk_file_filter_filter)     (GtkFileFilter           *filter,
					       const GtkFileFilterInfo *filter_info);

G_END_DECLS

#endif /* __GTK_FILE_FILTER_H__ */
