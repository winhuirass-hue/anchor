/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Novell, Inc.
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
 * Author: Anders Carlsson <andersca@imendio.com>
 *
 */

#ifndef NAUTILUS_SEARCH_ENGINE_H
#define NAUTILUS_SEARCH_ENGINE_H

#include <glib-object.h>

#include <libanchor-private/anchor-directory.h>
#include <libanchor-private/anchor-search-engine-model.h>
#include <libanchor-private/anchor-search-engine-simple.h>

#define ANCHOR_TYPE_SEARCH_ENGINE		(anchor_search_engine_get_type ())
#define NAUTILUS_SEARCH_ENGINE(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_SEARCH_ENGINE, AnchorSearchEngine))
#define NAUTILUS_SEARCH_ENGINE_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_SEARCH_ENGINE, AnchorSearchEngineClass))
#define NAUTILUS_IS_SEARCH_ENGINE(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_SEARCH_ENGINE))
#define NAUTILUS_IS_SEARCH_ENGINE_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_SEARCH_ENGINE))
#define NAUTILUS_SEARCH_ENGINE_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_SEARCH_ENGINE, AnchorSearchEngineClass))

typedef struct AnchorSearchEngineDetails AnchorSearchEngineDetails;

typedef struct AnchorSearchEngine {
	GObject parent;
	AnchorSearchEngineDetails *details;
} AnchorSearchEngine;

typedef struct {
	GObjectClass parent_class;
} AnchorSearchEngineClass;

GType                 anchor_search_engine_get_type           (void);

AnchorSearchEngine *anchor_search_engine_new                (void);
AnchorSearchEngineModel *
                      anchor_search_engine_get_model_provider (AnchorSearchEngine *engine);
AnchorSearchEngineSimple *
                      anchor_search_engine_get_simple_provider (AnchorSearchEngine *engine);

#endif /* NAUTILUS_SEARCH_ENGINE_H */
