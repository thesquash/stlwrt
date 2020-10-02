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

#ifndef __GTK_MAIN_H__
#define __GTK_MAIN_H__

#include <stlwrt.h>


#include <gdk.h>

#include <gtkwidget.h>
#ifdef G_PLATFORM_WIN32

#include <gtkbox.h>

#include <gtkwindow.h>
#endif

G_BEGIN_DECLS

/* Priorities for redrawing and resizing
 */
#define GTK_PRIORITY_RESIZE     (G_PRIORITY_HIGH_IDLE + 10)

#ifndef GTK_DISABLE_DEPRECATED

/* Use GDK_PRIORITY_REDRAW */
#define GTK_PRIORITY_REDRAW     (G_PRIORITY_HIGH_IDLE + 20)

/* Deprecated. Use G_PRIORITY #define's instead
 */
#define GTK_PRIORITY_HIGH       G_PRIORITY_HIGH
#define GTK_PRIORITY_INTERNAL   GTK_PRIORITY_REDRAW
#define GTK_PRIORITY_DEFAULT	G_PRIORITY_DEFAULT_IDLE
#define GTK_PRIORITY_LOW	G_PRIORITY_LOW

#endif /* GTK_DISABLE_DEPRECATED */

typedef gint	(*GtkKeySnoopFunc)	    (GtkWidget	  *grab_widget,
					     GdkEventKey  *event,
					     gpointer	   func_data);

/* Gtk version.
 */
#ifdef G_PLATFORM_WIN32
#ifdef STLWRT_COMPILATION
#define GTKMAIN_C_VAR __declspec(dllexport)
#else
#define GTKMAIN_C_VAR extern __declspec(dllimport)
#endif
#else
#define GTKMAIN_C_VAR extern
#endif


const gchar* SF(gtk_check_version) (guint	required_major,
			        guint	required_minor,
			        guint	required_micro);


/* Initialization, exit, mainloop and miscellaneous routines
 */

gboolean SF(gtk_parse_args)           (int    *argc,
				   char ***argv);

void     SF(gtk_init)                 (int    *argc,
                                   char ***argv);

gboolean SF(gtk_init_check)           (int    *argc,
                                   char ***argv);
  
gboolean SF(gtk_init_with_args)       (int            *argc,
				   char         ***argv,
				   const char     *parameter_string,
				   GOptionEntry   *entries,
				   const char     *translation_domain,
				   GError        **error);

GOptionGroup *SF(gtk_get_option_group) (gboolean open_default_display);
  
#ifdef G_PLATFORM_WIN32

/* Variants that are used to check for correct struct packing
 * when building GTK+-using code.
 */
void	 SF(gtk_init_abi_check)       (int	  *argc,
				   char	***argv,
				   int     num_checks,
				   size_t  sizeof_GtkWindow,
				   size_t  sizeof_GtkBox);
gboolean SF(gtk_init_check_abi_check) (int	  *argc,
				   char	***argv,
				   int     num_checks,
				   size_t  sizeof_GtkWindow,
				   size_t  sizeof_GtkBox);

#define gtk_init(argc, argv) gtk_init_abi_check (argc, argv, 2, sizeof (GtkWindow), sizeof (GtkBox))
#define gtk_init_check(argc, argv) gtk_init_check_abi_check (argc, argv, 2, sizeof (GtkWindow), sizeof (GtkBox))

#endif

#ifndef GTK_DISABLE_DEPRECATED
void     SF(gtk_exit)                 (gint    error_code);
gchar *        SF(gtk_set_locale)           (void);
#endif /* GTK_DISABLE_DEPRECATED */

void           SF(gtk_disable_setlocale)    (void);
PangoLanguage *SF(gtk_get_default_language) (void);
gboolean       SF(gtk_events_pending)       (void);

/* The following is the event func GTK+ registers with GDK
 * we expose it mainly to allow filtering of events between
 * GDK and GTK+.
 */
void 	   SF(gtk_main_do_event)	   (GdkEvent           *event);

