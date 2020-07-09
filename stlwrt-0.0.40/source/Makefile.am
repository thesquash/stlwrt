# Makefile.am for stlwrt/source
include $(top_srcdir)/Makefile.decl

if USE_QUARTZ
STLWRT_PRINT_PREVIEW_COMMAND="open -a /Applications/Preview.app %f"
else
STLWRT_PRINT_PREVIEW_COMMAND="evince --unlink-tempfile --preview --print-settings %s %f"
endif



if HAVE_CUPS
STLWRT_PRINT_BACKENDS=file,cups
else
STLWRT_PRINT_BACKENDS=file,lpr
endif

STLWRT_BASE_CFLAGS_DEFINES =				\
	-DG_LOG_DOMAIN=\"STLWRT\"			\
	-DGTK_COMPILATION				\
	-DGTK_DISABLE_DEPRECATED			\
	-DSTLWRT_FILE_SYSTEM_ENABLE_UNSUPPORTED		\
	-DSTLWRT_PRINT_BACKEND_ENABLE_UNSUPPORTED

STLWRT_PLAT_CFLAGS_DEFINES =				\
	-DSTLWRT_LIBDIR=\"$(libdir)\"			\
	-DSTLWRT_DATADIR=\"$(datadir)\"			\
	-DSTLWRT_DATA_PREFIX=\"$(prefix)\"		\
	-DSTLWRT_SYSCONFDIR=\"$(sysconfdir)\"		\
	-DSTLWRT_HOST=\"$(host)\"			\
	-DSTLWRT_PRINT_BACKENDS=\"$(STLWRT_PRINT_BACKENDS)\"\
	-DSTLWRT_PRINT_PREVIEW_COMMAND=\"$(STLWRT_PRINT_PREVIEW_COMMAND)\"

AM_CFLAGS =						\
	$(STLWRT_BASE_CFLAGS_DEFINES)			\
	$(STLWRT_PLAT_CFLAGS_DEFINES)			\
	-I$(top_builddir)				\
	-I$(top_builddir)/include/private/stlwrt	\
	-I$(top_builddir)/include/private/backends	\
	$(GMODULE_CFLAGS)				\
	$(STLWRT_DEBUG_FLAGS)				\
	$(STLWRT_DEP_CFLAGS)				\
	$(INCLUDED_IMMODULE_DEFINE)


libadd =		\
	$(GMODULE_LIBS) \
	$(STLWRT_DEP_LIBS)

# libtool stuff: set version and export symbols for resolving
# since automake doesn't support conditionalized libsomething_la_LDFLAGS
# we use the general approach here
stlwrt_libtool_opts =						\
  -version-info $(STLWRT_LT_VERSION_INFO)			\
  -export-dynamic $(no_undefined) $(LIBTOOL_EXPORT_OPTIONS)	\
  -rpath $(libdir)


included-modules:
if HAVE_INCLUDED_IMMMODULES
	@cd $(top_builddir)/modules/input && $(MAKE) $(AM_MAKEFLAGS) included-modules

libadd += $(INCLUDED_IMMODULE_OBJ)
deps = $(INCLUDED_IMMODULE_OBJ)

$(INCLUDED_IMMODULE_OBJ): included-modules
	@true
endif

.PHONY: included-modules

#
# setup source file variables
#

# STLWRT C sources to build the library from
stlwrt_c_sources =            \
	$(find . -maxdepth 1 -name '*.c' -print)


lib_LTLIBRARIES = $(stlwrt_lib)

libstlwrt_la_SOURCES = $(stlwrt_c_sources)
libstlwrt_la_LDFLAGS = $(stlwrt_libtool_opts)
libstlwrt_la_LIBADD = $(libadd)
libstlwrt_la_DEPENDENCIES = $(deps)


EXTRA_LTLIBRARIES = libstlwrt.la

install-exec-hook: 
if DISABLE_EXPLICIT_DEPS
	$(SHELL) $(top_srcdir)/sanitize-la.sh $(DESTDIR)$(libdir)/$(stlwrt_lib)
endif

LDADDS = $(STLWRT_DEP_LIBS)


.PHONY: files test test-debug

files:
	@files=`ls $(DISTFILES) 2> /dev/null `; for p in $$files; do \
	  echo $$p; \
	done

-include $(top_srcdir)/git.mk
