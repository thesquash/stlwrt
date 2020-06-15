## Makefile.am for STLWRT
include $(top_srcdir)/Makefile.decl

SRC_SUBDIRS = source public-libs modules utilities
SUBDIRS = po po-properties themes $(SRC_SUBDIRS)

# require automake 1.7
AUTOMAKE_OPTIONS = 1.7
ACLOCAL_AMFLAGS = -I m4

EXTRA_DIST +=			\
	HACKING			\
	makecopyright		\
	README.commits		\
	sanitize-la.sh		\
	po/README.translators	\
	po/po2tbl.sed.in	\
MAINTAINERCLEANFILES = \
	$(srcdir)/INSTALL \
	$(srcdir)/README \
	$(srcdir)/aclocal.m4 \
	$(srcdir)/autoscan.log \
	$(srcdir)/compile \
	$(srcdir)/config.guess \
	$(srcdir)/config.h.in \
	$(srcdir)/config.sub \
	$(srcdir)/configure.scan \
	$(srcdir)/depcomp \
	$(srcdir)/install-sh \
	$(srcdir)/ltmain.sh \
	$(srcdir)/missing \
	$(srcdir)/mkinstalldirs \
	$(srcdir)/omf.make \
	$(srcdir)/xmldocs.make \
	$(srcdir)/gtk-doc.make \
	$(srcdir)/ChangeLog \
	`find "$(srcdir)" -type f -name Makefile.in -print`


DISTCLEANFILES = config.lt

distclean-local:
	if test "$(srcdir)" = "."; then :; else \
		rm -f ChangeLog; \
	fi

dist-hook:
	if test -f $(srcdir)/INSTALL.in && test -f $(srcdir)/README.in ; then \
	     CONFIG_FILES="INSTALL:$(srcdir)/INSTALL.in README:$(srcdir)/README.in" \
	     CONFIG_HEADERS= \
	     $(SHELL) config.status \
	     && cp INSTALL README $(distdir) ; \
	fi

DISTCHECK_CONFIGURE_FLAGS = --enable-gtk-doc --enable-man --disable-rebuilds --enable-introspection

GITIGNOREFILES = \
	po-properties/Makefile.in.in \
	po-properties/Makefile.in \
	po-properties/Makefile \
	po-properties/*.gmo \
	po-properties/*.mo \
	po-properties/POTFILES \
	po-properties/stamp-it \
	po-properties/.intltool-merge-cache

-include $(top_srcdir)/git.mk
