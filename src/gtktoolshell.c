/* gtktoolshell.c
 * Copyright (C) 2007  Openismus GmbH
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
 *
 * Author:
 *   Mathias Hasselmann
 */

#include "config.h"

#include <stlwrt.h>
#include <gtktoolshell.h>
#include <gtkwidget.h>
#include <gtkintl.h>


/**
 * SECTION:gtktoolshell
 * @Short_description: Interface for containers containing GtkToolItem widgets
 * @Title: GtkToolShell
 *
 * The #GtkToolShell interface allows container widgets to provide additional
 * information when embedding #GtkToolItem widgets.
 *
 * @see_also: #GtkToolbar, #GtkToolItem
 */

/**
 * GtkToolShell:
 *
 * Dummy structure for accessing instances of #GtkToolShellIface.
 */

STLWRT_DEFINE_INTERFACE (GtkToolShell, gtk_tool_shell, GTK_TYPE_WIDGET, NULL, ;)

/**
 * __gtk_tool_shell_get_icon_size:
 * @shell: a #GtkToolShell
 *
 * Retrieves the icon size for the tool shell. Tool items must not call this
 * function directly, but rely on __gtk_tool_item_get_icon_size() instead.
 *
 * Return value: (type int): the current size for icons of @shell
 *
 * Since: 2.14
 **/
GtkIconSize
__gtk_tool_shell_get_icon_size (GtkToolShell *shell)
{
  return GTK_TOOL_SHELL_GET_IFACE (shell)->get_icon_size (shell);
}

/**
 * __gtk_tool_shell_get_orientation:
 * @shell: a #GtkToolShell
 *
 * Retrieves the current orientation for the tool shell. Tool items must not
 * call this function directly, but rely on __gtk_tool_item_get_orientation()
 * instead.
 *
 * Return value: the current orientation of @shell
 *
 * Since: 2.14
 **/
GtkOrientation
__gtk_tool_shell_get_orientation (GtkToolShell *shell)
{
  return GTK_TOOL_SHELL_GET_IFACE (shell)->get_orientation (shell);
}

/**
 * __gtk_tool_shell_get_style:
 * @shell: a #GtkToolShell
 *
 * Retrieves whether the tool shell has text, icons, or both. Tool items must
 * not call this function directly, but rely on gtk_tool_item_get_style()
 * instead.
 *
 * Return value: the current style of @shell
 *
 * Since: 2.14
 **/
GtkToolbarStyle
__gtk_tool_shell_get_style (GtkToolShell *shell)
{
  return GTK_TOOL_SHELL_GET_IFACE (shell)->get_style (shell);
}

/**
 * __gtk_tool_shell_get_relief_style:
 * @shell: a #GtkToolShell
 *
 * Returns the relief style of buttons on @shell. Tool items must not call this
 * function directly, but rely on __gtk_tool_item_get_relief_style() instead.
 *
 * Return value: The relief style of buttons on @shell.
 *
 * Since: 2.14
 **/
GtkReliefStyle
__gtk_tool_shell_get_relief_style (GtkToolShell *shell)
{
  GtkToolShellIface *iface = GTK_TOOL_SHELL_GET_IFACE (shell);

  if (iface->get_relief_style)
    return iface->get_relief_style (shell);

  return GTK_RELIEF_NONE;
}

/**
 * __gtk_tool_shell_rebuild_menu:
 * @shell: a #GtkToolShell
 *
 * Calling this function signals the tool shell that the overflow menu item for
 * tool items have changed. If there is an overflow menu and if it is visible
 * when this function it called, the menu will be rebuilt.
 *
 * Tool items must not call this function directly, but rely on
 * __gtk_tool_item_rebuild_menu() instead.
 *
 * Since: 2.14
 **/
void
__gtk_tool_shell_rebuild_menu (GtkToolShell *shell)
{
  GtkToolShellIface *iface = GTK_TOOL_SHELL_GET_IFACE (shell);

  if (iface->rebuild_menu)
    iface->rebuild_menu (shell);
}

/**
 * __gtk_tool_shell_get_text_orientation:
 * @shell: a #GtkToolShell
 *
 * Retrieves the current text orientation for the tool shell. Tool items must not
 * call this function directly, but rely on __gtk_tool_item_get_text_orientation()
 * instead.
 *
 * Return value: the current text orientation of @shell
 *
 * Since: 2.20
 **/
GtkOrientation
__gtk_tool_shell_get_text_orientation (GtkToolShell *shell)
{
  GtkToolShellIface *iface = GTK_TOOL_SHELL_GET_IFACE (shell);

  if (iface->get_text_orientation)
    return GTK_TOOL_SHELL_GET_IFACE (shell)->get_text_orientation (shell);

  return GTK_ORIENTATION_HORIZONTAL;
}

/**
 * __gtk_tool_shell_get_text_alignment:
 * @shell: a #GtkToolShell
 *
 * Retrieves the current text alignment for the tool shell. Tool items must not
 * call this function directly, but rely on __gtk_tool_item_get_text_alignment()
 * instead.
 *
 * Return value: the current text alignment of @shell
 *
 * Since: 2.20
 **/
gfloat
__gtk_tool_shell_get_text_alignment (GtkToolShell *shell)
{
  GtkToolShellIface *iface = GTK_TOOL_SHELL_GET_IFACE (shell);

  if (iface->get_text_alignment)
    return GTK_TOOL_SHELL_GET_IFACE (shell)->get_text_alignment (shell);

  return 0.5f;
}

/**
 * __gtk_tool_shell_get_ellipsize_mode
 * @shell: a #GtkToolShell
 *
 * Retrieves the current ellipsize mode for the tool shell. Tool items must not
 * call this function directly, but rely on __gtk_tool_item_get_ellipsize_mode()
 * instead.
 *
 * Return value: the current ellipsize mode of @shell
 *
 * Since: 2.20
 **/
PangoEllipsizeMode
__gtk_tool_shell_get_ellipsize_mode (GtkToolShell *shell)
{
  GtkToolShellIface *iface = GTK_TOOL_SHELL_GET_IFACE (shell);

  if (iface->get_ellipsize_mode)
    return GTK_TOOL_SHELL_GET_IFACE (shell)->get_ellipsize_mode (shell);

  return PANGO_ELLIPSIZE_NONE;
}

/**
 * __gtk_tool_shell_get_text_size_group:
 * @shell: a #GtkToolShell
 *
 * Retrieves the current text size group for the tool shell. Tool items must not
 * call this function directly, but rely on __gtk_tool_item_get_text_size_group()
 * instead.
 *
 * Return value: (transfer none): the current text size group of @shell
 *
 * Since: 2.20
 **/
GtkSizeGroup *
__gtk_tool_shell_get_text_size_group (GtkToolShell *shell)
{
  GtkToolShellIface *iface = GTK_TOOL_SHELL_GET_IFACE (shell);

  if (iface->get_text_size_group)
    return GTK_TOOL_SHELL_GET_IFACE (shell)->get_text_size_group (shell);

  return NULL;
}
