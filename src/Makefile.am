include $(top_srcdir)/Makefile.shared

bin_PROGRAMS=					\
	anchor				\
	anchor-autorun-software		\
	anchor-connect-server			\
	$(NULL)

libexec_PROGRAMS=				\
	anchor-convert-metadata		\
	$(NULL)

AM_CPPFLAGS =							\
	-I$(top_srcdir) 					\
	-I$(top_builddir)/libanchor-private 			\
	$(BASE_CFLAGS) 						\
	$(COMMON_CFLAGS)					\
	$(NAUTILUS_CFLAGS)					\
	$(WARNING_CFLAGS)					\
	$(EXIF_CFLAGS)						\
	$(EXEMPI_CFLAGS)                                        \
	-DDATADIR=\""$(datadir)"\" 				\
	-DLIBDIR=\""$(libdir)"\" 				\
	-DLIBEXECDIR=\""$(libexecdir)"\" 			\
	-DNAUTILUS_DATADIR=\""$(datadir)/anchor"\" 		\
	-DUIDIR=\""$(datadir)/anchor/ui"\" 		\
	-DNAUTILUS_PIXMAPDIR=\""$(datadir)/pixmaps/anchor"\" 	\
	-DPREFIX=\""$(prefix)"\" 				\
	-DSYSCONFDIR=\""$(sysconfdir)"\" 			\
	-DVERSION="\"$(VERSION)\"" 				\
	$(DISABLE_DEPRECATED) 		                        \
	$(NULL)

LDADD =\
	$(top_builddir)/libanchor-private/libanchor-private.la  \
	$(BASE_LIBS) \
	$(COMMON_LIBS) \
	$(NAUTILUS_LIBS) \
	$(CORE_LIBS) \
	$(EXIF_LIBS) \
	$(EXEMPI_LIBS) \
	$(POPT_LIBS) \
	$(NULL)

dbus_freedesktop_built_sources =			\
	anchor-freedesktop-generated.c		\
	anchor-freedesktop-generated.h

$(dbus_freedesktop_built_sources) : Makefile.am $(top_srcdir)/data/freedesktop-dbus-interfaces.xml
	gdbus-codegen									\
		--interface-prefix org.freedesktop.					\
		--c-namespace AnchorFreedesktop					\
		--c-generate-object-manager						\
		--generate-c-code anchor-freedesktop-generated			\
		$(top_srcdir)/data/freedesktop-dbus-interfaces.xml			\
		$(NULL)

dbus_shell_search_provider_built_sources =		\
	anchor-shell-search-provider-generated.c	\
	anchor-shell-search-provider-generated.h

$(dbus_shell_search_provider_built_sources) : Makefile.am $(top_srcdir)/data/shell-search-provider-dbus-interfaces.xml
	gdbus-codegen									\
		--interface-prefix org.gnome.						\
		--c-namespace Anchor							\
		--generate-c-code anchor-shell-search-provider-generated		\
		$(top_srcdir)/data/shell-search-provider-dbus-interfaces.xml		\
		$(NULL)

headers =					\
	anchor-special-location-bar.h		\
	$(NULL)

resource_files = $(shell glib-compile-resources --sourcedir=$(srcdir) --generate-dependencies $(srcdir)/anchor.gresource.xml)
anchor-resources.c: anchor.gresource.xml $(resource_files)
	glib-compile-resources --target=$@ --sourcedir=$(srcdir) --generate-source --c-name anchor $(srcdir)/anchor.gresource.xml
anchor-resources.h: anchor.gresource.xml $(resource_files)
	glib-compile-resources --target=$@ --sourcedir=$(srcdir) --generate-header --c-name anchor $(srcdir)/anchor.gresource.xml

anchor-enum-types.h: stamp-anchor-enum-types.h
	$(AM_V_GEN)true

stamp-anchor-enum-types.h: $(headers) Makefile
	$(AM_V_GEN)(cd $(srcdir) \
	&& glib-mkenums \
			--fhead "#ifndef __NAUTILUS_ENUM_TYPES_H__\n" \
			--fhead "#define __NAUTILUS_ENUM_TYPES_H__ 1\n\n" \
			--fhead "#include <glib-object.h>\n\n" \
			--fhead "G_BEGIN_DECLS\n\n" \
			--ftail "G_END_DECLS\n\n" \
			--ftail "#endif /* __NAUTILUS_ENUM_TYPES_H__ */\n" \
			--fprod "#include \"@filename@\"\n" \
			--eprod "#define @ENUMPREFIX@_TYPE_@ENUMSHORT@ @enum_name@_get_type()\n" \
			--eprod "GType @enum_name@_get_type (void);\n" \
		$(headers) ) > xgen-gth \
	&& (cmp -s xgen-gth anchor-enum-type.h || cp xgen-gth anchor-enum-types.h) \
	&& rm -f xgen-gth \
	&& echo timestamp > $(@F)

