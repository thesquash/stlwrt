include $(top_srcdir)/Makefile.decl

if PLATFORM_WIN32
no_undefined = -no-undefined
endif

AM_CFLAGS = \
	-I$(top_srcdir) 				\
	-I$(top_srcdir)/stlwrt				\
	-I$(top_builddir)/stlwrt			\
	-I$(top_srcdir)/stlwrx				\
	-I$(top_builddir)/stlwrx			\
	$(CUPS_CFLAGS)					\
	-DSTLWRT_PRINT_BACKEND_ENABLE_UNSUPPORTED		\
	$(STLWRT_DEP_CFLAGS)				\
	$(STLWRT_DEBUG_FLAGS)

LDADDS = \
	$(top_builddir)/stlwrt/$(gtk2_target_lib)	\
	$(top_builddir)/stlwrx/$(gdk2_target_lib)	\
	$(STLWRT_DEP_LIBS)

backenddir = $(libdir)/gtk-2.0/$(GTK2_BINARY_VERSION)/printbackends

backend_LTLIBRARIES = libprintbackend-cups.la

libprintbackend_cups_la_SOURCES =	\
	gtkprintbackendcups.c		\
	gtkprintercups.c		\
	gtkcupsutils.c

noinst_HEADERS =			\
	gtkprintbackendcups.h		\
	gtkprintercups.h		\
	gtkcupsutils.h

libprintbackend_cups_la_LDFLAGS =  -avoid-version -module $(no_undefined)
libprintbackend_cups_la_LIBADD = $(LDADDS) $(CUPS_LIBS)

-include $(top_srcdir)/git.mk
