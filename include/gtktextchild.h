/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * gtktextchild.h Copyright (C) 2000 Red Hat, Inc.
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

#ifndef __GTK_TEXT_CHILD_H__
#define __GTK_TEXT_CHILD_H__

#include <stlwrt.h>


#include <gdkconfig.h>

#include <glib-object.h>

G_BEGIN_DECLS

/* A GtkTextChildAnchor is a spot in the buffer where child widgets
 * can be "anchored" (inserted inline, as if they were characters).
 * The anchor can have multiple widgets anchored, to allow for multiple
 * views.
 */

typedef struct _GtkTextChildAnchorClass GtkTextChildAnchorClass;

#define GTK_TYPE_TEXT_CHILD_ANCHOR              (gtk_text_child_anchor_get_type ())
#define GTK_TEXT_CHILD_ANCHOR(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GTK_TYPE_TEXT_CHILD_ANCHOR, GtkTextChildAnchor))
#define GTK_TEXT_CHILD_ANCHOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TEXT_CHILD_ANCHOR, GtkTextChildAnchorClass))
#define GTK_IS_TEXT_CHILD_ANCHOR(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GTK_TYPE_TEXT_CHILD_ANCHOR))
#define GTK_IS_TEXT_CHILD_ANCHOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TEXT_CHILD_ANCHOR))
#define GTK_TEXT_CHILD_ANCHOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TEXT_CHILD_ANCHOR, GtkTextChildAnchorClass))

STLWRT_DECLARE_FTYPE_FPARENT(GtkTextChildAnchor, gtk_text_child_anchor, GObject,
  gpointer  (segment);
)

struct _GtkTextChildAnchorClass
{
  GObjectClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

GtkTextChildAnchor* SF(gtk_text_child_anchor_new) (void);

GList*   SF(gtk_text_child_anchor_get_widgets) (GtkTextChildAnchor *anchor);
gboolean SF(gtk_text_child_anchor_get_deleted) (GtkTextChildAnchor *anchor);

G_END_DECLS

#endif
