
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

/*
 * Modified by the GTK+ Team and others 1997-2000.  See the AUTHORS
 * file for a list of people on the GTK+ Team.  See the ChangeLog
 * files for a list of changes.  These files are distributed with
 * GTK+ at ftp://ftp.gtk.org/pub/.
 */

#ifndef __GTK_MENU_H__
#define __GTK_MENU_H__


#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtkaccelgroup.h>
#include <gtkmenushell.h>


G_BEGIN_DECLS

#define GTK_TYPE_MENU			(__gtk_menu_get_type ())
#define GTK_MENU(obj)			(G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_MENU, GtkMenu))
#define GTK_MENU_CLASS(klass)		(G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_MENU, GtkMenuClass))
#define GTK_IS_MENU(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_MENU))
#define GTK_IS_MENU_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_MENU))
#define GTK_MENU_GET_CLASS(obj)         (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_MENU, GtkMenuClass))


typedef struct _GtkMenu	      GtkMenuFat;
typedef struct _GtkMenu	      GtkMenuSkinny;
typedef struct _GtkMenuClass  GtkMenuClassFat;
typedef struct _GtkMenuClass  GtkMenuClassSkinny;

typedef void (*GtkMenuPositionFunc) (GtkMenu   *menu,
				     gint      *x,
				     gint      *y,
				     gboolean  *push_in,
				     gpointer	user_data);
typedef void (*GtkMenuDetachFunc)   (GtkWidget *attach_widget,
				     GtkMenu   *menu);

struct _GtkMenu
{
  GtkMenuShell  (menu_shell);
  
  GtkWidget * (parent_menu_item);
  GtkWidget * (old_active_menu_item);

  GtkAccelGroup * (accel_group);
  gchar         * (accel_path);
  GtkMenuPositionFunc  (position_func);
  gpointer  (position_func_data);

  guint  (toggle_size);
  /* Do _not_ touch these widgets directly. We hide the reference
   * count from the toplevel to the menu, so it must be restored
   * before operating on these widgets
   */
  GtkWidget * (toplevel);
  
  GtkWidget * (tearoff_window);
  GtkWidget * (tearoff_hbox);
  GtkWidget * (tearoff_scrollbar);
  GtkAdjustment * (tearoff_adjustment);

  GdkWindow * (view_window);
  GdkWindow * (bin_window);

  gint  (scroll_offset);
  gint  (saved_scroll_offset);
  gint  (scroll_step);
  guint  (timeout_id);
  
  /* When a submenu of this menu is popped up, motion in this
   * region is ignored
   */
  GdkRegion * (navigation_region); /* unused */
  guint  (navigation_timeout);

  guint  (needs_destruction_ref_count) : 1;
  guint  (torn_off) : 1;
  /* The tearoff is active when it is torn off and the not-torn-off
   * menu is not popped up.
   */
  guint  (tearoff_active) : 1;

  guint  (scroll_fast) : 1;

  guint  (upper_arrow_visible) : 1;
  guint  (lower_arrow_visible) : 1;
  guint  (upper_arrow_prelight) : 1;
  guint  (lower_arrow_prelight) : 1;
};

struct _GtkMenuClass
{
  GtkMenuShellClass parent_class;

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GType	   __gtk_menu_get_type		  (void) G_GNUC_CONST;
GtkWidget* __gtk_menu_new			  (void);

/* Display the menu onscreen */
void	   __gtk_menu_popup		  (GtkMenu	       *menu,
					   GtkWidget	       *parent_menu_shell,
					   GtkWidget	       *parent_menu_item,
					   GtkMenuPositionFunc	func,
					   gpointer		data,
					   guint		button,
					   guint32		activate_time);

/* Position the menu according to its position function. Called
 * from gtkmenuitem.c when a menu-item changes its allocation
 */
void	   __gtk_menu_reposition		  (GtkMenu	       *menu);

void	   __gtk_menu_popdown		  (GtkMenu	       *menu);

/* Keep track of the last menu item selected. (For the purposes
 * of the option menu
 */
GtkWidget* __gtk_menu_get_active		  (GtkMenu	       *menu);
void	   __gtk_menu_set_active		  (GtkMenu	       *menu,
					   guint		index_);

/* set/get the accelerator group that holds global accelerators (should
 * be added to the corresponding toplevel with __gtk_window_add_accel_group().
 */
void	       __gtk_menu_set_accel_group	  (GtkMenu	       *menu,
					   GtkAccelGroup       *accel_group);
GtkAccelGroup* __gtk_menu_get_accel_group	  (GtkMenu	       *menu);
void           __gtk_menu_set_accel_path    (GtkMenu             *menu,
					   const gchar         *accel_path);
const gchar*   __gtk_menu_get_accel_path    (GtkMenu             *menu);

/* A reference count is kept for a widget when it is attached to
 * a particular widget. This is typically a menu item; it may also
 * be a widget with a popup menu - for instance, the Notebook widget.
 */
void	   __gtk_menu_attach_to_widget	  (GtkMenu	       *menu,
					   GtkWidget	       *attach_widget,
					   GtkMenuDetachFunc	detacher);
void	   __gtk_menu_detach		  (GtkMenu	       *menu);

/* This should be dumped in favor of data set when the menu is popped
 * up - that is currently in the ItemFactory code, but should be
 * in the Menu code.
 */
GtkWidget* __gtk_menu_get_attach_widget	  (GtkMenu	       *menu);

void       __gtk_menu_set_tearoff_state     (GtkMenu             *menu,
					   gboolean             torn_off);
gboolean   __gtk_menu_get_tearoff_state     (GtkMenu             *menu);

/* This sets the window manager title for the window that
 * appears when a menu is torn off
 */
void       __gtk_menu_set_title             (GtkMenu             *menu,
					   const gchar         *title);
const gchar *__gtk_menu_get_title           (GtkMenu             *menu);

void       __gtk_menu_reorder_child         (GtkMenu             *menu,
                                           GtkWidget           *child,
                                           gint                position);

void	   __gtk_menu_set_screen		  (GtkMenu	       *menu,
					   GdkScreen	       *screen);

void       __gtk_menu_attach                (GtkMenu             *menu,
                                           GtkWidget           *child,
                                           guint                left_attach,
                                           guint                right_attach,
                                           guint                top_attach,
                                           guint                bottom_attach);

void       __gtk_menu_set_monitor           (GtkMenu             *menu,
                                           gint                 monitor_num);
gint       __gtk_menu_get_monitor           (GtkMenu             *menu);
GList*     __gtk_menu_get_for_attach_widget (GtkWidget           *widget); 

#ifndef GTK_DISABLE_DEPRECATED
#define __gtk_menu_append(menu,child)	__gtk_menu_shell_append  ((GtkMenuShell *)(menu),(child))
#define __gtk_menu_prepend(menu,child)    __gtk_menu_shell_prepend ((GtkMenuShell *)(menu),(child))
#define __gtk_menu_insert(menu,child,pos)	__gtk_menu_shell_insert ((GtkMenuShell *)(menu),(child),(pos))
#endif /* GTK_DISABLE_DEPRECATED */

void     __gtk_menu_set_reserve_toggle_size (GtkMenu  *menu,
                                          gboolean   reserve_toggle_size);
gboolean __gtk_menu_get_reserve_toggle_size (GtkMenu  *menu);


G_END_DECLS

#endif /* __GTK_MENU_H__ */
