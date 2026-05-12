/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Anchor
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

#ifndef __NAUTILUS_TOOLBAR_H__
#define __NAUTILUS_TOOLBAR_H__

#include <gtk/gtk.h>

#include "anchor-window.h"

#define ANCHOR_TYPE_TOOLBAR anchor_toolbar_get_type()
#define NAUTILUS_TOOLBAR(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_TOOLBAR, AnchorToolbar))
#define NAUTILUS_TOOLBAR_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_TOOLBAR, AnchorToolbarClass))
#define NAUTILUS_IS_TOOLBAR(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_TOOLBAR))
#define NAUTILUS_IS_TOOLBAR_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_TOOLBAR))
#define NAUTILUS_TOOLBAR_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_TOOLBAR, AnchorToolbarClass))

typedef struct _NautilusToolbar AnchorToolbar;
typedef struct _NautilusToolbarPriv AnchorToolbarPriv;
typedef struct _NautilusToolbarClass AnchorToolbarClass;

typedef enum {
	NAUTILUS_TOOLBAR_MODE_PATH_BAR,
	NAUTILUS_TOOLBAR_MODE_LOCATION_ENTRY,
} AnchorToolbarMode;

struct _NautilusToolbar {
	GtkHeaderBar parent;

	/* private */
	AnchorToolbarPriv *priv;
};

struct _NautilusToolbarClass {
	GtkHeaderBarClass parent_class;
};

GType anchor_toolbar_get_type (void);

GtkWidget *anchor_toolbar_new (AnchorWindow *window);

GtkWidget *anchor_toolbar_get_path_bar (AnchorToolbar *self);
GtkWidget *anchor_toolbar_get_location_entry (AnchorToolbar *self);

void anchor_toolbar_set_show_main_bar (AnchorToolbar *self,
					 gboolean show_main_bar);
void anchor_toolbar_set_show_location_entry (AnchorToolbar *self,
					       gboolean show_location_entry);

#endif /* __NAUTILUS_TOOLBAR_H__ */
