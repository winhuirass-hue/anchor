include $(top_srcdir)/Makefile.shared

noinst_LTLIBRARIES=libanchor-private.la

AM_CPPFLAGS = 						\
	-I$(top_srcdir) 				\
	-I$(top_builddir) 				\
	$(BASE_CFLAGS)					\
	$(COMMON_CFLAGS)				\
	$(NAUTILUS_CFLAGS)				\
	$(WARNING_CFLAGS)				\
	$(DISABLE_DEPRECATED)				\
	$(TRACKER_CFLAGS)				\
	-DDATADIR=\""$(datadir)"\" 			\
	-DSYSCONFDIR=\""$(sysconfdir)"\" 			\
	-DNAUTILUS_DATADIR=\""$(datadir)/anchor"\" 	\
	-DNAUTILUS_EXTENSIONDIR=\""$(libdir)/anchor/extensions-3.0"\" \
	$(NULL)

dbus_built_sources = anchor-generated.c anchor-generated.h
$(dbus_built_sources) : Makefile.am $(top_srcdir)/data/dbus-interfaces.xml
	gdbus-codegen									\
		--interface-prefix org.freedesktop.Anchor.					\
		--c-namespace AnchorDBus						\
		--c-generate-object-manager						\
		--generate-c-code anchor-generated					\
		$(top_srcdir)/data/dbus-interfaces.xml					\
		$(NULL)

BUILT_SOURCES = \
	$(dbus_built_sources) \
	$(NULL)

if ENABLE_TRACKER
anchor_tracker_engine_sources = \
	anchor-search-engine-tracker.c \
	anchor-search-engine-tracker.h
endif

libanchor_private_la_LDFLAGS =	\
	-no-undefined			\
	$(NULL)

libanchor_private_la_LIBADD =		\
	$(SELINUX_LIBS)			\
	$(TRACKER_LIBS)			\
	$(top_builddir)/eel/libeel-2.la \
	$(top_builddir)/libanchor-extension/libanchor-extension.la \
	$(BASE_LIBS)			\
	$(COMMON_LIBS)			\
	$(NAUTILUS_LIBS)		\
	-lm				\
	$(NULL)

