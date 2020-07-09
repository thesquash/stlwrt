# Process this file with autoconf to produce a configure script.
# require autoconf 2.54
AC_PREREQ(2.54)

# Let's break apart the portions of the STLWRT and GTK+ version numbers here:
#
# GTK+ is usually quoted as a major version followed by a minor version followed
# by a micro version in the format 2.24.32.  The 2 is the major version; when we
# speak of GTK+ 2 we mean a version of GTK+ whose major version number is 2.
# The 24 is the minor version and the 32 is the micro version.  Generally
# speaking, the micro version is incremented when this release incorporates
# relatively tiny changes compared to the previous version; if this release
# incorporates any potentially breaking changes (or there hasn't been a release
# of a new minor version for several months), the minor version is incremented
# by one and the micro version is reset to zero.  When _very_ breaking changes
# are implemented, both the minor and micro versions are zeroed and the major
# version is incremented by one.  Usually, GTK+ developers will wait to
# introduce new widgets into the toolkit until the minor number is about to be
# incremented.  If the minor number of a particular version of GTK+ is 90 or
# greater, then that version of GTK+ is in beta testing and may not be
# completely stable.  Bigger isn't always better!
#
# The interface age is similar to the micro version, except the interface age
# indicates the lowest version of GTK+ that this version of GTK+ is 100% binary-
# compatible with in all respects.  See the next paragraph for an explanation of
# what the interface age is used for.
#
# The binary age is a condensed version of the minor version plus the micro
# version.  It is used along with the major version and the interface age to
# name the GTK+ libraries.  A few examples:
#
# libgtk-x11-2.0.so.0.2432.32
# libgtk-3.so.0.2404.9
# libgtk-4.so.0.203.0
# libstlwrt-common.so.0.1.1
# libstlwrt-gtk3-compat.so.0.1.1
#
# In the first two cases, the major number is clearly seen as the first number
# before the first period character in the library name; the binary age is the
# four- or three-digit number preceding the last period character in the library
# name; the interface age is the last number in the name.  Please note that as I
# write this GTK 4 (the name no longer includes a plus sign) is _not actually
# up to version 4.2.3_!  I simply made that version number up as an example.
#
# End of versioning diatribe.

# First define what version of STLWRT this is.
m4_define([stlwrt_major_version], [0])
m4_define([stlwrt_minor_version], [0])
m4_define([stlwrt_micro_version], [51])
m4_define([stlwrt_interface_age], [51])
m4_define([stlwrt_binary_age],
          [m4_eval(100 * stlwrt_minor_version + stlwrt_micro_version)])
m4_define([stlwrt_version],
          [stlwrt_major_version.stlwrt_minor_version.stlwrt_micro_version])



# required versions of other packages
m4_define([glib_required_version], [2.28.0])
m4_define([pango_required_version], [1.20])
m4_define([atk_required_version], [1.29.2])
m4_define([cairo_required_version], [1.6])
m4_define([gdk_pixbuf_required_version], [2.21.0])


AC_INIT([STLWRT], [stlwrt_version],
        [a currently unknown bug reporting address],
	[STLWRT])

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])

# Save this value here, since automake will set cflags later
cflags_set=${CFLAGS+set}

AM_INIT_AUTOMAKE([no-define -Wno-portability dist-bzip2])

# Support silent build rules, requires at least automake-1.11. Enable
# by either passing --enable-silent-rules to configure or passing V=0
# to make
m4_ifdef([AM_SILENT_RULES], [AM_SILENT_RULES([no])])

#
# For each of the libraries we build, we define the following
 
# substituted variables:
#
# foo_PACKAGES:     pkg-config packages this library requires
# foo_EXTRA_LIBS:   Libraries this module requires not pulled in by pkg-config
# foo_EXTRA_CFLAGS: cflags this module requires not pulled in by pkg-config
# foo_DEP_LIBS:     All libraries this module requires
# foo_DEP_CFLAGS:   All cflags this module requires


STLWRT_MAJOR_VERSION=stlwrt_major_version
STLWRT_MINOR_VERSION=stlwrt_minor_version
STLWRT_MICRO_VERSION=stlwrt_micro_version
AC_SUBST(STLWRT_MAJOR_VERSION)
AC_SUBST(STLWRT_MINOR_VERSION)
AC_SUBST(STLWRT_MICRO_VERSION)


m4_define([lt_current], [m4_eval(100 * stlwrt_minor_version + stlwrt_micro_version - stlwrt_interface_age)])
m4_define([lt_revision], [stlwrt_interface_age])
m4_define([lt_age], [m4_eval(stlwrt_binary_age - stlwrt_interface_age)])
STLWRT_LT_VERSION_INFO="lt_current:lt_revision:lt_age"
AC_SUBST(STLWRT_LT_VERSION_INFO)



GETTEXT_PACKAGE=stlwrt
AC_SUBST(GETTEXT_PACKAGE)
AC_DEFINE_UNQUOTED(GETTEXT_PACKAGE, "$GETTEXT_PACKAGE",
                   [The prefix for our GetText translation domains.])

AC_CANONICAL_HOST

MATH_LIB=-lm
AC_MSG_CHECKING([for native Win32])
LIB_EXE_MACHINE_FLAG=X86
EXE_MANIFEST_ARCHITECTURE=X86
case "$host" in
  *-*-mingw*)
    os_win32=yes
    gio_can_sniff=no
    MATH_LIB=
    case "$host" in
    x86_64-*-*)
      LIB_EXE_MACHINE_FLAG=X64
      EXE_MANIFEST_ARCHITECTURE=AMD64
      ;;
    esac
    ;;
  *)
    os_win32=no
    ;;
esac
AC_MSG_RESULT([$os_win32])

AC_SUBST(LIB_EXE_MACHINE_FLAG)
AC_SUBST(EXE_MANIFEST_ARCHITECTURE)

case $host in
  *-*-linux*)
    os_linux=yes
    ;;
esac

dnl Initialize libtool
AC_PROG_CC
AM_DISABLE_STATIC

dnl 
dnl Check for a working C++ compiler, but do not bail out, if none is found.
dnl We use this for an automated test for C++ header correctness.
dnl 
AC_CHECK_TOOLS(CXX, [$CCC c++ g++ gcc CC cxx cc++ cl], gcc)
AC_LANG_SAVE
AC_LANG_CPLUSPLUS

AC_TRY_COMPILE(,[class a { int b; } c;], ,CXX=)
AM_CONDITIONAL(HAVE_CXX, test "$CXX" != "")

gtk_save_cxxflags="$CXXFLAGS"
CXXFLAGS="$CXXFLAGS -x objective-c++"
AC_TRY_COMPILE([@interface Foo @end],,OBJC=yes,OBJC=no)
AM_CONDITIONAL(HAVE_OBJC, test "$OBJC" = "yes")
CXXFLAGS="$gtk_save_cxxflags"
AC_LANG_RESTORE

if test "$os_win32" = "yes"; then
  if test x$enable_static = xyes -o x$enable_static = x; then
    AC_MSG_WARN([Disabling static library build, must build as DLL on Windows.])
    enable_static=no
  fi
  if test x$enable_shared = xno; then
    AC_MSG_WARN([Enabling shared library build, must build as DLL on Windows.])
  fi
  enable_shared=yes
fi

AC_LIBTOOL_WIN32_DLL
AM_PROG_LIBTOOL
dnl when using libtool 2.x create libtool early, because it's used in configure
m4_ifdef([LT_OUTPUT], [LT_OUTPUT])


# Make sure we use 64-bit versions of various file stuff.
AC_SYS_LARGEFILE

AM_PROG_AS
AC_PATH_TOOL(NM, nm, nm)

dnl Initialize maintainer mode
AM_MAINTAINER_MODE([enable])

AC_MSG_CHECKING([for some Win32 platform])
case "$host" in
  *-*-mingw*|*-*-cygwin*)
    platform_win32=yes
    ;;
  *)
    platform_win32=no
    ;;
esac
AC_MSG_RESULT([$platform_win32])
AM_CONDITIONAL(PLATFORM_WIN32, test "$platform_win32" = "yes")

AM_CONDITIONAL(OS_WIN32, test "$os_win32" = "yes")
AM_CONDITIONAL(OS_UNIX, test "$os_win32" != "yes")
AM_CONDITIONAL(OS_LINUX, test "$os_linux" = "yes")

if test "$os_win32" = "yes"; then
  AC_CHECK_TOOL(WINDRES, windres, no)
  if test "$WINDRES" = no; then
    AC_MSG_ERROR([*** Could not find an implementation of windres in your PATH.])
  fi
  AC_CHECK_PROG(ms_librarian, lib.exe, yes, no)
