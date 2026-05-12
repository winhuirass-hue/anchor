/*
 *  anchor-info-provider.h - Type definitions for Anchor extensions
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
 *  Author: Dave Camp <dave@ximian.com>
 * 
 */

/* This interface is implemented by Anchor extensions that want to 
 * provide information about files.  Extensions are called when Anchor 
 * needs information about a file.  They are passed a AnchorFileInfo 
 * object which should be filled with relevant information */

#ifndef NAUTILUS_EXTENSION_TYPES_H
#define NAUTILUS_EXTENSION_TYPES_H

#include <glib-object.h>

G_BEGIN_DECLS

#define ANCHOR_TYPE_OPERATION_RESULT (anchor_operation_result_get_type ())

/* Handle for asynchronous interfaces.  These are opaque handles that must
 * be unique within an extension object.  These are returned by operations
 * that return NAUTILUS_OPERATION_IN_PROGRESS */
typedef struct _NautilusOperationHandle AnchorOperationHandle;

typedef enum {
	/* Returned if the call succeeded, and the extension is done 
	 * with the request */
	NAUTILUS_OPERATION_COMPLETE,

	/* Returned if the call failed */
	NAUTILUS_OPERATION_FAILED,

	/* Returned if the extension has begun an async operation. 
	 * If this is returned, the extension must set the handle 
	 * parameter and call the callback closure when the 
	 * operation is complete. */
	NAUTILUS_OPERATION_IN_PROGRESS
} AnchorOperationResult;

GType anchor_operation_result_get_type (void);

void anchor_module_initialize (GTypeModule  *module);
void anchor_module_shutdown   (void);
void anchor_module_list_types (const GType **types,
				 int          *num_types);

G_END_DECLS

#endif
