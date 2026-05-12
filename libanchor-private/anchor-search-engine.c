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

#include <config.h>

#include <glib/gi18n.h>
#include "anchor-search-provider.h"
#include "anchor-search-engine.h"
#include "anchor-search-engine-simple.h"
#include "anchor-search-engine-model.h"
#define DEBUG_FLAG NAUTILUS_DEBUG_SEARCH
#include "anchor-debug.h"

#ifdef ENABLE_TRACKER
#include "anchor-search-engine-tracker.h"
#endif

struct AnchorSearchEngineDetails
{
#ifdef ENABLE_TRACKER
	AnchorSearchEngineTracker *tracker;
#endif
	AnchorSearchEngineSimple *simple;
	AnchorSearchEngineModel *model;

	GHashTable *uris;
	guint providers_running;
	guint providers_finished;
	guint providers_error;

	gboolean running;
	gboolean restart;
};

static void anchor_search_provider_init (AnchorSearchProviderIface  *iface);

G_DEFINE_TYPE_WITH_CODE (AnchorSearchEngine,
			 anchor_search_engine,
			 G_TYPE_OBJECT,
			 G_IMPLEMENT_INTERFACE (ANCHOR_TYPE_SEARCH_PROVIDER,
						anchor_search_provider_init))

static void
anchor_search_engine_set_query (AnchorSearchProvider *provider,
				  AnchorQuery          *query)
{
	AnchorSearchEngine *engine = NAUTILUS_SEARCH_ENGINE (provider);
#ifdef ENABLE_TRACKER
	anchor_search_provider_set_query (ANCHOR_SEARCH_PROVIDER (engine->details->tracker), query);
#endif
	anchor_search_provider_set_query (ANCHOR_SEARCH_PROVIDER (engine->details->model), query);
	anchor_search_provider_set_query (ANCHOR_SEARCH_PROVIDER (engine->details->simple), query);
}

static void
search_engine_start_real (AnchorSearchEngine *engine)
{
	engine->details->providers_running = 0;
	engine->details->providers_finished = 0;
	engine->details->providers_error = 0;

	engine->details->restart = FALSE;

	DEBUG ("Search engine start real");

	g_object_ref (engine);

#ifdef ENABLE_TRACKER
	anchor_search_provider_start (ANCHOR_SEARCH_PROVIDER (engine->details->tracker));
	engine->details->providers_running++;
#endif
	if (anchor_search_engine_model_get_model (engine->details->model)) {
		anchor_search_provider_start (ANCHOR_SEARCH_PROVIDER (engine->details->model));
		engine->details->providers_running++;
	}

	anchor_search_provider_start (ANCHOR_SEARCH_PROVIDER (engine->details->simple));
	engine->details->providers_running++;
}

static void
anchor_search_engine_start (AnchorSearchProvider *provider)
{
	AnchorSearchEngine *engine = NAUTILUS_SEARCH_ENGINE (provider);
	gint num_finished;

	DEBUG ("Search engine start");

	num_finished = engine->details->providers_error + engine->details->providers_finished;

	if (engine->details->running) {
		if (num_finished == engine->details->providers_running &&
		    engine->details->restart) {
			search_engine_start_real (engine);
		}

		return;
	}

	engine->details->running = TRUE;

	if (num_finished < engine->details->providers_running) {
		engine->details->restart = TRUE;
	} else {
		search_engine_start_real (engine);
	}
}

static void
anchor_search_engine_stop (AnchorSearchProvider *provider)
{
	AnchorSearchEngine *engine = NAUTILUS_SEARCH_ENGINE (provider);

	DEBUG ("Search engine stop");

#ifdef ENABLE_TRACKER
	anchor_search_provider_stop (ANCHOR_SEARCH_PROVIDER (engine->details->tracker));
#endif
	anchor_search_provider_stop (ANCHOR_SEARCH_PROVIDER (engine->details->model));
	anchor_search_provider_stop (ANCHOR_SEARCH_PROVIDER (engine->details->simple));

	engine->details->running = FALSE;
	engine->details->restart = FALSE;
}

static void
search_provider_hits_added (AnchorSearchProvider *provider,
			    GList                  *hits,
			    AnchorSearchEngine   *engine)
{
	GList *added = NULL;
	GList *l;

	if (!engine->details->running || engine->details->restart) {
		DEBUG ("Ignoring hits-added, since engine is %s",
		       !engine->details->running ? "not running" : "waiting to restart");
		return;
	}

	for (l = hits; l != NULL; l = l->next) {
		AnchorSearchHit *hit = l->data;
		int count;
		const char *uri;

		uri = anchor_search_hit_get_uri (hit);
		count = GPOINTER_TO_INT (g_hash_table_lookup (engine->details->uris, uri));
		if (count == 0)
			added = g_list_prepend (added, hit);
		g_hash_table_replace (engine->details->uris, g_strdup (uri), GINT_TO_POINTER (++count));
	}
	if (added != NULL) {
		added = g_list_reverse (added);
		anchor_search_provider_hits_added (ANCHOR_SEARCH_PROVIDER (engine), added);
		g_list_free (added);
	}
}