void	   SF(gtk_main)		   (void);
guint	   SF(gtk_main_level)	   (void);
void	   SF(gtk_main_quit)	   (void);
gboolean   SF(gtk_main_iteration)	   (void);
/* SF(gtk_main_iteration)() calls SF(gtk_main_iteration_do)(TRUE) */
gboolean   SF(gtk_main_iteration_do)   (gboolean blocking);

gboolean   SF(gtk_true)		   (void) G_GNUC_CONST;
gboolean   SF(gtk_false)		   (void) G_GNUC_CONST;

void	   SF(gtk_grab_add)		   (GtkWidget	       *widget);
GtkWidget* SF(gtk_grab_get_current)	   (void);
void	   SF(gtk_grab_remove)	   (GtkWidget	       *widget);

#if !defined (GTK_DISABLE_DEPRECATED) || defined (STLWRT_COMPILATION)
void	   SF(gtk_init_add)		   (GtkFunction	       function,
				    gpointer	       data);
void	   SF(gtk_quit_add_destroy)	   (guint	       main_level,
				    GObject	      *object);
guint	   SF(gtk_quit_add)		   (guint	       main_level,
				    GtkFunction	       function,
				    gpointer	       data);
guint	   SF(gtk_quit_add_full)	   (guint	       main_level,
				    GtkFunction	       function,
				    GtkCallbackMarshal marshal,
				    gpointer	       data,
				    GDestroyNotify     destroy);
void	   SF(gtk_quit_remove)	   (guint	       quit_handler_id);
void	   SF(gtk_quit_remove_by_data) (gpointer	       data);
guint	   SF(gtk_timeout_add)	   (guint32	       interval,
				    GtkFunction	       function,
				    gpointer	       data);
guint	   SF(gtk_timeout_add_full)	   (guint32	       interval,
				    GtkFunction	       function,
				    GtkCallbackMarshal marshal,
				    gpointer	       data,
				    GDestroyNotify     destroy);
void	   SF(gtk_timeout_remove)	   (guint	       timeout_handler_id);

guint	   SF(gtk_idle_add)		   (GtkFunction	       function,
				    gpointer	       data);
guint	   SF(gtk_idle_add_priority)   (gint	       priority,
				    GtkFunction	       function,
				    gpointer	       data);
guint	   SF(gtk_idle_add_full)	   (gint	       priority,
				    GtkFunction	       function,
				    GtkCallbackMarshal marshal,
				    gpointer	       data,
				    GDestroyNotify     destroy);
void	   SF(gtk_idle_remove)	   (guint	       idle_handler_id);
void	   SF(gtk_idle_remove_by_data) (gpointer	       data);
guint	   SF(gtk_input_add_full)	   (gint	       source,
				    GdkInputCondition  condition,
				    GdkInputFunction   function,
				    GtkCallbackMarshal marshal,
				    gpointer	       data,
				    GDestroyNotify     destroy);
void	   SF(gtk_input_remove)	   (guint	       input_handler_id);
#endif /* GTK_DISABLE_DEPRECATED */

guint	   SF(gtk_key_snooper_install) (GtkKeySnoopFunc snooper,
				    gpointer	    func_data);
void	   SF(gtk_key_snooper_remove)  (guint	    snooper_handler_id);

GdkEvent*       SF(gtk_get_current_event)       (void);
guint32         SF(gtk_get_current_event_time)  (void);
gboolean        SF(gtk_get_current_event_state) (GdkModifierType *state);

GtkWidget* SF(gtk_get_event_widget)	   (GdkEvent	   *event);


/* Private routines internal to GTK+ 
 */
void       SF(gtk_propagate_event)     (GtkWidget         *widget,
				    GdkEvent          *event);

gboolean SF(_gtk_boolean_handled_accumulator) (GSignalInvocationHint *ihint,
                                   GValue                *return_accu,
                                   const GValue          *handler_return,
                                   gpointer               dummy);

gchar *SF(_gtk_get_lc_ctype) (void);

gboolean SF(_gtk_module_has_mixed_deps) (GModule *module);


G_END_DECLS

#endif /* __GTK_MAIN_H__ */
