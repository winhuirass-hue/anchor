/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* Anchor - Canvas item for floating selection.
 *
 * Copyright (C) 1997, 1998, 1999, 2000 Free Software Foundation
 * Copyright (C) 2011 Red Hat Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Federico Mena <federico@nuclecu.unam.mx>
 *          Cosimo Cecchi <cosimoc@redhat.com>
 */

#ifndef __NAUTILUS_SELECTION_CANVAS_ITEM_H__
#define __NAUTILUS_SELECTION_CANVAS_ITEM_H__

#include <eel/eel-canvas.h>

G_BEGIN_DECLS

#define ANCHOR_TYPE_SELECTION_CANVAS_ITEM anchor_selection_canvas_item_get_type()
#define NAUTILUS_SELECTION_CANVAS_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_SELECTION_CANVAS_ITEM, AnchorSelectionCanvasItem))
#define NAUTILUS_SELECTION_CANVAS_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_SELECTION_CANVAS_ITEM, AnchorSelectionCanvasItemClass))
#define NAUTILUS_IS_SELECTION_CANVAS_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_SELECTION_CANVAS_ITEM))
#define NAUTILUS_IS_SELECTION_CANVAS_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_SELECTION_CANVAS_ITEM))
#define NAUTILUS_SELECTION_CANVAS_ITEM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_SELECTION_CANVAS_ITEM, AnchorSelectionCanvasItemClass))

typedef struct _NautilusSelectionCanvasItem AnchorSelectionCanvasItem;
typedef struct _NautilusSelectionCanvasItemClass AnchorSelectionCanvasItemClass;
typedef struct _NautilusSelectionCanvasItemDetails AnchorSelectionCanvasItemDetails;

struct _NautilusSelectionCanvasItem {
	EelCanvasItem item;
	AnchorSelectionCanvasItemDetails *priv;
	gpointer user_data;
};

struct _NautilusSelectionCanvasItemClass {
	EelCanvasItemClass parent_class;
};

/* GObject */
GType       anchor_selection_canvas_item_get_type                 (void);

void anchor_selection_canvas_item_fade_out (AnchorSelectionCanvasItem *self,
					      guint transition_time);

G_END_DECLS

#endif /* __NAUTILUS_SELECTION_CANVAS_ITEM_H__ */