anchor-enum-types.c: $(headers) Makefile
	$(AM_V_GEN)(cd $(srcdir) \
	&& glib-mkenums \
			--fhead "#include <config.h>\n" \
			--fhead "#include <glib-object.h>\n" \
			--fhead "#include \"anchor-enum-types.h\"\n\n" \
			--fprod "\n/* enumerations from \"@filename@\" */" \
			--vhead "static const G@Type@Value _@enum_name@_values[] = {" \
			--vprod "  { @VALUENAME@, \"@VALUENAME@\", \"@valuenick@\" }," \
			--vtail "  { 0, NULL, NULL }\n};\n\n" \
			--vtail "GType\n@enum_name@_get_type (void)\n{\n" \
			--vtail "  static GType type = 0;\n\n" \
			--vtail "  if (!type)\n" \
			--vtail "    type = g_@type@_register_static (\"@EnumName@\", _@enum_name@_values);\n\n" \
			--vtail "  return type;\n}\n\n" \
		$(headers) ) > xgen-gtc \
	&& cp xgen-gtc $(@F) \
	&& rm -f xgen-gtc

anchor_built_sources = \
	$(dbus_freedesktop_built_sources) \
	$(dbus_shell_search_provider_built_sources) \
	anchor-resources.c \
	anchor-resources.h \
	anchor-enum-types.h \
	anchor-enum-types.c \
	$(NULL)

anchor_SOURCES = \
	anchor-actions.h			\
	anchor-application.c			\
	anchor-application.h			\
	anchor-bookmark-list.c		\
	anchor-bookmark-list.h		\
	anchor-bookmarks-window.c		\
	anchor-bookmarks-window.h		\
	anchor-canvas-view.c			\
	anchor-canvas-view.h			\
	anchor-canvas-view-container.c	\
	anchor-canvas-view-container.h	\
	anchor-connect-server-dialog.c	\
	anchor-connect-server-dialog.h	\
	anchor-desktop-canvas-view.c		\
	anchor-desktop-canvas-view.h		\
	anchor-desktop-item-properties.c	\
	anchor-desktop-item-properties.h	\
	anchor-desktop-window.c		\
	anchor-desktop-window.h		\
	anchor-error-reporting.c		\
	anchor-error-reporting.h		\
	anchor-file-management-properties.c	\
	anchor-file-management-properties.h	\
	anchor-floating-bar.c			\
	anchor-floating-bar.h			\
	anchor-freedesktop-dbus.c		\
	anchor-freedesktop-dbus.h		\
	anchor-image-properties-page.c	\
	anchor-image-properties-page.h	\
	anchor-list-model.c			\
	anchor-list-model.h			\
	anchor-list-view-private.h 		\
	anchor-list-view.c			\
	anchor-list-view.h 			\
	anchor-location-entry.c               \
	anchor-location-entry.h               \
	anchor-main.c 			\
	anchor-mime-actions.c 		\
	anchor-mime-actions.h 		\
	anchor-notebook.c			\
	anchor-notebook.h			\
	anchor-pathbar.c			\
	anchor-pathbar.h			\
	anchor-previewer.c			\
	anchor-previewer.h			\
	anchor-progress-info-widget.c		\
	anchor-progress-info-widget.h		\
	anchor-progress-ui-handler.c		\
	anchor-progress-ui-handler.h		\
	anchor-properties-window.c		\
	anchor-properties-window.h		\
	anchor-query-editor.c			\
	anchor-query-editor.h			\
	anchor-self-check-functions.c 	\
	anchor-self-check-functions.h 	\
	anchor-shell-search-provider.h	\
	anchor-shell-search-provider.c	\
	anchor-special-location-bar.c		\
	anchor-special-location-bar.h		\
	anchor-toolbar.c			\
	anchor-toolbar.h			\
	anchor-trash-bar.c			\
	anchor-trash-bar.h			\
	anchor-view.c				\
	anchor-view.h				\
	anchor-view-dnd.c			\
	anchor-view-dnd.h			\
	anchor-window-menus.c         	\
	anchor-window-private.h		\
	anchor-window-slot.c			\
	anchor-window-slot.h			\
	anchor-window-slot-dnd.c		\
	anchor-window-slot-dnd.h		\
	anchor-window.c			\
	anchor-window.h			\
	anchor-x-content-bar.c		\
	anchor-x-content-bar.h		\
	$(NULL)

nodist_anchor_SOURCES = \
	$(anchor_built_sources) \
	$(NULL)

EMPTY_VIEW_SOURCES = \
	anchor-empty-view.c 			\
	anchor-empty-view.h

if ENABLE_EMPTY_VIEW
anchor_SOURCES += $(EMPTY_VIEW_SOURCES)
endif

anchor_autorun_software_SOURCES= 			\
	anchor-autorun-software.c			\
	$(NULL)

anchor_connect_server_SOURCES= \
	anchor-connect-server-dialog.c        \
	anchor-connect-server-dialog.h        \
	anchor-connect-server.c		\
	$(NULL)

anchor_convert_metadata_SOURCES= \
	anchor-convert-metadata.c	\
	$(NULL)

BUILT_SOURCES = 					\
	$(anchor_built_sources) 			\
	$(dbus_shell_search_provider_built_sources) 	\
	$(NULL)

TESTS=check-anchor

@INTLTOOL_SERVER_RULE@

CLEANFILES = \
	$(BUILT_SOURCES)			\
	$(server_DATA)				\
	stamp-anchor-enum-types.h		\
	$(NULL)

EXTRA_DIST =					\
	$(resource_files)			\
	$(server_in_files)			\
	anchor.gresource.xml			\
	check-anchor				\
	$(NULL)

dist-hook:
	cd $(distdir); rm -f $(CLEANFILES)

-include $(top_srcdir)/git.mk
