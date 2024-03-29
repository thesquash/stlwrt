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



#include "config.h"

#include <stlwrt.h>

#include <gtkmain.h>

#include <glib.h>
#include <gdkconfig.h>

#include <locale.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif
#include <sys/types.h>		/* For uid_t, gid_t */

#ifdef G_OS_WIN32
#define STRICT
#include <windows.h>
#undef STRICT
#endif

#include <gtkintl.h>

#include <gtkaccelmap.h>
#include <gtkbox.h>
#include <gtkclipboard.h>
#include <gtkdnd.h>
#include <gtkversion.h>
#include <gtkmodules.h>
#include <gtkrc.h>
#include <gtkrecentmanager.h>
#include <gtkselection.h>
#include <gtksettings.h>
#include <gtkwidget.h>
#include <gtkwindow.h>
#include <gtktooltip.h>
#include <gtkdebug.h>

#include <gtkmenu.h>
#include <gdkkeysyms.h>

#include <gdkprivate.h> /* for GDK_WINDOW_DESTROYED */

#ifdef G_OS_WIN32

static HMODULE gtk_dll;

BOOL WINAPI
DllMain (HINSTANCE hinstDLL,
	 DWORD     fdwReason,
	 LPVOID    lpvReserved)
{
  switch (fdwReason)
    {
    case DLL_PROCESS_ATTACH:
      gtk_dll = (HMODULE) hinstDLL;
      break;
    }

  return TRUE;
}

/* These here before inclusion of gtkprivate.h so that the original
 * STLWRT_LIBDIR and STLWRT_LOCALEDIR definitions are seen. Yeah, this is a
 * bit sucky.
 */
const gchar *
_gtk_get_libdir (void)
{
  static char *gtk_libdir = NULL;
  if (gtk_libdir == NULL)
    {
      gchar *root = g_win32_get_package_installation_directory_of_module (gtk_dll);
      gchar *slash = root ? strrchr (root, '\\') : NULL;
      if (slash != NULL &&
          g_ascii_strcasecmp (slash + 1, ".libs") == 0)
	gtk_libdir = STLWRT_LIBDIR;
      else
	gtk_libdir = g_build_filename (root, "lib", NULL);
      g_free (root);
    }

  return gtk_libdir;
}

const gchar *
_gtk_get_localedir (void)
{
  static char *gtk_localedir = NULL;
  if (gtk_localedir == NULL)
    {
      const gchar *p;
      gchar *root, *temp;
      
      /* STLWRT_LOCALEDIR ends in either /lib/locale or
       * /share/locale. Scan for that slash.
       */
      p = STLWRT_LOCALEDIR + strlen (STLWRT_LOCALEDIR);
      while (*--p != '/')
	;
      while (*--p != '/')
	;

      root = g_win32_get_package_installation_directory_of_module (gtk_dll);
      temp = g_build_filename (root, p, NULL);
      g_free (root);

      /* gtk_localedir is passed to bindtextdomain() which isn't
       * UTF-8-aware.
       */
      gtk_localedir = g_win32_locale_filename_from_utf8 (temp);
      g_free (temp);
    }
  return gtk_localedir;
}

#endif

#include <gtkprivate.h>

/* Private type definitions
 */

static GtkWindowGroup *__gtk_main_get_window_group (GtkWidget   *widget);

/* In GTK, the "gtk_*_version" and "gtk_*_age" constants were declared here;
   in STLWRT, they are declared in gtk2/gtkversion.c and gtk3/gtkversion.c. */

static guint __gtk_main_loop_level = 0;
static gint pre_initialized = FALSE;
static gint __gtk_initialized = FALSE;
static GList *current_events = NULL;

static GSList *main_loops = NULL;      /* stack of currently executing main loops */

guint gtk_debug_flags = 0;		   /* Global GTK debug flag */

#ifdef G_ENABLE_DEBUG
static const GDebugKey gtk_debug_keys[] = {
  {"misc", GTK_DEBUG_MISC},
  {"plugsocket", GTK_DEBUG_PLUGSOCKET},
  {"text", GTK_DEBUG_TEXT},
  {"tree", GTK_DEBUG_TREE},
  {"updates", GTK_DEBUG_UPDATES},
  {"keybindings", GTK_DEBUG_KEYBINDINGS},
  {"multihead", GTK_DEBUG_MULTIHEAD},
  {"modules", GTK_DEBUG_MODULES},
  {"geometry", GTK_DEBUG_GEOMETRY},
  {"icontheme", GTK_DEBUG_ICONTHEME},
  {"printing", GTK_DEBUG_PRINTING},
  {"builder", GTK_DEBUG_BUILDER}
};
#endif /* G_ENABLE_DEBUG */

/**
 * __gtk_check_version:
 * @required_major: the required major version.
 * @required_minor: the required minor version.
 * @required_micro: the required micro version.
 * 
 * Checks that the GTK+ library in use is compatible with the
 * given version. Generally you would pass in the constants
 * #GTK_MAJOR_VERSION, #GTK_MINOR_VERSION, #GTK_MICRO_VERSION
 * as the three arguments to this function; that produces
 * a check that the library in use is compatible with
 * the version of GTK+ the application or module was compiled
 * against.
 *
 * Compatibility is defined by two things: first the version
 * of the running library is newer than the version
 * @required_major.required_minor.@required_micro. Second
 * the running library must be binary compatible with the
 * version @required_major.required_minor.@required_micro
 * (same major version.)
 *
 * This function is primarily for GTK+ modules; the module
 * can call this function to check that it wasn't loaded
 * into an incompatible version of GTK+. However, such a
 * a check isn't completely reliable, since the module may be
 * linked against an old version of GTK+ and calling the
 * old version of __gtk_check_version(), but still get loaded
 * into an application using a newer version of GTK+.
 *
 * Return value: %NULL if the GTK+ library is compatible with the
 *   given version, or a string describing the version mismatch.
 *   The returned string is owned by GTK+ and should not be modified
 *   or freed.
 **/
const gchar*
__gtk_check_version (guint required_major,
		   guint required_minor,
		   guint required_micro)
{
#if 0
  gint gtk_effective_micro = 100 * gtk_minor_version +
                                   gtk_micro_version;
  
  gint required_effective_micro = 100 * required_minor + required_micro;

  if (required_major > gtk_major_version)
    return "Gtk+ version too old (major mismatch)";
  if (required_major < gtk_major_version)
    return "Gtk+ version too new (major mismatch)";
  if (required_effective_micro < gtk_effective_micro - gtk_binary_age)
    return "Gtk+ version too new (micro mismatch)";
  if (required_effective_micro > gtk_effective_micro)
    return "Gtk+ version too old (micro mismatch)";
  return NULL;
#endif
  /* The whole point of STLWRT is to let applications and modules from
     different versions of GTK be used together.  Don't even bother checking.
     (Until we get complaints of huge crashes at least.) */
  return NULL;
}

