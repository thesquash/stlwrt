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

#ifndef __GDK_X_H__
#define __GDK_X_H__

#include <gdkprivate.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>

G_BEGIN_DECLS

#if (!defined (GDK_DISABLE_DEPRECATED) && !defined (GDK_MULTIHEAD_SAFE)) || defined (STLWRT_COMPILATION)
extern Display          *gdk_display;
#endif

Display *__gdk_x11_drawable_get_xdisplay    (GdkDrawable *drawable);
XID      __gdk_x11_drawable_get_xid         (GdkDrawable *drawable);
GdkDrawable *__gdk_x11_window_get_drawable_impl (GdkWindow *window);
GdkDrawable *__gdk_x11_pixmap_get_drawable_impl (GdkPixmap *pixmap);
Display *__gdk_x11_image_get_xdisplay       (GdkImage    *image);
XImage  *__gdk_x11_image_get_ximage         (GdkImage    *image);
Display *__gdk_x11_colormap_get_xdisplay    (GdkColormap *colormap);
Colormap __gdk_x11_colormap_get_xcolormap   (GdkColormap *colormap);
Display *__gdk_x11_cursor_get_xdisplay      (GdkCursor   *cursor);
Cursor   __gdk_x11_cursor_get_xcursor       (GdkCursor   *cursor);
Display *__gdk_x11_display_get_xdisplay     (GdkDisplay  *display);
Visual * __gdk_x11_visual_get_xvisual       (GdkVisual   *visual);
#if !defined(GDK_DISABLE_DEPRECATED) || defined(STLWRT_COMPILATION)
Display *__gdk_x11_gc_get_xdisplay          (GdkGC       *gc);
GC       __gdk_x11_gc_get_xgc               (GdkGC       *gc);
#endif
Screen * __gdk_x11_screen_get_xscreen       (GdkScreen   *screen);
int      __gdk_x11_screen_get_screen_number (GdkScreen   *screen);
void     __gdk_x11_window_set_user_time     (GdkWindow   *window,
					   guint32      timestamp);
void     __gdk_x11_window_move_to_current_desktop (GdkWindow   *window);

const char* __gdk_x11_screen_get_window_manager_name (GdkScreen *screen);

#ifndef GDK_MULTIHEAD_SAFE
Window   __gdk_x11_get_default_root_xwindow (void);
Display *__gdk_x11_get_default_xdisplay     (void);
gint     __gdk_x11_get_default_screen       (void);
#endif

#define GDK_COLORMAP_XDISPLAY(cmap)   (__gdk_x11_colormap_get_xdisplay (cmap))
#define GDK_COLORMAP_XCOLORMAP(cmap)  (__gdk_x11_colormap_get_xcolormap (cmap))
#define GDK_CURSOR_XDISPLAY(cursor)   (__gdk_x11_cursor_get_xdisplay (cursor))
#define GDK_CURSOR_XCURSOR(cursor)    (__gdk_x11_cursor_get_xcursor (cursor))
#define GDK_IMAGE_XDISPLAY(image)     (__gdk_x11_image_get_xdisplay (image))
#define GDK_IMAGE_XIMAGE(image)       (__gdk_x11_image_get_ximage (image))

#if (!defined (GDK_DISABLE_DEPRECATED) && !defined (GDK_MULTIHEAD_SAFE)) || defined (STLWRT_COMPILATION)
#define GDK_DISPLAY()                 gdk_display
#endif

#ifdef STLWRT_COMPILATION

#include "gdkprivate-x11.h"
#include "gdkscreen-x11.h"

#define GDK_DISPLAY_XDISPLAY(display) (GDK_DISPLAY_X11(display)->xdisplay)

