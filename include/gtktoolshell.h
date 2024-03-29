/* STLWRT - A fork of GTK+ 2 supporting future applications as well
 * Copyright (C) 2007  Openismus GmbH
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
 *
 * Author:
 *   Mathias Hasselmann
 */

#ifndef __GTK_TOOL_SHELL_H__
#define __GTK_TOOL_SHELL_H__

#include <stlwrt.h>


#include <gtkenums.h>

#include <pango/pango.h>

#include <gtksizegroup.h>


G_BEGIN_DECLS

#define GTK_TYPE_TOOL_SHELL            (gtk_tool_shell_get_type ())
#define GTK_TOOL_SHELL(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_TOOL_SHELL, GtkToolShell))
#define GTK_IS_TOOL_SHELL(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_TOOL_SHELL))
#define GTK_TOOL_SHELL_GET_IFACE(obj)  (G_TYPE_INSTANCE_GET_INTERFACE ((obj), GTK_TYPE_TOOL_SHELL, GtkToolShellIface))

/**
 * GtkToolShellIface:
 * @get_icon_size:        mandatory implementation of SF(gtk_tool_shell_get_icon_size)().
 * @get_orientation:      mandatory implementation of SF(gtk_tool_shell_get_orientation)().
 * @get_style:            mandatory implementation of SF(gtk_tool_shell_get_style)().
 * @get_relief_style:     optional implementation of SF(gtk_tool_shell_get_relief_style)().
 * @rebuild_menu:         optional implementation of SF(gtk_tool_shell_rebuild_menu)().
 * @get_text_orientation: optional implementation of SF(gtk_tool_shell_get_text_orientation)().
 * @get_text_alignment:   optional implementation of SF(gtk_tool_shell_get_text_alignment)().
 * @get_ellipsize_mode:   optional implementation of SF(gtk_tool_shell_get_ellipsize_mode)().
 * @get_text_size_group:  optional implementation of SF(gtk_tool_shell_get_text_size_group)().
 *
 * Virtual function table for the #GtkToolShell interface.
 */
STLWRT_DECLARE_INTERFACE(GtkToolShell, gtk_tool_shell,
  /*< public >*/
  GtkIconSize        (*get_icon_size)        (GtkToolShell *shell);
  GtkOrientation     (*get_orientation)      (GtkToolShell *shell);
  GtkToolbarStyle    (*get_style)            (GtkToolShell *shell);
  GtkReliefStyle     (*get_relief_style)     (GtkToolShell *shell);
  void               (*rebuild_menu)         (GtkToolShell *shell);
  GtkOrientation     (*get_text_orientation) (GtkToolShell *shell);
  gfloat             (*get_text_alignment)   (GtkToolShell *shell);
  PangoEllipsizeMode (*get_ellipsize_mode)   (GtkToolShell *shell);
  GtkSizeGroup *     (*get_text_size_group)  (GtkToolShell *shell);
)

GtkIconSize        SF(gtk_tool_shell_get_icon_size)        (GtkToolShell *shell);
GtkOrientation     SF(gtk_tool_shell_get_orientation)      (GtkToolShell *shell);
GtkToolbarStyle    SF(gtk_tool_shell_get_style)            (GtkToolShell *shell);
GtkReliefStyle     SF(gtk_tool_shell_get_relief_style)     (GtkToolShell *shell);
void               SF(gtk_tool_shell_rebuild_menu)         (GtkToolShell *shell);
GtkOrientation     SF(gtk_tool_shell_get_text_orientation) (GtkToolShell *shell);
gfloat             SF(gtk_tool_shell_get_text_alignment)   (GtkToolShell *shell);
PangoEllipsizeMode SF(gtk_tool_shell_get_ellipsize_mode)   (GtkToolShell *shell);
GtkSizeGroup *     SF(gtk_tool_shell_get_text_size_group)  (GtkToolShell *shell);

G_END_DECLS

#endif /* __GTK_TOOL_SHELL_H__ */
