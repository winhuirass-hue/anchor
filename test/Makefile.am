NULL=

AM_CPPFLAGS =\
	-I$(top_srcdir) \
	$(BASE_CFLAGS) \
	$(COMMON_CFLAGS) \
	$(NAUTILUS_CFLAGS) \
	$(WARNING_CFLAGS) \
	-DVERSION="\"$(VERSION)\"" \
	-DNAUTILUS_DATADIR=\""$(datadir)/anchor"\" \
	-DGNOMELOCALEDIR=\""$(prefix)/${DATADIRNAME}/locale"\"	\
	$(NULL)

LDADD =\
	$(top_builddir)/libanchor-private/libanchor-private.la \
	$(BASE_LIBS) \
	$(COMMON_LIBS) \
	$(NAUTILUS_LIBS) \
	$(NULL)

noinst_PROGRAMS =\
	test-anchor-search-engine \
	test-anchor-directory-async \
	test-anchor-copy \
	test-eel-editable-label	\
	$(NULL)

test_anchor_copy_SOURCES = test-copy.c test.c

test_anchor_search_engine_SOURCES = test-anchor-search-engine.c 

test_anchor_directory_async_SOURCES = test-anchor-directory-async.c

EXTRA_DIST = \
	test.h \
	$(NULL)

-include $(top_srcdir)/git.mk
