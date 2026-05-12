CLEANFILES = $(NULL)

include $(top_srcdir)/Makefile.shared

lib_LTLIBRARIES=libanchor-extension.la

AM_CPPFLAGS=\
	-I$(top_srcdir) \
	-I$(top_builddir) \
	$(BASE_CFLAGS) \
	$(WARNING_CFLAGS) \
	$(DISABLE_DEPRECATED) \
	-DDATADIR=\""$(datadir)"\" \
	$(NULL)

libanchor_extension_la_LDFLAGS=\
	-version-info @NAUTILUS_EXTENSION_VERSION_INFO@ \
	-no-undefined \
	$(NULL)

libanchor_extension_la_LIBADD = $(BASE_LIBS)

libanchor_extension_includedir=$(includedir)/anchor/libanchor-extension

libanchor_extension_include_HEADERS=		\
	anchor-column-provider.h              \
	anchor-column.h                       \
	anchor-extension-types.h              \
	anchor-file-info.h                    \
	anchor-info-provider.h                \
	anchor-location-widget-provider.h     \
	anchor-menu-item.h                    \
	anchor-menu-provider.h                \
	anchor-property-page-provider.h       \
	anchor-property-page.h                \
	anchor-menu.h                         \
	$(NULL)

libanchor_extension_la_SOURCES=			\
	anchor-column-provider.c                      \
	anchor-column.c                               \
	anchor-extension-i18n.h                       \
	anchor-extension-types.c                      \
	anchor-extension-private.h			\
	anchor-file-info.c                            \
	anchor-info-provider.c                        \
	anchor-location-widget-provider.c             \
	anchor-menu-item.c                            \
	anchor-menu-provider.c                        \
	anchor-property-page-provider.c               \
	anchor-property-page.c                        \
	anchor-menu.c                                 \
	$(NULL)

pkgconfigdir=$(libdir)/pkgconfig
pkgconfig_DATA=libanchor-extension.pc

EXTRA_DIST = \
	libanchor-extension.pc.in \
	$(NULL)

-include $(INTROSPECTION_MAKEFILE)
INTROSPECTION_GIRS =
INTROSPECTION_SCANNER_ARGS = --add-include-path=$(srcdir)
INTROSPECTION_COMPILER_ARGS = --includedir=$(srcdir)

if HAVE_INTROSPECTION
introspection_files =					\
	$(libanchor_extension_include_HEADERS)	\
	$(libanchor_extension_la_SOURCES)
Anchor-3.0.gir: libanchor-extension.la Makefile
Nautilus_3_0_gir_INCLUDES = Gtk-3.0 Gio-2.0 GLib-2.0
Nautilus_3_0_gir_CFLAGS = \
	-I$(top_srcdir) \
	-I$(top_builddir) \
	$(BASE_CFLAGS)
Nautilus_3_0_gir_LIBS = libanchor-extension.la
Nautilus_3_0_gir_FILES = $(addprefix $(srcdir)/, $(introspection_files))
INTROSPECTION_GIRS += Anchor-3.0.gir

girdir = $(datadir)/gir-1.0/
gir_DATA = $(INTROSPECTION_GIRS)

typelibsdir = $(libdir)/girepository-1.0/
typelibs_DATA = $(INTROSPECTION_GIRS:.gir=.typelib)

CLEANFILES += $(gir_DATA) $(typelibs_DATA)
endif

-include $(top_srcdir)/git.mk