fi
AM_CONDITIONAL(MS_LIB_AVAILABLE, test x$ms_librarian = xyes)

m4_define([debug_default],
          m4_if(m4_eval(stlwrt_minor_version % 2), [1], [yes], [minimum]))

dnl declare --enable-* args and collect ac_help strings
AC_ARG_ENABLE(atk,
              [AC_HELP_STRING([--enable-atk],
	                      [Use ATK for accessibility features if it's available [default=yes]])],,
              [enable_atk="yes"])

AC_ARG_ENABLE(debug,
              AC_HELP_STRING([--enable-debug=@<:@no/minimum/yes@:>@],
                             [turn on debugging @<:@default=debug_default@:>@]),,
              enable_debug=debug_default)
AC_ARG_ENABLE(shm,
              [AC_HELP_STRING([--enable-shm],
                              [support shared memory if available [default=yes]])],,
              [enable_shm="yes"])
AC_ARG_ENABLE(xkb,
              [AC_HELP_STRING([--enable-xkb],
                              [support XKB [default=maybe]])],,
              [enable_xkb="maybe"])
AC_ARG_ENABLE(xinerama,
              [AC_HELP_STRING([--enable-xinerama],
	                      [support Xinerama extension if available [default=yes]])],,
              [enable_xinerama="yes"])
AC_ARG_ENABLE(rebuilds,
              [AC_HELP_STRING([--disable-rebuilds],
                              [disable all source autogeneration rules])],,
              [enable_rebuilds=yes])
AC_ARG_ENABLE(visibility,
              [AC_HELP_STRING([--disable-visibility],
                              [don't use ELF visibility attributes])],,
              [enable_visibility=yes])

AC_ARG_WITH(xinput,
            [AC_HELP_STRING([--with-xinput=@<:@no/yes@:>@], [support XInput])])


stlwrt_target=x11

AC_ARG_WITH(stlwrt-target, [  --with-stlwrt-target=[[x11/directfb]] select non-default STLWRT target],
	stlwrt_target=$with_stlwrt_target)

AC_SUBST(stlwrt_target)
case $stlwrt_target in
  x11|directfb) ;;
  *) AC_MSG_ERROR([Invalid target for STLWRT: use x11 or directfb.]);;
esac



if test "x$enable_debug" = "xyes"; then
  test "$cflags_set" = set || CFLAGS="$CFLAGS -g"
  STLWRT_DEBUG_FLAGS="-DG_ENABLE_DEBUG -DG_ERRORCHECK_MUTEXES" 
else
  if test "x$enable_debug" = "xno"; then
    STLWRT_DEBUG_FLAGS="-DG_DISABLE_ASSERT -DG_DISABLE_CHECKS -DG_DISABLE_CAST_CHECKS"
  else
    STLWRT_DEBUG_FLAGS="-DG_DISABLE_CAST_CHECKS"
  fi
fi


if test "x$enable_visibility" = "xno"; then
  STLWRT_DEBUG_FLAGS="$STLWRT_DEBUG_FLAGS -DDISABLE_VISIBILITY"
fi


AC_DEFINE_UNQUOTED(STLWRT_COMPILED_WITH_DEBUGGING, "${enable_debug}",
                   [Define if debugging is enabled])

			
# Build time sanity check...
AM_SANITY_CHECK

# Checks for programs.
AC_ISC_POSIX
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_MAKE_SET

changequote(,)dnl
if test "x$GCC" = "xyes"; then
  case " $CFLAGS " in
  *[\ \	]-Wall[\ \	]*) ;;
  *) CFLAGS="$CFLAGS -Wall" ;;
  esac

  if test "x$enable_ansi" = "xyes"; then
    case " $CFLAGS " in
    *[\ \	]-ansi[\ \	]*) ;;
    *) CFLAGS="$CFLAGS -ansi" ;;
    esac

    case " $CFLAGS " in
    *[\ \	]-pedantic[\ \	]*) ;;
    *) CFLAGS="$CFLAGS -pedantic" ;;
    esac
  fi
fi
changequote([,])dnl

CPPFLAGS="$CPPFLAGS -DG_DISABLE_SINGLE_INCLUDES -DATK_DISABLE_SINGLE_INCLUDES -DGDK_PIXBUF_DISABLE_SINGLE_INCLUDES -DGTK_DISABLE_SINGLE_INCLUDES"

# Ensure MSVC-compatible struct packing convention is used when
# compiling for Win32 with gcc.
# What flag to depends on gcc version: gcc3 uses "-mms-bitfields", while
# gcc2 uses "-fnative-struct".
if test x"$os_win32" = xyes; then
  if test x"$GCC" = xyes; then
    msnative_struct=''
    AC_MSG_CHECKING([how to get MSVC-compatible struct packing])
    if test -z "$ac_cv_prog_CC"; then
      our_gcc="$CC"
    else
      our_gcc="$ac_cv_prog_CC"
    fi
    case `$our_gcc --version | sed -e 's,\..*,.,' -e q` in
      2.)
	if $our_gcc -v --help 2>/dev/null | grep fnative-struct >/dev/null; then
	  msnative_struct='-fnative-struct'
	fi
	;;
      *)
	if $our_gcc -v --help 2>/dev/null | grep ms-bitfields >/dev/null; then
	  msnative_struct='-mms-bitfields'
	fi
	;;
    esac
    if test x"$msnative_struct" = x ; then
      AC_MSG_RESULT([no way])
      AC_MSG_WARN([produced libraries might be incompatible with MSVC-compiled code])
    else
      CFLAGS="$CFLAGS $msnative_struct"
      AC_MSG_RESULT([${msnative_struct}])
    fi
  fi
fi

# Honor aclocal flags
ACLOCAL="$ACLOCAL $ACLOCAL_FLAGS"

## Initial sanity check, done here so that users get told they 
## have the wrong dependencies as early in the process as possible.
## Later on we actually use the cflags/libs from separate pkg-config
## calls. Oh, also the later pkg-config calls don't include 
## the version requirements since those make the module lists 
## annoying to construct
PKG_CHECK_MODULES(BASE_DEPENDENCIES,
  [glib-2.0 >= glib_required_version dnl
   pango >= pango_required_version dnl
   cairo >= cairo_required_version dnl
   gdk-pixbuf-2.0 >= gdk_pixbuf_required_version])


if test "x$enable_atk" = "xyes"; then
  # Check for ATK.  If it's present, great.  If not, that can work too.
  AC_MSG_CHECKING(for ATK)
	  
  have_atk=false
  if $PKG_CONFIG --atleast-version="${atk_required_version}" --exists atk; then
    have_atk=true
    AC_MSG_RESULT(yes)
  else
    AC_MSG_RESULT(no)
    AC_MSG_WARN([ATK is not installed.  STLWRT will not be built with accessibility support!])
  fi
fi

## In addition to checking that cairo is present, we also need to
## check that the correct cairo backend is there. E.g. if the STLWRT
## target is win32 we need the cairo-win32 backend and so on.
cairo_backend=$stlwrt_target

# STLWRT calls the xlib backend "x11," cairo calls it "xlib."  Technically
# speaking, "x11" is more correct, since today the implementation of the "Xlib"
# library is actually XCB.  Other backend names are identical.
if test "x$cairo_backend" = "xx11"; then
   cairo_backend=xlib
fi
PKG_CHECK_MODULES(CAIRO_BACKEND,
  [cairo-$cairo_backend >= cairo_required_version])

PKG_CHECK_MODULES(GMODULE, [gmodule-2.0])

if test "$os_win32" != yes; then
    # libtool option to control which symbols are exported
    # right now, symbols starting with _ are not exported
    LIBTOOL_EXPORT_OPTIONS='-export-symbols-regex "^[[^_]].*"'
else
    # We currently use .def files on Windows
    LIBTOOL_EXPORT_OPTIONS=
fi
AC_SUBST(LIBTOOL_EXPORT_OPTIONS)

dnl ******************************************************
dnl * See whether to include shared library dependencies *
dnl ******************************************************

AC_ARG_ENABLE(explicit-deps, 
              [AC_HELP_STRING([--enable-explicit-deps=@<:@yes/no/auto@:>@],
                              [use explicit dependencies in .pc files [default=auto]])],,
              [enable_explicit_deps=auto])

