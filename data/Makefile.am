NULL=

xml_in_files = anchor.xml.in
xml_files = $(xml_in_files:.xml.in=.xml)
@INTLTOOL_XML_RULE@

desktopdir       = $(datadir)/applications
desktop_DATA     = 					\
	$(desktop_in_files:.desktop.in=.desktop)	\
	anchor-classic.desktop
desktop_in_files = 					\
	anchor.desktop.in				\
	anchor-autorun-software.desktop.in		\
	anchor-connect-server.desktop.in

autostartdir     = $(sysconfdir)/xdg/autostart
autostart_in_files = anchor-autostart.desktop.in
autostart_DATA = $(autostart_in_files:.desktop.in=.desktop)

appdatadir = $(datadir)/appdata
appdata_in_files = anchor.appdata.xml.in
appdata_DATA = $(appdata_in_files:.xml.in=.xml)
@INTLTOOL_XML_RULE@

@INTLTOOL_DESKTOP_RULE@

mimedir = $(datadir)/mime/packages
mime_DATA = $(xml_files)

servicedir = $(datadir)/dbus-1/services
service_DATA = $(service_in_files:.service.in=.service)
service_in_files = 				\
	org.freedesktop.FileManager1.service.in	\
	org.freedesktop.Anchor.service.in		\
	org.freedesktop.Anchor.SearchProvider.service.in

org.freedesktop.Anchor.service: org.freedesktop.Anchor.service.in Makefile
	$(AM_V_GEN) sed -e "s|\@bindir\@|$(bindir)|" $< > $@

org.freedesktop.FileManager1.service: org.freedesktop.FileManager1.service.in Makefile
	$(AM_V_GEN) sed -e "s|\@bindir\@|$(bindir)|" $< > $@

org.freedesktop.Anchor.SearchProvider.service: org.freedesktop.Anchor.SearchProvider.service.in Makefile
	$(AM_V_GEN) sed -e "s|\@bindir\@|$(bindir)|" $< > $@

searchproviderdir = $(datadir)/gnome-shell/search-providers
searchprovider_DATA = anchor-search-provider.ini

anchordatadir = $(datadir)/anchor

anchordata_DATA = \
	anchor-extras.placeholder 	\
	anchor-suggested.placeholder 	\
	$(NULL)

EXTRA_DIST = $(anchordata_DATA) 	\
	anchor.desktop		\
	anchor.desktop.in		\
	dbus-interfaces.xml		\
	freedesktop-dbus-interfaces.xml	\
	shell-search-provider-dbus-interfaces.xml \
	$(xml_in_files) 		\
	$(desktop_in_files)		\
	$(service_in_files)		\
	$(autostart_in_files)		\
	$(searchprovider_DATA)		\
	$(appdata_in_files)		\
	$(NULL)

CLEANFILES = $(xml_files)	\
	$(desktop_DATA)		\
	$(service_DATA)		\
	$(autostart_DATA)	\
	$(appdata_DATA)		\
	$(NULL)

install-data-hook:
if ENABLE_UPDATE_MIMEDB
	$(UPDATE_MIME_DATABASE) "$(DESTDIR)$(datadir)/mime"
endif

uninstall-hook:
if ENABLE_UPDATE_MIMEDB
	$(UPDATE_MIME_DATABASE) "$(DESTDIR)$(datadir)/mime"
endif

-include $(top_srcdir)/git.mk