#define GDK_WINDOW_XDISPLAY(win)      (GDK_SCREEN_X11 (GDK_WINDOW_SCREEN (win))->xdisplay)
#define GDK_WINDOW_XID(win)           (GDK_DRAWABLE_IMPL_X11(((GdkWindowObject *)win)->impl)->xid)
#define GDK_PIXMAP_XDISPLAY(pix)      (GDK_SCREEN_X11 (GDK_PIXMAP_SCREEN (pix))->xdisplay)
#define GDK_PIXMAP_XID(pix)           (GDK_DRAWABLE_IMPL_X11(((GdkPixmapObject *)pix)->impl)->xid)
#define GDK_DRAWABLE_XDISPLAY(win)    (GDK_IS_WINDOW (win) ? GDK_WINDOW_XDISPLAY (win) : GDK_PIXMAP_XDISPLAY (win))
#define GDK_DRAWABLE_XID(win)         (GDK_IS_WINDOW (win) ? GDK_WINDOW_XID (win) : GDK_PIXMAP_XID (win))
#define GDK_GC_XDISPLAY(gc)           (GDK_SCREEN_XDISPLAY(GDK_GC_X11(gc)->screen))
#define GDK_GC_XGC(gc)		      (GDK_GC_X11(gc)->xgc)
#define GDK_SCREEN_XDISPLAY(screen)   (GDK_SCREEN_X11 (screen)->xdisplay)
#define GDK_SCREEN_XSCREEN(screen)    (GDK_SCREEN_X11 (screen)->xscreen)
#define GDK_SCREEN_XNUMBER(screen)    (GDK_SCREEN_X11 (screen)->screen_num) 
#define GDK_VISUAL_XVISUAL(vis)       (((GdkVisualPrivate *) vis)->xvisual)
#define GDK_GC_GET_XGC(gc)	      (GDK_GC_X11(gc)->dirty_mask ? ___gdk_x11_gc_flush (gc) : ((GdkGCX11 *)(gc))->xgc)
#define GDK_WINDOW_XWINDOW	      GDK_DRAWABLE_XID

#else /* STLWRT_COMPILATION */

#ifndef GDK_MULTIHEAD_SAFE
#define GDK_ROOT_WINDOW()             (__gdk_x11_get_default_root_xwindow ())
#endif

#define GDK_DISPLAY_XDISPLAY(display) (__gdk_x11_display_get_xdisplay (display))

#define GDK_WINDOW_XDISPLAY(win)      (__gdk_x11_drawable_get_xdisplay (gdk_x11_window_get_drawable_impl (win)))
#define GDK_WINDOW_XID(win)           (__gdk_x11_drawable_get_xid (win))
#define GDK_WINDOW_XWINDOW(win)       (__gdk_x11_drawable_get_xid (win))
#define GDK_PIXMAP_XDISPLAY(win)      (__gdk_x11_drawable_get_xdisplay (gdk_x11_pixmap_get_drawable_impl (win)))
#define GDK_PIXMAP_XID(win)           (__gdk_x11_drawable_get_xid (win))
#define GDK_DRAWABLE_XDISPLAY(win)    (__gdk_x11_drawable_get_xdisplay (win))
#define GDK_DRAWABLE_XID(win)         (__gdk_x11_drawable_get_xid (win))
#define GDK_GC_XDISPLAY(gc)           (__gdk_x11_gc_get_xdisplay (gc))
#define GDK_GC_XGC(gc)                (__gdk_x11_gc_get_xgc (gc))
#define GDK_SCREEN_XDISPLAY(screen)   (__gdk_x11_display_get_xdisplay (gdk_screen_get_display (screen)))
#define GDK_SCREEN_XSCREEN(screen)    (__gdk_x11_screen_get_xscreen (screen))
#define GDK_SCREEN_XNUMBER(screen)    (__gdk_x11_screen_get_screen_number (screen))
#define GDK_VISUAL_XVISUAL(visual)    (__gdk_x11_visual_get_xvisual (visual))

#endif /* STLWRT_COMPILATION */

GdkVisual* __gdk_x11_screen_lookup_visual (GdkScreen *screen,
					 VisualID   xvisualid);
#ifndef GDK_DISABLE_DEPRECATED
#ifndef GDK_MULTIHEAD_SAFE
GdkVisual* gdkx_visual_get            (VisualID   xvisualid);
#endif
#endif

#ifdef GDK_ENABLE_BROKEN
/* XXX: An X Colormap is useless unless we also have the visual. */
GdkColormap* gdkx_colormap_get (Colormap xcolormap);
#endif

GdkColormap *__gdk_x11_colormap_foreign_new (GdkVisual *visual,
					   Colormap   xcolormap);

#if !defined (GDK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
gpointer      __gdk_xid_table_lookup_for_display (GdkDisplay *display,
						XID         xid);
#endif
guint32       __gdk_x11_get_server_time  (GdkWindow       *window);
guint32       __gdk_x11_display_get_user_time (GdkDisplay *display);

const gchar * __gdk_x11_display_get_startup_notification_id (GdkDisplay *display);

void          __gdk_x11_display_set_cursor_theme (GdkDisplay  *display,
						const gchar *theme,
						const gint   size);

void __gdk_x11_display_broadcast_startup_message (GdkDisplay *display,
						const char *message_type,
						...) G_GNUC_NULL_TERMINATED;

