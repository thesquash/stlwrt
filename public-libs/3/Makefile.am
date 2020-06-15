# Makefile.am for stlwrt/stlwrt/gtk3
# This Makefile is responsible for compiling the GTK+ 3-specific parts of STLWRT
# into the "libgtk-3" library.
include $(top_srcdir)/Makefile.decl


# STLWRT C sources to build the library from
stlwrt_gtk3_c_sources =		\
	gtkversion.c		\
	gtkfunction-declarations.c


libgtk3_libtool_opts =						\
  -version-info $(GTK3_LT_VERSION_INFO)				\
  -export-dynamic $(no_undefined) $(LIBTOOL_EXPORT_OPTIONS)	\
  -rpath $(libdir)



stlwrt_gtk3_includedir = $(includedir)/stlwrt
stlwrt_gtk3_include_HEADERS = gtkversion.h

libstlwrt_gtk3_la_SOURCES = $(stlwrt_gtk3_c_sources)


libgtk_3_la_LDFLAGS             = $(libgtk3_libtool_opts)
libgtk_3_la_LIBADD              = ../$(stlwrt_lib) $(stlwrt_gtk3_lib)
libgtk_3_la_DEPENDENCIES        = ../$(stlwrt_lib) $(stlwrt_gtk3_lib)

EXTRA_LTLIBRARIES = libgtk-3.la

noinst_LTLIBRARIES = libstlwrt-gtk3.la


# Install gtkversion.h into the gtk3 subdirectory.
install-exec-local:
	$(mkinstalldirs) $(DESTDIR)$(includedir)/stlwrt/gtk/gtk3
	file=$(DESTDIR)$(includedir)/stlwrt/gtk/gtk3/gtkversion.h; \
	if test -r $$file && cmp -s gtkversion.h $$file; then :; \
	else $(INSTALL_DATA) gtkversion.h $$file; fi


.PHONY: files test test-debug


-include $(top_srcdir)/git.mk
