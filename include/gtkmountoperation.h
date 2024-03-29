/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) Christian Kellner <gicmo@gnome.org>
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

#ifndef __GTK_MOUNT_OPERATION_H__
#define __GTK_MOUNT_OPERATION_H__

#include <stlwrt.h>

G_BEGIN_DECLS

#define GTK_TYPE_MOUNT_OPERATION         (gtk_mount_operation_get_type ())
#define GTK_MOUNT_OPERATION(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), GTK_TYPE_MOUNT_OPERATION, GtkMountOperation))
#define GTK_MOUNT_OPERATION_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), GTK_TYPE_MOUNT_OPERATION, GtkMountOperationClass))
#define GTK_IS_MOUNT_OPERATION(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), GTK_TYPE_MOUNT_OPERATION))
#define GTK_IS_MOUNT_OPERATION_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), GTK_TYPE_MOUNT_OPERATION))
#define GTK_MOUNT_OPERATION_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), GTK_TYPE_MOUNT_OPERATION, GtkMountOperationClass))

typedef struct _GtkMountOperationClass    GtkMountOperationClass;
typedef struct _GtkMountOperationPrivate  GtkMountOperationPrivate;

/**
 * GtkMountOperation:
 *
 * This should not be accessed directly. Use the accessor functions below.
 */
STLWRT_DECLARE_FTYPE_FPARENT(GtkMountOperation, gtk_mount_operation, GMountOperation,
  GtkMountOperationPrivate *priv;
)

struct _GtkMountOperationClass
{
  GMountOperationClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GMountOperation *SF(gtk_mount_operation_new)        (GtkWindow         *parent);
gboolean         SF(gtk_mount_operation_is_showing) (GtkMountOperation *op);
void             SF(gtk_mount_operation_set_parent) (GtkMountOperation *op,
                                                 GtkWindow         *parent);
GtkWindow *      SF(gtk_mount_operation_get_parent) (GtkMountOperation *op);
void             SF(gtk_mount_operation_set_screen) (GtkMountOperation *op,
                                                 GdkScreen         *screen);
GdkScreen       *SF(gtk_mount_operation_get_screen) (GtkMountOperation *op);

G_END_DECLS

#endif /* __GTK_MOUNT_OPERATION_H__ */