/* This checks to see if the process is running suid or sgid
 * at the current time. If so, we don't allow GTK+ to be initialized.
 * This is meant to be a mild check - we only error out if we
 * can prove the programmer is doing something wrong, not if
 * they could be doing something wrong. For this reason, we
 * don't use issetugid() on BSD or prctl (PR_GET_DUMPABLE).
 */
static gboolean
check_setugid (void)
{
/* this isn't at all relevant on MS Windows and doesn't compile ... --hb */
#ifndef G_OS_WIN32
  uid_t ruid, euid, suid; /* Real, effective and saved user ID's */
  gid_t rgid, egid, sgid; /* Real, effective and saved group ID's */
  
#ifdef HAVE_GETRESUID
  /* These aren't in the header files, so we prototype them here.
   */
  int getresuid(uid_t *ruid, uid_t *euid, uid_t *suid);
  int getresgid(gid_t *rgid, gid_t *egid, gid_t *sgid);

  if (getresuid (&ruid, &euid, &suid) != 0 ||
      getresgid (&rgid, &egid, &sgid) != 0)
#endif /* HAVE_GETRESUID */
    {
      suid = ruid = getuid ();
      sgid = rgid = getgid ();
      euid = geteuid ();
      egid = getegid ();
    }

  if (ruid != euid || ruid != suid ||
      rgid != egid || rgid != sgid)
    {
      g_warning ("This process is currently running setuid or setgid.\n"
		 "This is not a supported use of GTK+. You must create a helper\n"
		 "program instead. For further details, see:\n\n"
		 "    http://www.gtk.org/setuid.html\n\n"
		 "Refusing to initialize GTK+.");
      exit (1);
    }
#endif
  return TRUE;
}

#ifdef G_OS_WIN32

const gchar *
_gtk_get_datadir (void)
{
  static char *gtk_datadir = NULL;
  if (gtk_datadir == NULL)
    {
      gchar *root = g_win32_get_package_installation_directory_of_module (gtk_dll);
      gtk_datadir = g_build_filename (root, "share", NULL);
      g_free (root);
    }

  return gtk_datadir;
}

const gchar *
_gtk_get_sysconfdir (void)
{
  static char *gtk_sysconfdir = NULL;
  if (gtk_sysconfdir == NULL)
    {
      gchar *root = g_win32_get_package_installation_directory_of_module (gtk_dll);
      gtk_sysconfdir = g_build_filename (root, "etc", NULL);
      g_free (root);
    }

  return gtk_sysconfdir;
}

const gchar *
_gtk_get_data_prefix (void)
{
  static char *gtk_data_prefix = NULL;
  if (gtk_data_prefix == NULL)
    gtk_data_prefix = g_win32_get_package_installation_directory_of_module (gtk_dll);

  return gtk_data_prefix;
}

#endif /* G_OS_WIN32 */

static gboolean do_setlocale = TRUE;

#ifdef G_PLATFORM_WIN32
#undef __gtk_init_check
#endif

static GString *gtk_modules_string = NULL;
static gboolean g_fatal_warnings = FALSE;

#ifdef G_ENABLE_DEBUG
static gboolean
gtk_arg_debug_cb (const char *key, const char *value, gpointer user_data)
{
  gtk_debug_flags |= g_parse_debug_string (value,
					   gtk_debug_keys,
					   G_N_ELEMENTS (gtk_debug_keys));

  return TRUE;
}

static gboolean
gtk_arg_no_debug_cb (const char *key, const char *value, gpointer user_data)
{
  gtk_debug_flags &= ~g_parse_debug_string (value,
					    gtk_debug_keys,
					    G_N_ELEMENTS (gtk_debug_keys));

  return TRUE;
}
#endif /* G_ENABLE_DEBUG */

static gboolean
gtk_arg_module_cb (const char *key, const char *value, gpointer user_data)
{
  if (value && *value)
    {
      if (gtk_modules_string)
	g_string_append_c (gtk_modules_string, G_SEARCHPATH_SEPARATOR);
      else
	gtk_modules_string = g_string_new (NULL);
      
      g_string_append (gtk_modules_string, value);
    }

  return TRUE;
}

static const GOptionEntry gtk_args[] = {
  { "gtk-module",       0, 0, G_OPTION_ARG_CALLBACK, gtk_arg_module_cb,   
    /* Description of --gtk-module=MODULES in --help output */ N_("Load additional GTK+ modules"), 
    /* Placeholder in --gtk-module=MODULES in --help output */ N_("MODULES") },
  { "g-fatal-warnings", 0, 0, G_OPTION_ARG_NONE, &g_fatal_warnings, 
    /* Description of --g-fatal-warnings in --help output */   N_("Make all warnings fatal"), NULL },
#ifdef G_ENABLE_DEBUG
  { "gtk-debug",        0, 0, G_OPTION_ARG_CALLBACK, gtk_arg_debug_cb,    
    /* Description of --gtk-debug=FLAGS in --help output */    N_("GTK+ debugging flags to set"), 
    /* Placeholder in --gtk-debug=FLAGS in --help output */    N_("FLAGS") },
  { "gtk-no-debug",     0, 0, G_OPTION_ARG_CALLBACK, gtk_arg_no_debug_cb, 
    /* Description of --gtk-no-debug=FLAGS in --help output */ N_("GTK+ debugging flags to unset"), 
    /* Placeholder in --gtk-no-debug=FLAGS in --help output */ N_("FLAGS") },
#endif 
  { NULL }
};

#ifdef G_OS_WIN32

static char *iso639_to_check = NULL;
static char *iso3166_to_check = NULL;
static char *script_to_check = NULL;
static gboolean setlocale_called = FALSE;

static BOOL CALLBACK
enum_locale_proc (LPTSTR locale)
{
  LCID lcid;
  char iso639[10];
  char iso3166[10];
  char *endptr;


  lcid = strtoul (locale, &endptr, 16);
  if (*endptr == '\0' &&
      GetLocaleInfo (lcid, LOCALE_SISO639LANGNAME, iso639, sizeof (iso639)) &&
      GetLocaleInfo (lcid, LOCALE_SISO3166CTRYNAME, iso3166, sizeof (iso3166)))
    {
      if (strcmp (iso639, iso639_to_check) == 0 &&
	  ((iso3166_to_check != NULL &&
	    strcmp (iso3166, iso3166_to_check) == 0) ||
	   (iso3166_to_check == NULL &&
	    SUBLANGID (LANGIDFROMLCID (lcid)) == SUBLANG_DEFAULT)))
	{
	  char language[100], country[100];
	  char locale[300];

	  if (script_to_check != NULL)
	    {
	      /* If lcid is the "other" script for this language,
	       * return TRUE, i.e. continue looking.
	       */
	      if (strcmp (script_to_check, "Latn") == 0)
		{
		  switch (LANGIDFROMLCID (lcid))
		    {
		    case MAKELANGID (LANG_AZERI, SUBLANG_AZERI_CYRILLIC):
		      return TRUE;
		    case MAKELANGID (LANG_UZBEK, SUBLANG_UZBEK_CYRILLIC):
		      return TRUE;
		    case MAKELANGID (LANG_SERBIAN, SUBLANG_SERBIAN_CYRILLIC):
		      return TRUE;
		    case MAKELANGID (LANG_SERBIAN, 0x07):
		      /* Serbian in Bosnia and Herzegovina, Cyrillic */
		      return TRUE;
		    }
		}
	      else if (strcmp (script_to_check, "Cyrl") == 0)
		{
		  switch (LANGIDFROMLCID (lcid))
		    {
		    case MAKELANGID (LANG_AZERI, SUBLANG_AZERI_LATIN):
		      return TRUE;
		    case MAKELANGID (LANG_UZBEK, SUBLANG_UZBEK_LATIN):
		      return TRUE;
		    case MAKELANGID (LANG_SERBIAN, SUBLANG_SERBIAN_LATIN):
		      return TRUE;
		    case MAKELANGID (LANG_SERBIAN, 0x06):
		      /* Serbian in Bosnia and Herzegovina, Latin */
		      return TRUE;
		    }
		}
	    }

	  SetThreadLocale (lcid);

	  if (GetLocaleInfo (lcid, LOCALE_SENGLANGUAGE, language, sizeof (language)) &&
	      GetLocaleInfo (lcid, LOCALE_SENGCOUNTRY, country, sizeof (country)))
	    {
	      strcpy (locale, language);
	      strcat (locale, "_");
	      strcat (locale, country);

	      if (setlocale (LC_ALL, locale) != NULL)
		setlocale_called = TRUE;
	    }

	  return FALSE;
	}
    }

  return TRUE;
}
  
