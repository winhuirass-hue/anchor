/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Anchor
 *
 * Copyright (C) 1999, 2000 Eazel, Inc.
 *
 * Anchor is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Anchor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: John Sullivan <sullivan@eazel.com>
 */

/* anchor-bookmark-list.h - interface for centralized list of bookmarks.
 */

#ifndef NAUTILUS_BOOKMARK_LIST_H
#define NAUTILUS_BOOKMARK_LIST_H

#include <libanchor-private/anchor-bookmark.h>
#include <gio/gio.h>

typedef struct AnchorBookmarkList AnchorBookmarkList;
typedef struct AnchorBookmarkListClass AnchorBookmarkListClass;

#define ANCHOR_TYPE_BOOKMARK_LIST anchor_bookmark_list_get_type()
#define NAUTILUS_BOOKMARK_LIST(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_BOOKMARK_LIST, AnchorBookmarkList))
#define NAUTILUS_BOOKMARK_LIST_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_BOOKMARK_LIST, AnchorBookmarkListClass))
#define NAUTILUS_IS_BOOKMARK_LIST(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_BOOKMARK_LIST))
#define NAUTILUS_IS_BOOKMARK_LIST_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_BOOKMARK_LIST))
#define NAUTILUS_BOOKMARK_LIST_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_BOOKMARK_LIST, AnchorBookmarkListClass))

struct AnchorBookmarkList {
	GObject object;

	GList *list; 
	GFileMonitor *monitor;
	GQueue *pending_ops;
};

struct AnchorBookmarkListClass {
	GObjectClass parent_class;
	void (* changed) (AnchorBookmarkList *bookmarks);
};

GType                   anchor_bookmark_list_get_type            (void);
AnchorBookmarkList *  anchor_bookmark_list_new                 (void);
void                    anchor_bookmark_list_append              (AnchorBookmarkList   *bookmarks,
								    AnchorBookmark *bookmark);
void                    anchor_bookmark_list_delete_item_at      (AnchorBookmarkList   *bookmarks,
								    guint                   index);
void                    anchor_bookmark_list_insert_item         (AnchorBookmarkList   *bookmarks,
								    AnchorBookmark *bookmark,
								    guint                   index);
guint                   anchor_bookmark_list_length              (AnchorBookmarkList   *bookmarks);
AnchorBookmark *      anchor_bookmark_list_item_at             (AnchorBookmarkList   *bookmarks,
								    guint                   index);
AnchorBookmark *      anchor_bookmark_list_item_with_location  (AnchorBookmarkList *bookmarks,
								    GFile                *location,
								    guint                *index);
void                    anchor_bookmark_list_move_item           (AnchorBookmarkList *bookmarks,
								    guint                 index,
								    guint                 destination);
gboolean                anchor_bookmark_list_can_bookmark_location (AnchorBookmarkList *list,
								      GFile                *location);

#endif /* NAUTILUS_BOOKMARK_LIST_H */
