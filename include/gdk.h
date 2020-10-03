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

#ifndef __GDK_H__
#define __GDK_H__

#include <stlwrt.h>
#include <stlwrt-typedefs.h>

#define __GDK_H_INSIDE__

#include <stlwrt.h>
#include <stlwrt-typedefs.h>


#include <gdkapplaunchcontext.h>

#include <gdkcairo.h>

#include <gdkcolor.h>

#include <gdkcursor.h>

#include <gdkdisplay.h>

#include <gdkdisplaymanager.h>

#include <gdkdnd.h>

#include <gdkdrawable.h>

#include <gdkenumtypes.h>

#include <gdkevents.h>

#include <gdkfont.h>

#include <gdkgc.h>

#include <gdkimage.h>

#include <gdkinput.h>

#include <gdkkeys.h>

#include <gdkpango.h>

#include <gdkpixbuf.h>

#include <gdkpixmap.h>

#include <gdkproperty.h>

#include <gdkregion.h>

#include <gdkrgb.h>

#include <gdkscreen.h>

#include <gdkselection.h>

#include <gdkspawn.h>

#include <gdktestutils.h>

#include <gdktypes.h>

#include <gdkvisual.h>

#include <gdkwindow.h>

#undef __GDK_H_INSIDE__

G_BEGIN_DECLS


/* Initialization, exit and events
 */
#define	  GDK_PRIORITY_EVENTS		(G_PRIORITY_DEFAULT)
void 	  SF(gdk_parse_args)	   	(gint	   	*argc,
					 gchar        ***argv);
void 	  SF(gdk_init)		   	(gint	   	*argc,
					 gchar        ***argv);
gboolean  SF(gdk_init_check)   	        (gint	   	*argc,
					 gchar        ***argv);
void SF(gdk_add_option_entries_libgtk_only) (GOptionGroup *group);
void SF(gdk_pre_parse_libgtk_only)          (void);

#ifndef GDK_DISABLE_DEPRECATED
void  	  SF(gdk_exit)		   	(gint	    	 error_code);
gchar*	  SF(gdk_set_locale)	   	(void);
#endif /* GDK_DISABLE_DEPRECATED */

char *gdk_progclass;

const char *         SF(gdk_get_program_class) (void);
void                 SF(gdk_set_program_class) (const char *program_class);

/* Push and pop error handlers for X errors
 */
void      SF(gdk_error_trap_push)           (void);
gint      SF(gdk_error_trap_pop)            (void);

#ifndef GDK_DISABLE_DEPRECATED
void	  SF(gdk_set_use_xshm)		(gboolean	 use_xshm);
gboolean  SF(gdk_get_use_xshm)		(void);
#endif /* GDK_DISABLE_DEPRECATED */

gchar*	                  SF(gdk_get_display)		(void);
const gchar*	          SF(gdk_get_display_arg_name)	(void);

#ifndef GDK_DISABLE_DEPRECATED
gint SF(gdk_input_add_full)	  (gint		     source,
			   GdkInputCondition condition,
			   GdkInputFunction  function,
			   gpointer	     data,
			   GDestroyNotify    destroy);
gint SF(gdk_input_add)	  (gint		     source,
			   GdkInputCondition condition,
			   GdkInputFunction  function,
			   gpointer	     data);
void SF(gdk_input_remove)	  (gint		     tag);
#endif /* GDK_DISABLE_DEPRECATED */

GdkGrabStatus SF(gdk_pointer_grab)       (GdkWindow    *window,
				      gboolean      owner_events,
				      GdkEventMask  event_mask,
				      GdkWindow    *confine_to,
				      GdkCursor    *cursor,
				      guint32       time_);
GdkGrabStatus SF(gdk_keyboard_grab)      (GdkWindow    *window,
				      gboolean      owner_events,
				      guint32       time_);

