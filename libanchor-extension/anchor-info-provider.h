/*
 *  anchor-info-provider.h - Interface for Anchor extensions that 
 *                             provide info about files.
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
 * provide information about files.  Extensions are called when Anchor 
 * needs information about a file.  They are passed a AnchorFileInfo 
 * object which should be filled with relevant information */

#ifndef NAUTILUS_INFO_PROVIDER_H
#define NAUTILUS_INFO_PROVIDER_H

#include <glib-object.h>
#include "anchor-extension-types.h"
#include "anchor-file-info.h"

G_BEGIN_DECLS

#define ANCHOR_TYPE_INFO_PROVIDER           (anchor_info_provider_get_type ())
#define NAUTILUS_INFO_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_INFO_PROVIDER, AnchorInfoProvider))
#define NAUTILUS_IS_INFO_PROVIDER(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_INFO_PROVIDER))
#define NAUTILUS_INFO_PROVIDER_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), ANCHOR_TYPE_INFO_PROVIDER, AnchorInfoProviderIface))

typedef struct _NautilusInfoProvider       AnchorInfoProvider;
typedef struct _NautilusInfoProviderIface  AnchorInfoProviderIface;

typedef void (*AnchorInfoProviderUpdateComplete) (AnchorInfoProvider    *provider,
						    AnchorOperationHandle *handle,
						    AnchorOperationResult  result,
						    gpointer                 user_data);

struct _NautilusInfoProviderIface {
	GTypeInterface g_iface;

	AnchorOperationResult (*update_file_info) (AnchorInfoProvider     *provider,
						     AnchorFileInfo         *file,
						     GClosure                 *update_complete,
						     AnchorOperationHandle **handle);
	void                    (*cancel_update)    (AnchorInfoProvider     *provider,
						     AnchorOperationHandle  *handle);
};

/* Interface Functions */
GType                   anchor_info_provider_get_type               (void);
AnchorOperationResult anchor_info_provider_update_file_info       (AnchorInfoProvider     *provider,
								       AnchorFileInfo         *file,
								       GClosure                 *update_complete,
								       AnchorOperationHandle **handle);
void                    anchor_info_provider_cancel_update          (AnchorInfoProvider     *provider,
								       AnchorOperationHandle  *handle);



/* Helper functions for implementations */
void                    anchor_info_provider_update_complete_invoke (GClosure                 *update_complete,
								       AnchorInfoProvider     *provider,
								       AnchorOperationHandle  *handle,
								       AnchorOperationResult   result);

G_END_DECLS

#endif