AC_MSG_CHECKING([Whether to write dependencies into .pc files])
case $enable_explicit_deps in
  auto)
    export SED
    deplibs_check_method=`(./libtool --config; echo 'eval echo \"$deplibs_check_method\"') | sh`
    if test "x$deplibs_check_method" '!=' xpass_all || test "x$enable_static" = xyes ; then
      enable_explicit_deps=yes  
    else
      enable_explicit_deps=no  
    fi
  ;;
  yes|no) 
  ;;
  *) AC_MSG_ERROR([Value given to --enable-explicit-deps must be one of yes, no or auto])
  ;;
esac
AC_MSG_RESULT($enable_explicit_deps)

AM_CONDITIONAL(DISABLE_EXPLICIT_DEPS, test $enable_explicit_deps = no)

# define a MAINT-like variable REBUILD which is set if Perl
# and awk are found, so autogenerated sources can be rebuilt

AC_PATH_PROGS(PERL, perl5 perl)

# We would like indent, but don't require it.
AC_CHECK_PROG(INDENT, indent, indent)

REBUILD=\#
if test "x$enable_rebuilds" = "xyes" && \
     test -n "$PERL" && \
     $PERL -e 'exit !($] >= 5.002)' > /dev/null 2>&1 ; then
  REBUILD=
fi
AC_SUBST(REBUILD)

AC_CHECK_FUNCS(lstat mkstemp flockfile getc_unlocked)
AC_CHECK_FUNCS(localtime_r)

# _NL_TIME_FIRST_WEEKDAY is an enum and not a define
AC_MSG_CHECKING([for _NL_TIME_FIRST_WEEKDAY])
AC_TRY_LINK([#include <langinfo.h>], [
char c;
c = *((unsigned char *)  nl_langinfo(_NL_TIME_FIRST_WEEKDAY));
], stlwrt_ok=yes, stlwrt_ok=no)
AC_MSG_RESULT($stlwrt_ok)
if test "$stlwrt_ok" = "yes"; then
  AC_DEFINE([HAVE__NL_TIME_FIRST_WEEKDAY], [1],
	    [Define if _NL_TIME_FIRST_WEEKDAY is available])
fi

# _NL_MEASUREMENT_MEASUREMENT is an enum and not a define
AC_MSG_CHECKING([for _NL_MEASUREMENT_MEASUREMENT])
AC_TRY_LINK([#include <langinfo.h>], [
char c;
c = *((unsigned char *)  nl_langinfo(_NL_MEASUREMENT_MEASUREMENT));
], stlwrt_ok=yes, stlwrt_ok=no)
AC_MSG_RESULT($stlwrt_ok)
if test "$stlwrt_ok" = "yes"; then
  AC_DEFINE([HAVE__NL_MEASUREMENT_MEASUREMENT], [1],
	    [Define if _NL_MEASUREMENT_MEASUREMENT is available])
fi

# _NL_PAPER_HEIGHT is an enum and not a define
AC_MSG_CHECKING([for _NL_PAPER_HEIGHT])
AC_TRY_LINK([#include <langinfo.h>], [
char c;
c = *((unsigned char *)  nl_langinfo(_NL_PAPER_HEIGHT));
], stlwrt_ok=yes, stlwrt_ok=no)
AC_MSG_RESULT($stlwrt_ok)
if test "$stlwrt_ok" = "yes"; then
  AC_DEFINE([HAVE__NL_PAPER_HEIGHT], [1],
	    [Define if _NL_PAPER_HEIGHT is available])
fi

# _NL_PAPER_WIDTH is an enum and not a define
AC_MSG_CHECKING([for _NL_PAPER_WIDTH])
AC_TRY_LINK([#include <langinfo.h>], [
char c;
c = *((unsigned char *)  nl_langinfo(_NL_PAPER_WIDTH));
], stlwrt_ok=yes, stlwrt_ok=no)
AC_MSG_RESULT($stlwrt_ok)
if test "$stlwrt_ok" = "yes"; then
  AC_DEFINE([HAVE__NL_PAPER_WIDTH], [1],
	    [Define if _NL_PAPER_WIDTH is available])
fi
	
# sigsetjmp is a macro on some platforms, so AC_CHECK_FUNCS is not reliable
AC_MSG_CHECKING(for sigsetjmp)
AC_TRY_LINK([#include <setjmp.h>], [
sigjmp_buf env;
sigsetjmp(env, 0);
], stlwrt_ok=yes, stlwrt_ok=no)
AC_MSG_RESULT($stlwrt_ok)
if test "$stlwrt_ok" = "yes"; then
  AC_DEFINE(HAVE_SIGSETJMP, 1,
            [Define to 1 if sigsetjmp is available])
fi

# i18n stuff
ALL_LINGUAS="`grep -v '^#' "$srcdir/po/LINGUAS" | tr '\n' ' '`"
AM_GLIB_GNU_GETTEXT
LIBS="$LIBS $INTLLIBS"
AC_OUTPUT_COMMANDS([case "$CONFIG_FILES" in *po-properties/Makefile.in*)
        sed -e "/POTFILES =/r po-properties/POTFILES" po-properties/Makefile.in > po-properties/Makefile
      esac])

dnl Snippet below is copied from AM_GLIB_GNU_GETTEXT to generate a first
dnl po-properties/POTFILES during configure; see GNOME #573515.
dnl
dnl Generate list of files to be processed by xgettext which will
dnl be included in po-properties/Makefile.
test -d po-properties || mkdir po-properties
if test "x$srcdir" != "x."; then
  if test "x`echo $srcdir | sed 's@/.*@@'`" = "x"; then
    popropsrcprefix="$srcdir/"
  else
    popropsrcprefix="../$srcdir/"
  fi
else
  popropsrcprefix="../"
fi
rm -f po-properties/POTFILES
sed -e "/^#/d" -e "/^\$/d" -e "s,.*,	$popropsrcprefix& \\\\," -e "\$s/\(.*\) \\\\/\1/" \
< $srcdir/po-properties/POTFILES.in > po-properties/POTFILES
dnl (End of adapted AM_GLIB_GNU_GETTEXT snippet.)

AM_GLIB_DEFINE_LOCALEDIR(STLWRT_LOCALEDIR)

dnl The DU4 header files don't provide library prototypes unless 
dnl -std1 is given to the native cc.
AC_MSG_CHECKING([for extra flags to get ANSI library prototypes])

stlwrt_save_LIBS=$LIBS
LIBS="$LIBS -lm"
AC_TRY_RUN([#include <math.h>
             int main (void) { return (log(1) != log(1.)); }],
     AC_MSG_RESULT(none needed),
     stlwrt_save_CFLAGS="$CFLAGS"
     CFLAGS="$CFLAGS -std1"
     AC_TRY_RUN([#include <math.h>
	         int main (void) { return (log(1) != log(1.)); }],
         AC_MSG_RESULT(-std1),
         AC_MSG_RESULT()
         CFLAGS="$stlwrt_save_CFLAGS"
         AC_MSG_WARN(
                [No ANSI prototypes found in library. (-std1 didn't work.)]),
	 true
     ),
     AC_MSG_RESULT(none needed)
)
LIBS=$stlwrt_save_LIBS

AC_MSG_CHECKING(for the BeOS)
case $host in
  *-*-beos*)
    AC_MSG_RESULT(yes)
    MATH_LIB=
  ;;
  *)
    AC_MSG_RESULT(no)
  ;;
esac

AC_SUBST(MATH_LIB)
#
# see bug 162979
#
AC_MSG_CHECKING(for HP-UX)
case $host_os in
  hpux9* | hpux10* | hpux11*)
    AC_MSG_RESULT(yes)
    CFLAGS="$CFLAGS -DHPPEX -DSHMLINK"
  ;;
  *)
    AC_MSG_RESULT(no)
  ;;
esac

#
# Run AM_PATH_GLIB_2_0 to make sure that GLib is installed and working
# 

GLIB_PACKAGES="gobject-2.0 gio-2.0 gmodule-no-export-2.0"

