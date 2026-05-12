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

/* anchor-bookmarks-window.h - interface for bookmark-editing window.
 */

#ifndef NAUTILUS_BOOKMARKS_WINDOW_H
#define NAUTILUS_BOOKMARKS_WINDOW_H

#include <gtk/gtk.h>

#include "anchor-window.h"
#include "anchor-bookmark-list.h"

#define ANCHOR_TYPE_BOOKMARKS_WINDOW anchor_bookmarks_window_get_type()
#define NAUTILUS_BOOKMARKS_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_BOOKMARKS_WINDOW, AnchorBookmarksWindow))
#define NAUTILUS_BOOKMARKS_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_BOOKMARKS_WINDOW, AnchorBookmarksWindowClass))
#define NAUTILUS_IS_BOOKMARKS_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_BOOKMARKS_WINDOW))
#define NAUTILUS_IS_BOOKMARKS_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_BOOKMARKS_WINDOW))
#define NAUTILUS_BOOKMARKS_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_BOOKMARKS_WINDOW, AnchorBookmarksWindowClass))

typedef struct AnchorBookmarksWindowPrivate AnchorBookmarksWindowPrivate;

typedef struct  {
	GtkWindow parent;

	AnchorBookmarksWindowPrivate *priv;
} AnchorBookmarksWindow;

typedef struct {
	GtkWindowClass parent_class;
} AnchorBookmarksWindowClass;

GType anchor_bookmarks_window_get_type (void);

GtkWindow *anchor_bookmarks_window_new (AnchorWindow       *parent_window);

#endif /* NAUTILUS_BOOKMARKS_WINDOW_H */
