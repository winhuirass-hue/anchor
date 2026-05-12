/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* anchor-canvas-view.h - interface for canvas view of directory.
 *
 * Copyright (C) 2000 Eazel, Inc.
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
 *
 */

#ifndef NAUTILUS_CANVAS_VIEW_H
#define NAUTILUS_CANVAS_VIEW_H

#include "anchor-view.h"
#include "libanchor-private/anchor-canvas-container.h"

typedef struct AnchorCanvasView AnchorCanvasView;
typedef struct AnchorCanvasViewClass AnchorCanvasViewClass;

#define ANCHOR_TYPE_CANVAS_VIEW anchor_canvas_view_get_type()
#define NAUTILUS_CANVAS_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_CANVAS_VIEW, AnchorCanvasView))
#define NAUTILUS_CANVAS_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_CANVAS_VIEW, AnchorCanvasViewClass))
#define NAUTILUS_IS_CANVAS_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_CANVAS_VIEW))
#define NAUTILUS_IS_CANVAS_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_CANVAS_VIEW))
#define NAUTILUS_CANVAS_VIEW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_CANVAS_VIEW, AnchorCanvasViewClass))

typedef struct AnchorCanvasViewDetails AnchorCanvasViewDetails;

struct AnchorCanvasView {
	AnchorView parent;
	AnchorCanvasViewDetails *details;
};

struct AnchorCanvasViewClass {
	AnchorViewClass parent_class;
};

/* GObject support */
GType   anchor_canvas_view_get_type      (void);
int     anchor_canvas_view_compare_files (AnchorCanvasView   *canvas_view,
					  AnchorFile *a,
					  AnchorFile *b);
void    anchor_canvas_view_filter_by_screen (AnchorCanvasView *canvas_view,
					     gboolean filter);
void    anchor_canvas_view_clean_up_by_name (AnchorCanvasView *canvas_view);

AnchorView * anchor_canvas_view_new (AnchorWindowSlot *slot);

AnchorCanvasContainer * anchor_canvas_view_get_canvas_container (AnchorCanvasView *view);

#endif /* NAUTILUS_CANVAS_VIEW_H */
