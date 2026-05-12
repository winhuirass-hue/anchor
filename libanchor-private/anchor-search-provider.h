/*
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
 */

#ifndef ANCHOR_SEARCH_PROVIDER_H
#define ANCHOR_SEARCH_PROVIDER_H

#include <glib-object.h>
#include <libanchor-private/anchor-query.h>
#include <libanchor-private/anchor-search-hit.h>

G_BEGIN_DECLS

#define ANCHOR_TYPE_SEARCH_PROVIDER           (anchor_search_provider_get_type ())
#define ANCHOR_SEARCH_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_SEARCH_PROVIDER, AnchorSearchProvider))
#define NAUTILUS_IS_SEARCH_PROVIDER(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_SEARCH_PROVIDER))
#define ANCHOR_SEARCH_PROVIDER_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), ANCHOR_TYPE_SEARCH_PROVIDER, AnchorSearchProviderIface))

typedef struct _AnchorSearchProvider       AnchorSearchProvider;
typedef struct _AnchorSearchProviderIface  AnchorSearchProviderIface;

struct _AnchorSearchProviderIface {
        GTypeInterface g_iface;

        /* VTable */
        void (*set_query) (AnchorSearchProvider *provider, AnchorQuery *query);
        void (*start) (AnchorSearchProvider *provider);
        void (*stop) (AnchorSearchProvider *provider);

        /* Signals */
        void (*hits_added) (AnchorSearchProvider *provider, GList *hits);
        void (*finished) (AnchorSearchProvider *provider);
        void (*error) (AnchorSearchProvider *provider, const char *error_message);
};

GType          anchor_search_provider_get_type        (void) G_GNUC_CONST;

/* Interface Functions */
void           anchor_search_provider_set_query       (AnchorSearchProvider *provider,
                                                         AnchorQuery *query);
void           anchor_search_provider_start           (AnchorSearchProvider *provider);
void           anchor_search_provider_stop            (AnchorSearchProvider *provider);

void           anchor_search_provider_hits_added      (AnchorSearchProvider *provider,
                                                         GList *hits);
void           anchor_search_provider_finished        (AnchorSearchProvider *provider);
void           anchor_search_provider_error           (AnchorSearchProvider *provider,
                                                         const char *error_message);

G_END_DECLS

#endif
