/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-list-view.h - interface for list view of directory.

   Copyright (C) 2000 Eazel, Inc.
   Copyright (C) 2001 Anders Carlsson <andersca@gnu.org>
   
   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   see <http://www.gnu.org/licenses/>.

   Authors: John Sullivan <sullivan@eazel.com>
            Anders Carlsson <andersca@gnu.org>
*/

#ifndef NAUTILUS_LIST_VIEW_H
#define NAUTILUS_LIST_VIEW_H

#include "anchor-view.h"

#define ANCHOR_TYPE_LIST_VIEW anchor_list_view_get_type()
#define NAUTILUS_LIST_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_LIST_VIEW, AnchorListView))
#define NAUTILUS_LIST_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_LIST_VIEW, AnchorListViewClass))
#define NAUTILUS_IS_LIST_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_LIST_VIEW))
#define NAUTILUS_IS_LIST_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_LIST_VIEW))
#define NAUTILUS_LIST_VIEW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_LIST_VIEW, AnchorListViewClass))

typedef struct AnchorListViewDetails AnchorListViewDetails;

typedef struct {
	AnchorView parent_instance;
	AnchorListViewDetails *details;
} AnchorListView;

typedef struct {
	AnchorViewClass parent_class;
} AnchorListViewClass;

GType anchor_list_view_get_type (void);
AnchorView * anchor_list_view_new (AnchorWindowSlot *slot);
GtkTreeView* anchor_list_view_get_tree_view (AnchorListView *list_view);

#endif /* NAUTILUS_LIST_VIEW_H */