libanchor_private_la_SOURCES = \
	anchor-bookmark.c \
	anchor-bookmark.h \
	anchor-canvas-container.c \
	anchor-canvas-container.h \
	anchor-canvas-dnd.c \
	anchor-canvas-dnd.h \
	anchor-canvas-item.c \
	anchor-canvas-item.h \
	anchor-canvas-private.h \
	anchor-clipboard-monitor.c \
	anchor-clipboard-monitor.h \
	anchor-clipboard.c \
	anchor-clipboard.h \
	anchor-column-chooser.c \
	anchor-column-chooser.h \
	anchor-column-utilities.c \
	anchor-column-utilities.h \
	anchor-dbus-manager.c \
	anchor-dbus-manager.h \
	anchor-debug.c \
	anchor-debug.h \
	anchor-default-file-icon.c \
	anchor-default-file-icon.h \
	anchor-desktop-directory-file.c \
	anchor-desktop-directory-file.h \
	anchor-desktop-directory.c \
	anchor-desktop-directory.h \
	anchor-desktop-icon-file.c \
	anchor-desktop-icon-file.h \
	anchor-desktop-link-monitor.c \
	anchor-desktop-link-monitor.h \
	anchor-desktop-link.c \
	anchor-desktop-link.h \
	anchor-desktop-metadata.c \
	anchor-desktop-metadata.h \
	anchor-directory-async.c \
	anchor-directory-notify.h \
	anchor-directory-private.h \
	anchor-directory.c \
	anchor-directory.h \
	anchor-dnd.c \
	anchor-dnd.h \
	anchor-entry.c \
	anchor-entry.h \
	anchor-file-attributes.h \
	anchor-file-changes-queue.c \
	anchor-file-changes-queue.h \
	anchor-file-conflict-dialog.c \
	anchor-file-conflict-dialog.h \
	anchor-file-dnd.c \
	anchor-file-dnd.h \
	anchor-file-operations.c \
	anchor-file-operations.h \
	anchor-file-private.h \
	anchor-file-queue.c \
	anchor-file-queue.h \
	anchor-file-utilities.c \
	anchor-file-utilities.h \
	anchor-file.c \
	anchor-file.h \
	anchor-generated.c \
	anchor-generated.h \
	anchor-global-preferences.c \
	anchor-global-preferences.h \
	anchor-icon-info.c \
	anchor-icon-info.h \
	anchor-icon-names.h \
	anchor-keyfile-metadata.c \
	anchor-keyfile-metadata.h \
	anchor-lib-self-check-functions.c \
	anchor-lib-self-check-functions.h \
	anchor-link.c \
	anchor-link.h \
	anchor-metadata.h \
	anchor-metadata.c \
	anchor-mime-application-chooser.c \
	anchor-mime-application-chooser.h \
	anchor-module.c \
	anchor-module.h \
	anchor-monitor.c \
	anchor-monitor.h \
	anchor-profile.c \
	anchor-profile.h \
	anchor-progress-info.c \
	anchor-progress-info.h \
	anchor-progress-info-manager.c \
	anchor-progress-info-manager.h \
	anchor-program-choosing.c \
	anchor-program-choosing.h \
	anchor-recent.c \
	anchor-recent.h \
	anchor-search-directory.c \
	anchor-search-directory.h \
	anchor-search-directory-file.c \
	anchor-search-directory-file.h \
	anchor-search-provider.c \
	anchor-search-provider.h \
	anchor-search-engine.c \
	anchor-search-engine.h \
	anchor-search-engine-model.c \
	anchor-search-engine-model.h \
	anchor-search-engine-simple.c \
	anchor-search-engine-simple.h \
	anchor-search-hit.c \
	anchor-search-hit.h \
	anchor-selection-canvas-item.c \
	anchor-selection-canvas-item.h \
	anchor-signaller.h \
	anchor-signaller.c \
	anchor-query.c \
	anchor-query.h \
	anchor-thumbnails.c \
	anchor-thumbnails.h \
	anchor-trash-monitor.c \
	anchor-trash-monitor.h \
	anchor-tree-view-drag-dest.c \
	anchor-tree-view-drag-dest.h \
	anchor-ui-utilities.c \
	anchor-ui-utilities.h \
	anchor-video-mime-types.h \
	anchor-vfs-directory.c \
	anchor-vfs-directory.h \
	anchor-vfs-file.c \
	anchor-vfs-file.h \
	anchor-file-undo-operations.c \
	anchor-file-undo-operations.h \
	anchor-file-undo-manager.c \
	anchor-file-undo-manager.h \
	$(anchor_tracker_engine_sources) \
	$(NULL)

nodist_libanchor_private_la_SOURCES =\
	$(BUILT_SOURCES) \
	$(NULL)

gsettingsschema_in_files = org.gnome.anchor.gschema.xml.in
gsettings_SCHEMAS = $(gsettingsschema_in_files:.xml.in=.xml)
.PRECIOUS: $(gsettings_SCHEMAS)

convertdir = $(datadir)/GConf/gsettings
convert_DATA = anchor.convert

@INTLTOOL_XML_NOMERGE_RULE@

@GSETTINGS_RULES@

EXTRA_DIST =				\
	anchor.convert		\
	$(gsettingsschema_in_files)     \
	$(TRACKER_SOURCES)		\
	$(NULL)

CLEANFILES = \
	$(BUILT_SOURCES)				\
	$(gsettings_SCHEMAS)				\
	$(NULL)

dist-hook:
	cd $(distdir); rm -f $(CLEANFILES)

-include $(top_srcdir)/git.mk
