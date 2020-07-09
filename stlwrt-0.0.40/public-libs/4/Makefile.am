# Makefile.am for stlwrt/stlwrt/gtk4
# This Makefile is responsible for compiling the GTK 4-specific parts of STLWRT
# into the "libgtk-4" library.
include $(top_srcdir)/Makefile.decl


# STLWRT C sources to build the library from
stlwrt_gtk4_c_sources =		\
	gtkversion.c		\
	gtkfunction-declarations.c


libgtk4_libtool_opts =						\
  -version-info $(GTK4_LT_VERSION_INFO)				\
  -export-dynamic $(no_undefined) $(LIBTOOL_EXPORT_OPTIONS)	\
  -rpath $(libdir)



stlwrt_gtk4_includedir = $(includedir)/stlwrt
stlwrt_gtk4_include_HEADERS = gtkversion.h

libstlwrt_gtk4_la_SOURCES = $(stlwrt_gtk4_c_sources)


libgtk_4_la_LDFLAGS             = $(libgtk4_libtool_opts)
libgtk_4_la_LIBADD              = ../$(stlwrt_lib) $(stlwrt_gtk4_lib)
libgtk_4_la_DEPENDENCIES        = ../$(stlwrt_lib) $(stlwrt_gtk4_lib)

EXTRA_LTLIBRARIES = libgtk-4.la

noinst_LTLIBRARIES = libstlwrt-gtk4.la


# Install gtkversion.h into the gtk4 subdirectory.
install-exec-local:
	$(mkinstalldirs) $(DESTDIR)$(includedir)/stlwrt/gtk/gtk4
	file=$(DESTDIR)$(includedir)/stlwrt/gtk/gtk4/gtkversion.h; \
	if test -r $$file && cmp -s gtkversion.h $$file; then :; \
	else $(INSTALL_DATA) gtkversion.h $$file; fi


.PHONY: files test test-debug


-include $(top_srcdir)/git.mk
