# Makefile.am for stlwrt/themes
include $(top_srcdir)/Makefile.decl

if USE_QUARTZ
install-mac-key-theme:
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/Mac/gtk-2.0-key
	$(INSTALL_DATA) $(srcdir)/gtkrc.key.mac $(DESTDIR)$(datadir)/themes/Mac/gtk-2.0-key/gtkrc
uninstall-mac-key-theme:
	rm -f $(DESTDIR)$(datadir)/themes/Mac/gtk-2.0-key/gtkrc
else
install-mac-key-theme:
uninstall-mac-key-theme:
endif


# Install a RC file for the default GTK+ theme, and key themes
install-data-local: install-mac-key-theme
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/Default/gtk-2.0
	$(INSTALL_DATA) $(srcdir)/gtkrc.default $(DESTDIR)$(datadir)/themes/Default/gtk-2.0/gtkrc
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/Default/gtk-2.0-key
	$(INSTALL_DATA) $(srcdir)/gtkrc.key.default $(DESTDIR)$(datadir)/themes/Default/gtk-2.0-key/gtkrc
	$(mkinstalldirs) $(DESTDIR)$(datadir)/themes/Emacs/gtk-2.0-key
	$(INSTALL_DATA) $(srcdir)/gtkrc.key.emacs $(DESTDIR)$(datadir)/themes/Emacs/gtk-2.0-key/gtkrc

uninstall-local: uninstall-mac-key-theme
	rm -f $(DESTDIR)$(datadir)/themes/Raleigh/gtk-2.0/gtkrc
	rm -f $(DESTDIR)$(datadir)/themes/Default/gtk-2.0-key/gtkrc
	rm -f $(DESTDIR)$(datadir)/themes/Emacs/gtk-2.0-key/gtkrc

EXTRA_DIST +=                   \
	gtkrc.default		\
	gtkrc.key.default	\
	gtkrc.key.emacs		\
	gtkrc.key.mac


-include $(top_srcdir)/git.mk
