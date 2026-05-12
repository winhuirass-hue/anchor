/*
 * anchor-previewer: anchor previewer DBus wrapper
 *
 * Copyright (C) 2011, Red Hat, Inc.
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
 * Author: Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#ifndef __NAUTILUS_PREVIEWER_H__
#define __NAUTILUS_PREVIEWER_H__

#include <glib-object.h>

G_BEGIN_DECLS

#define ANCHOR_TYPE_PREVIEWER anchor_previewer_get_type()
#define NAUTILUS_PREVIEWER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_PREVIEWER, AnchorPreviewer))
#define NAUTILUS_PREVIEWER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_PREVIEWER, AnchorPreviewerClass))
#define NAUTILUS_IS_PREVIEWER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_PREVIEWER))
#define NAUTILUS_IS_PREVIEWER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_PREVIEWER))
#define NAUTILUS_PREVIEWER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_PREVIEWER, AnchorPreviewerClass))

typedef struct _NautilusPreviewerPriv AnchorPreviewerPriv;

typedef struct {
  GObject parent;

  /* private */
  AnchorPreviewerPriv *priv;
} AnchorPreviewer;

typedef struct {
  GObjectClass parent_class;
} AnchorPreviewerClass;

GType anchor_previewer_get_type (void);

AnchorPreviewer *anchor_previewer_get_singleton (void);
void anchor_previewer_call_show_file (AnchorPreviewer *previewer,
                                        const gchar *uri,
                                        guint xid,
					gboolean close_if_already_visible);
void anchor_previewer_call_close (AnchorPreviewer *previewer);

G_END_DECLS

#endif /* __NAUTILUS_PREVIEWER_H__ */