#endif

static void
setlocale_initialization (void)
{
  static gboolean initialized = FALSE;

  if (initialized)
    return;
  initialized = TRUE;

  if (!setlocale (LC_ALL, ""))
    g_warning ("Locale not supported by C library.\n\tUsing the fallback 'C' locale.");
}

static void
gettext_initialization (void)
{
  setlocale_initialization ();

#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, STLWRT_LOCALEDIR);
  bindtextdomain (GETTEXT_PACKAGE "-properties", STLWRT_LOCALEDIR);
#    ifdef HAVE_BIND_TEXTDOMAIN_CODESET
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  bind_textdomain_codeset (GETTEXT_PACKAGE "-properties", "UTF-8");
#    endif
#endif  
}

static void
do_post_parse_initialization (int    *argc,
			      char ***argv)
{
  if (__gtk_initialized)
    return;

  gettext_initialization ();

#ifdef SIGPIPE
  signal (SIGPIPE, SIG_IGN);
#endif

  if (g_fatal_warnings)
    {
      GLogLevelFlags fatal_mask;

      fatal_mask = g_log_set_always_fatal (G_LOG_FATAL_MASK);
      fatal_mask |= G_LOG_LEVEL_WARNING | G_LOG_LEVEL_CRITICAL;
      g_log_set_always_fatal (fatal_mask);
    }

  if (gtk_debug_flags & GTK_DEBUG_UPDATES)
    __gdk_window_set_debug_updates (TRUE);

  {
  /* Translate to default:RTL if you want your widgets
   * to be RTL, otherwise translate to default:LTR.
   * Do *not* translate it to "predefinito:LTR", if it
   * it isn't default:LTR or default:RTL it will not work 
   */
    char *e = _("default:LTR");
    if (strcmp (e, "default:RTL")==0) 
      __gtk_widget_set_default_direction (GTK_TEXT_DIR_RTL);
    else if (strcmp (e, "default:LTR"))
      g_warning ("Whoever translated default:LTR did so wrongly.\n");
  }

  /* do what the call to __gtk_type_init() used to do */
  g_type_init ();

  ___gtk_accel_map_init ();
  ___gtk_rc_init ();

  /* Set the 'initialized' flag.
   */
  __gtk_initialized = TRUE;

  /* load gtk modules */
  if (gtk_modules_string)
    {
      ___gtk_modules_init (argc, argv, gtk_modules_string->str);
      g_string_free (gtk_modules_string, TRUE);
    }
  else
    {
      ___gtk_modules_init (argc, argv, NULL);
    }
}


typedef struct
{
  gboolean open_default_display;
} OptionGroupInfo;

static gboolean
post_parse_hook (GOptionContext *context,
		 GOptionGroup   *group,
		 gpointer	data,
		 GError        **error)
{
  OptionGroupInfo *info = data;

  
  do_post_parse_initialization (NULL, NULL);
  
  if (info->open_default_display)
    {
      if (__gdk_display_open_default_libgtk_only () == NULL)
	{
	  const char *display_name = __gdk_get_display_arg_name ();
	  g_set_error (error, 
		       G_OPTION_ERROR, 
		       G_OPTION_ERROR_FAILED,
		       _("Cannot open display: %s"),
		       display_name ? display_name : "" );
	  
	  return FALSE;
	}
    }

  return TRUE;
}


/**
 * __gtk_get_option_group:
 * @open_default_display: whether to open the default display 
 *    when parsing the commandline arguments
 * 
 * Returns a #GOptionGroup for the commandline arguments recognized
 * by GTK+ and GDK. You should add this group to your #GOptionContext 
 * with g_option_context_add_group(), if you are using 
 * g_option_context_parse() to parse your commandline arguments.
 *
 * Returns: a #GOptionGroup for the commandline arguments recognized
 *   by GTK+
 *
 * Since: 2.6
 */
GOptionGroup *
__gtk_get_option_group (gboolean open_default_display)
{
  GOptionGroup *group;
  OptionGroupInfo *info;

  gettext_initialization ();

  info = g_new0 (OptionGroupInfo, 1);
  info->open_default_display = open_default_display;
  
  group = g_option_group_new ("gtk", _("GTK+ Options"), _("Show GTK+ Options"), info, g_free);
  g_option_group_set_parse_hooks (group, NULL, post_parse_hook);

  __gdk_add_option_entries_libgtk_only (group);
  g_option_group_add_entries (group, gtk_args);
  g_option_group_set_translation_domain (group, GETTEXT_PACKAGE);
  
  return group;
}

/**
 * __gtk_init_with_args:
 * @argc: a pointer to the number of command line arguments.
 * @argv: (inout) (array length=argc): a pointer to the array of
 *    command line arguments.
 * @parameter_string: a string which is displayed in
 *    the first line of <option>--help</option> output, after 
 *    <literal><replaceable>programname</replaceable> [OPTION...]</literal>
 * @entries: (array zero-terminated=1):  a %NULL-terminated array
 *    of #GOptionEntry<!-- -->s describing the options of your program
 * @translation_domain: a translation domain to use for translating
 *    the <option>--help</option> output for the options in @entries
 *    and the @parameter_string with gettext(), or %NULL
 * @error: a return location for errors 
 *
 * This function does the same work as __gtk_init_check(). 
 * Additionally, it allows you to add your own commandline options, 
 * and it automatically generates nicely formatted 
 * <option>--help</option> output. Note that your program will
 * be terminated after writing out the help output.
 *
 * Returns: %TRUE if the GUI has been successfully initialized, 
 *               %FALSE otherwise.
 * 
 * Since: 2.6
 */
