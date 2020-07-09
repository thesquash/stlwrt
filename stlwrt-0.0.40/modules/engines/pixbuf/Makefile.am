include $(top_srcdir)/Makefile.decl

if PLATFORM_WIN32
no_undefined = -no-undefined
endif

AM_CFLAGS = \
	-I$(top_srcdir) 		\
	-I$(top_srcdir)/stlwrx		\
	-I$(top_builddir)/stlwrt	\
	-DGDK_DISABLE_DEPRECATED	\
	$(STLWRT_DEP_CFLAGS)

LDADDS = \
	$(STLWRT_DEP_LIBS)				\
	$(top_builddir)/stlwrx/$(stlwrx_target_lib)	\
	$(top_builddir)/stlwrt/$(stlwrt_target_lib)

enginedir = $(libdir)/gtk-2.0/$(GTK2_BINARY_VERSION)/engines

engine_LTLIBRARIES = libpixmap.la

libpixmap_la_SOURCES = 		\
	pixbuf-draw.c		\
	pixbuf-main.c		\
	pixbuf-render.c		\
	pixbuf-rc-style.c	\
	pixbuf-rc-style.h	\
	pixbuf-style.h		\
	pixbuf.h

libpixmap_la_LDFLAGS =  -avoid-version -module $(no_undefined)
libpixmap_la_LIBADD = $(LDADDS)


-include $(top_srcdir)/git.mk
