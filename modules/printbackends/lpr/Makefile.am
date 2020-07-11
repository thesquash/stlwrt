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
	-DSTLWRT_PRINT_BACKEND_ENABLE_UNSUPPORTED		\
	$(STLWRT_DEP_CFLAGS)				\
	$(STLWRT_DEBUG_FLAGS)

LDADDS = \
	$(STLWRT_DEP_LIBS)				\
	$(top_builddir)/stlwrt/$(gtk2_target_lib)		

backenddir = $(libdir)/gtk-2.0/$(GTK2_BINARY_VERSION)/printbackends

backend_LTLIBRARIES = libprintbackend-lpr.la

libprintbackend_lpr_la_SOURCES =	\
	gtkprintbackendlpr.c

noinst_HEADERS =			\
	gtkprintbackendlpr.h

libprintbackend_lpr_la_LDFLAGS =  -avoid-version -module $(no_undefined)
libprintbackend_lpr_la_LIBADD = $(LDADDS)

-include $(top_srcdir)/git.mk
