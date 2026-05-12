/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Anchor
 *
 * Copyright (C) 2002 Sun Microsystems, Inc.
 *
 * Anchor is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Anchor is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>.
 * 
 * Author: Dave Camp <dave@ximian.com>
 */

/* anchor-tree-view-drag-dest.h: Handles drag and drop for treeviews which 
 *                                 contain a hierarchy of files
 */

#ifndef NAUTILUS_TREE_VIEW_DRAG_DEST_H
#define NAUTILUS_TREE_VIEW_DRAG_DEST_H

#include <gtk/gtk.h>

#include "anchor-file.h"

G_BEGIN_DECLS

#define ANCHOR_TYPE_TREE_VIEW_DRAG_DEST	(anchor_tree_view_drag_dest_get_type ())
#define NAUTILUS_TREE_VIEW_DRAG_DEST(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_TREE_VIEW_DRAG_DEST, AnchorTreeViewDragDest))
#define NAUTILUS_TREE_VIEW_DRAG_DEST_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_TREE_VIEW_DRAG_DEST, AnchorTreeViewDragDestClass))
#define NAUTILUS_IS_TREE_VIEW_DRAG_DEST(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_TREE_VIEW_DRAG_DEST))
#define NAUTILUS_IS_TREE_VIEW_DRAG_DEST_CLASS(klass)	(G_TYPE_CLASS_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_TREE_VIEW_DRAG_DEST))

typedef struct _NautilusTreeViewDragDest        AnchorTreeViewDragDest;
typedef struct _NautilusTreeViewDragDestClass   AnchorTreeViewDragDestClass;
typedef struct _NautilusTreeViewDragDestDetails AnchorTreeViewDragDestDetails;

struct _NautilusTreeViewDragDest {
	GObject parent;
	
	AnchorTreeViewDragDestDetails *details;
};

struct _NautilusTreeViewDragDestClass {
	GObjectClass parent;
	
	char *(*get_root_uri) (AnchorTreeViewDragDest *dest);
	AnchorFile *(*get_file_for_path) (AnchorTreeViewDragDest *dest,
					    GtkTreePath *path);
	void (*move_copy_items) (AnchorTreeViewDragDest *dest,
				 const GList *item_uris,
				 const char *target_uri,
				 GdkDragAction action,
				 int x,
				 int y);
	void (* handle_netscape_url) (AnchorTreeViewDragDest *dest,
				 const char *url,
				 const char *target_uri,
				 GdkDragAction action,
				 int x,
				 int y);
	void (* handle_uri_list) (AnchorTreeViewDragDest *dest,
				  const char *uri_list,
				  const char *target_uri,
				  GdkDragAction action,
				  int x,
				  int y);
	void (* handle_text)    (AnchorTreeViewDragDest *dest,
				  const char *text,
				  const char *target_uri,
				  GdkDragAction action,
				  int x,
				  int y);
	void (* handle_raw)    (AnchorTreeViewDragDest *dest,
				  char *raw_data,
				  int length,
				  const char *target_uri,
				  const char *direct_save_uri,
				  GdkDragAction action,
				  int x,
				  int y);
	void (* handle_hover)   (AnchorTreeViewDragDest *dest,
				 const char *target_uri);
};

GType                     anchor_tree_view_drag_dest_get_type (void);
AnchorTreeViewDragDest *anchor_tree_view_drag_dest_new      (GtkTreeView *tree_view);

G_END_DECLS

#endif