gboolean
__gtk_init_with_args (int            *argc,
		    char         ***argv,
		    const char     *parameter_string,
		    GOptionEntry   *entries,
		    const char     *translation_domain,
		    GError        **error)
{
  GOptionContext *context;
  GOptionGroup *gtk_group;
  gboolean retval;

  if (__gtk_initialized)
    return __gdk_display_open_default_libgtk_only () != NULL;

  gettext_initialization ();

  if (!check_setugid ())
    return FALSE;

  gtk_group = __gtk_get_option_group (TRUE);
  
  context = g_option_context_new (parameter_string);
  g_option_context_add_group (context, gtk_group);
  
  g_option_context_set_translation_domain (context, translation_domain);

  if (entries)
    g_option_context_add_main_entries (context, entries, translation_domain);
  retval = g_option_context_parse (context, argc, argv, error);
  
  g_option_context_free (context);

  return retval;
}


/**
 * __gtk_parse_args:
 * @argc: (inout): a pointer to the number of command line arguments
 * @argv: (array length=argc) (inout): a pointer to the array of
 *     command line arguments
 *
 * Parses command line arguments, and initializes global
 * attributes of GTK+, but does not actually open a connection
 * to a display. (See __gdk_display_open(), __gdk_get_display_arg_name())
 *
 * Any arguments used by GTK+ or GDK are removed from the array and
 * @argc and @argv are updated accordingly.
 *
 * There is no need to call this function explicitely if you are using
 * __gtk_init(), or __gtk_init_check().
 *
 * Return value: %TRUE if initialization succeeded, otherwise %FALSE.
 **/
gboolean
__gtk_parse_args (int    *argc,
		char ***argv)
{
  GOptionContext *option_context;
  GOptionGroup *gtk_group;
  GError *error = NULL;
  
  if (__gtk_initialized)
    return TRUE;

  gettext_initialization ();

  if (!check_setugid ())
    return FALSE;

  option_context = g_option_context_new (NULL);
  g_option_context_set_ignore_unknown_options (option_context, TRUE);
  g_option_context_set_help_enabled (option_context, FALSE);
  gtk_group = __gtk_get_option_group (FALSE);
  g_option_context_set_main_group (option_context, gtk_group);
  if (!g_option_context_parse (option_context, argc, argv, &error))
    {
      g_warning ("%s", error->message);
      g_error_free (error);
    }

  g_option_context_free (option_context);

  return TRUE;
}

/**
 * __gtk_init_check:
 * @argc: (inout): Address of the <parameter>argc</parameter> parameter of your
 *   main() function. Changed if any arguments were handled.
 * @argv: (array length=argc) (inout) (allow-none): Address of the <parameter>argv</parameter> parameter of main().
 *   Any parameters understood by __gtk_init() are stripped before return.
 *
 * This function does the same work as __gtk_init() with only
 * a single change: It does not terminate the program if the GUI can't be
 * initialized. Instead it returns %FALSE on failure.
 *
 * This way the application can fall back to some other means of communication 
 * with the user - for example a curses or command line interface.
 * 
 * Return value: %TRUE if the GUI has been successfully initialized, 
 *               %FALSE otherwise.
 **/
gboolean
__gtk_init_check (int	 *argc,
		char   ***argv)
{
  if (!__gtk_parse_args (argc, argv))
    return FALSE;

  return __gdk_display_open_default_libgtk_only () != NULL;
}

/**
 * __gtk_init:
 * @argc: (inout): Address of the <parameter>argc</parameter> parameter of
 *     your main() function. Changed if any arguments were handled
 * @argv: (array length=argc) (inout) (allow-none): Address of the
 *     <parameter>argv</parameter> parameter of main(). Any options
 *     understood by GTK+ are stripped before return.
 *
 * Call this function before using any other GTK+ functions in your GUI
 * applications.  It will initialize everything needed to operate the
 * toolkit and parses some standard command line options.
 *
 * @argc and @argv are adjusted accordingly so your own code will
 * never see those standard arguments.
 *
 * Note that there are some alternative ways to initialize GTK+:
 * if you are calling __gtk_parse_args(), __gtk_init_check(),
 * __gtk_init_with_args() or g_option_context_parse() with
 * the option group returned by __gtk_get_option_group(),
 * you <emphasis>don't</emphasis> have to call __gtk_init().
 *
 * <note><para>
 * This function will terminate your program if it was unable to
 * initialize the windowing system for some reason. If you want
 * your program to fall back to a textual interface you want to
 * call __gtk_init_check() instead.
 * </para></note>
 *
 * <note><para>
 * Since 2.18, GTK+ calls <literal>signal (SIGPIPE, SIG_IGN)</literal>
 * during initialization, to ignore SIGPIPE signals, since these are
 * almost never wanted in graphical applications. If you do need to
 * handle SIGPIPE for some reason, reset the handler after __gtk_init(),
 * but notice that other libraries (e.g. libdbus or gvfs) might do
 * similar things.
 * </para></note>
 */
void
__gtk_init (int *argc, char ***argv)
{
  if (!__gtk_init_check (argc, argv))
    {
      const char *display_name_arg = __gdk_get_display_arg_name ();
      if (display_name_arg == NULL)
        display_name_arg = getenv("DISPLAY");
      g_warning ("cannot open display: %s", display_name_arg ? display_name_arg : "");
      exit (1);
    }
}

/**
 * ___gtk_get_lc_ctype:
 *
 * Return the Unix-style locale string for the language currently in
 * effect. On Unix systems, this is the return value from
 * <literal>setlocale(LC_CTYPE, NULL)</literal>, and the user can
 * affect this through the environment variables LC_ALL, LC_CTYPE or
 * LANG (checked in that order). The locale strings typically is in
 * the form lang_COUNTRY, where lang is an ISO-639 language code, and
 * COUNTRY is an ISO-3166 country code. For instance, sv_FI for
 * Swedish as written in Finland or pt_BR for Portuguese as written in
 * Brazil.
 * 
 * On Windows, the C library doesn't use any such environment
 * variables, and setting them won't affect the behaviour of functions
 * like ctime(). The user sets the locale through the Regional Options 
 * in the Control Panel. The C library (in the setlocale() function) 
 * does not use country and language codes, but country and language 
 * names spelled out in English. 
 * However, this function does check the above environment
 * variables, and does return a Unix-style locale string based on
 * either said environment variables or the thread's current locale.
 *
 * Return value: a dynamically allocated string, free with g_free().
 */

gchar *
___gtk_get_lc_ctype (void)
{
#ifdef G_OS_WIN32
  /* Somebody might try to set the locale for this process using the
   * LANG or LC_ environment variables. The Microsoft C library
   * doesn't know anything about them. You set the locale in the
   * Control Panel. Setting these env vars won't have any affect on
   * locale-dependent C library functions like ctime(). But just for
   * kicks, do obey LC_ALL, LC_CTYPE and LANG in GTK. (This also makes
   * it easier to test GTK and Pango in various default languages, you
   * don't have to clickety-click in the Control Panel, you can simply
   * start the program with LC_ALL=something on the command line.)
   */
  gchar *p;

  p = getenv ("LC_ALL");
  if (p != NULL)
    return g_strdup (p);

  p = getenv ("LC_CTYPE");
  if (p != NULL)
    return g_strdup (p);

  p = getenv ("LANG");
  if (p != NULL)
    return g_strdup (p);

  return g_win32_getlocale ();
#else
  return g_strdup (setlocale (LC_CTYPE, NULL));
#endif
}

