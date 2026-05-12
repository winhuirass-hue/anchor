/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Mr Jamie McCracken
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
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * see <http://www.gnu.org/licenses/>.
 *
 * Author: Jamie McCracken (jamiemcc@gnome.org)
 *
 */

#ifndef NAUTILUS_SEARCH_ENGINE_TRACKER_H
#define NAUTILUS_SEARCH_ENGINE_TRACKER_H

#include <libanchor-private/anchor-search-engine.h>

#define ANCHOR_TYPE_SEARCH_ENGINE_TRACKER		(anchor_search_engine_tracker_get_type ())
#define NAUTILUS_SEARCH_ENGINE_TRACKER(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_SEARCH_ENGINE_TRACKER, AnchorSearchEngineTracker))
#define NAUTILUS_SEARCH_ENGINE_TRACKER_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_SEARCH_ENGINE_TRACKER, AnchorSearchEngineTrackerClass))
#define NAUTILUS_IS_SEARCH_ENGINE_TRACKER(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_SEARCH_ENGINE_TRACKER))
#define NAUTILUS_IS_SEARCH_ENGINE_TRACKER_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_SEARCH_ENGINE_TRACKER))
#define NAUTILUS_SEARCH_ENGINE_TRACKER_GET_CLASS(obj)   (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_SEARCH_ENGINE_TRACKER, AnchorSearchEngineTrackerClass))

typedef struct AnchorSearchEngineTrackerDetails AnchorSearchEngineTrackerDetails;

typedef struct AnchorSearchEngineTracker {
	GObject parent;
	AnchorSearchEngineTrackerDetails *details;
} AnchorSearchEngineTracker;

typedef struct {
	GObjectClass parent_class;
} AnchorSearchEngineTrackerClass;

GType anchor_search_engine_tracker_get_type (void);

AnchorSearchEngineTracker* anchor_search_engine_tracker_new (void);

#endif /* NAUTILUS_SEARCH_ENGINE_TRACKER_H */