gboolean SF(gdk_pointer_grab_info_libgtk_only) (GdkDisplay *display,
					    GdkWindow **grab_window,
					    gboolean   *owner_events);
gboolean SF(gdk_keyboard_grab_info_libgtk_only) (GdkDisplay *display,
					     GdkWindow **grab_window,
					     gboolean   *owner_events);

#ifndef GDK_MULTIHEAD_SAFE
void          SF(gdk_pointer_ungrab)     (guint32       time_);
void          SF(gdk_keyboard_ungrab)    (guint32       time_);
gboolean      SF(gdk_pointer_is_grabbed) (void);

gint SF(gdk_screen_width)  (void) G_GNUC_CONST;
gint SF(gdk_screen_height) (void) G_GNUC_CONST;

gint SF(gdk_screen_width_mm)  (void) G_GNUC_CONST;
gint SF(gdk_screen_height_mm) (void) G_GNUC_CONST;

void SF(gdk_beep) (void);
#endif /* GDK_MULTIHEAD_SAFE */

void SF(gdk_flush) (void);

#ifndef GDK_MULTIHEAD_SAFE
void SF(gdk_set_double_click_time)             (guint       msec);
#endif

/* Miscellaneous */
#ifndef GDK_MULTIHEAD_SAFE
gboolean SF(gdk_event_send_client_message)      (GdkEvent       *event,
					     GdkNativeWindow winid);
void     SF(gdk_event_send_clientmessage_toall) (GdkEvent  *event);
#endif
gboolean SF(gdk_event_send_client_message_for_display) (GdkDisplay *display,
						    GdkEvent       *event,
						    GdkNativeWindow winid);

void SF(gdk_notify_startup_complete) (void);

void SF(gdk_notify_startup_complete_with_id) (const gchar* startup_id);

/* Threading
 */

#if !defined (GDK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
GDKVAR GMutex  gdk_threads_mutex; /* private */
#endif

GDKVAR GCallback gdk_threads_lock;
GDKVAR GCallback gdk_threads_unlock;

void     SF(gdk_threads_enter)                    (void);
void     SF(gdk_threads_leave)                    (void);
void     SF(gdk_threads_init)                     (void);
void     SF(gdk_threads_set_lock_functions)       (GCallback enter_fn,
					       GCallback leave_fn);

guint    SF(gdk_threads_add_idle_full)            (gint           priority,
		                               GSourceFunc    function,
		                               gpointer       data,
		                               GDestroyNotify notify);
guint    SF(gdk_threads_add_idle)                 (GSourceFunc    function,
		                               gpointer       data);
guint    SF(gdk_threads_add_timeout_full)         (gint           priority,
                                               guint          interval,
                                               GSourceFunc    function,
                                               gpointer       data,
                                               GDestroyNotify notify);
guint    SF(gdk_threads_add_timeout)              (guint          interval,
                                               GSourceFunc    function,
                                               gpointer       data);
guint    SF(gdk_threads_add_timeout_seconds_full) (gint           priority,
                                               guint          interval,
                                               GSourceFunc    function,
                                               gpointer       data,
                                               GDestroyNotify notify);
guint    SF(gdk_threads_add_timeout_seconds)      (guint          interval,
                                               GSourceFunc    function,
                                               gpointer       data);

#ifdef	G_THREADS_ENABLED
#  define GDK_THREADS_ENTER()	G_STMT_START {	\
      if (gdk_threads_lock)                 	\
        (*gdk_threads_lock) ();			\
   } G_STMT_END
#  define GDK_THREADS_LEAVE()	G_STMT_START { 	\
      if (gdk_threads_unlock)                 	\
        (*gdk_threads_unlock) ();		\
   } G_STMT_END
#else	/* !G_THREADS_ENABLED */
#  define GDK_THREADS_ENTER()
#  define GDK_THREADS_LEAVE()
#endif	/* !G_THREADS_ENABLED */

G_END_DECLS


#endif /* __GDK_H__ */
