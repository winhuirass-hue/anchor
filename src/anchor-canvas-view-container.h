/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-icon-container.h - the container widget for file manager icons

   Copyright (C) 2002 Sun Microsystems, Inc.

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

   Author: Michael Meeks <michael@ximian.com>
*/

#ifndef NAUTILUS_CANVAS_VIEW_CONTAINER_H
#define NAUTILUS_CANVAS_VIEW_CONTAINER_H

#include "anchor-canvas-view.h"

#include <libanchor-private/anchor-canvas-container.h>

typedef struct AnchorCanvasViewContainer AnchorCanvasViewContainer;
typedef struct AnchorCanvasViewContainerClass AnchorCanvasViewContainerClass;

#define ANCHOR_TYPE_CANVAS_VIEW_CONTAINER anchor_canvas_view_container_get_type()
#define NAUTILUS_CANVAS_VIEW_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_CANVAS_VIEW_CONTAINER, AnchorCanvasViewContainer))
#define NAUTILUS_CANVAS_VIEW_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_CANVAS_VIEW_CONTAINER, AnchorCanvasViewContainerClass))
#define NAUTILUS_IS_CANVAS_VIEW_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_CANVAS_VIEW_CONTAINER))
#define NAUTILUS_IS_CANVAS_VIEW_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_CANVAS_VIEW_CONTAINER))
#define NAUTILUS_CANVAS_VIEW_CONTAINER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_CANVAS_VIEW_CONTAINER, AnchorCanvasViewContainerClass))

typedef struct AnchorCanvasViewContainerDetails AnchorCanvasViewContainerDetails;

struct AnchorCanvasViewContainer {
	AnchorCanvasContainer parent;

	AnchorCanvasView *view;
	gboolean    sort_for_desktop;
};

struct AnchorCanvasViewContainerClass {
	AnchorCanvasContainerClass parent_class;
};

GType                  anchor_canvas_view_container_get_type         (void);
AnchorCanvasContainer *anchor_canvas_view_container_construct        (AnchorCanvasViewContainer *canvas_container,
								      AnchorCanvasView      *view);
AnchorCanvasContainer *anchor_canvas_view_container_new              (AnchorCanvasView      *view);
void                   anchor_canvas_view_container_set_sort_desktop (AnchorCanvasViewContainer *container,
								      gboolean         desktop);

#endif /* NAUTILUS_CANVAS_VIEW_CONTAINER_H */
