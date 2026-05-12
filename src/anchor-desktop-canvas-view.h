/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-icon-view.h - interface for icon view of directory.

   Copyright (C) 2000 Eazel, Inc.

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

   Authors: Mike Engber <engber@eazel.com>
*/

#ifndef NAUTILUS_DESKTOP_CANVAS_VIEW_H
#define NAUTILUS_DESKTOP_CANVAS_VIEW_H

#include "anchor-canvas-view.h"

#define ANCHOR_TYPE_DESKTOP_CANVAS_VIEW anchor_desktop_canvas_view_get_type()
#define NAUTILUS_DESKTOP_CANVAS_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_DESKTOP_CANVAS_VIEW, AnchorDesktopCanvasView))
#define NAUTILUS_DESKTOP_CANVAS_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_DESKTOP_CANVAS_VIEW, AnchorDesktopCanvasViewClass))
#define NAUTILUS_IS_DESKTOP_CANVAS_VIEW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_DESKTOP_CANVAS_VIEW))
#define NAUTILUS_IS_DESKTOP_CANVAS_VIEW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_DESKTOP_CANVAS_VIEW))
#define NAUTILUS_DESKTOP_CANVAS_VIEW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_DESKTOP_CANVAS_VIEW, AnchorDesktopCanvasViewClass))

typedef struct AnchorDesktopCanvasViewDetails AnchorDesktopCanvasViewDetails;
typedef struct {
	AnchorCanvasView parent;
	AnchorDesktopCanvasViewDetails *details;
} AnchorDesktopCanvasView;

typedef struct {
	AnchorCanvasViewClass parent_class;
} AnchorDesktopCanvasViewClass;

/* GObject support */
GType   anchor_desktop_canvas_view_get_type (void);
AnchorView * anchor_desktop_canvas_view_new (AnchorWindowSlot *slot);

#endif /* NAUTILUS_DESKTOP_CANVAS_VIEW_H */
