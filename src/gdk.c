
/* GDK - The GIMP Drawing Kit
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



#include "config.h"

#include <stlwrtobject.h>

#include <string.h>
#include <stdlib.h>

#include <gdk.h>
#include <gdkinternals.h>
#include <gdkintl.h>

#ifndef HAVE_XCONVERTCASE
#include <gdkkeysyms.h>
#endif


typedef struct _GdkPredicate  GdkPredicate;

struct _GdkPredicate
{
  GdkEventFunc func;
  gpointer data;
};

typedef struct _GdkThreadsDispatch GdkThreadsDispatch;

struct _GdkThreadsDispatch
{
  GSourceFunc func;
  gpointer data;
  GDestroyNotify destroy;
};


/* Private variable declarations
 */
static int __gdk_initialized = 0;			    /* 1 if the library is initialized,
						     * 0 otherwise.
						     */

#ifdef G_ENABLE_DEBUG
static const GDebugKey gdk_debug_keys[] = {
  {"events",	    GDK_DEBUG_EVENTS},
  {"misc",	    GDK_DEBUG_MISC},
  {"dnd",	    GDK_DEBUG_DND},
  {"xim",	    GDK_DEBUG_XIM},
  {"nograbs",       GDK_DEBUG_NOGRABS},
  {"colormap",	    GDK_DEBUG_COLORMAP},
  {"gdkrgb",	    GDK_DEBUG_GDKRGB},
  {"gc",	    GDK_DEBUG_GC},
  {"pixmap",	    GDK_DEBUG_PIXMAP},
  {"image",	    GDK_DEBUG_IMAGE},
  {"input",	    GDK_DEBUG_INPUT},
  {"cursor",	    GDK_DEBUG_CURSOR},
  {"multihead",	    GDK_DEBUG_MULTIHEAD},
  {"xinerama",	    GDK_DEBUG_XINERAMA},
  {"draw",	    GDK_DEBUG_DRAW},
  {"eventloop",	    GDK_DEBUG_EVENTLOOP}
};

static const int gdk_ndebug_keys = G_N_ELEMENTS (gdk_debug_keys);

#endif /* G_ENABLE_DEBUG */


/**
 * __gdk_display_open_default_libgtk_only:
 * 
 * Opens the default display specified by command line arguments or
 * environment variables, sets it as the default display, and returns
 * it.  __gdk_parse_args must have been called first. If the default
 * display has previously been set, simply returns that. An internal
 * function that should not be used by applications.
 * 
 * Return value: the default display, if it could be opened,
 *   otherwise %NULL.
 **/
GdkDisplay *
__gdk_display_open_default_libgtk_only (void)
{
  GdkDisplay *display;

  g_return_val_if_fail (__gdk_initialized, NULL);
  
  display = __gdk_display_get_default ();
  if (display)
    return display;

  display = __gdk_display_open (__gdk_get_display_arg_name ());

  if (!display && _gdk_screen_number >= 0)
    {
      g_free (_gdk_display_arg_name);
      _gdk_display_arg_name = g_strdup (_gdk_display_name);
      
      display = __gdk_display_open (_gdk_display_name);
    }
  
  if (display)
    __gdk_display_manager_set_default_display (__gdk_display_manager_get (),
					     display);
  
  return display;
}

/**
 * __gdk_init_check:
 *
 *   Initialize the library for use.
 *
 * Results:
 *   If initialization fails, returns FALSE, otherwise TRUE.
 *
 * Side effects:
 *   The library is initialized.
 *
 *--------------------------------------------------------------
 */
gboolean
__gdk_init_check (int * argc, char *** argv)
{
  (void) argc;
  (void) argv;
  
  return __gdk_display_open_default_libgtk_only () != NULL;
}


/**
 * __gdk_init:
 * @argc: (inout):
 * @argv: (array length=argc) (inout):
 */
