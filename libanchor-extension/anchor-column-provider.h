/*
 *  anchor-column-provider.h - Interface for Anchor extensions that 
 *                               provide column descriptions.
 *
 *  Copyright (C) 2003 Novell, Inc.
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
 *  Author:  Dave Camp <dave@ximian.com>
 *
 */

/* This interface is implemented by Anchor extensions that want to
 * add columns to the list view and details to the icon view.
 * Extensions are asked for a list of columns to display.  Each
 * returned column refers to a string attribute which can be filled in
 * by AnchorInfoProvider */

#ifndef NAUTILUS_COLUMN_PROVIDER_H
#define NAUTILUS_COLUMN_PROVIDER_H

#include <glib-object.h>
#include "anchor-extension-types.h"
#include "anchor-column.h"

G_BEGIN_DECLS

#define ANCHOR_TYPE_COLUMN_PROVIDER           (anchor_column_provider_get_type ())
#define NAUTILUS_COLUMN_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_COLUMN_PROVIDER, AnchorColumnProvider))
#define NAUTILUS_IS_COLUMN_PROVIDER(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_COLUMN_PROVIDER))
#define NAUTILUS_COLUMN_PROVIDER_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), ANCHOR_TYPE_COLUMN_PROVIDER, AnchorColumnProviderIface))

typedef struct _NautilusColumnProvider       AnchorColumnProvider;
typedef struct _NautilusColumnProviderIface  AnchorColumnProviderIface;

struct _NautilusColumnProviderIface {
	GTypeInterface g_iface;

	GList *(*get_columns) (AnchorColumnProvider *provider);
};

/* Interface Functions */
GType                   anchor_column_provider_get_type       (void);
GList                  *anchor_column_provider_get_columns    (AnchorColumnProvider *provider);

G_END_DECLS

#endif
