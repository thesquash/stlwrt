include $(top_srcdir)/Makefile.decl

if PLATFORM_WIN32
no_undefined = -no-undefined
endif

backenddir = $(libdir)/gtk-2.0/$(GTK2_BINARY_VERSION)/printbackends

backend_LTLIBRARIES = libprintbackend-file.la

libprintbackend_file_la_SOURCES = \
	gtkprintbackendfile.c

libprintbackend_file_la_CPPFLAGS = \
	-I$(top_srcdir) 				\
	-I$(top_srcdir)/stlwrt				\
	-I$(top_builddir)/stlwrt			\
	-I$(top_srcdir)/stlwrx				\
	-I$(top_builddir)/stlwrx			\
	-DSTLWRT_PRINT_BACKEND_ENABLE_UNSUPPORTED		\
	$(AM_CPPFLAGS)

libprintbackend_file_la_CFLAGS = \
	$(STLWRT_DEP_CFLAGS)				\
	$(AM_CFLAGS)

libprintbackend_file_la_LDFLAGS =  \
	-avoid-version -module $(no_undefined)

libprintbackend_file_la_LIBADD = \
	$(top_builddir)/stlwrt/$(gtk2_target_lib)	\
	$(top_builddir)/stlwrx/$(gdk2_target_lib)	\
	$(GTK_DEP_LIBS)

noinst_HEADERS =			\
	gtkprintbackendfile.h

-include $(top_srcdir)/git.mk
