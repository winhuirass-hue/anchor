/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* anchor-empty-view.h - interface for empty view of directory.

   Copyright (C) 2006 Free Software Foundation, Inc.
   
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

   Authors: Christian Neumair <chris@gnome-de.org>
*/

#ifndef NAUTILUS_EMPTY_VIEW_H
#define NAUTILUS_EMPTY_VIEW_H

#include "anchor-view.h"

#define ANCHOR_TYPE_EMPTY_VIEW anchor_empty_view_get_type()
#define NAUTILUS_EMPTY_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_EMPTY_VIEW, AnchorEmptyView))
#define NAUTILUS_EMPTY_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_EMPTY_VIEW, AnchorEmptyViewClass))
#define NAUTILUS_IS_EMPTY_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_EMPTY_VIEW))
#define NAUTILUS_IS_EMPTY_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_EMPTY_VIEW))
#define NAUTILUS_EMPTY_VIEW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_EMPTY_VIEW, AnchorEmptyViewClass))

typedef struct AnchorEmptyViewDetails AnchorEmptyViewDetails;

typedef struct {
	AnchorView parent_instance;
	AnchorEmptyViewDetails *details;
} AnchorEmptyView;

typedef struct {
	AnchorViewClass parent_class;
} AnchorEmptyViewClass;

GType anchor_empty_view_get_type (void);
AnchorView * anchor_empty_view_new (AnchorWindowSlot *slot);

#endif /* NAUTILUS_EMPTY_VIEW_H */
