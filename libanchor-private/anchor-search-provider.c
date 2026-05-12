/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-
 *
 *  Copyright (C) 2012 Red Hat, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <config.h>
#include "anchor-search-provider.h"

#include <glib-object.h>

enum {
       HITS_ADDED,
       FINISHED,
       ERROR,
       LAST_SIGNAL
};

static guint signals[LAST_SIGNAL];

static void anchor_search_provider_base_init (gpointer g_iface);

GType
anchor_search_provider_get_type (void)
{
	static GType search_provider_type = 0;

	if (!search_provider_type) {
		const GTypeInfo search_provider_info = {
			sizeof (AnchorSearchProviderIface), /* class_size */
			anchor_search_provider_base_init,   /* base_init */
			NULL,           /* base_finalize */
			NULL,
			NULL,           /* class_finalize */
			NULL,           /* class_data */
			0,
			0,              /* n_preallocs */
			NULL
		};

		search_provider_type = g_type_register_static (G_TYPE_INTERFACE,
							       "AnchorSearchProvider",
							       &search_provider_info,
							       0);

		g_type_interface_add_prerequisite (search_provider_type, G_TYPE_OBJECT);
	}

	return search_provider_type;
}

static void
anchor_search_provider_base_init (gpointer g_iface)
{
	static gboolean initialized = FALSE;

	if (initialized) {
		return;
	}

	signals[HITS_ADDED] = g_signal_new ("hits-added",
					    ANCHOR_TYPE_SEARCH_PROVIDER,
					    G_SIGNAL_RUN_LAST,
					    G_STRUCT_OFFSET (AnchorSearchProviderIface, hits_added),
					    NULL, NULL,
					    g_cclosure_marshal_VOID__POINTER,
					    G_TYPE_NONE, 1,
					    G_TYPE_POINTER);

	signals[FINISHED] = g_signal_new ("finished",
					  ANCHOR_TYPE_SEARCH_PROVIDER,
					  G_SIGNAL_RUN_LAST,
					  G_STRUCT_OFFSET (AnchorSearchProviderIface, finished),
					  NULL, NULL,
					  g_cclosure_marshal_VOID__VOID,
					  G_TYPE_NONE, 0);

	signals[ERROR] = g_signal_new ("error",
				       ANCHOR_TYPE_SEARCH_PROVIDER,
				       G_SIGNAL_RUN_LAST,
				       G_STRUCT_OFFSET (AnchorSearchProviderIface, error),
				       NULL, NULL,
				       g_cclosure_marshal_VOID__STRING,
				       G_TYPE_NONE, 1,
				       G_TYPE_STRING);

	initialized = TRUE;
}

void
anchor_search_provider_set_query (AnchorSearchProvider *provider, AnchorQuery *query)
{
	g_return_if_fail (NAUTILUS_IS_SEARCH_PROVIDER (provider));
	g_return_if_fail (ANCHOR_SEARCH_PROVIDER_GET_IFACE (provider)->set_query != NULL);
	g_return_if_fail (NAUTILUS_IS_QUERY (query));

	ANCHOR_SEARCH_PROVIDER_GET_IFACE (provider)->set_query (provider, query);
}

void
anchor_search_provider_start (AnchorSearchProvider *provider)
{
	g_return_if_fail (NAUTILUS_IS_SEARCH_PROVIDER (provider));
	g_return_if_fail (ANCHOR_SEARCH_PROVIDER_GET_IFACE (provider)->start != NULL);

	ANCHOR_SEARCH_PROVIDER_GET_IFACE (provider)->start (provider);
}

void
anchor_search_provider_stop (AnchorSearchProvider *provider)
{
	g_return_if_fail (NAUTILUS_IS_SEARCH_PROVIDER (provider));
	g_return_if_fail (ANCHOR_SEARCH_PROVIDER_GET_IFACE (provider)->stop != NULL);

	ANCHOR_SEARCH_PROVIDER_GET_IFACE (provider)->stop (provider);
}

void
anchor_search_provider_hits_added (AnchorSearchProvider *provider, GList *hits)
{
	g_return_if_fail (NAUTILUS_IS_SEARCH_PROVIDER (provider));

	g_signal_emit (provider, signals[HITS_ADDED], 0, hits);
}

void
anchor_search_provider_finished (AnchorSearchProvider *provider)
{
	g_return_if_fail (NAUTILUS_IS_SEARCH_PROVIDER (provider));

	g_signal_emit (provider, signals[FINISHED], 0);
}

void
anchor_search_provider_error (AnchorSearchProvider *provider, const char *error_message)
{
	g_return_if_fail (NAUTILUS_IS_SEARCH_PROVIDER (provider));

	g_warning ("Provider %s failed with error %s\n",
		   G_OBJECT_TYPE_NAME (provider), error_message);
	g_signal_emit (provider, signals[ERROR], 0, error_message);
}
