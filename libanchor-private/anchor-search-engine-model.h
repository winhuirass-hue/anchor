/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Red Hat, Inc
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
 * Author: Alexander Larsson <alexl@redhat.com>
 *
 */

#ifndef NAUTILUS_SEARCH_ENGINE_MODEL_H
#define NAUTILUS_SEARCH_ENGINE_MODEL_H

#include <libanchor-private/anchor-directory.h>

#define ANCHOR_TYPE_SEARCH_ENGINE_MODEL		(anchor_search_engine_model_get_type ())
#define NAUTILUS_SEARCH_ENGINE_MODEL(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_SEARCH_ENGINE_MODEL, AnchorSearchEngineModel))
#define NAUTILUS_SEARCH_ENGINE_MODEL_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_SEARCH_ENGINE_MODEL, AnchorSearchEngineModelClass))
#define NAUTILUS_IS_SEARCH_ENGINE_MODEL(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_SEARCH_ENGINE_MODEL))
#define NAUTILUS_IS_SEARCH_ENGINE_MODEL_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_SEARCH_ENGINE_MODEL))
#define NAUTILUS_SEARCH_ENGINE_MODEL_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_SEARCH_ENGINE_MODEL, AnchorSearchEngineModelClass))

typedef struct AnchorSearchEngineModelDetails AnchorSearchEngineModelDetails;

typedef struct AnchorSearchEngineModel {
	GObject parent;
	AnchorSearchEngineModelDetails *details;
} AnchorSearchEngineModel;

typedef struct {
	GObjectClass parent_class;
} AnchorSearchEngineModelClass;

GType          anchor_search_engine_model_get_type  (void);

AnchorSearchEngineModel* anchor_search_engine_model_new       (void);
void                       anchor_search_engine_model_set_model (AnchorSearchEngineModel *model,
								   AnchorDirectory         *directory);
AnchorDirectory *        anchor_search_engine_model_get_model (AnchorSearchEngineModel *model);

#endif /* NAUTILUS_SEARCH_ENGINE_MODEL_H */
