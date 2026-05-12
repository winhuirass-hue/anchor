/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-
 *
 * Copyright (C) 2012 Red Hat, Inc.
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
 */

#ifndef NAUTILUS_SEARCH_HIT_H
#define NAUTILUS_SEARCH_HIT_H

#include <glib-object.h>
#include "anchor-query.h"

#define ANCHOR_TYPE_SEARCH_HIT		(anchor_search_hit_get_type ())
#define NAUTILUS_SEARCH_HIT(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_SEARCH_HIT, AnchorSearchHit))
#define NAUTILUS_SEARCH_HIT_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_SEARCH_HIT, AnchorSearchHitClass))
#define NAUTILUS_IS_SEARCH_HIT(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_SEARCH_HIT))
#define NAUTILUS_IS_SEARCH_HIT_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_SEARCH_HIT))
#define NAUTILUS_SEARCH_HIT_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_SEARCH_HIT, AnchorSearchHitClass))

typedef struct AnchorSearchHitDetails AnchorSearchHitDetails;

typedef struct AnchorSearchHit {
	GObject parent;
	AnchorSearchHitDetails *details;
} AnchorSearchHit;

typedef struct {
	GObjectClass parent_class;
} AnchorSearchHitClass;

GType               anchor_search_hit_get_type      (void);

AnchorSearchHit * anchor_search_hit_new                   (const char        *uri);

void                anchor_search_hit_set_fts_rank          (AnchorSearchHit *hit,
							       gdouble            fts_rank);
void                anchor_search_hit_set_modification_time (AnchorSearchHit *hit,
							       GDateTime         *date);
void                anchor_search_hit_set_access_time       (AnchorSearchHit *hit,
							       GDateTime         *date);

void                anchor_search_hit_compute_scores        (AnchorSearchHit *hit,
							       AnchorQuery     *query);

const char *        anchor_search_hit_get_uri               (AnchorSearchHit *hit);
gdouble             anchor_search_hit_get_relevance         (AnchorSearchHit *hit);

#endif /* NAUTILUS_SEARCH_HIT_H */