void
__gdk_init (int *argc, char ***argv)
{
  
  /* STLWRT actually just silently ignores argc and argv.  Unless someone tells
     me otherwise, environment variables are really better for the purpose
     anyway. */
  
  if (!__gdk_init_check (argc, argv))
    {
      g_error("No usable graphical output backends available.");
      g_error("Are you running an X or Wayland server?");
      g_error("Also be sure to set the DISPLAY environment");
      g_error("variable to reflect whatever display system you");
      g_error("might be using; on X, you can use a command");
      g_error("like this:");
      g_error("   export DISPLAY=\":0\"");
      g_error("On Wayland you can use a command like this:");
      g_error("   export DISPLAY=\"wayland-0\"");
      exit(1);
    }
  
  
  
  __gdk_initialized = TRUE;

  /* We set the fallback program class here, rather than lazily in
   * __gdk_get_program_class, since we don't want -name to override it.
   */
  gdk_progclass = g_strdup (g_get_prgname ());
  if (gdk_progclass && gdk_progclass[0])
    gdk_progclass[0] = g_ascii_toupper (gdk_progclass[0]);
  
#ifdef G_ENABLE_DEBUG
  {
    gchar *debug_string = getenv("GDK_DEBUG");
    if (debug_string != NULL)
      _gdk_debug_flags = g_parse_debug_string (debug_string,
					      (GDebugKey *) gdk_debug_keys,
					      gdk_ndebug_keys);
  }
#endif	/* G_ENABLE_DEBUG */

  if (getenv ("GDK_NATIVE_WINDOWS"))
    {
      _gdk_native_windows = TRUE;
      /* Ensure that this is not propagated
	 to spawned applications */
      g_unsetenv ("GDK_NATIVE_WINDOWS");
    }

#if 0
  /* We don't need to call g_type_init() anymore in GLib; in fact, it's
   * deprecated now. */
  g_type_init ();
#endif

  /* Do any setup particular to the windowing system
   */
  _gdk_windowing_init ();
}

/*
 *--------------------------------------------------------------
 * __gdk_exit
 *
 *   Restores the library to an un-itialized state and exits
 *   the program using the "exit" system call.
 *
 * Arguments:
 *   "errorcode" is the error value to pass to "exit".
 *
 * Results:
 *   Allocated structures are freed and the program exits
 *   cleanly.
 *
 * Side effects:
 *
 *--------------------------------------------------------------
 */

void
__gdk_exit (gint errorcode)
{
  exit (errorcode);
}

void
__gdk_threads_enter (void)
{
  GDK_THREADS_ENTER ();
}

void
__gdk_threads_leave (void)
{
  GDK_THREADS_LEAVE ();
}

static void
gdk_threads_impl_lock (void)
{
    g_mutex_lock (&gdk_threads_mutex);
}

static void
gdk_threads_impl_unlock (void)
{
      /* we need a trylock() here because trying to unlock a mutex
       * that hasn't been locked yet is:
       *
       *  a) not portable
       *  b) fail on GLib ≥ 2.41
       *
       * trylock() will either succeed because nothing is holding the
       * GDK mutex, and will be unlocked right afterwards; or it's
       * going to fail because the mutex is locked already, in which
       * case we unlock it as expected.
       *
       * this is needed in the case somebody called __gdk_threads_init()
       * without calling __gdk_threads_enter() before calling __gtk_main().
       * in theory, we could just say that this is undefined behaviour,
       * but our documentation has always been *less* than explicit as
       * to what the behaviour should actually be.
       *
       * see bug: https://bugzilla.gnome.org/show_bug.cgi?id=735428
       */
      g_mutex_trylock (&gdk_threads_mutex);
      g_mutex_unlock (&gdk_threads_mutex);
}

/**
 * __gdk_threads_init:
 * 
 * Initializes GDK so that it can be used from multiple threads
 * in conjunction with __gdk_threads_enter() and __gdk_threads_leave().
 * g_thread_init() must be called previous to this function.
 *
 * This call must be made before any use of the main loop from
 * GTK+; to be safe, call it before __gtk_init().
 **/
