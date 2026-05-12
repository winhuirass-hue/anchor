include $(top_srcdir)/Makefile.shared

AM_CPPFLAGS=\
	-I$(top_srcdir) \
	-I$(top_builddir) \
	$(BASE_CFLAGS) \
	$(WARNING_CFLAGS) \
	$(DISABLE_DEPRECATED) \
	-DDATADIR=\""$(datadir)"\" \
	$(NULL)

anchor_extensiondir = $(libdir)/anchor/extensions-3.0
anchor_extension_LTLIBRARIES = libanchor-sendto.la
libanchor_sendto_la_SOURCES =		\
	anchor-nste.c			\
	anchor-nste.h			\
	anchor-sendto-module.c

libanchor_sendto_la_LDFLAGS = -module -avoid-version -no-undefined
libanchor_sendto_la_LIBADD  = \
  $(top_builddir)/libanchor-extension/libanchor-extension.la \
  $(BASE_LIBS)

-include $(top_srcdir)/git.mk