AM_PATH_GLIB_2_0(glib_required_version, :,
  AC_MSG_ERROR([
*** GLIB glib_required_version or better is required. The latest version of 
*** GLIB is always available from ftp://ftp.gtk.org/pub/gtk/.]),
  gobject gmodule-no-export gthread)

# See if it's safe to turn G_DISABLE_DEPRECATED on.
GLIB_VERSION_MAJOR_MINOR=`$PKG_CONFIG --modversion glib-2.0 | sed "s/\.@<:@^.@:>@*\$//"`
GLIB_REQUIRED_VERSION_MAJOR_MINOR=`echo glib_required_version | sed "s/\.@<:@^.@:>@*\$//"`
if test "x$GLIB_VERSION_MAJOR_MINOR" = "x$GLIB_REQUIRED_VERSION_MAJOR_MINOR"; then
  CFLAGS="-DG_DISABLE_DEPRECATED $CFLAGS"
fi


dnl
dnl Check for bind_textdomain_codeset, including -lintl if GLib brings it in.
dnl
stlwrt_save_LIBS=$LIBS
LIBS="$LIBS $GLIB_LIBS"
AC_CHECK_FUNCS(bind_textdomain_codeset)
LIBS=$stlwrt_save_LIBS

AC_CHECK_HEADERS(pwd.h,
                 AC_DEFINE(HAVE_PWD_H, 1,
                           [Define to 1 if pwd.h is available]))
AC_CHECK_HEADERS(sys/time.h,
                 AC_DEFINE(HAVE_SYS_TIME_H, 1,
                           [Define to 1 if time.h is available]))
AC_CHECK_HEADERS(unistd.h,
                 AC_DEFINE(HAVE_UNISTD_H, 1,
                           [Define to 1 if unistd.h is available]))
AC_CHECK_HEADERS(ftw.h,
                 AC_DEFINE(HAVE_FTW_H, 1,
                           [Define to 1 if ftw.h is available]))

AC_MSG_CHECKING([for GNU ftw extensions])
AC_TRY_COMPILE([#define _XOPEN_SOURCE 500
#define _GNU_SOURCE
#include <ftw.h>], [int flags = FTW_ACTIONRETVAL;], stlwrt_ok=yes, stlwrt_ok=no)
if test $stlwrt_ok = yes; then
    AC_MSG_RESULT([yes])
    AC_DEFINE(HAVE_GNU_FTW, 1, [Have GNU ftw])
else
    AC_MSG_RESULT([no])
fi

saved_cflags="$CFLAGS"
saved_ldflags="$LDFLAGS"


# Checks for header files.
AC_HEADER_STDC

# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST

# Checks for library functions.
AC_TYPE_SIGNAL
AC_FUNC_MMAP

AC_CHECK_FUNCS(mallinfo)
AC_CHECK_FUNCS(getresuid)
AC_TYPE_UID_T

# Check if <sys/select.h> needs to be included for fd_set
AC_MSG_CHECKING([for fd_set])
AC_TRY_COMPILE([#include <sys/types.h>],
        [fd_set readMask, writeMask;], stlwrt_ok=yes, stlwrt_ok=no)
if test $stlwrt_ok = yes; then
    AC_MSG_RESULT([yes, found in sys/types.h])
else
    AC_HEADER_EGREP(fd_mask, sys/select.h, stlwrt_ok=yes)
    if test $stlwrt_ok = yes; then
        AC_DEFINE(HAVE_SYS_SELECT_H, 1,
                  [Define to 1 if sys/select.h is available])
        AC_MSG_RESULT([yes, found in sys/select.h])
    else
	AC_DEFINE(NO_FD_SET, 1,
                  [Define to 1 if fd_set is not available])
	AC_MSG_RESULT(no)
    fi
fi

# `widechar' tests for gdki18n.h
AC_MSG_CHECKING(for wchar.h)
AC_TRY_CPP([#include <wchar.h>], gdk_wchar_h=yes, gdk_wchar_h=no)
if test $gdk_wchar_h = yes; then
   AC_DEFINE(HAVE_WCHAR_H, 1, [Have wchar.h include file])
fi
AC_MSG_RESULT($gdk_wchar_h)

# Check for wctype.h (for iswalnum)
AC_MSG_CHECKING(for wctype.h)
AC_TRY_CPP([#include <wctype.h>], gdk_wctype_h=yes, gdk_wctype_h=no)
if test $gdk_wctype_h = yes; then
   AC_DEFINE(HAVE_WCTYPE_H, 1, [Have wctype.h include file])
fi
AC_MSG_RESULT($gdk_wctype_h)

# in Solaris 2.5, `iswalnum' is in -lw
GDK_WLIBS=
AC_CHECK_FUNC(iswalnum,,[AC_CHECK_LIB(w,iswalnum,GDK_WLIBS=-lw)])

oLIBS="$LIBS"
LIBS="$LIBS $GDK_WLIBS"
# The following is necessary for Linux libc-5.4.38
AC_MSG_CHECKING(if iswalnum() and friends are properly defined)
AC_TRY_LINK([#include <stdlib.h>],[
#if (defined(HAVE_WCTYPE_H) || defined(HAVE_WCHAR_H))
#  ifdef HAVE_WCTYPE_H
#    include <wctype.h>
#  else
#    ifdef HAVE_WCHAR_H
#      include <wchar.h>
#    endif
#  endif
#else
#  define iswalnum(c) ((wchar_t)(c) <= 0xFF && isalnum(c))
#endif
iswalnum((wchar_t) 0);
], gdk_working_wctype=yes, gdk_working_wctype=no)
LIBS="$oLIBS"

if test $gdk_working_wctype = no; then
   AC_DEFINE(HAVE_BROKEN_WCTYPE, 1, [Is the wctype implementation broken])
   GDK_WLIBS=
fi
AC_MSG_RESULT($gdk_working_wctype)
AC_SUBST(GDK_WLIBS)

# Check for uxtheme.h (for MS-Windows Engine)
AC_MSG_CHECKING(for uxtheme.h)
AC_TRY_CPP([#include <uxtheme.h>], stlwrt_uxtheme_h=yes, stlwrt_uxtheme_h=no)
if test $stlwrt_uxtheme_h = yes; then
   AC_DEFINE(HAVE_UXTHEME_H, 1, [Have uxtheme.h include file])
fi
AC_MSG_RESULT($stlwrt_uxtheme_h)

# Checks for gdkspawn
AC_CHECK_HEADERS(crt_externs.h)
AC_CHECK_FUNCS(_NSGetEnviron)

AC_MSG_CHECKING(whether to build dynamic modules)

AC_ARG_ENABLE(modules,
              [AC_HELP_STRING([--disable-modules],
                              [disable dynamic module loading])])

dynworks=false
deps=
if test x$enable_modules = xno; then
    AC_MSG_RESULT(no)
else
    AC_MSG_RESULT(yes)
    AC_MSG_CHECKING(whether dynamic modules work)
    ## for loop is to strip newline 
    tmp=`$PKG_CONFIG --variable=gmodule_supported gmodule-no-export-2.0`
    for I in $tmp; do
        dynworks=$I
    done

    dnl Now we check to see if our libtool supports shared lib deps
    dnl (in a rather ugly way even)
    if $dynworks; then
        module_libtool_config="${CONFIG_SHELL-/bin/sh} ./libtool --config"
        module_deplibs_check=`$module_libtool_config | \
            grep '^[[a-z_]]*check[[a-z_]]*_method=[['\''"]]' | \
            sed 's/.*[['\''"]]\(.*\)[['\''"]]$/\1/'`
        if test "x$module_deplibs_check" = "xnone" || \
           test "x$module_deplibs_check" = "xunknown" || \
           test "x$module_deplibs_check" = "x"; then
            dynworks=false
        fi
    fi

    if $dynworks; then
        AC_DEFINE(USE_GMODULE, 1,
                  [Define to 1 if gmodule works and should be used])
        AC_MSG_RESULT(yes)
    else
        AC_MSG_RESULT(no)
    fi
fi

AM_CONDITIONAL(BUILD_DYNAMIC_MODULES, $dynworks)

#
# Allow building some or all immodules included
#
AC_MSG_CHECKING(immodules to build)

dnl due to an autoconf bug, commas in the first arg to
dnl AC_HELP_STRING cause problems.
dnl AC_HELP_STRING([--with-included-immodules=MODULE1 MODULE2 ...],
dnl                [build the specified input method modules into STLWRT])
AC_ARG_WITH(included_immodules,
[  --with-included-immodules=MODULE1,MODULE2,...
                          build the specified input methods into STLWRT])

if $dynworks; then 
   :
else
   ## if the option was specified, leave it; otherwise disable included immodules
   if test x$with_included_immodules = xno; then
           with_included_immodules=yes
   fi
fi

all_immodules="am-et,cedilla,cyrillic-translit"
if test "$stlwrttarget" = "win32"; then
   all_immodules="${all_immodules},ime"
fi
if test "$stlwrttarget" = "quartz"; then
   all_immodules="${all_immodules},quartz"
fi
all_immodules="${all_immodules},inuktitut,ipa,multipress,thai,ti-er,ti-et,viqr"
if test "$stlwrttarget" = "x11"; then
   all_immodules="${all_immodules},xim"
fi

included_immodules=""
# If the switch specified without listing any specific ones, include all
if test "x$with_included_immodules" = xyes ; then
  included_immodules="$all_immodules"
else
  included_immodules="$with_included_immodules"
fi

AC_MSG_RESULT($included_immodules)
AM_CONDITIONAL(HAVE_INCLUDED_IMMMODULES, test "x$included_immodules" != x)

INCLUDED_IMMODULE_OBJ=
INCLUDED_IMMODULE_DEFINE=

IFS="${IFS= 	}"; gtk_save_ifs="$IFS"; IFS=","
for immodule in $included_immodules; do
 immodule_underscores=`echo $immodule | sed -e 's/-/_/g'`
 if echo "$all_immodules" | egrep "(^|,)$immodule(\$|,)" > /dev/null; then
   :
 else
   AC_MSG_ERROR([the specified input method $immodule does not exist])
 fi

 INCLUDED_IMMODULE_OBJ="$INCLUDED_IMMODULE_OBJ ../modules/input/libstatic-im-$immodule.la"
 INCLUDED_IMMODULE_DEFINE="$INCLUDED_IMMODULE_DEFINE -DINCLUDE_IM_$immodule_underscores"
 eval INCLUDE_$immodule_underscores=yes
done
IFS="$gtk_save_ifs"
AC_SUBST(INCLUDED_IMMODULE_OBJ)
AC_SUBST(INCLUDED_IMMODULE_DEFINE)

AM_CONDITIONAL(INCLUDE_IM_AM_ET, [test x"$INCLUDE_am_et" = xyes])
AM_CONDITIONAL(INCLUDE_IM_CEDILLA, [test x"$INCLUDE_cedilla" = xyes])
AM_CONDITIONAL(INCLUDE_IM_CYRILLIC_TRANSLIT, [test x"$INCLUDE_cyrillic_translit" = xyes])
AM_CONDITIONAL(INCLUDE_IM_IME, [test x"$INCLUDE_ime" = xyes])
AM_CONDITIONAL(INCLUDE_IM_INUKTITUT, [test x"$INCLUDE_inuktitut" = xyes])
AM_CONDITIONAL(INCLUDE_IM_IPA, [test x"$INCLUDE_ipa" = xyes])
AM_CONDITIONAL(INCLUDE_IM_MULTIPRESS, [test x"$INCLUDE_multipress" = xyes])
AM_CONDITIONAL(INCLUDE_IM_QUARTZ, [test x"$INCLUDE_quartz" = xyes])
AM_CONDITIONAL(INCLUDE_IM_THAI, [test x"$INCLUDE_thai" = xyes])
AM_CONDITIONAL(INCLUDE_IM_TI_ER, [test x"$INCLUDE_ti_er" = xyes])
AM_CONDITIONAL(INCLUDE_IM_TI_ET, [test x"$INCLUDE_ti_et" = xyes])
AM_CONDITIONAL(INCLUDE_IM_VIQR, [test x"$INCLUDE_viqr" = xyes])
AM_CONDITIONAL(INCLUDE_IM_XIM, [test x"$INCLUDE_xim" = xyes])

AC_HEADER_SYS_WAIT

AC_TYPE_SIGNAL


dnl Look for a host system's gdk-pixbuf-csource if we are cross-compiling

AM_CONDITIONAL(CROSS_COMPILING, test $cross_compiling = yes)

if test $cross_compiling = yes; then
  AC_PATH_PROG(GTK_UPDATE_ICON_CACHE, gtk-update-icon-cache, no)
  if test x$GTK_UPDATE_ICON_CACHE = xno; then
    REBUILD_PNGS=#
  fi
fi

AC_PATH_PROG(GDK_PIXBUF_CSOURCE, gdk-pixbuf-csource, no)

if test ! -f $srcdir/gtk/gtkbuiltincache.h && 
   test "x$REBUILD_PNGS" = "x#" ; then
     AC_MSG_ERROR([
*** gtkbuiltincache.h is not in the tree, and cannot be built
*** because you don't have libpng, or (when cross-compiling) you 
*** don't have a prebuilt gtk-update-icon-cache on the build system.])
fi

########################################
# Windowing system checks
########################################

STLWRT_EXTRA_LIBS="$STLWRT_WLIBS"
STLWRT_EXTRA_CFLAGS=
	
# STLWRT uses some X calls, so needs to link against X directly
STLWRT_DEP_PACKAGES_FOR_X=
STLWRT_DEP_LIBS_FOR_X=

if test "x$stlwrt_target" = "xx11"; then
  X_PACKAGES=fontconfig

  #
  # We use fontconfig very peripherally when decoding the default
  # settings.
  #
  if $PKG_CONFIG --exists fontconfig; then : ; else
    AC_MSG_ERROR([
*** fontconfig (http://www.fontconfig.org) is required by the X11 backend.])
  fi

  #
  # Check for basic X packages; we use pkg-config if available
  #
  if $PKG_CONFIG --exists x11 xext xrender; then
    have_base_x_pc=true
    X_PACKAGES="$X_PACKAGES x11 xext xrender"
    x_libs="`$PKG_CONFIG --libs x11 xext xrender`"
    X_CFLAGS="`$PKG_CONFIG --cflags x11 xext xrender`"
    
    # Strip out any .la files that pkg-config might give us (this happens
    # with -uninstalled.pc files)
    x_libs_for_checks=
    for I in $x_libs ; do
      case $I in 
        *.la) ;;
        *) x_libs_for_checks="$x_libs_for_checks $I" ;;
      esac
    done

    STLWRT_PACKAGES_FOR_X="x11"
  else
    have_base_x_pc=false
    AC_PATH_XTRA
    if test x$no_x = xyes ; then
      AC_MSG_ERROR([X development libraries not found])
    fi
    
    x_cflags="$X_CFLAGS"
    x_libs_for_checks="$X_LIBS -lXext -lXrender -lX11 $X_EXTRA_LIBS"

    STLWRT_DEP_LIBS_FOR_X="$X_LIBS -lXrender -lX11 $X_EXTRA_LIBS"
  fi

  # Extra libraries found during checks (-lXinerama, etc), not from pkg-config.
  x_extra_libs=

  stlwrt_save_cppflags="$CPPFLAGS"
  CPPFLAGS="$CPPFLAGS $X_CFLAGS"

  stlwrt_save_LIBS=$LIBS
  LIBS="$x_libs_for_checks $LIBS"
  
  # Sanity check for the X11 and Xext libraries. While everything we need from
  # Xext is optional, the chances a system has *none* of these things is so
  # small that we just unconditionally require it.
  AC_CHECK_FUNC(XOpenDisplay, :,
                AC_MSG_ERROR([*** libX11 not found. Check 'config.log' for more details.]))
  AC_CHECK_FUNC(XextFindDisplay, :,
                AC_MSG_ERROR([*** libXext not found. Check 'config.log' for more details.]))
  AC_CHECK_FUNC(XRenderQueryExtension, :,
                AC_MSG_ERROR([*** libXrender not found. Check 'config.log' for more details.]))

  # Check for xReply

  AC_MSG_CHECKING([if <X11/extensions/XIproto.h> is needed for xReply])
  AC_TRY_COMPILE([#include <X11/Xlibint.h>],
      [xReply *rep;],
      [AC_MSG_RESULT([no])],
      [AC_TRY_COMPILE([#include <X11/extensions/XIproto.h>
#include <X11/Xlibint.h>],
           [xReply *rep;],
           [AC_MSG_RESULT([yes])
            AC_DEFINE([NEED_XIPROTO_H_FOR_XREPLY], 1,
                      [Define if <X11/extensions/XIproto.h> needed for xReply])],
           [AC_MSG_RESULT([unknown])
            AC_MSG_ERROR([xReply type unavailable. X11 is too old])])])

  # Check for XConvertCase, XInternAtoms (X11R6 specific)

  AC_CHECK_FUNCS(XConvertCase XInternAtoms)

  # Generic X11R6 check needed for XIM support; we could
  # probably use this to replace the above, but we'll
  # leave the separate checks for XConvertCase and XInternAtoms 
  # for clarity
  
  have_x11r6=false
  AC_CHECK_FUNC(XAddConnectionWatch,
      have_x11r6=true)

  if $have_x11r6; then
    AC_DEFINE(HAVE_X11R6, 1, [Define if we have X11R6])
  fi
  AM_CONDITIONAL(HAVE_X11R6, $have_x11r6)

  # Check for XKB support.

  if test "x$enable_xkb" = "xyes"; then
        AC_MSG_WARN(XKB support explicitly enabled)
        AC_DEFINE(HAVE_XKB, 1, [Define to use XKB extension])
  elif test "x$enable_xkb" = "xmaybe"; then
        AC_CHECK_FUNC(XkbQueryExtension,
                      AC_DEFINE(HAVE_XKB, 1, [Define to use XKB extension]))
  else
        AC_MSG_WARN(XKB support explicitly disabled)
  fi

  # Check for shaped window extension

  AC_CHECK_FUNC(XShapeCombineMask, :,
     [AC_MSG_ERROR([Shape extension not found, check your development headers])])

  # X SYNC check
  stlwrt_save_CFLAGS="$CFLAGS"
  CFLAGS="$CFLAGS $x_cflags"
  
  AC_CHECK_FUNC(XSyncQueryExtension,
      [AC_CHECK_HEADER(X11/extensions/sync.h,
	  AC_DEFINE(HAVE_XSYNC, 1, [Have the SYNC extension library]),
	  :, [#include <X11/Xlib.h>])])

  CFLAGS="$stlwrt_save_CFLAGS"

  # Xshm checks

  if test "x$enable_shm" = "xyes"; then
     # Check for the XShm extension, normally in Xext
     AC_CHECK_FUNC(XShmAttach,
	:,
	# On AIX, it is in XextSam instead
	[AC_CHECK_LIB(XextSam, XShmAttach, 
	    [STLWRT_ADD_LIB(x_extra_libs,XextSam)])])
  fi    

  if test "x$enable_shm" = "xyes"; then
    # Check for shared memory
    AC_CHECK_HEADER(sys/ipc.h,
                    AC_DEFINE(HAVE_IPC_H, 1,
                              [Define to 1 if ipc.h is available]),
                    no_sys_ipc=yes)
    AC_CHECK_HEADER(sys/shm.h,
                    AC_DEFINE(HAVE_SHM_H, 1,
                              [Define to 1 if shm.h is available]),
                    no_sys_shm=yes)

    # Check for the X shared memory extension header file
    have_xshm=no 
    AC_MSG_CHECKING(X11/extensions/XShm.h)
    if test "x$no_xext_lib" = "xyes"; then
      :
    else
      stlwrt_save_CFLAGS="$CFLAGS"
      CFLAGS="$CFLAGS $x_cflags"
      AC_TRY_COMPILE([
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/extensions/XShm.h>
], [XShmSegmentInfo *x_shm_info;], have_xshm=yes)
      CFLAGS="$stlwrt_save_CFLAGS"
    fi
    AC_MSG_RESULT($have_xshm)
    if test $have_xshm = yes ; then
      AC_DEFINE(HAVE_XSHM_H, 1,
                [Define to 1 if xshm.h is available])
    fi
  fi

  if test "x$enable_xinerama" = "xyes"; then
    # Check for Xinerama extension (Solaris impl or Xfree impl)
    stlwrt_save_cppflags="$CPPFLAGS"
    CPPFLAGS="$CPPFLAGS $x_cflags"
  
    # Check for XFree
    AC_MSG_CHECKING(for Xinerama support on XFree86)
	  
    have_xfree_xinerama=false
    if $PKG_CONFIG --exists xinerama ; then
       have_xfree_xinerama=true
       X_PACKAGES="$X_PACKAGES xinerama"
    else    
       AC_CHECK_LIB(Xinerama, XineramaQueryExtension,
	   [AC_CHECK_HEADER(X11/extensions/Xinerama.h, 
	   [STLWRT_ADD_LIB(x_extra_libs,Xinerama)
	   have_xfree_xinerama=true], :,
           [#include <X11/Xlib.h>])])
    fi

    if $have_xfree_xinerama ; then
      AC_DEFINE(HAVE_XFREE_XINERAMA, 1,
                [Define to 1 if XFree Xinerama is available])
      AC_DEFINE(HAVE_XINERAMA, 1,
                [Define to 1 is Xinerama is available])
      AC_MSG_RESULT(yes)
    else
      AC_MSG_RESULT(no)

      case "$host" in
        *-*-solaris*)
            # Check for solaris
	    AC_MSG_CHECKING(for Xinerama support on Solaris)
	  
	    have_solaris_xinerama=false
	    AC_CHECK_FUNC(XineramaGetInfo,
	        [AC_CHECK_HEADER(X11/extensions/xinerama.h,
  	  	    [have_solaris_xinerama=true], :,
		    [#include <X11/Xlib.h>])])
		
            if $have_solaris_xinerama ; then
              AC_DEFINE(HAVE_SOLARIS_XINERAMA, 1,
                        [Define to 1 if solaris xinerama is available])
	      AC_DEFINE(HAVE_XINERAMA, 1,
                        [Define to 1 if xinerama is available])
              AC_MSG_RESULT(yes)
            else
              AC_MSG_RESULT(no)
            fi
            ;;
        *)
            ;;
      esac
    fi
  fi
  
  # set up things for XInput

  if test "x$with_xinput" = "xxfree" || test "x$with_xinput" = "xyes"; then
    AC_DEFINE(XINPUT_XFREE, 1,
              [Define to 1 if XFree XInput should be used])
    
    if $PKG_CONFIG --exists xi ; then
      X_PACKAGES="$X_PACKAGES xi"
    else
      STLWRT_ADD_LIB(x_extra_libs, Xi)
    fi
  else
    AC_DEFINE(XINPUT_NONE, 1,
              [Define to 1 if no XInput should be used])
  fi

  AM_CONDITIONAL(XINPUT_XFREE, test x$with_xinput = xxfree || test x$with_xinput = xyes)

  # Check for the RANDR extension
  if $PKG_CONFIG --exists "xrandr >= 1.2.99" ; then
     AC_DEFINE(HAVE_RANDR, 1, [Have the Xrandr extension library])

     if $PKG_CONFIG --exists "xrandr >= 1.5.0" ; then
       AC_DEFINE(HAVE_RANDR15, 1, [Have the Xrandr 1.5 extension library])
     fi
     X_PACKAGES="$X_PACKAGES xrandr"
  fi
       
  # Checks for Xcursor library
  
  if $PKG_CONFIG --exists xcursor ; then
    AC_DEFINE(HAVE_XCURSOR, 1, [Have the Xcursor library])
    
    X_PACKAGES="$X_PACKAGES xcursor"
  fi

  # Checks for XFixes extension
  
  if $PKG_CONFIG --exists xfixes ; then
    AC_DEFINE(HAVE_XFIXES, 1, [Have the XFIXES X extension])
    
    X_PACKAGES="$X_PACKAGES xfixes"
    STLWRT_PACKAGES_FOR_X="$STLWRT_PACKAGES_FOR_X xfixes"
  fi

  # Checks for Xcomposite extension

  if $PKG_CONFIG --exists xcomposite ; then
    AC_DEFINE(HAVE_XCOMPOSITE, 1, [Have the XCOMPOSITE X extension])

    X_PACKAGES="$X_PACKAGES xcomposite"
    STLWRT_PACKAGES_FOR_X="$STLWRT_PACKAGES_FOR_X xcomposite"
  fi

  # Checks for Xdamage extension

  if $PKG_CONFIG --exists xdamage ; then
    AC_DEFINE(HAVE_XDAMAGE, 1, [Have the XDAMAGE X extension])

    X_PACKAGES="$X_PACKAGES xdamage"
    STLWRT_PACKAGES_FOR_X="$STLWRT_PACKAGES_FOR_X xdamage"
  fi

  if $have_base_x_pc ; then
    STLWRT_EXTRA_LIBS="$x_extra_libs"
  else
    STLWRT_EXTRA_LIBS="$X_LIBS $x_extra_libs -lXext -lX11 $STLWRT_EXTRA_LIBS"
  fi

  CPPFLAGS="$stlwrt_save_cppflags"
  LIBS="$stlwrt_save_libs"

  AM_CONDITIONAL(USE_X11, true)
else
  XPACKAGES=

  AM_CONDITIONAL(XINPUT_XFREE, false)
  AM_CONDITIONAL(USE_X11, false)
  AM_CONDITIONAL(HAVE_X11R6, false)
fi

if test "x$stlwrt_target" = "xwin32"; then
  STLWRT_EXTRA_LIBS="$STLWRT_EXTRA_LIBS -lgdi32 -limm32 -lshell32 -lole32 -Wl,-luuid"
  AM_CONDITIONAL(USE_WIN32, true)
else
  AM_CONDITIONAL(USE_WIN32, false)
fi

AC_ARG_ENABLE(quartz-relocation,
              [AS_HELP_STRING([--enable-quartz-relocation],
                              [enable bundle-based relocation functions])],
                              [quartz_relocation=yes])

if test "x$stlwrt_target" = "xquartz"; then
  STLWRT_EXTRA_LIBS="$STLWRT_EXTRA_LIBS -framework Cocoa"
  AM_CONDITIONAL(USE_QUARTZ, true)
  if test "x$quartz_relocation" = xyes; then
    AC_DEFINE([QUARTZ_RELOCATION], [1], [Use NSBundle functions to determine load paths for libraries, translations, etc.])
  fi

else
  AM_CONDITIONAL(USE_QUARTZ, false)
fi

# This section is supposed to check for DirectFB.
# In practice DirectFB is archaic (no offense to anybody who still works on
# DirectFB); the only reason I'm leaving this code in here for the time being is
# I want to use it as a template for adding Wayland support eventually.
#if test "x$stlwrt_target" = "xdirectfb"; then
#  DIRECTFB_REQUIRED_VERSION=1.0.0
#  AC_MSG_CHECKING(for DirectFB)
#
#  PKG_CHECK_MODULES(DIRECTFB, [directfb >= $DIRECTFB_REQUIRED_VERSION])
#  AM_CONDITIONAL(USE_DIRECTFB, true)
#else
#  AM_CONDITIONAL(USE_DIRECTFB, false)
#fi


# Check for Pango flags

if test "x$stlwrt_target" = "xwin32"; then
	PANGO_PACKAGES="pangowin32 pangocairo"
else
	PANGO_PACKAGES="pango pangocairo"
fi

AC_MSG_CHECKING(Pango flags)
if $PKG_CONFIG --exists $PANGO_PACKAGES ; then
        PANGO_CFLAGS=`$PKG_CONFIG --cflags $PANGO_PACKAGES`
        PANGO_LIBS=`$PKG_CONFIG --libs $PANGO_PACKAGES`

        AC_MSG_RESULT($PANGO_CFLAGS $PANGO_LIBS)
else
        AC_MSG_ERROR([
*** Pango not found. Pango built with Cairo support is required
*** to build STLWRT. See http://www.pango.org for Pango information.
])
fi

CFLAGS="$CFLAGS $PANGO_CFLAGS"

if $PKG_CONFIG --uninstalled $PANGO_PACKAGES; then
        :
else
	gtk_save_LIBS="$LIBS"
        LIBS="$PANGO_LIBS $LIBS"
        AC_TRY_LINK_FUNC(pango_context_new, :, AC_MSG_ERROR([
*** Can't link to Pango. Pango is required to build
*** STLWRT. For more information see http://www.pango.org]))
        LIBS="$gtk_save_LIBS"
fi

CFLAGS="$saved_cflags"
LDFLAGS="$saved_ldflags"

# Pull in gio-unix for GDesktopAppInfo usage, see at least gdkapplaunchcontext-x11.c
if test "x$stlwrt_target" = "xx11"; then
  STLWRT_PACKAGES="$PANGO_PACKAGES gio-unix-2.0 $X_PACKAGES gdk-pixbuf-2.0 cairo-$cairo_backend"
else
  STLWRT_PACKAGES="$PANGO_PACKAGES gio-2.0 gdk-pixbuf-2.0 cairo-$cairo_backend"
fi

STLWRT_DEP_LIBS="$STLWRT_EXTRA_LIBS `$PKG_CONFIG --libs $STLWRT_PACKAGES` $MATH_LIB"
STLWRT_DEP_CFLAGS="`$PKG_CONFIG --cflags  gthread-2.0 $STLWRT_PACKAGES` $STLWRT_EXTRA_CFLAGS"
#
# If we aren't writing explicit dependencies, then don't put the extra libraries we need
# into the pkg-config files
#
if test $enable_explicit_deps != yes ; then
  STLWRT_PACKAGES="$PANGO_PACKAGES gdk-pixbuf-2.0"
  STLWRT_EXTRA_LIBS=
fi

AC_SUBST(STLWRT_PACKAGES)
AC_SUBST(STLWRT_EXTRA_LIBS)
AC_SUBST(STLWRT_EXTRA_CFLAGS)
AC_SUBST(STLWRT_DEP_LIBS)
AC_SUBST(STLWRT_DEP_CFLAGS)


########################################
# Check for Accessibility Toolkit flags
########################################

if test x$enable_atk = "xyes" && test x$have_atk = "xtrue"; then
	ATK_PACKAGES=atk
	AC_MSG_CHECKING(ATK flags)
	if $PKG_CONFIG --exists $ATK_PACKAGES ; then
		ATK_CFLAGS=`$PKG_CONFIG --cflags $ATK_PACKAGES`
		ATK_LIBS=`$PKG_CONFIG --libs $ATK_PACKAGES`

		AC_MSG_RESULT($ATK_CFLAGS $ATK_LIBS)
	else
		AC_MSG_ERROR([
	*** Accessibility Toolkit not found. Accessibility Toolkit is required
	*** for accessibility support in STLWRT.
	])
	fi

	if $PKG_CONFIG --uninstalled $ATK_PACKAGES; then
		:
	else
		gtk_save_LIBS="$LIBS"
		LIBS="$ATK_LIBS $LIBS"
		AC_TRY_LINK_FUNC(atk_object_get_type, : , AC_MSG_ERROR([
		        *** Cannot link to Accessibility Toolkit. Accessibility Toolkit is required
		        *** for accessibility support in STLWRT.]))
		LIBS="$gtk_save_LIBS"
	fi
	
	STLWRT_PACKAGES="atk"
else
	STLWRT_PACKAGES=""
	ATK_CFLAGS="-DSTLWRT_WITHOUT_ATK"
fi

STLWRT_PACKAGES="$STLWRT_PACKAGES cairo gdk-pixbuf-2.0 gio-2.0"
if test "x$stlwrt_target" = "xx11"; then
  STLWRT_PACKAGES="$STLWRT_PACKAGES pangoft2"
fi
STLWRT_EXTRA_LIBS=
STLWRT_EXTRA_CFLAGS= 
STLWRT_DEP_LIBS="$STLWRT_EXTRA_LIBS $STLWRT_DEP_LIBS_FOR_X `$PKG_CONFIG --libs $PANGO_PACKAGES $STLWRT_PACKAGES_FOR_X $STLWRT_PACKAGES` $STLWRT_EXTRA_LIBS $MATH_LIB"
STLWRT_DEP_CFLAGS="`$PKG_CONFIG --cflags  gthread-2.0 $STLWRT_PACKAGES $STLWRT_PACKAGES` $STLWRT_EXTRA_CFLAGS $STLWRT_EXTRA_CFLAGS"

if test x"$os_win32" = xyes; then
  STLWRT_EXTRA_CFLAGS="$msnative_struct"
fi

GLIB_PREFIX="`$PKG_CONFIG --variable=prefix glib-2.0`"
ATK_PREFIX="`$PKG_CONFIG --variable=prefix atk`"
PANGO_PREFIX="`$PKG_CONFIG --variable=prefix pango`"
CAIRO_PREFIX="`pkg-config --variable=prefix cairo`"

AC_SUBST(STLWRT_PACKAGES)
AC_SUBST(STLWRT_EXTRA_LIBS)
AC_SUBST(STLWRT_EXTRA_CFLAGS)
AC_SUBST(STLWRT_DEP_LIBS)
AC_SUBST(STLWRT_DEP_CFLAGS)

AC_SUBST(GLIB_PREFIX)
AC_SUBST(ATK_PREFIX)
AC_SUBST(PANGO_PREFIX)
AC_SUBST(CAIRO_PREFIX)

AC_SUBST(STLWRT_DEBUG_FLAGS)
AC_SUBST(STLWRT_XIM_FLAGS)

GDK_PIXBUF_LIBS=`$PKG_CONFIG --libs gdk-pixbuf-2.0`
AC_SUBST(GDK_PIXBUF_LIBS)

################################################################
# Printing system checks
################################################################

AC_ARG_ENABLE(cups,
              [AC_HELP_STRING([--disable-cups]
                              [disable cups print backend])],,
              [enable_cups=auto])

if test "x$enable_cups" = "xno"; then
  AM_CONDITIONAL(HAVE_CUPS, false)
else
  AC_PATH_TOOL(CUPS_CONFIG, cups-config, no)
  if test "x$CUPS_CONFIG" = "xno"; then
    if test "x$enable_cups" = "xauto"; then
      AM_CONDITIONAL(HAVE_CUPS, false)
    else
      AC_MSG_ERROR([
*** cups not found.
])
    fi
  else
    CUPS_CFLAGS=`$CUPS_CONFIG --cflags | sed 's/-O[0-9]*//' | sed 's/-m[^\t]*//g'`
    CUPS_LIBS=`$CUPS_CONFIG --libs`

    CUPS_API_VERSION=`$CUPS_CONFIG --api-version`
    CUPS_API_MAJOR=`echo $ECHO_N $CUPS_API_VERSION | awk -F. '{print $1}'`
    CUPS_API_MINOR=`echo $ECHO_N $CUPS_API_VERSION | awk -F. '{print $2}'`

    if test $CUPS_API_MAJOR -gt 1 -o \
            $CUPS_API_MAJOR -eq 1 -a $CUPS_API_MINOR -ge 2; then
      AC_DEFINE(HAVE_CUPS_API_1_2, 1, 
                [Define to 1 if CUPS 1.2 API is available])
    fi
    if test $CUPS_API_MAJOR -gt 1 -o \
	    $CUPS_API_MAJOR -eq 1 -a $CUPS_API_MINOR -ge 6; then
      AC_DEFINE(HAVE_CUPS_API_1_6, 1,
                [Define to 1 if CUPS 1.6 API is available])
      have_cups_api_1_6=yes
    fi

    AC_SUBST(CUPS_API_MAJOR)
    AC_SUBST(CUPS_API_MINOR)
    AC_SUBST(CUPS_CFLAGS)
    AC_SUBST(CUPS_LIBS)

    AC_CHECK_HEADER(cups/cups.h,,AC_MSG_ERROR([[*** Sorry, cups-config present but cups/cups.h missing.]]))

    AM_CONDITIONAL(HAVE_CUPS, true)

    stlwrt_save_cflags="$CFLAGS"
    CFLAGS="$CUPS_CFLAGS"
    AC_TRY_COMPILE([#include <cups/http.h>],
                   [http_t http; char *s = http.authstring;],
                   [AC_DEFINE(HAVE_HTTP_AUTHSTRING, [],
                              [Define if cups http_t authstring field is accessible])],)
    CFLAGS="$stlwrt_save_cflags"

    AC_SUBST(HAVE_HTTP_AUTHSTRING)

    stlwrt_save_libs="$LIBS"
    LIBS="$CUPS_LIBS"
    AC_CHECK_FUNCS(httpGetAuthString)
    LIBS="$stlwrt_save_libs"
  fi
fi

stlwrt_save_cppflags="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $STLWRT_DEP_CFLAGS $STLWRT_DEP_CFLAGS"

AC_CHECK_HEADER(cairo-pdf.h,,AC_MSG_ERROR([
*** Can't find cairo-pdf.h. You must build Cairo with the pdf
*** backend enabled.]))

if test "$os_win32" != "yes"; then
  AC_CHECK_HEADER(cairo-ps.h,,AC_MSG_ERROR([
*** Can't find cairo-ps.h. You must build Cairo with the 
*** postscript backend enabled.]))

  AC_CHECK_HEADER(cairo-svg.h,,AC_MSG_ERROR([
*** Can't find cairo-svg.h. You must build Cairo with the
*** svg backend enabled.]))
fi   

CPPFLAGS="$gtk_save_cppflags"
     
			  	
AC_ARG_ENABLE(test-print-backend,
              [AC_HELP_STRING([--enable-test-print-backend],
                              [build test print backend])],,
              [enable_test_print_backend=no])
AM_CONDITIONAL(TEST_PRINT_BACKEND, test "x$enable_test_print_backend" != "xno")

if test "$os_win32" = "yes"; then
  AC_CHECK_TYPES([IPrintDialogCallback],[],[],[[#include <windows.h>]])
fi

################################################################
# Strip -export-dynamic from the link lines of various libraries
################################################################

#
# pkg-config --libs gmodule includes the "export_dynamic" flag,
#  but this flag is only meaningful for executables. For libraries
#  the effect is undefined; what it causes on Linux is that the
#  export list from -export-symbols-regex is ignored and everything
#  is exported
#
# We are using gmodule-no-export now, but I'm leaving the stripping
# code in place for now, since pango and atk still require gmodule.
export SED
export_dynamic=`(./libtool --config; echo eval echo \\$export_dynamic_flag_spec) | sh`
if test -n "$export_dynamic"; then
  STLWRT_DEP_LIBS=`echo $STLWRT_DEP_LIBS | sed -e "s/$export_dynamic//"`
  STLWRT_DEP_LIBS=`echo $STLWRT_DEP_LIBS | sed -e "s/$export_dynamic//"`
fi

##################################################
# GObject introspection
##################################################

GOBJECT_INTROSPECTION_CHECK([0.9.3])

##################################################
# Checks for gtk-doc and docbook-tools
##################################################

GTK_DOC_CHECK([1.11])

AC_CHECK_PROG(DB2HTML, db2html, true, false)
AM_CONDITIONAL(HAVE_DOCBOOK, $DB2HTML)

AC_ARG_ENABLE(man,
              [AC_HELP_STRING([--enable-man],
                              [regenerate man pages from Docbook [default=no]])],,
              enable_man=no)

if test "${enable_man}" != no; then
  dnl
  dnl Check for xsltproc
  dnl
  AC_PATH_PROG([XSLTPROC], [xsltproc])
  if test -z "$XSLTPROC"; then
    enable_man=no
  fi

  dnl check for DocBook DTD and stylesheets in the local catalog.
  JH_CHECK_XML_CATALOG([-//OASIS//DTD DocBook XML V4.1.2//EN],
     [DocBook XML DTD V4.1.2],,enable_man=no)
  JH_CHECK_XML_CATALOG([http://docbook.sourceforge.net/release/xsl/current/manpages/docbook.xsl],
     [DocBook XSL Stylesheets],,enable_man=no)
fi

AM_CONDITIONAL(ENABLE_MAN, test x$enable_man != xno)

##################################################
# Check whether MSVC toolset is explicitly set
##################################################

AM_CONDITIONAL(MSVC_BASE_NO_TOOLSET_SET, [test x$MSVC_BASE_TOOLSET = x])
AM_CONDITIONAL(MSVC_NO_TOOLSET_SET, [test x$MSVC_TOOLSET = x])

##################################################
# Output commands
##################################################

AC_CONFIG_COMMANDS([stlwrt/gdkconfig.h], [
	outfile=gdkconfig.h-tmp
	cat > $outfile <<\_______EOF
/* gdkconfig.h
 *
 * This is a generated file.  Please modify `configure.ac'
 */

#ifndef GDKCONFIG_H
#define GDKCONFIG_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#ifndef GSEAL
/* introduce GSEAL() here for all of STLWRT and STLWRT without the need to modify GLib */
#  ifdef GSEAL_ENABLE
#    define GSEAL(ident)      _g_sealed__ ## ident
#  else
#    define GSEAL(ident)      ident
#  endif
#endif /* !GSEAL */

_______EOF

	cat >>$outfile <<_______EOF
$stlwrt_windowing
$stlwrt_wc
_______EOF

	cat >>$outfile <<_______EOF

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* GDKCONFIG_H */
_______EOF


	if cmp -s $outfile stlwrt/gdkconfig.h; then
          AC_MSG_NOTICE([stlwrt/gdkconfig.h is unchanged])
	  rm -f $outfile
	else
	  mv $outfile stlwrt/gdkconfig.h
	fi
],[
if test "x$stlwrt_target" = "xx11" ; then
  stlwrt_windowing='
#define GDK_WINDOWING_X11'
elif test "x$stlwrt_target" = "xdirectfb" ; then
  stlwrt_windowing='
#define GDK_WINDOWING_DIRECTFB'
fi

if test x$stlwrt_wchar_h = xyes; then
  stlwrt_wc='
#define GDK_HAVE_WCHAR_H 1'
fi
if test x$stlwrt_wctype_h = xyes; then
  stlwrt_wc="\$stlwrt_wc
#define GDK_HAVE_WCTYPE_H 1"
fi
if test x$stlwrt_working_wctype = xno; then
  stlwrt_wc="\$stlwrt_wc
#define GDK_HAVE_BROKEN_WCTYPE 1"
fi


])

AC_CONFIG_FILES([
Makefile

utilities/Makefile.in
utilities/public/Makefile.in

po/Makefile.in

po-properties/Makefile.in

source/Makefile

modules/Makefile
modules/other/Makefile
modules/engines/Makefile
modules/engines/pixbuf/Makefile
modules/input/Makefile
modules/printbackends/Makefile
modules/printbackends/cups/Makefile
modules/printbackends/lpr/Makefile
modules/printbackends/file/Makefile
])

AC_OUTPUT

echo "STLWRT Configuration:
        Display backends: $stlwrt_target"