void
__gdk_threads_init (void)
{
  if (!g_thread_supported ())
    g_error ("g_thread_init() must be called before __gdk_threads_init()");

  g_mutex_init (&gdk_threads_mutex);
  
  if (!gdk_threads_lock)
    gdk_threads_lock = gdk_threads_impl_lock;
  if (!gdk_threads_unlock)
    gdk_threads_unlock = gdk_threads_impl_unlock;
}

/**
 * __gdk_threads_set_lock_functions:
 * @enter_fn:   function called to guard GDK
 * @leave_fn: function called to release the guard
 *
 * Allows the application to replace the standard method that
 * GDK uses to protect its data structures. Normally, GDK
 * creates a single #GMutex that is locked by __gdk_threads_enter(),
 * and released by __gdk_threads_leave(); using this function an
 * application provides, instead, a function @enter_fn that is
 * called by __gdk_threads_enter() and a function @leave_fn that is
 * called by __gdk_threads_leave().
 *
 * The functions must provide at least same locking functionality
 * as the default implementation, but can also do extra application
 * specific processing.
 *
 * As an example, consider an application that has its own recursive
 * lock that when held, holds the GTK+ lock as well. When GTK+ unlocks
 * the GTK+ lock when entering a recursive main loop, the application
 * must temporarily release its lock as well.
 *
 * Most threaded GTK+ apps won't need to use this method.
 *
 * This method must be called before __gdk_threads_init(), and cannot
 * be called multiple times.
 *
 * Since: 2.4
 **/
void
__gdk_threads_set_lock_functions (GCallback enter_fn,
				GCallback leave_fn)
{
  g_return_if_fail (gdk_threads_lock == NULL &&
		    gdk_threads_unlock == NULL);

  gdk_threads_lock = enter_fn;
  gdk_threads_unlock = leave_fn;
}

static gboolean
gdk_threads_dispatch (gpointer data)
{
  GdkThreadsDispatch *dispatch = data;
  gboolean ret = FALSE;

  GDK_THREADS_ENTER ();

  if (!g_source_is_destroyed (g_main_current_source ()))
    ret = dispatch->func (dispatch->data);

  GDK_THREADS_LEAVE ();

  return ret;
}

static void
gdk_threads_dispatch_free (gpointer data)
{
  GdkThreadsDispatch *dispatch = data;

  if (dispatch->destroy && dispatch->data)
    dispatch->destroy (dispatch->data);

  g_slice_free (GdkThreadsDispatch, data);
}


/**
 * __gdk_threads_add_idle_full:
 * @priority: the priority of the idle source. Typically this will be in the
 *            range btweeen #G_PRIORITY_DEFAULT_IDLE and #G_PRIORITY_HIGH_IDLE
 * @function: function to call
 * @data:     data to pass to @function
 * @notify: (allow-none):   function to call when the idle is removed, or %NULL
 *
 * Adds a function to be called whenever there are no higher priority
 * events pending.  If the function returns %FALSE it is automatically
 * removed from the list of event sources and will not be called again.
 *
 * This variant of g_idle_add_full() calls @function with the GDK lock
 * held. It can be thought of a MT-safe version for GTK+ widgets for the 
 * following use case, where you have to worry about idle_callback()
 * running in thread A and accessing @self after it has been finalized
 * in thread B:
 *
 * |[
 * static gboolean
 * idle_callback (gpointer data)
 * {
 *    /&ast; __gdk_threads_enter(); would be needed for g_idle_add() &ast;/
 *
 *    SomeWidget *self = data;
 *    /&ast; do stuff with self &ast;/
 *
 *    self->idle_id = 0;
 *
 *    /&ast; __gdk_threads_leave(); would be needed for g_idle_add() &ast;/
 *    return FALSE;
 * }
 *
 * static void
 * some_widget_do_stuff_later (SomeWidget *self)
 * {
 *    self->idle_id = __gdk_threads_add_idle (idle_callback, self)
 *    /&ast; using g_idle_add() here would require thread protection in the callback &ast;/
 * }
 *
 * static void
 * some_widget_finalize (GObject *object)
 * {
 *    SomeWidget *self = SOME_WIDGET (object);
 *    if (self->idle_id)
 *      g_source_remove (self->idle_id);
 *    G_OBJECT_CLASS (parent_class)->finalize (object);
 * }
 * ]|
 *
 * Return value: the ID (greater than 0) of the event source.
 *
 * Since: 2.12
 */