/* returns TRUE if we support the given WM spec feature */
gboolean __gdk_x11_screen_supports_net_wm_hint (GdkScreen *screen,
					      GdkAtom    property);

XID      __gdk_x11_screen_get_monitor_output   (GdkScreen *screen,
                                              gint       monitor_num);

#ifndef GDK_MULTIHEAD_SAFE
#ifndef GDK_DISABLE_DEPRECATED
gpointer      __gdk_xid_table_lookup   (XID              xid);
gboolean      __gdk_net_wm_supports    (GdkAtom    property);
#endif
void          __gdk_x11_grab_server    (void);
void          __gdk_x11_ungrab_server  (void);
#endif

GdkDisplay   *__gdk_x11_lookup_xdisplay (Display *xdisplay);


/* Functions to get the X Atom equivalent to the GdkAtom */
Atom	              __gdk_x11_atom_to_xatom_for_display (GdkDisplay  *display,
							 GdkAtom      atom);
GdkAtom		      __gdk_x11_xatom_to_atom_for_display (GdkDisplay  *display,
							 Atom	      xatom);
Atom		      __gdk_x11_get_xatom_by_name_for_display (GdkDisplay  *display,
							     const gchar *atom_name);
const gchar *         __gdk_x11_get_xatom_name_for_display (GdkDisplay  *display,
							  Atom         xatom);
#ifndef GDK_MULTIHEAD_SAFE
Atom                  __gdk_x11_atom_to_xatom     (GdkAtom      atom);
GdkAtom               __gdk_x11_xatom_to_atom     (Atom         xatom);
Atom                  __gdk_x11_get_xatom_by_name (const gchar *atom_name);
const gchar *         __gdk_x11_get_xatom_name    (Atom         xatom);
#endif

void	    __gdk_x11_display_grab	      (GdkDisplay *display);
void	    __gdk_x11_display_ungrab	      (GdkDisplay *display);
void        __gdk_x11_register_standard_event_type (GdkDisplay *display,
						  gint        event_base,
						  gint        n_events);

#if !defined(GDK_DISABLE_DEPRECATED) || defined(STLWRT_COMPILATION)

gpointer             __gdk_x11_font_get_xfont    (GdkFont *font);
#define GDK_FONT_XFONT(font)          (__gdk_x11_font_get_xfont (font))

#define __gdk_font_lookup_for_display(display, xid) ((GdkFont*) __gdk_xid_table_lookup_for_display (display, ((xid)|XID_FONT_BIT)))

#endif /* !GDK_DISABLE_DEPRECATED || STLWRT_COMPILATION */

#ifndef GDK_DISABLE_DEPRECATED

Display *            __gdk_x11_font_get_xdisplay (GdkFont *font);
const char *         __gdk_x11_font_get_name     (GdkFont *font);

#define GDK_FONT_XDISPLAY(font)       (__gdk_x11_font_get_xdisplay (font))

#ifndef GDK_MULTIHEAD_SAFE

#define __gdk_font_lookup(xid)	   ((GdkFont*) __gdk_xid_table_lookup (xid))

#endif /* GDK_MULTIHEAD_SAFE */
#endif /* GDK_DISABLE_DEPRECATED */

void        __gdk_x11_set_sm_client_id (const gchar *sm_client_id);

GdkWindow  *__gdk_x11_window_foreign_new_for_display (GdkDisplay *display,
                                                    Window      window);
GdkWindow  *__gdk_x11_window_lookup_for_display      (GdkDisplay *display,
                                                    Window      window);

gint     __gdk_x11_display_text_property_to_text_list (GdkDisplay   *display,
                                                     GdkAtom       encoding,
                                                     gint          format,
                                                     const guchar *text,
                                                     gint          length,
                                                     gchar      ***list);
void     __gdk_x11_free_text_list                     (gchar       **list);
gint     __gdk_x11_display_string_to_compound_text    (GdkDisplay   *display,
                                                     const gchar  *str,
                                                     GdkAtom      *encoding,
                                                     gint         *format,
                                                     guchar      **ctext,
                                                     gint         *length);
gboolean __gdk_x11_display_utf8_to_compound_text      (GdkDisplay   *display,
                                                     const gchar  *str,
                                                     GdkAtom      *encoding,
                                                     gint         *format,
                                                     guchar      **ctext,
                                                     gint         *length);
void     __gdk_x11_free_compound_text                 (guchar       *ctext);


G_END_DECLS

#endif /* __GDK_X_H__ */
