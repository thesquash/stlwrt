/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2006-2007 Async Open Source,
 *                         Johan Dahlin <jdahlin@async.com.br>
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

#ifndef __GTK_BUILDER_H__
#define __GTK_BUILDER_H__

#include <stlwrt.h>


#include <gdkconfig.h>

#include <glib-object.h>

G_BEGIN_DECLS

#define GTK_TYPE_BUILDER                 (gtk_builder_get_type ())
#define GTK_BUILDER(obj)                 (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_BUILDER, GtkBuilder))
#define GTK_BUILDER_CLASS(klass)         (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_BUILDER, GtkBuilderClass))
#define GTK_IS_BUILDER(obj)              (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_BUILDER))
#define GTK_IS_BUILDER_CLASS(klass)      (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_BUILDER))
#define GTK_BUILDER_GET_CLASS(obj)       (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_BUILDER, GtkBuilderClass))

#define GTK_BUILDER_ERROR                (SF(gtk_builder_error_quark) ())

typedef struct _GtkBuilderClass   GtkBuilderClass;
typedef struct _GtkBuilderPrivate GtkBuilderPrivate;

typedef enum 
{
  GTK_BUILDER_ERROR_INVALID_TYPE_FUNCTION,
  GTK_BUILDER_ERROR_UNHANDLED_TAG,
  GTK_BUILDER_ERROR_MISSING_ATTRIBUTE,
  GTK_BUILDER_ERROR_INVALID_ATTRIBUTE,
  GTK_BUILDER_ERROR_INVALID_TAG,
  GTK_BUILDER_ERROR_MISSING_PROPERTY_VALUE,
  GTK_BUILDER_ERROR_INVALID_VALUE,
  GTK_BUILDER_ERROR_VERSION_MISMATCH,
  GTK_BUILDER_ERROR_DUPLICATE_ID
} GtkBuilderError;

GQuark SF(gtk_builder_error_quark) (void);


STLWRT_DECLARE_FTYPE_FPARENT(GtkBuilder, gtk_builder, GObject,
  GtkBuilderPrivate * (priv);
)

struct _GtkBuilderClass
{
  GObjectClass parent_class;
  
  GType (* get_type_from_name) (GtkBuilder *builder,
                                const char *type_name);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
  void (*_gtk_reserved5) (void);
  void (*_gtk_reserved6) (void);
  void (*_gtk_reserved7) (void);
  void (*_gtk_reserved8) (void);
};

typedef void (*GtkBuilderConnectFunc) (GtkBuilder    *builder,
				       GObject       *object,
				       const gchar   *signal_name,
				       const gchar   *handler_name,
				       GObject       *connect_object,
				       GConnectFlags  flags,
				       gpointer       user_data);


GtkBuilder*  SF(gtk_builder_new)                     (void);

guint        SF(gtk_builder_add_from_file)           (GtkBuilder    *builder,
                                                  const gchar   *filename,
                                                  GError       **error);
guint        SF(gtk_builder_add_from_string)         (GtkBuilder    *builder,
                                                  const gchar   *buffer,
                                                  gsize          length,
                                                  GError       **error);
guint        SF(gtk_builder_add_objects_from_file)   (GtkBuilder    *builder,
                                                  const gchar   *filename,
                                                  gchar        **object_ids,
                                                  GError       **error);
guint        SF(gtk_builder_add_objects_from_string) (GtkBuilder    *builder,
                                                  const gchar   *buffer,
                                                  gsize          length,
                                                  gchar        **object_ids,
                                                  GError       **error);
GObject*     SF(gtk_builder_get_object)              (GtkBuilder    *builder,
                                                  const gchar   *name);
GSList*      SF(gtk_builder_get_objects)             (GtkBuilder    *builder);
void         SF(gtk_builder_connect_signals)         (GtkBuilder    *builder,
						  gpointer       user_data);
void         SF(gtk_builder_connect_signals_full)    (GtkBuilder    *builder,
                                                  GtkBuilderConnectFunc func,
						  gpointer       user_data);
void         SF(gtk_builder_set_translation_domain)  (GtkBuilder   	*builder,
                                                  const gchar  	*domain);
const gchar* SF(gtk_builder_get_translation_domain)  (GtkBuilder   	*builder);
GType        SF(gtk_builder_get_type_from_name)      (GtkBuilder   	*builder,
                                                  const char   	*type_name);

gboolean     SF(gtk_builder_value_from_string)       (GtkBuilder    *builder,
						  GParamSpec   	*pspec,
                                                  const gchar  	*string,
                                                  GValue       	*value,
						  GError       **error);
gboolean     SF(gtk_builder_value_from_string_type)  (GtkBuilder    *builder,
						  GType        	 type,
                                                  const gchar  	*string,
                                                  GValue       	*value,
						  GError       **error);

#define GTK_BUILDER_WARN_INVALID_CHILD_TYPE(object, type) \
  g_warning ("'%s' is not a valid child type of '%s'", type, g_type_name (G_OBJECT_TYPE (object)))

G_END_DECLS

#endif /* __GTK_BUILDER_H__ */
