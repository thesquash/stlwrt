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

#ifndef __GTK_BUILDABLE_H__
#define __GTK_BUILDABLE_H__

#include <stlwrt.h>


#include <gtkbuilder.h>

#include <gtktypeutils.h>

G_BEGIN_DECLS

#define GTK_TYPE_BUILDABLE            (gtk_buildable_get_type ())
#define GTK_BUILDABLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_BUILDABLE, GtkBuildable))
#define GTK_BUILDABLE_CLASS(obj)      (G_TYPE_CHECK_CLASS_CAST ((obj), GTK_TYPE_BUILDABLE, GtkBuildableIface))
#define GTK_IS_BUILDABLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_BUILDABLE))
#define GTK_BUILDABLE_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), GTK_TYPE_BUILDABLE, GtkBuildableIface))


/**
 * GtkBuildableIface:
 * @g_iface: the parent class
 * @set_name: Stores the name attribute given in the GtkBuilder UI definition.
 *  #GtkWidget stores the name as object data. Implement this method if your
 *  object has some notion of "name" and it makes sense to map the XML name
 *  attribute to it.
 * @get_name: The getter corresponding to @set_name. Implement this
 *  if you implement @set_name.
 * @add_child: Adds a child. The @type parameter can be used to
 *  differentiate the kind of child. #GtkContainer implements this
 *  to add add a child widget to the container, #GtkNotebook uses
 *  the @type to distinguish between page labels (of type "page-label")
 *  and normal children.
 * @set_buildable_property: Sets a property of a buildable object.
 *  It is normally not necessary to implement this, g_object_set_property()
 *  is used by default. #GtkWindow implements this to delay showing itself
 *  (i.e. setting the #GtkWidget:visible property) until the whole interface
 *  is created.
 * @construct_child: Constructs a child of a buildable that has been
 *  specified as "constructor" in the UI definition. #GtkUIManager implements
 *  this to reference to a widget created in a &lt;ui&gt; tag which is outside
 *  of the normal GtkBuilder UI definition hierarchy.  A reference to the
 *  constructed object is returned and becomes owned by the caller.
 * @custom_tag_start: Implement this if the buildable needs to parse
 *  content below &lt;child&gt;. To handle an element, the implementation
 *  must fill in the @parser structure and @user_data and return %TRUE.
 *  #GtkWidget implements this to parse keyboard accelerators specified
 *  in &lt;accelerator&gt; elements. #GtkContainer implements it to map
 *  properties defined via &lt;packing&gt; elements to child properties.
 *  Note that @user_data must be freed in @custom_tag_end or @custom_finished.
 * @custom_tag_end: Called for the end tag of each custom element that is
 *  handled by the buildable (see @custom_tag_start).
 * @custom_finished: Called for each custom tag handled by the buildable
 *  when the builder finishes parsing (see @custom_tag_start)
 * @parser_finished: Called when a builder finishes the parsing
 *  of a UI definition. It is normally not necessary to implement this,
 *  unless you need to perform special cleanup actions. #GtkWindow sets
 *  the #GtkWidget:visible property here.
 * @get_internal_child: Returns an internal child of a buildable.
 *  #GtkDialog implements this to give access to its @vbox, making
 *  it possible to add children to the vbox in a UI definition.
 *  Implement this if the buildable has internal children that may
 *  need to be accessed from a UI definition.
 *
 * The GtkBuildableIface interface contains method that are
 * necessary to allow #GtkBuilder to construct an object from
 * a GtkBuilder UI definition.
 */
STLWRT_DECLARE_INTERFACE(GtkBuildable, gtk_buildable,
  /* virtual table */
  void          (* set_name)               (GtkBuildable  *buildable,
                                            const gchar   *name);
  const gchar * (* get_name)               (GtkBuildable  *buildable);
  void          (* add_child)              (GtkBuildable  *buildable,
					    GtkBuilder    *builder,
					    GObject       *child,
					    const gchar   *type);
  void          (* set_buildable_property) (GtkBuildable  *buildable,
					    GtkBuilder    *builder,
					    const gchar   *name,
					    const GValue  *value);
  GObject *     (* construct_child)        (GtkBuildable  *buildable,
					    GtkBuilder    *builder,
					    const gchar   *name);
  gboolean      (* custom_tag_start)       (GtkBuildable  *buildable,
					    GtkBuilder    *builder,
					    GObject       *child,
					    const gchar   *tagname,
					    GMarkupParser *parser,
					    gpointer      *data);
  void          (* custom_tag_end)         (GtkBuildable  *buildable,
					    GtkBuilder    *builder,
					    GObject       *child,
					    const gchar   *tagname,
					    gpointer      *data);
  void          (* custom_finished)        (GtkBuildable  *buildable,
					    GtkBuilder    *builder,
					    GObject       *child,
					    const gchar   *tagname,
					    gpointer       data);
  void          (* parser_finished)        (GtkBuildable  *buildable,
					    GtkBuilder    *builder);

  GObject *     (* get_internal_child)     (GtkBuildable  *buildable,
					    GtkBuilder    *builder,
					    const gchar   *childname);
)


void      SF(gtk_buildable_set_name)               (GtkBuildable        *buildable,
						const gchar         *name);
const gchar * SF(gtk_buildable_get_name)           (GtkBuildable        *buildable);
void      SF(gtk_buildable_add_child)              (GtkBuildable        *buildable,
						GtkBuilder          *builder,
						GObject             *child,
						const gchar         *type);
void      SF(gtk_buildable_set_buildable_property) (GtkBuildable        *buildable,
						GtkBuilder          *builder,
						const gchar         *name,
						const GValue        *value);
GObject * SF(gtk_buildable_construct_child)        (GtkBuildable        *buildable,
						GtkBuilder          *builder,
						const gchar         *name);
gboolean  SF(gtk_buildable_custom_tag_start)       (GtkBuildable        *buildable,
						GtkBuilder          *builder,
						GObject             *child,
						const gchar         *tagname,
						GMarkupParser       *parser,
						gpointer            *data);
void      SF(gtk_buildable_custom_tag_end)         (GtkBuildable        *buildable,
						GtkBuilder          *builder,
						GObject             *child,
						const gchar         *tagname,
						gpointer            *data);
void      SF(gtk_buildable_custom_finished)        (GtkBuildable        *buildable,
						GtkBuilder          *builder,
						GObject             *child,
						const gchar         *tagname,
						gpointer             data);
void      SF(gtk_buildable_parser_finished)        (GtkBuildable        *buildable,
						GtkBuilder          *builder);
GObject * SF(gtk_buildable_get_internal_child)     (GtkBuildable        *buildable,
						GtkBuilder          *builder,
						const gchar         *childname);

G_END_DECLS

#endif /* __GTK_BUILDABLE_H__ */
