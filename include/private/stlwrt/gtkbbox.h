
/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 1995-1997 Peter Mattis, Spencer Kimball and Josh MacDonald
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

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/.
 */

#ifndef __GTK_BUTTON_BOX_H__
#define __GTK_BUTTON_BOX_H__

#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtkbox.h>


G_BEGIN_DECLS  

#define GTK_TYPE_BUTTON_BOX             (gtk_button_box_get_type ())
#define GTK_BUTTON_BOX(obj)             (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_BUTTON_BOX, GtkButtonBox))
#define GTK_BUTTON_BOX_CLASS(klass)     (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_BUTTON_BOX, GtkButtonBoxClass))
#define GTK_IS_BUTTON_BOX(obj)          (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_BUTTON_BOX))
#define GTK_IS_BUTTON_BOX_CLASS(klass)  (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_BUTTON_BOX))
#define GTK_BUTTON_BOX_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_BUTTON_BOX, GtkButtonBoxClass))
  

#define GTK_BUTTONBOX_DEFAULT -1
 
typedef struct _GtkButtonBox       GtkButtonBoxFat;
typedef struct _GtkButtonBox       GtkButtonBoxSkinny;
typedef struct _GtkButtonBoxClass  GtkButtonBoxClassFat;
typedef struct _GtkButtonBoxClass  GtkButtonBoxClassSkinny;

struct _GtkButtonBox
{
  GtkBox box;
  gint  (child_min_width);
  gint  (child_min_height);
  gint  (child_ipad_x);
  gint  (child_ipad_y);
  GtkButtonBoxStyle  (layout_style);
};

struct _GtkButtonBoxClass
{
  GtkBoxClass parent_class;
};


GType _T2_gtk_button_box_get_type (void) G_GNUC_CONST;
GType _3T_gtk_button_box_get_type (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType gtk_button_box_get_type (void) G_GNUC_CONST;

GtkButtonBoxStyle __gtk_button_box_get_layout          (GtkButtonBox      *widget);
void              __gtk_button_box_set_layout          (GtkButtonBox      *widget,
						      GtkButtonBoxStyle  layout_style);
gboolean          __gtk_button_box_get_child_secondary (GtkButtonBox      *widget,
						      GtkWidget         *child);
void              __gtk_button_box_set_child_secondary (GtkButtonBox      *widget,
						      GtkWidget         *child,
						      gboolean           is_secondary);

#ifndef GTK_DISABLE_DEPRECATED
#define __gtk_button_box_set_spacing(b,s) __gtk_box_set_spacing (GTK_BOX (b), s)
#define __gtk_button_box_get_spacing(b)   __gtk_box_get_spacing (GTK_BOX (b))

void __gtk_button_box_set_child_size     (GtkButtonBox *widget,
					gint          min_width,
					gint          min_height);
void __gtk_button_box_set_child_ipadding (GtkButtonBox *widget,
					gint          ipad_x,
					gint          ipad_y);
void __gtk_button_box_get_child_size     (GtkButtonBox *widget,
					gint         *min_width,
					gint         *min_height);
void __gtk_button_box_get_child_ipadding (GtkButtonBox *widget,
					gint         *ipad_x,
					gint         *ipad_y);
#endif

/* Internal method - do not use. */
void ___gtk_button_box_child_requisition (GtkWidget *widget,
					int       *nvis_children,
					int       *nvis_secondaries,
					int       *width,
					int       *height);
G_END_DECLS

#endif /* __GTK_BUTTON_BOX_H__ */
