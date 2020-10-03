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
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
 */

#ifndef __GTK_MENU_SHELL_H__
#define __GTK_MENU_SHELL_H__

#include <stlwrt.h>
#include <stlwrt-typedefs.h>


#include <gtkcontainer.h>


G_BEGIN_DECLS

#define	GTK_TYPE_MENU_SHELL		(gtk_menu_shell_get_type ())
#define GTK_MENU_SHELL(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_MENU_SHELL, GtkMenuShell))
#define GTK_MENU_SHELL_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_MENU_SHELL, GtkMenuShellClass))
#define GTK_IS_MENU_SHELL(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_MENU_SHELL))
#define GTK_IS_MENU_SHELL_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_MENU_SHELL))
#define GTK_MENU_SHELL_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_MENU_SHELL, GtkMenuShellClass))

typedef struct _GtkMenuShellClass  GtkMenuShellClass;

STLWRT_DECLARE_VTYPE_VPARENT(GtkMenuShell, gtk_menu_shell, GtkContainer,
  GList * (children);
  GtkWidget * (active_menu_item);
  GtkWidget * (parent_menu_shell);

  guint  (button);
  guint32  (activate_time);

  guint  (active) : 1;
  guint  (have_grab) : 1;
  guint  (have_xgrab) : 1;
  guint  (ignore_leave) : 1; /* unused */
  guint  (menu_flag) : 1;    /* unused */
  guint  (ignore_enter) : 1;
  guint  (keyboard_mode) : 1;
)

struct _GtkMenuShellPrivate
{
  GtkMnemonicHash *mnemonic_hash;
  GtkKeyHash *key_hash;

  guint take_focus : 1;
  guint activated_submenu : 1;
  /* This flag is a crutch to keep mnemonics in the same menu
   * if the user moves the mouse over an unselectable menuitem.
   */
  guint in_unselectable_item : 1;
};

struct _GtkMenuShellClass
{
  GtkContainerClass parent_class;
  
  guint submenu_placement : 1;
  
  void (*deactivate)     (GtkMenuShell *menu_shell);
  void (*selection_done) (GtkMenuShell *menu_shell);

  void (*move_current)     (GtkMenuShell        *menu_shell,
			    GtkMenuDirectionType direction);
  void (*activate_current) (GtkMenuShell *menu_shell,
			    gboolean      force_hide);
  void (*cancel)           (GtkMenuShell *menu_shell);
  void (*select_item)      (GtkMenuShell *menu_shell,
			    GtkWidget    *menu_item);
  void (*insert)           (GtkMenuShell *menu_shell,
			    GtkWidget    *child,
			    gint          position);
  gint (*get_popup_delay)  (GtkMenuShell *menu_shell);
  gboolean (*move_selected) (GtkMenuShell *menu_shell,
			     gint          distance);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
};


void  SF(gtk_menu_shell_append)            (GtkMenuShell *menu_shell,
					GtkWidget    *child);
void  SF(gtk_menu_shell_prepend)           (GtkMenuShell *menu_shell,
					GtkWidget    *child);
void  SF(gtk_menu_shell_insert)            (GtkMenuShell *menu_shell,
					GtkWidget    *child,
					gint          position);
void  SF(gtk_menu_shell_deactivate)        (GtkMenuShell *menu_shell);
void  SF(gtk_menu_shell_select_item)       (GtkMenuShell *menu_shell,
					GtkWidget    *menu_item);
void  SF(gtk_menu_shell_deselect)          (GtkMenuShell *menu_shell);
void  SF(gtk_menu_shell_activate_item)     (GtkMenuShell *menu_shell,
					GtkWidget    *menu_item,
					gboolean      force_deactivate);
void  SF(gtk_menu_shell_select_first)      (GtkMenuShell *menu_shell,
					gboolean      search_sensitive);
void SF(_gtk_menu_shell_select_last)       (GtkMenuShell *menu_shell,
					gboolean      search_sensitive);
gint  SF(_gtk_menu_shell_get_popup_delay)  (GtkMenuShell *menu_shell);
void  SF(gtk_menu_shell_cancel)            (GtkMenuShell *menu_shell);

void  SF(_gtk_menu_shell_add_mnemonic)     (GtkMenuShell *menu_shell,
                                        guint         keyval,
                                        GtkWidget    *target);
void  SF(_gtk_menu_shell_remove_mnemonic)  (GtkMenuShell *menu_shell,
                                        guint         keyval,
                                        GtkWidget    *target);

gboolean SF(gtk_menu_shell_get_take_focus) (GtkMenuShell *menu_shell);
void     SF(gtk_menu_shell_set_take_focus) (GtkMenuShell *menu_shell,
                                        gboolean      take_focus);

void     SF(_gtk_menu_shell_update_mnemonics)  (GtkMenuShell *menu_shell);
void     SF(_gtk_menu_shell_set_keyboard_mode) (GtkMenuShell *menu_shell,
                                            gboolean      keyboard_mode);
gboolean SF(_gtk_menu_shell_get_keyboard_mode) (GtkMenuShell *menu_shell);

G_END_DECLS

#endif /* __GTK_MENU_SHELL_H__ */
