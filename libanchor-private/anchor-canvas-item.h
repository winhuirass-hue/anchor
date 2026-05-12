/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* Anchor - Canvas item class for canvas container.
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Author: Andy Hertzfeld <andy@eazel.com>
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
 */

#ifndef NAUTILUS_CANVAS_ITEM_H
#define NAUTILUS_CANVAS_ITEM_H

#include <eel/eel-canvas.h>
#include <eel/eel-art-extensions.h>

G_BEGIN_DECLS

#define ANCHOR_TYPE_CANVAS_ITEM anchor_canvas_item_get_type()
#define NAUTILUS_CANVAS_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_CANVAS_ITEM, AnchorCanvasItem))
#define NAUTILUS_CANVAS_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_CANVAS_ITEM, AnchorCanvasItemClass))
#define NAUTILUS_IS_CANVAS_ITEM(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_CANVAS_ITEM))
#define NAUTILUS_IS_CANVAS_ITEM_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_CANVAS_ITEM))
#define NAUTILUS_CANVAS_ITEM_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_CANVAS_ITEM, AnchorCanvasItemClass))

typedef struct AnchorCanvasItem AnchorCanvasItem;
typedef struct AnchorCanvasItemClass AnchorCanvasItemClass;
typedef struct AnchorCanvasItemDetails AnchorCanvasItemDetails;

struct AnchorCanvasItem {
	EelCanvasItem item;
	AnchorCanvasItemDetails *details;
	gpointer user_data;
};

struct AnchorCanvasItemClass {
	EelCanvasItemClass parent_class;
};

/* not namespaced due to their length */
typedef enum {
	BOUNDS_USAGE_FOR_LAYOUT,
	BOUNDS_USAGE_FOR_ENTIRE_ITEM,
	BOUNDS_USAGE_FOR_DISPLAY
} AnchorCanvasItemBoundsUsage;

/* GObject */
GType       anchor_canvas_item_get_type                 (void);

/* attributes */
void        anchor_canvas_item_set_image                (AnchorCanvasItem       *item,
							   GdkPixbuf                *image);
cairo_surface_t* anchor_canvas_item_get_drag_surface    (AnchorCanvasItem       *item);
void        anchor_canvas_item_set_emblems              (AnchorCanvasItem       *item,
							   GList                    *emblem_pixbufs);
void        anchor_canvas_item_set_show_stretch_handles (AnchorCanvasItem       *item,
							   gboolean                  show_stretch_handles);
void        anchor_canvas_item_set_attach_points        (AnchorCanvasItem       *item,
							   GdkPoint                 *attach_points,
							   int                       n_attach_points);
void        anchor_canvas_item_set_embedded_text_rect   (AnchorCanvasItem       *item,
							   const GdkRectangle       *text_rect);
void        anchor_canvas_item_set_embedded_text        (AnchorCanvasItem       *item,
							   const char               *text);
double      anchor_canvas_item_get_max_text_width       (AnchorCanvasItem       *item);
const char *anchor_canvas_item_get_editable_text        (AnchorCanvasItem       *canvas_item);
void        anchor_canvas_item_set_renaming             (AnchorCanvasItem       *canvas_item,
							   gboolean                  state);

/* geometry and hit testing */
gboolean    anchor_canvas_item_hit_test_rectangle       (AnchorCanvasItem       *item,
							   EelIRect                  canvas_rect);
gboolean    anchor_canvas_item_hit_test_stretch_handles (AnchorCanvasItem       *item,
							   gdouble                   world_x,
							   gdouble                   world_y,
							   GtkCornerType            *corner);
void        anchor_canvas_item_invalidate_label         (AnchorCanvasItem       *item);
void        anchor_canvas_item_invalidate_label_size    (AnchorCanvasItem       *item);
EelDRect    anchor_canvas_item_get_icon_rectangle     (const AnchorCanvasItem *item);
EelDRect    anchor_canvas_item_get_text_rectangle       (AnchorCanvasItem       *item,
							   gboolean                  for_layout);
void        anchor_canvas_item_get_bounds_for_layout    (AnchorCanvasItem       *item,
							   double *x1, double *y1, double *x2, double *y2);
void        anchor_canvas_item_get_bounds_for_entire_item (AnchorCanvasItem       *item,
							     double *x1, double *y1, double *x2, double *y2);
void        anchor_canvas_item_update_bounds            (AnchorCanvasItem       *item,
							   double i2w_dx, double i2w_dy);
void        anchor_canvas_item_set_is_visible           (AnchorCanvasItem       *item,
							   gboolean                  visible);
/* whether the entire label text must be visible at all times */
void        anchor_canvas_item_set_entire_text          (AnchorCanvasItem       *canvas_item,
							   gboolean                  entire_text);

G_END_DECLS

#endif /* NAUTILUS_CANVAS_ITEM_H */