/**
 * __gtk_get_default_language:
 *
 * Returns the #PangoLanguage for the default language currently in
 * effect. (Note that this can change over the life of an
 * application.)  The default language is derived from the current
 * locale. It determines, for example, whether GTK+ uses the
 * right-to-left or left-to-right text direction.
 *
 * This function is equivalent to pango_language_get_default().  See
 * that function for details.
 * 
 * Return value: the default language as a #PangoLanguage, must not be
 * freed
 **/
PangoLanguage *
__gtk_get_default_language (void)
{
  return pango_language_get_default ();
}

void
__gtk_main (void)
{
  GMainLoop *loop;

  __gtk_main_loop_level++;
  
  loop = g_main_loop_new (NULL, TRUE);
  main_loops = g_slist_prepend (main_loops, loop);

  if (g_main_loop_is_running (main_loops->data))
    {
      GDK_THREADS_LEAVE ();
      g_main_loop_run (loop);
      GDK_THREADS_ENTER ();
      __gdk_flush ();
    }

  main_loops = g_slist_remove (main_loops, loop);

  g_main_loop_unref (loop);

  __gtk_main_loop_level--;

  if (__gtk_main_loop_level == 0)
    {
      /* Try storing all clipboard data we have */
      ___gtk_clipboard_store_all ();

      /* Synchronize the recent manager singleton */
      ___gtk_recent_manager_sync ();
    }
}

guint
__gtk_main_level (void)
{
  return __gtk_main_loop_level;
}

void
__gtk_main_quit (void)
{
  g_return_if_fail (main_loops != NULL);

  g_main_loop_quit (main_loops->data);
}

gboolean
__gtk_events_pending (void)
{
  gboolean result;
  
  GDK_THREADS_LEAVE ();  
  result = g_main_context_pending (NULL);
  GDK_THREADS_ENTER ();

  return result;
}

gboolean
__gtk_main_iteration (void)
{
  GDK_THREADS_LEAVE ();
  g_main_context_iteration (NULL, TRUE);
  GDK_THREADS_ENTER ();

  if (main_loops)
    return !g_main_loop_is_running (main_loops->data);
  else
    return TRUE;
}

gboolean
__gtk_main_iteration_do (gboolean blocking)
{
  GDK_THREADS_LEAVE ();
  g_main_context_iteration (NULL, blocking);
  GDK_THREADS_ENTER ();

  if (main_loops)
    return !g_main_loop_is_running (main_loops->data);
  else
    return TRUE;
}

/* private libgtk to libgdk interfaces
 */
gboolean __gdk_pointer_grab_info_libgtk_only  (GdkDisplay *display,
					     GdkWindow **grab_window,
					     gboolean   *owner_events);
gboolean __gdk_keyboard_grab_info_libgtk_only (GdkDisplay *display,
					     GdkWindow **grab_window,
					     gboolean   *owner_events);

static void
rewrite_events_translate (GdkWindow *old_window,
			  GdkWindow *new_window,
			  gdouble   *x,
			  gdouble   *y)
{
  gint old_origin_x, old_origin_y;
  gint new_origin_x, new_origin_y;

  __gdk_window_get_origin	(old_window, &old_origin_x, &old_origin_y);
  __gdk_window_get_origin	(new_window, &new_origin_x, &new_origin_y);

  *x += old_origin_x - new_origin_x;
  *y += old_origin_y - new_origin_y;
}

static GdkEvent *
rewrite_event_for_window (GdkEvent  *event,
			  GdkWindow *new_window)
{
  event = __gdk_event_copy (event);

  switch (event->type)
    {
    case GDK_SCROLL:
      rewrite_events_translate (event->any.window,
				new_window,
				&event->scroll.x, &event->scroll.y);
      break;
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
      rewrite_events_translate (event->any.window,
				new_window,
				&event->button.x, &event->button.y);
      break;
    case GDK_MOTION_NOTIFY:
      rewrite_events_translate (event->any.window,
				new_window,
				&event->motion.x, &event->motion.y);
      break;
    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
    case GDK_PROXIMITY_IN:
    case GDK_PROXIMITY_OUT:
      break;

    default:
      return event;
    }

  g_object_unref (event->any.window);
  event->any.window = g_object_ref (new_window);

  return event;
}

/* If there is a pointer or keyboard grab in effect with owner_events = TRUE,
 * then what X11 does is deliver the event normally if it was going to this
 * client, otherwise, delivers it in terms of the grab window. This function
 * rewrites events to the effect that events going to the same window group
 * are delivered normally, otherwise, the event is delivered in terms of the
 * grab window.
 */
static GdkEvent *
rewrite_event_for_grabs (GdkEvent *event)
{
  GdkWindow *grab_window;
  GtkWidget *event_widget, *grab_widget;
  gpointer grab_widget_ptr;
  gboolean owner_events;
  GdkDisplay *display;

  switch (event->type)
    {
    case GDK_SCROLL:
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
    case GDK_BUTTON_RELEASE:
    case GDK_MOTION_NOTIFY:
    case GDK_PROXIMITY_IN:
    case GDK_PROXIMITY_OUT:
      display = __gdk_window_get_display (event->proximity.window);
      if (!__gdk_pointer_grab_info_libgtk_only (display, &grab_window, &owner_events) ||
	  !owner_events)
	return NULL;
      break;

    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
      display = __gdk_window_get_display (event->key.window);
      if (!__gdk_keyboard_grab_info_libgtk_only (display, &grab_window, &owner_events) ||
	  !owner_events)
	return NULL;
      break;

    default:
      return NULL;
    }

  event_widget = __gtk_get_event_widget (event);
  __gdk_window_get_user_data (grab_window, &grab_widget_ptr);
  grab_widget = grab_widget_ptr;

  if (grab_widget &&
      __gtk_main_get_window_group (grab_widget) != __gtk_main_get_window_group (event_widget))
    return rewrite_event_for_window (event, grab_window);
  else
    return NULL;
}