guint
__gdk_threads_add_idle_full (gint           priority,
		           GSourceFunc    function,
		           gpointer       data,
		           GDestroyNotify notify)
{
  GdkThreadsDispatch *dispatch;

  g_return_val_if_fail (function != NULL, 0);

  dispatch = g_slice_new (GdkThreadsDispatch);
  dispatch->func = function;
  dispatch->data = data;
  dispatch->destroy = notify;

  return g_idle_add_full (priority,
                          gdk_threads_dispatch,
                          dispatch,
                          gdk_threads_dispatch_free);
}

/**
 * __gdk_threads_add_idle:
 * @function: function to call
 * @data:     data to pass to @function
 *
 * A wrapper for the common usage of __gdk_threads_add_idle_full() 
 * assigning the default priority, #G_PRIORITY_DEFAULT_IDLE.
 *
 * See __gdk_threads_add_idle_full().
 *
 * Return value: the ID (greater than 0) of the event source.
 * 
 * Since: 2.12
 */
guint
__gdk_threads_add_idle (GSourceFunc    function,
		      gpointer       data)
{
  return __gdk_threads_add_idle_full (G_PRIORITY_DEFAULT_IDLE,
                                    function, data, NULL);
}


/**
 * __gdk_threads_add_timeout_full:
 * @priority: the priority of the timeout source. Typically this will be in the
 *            range between #G_PRIORITY_DEFAULT_IDLE and #G_PRIORITY_HIGH_IDLE.
 * @interval: the time between calls to the function, in milliseconds
 *             (1/1000ths of a second)
 * @function: function to call
 * @data:     data to pass to @function
 * @notify: (allow-none):   function to call when the timeout is removed, or %NULL
 *
 * Sets a function to be called at regular intervals holding the GDK lock,
 * with the given priority.  The function is called repeatedly until it 
 * returns %FALSE, at which point the timeout is automatically destroyed 
 * and the function will not be called again.  The @notify function is
 * called when the timeout is destroyed.  The first call to the
 * function will be at the end of the first @interval.
 *
 * Note that timeout functions may be delayed, due to the processing of other
 * event sources. Thus they should not be relied on for precise timing.
 * After each call to the timeout function, the time of the next
 * timeout is recalculated based on the current time and the given interval
 * (it does not try to 'catch up' time lost in delays).
 *
 * This variant of g_timeout_add_full() can be thought of a MT-safe version 
 * for GTK+ widgets for the following use case:
 *
 * |[
 * static gboolean timeout_callback (gpointer data)
 * {
 *    SomeWidget *self = data;
 *    
 *    /&ast; do stuff with self &ast;/
 *    
 *    self->timeout_id = 0;
 *    
 *    return FALSE;
 * }
 *  
 * static void some_widget_do_stuff_later (SomeWidget *self)
 * {
 *    self->timeout_id = g_timeout_add (timeout_callback, self)
 * }
 *  
 * static void some_widget_finalize (GObject *object)
 * {
 *    SomeWidget *self = SOME_WIDGET (object);
 *    
 *    if (self->timeout_id)
 *      g_source_remove (self->timeout_id);
 *    
 *    G_OBJECT_CLASS (parent_class)->finalize (object);
 * }
 * ]|
 *
 * Return value: the ID (greater than 0) of the event source.
 * 
 * Since: 2.12
 */
