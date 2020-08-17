
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

#include <gdkconfig.h>
#include <glib-object.h>

G_BEGIN_DECLS

/* A GtkTextChildAnchor is a spot in the buffer where child widgets
 * can be "anchored" (inserted inline, as if they were characters).
 * The anchor can have multiple widgets anchored, to allow for multiple
 * views.
 */

typedef struct _GtkTextChildAnchor      GtkTextChildAnchorFat;
typedef struct _GtkTextChildAnchor      GtkTextChildAnchorThin;

typedef struct _GtkTextChildAnchorClass GtkTextChildAnchorClass;

#define GTK_TYPE_TEXT_CHILD_ANCHOR              (gtk_text_child_anchor_get_type ())
#define GTK_TEXT_CHILD_ANCHOR(object)           (G_TYPE_CHECK_INSTANCE_CAST ((object), GTK_TYPE_TEXT_CHILD_ANCHOR, GtkTextChildAnchor))
#define GTK_TEXT_CHILD_ANCHOR_CLASS(klass)      (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_TEXT_CHILD_ANCHOR, GtkTextChildAnchorClass))
#define GTK_IS_TEXT_CHILD_ANCHOR(object)        (G_TYPE_CHECK_INSTANCE_TYPE ((object), GTK_TYPE_TEXT_CHILD_ANCHOR))
#define GTK_IS_TEXT_CHILD_ANCHOR_CLASS(klass)   (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_TEXT_CHILD_ANCHOR))
#define GTK_TEXT_CHILD_ANCHOR_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_TEXT_CHILD_ANCHOR, GtkTextChildAnchorClass))

/********************************************************************/
struct _GtkTextChildAnchorProps
{


  gpointer  (segment);
};

struct _GtkTextChildAnchorFat
{
  GObject   parent_instance;

  struct _GtkTextChildAnchorProps instance_properties;
};

struct _GtkTextChildAnchorThin
{
  GObject  parent_instance;

  gpointer reserved;
};


#ifdef STLWRT_COMPILATION
typedef union
{
  struct _GtkTextChildAnchorFat   fat_instance;
  struct _GtkTextChildAnchorThin  thin_instance;
}   GtkTextChildAnchor;
#elif STLWRT_GTK_VERSION <= 2
typedef struct _GtkTextChildAnchorFat GtkTextChildAnchor;
#elif STLWRT_GTK_VERSION >= 3
typedef struct _GtkTextChildAnchorThin GtkTextChildAnchor;
#endif
/********************************************************************/



struct _GtkTextChildAnchorClass
{
  GObjectClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

GType _T2_gtk_text_child_anchor_get_type (void) G_GNUC_CONST;
GType _3T_gtk_text_child_anchor_get_type (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType gtk_text_child_anchor_get_type (void) G_GNUC_CONST;

GtkTextChildAnchor* __gtk_text_child_anchor_new (void);

GList*   __gtk_text_child_anchor_get_widgets (GtkTextChildAnchor *anchor);
gboolean __gtk_text_child_anchor_get_deleted (GtkTextChildAnchor *anchor);

G_END_DECLS

#endif