void 
__gtk_main_do_event (GdkEvent *event)
{
  GtkWidget *event_widget;
  GtkWidget *grab_widget;
  GtkWindowGroup *window_group;
  GdkEvent *rewritten_event = NULL;
  GList *tmp_list;

  if (event->type == GDK_SETTING)
    {
      ___gtk_settings_handle_event (&event->setting);
      return;
    }

  if (event->type == GDK_OWNER_CHANGE)
    {
      ___gtk_clipboard_handle_event (&event->owner_change);
      return;
    }

  /* Find the widget which got the event. We store the widget
   *  in the user_data field of GdkWindow's.
   *  Ignore the event if we don't have a widget for it, except
   *  for GDK_PROPERTY_NOTIFY events which are handled specialy.
   *  Though this happens rarely, bogus events can occour
   *  for e.g. destroyed GdkWindows. 
   */
  event_widget = __gtk_get_event_widget (event);
  if (!event_widget)
    {
      /* To handle selection INCR transactions, we select
       * PropertyNotify events on the requestor window and create
       * a corresponding (fake) GdkWindow so that events get
       * here. There won't be a widget though, so we have to handle
	   * them specially
	   */
      if (event->type == GDK_PROPERTY_NOTIFY)
	___gtk_selection_incr_event (event->any.window,
				   &event->property);

      return;
    }

  /* If pointer or keyboard grabs are in effect, munge the events
   * so that each window group looks like a separate app.
   */
  rewritten_event = rewrite_event_for_grabs (event);
  if (rewritten_event)
    {
      event = rewritten_event;
      event_widget = __gtk_get_event_widget (event);
    }
  
  window_group = __gtk_main_get_window_group (event_widget);

  /* Push the event onto a stack of current events for
   * gtk_current_event_get().
   */
  current_events = g_list_prepend (current_events, event);

  /* If there is a grab in effect...
   */
  if (window_group->grabs)
    {
      grab_widget = window_group->grabs->data;
      
      /* If the grab widget is an ancestor of the event widget
       *  then we send the event to the original event widget.
       *  This is the key to implementing modality.
       */
      if ((__gtk_widget_is_sensitive (event_widget) || event->type == GDK_SCROLL) &&
	  __gtk_widget_is_ancestor (event_widget, grab_widget))
	grab_widget = event_widget;
    }
  else
    {
      grab_widget = event_widget;
    }

  /* Not all events get sent to the grabbing widget.
   * The delete, destroy, expose, focus change and resize
   *  events still get sent to the event widget because
   *  1) these events have no meaning for the grabbing widget
   *  and 2) redirecting these events to the grabbing widget
   *  could cause the display to be messed up.
   * 
   * Drag events are also not redirected, since it isn't
   *  clear what the semantics of that would be.
   */
  switch (event->type)
    {
    case GDK_NOTHING:
      break;
      
    case GDK_DELETE:
      g_object_ref (event_widget);
      if ((!window_group->grabs || __gtk_widget_get_toplevel (window_group->grabs->data) == event_widget) &&
	  !__gtk_widget_event (event_widget, event))
	__gtk_widget_destroy (event_widget);
      g_object_unref (event_widget);
      break;
      
    case GDK_DESTROY:
      /* Unexpected GDK_DESTROY from the outside, ignore for
       * child windows, handle like a GDK_DELETE for toplevels
       */
      if (!gtk_widget_get_props (event_widget)->parent)
	{
	  g_object_ref (event_widget);
	  if (!__gtk_widget_event (event_widget, event) &&
	      __gtk_widget_get_realized (event_widget))
	    __gtk_widget_destroy (event_widget);
	  g_object_unref (event_widget);
	}
      break;
      
    case GDK_EXPOSE:
      if (event->any.window && __gtk_widget_get_double_buffered (event_widget))
	{
	  __gdk_window_begin_paint_region (event->any.window, event->expose.region);
	  __gtk_widget_send_expose (event_widget, event);
	  __gdk_window_end_paint (event->any.window);
	}
      else
	{
	  /* The app may paint with a previously allocated cairo_t,
	     which will draw directly to the window. We can't catch cairo
	     drap operatoins to automatically flush the window, thus we
	     need to explicitly flush any outstanding moves or double
	     buffering */
	  __gdk_window_flush (event->any.window);
	  __gtk_widget_send_expose (event_widget, event);
	}
      break;

    case GDK_PROPERTY_NOTIFY:
    case GDK_NO_EXPOSE:
    case GDK_FOCUS_CHANGE:
    case GDK_CONFIGURE:
    case GDK_MAP:
    case GDK_UNMAP:
    case GDK_SELECTION_CLEAR:
    case GDK_SELECTION_REQUEST:
    case GDK_SELECTION_NOTIFY:
    case GDK_CLIENT_EVENT:
    case GDK_VISIBILITY_NOTIFY:
    case GDK_WINDOW_STATE:
    case GDK_GRAB_BROKEN:
    case GDK_DAMAGE:
      __gtk_widget_event (event_widget, event);
      break;

    case GDK_SCROLL:
    case GDK_BUTTON_PRESS:
    case GDK_2BUTTON_PRESS:
    case GDK_3BUTTON_PRESS:
      __gtk_propagate_event (grab_widget, event);
      break;

    case GDK_KEY_PRESS:
    case GDK_KEY_RELEASE:
      /* Catch alt press to enable auto-mnemonics;
       * menus are handled elsewhere
       */
      if ((event->key.keyval == GDK_Alt_L || event->key.keyval == GDK_Alt_R) &&
          !GTK_IS_MENU_SHELL (grab_widget))
        {
          gboolean auto_mnemonics;

          g_object_get (__gtk_widget_get_settings (grab_widget),
                        "gtk-auto-mnemonics", &auto_mnemonics, NULL);

          if (auto_mnemonics)
            {
              gboolean mnemonics_visible;
              GtkWidget *window;

              mnemonics_visible = (event->type == GDK_KEY_PRESS);

              window = __gtk_widget_get_toplevel (grab_widget);

              if (GTK_IS_WINDOW (window))
                __gtk_window_set_mnemonics_visible (GTK_WINDOW (window), mnemonics_visible);
            }
        }
      /* else fall through */
    case GDK_MOTION_NOTIFY:
    case GDK_BUTTON_RELEASE:
    case GDK_PROXIMITY_IN:
    case GDK_PROXIMITY_OUT:
      __gtk_propagate_event (grab_widget, event);
      break;
      
    case GDK_ENTER_NOTIFY:
      GTK_PRIVATE_SET_FLAG (event_widget, GTK_HAS_POINTER);
      ___gtk_widget_set_pointer_window (event_widget, event->any.window);
      if (__gtk_widget_is_sensitive (grab_widget))
	__gtk_widget_event (grab_widget, event);
      break;
      
    case GDK_LEAVE_NOTIFY:
      GTK_PRIVATE_UNSET_FLAG (event_widget, GTK_HAS_POINTER);
      if (__gtk_widget_is_sensitive (grab_widget))
	__gtk_widget_event (grab_widget, event);
      break;
      
    case GDK_DRAG_STATUS:
    case GDK_DROP_FINISHED:
      ___gtk_drag_source_handle_event (event_widget, event);
      break;
    case GDK_DRAG_ENTER:
    case GDK_DRAG_LEAVE:
    case GDK_DRAG_MOTION:
    case GDK_DROP_START:
      ___gtk_drag_dest_handle_event (event_widget, event);
      break;
    default:
      g_assert_not_reached ();
      break;
    }

  if (event->type == GDK_ENTER_NOTIFY
      || event->type == GDK_LEAVE_NOTIFY
      || event->type == GDK_BUTTON_PRESS
      || event->type == GDK_2BUTTON_PRESS
      || event->type == GDK_3BUTTON_PRESS
      || event->type == GDK_KEY_PRESS
      || event->type == GDK_DRAG_ENTER
      || event->type == GDK_GRAB_BROKEN
      || event->type == GDK_MOTION_NOTIFY
      || event->type == GDK_SCROLL)
    {
      ___gtk_tooltip_handle_event (event);
    }
  
  tmp_list = current_events;
  current_events = g_list_remove_link (current_events, tmp_list);
  g_list_free_1 (tmp_list);

  if (rewritten_event)
    __gdk_event_free (rewritten_event);
}

