/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2000 Red Hat, Inc.
 *               2008 Johan Dahlin
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

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/gtk/.
 */

#include "config.h"

#include <stlwrt.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <gtkiconfactory.h>
#include <gtkiconcache.h>
#include <gtkdebug.h>
#include <gtkicontheme.h>
#include <gtksettings.h>
#include <gtkstock.h>
#include <gtkwidget.h>
#include <gtkintl.h>
#include <gtkbuildable.h>
#include <gtkbuilderprivate.h>



static GSList *all_icon_factories = NULL;

typedef enum {
  GTK_ICON_SOURCE_EMPTY,
  GTK_ICON_SOURCE_ICON_NAME,
  GTK_ICON_SOURCE_STATIC_ICON_NAME,
  GTK_ICON_SOURCE_FILENAME,
  GTK_ICON_SOURCE_PIXBUF
} GtkIconSourceType;

struct _GtkIconSource
{
  GtkIconSourceType type;

  union {
    gchar *icon_name;
    gchar *filename;
    GdkPixbuf *pixbuf;
  } source;

  GdkPixbuf *filename_pixbuf;

  GtkTextDirection direction;
  GtkStateType state;
  GtkIconSize size;

  /* If TRUE, then the parameter is wildcarded, and the above
   * fields should be ignored. If FALSE, the parameter is
   * specified, and the above fields should be valid.
   */
  guint any_direction : 1;
  guint any_state : 1;
  guint any_size : 1;

#if defined (G_OS_WIN32) && !defined (_WIN64)
  /* System codepage version of filename, for DLL ABI backward
   * compatibility functions.
   */
  gchar *cp_filename;
#endif
};


static void
gtk_icon_factory_buildable_init  (GtkBuildableIface      *iface);

static gboolean gtk_icon_factory_buildable_custom_tag_start (GtkBuildable     *buildable,
							     GtkBuilder       *builder,
							     GObject          *child,
							     const gchar      *tagname,
							     GMarkupParser    *parser,
							     gpointer         *data);
static void gtk_icon_factory_buildable_custom_tag_end (GtkBuildable *buildable,
						       GtkBuilder   *builder,
						       GObject      *child,
						       const gchar  *tagname,
						       gpointer     *user_data);
static void gtk_icon_factory_finalize   (GObject             *object);
static void get_default_icons           (GtkIconFactory      *icon_factory);
static void icon_source_clear           (GtkIconSource       *source);

static GtkIconSize icon_size_register_intern (const gchar *name,
					      gint         width,
					      gint         height);

#define GTK_ICON_SOURCE_INIT(any_direction, any_state, any_size)	\
  { GTK_ICON_SOURCE_EMPTY, { NULL }, NULL,				\
   0, 0, 0,								\
   any_direction, any_state, any_size }

STLWRT_DEFINE_TYPE_WITH_CODE (GtkIconFactory, gtk_icon_factory, G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (GTK_TYPE_BUILDABLE,
						gtk_icon_factory_buildable_init))

static void
gtk_icon_factory_init (GtkIconFactory *factory)
{
  factory->icons = g_hash_table_new (g_str_hash, g_str_equal);
  all_icon_factories = g_slist_prepend (all_icon_factories, factory);
}

static void
gtk_icon_factory_class_init (GtkIconFactoryClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->finalize = gtk_icon_factory_finalize;
}

static void
gtk_icon_factory_buildable_init