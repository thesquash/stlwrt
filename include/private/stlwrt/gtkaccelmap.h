
/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1998, 2001 Tim Janik
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_ACCEL_MAP_H__
#define __GTK_ACCEL_MAP_H__


#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtkaccelgroup.h>

G_BEGIN_DECLS

/* --- global GtkAccelMap object --- */
#define GTK_TYPE_ACCEL_MAP                (gtk_accel_map_get_type ())
#define GTK_ACCEL_MAP(accel_map)	  (G_TYPE_CHECK_INSTANCE_CAST ((accel_map), GTK_TYPE_ACCEL_MAP, GtkAccelMap))
#define GTK_ACCEL_MAP_CLASS(klass)	  (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_ACCEL_MAP, GtkAccelMapClass))
#define GTK_IS_ACCEL_MAP(accel_map)	  (G_TYPE_CHECK_INSTANCE_TYPE ((accel_map), GTK_TYPE_ACCEL_MAP))
#define GTK_IS_ACCEL_MAP_CLASS(klass)	  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_ACCEL_MAP))
#define GTK_ACCEL_MAP_GET_CLASS(obj)      (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_ACCEL_MAP, GtkAccelMapClass))

typedef struct _GtkAccelMap      GtkAccelMapFat;
typedef struct _GtkAccelMap      GtkAccelMapThin;
typedef struct _GtkAccelMapClass GtkAccelMapClassFat;
typedef struct _GtkAccelMapClass GtkAccelMapClassThin;

/* --- notifier --- */
typedef void (*GtkAccelMapForeach)		(gpointer	 data,
						 const gchar	*accel_path,
						 guint           accel_key,
						 GdkModifierType accel_mods,
						 gboolean	 changed);


/* --- public API --- */

#ifdef G_OS_WIN32
/* Reserve old names for DLL ABI backward compatibility */
#define gtk_accel_map_load gtk_accel_map_load_utf8
#define gtk_accel_map_save gtk_accel_map_save_utf8
#endif

void	   __gtk_accel_map_add_entry	(const gchar		*accel_path,
					 guint			 accel_key,
					 GdkModifierType         accel_mods);
gboolean   __gtk_accel_map_lookup_entry	(const gchar		*accel_path,
					 GtkAccelKey		*key);
gboolean   __gtk_accel_map_change_entry	(const gchar		*accel_path,
					 guint			 accel_key,
					 GdkModifierType	 accel_mods,
					 gboolean		 replace);
void	   __gtk_accel_map_load		(const gchar		*file_name);
void	   __gtk_accel_map_save		(const gchar		*file_name);
void	   __gtk_accel_map_foreach	(gpointer		 data,
					 GtkAccelMapForeach	 foreach_func);
void	   __gtk_accel_map_load_fd	(gint			 fd);
void	   __gtk_accel_map_load_scanner	(GScanner		*scanner);
void	   __gtk_accel_map_save_fd	(gint			 fd);

void       __gtk_accel_map_lock_path      (const gchar            *accel_path);
void       __gtk_accel_map_unlock_path    (const gchar            *accel_path);

/* --- filter functions --- */
void	__gtk_accel_map_add_filter	 (const gchar		*filter_pattern);
void	__gtk_accel_map_foreach_unfiltered (gpointer		 data,
					  GtkAccelMapForeach	 foreach_func);

/* --- notification --- */
GType        _T2_gtk_accel_map_get_type (void) G_GNUC_CONST;
GType        _3T_gtk_accel_map_get_type (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType        gtk_accel_map_get_type (void) G_GNUC_CONST;
GtkAccelMap *__gtk_accel_map_get      (void);


/* --- internal API --- */
void		___gtk_accel_map_init		(void);

void            ___gtk_accel_map_add_group	 (const gchar   *accel_path,
						  GtkAccelGroup *accel_group);
void            ___gtk_accel_map_remove_group 	 (const gchar   *accel_path,
						  GtkAccelGroup *accel_group);
gboolean	___gtk_accel_path_is_valid	 (const gchar	*accel_path);


G_END_DECLS

#endif /* __GTK_ACCEL_MAP_H__ */
