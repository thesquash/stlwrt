/* gtkcellrendereraccel.h
 * Copyright (C) 2000  Red Hat, Inc.,  Jonathan Blandford <jrb@redhat.com>
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

#ifndef __GTK_CELL_RENDERER_ACCEL_H__
#define __GTK_CELL_RENDERER_ACCEL_H__

#include <stlwrt.h>


#include <gtkcellrenderertext.h>

G_BEGIN_DECLS

#define GTK_TYPE_CELL_RENDERER_ACCEL            (gtk_cell_renderer_accel_get_type ())
#define GTK_CELL_RENDERER_ACCEL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_CELL_RENDERER_ACCEL, GtkCellRendererAccel))
#define GTK_CELL_RENDERER_ACCEL_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_CELL_RENDERER_ACCEL, GtkCellRendererAccelClass))
#define GTK_IS_CELL_RENDERER_ACCEL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_CELL_RENDERER_ACCEL))
#define GTK_IS_CELL_RENDERER_ACCEL_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_CELL_RENDERER_ACCEL))
#define GTK_CELL_RENDERER_ACCEL_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_CELL_RENDERER_ACCEL, GtkCellRendererAccelClass))

typedef struct _GtkCellRendererAccelClass GtkCellRendererAccelClass;


typedef enum 
{
  GTK_CELL_RENDERER_ACCEL_MODE_GTK,
  GTK_CELL_RENDERER_ACCEL_MODE_OTHER
} GtkCellRendererAccelMode;


STLWRT_DECLARE_VTYPE_VPARENT(GtkCellRendererAccel, gtk_cell_renderer_accel, GtkCellRendererText,
  /*< private >*/
  guint  (accel_key);
  GdkModifierType  (accel_mods);
  guint  (keycode);
  GtkCellRendererAccelMode  (accel_mode);

  GtkWidget * (edit_widget);
  GtkWidget * (grab_widget);
  GtkWidget * (sizing_label);
)

struct _GtkCellRendererAccelClass
{
  GtkCellRendererTextClass parent_class;

  void (* accel_edited)  (GtkCellRendererAccel *accel,
		 	  const gchar          *path_string,
			  guint                 accel_key,
			  GdkModifierType       accel_mods,
			  guint                 hardware_keycode);

  void (* accel_cleared) (GtkCellRendererAccel *accel,
			  const gchar          *path_string);

  /* Padding for future expansion */
  void (*_gtk_reserved0) (void);
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};

GtkCellRenderer *SF(gtk_cell_renderer_accel_new)             (void);


G_END_DECLS


#endif /* __GTK_CELL_RENDERER_ACCEL_H__ */
