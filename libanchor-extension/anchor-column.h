/*
 *  anchor-column.h - Info columns exported by 
 *                      AnchorColumnProvider objects.
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

#ifndef NAUTILUS_COLUMN_H
#define NAUTILUS_COLUMN_H

#include <glib-object.h>
#include "anchor-extension-types.h"

G_BEGIN_DECLS

#define ANCHOR_TYPE_COLUMN            (anchor_column_get_type())
#define NAUTILUS_COLUMN(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_COLUMN, AnchorColumn))
#define NAUTILUS_COLUMN_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_COLUMN, AnchorColumnClass))
#define NAUTILUS_INFO_IS_COLUMN(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_COLUMN))
#define NAUTILUS_INFO_IS_COLUMN_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), ANCHOR_TYPE_COLUMN))
#define NAUTILUS_COLUMN_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), ANCHOR_TYPE_COLUMN, AnchorColumnClass))

typedef struct _NautilusColumn        AnchorColumn;
typedef struct _NautilusColumnDetails AnchorColumnDetails;
typedef struct _NautilusColumnClass   AnchorColumnClass;

struct _NautilusColumn {
	GObject parent;

	AnchorColumnDetails *details;
};

struct _NautilusColumnClass {
	GObjectClass parent;
};

GType             anchor_column_get_type        (void);
AnchorColumn *  anchor_column_new             (const char     *name,
						   const char     *attribute,
						   const char     *label,
						   const char     *description);

/* AnchorColumn has the following properties:
 *   name (string)        - the identifier for the column
 *   attribute (string)   - the file attribute to be displayed in the 
 *                          column
 *   label (string)       - the user-visible label for the column
 *   description (string) - a user-visible description of the column
 *   xalign (float)       - x-alignment of the column 
 *   default-sort-order (GtkSortType) - default sort order of the column
 */

G_END_DECLS

#endif