guint
__gdk_threads_add_timeout_full (gint           priority,
                              guint          interval,
                              GSourceFunc    function,
                              gpointer       data,
                              GDestroyNotify notify)
{
  GdkThreadsDispatch *dispatch;

  g_return_val_if_fail (function != NULL, 0);

  dispatch = g_slice_new (GdkThreadsDispatch);
  dispatch->func = function;
  dispatch->data = data;
  dispatch->destroy = notify;

  return g_timeout_add_full (priority, 
                             interval,
                             gdk_threads_dispatch, 
                             dispatch, 
                             gdk_threads_dispatch_free);
}

/**
 * __gdk_threads_add_timeout:
 * @interval: the time between calls to the function, in milliseconds
 *             (1/1000ths of a second)
 * @function: function to call
 * @data:     data to pass to @function
 *
 * A wrapper for the common usage of __gdk_threads_add_timeout_full() 
 * assigning the default priority, #G_PRIORITY_DEFAULT.
 *
 * See __gdk_threads_add_timeout_full().
 * 
 * Return value: the ID (greater than 0) of the event source.
 *
 * Since: 2.12
 */
guint
__gdk_threads_add_timeout (guint       interval,
                         GSourceFunc function,
                         gpointer    data)
{
  return __gdk_threads_add_timeout_full (G_PRIORITY_DEFAULT,
                                       interval, function, data, NULL);
}


/**
 * __gdk_threads_add_timeout_seconds_full:
 * @priority: the priority of the timeout source. Typically this will be in the
 *            range between #G_PRIORITY_DEFAULT_IDLE and #G_PRIORITY_HIGH_IDLE.
 * @interval: the time between calls to the function, in seconds
 * @function: function to call
 * @data:     data to pass to @function
 * @notify: (allow-none):   function to call when the timeout is removed, or %NULL
 *
 * A variant of gdk_threads_add_timout_full() with second-granularity.
 * See g_timeout_add_seconds_full() for a discussion of why it is
 * a good idea to use this function if you don't need finer granularity.
 *
 *  Return value: the ID (greater than 0) of the event source.
 * 
 * Since: 2.14
 */
guint
__gdk_threads_add_timeout_seconds_full (gint           priority,
                                      guint          interval,
                                      GSourceFunc    function,
                                      gpointer       data,
                                      GDestroyNotify notify)
{
  GdkThreadsDispatch *dispatch;

  g_return_val_if_fail (function != NULL, 0);

  dispatch = g_slice_new (GdkThreadsDispatch);
  dispatch->func = function;
  dispatch->data = data;
  dispatch->destroy = notify;

  return g_timeout_add_seconds_full (priority, 
                                     interval,
                                     gdk_threads_dispatch, 
                                     dispatch, 
                                     gdk_threads_dispatch_free);
}

/**
 * __gdk_threads_add_timeout_seconds:
 * @interval: the time between calls to the function, in seconds
 * @function: function to call
 * @data:     data to pass to @function
 *
 * A wrapper for the common usage of __gdk_threads_add_timeout_seconds_full() 
 * assigning the default priority, #G_PRIORITY_DEFAULT.
 *
 * For details, see __gdk_threads_add_timeout_full().
 * 
 * Return value: the ID (greater than 0) of the event source.
 *
 * Since: 2.14
 */
guint
__gdk_threads_add_timeout_seconds (guint       interval,
                                 GSourceFunc function,
                                 gpointer    data)
{
  return __gdk_threads_add_timeout_seconds_full (G_PRIORITY_DEFAULT,
                                               interval, function, data, NULL);
}


const char *
__gdk_get_program_class (void)
{
  return gdk_progclass;
}

void
__gdk_set_program_class (const char *program_class)
{
  g_free (gdk_progclass);

  gdk_progclass = g_strdup (program_class);
}

#define __GDK_C__
