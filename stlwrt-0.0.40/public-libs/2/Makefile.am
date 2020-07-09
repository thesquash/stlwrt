# Makefile.am for stlwrt/stlwrt/gtk2
# This Makefile is responsible for compiling the GTK+ 2-specific parts of STLWRT
# into the "libgtk-2" library.
include $(top_srcdir)/Makefile.decl


# STLWRT C sources to build the library from
stlwrt_gtk2_c_sources =		\
	gtkversion.c		\
	gtkfunction-declarations.c


libgtk2_libtool_opts =						\
  -version-info $(GTK2_LT_VERSION_INFO)				\
  -export-dynamic $(no_undefined) $(LIBTOOL_EXPORT_OPTIONS)	\
  -rpath $(libdir)


libgtk_x11_2_0_la_SOURCES             = $(stlwrt_gtk2_c_sources)
libgtk_directfb_2_0_la_SOURCES        = $(stlwrt_gtk2_c_sources)

libgtk_x11_2_0_la_LDFLAGS             = $(libgtk2_libtool_opts)
libgtk_quartz_2_0_la_LDFLAGS          = $(libgtk2_libtool_opts)

libgtk_x11_2_0_la_LIBADD              = ../$(stlwrt_lib)
libgtk_quartz_2_0_la_LIBADD           = ../$(stlwrt_lib)

libgtk_x11_2_0_la_DEPENDENCIES        = ../$(stlwrt_lib)
libgtk_quartz_2_0_la_DEPENDENCIES     = ../$(stlwrt_lib)

EXTRA_LTLIBRARIES = libgtk-x11-2.0.la  libgtk-directfb-2.0.la


# Install gtkversion.h into the gtk2 subdirectory.
install-exec-local:
	$(mkinstalldirs) $(DESTDIR)$(includedir)/stlwrt/gtk/gtk2
	file=$(DESTDIR)$(includedir)/stlwrt/gtk/gtk2/gtkversion.h; \
	if test -r $$file && cmp -s gtkversion.h $$file; then :; \
	else $(INSTALL_DATA) gtkversion.h $$file; fi


.PHONY: files test test-debug


if HAVE_INTROSPECTION
introspection_files = $(stlwrt_gtk2_c_sources)

Gtk-2.0.gir: $(INTROSPECTION_SCANNER) $(gtk2_target_lib) $(top_builddir)/stlwrx/gdk2/Gdk-2.0.gir Makefile
Gtk_2_0_gir_SCANNERFLAGS = --include-uninstalled=$(top_builddir)/stlwrx/gdk2/Gdk-2.0.gir
if USE_X11
Gtk_2_0_gir_SCANNERFLAGS += --add-include-path=$(top_builddir)/stlwrx/x11
endif
Gtk_2_0_gir_AM_CFLAGS = Atk-1.0
Gtk_2_0_gir_CFLAGS = \
		$(AM_CFLAGS) \
		-UGDK_DISABLE_DEPRECATED \
		-UGTK_DISABLE_DEPRECATED
Gtk_2_0_gir_LIBS = $(gtk2_target_lib)
Gtk_2_0_gir_FILES = $(introspection_files)
Gtk_2_0_gir_EXPORT_PACKAGES = gtk+-2.0
INTROSPECTION_GIRS += Gtk-2.0.gir

girdir = $(datadir)/gir-1.0
gir_DATA = $(INTROSPECTION_GIRS)

typelibsdir = $(libdir)/girepository-1.0
typelibs_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelibs_DATA)
endif


# pkg-config stuff...
STLWRX_TARGET=@stlwrx_target@

## Copy .pc files to target-specific names
gtk+-$(STLWRX_TARGET)-2.0.pc: gtk+-2.0.pc
	rm -f gtk+-$(STLWRX_TARGET)-2.0.pc && \
	cp gtk+-2.0.pc gtk+-$(STLWRX_TARGET)-2.0.pc

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA= gtk+-$(STLWRX_TARGET)-2.0.pc

if OS_UNIX
pkgconfig_DATA += gtk+-unix-print-2.0.pc
endif

DISTCLEANFILES =				\
	gtk+-unix-print-2.0.pc			\
	gtk+-$(STLWRX_TARGET)-2.0.pc

## copy the default target for this platform to gdk-2.0.pc and gtk+-2.0.pc
install-data-hook:
	(cd $(DESTDIR)$(pkgconfigdir) && \
	 test -f gtk+-$(STLWRX_TARGET)-2.0.pc && \
	 rm -f gtk+-2.0.pc && cp -f gtk+-$(STLWRX_TARGET)-2.0.pc gtk+-2.0.pc) || \
	(cd $(DESTDIR)$(pkgconfigdir) && \
	 rm -f gtk+-2.0.pc && cp -f gtk+-$(STLWRX_TARGET)-2.0.pc gtk+-2.0.pc)

uninstall-local:
	rm -f $(DESTDIR)$(pkgconfigdir)/gtk+-2.0.pc
	
# End of pkg-config stuff.


-include $(top_srcdir)/git.mk
