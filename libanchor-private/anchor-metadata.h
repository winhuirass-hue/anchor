/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-metadata.h: #defines and other metadata-related info
 
   Copyright (C) 2000 Eazel, Inc.
  
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
  
   You should have received a copy of the GNU General Public
   License along with this program; if not, see <http://www.gnu.org/licenses/>.
  
   Author: John Sullivan <sullivan@eazel.com>
*/

#ifndef NAUTILUS_METADATA_H
#define NAUTILUS_METADATA_H

/* Keys for getting/setting Anchor metadata. All metadata used in Anchor
 * should define its key here, so we can keep track of the whole set easily.
 * Any updates here needs to be added in anchor-metadata.c too.
 */

#include <glib.h>

/* Per-file */

#define NAUTILUS_METADATA_KEY_LOCATION_BACKGROUND_COLOR 	"folder-background-color"
#define NAUTILUS_METADATA_KEY_LOCATION_BACKGROUND_IMAGE 	"folder-background-image"

#define NAUTILUS_METADATA_KEY_ICON_VIEW_AUTO_LAYOUT      	"anchor-icon-view-auto-layout"
#define NAUTILUS_METADATA_KEY_ICON_VIEW_SORT_BY          	"anchor-icon-view-sort-by"
#define NAUTILUS_METADATA_KEY_ICON_VIEW_SORT_REVERSED    	"anchor-icon-view-sort-reversed"
#define NAUTILUS_METADATA_KEY_ICON_VIEW_KEEP_ALIGNED            "anchor-icon-view-keep-aligned"
#define NAUTILUS_METADATA_KEY_ICON_VIEW_LAYOUT_TIMESTAMP	"anchor-icon-view-layout-timestamp"

#define NAUTILUS_METADATA_KEY_LIST_VIEW_SORT_COLUMN      	"anchor-list-view-sort-column"
#define NAUTILUS_METADATA_KEY_LIST_VIEW_SORT_REVERSED    	"anchor-list-view-sort-reversed"
#define NAUTILUS_METADATA_KEY_LIST_VIEW_VISIBLE_COLUMNS    	"anchor-list-view-visible-columns"
#define NAUTILUS_METADATA_KEY_LIST_VIEW_COLUMN_ORDER    	"anchor-list-view-column-order"

#define NAUTILUS_METADATA_KEY_WINDOW_GEOMETRY			"anchor-window-geometry"
#define NAUTILUS_METADATA_KEY_WINDOW_SCROLL_POSITION		"anchor-window-scroll-position"
#define NAUTILUS_METADATA_KEY_WINDOW_SHOW_HIDDEN_FILES		"anchor-window-show-hidden-files"
#define NAUTILUS_METADATA_KEY_WINDOW_MAXIMIZED			"anchor-window-maximized"
#define NAUTILUS_METADATA_KEY_WINDOW_STICKY			"anchor-window-sticky"
#define NAUTILUS_METADATA_KEY_WINDOW_KEEP_ABOVE			"anchor-window-keep-above"

#define NAUTILUS_METADATA_KEY_SIDEBAR_BACKGROUND_COLOR   	"anchor-sidebar-background-color"
#define NAUTILUS_METADATA_KEY_SIDEBAR_BACKGROUND_IMAGE   	"anchor-sidebar-background-image"
#define NAUTILUS_METADATA_KEY_SIDEBAR_BUTTONS			"anchor-sidebar-buttons"

#define NAUTILUS_METADATA_KEY_ICON_POSITION              	"anchor-icon-position"
#define NAUTILUS_METADATA_KEY_ICON_POSITION_TIMESTAMP		"anchor-icon-position-timestamp"
#define NAUTILUS_METADATA_KEY_ANNOTATION                 	"annotation"
#define NAUTILUS_METADATA_KEY_ICON_SCALE                 	"icon-scale"
#define NAUTILUS_METADATA_KEY_CUSTOM_ICON                	"custom-icon"
#define NAUTILUS_METADATA_KEY_CUSTOM_ICON_NAME                	"custom-icon-name"
#define NAUTILUS_METADATA_KEY_SCREEN				"screen"
#define NAUTILUS_METADATA_KEY_EMBLEMS				"emblems"

guint anchor_metadata_get_id (const char *metadata);

#endif /* NAUTILUS_METADATA_H */
