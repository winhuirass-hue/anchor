/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* anchor-bookmark.h - implementation of individual bookmarks.
 *
 * Copyright (C) 1999, 2000 Eazel, Inc.
 * Copyright (C) 2011, Red Hat, Inc.
 *
 * The Gnome Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * The Gnome Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the Gnome Library; see the file COPYING.LIB.  If not,
 * see <http://www.gnu.org/licenses/>.
 *
 * Authors: John Sullivan <sullivan@eazel.com>
 *          Cosimo Cecchi <cosimoc@redhat.com>
 */

#ifndef NAUTILUS_BOOKMARK_H
#define NAUTILUS_BOOKMARK_H

#include <gtk/gtk.h>
#include <gio/gio.h>
typedef struct AnchorBookmark AnchorBookmark;

#define ANCHOR_TYPE_BOOKMARK anchor_bookmark_get_type()
#define NAUTILUS_BOOKMARK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_BOOKMARK, AnchorBookmark))
#define NAUTILUS_BOOKMARK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_BOOKMARK, AnchorBookmarkClass))
#define NAUTILUS_IS_BOOKMARK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_BOOKMARK))
#define NAUTILUS_IS_BOOKMARK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_BOOKMARK))
#define NAUTILUS_BOOKMARK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_BOOKMARK, AnchorBookmarkClass))

typedef struct AnchorBookmarkDetails AnchorBookmarkDetails;

struct AnchorBookmark {
	GObject object;
	AnchorBookmarkDetails *details;	
};

struct AnchorBookmarkClass {
	GObjectClass parent_class;

	/* Signals that clients can connect to. */

	/* The contents-changed signal is emitted when the bookmark's contents
	 * (custom name or URI) changed.
	 */
	void	(* contents_changed) (AnchorBookmark *bookmark);
};

typedef struct AnchorBookmarkClass AnchorBookmarkClass;

GType                 anchor_bookmark_get_type               (void);
AnchorBookmark *    anchor_bookmark_new                    (GFile *location,
                                                                const char *custom_name);
const char *          anchor_bookmark_get_name               (AnchorBookmark      *bookmark);
GFile *               anchor_bookmark_get_location           (AnchorBookmark      *bookmark);
char *                anchor_bookmark_get_uri                (AnchorBookmark      *bookmark);
GIcon *               anchor_bookmark_get_icon               (AnchorBookmark      *bookmark);
GIcon *               anchor_bookmark_get_symbolic_icon      (AnchorBookmark      *bookmark);
gboolean              anchor_bookmark_get_exists             (AnchorBookmark      *bookmark);
gboolean              anchor_bookmark_get_xdg_type           (AnchorBookmark      *bookmark,
								GUserDirectory        *directory);
gboolean              anchor_bookmark_get_is_builtin         (AnchorBookmark      *bookmark);
gboolean	      anchor_bookmark_get_has_custom_name    (AnchorBookmark      *bookmark);
void                  anchor_bookmark_set_custom_name        (AnchorBookmark      *bookmark,
								const char            *new_name);
int                   anchor_bookmark_compare_with           (gconstpointer          a,
								gconstpointer          b);

void                  anchor_bookmark_set_scroll_pos         (AnchorBookmark      *bookmark,
								const char            *uri);
char *                anchor_bookmark_get_scroll_pos         (AnchorBookmark      *bookmark);


/* Helper functions for displaying bookmarks */
GtkWidget *           anchor_bookmark_menu_item_new          (AnchorBookmark      *bookmark);

#endif /* NAUTILUS_BOOKMARK_H */
