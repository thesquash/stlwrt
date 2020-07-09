include $(top_srcdir)/Makefile.decl

SUBDIRS = file lpr

if HAVE_CUPS
SUBDIRS += cups
endif

DIST_SUBDIRS = cups file lpr

-include $(top_srcdir)/git.mk