gboolean
__gtk_true (void)
{
  return TRUE;
}

gboolean
__gtk_false (void)
{
  return FALSE;
}

static GtkWindowGroup *
__gtk_main_get_window_group (GtkWidget   *widget)
{
  GtkWidget *toplevel = NULL;

  if (widget)
    toplevel = __gtk_widget_get_toplevel (widget);

  if (GTK_IS_WINDOW (toplevel))
    return __gtk_window_get_group (GTK_WINDOW (toplevel));
  else
    return __gtk_window_get_group (NULL);
}

typedef struct
{
  GtkWidget *old_grab_widget;
  GtkWidget *new_grab_widget;
  gboolean   was_grabbed;
  gboolean   is_grabbed;
  gboolean   from_grab;
} GrabNotifyInfo;

static void
gtk_grab_notify_foreach (GtkWidget *child,
			 gpointer   data)
                        
{
  GrabNotifyInfo *info = data;
 
  gboolean was_grabbed, is_grabbed, was_shadowed, is_shadowed;

  was_grabbed = info->was_grabbed;
  is_grabbed = info->is_grabbed;

  info->was_grabbed = info->was_grabbed || (child == info->old_grab_widget);
  info->is_grabbed = info->is_grabbed || (child == info->new_grab_widget);

  was_shadowed = info->old_grab_widget && !info->was_grabbed;
  is_shadowed = info->new_grab_widget && !info->is_grabbed;

  g_object_ref (child);

  if ((was_shadowed || is_shadowed) && GTK_IS_CONTAINER (child))
    __gtk_container_forall (GTK_CONTAINER (child), gtk_grab_notify_foreach, info);
  
  if (is_shadowed)
    {
      GTK_PRIVATE_SET_FLAG (child, GTK_SHADOWED);
      if (!was_shadowed && GTK_WIDGET_HAS_POINTER (child)
	  && __gtk_widget_is_sensitive (child))
	___gtk_widget_synthesize_crossing (child, info->new_grab_widget,
					 GDK_CROSSING_GTK_GRAB);
    }
  else
    {
      GTK_PRIVATE_UNSET_FLAG (child, GTK_SHADOWED);
      if (was_shadowed && GTK_WIDGET_HAS_POINTER (child)
	  && __gtk_widget_is_sensitive (child))
	___gtk_widget_synthesize_crossing (info->old_grab_widget, child,
					 info->from_grab ? GDK_CROSSING_GTK_GRAB
					 : GDK_CROSSING_GTK_UNGRAB);
    }

  if (was_shadowed != is_shadowed)
    ___gtk_widget_grab_notify (child, was_shadowed);
  
  g_object_unref (child);
  
  info->was_grabbed = was_grabbed;
  info->is_grabbed = is_grabbed;
}

static void
gtk_grab_notify (GtkWindowGroup *group,
		 GtkWidget      *old_grab_widget,
		 GtkWidget      *new_grab_widget,
		 gboolean        from_grab)
{
  GList *toplevels;
  GrabNotifyInfo info;

  if (old_grab_widget == new_grab_widget)
    return;

  info.old_grab_widget = old_grab_widget;
  info.new_grab_widget = new_grab_widget;
  info.from_grab = from_grab;

  g_object_ref (group);

  toplevels = __gtk_window_list_toplevels ();
  g_list_foreach (toplevels, (GFunc)g_object_ref, NULL);
			    
  while (toplevels)
    {
      GtkWindow *toplevel = toplevels->data;
      toplevels = g_list_delete_link (toplevels, toplevels);

      info.was_grabbed = FALSE;
      info.is_grabbed = FALSE;

      if (group == __gtk_window_get_group (toplevel))
	gtk_grab_notify_foreach (GTK_WIDGET (toplevel), &info);
      g_object_unref (toplevel);
    }

  g_object_unref (group);
}

void
__gtk_grab_add (GtkWidget *widget)
{
  GtkWindowGroup *group;
  GtkWidget *old_grab_widget;
  
  g_return_if_fail (widget != NULL);
  
  if (!__gtk_widget_has_grab (widget) && __gtk_widget_is_sensitive (widget))
    {
      ___gtk_widget_set_has_grab (widget, TRUE);
      
      group = __gtk_main_get_window_group (widget);

      if (group->grabs)
	old_grab_widget = (GtkWidget *)group->grabs->data;
      else
	old_grab_widget = NULL;

      g_object_ref (widget);
      group->grabs = g_slist_prepend (group->grabs, widget);

      gtk_grab_notify (group, old_grab_widget, widget, TRUE);
    }
}

/**
 * __gtk_grab_get_current:
 *
 * Queries the current grab of the default window group.
 *
 * Return value: (transfer none): The widget which currently
 *     has the grab or %NULL if no grab is active
 */
GtkWidget*
__gtk_grab_get_current (void)
{
  GtkWindowGroup *group;

  group = __gtk_main_get_window_group (NULL);

  if (group->grabs)
    return GTK_WIDGET (group->grabs->data);
  return NULL;
}

void
__gtk_grab_remove (GtkWidget *widget)
{
  GtkWindowGroup *group;
  GtkWidget *new_grab_widget;
  
  g_return_if_fail (widget != NULL);
  
  if (__gtk_widget_has_grab (widget))
    {
      ___gtk_widget_set_has_grab (widget, FALSE);

      group = __gtk_main_get_window_group (widget);
      group->grabs = g_slist_remove (group->grabs, widget);
      
      if (group->grabs)
	new_grab_widget = (GtkWidget *)group->grabs->data;
      else
	new_grab_widget = NULL;

      gtk_grab_notify (group, widget, new_grab_widget, FALSE);
      
      g_object_unref (widget);
    }
}

/**
 * __gtk_get_current_event:
 * 
 * Obtains a copy of the event currently being processed by GTK+.  For
 * example, if you get a "clicked" signal from #GtkButton, the current
 * event will be the #GdkEventButton that triggered the "clicked"
 * signal. The returned event must be freed with __gdk_event_free().
 * If there is no current event, the function returns %NULL.
 * 
 * Return value: (transfer full): a copy of the current event, or %NULL if no
 *     current event.
 **/
GdkEvent*
__gtk_get_current_event (void)
{
  if (current_events)
    return __gdk_event_copy (current_events->data);
  else
    return NULL;
}

/**
 * __gtk_get_current_event_time:
 * 
 * If there is a current event and it has a timestamp, return that
 * timestamp, otherwise return %GDK_CURRENT_TIME.
 * 
 * Return value: the timestamp from the current event, or %GDK_CURRENT_TIME.
 **/