static void
check_providers_status (AnchorSearchEngine *engine)
{
	gint num_finished = engine->details->providers_error + engine->details->providers_finished;

	if (num_finished < engine->details->providers_running) {
		return;
	}

	if (num_finished == engine->details->providers_error) {
		DEBUG ("Search engine error");
		anchor_search_provider_error (ANCHOR_SEARCH_PROVIDER (engine),
						_("Unable to complete the requested search"));
	} else {
		DEBUG ("Search engine finished");
		anchor_search_provider_finished (ANCHOR_SEARCH_PROVIDER (engine));
	}

	engine->details->running = FALSE;
	g_hash_table_remove_all (engine->details->uris);

	if (engine->details->restart) {
		DEBUG ("Restarting engine");
		anchor_search_engine_start (ANCHOR_SEARCH_PROVIDER (engine));
	}

	g_object_unref (engine);
}

static void
search_provider_error (AnchorSearchProvider *provider,
		       const char             *error_message,
		       AnchorSearchEngine   *engine)

{
	DEBUG ("Search provider error: %s", error_message);
	engine->details->providers_error++;

	check_providers_status (engine);
}

static void
search_provider_finished (AnchorSearchProvider *provider,
			  AnchorSearchEngine   *engine)

{
	DEBUG ("Search provider finished");
	engine->details->providers_finished++;

	check_providers_status (engine);
}

static void
connect_provider_signals (AnchorSearchEngine   *engine,
			  AnchorSearchProvider *provider)
{
	g_signal_connect (provider, "hits-added",
			  G_CALLBACK (search_provider_hits_added),
			  engine);
	g_signal_connect (provider, "finished",
			  G_CALLBACK (search_provider_finished),
			  engine);
	g_signal_connect (provider, "error",
			  G_CALLBACK (search_provider_error),
			  engine);
}

static void
anchor_search_provider_init (AnchorSearchProviderIface *iface)
{
	iface->set_query = anchor_search_engine_set_query;
	iface->start = anchor_search_engine_start;
	iface->stop = anchor_search_engine_stop;
}

static void
anchor_search_engine_finalize (GObject *object)
{
	AnchorSearchEngine *engine = NAUTILUS_SEARCH_ENGINE (object);

	g_hash_table_destroy (engine->details->uris);

#ifdef ENABLE_TRACKER
	g_clear_object (&engine->details->tracker);
#endif
	g_clear_object (&engine->details->model);
	g_clear_object (&engine->details->simple);

	G_OBJECT_CLASS (anchor_search_engine_parent_class)->finalize (object);
}

static void
anchor_search_engine_class_init (AnchorSearchEngineClass *class)
{
	GObjectClass *object_class;

	object_class = (GObjectClass *) class;

	object_class->finalize = anchor_search_engine_finalize;

	g_type_class_add_private (class, sizeof (AnchorSearchEngineDetails));
}

static void
anchor_search_engine_init (AnchorSearchEngine *engine)
{
	engine->details = G_TYPE_INSTANCE_GET_PRIVATE (engine,
						       ANCHOR_TYPE_SEARCH_ENGINE,
						       AnchorSearchEngineDetails);

	engine->details->uris = g_hash_table_new_full (g_str_hash, g_str_equal, g_free, NULL);

#ifdef ENABLE_TRACKER
	engine->details->tracker = anchor_search_engine_tracker_new ();
	connect_provider_signals (engine, ANCHOR_SEARCH_PROVIDER (engine->details->tracker));
#endif
	engine->details->model = anchor_search_engine_model_new ();
	connect_provider_signals (engine, ANCHOR_SEARCH_PROVIDER (engine->details->model));

	engine->details->simple = anchor_search_engine_simple_new ();
	connect_provider_signals (engine, ANCHOR_SEARCH_PROVIDER (engine->details->simple));
}

AnchorSearchEngine *
anchor_search_engine_new (void)
{
	AnchorSearchEngine *engine;

	engine = g_object_new (ANCHOR_TYPE_SEARCH_ENGINE, NULL);

	return engine;
}

AnchorSearchEngineModel *
anchor_search_engine_get_model_provider (AnchorSearchEngine *engine)
{
	return engine->details->model;
}

AnchorSearchEngineSimple *
anchor_search_engine_get_simple_provider (AnchorSearchEngine *engine)
{
	return engine->details->simple;
}
