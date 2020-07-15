
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

#ifndef __GTK_SCROLLED_WINDOW_H__
#define __GTK_SCROLLED_WINDOW_H__


#if defined(GTK_DISABLE_SINGLE_INCLUDES) && !defined (__GTK_H_INSIDE__) && !defined (STLWRT_COMPILATION)
#error "Only <gtk.h> can be included directly."
#endif

#include <gtkhscrollbar.h>
#include <gtkvscrollbar.h>
#include <gtkviewport.h>


G_BEGIN_DECLS


#define GTK_TYPE_SCROLLED_WINDOW            (gtk_scrolled_window_get_type ())
#define GTK_SCROLLED_WINDOW(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), GTK_TYPE_SCROLLED_WINDOW, GtkScrolledWindow))
#define GTK_SCROLLED_WINDOW_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), GTK_TYPE_SCROLLED_WINDOW, GtkScrolledWindowClass))
#define GTK_IS_SCROLLED_WINDOW(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), GTK_TYPE_SCROLLED_WINDOW))
#define GTK_IS_SCROLLED_WINDOW_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), GTK_TYPE_SCROLLED_WINDOW))
#define GTK_SCROLLED_WINDOW_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), GTK_TYPE_SCROLLED_WINDOW, GtkScrolledWindowClass))



typedef struct _GtkScrolledWindow       GtkScrolledWindowFat;
typedef struct _GtkScrolledWindow       GtkScrolledWindowThin;
typedef struct _GtkScrolledWindowClass  GtkScrolledWindowClassFat;
typedef struct _GtkScrolledWindowClass  GtkScrolledWindowClassThin;

struct _GtkScrolledWindow
{
  GtkBin container;

  /*< public >*/
  GtkWidget * (hscrollbar);
  GtkWidget * (vscrollbar);

  /*< private >*/
  guint  (hscrollbar_policy)      : 2;
  guint  (vscrollbar_policy)      : 2;
  guint  (hscrollbar_visible)     : 1;
  guint  (vscrollbar_visible)     : 1;
  guint  (window_placement)       : 2;
  guint  (focus_out)              : 1;	/* Flag used by ::move-focus-out implementation */

  guint16  (shadow_type);
};

struct _GtkScrolledWindowClass
{
  GtkBinClass parent_class;

  gint scrollbar_spacing;

  /* Action signals for keybindings. Do not connect to these signals
   */

  /* Unfortunately, GtkScrollType is deficient in that there is
   * no horizontal/vertical variants for GTK_SCROLL_START/END,
   * so we have to add an additional boolean flag.
   */
  gboolean (*scroll_child) (GtkScrolledWindow *scrolled_window,
	  		    GtkScrollType      scroll,
			    gboolean           horizontal);

  void (* move_focus_out) (GtkScrolledWindow *scrolled_window,
			   GtkDirectionType   direction);

  /* Padding for future expansion */
  void (*_gtk_reserved1) (void);
  void (*_gtk_reserved2) (void);
  void (*_gtk_reserved3) (void);
  void (*_gtk_reserved4) (void);
};


GType          _T2_gtk_scrolled_window_get_type          (void) G_GNUC_CONST;
GType          _3T_gtk_scrolled_window_get_type          (void) G_GNUC_CONST;
/* Supplied in the STLWRT public libraries */
GType          gtk_scrolled_window_get_type          (void) G_GNUC_CONST;
GtkWidget*     __gtk_scrolled_window_new               (GtkAdjustment     *hadjustment,
						      GtkAdjustment     *vadjustment);
void           __gtk_scrolled_window_set_hadjustment   (GtkScrolledWindow *scrolled_window,
						      GtkAdjustment     *hadjustment);
void           __gtk_scrolled_window_set_vadjustment   (GtkScrolledWindow *scrolled_window,
						      GtkAdjustment     *vadjustment);
GtkAdjustment* __gtk_scrolled_window_get_hadjustment   (GtkScrolledWindow *scrolled_window);
GtkAdjustment* __gtk_scrolled_window_get_vadjustment   (GtkScrolledWindow *scrolled_window);
GtkWidget*     __gtk_scrolled_window_get_hscrollbar    (GtkScrolledWindow *scrolled_window);
GtkWidget*     __gtk_scrolled_window_get_vscrollbar    (GtkScrolledWindow *scrolled_window);
void           __gtk_scrolled_window_set_policy        (GtkScrolledWindow *scrolled_window,
						      GtkPolicyType      hscrollbar_policy,
						      GtkPolicyType      vscrollbar_policy);
void           __gtk_scrolled_window_get_policy        (GtkScrolledWindow *scrolled_window,
						      GtkPolicyType     *hscrollbar_policy,
						      GtkPolicyType     *vscrollbar_policy);
void           __gtk_scrolled_window_set_placement     (GtkScrolledWindow *scrolled_window,
						      GtkCornerType      window_placement);
void           __gtk_scrolled_window_unset_placement   (GtkScrolledWindow *scrolled_window);

GtkCornerType  __gtk_scrolled_window_get_placement     (GtkScrolledWindow *scrolled_window);
void           __gtk_scrolled_window_set_shadow_type   (GtkScrolledWindow *scrolled_window,
						      GtkShadowType      type);
GtkShadowType  __gtk_scrolled_window_get_shadow_type   (GtkScrolledWindow *scrolled_window);
void	       __gtk_scrolled_window_add_with_viewport (GtkScrolledWindow *scrolled_window,
						      GtkWidget		*child);

gint ___gtk_scrolled_window_get_scrollbar_spacing (GtkScrolledWindow *scrolled_window);


G_END_DECLS


#endif /* __GTK_SCROLLED_WINDOW_H__ */