guint32
__gtk_get_current_event_time (void)
{
  if (current_events)
    return __gdk_event_get_time (current_events->data);
  else
    return GDK_CURRENT_TIME;
}

/**
 * __gtk_get_current_event_state:
 * @state: (out): a location to store the state of the current event
 * 
 * If there is a current event and it has a state field, place
 * that state field in @state and return %TRUE, otherwise return
 * %FALSE.
 * 
 * Return value: %TRUE if there was a current event and it had a state field
 **/
gboolean
__gtk_get_current_event_state (GdkModifierType *state)
{
  g_return_val_if_fail (state != NULL, FALSE);
  
  if (current_events)
    return __gdk_event_get_state (current_events->data, state);
  else
    {
      *state = 0;
      return FALSE;
    }
}

/**
 * __gtk_get_event_widget:
 * @event: a #GdkEvent
 *
 * If @event is %NULL or the event was not associated with any widget,
 * returns %NULL, otherwise returns the widget that received the event
 * originally.
 * 
 * Return value: (transfer none): the widget that originally
 *     received @event, or %NULL
 **/
GtkWidget*
__gtk_get_event_widget (GdkEvent *event)
{
  GtkWidget *widget;
  gpointer widget_ptr;

  widget = NULL;
  if (event && event->any.window && 
      (event->type == GDK_DESTROY || !GDK_WINDOW_DESTROYED (event->any.window)))
    {
      __gdk_window_get_user_data (event->any.window, &widget_ptr);
      widget = widget_ptr;
    }
  
  return widget;
}

/**
 * __gtk_propagate_event:
 * @widget: a #GtkWidget
 * @event: an event
 *
 * Sends an event to a widget, propagating the event to parent widgets
 * if the event remains unhandled. Events received by GTK+ from GDK
 * normally begin in __gtk_main_do_event(). Depending on the type of
 * event, existence of modal dialogs, grabs, etc., the event may be
 * propagated; if so, this function is used. __gtk_propagate_event()
 * calls __gtk_widget_event() on each widget it decides to send the
 * event to.  So __gtk_widget_event() is the lowest-level function; it
 * simply emits the "event" and possibly an event-specific signal on a
 * widget.  __gtk_propagate_event() is a bit higher-level, and
 * __gtk_main_do_event() is the highest level.
 *
 * All that said, you most likely don't want to use any of these
 * functions; synthesizing events is rarely needed. Consider asking on
 * the mailing list for better ways to achieve your goals. For
 * example, use __gdk_window_invalidate_rect() or
 * __gtk_widget_queue_draw() instead of making up expose events.
 * 
 **/
void
__gtk_propagate_event (GtkWidget *widget,
		     GdkEvent  *event)
{
  gint handled_event;
  
  g_return_if_fail (GTK_IS_WIDGET (widget));
  g_return_if_fail (event != NULL);
  
  handled_event = FALSE;

  g_object_ref (widget);
      
  if ((event->type == GDK_KEY_PRESS) ||
      (event->type == GDK_KEY_RELEASE))
    {
      /* Only send key events within Window widgets to the Window
       *  The Window widget will in turn pass the
       *  key event on to the currently focused widget
       *  for that window.
       */
      GtkWidget *window;

      window = __gtk_widget_get_toplevel (widget);
      if (GTK_IS_WINDOW (window))
	{
	  /* If there is a grab within the window, give the grab widget
	   * a first crack at the key event
	   */
	  if (widget != window && __gtk_widget_has_grab (widget))
	    handled_event = __gtk_widget_event (widget, event);
	  
	  if (!handled_event)
	    {
	      window = __gtk_widget_get_toplevel (widget);
	      if (GTK_IS_WINDOW (window))
		{
		  if (__gtk_widget_is_sensitive (window))
		    __gtk_widget_event (window, event);
		}
	    }
		  
	  handled_event = TRUE; /* don't send to widget */
	}
    }
  
  /* Other events get propagated up the widget tree
   *  so that parents can see the button and motion
   *  events of the children.
   */
  if (!handled_event)
    {
      while (TRUE)
	{
	  GtkWidget *tmp;

	  /* Scroll events are special cased here because it
	   * feels wrong when scrolling a GtkViewport, say,
	   * to have children of the viewport eat the scroll
	   * event
	   */
	  if (!__gtk_widget_is_sensitive (widget))
	    handled_event = event->type != GDK_SCROLL;
	  else
	    handled_event = __gtk_widget_event (widget, event);
	      
	  tmp = gtk_widget_get_props (widget)->parent;
	  g_object_unref (widget);

	  widget = tmp;
	  
	  if (!handled_event && widget)
	    g_object_ref (widget);
	  else
	    break;
	}
    }
  else
    g_object_unref (widget);
}

gboolean
___gtk_boolean_handled_accumulator (GSignalInvocationHint *ihint,
				  GValue                *return_accu,
				  const GValue          *handler_return,
				  gpointer               dummy)
{
  gboolean continue_emission;
  gboolean signal_handled;
  
  signal_handled = g_value_get_boolean (handler_return);
  g_value_set_boolean (return_accu, signal_handled);
  continue_emission = !signal_handled;
  
  return continue_emission;
}

gboolean
_gtk_button_event_triggers_context_menu (GdkEventButton *event)
{
  if (event->type == GDK_BUTTON_PRESS)
    {
      if (event->button == 3 &&
          ! (event->state & (GDK_BUTTON1_MASK | GDK_BUTTON2_MASK)))
        return TRUE;

#ifdef GDK_WINDOWING_QUARTZ
      if (event->button == 1 &&
          ! (event->state & (GDK_BUTTON2_MASK | GDK_BUTTON3_MASK)) &&
          (event->state & GDK_CONTROL_MASK))
        return TRUE;
#endif
    }

  return FALSE;
}

gboolean
_gtk_translate_keyboard_accel_state (GdkKeymap       *keymap,
                                     guint            hardware_keycode,
                                     GdkModifierType  state,
                                     GdkModifierType  accel_mask,
                                     gint             group,
                                     guint           *keyval,
                                     gint            *effective_group,
                                     gint            *level,
                                     GdkModifierType *consumed_modifiers)
{
  gboolean group_mask_disabled = FALSE;
  gboolean retval;

  /* if the group-toggling modifier is part of the accel mod mask, and
   * it is active, disable it for matching
   */
  if (accel_mask & state & GTK_TOGGLE_GROUP_MOD_MASK)
    {
      state &= ~GTK_TOGGLE_GROUP_MOD_MASK;
      group = 0;
      group_mask_disabled = TRUE;
    }

  retval = __gdk_keymap_translate_keyboard_state (keymap,
                                                hardware_keycode, state, group,
                                                keyval,
                                                effective_group, level,
                                                consumed_modifiers);

  /* add back the group mask, we want to match against the modifier,
   * but not against the keyval from its group
   */
  if (group_mask_disabled)
    {
      if (effective_group)
        *effective_group = 1;

      if (consumed_modifiers)
        *consumed_modifiers &= ~GTK_TOGGLE_GROUP_MOD_MASK;
    }

  return retval;
}
