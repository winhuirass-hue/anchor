/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* anchor-dnd.h - Common Drag & drop handling code shared by the icon container
   and the list view.

   Copyright (C) 2000 Eazel, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   see <http://www.gnu.org/licenses/>.

   Authors: Pavel Cisler <pavel@eazel.com>,
	    Ettore Perazzoli <ettore@gnu.org>
*/

#ifndef NAUTILUS_DND_H
#define NAUTILUS_DND_H

#include <gtk/gtk.h>
#include "anchor-file.h"

/* Drag & Drop target names. */
#define NAUTILUS_ICON_DND_GNOME_ICON_LIST_TYPE	"x-special/gnome-icon-list"
#define NAUTILUS_ICON_DND_URI_LIST_TYPE		"text/uri-list"
#define NAUTILUS_ICON_DND_NETSCAPE_URL_TYPE	"_NETSCAPE_URL"
#define NAUTILUS_ICON_DND_BGIMAGE_TYPE		"property/bgimage"
#define NAUTILUS_ICON_DND_ROOTWINDOW_DROP_TYPE	"application/x-rootwindow-drop"
#define NAUTILUS_ICON_DND_XDNDDIRECTSAVE_TYPE	"XdndDirectSave0" /* XDS Protocol Type */
#define NAUTILUS_ICON_DND_RAW_TYPE	"application/octet-stream"

/* Item of the drag selection list */
typedef struct {
	AnchorFile *file;
	char *uri;
	gboolean got_icon_position;
	int icon_x, icon_y;
	int icon_width, icon_height;
} AnchorDragSelectionItem;

/* Standard Drag & Drop types. */
typedef enum {
	NAUTILUS_ICON_DND_GNOME_ICON_LIST,
	NAUTILUS_ICON_DND_URI_LIST,
	NAUTILUS_ICON_DND_NETSCAPE_URL,
	NAUTILUS_ICON_DND_TEXT,
	NAUTILUS_ICON_DND_XDNDDIRECTSAVE,
	NAUTILUS_ICON_DND_RAW,
	NAUTILUS_ICON_DND_ROOTWINDOW_DROP
} AnchorIconDndTargetType;

/* drag&drop-related information. */
typedef struct {
	GtkTargetList *target_list;

	/* Stuff saved at "receive data" time needed later in the drag. */
	gboolean got_drop_data_type;
	AnchorIconDndTargetType data_type;
	GtkSelectionData *selection_data;
	char *direct_save_uri;

	/* Start of the drag, in window coordinates. */
	int start_x, start_y;

	/* List of AnchorDragSelectionItems, representing items being dragged, or NULL
	 * if data about them has not been received from the source yet.
	 */
	GList *selection_list;

	/* cache of selected URIs, representing items being dragged */
	GList *selection_cache;

	/* has the drop occured ? */
	gboolean drop_occured;

	/* whether or not need to clean up the previous dnd data */
	gboolean need_to_destroy;

	/* autoscrolling during dragging */
	int auto_scroll_timeout_id;
	gboolean waiting_to_autoscroll;
	gint64 start_auto_scroll_in;

} AnchorDragInfo;

typedef void		(* AnchorDragEachSelectedItemDataGet)	(const char *url, 
								 int x, int y, int w, int h, 
								 gpointer data);
typedef void		(* AnchorDragEachSelectedItemIterator)	(AnchorDragEachSelectedItemDataGet iteratee, 
								 gpointer iterator_context, 
								 gpointer data);

void			    anchor_drag_init				(AnchorDragInfo		      *drag_info,
									 const GtkTargetEntry		      *drag_types,
									 int				       drag_type_count,
									 gboolean			       add_text_targets);
void			    anchor_drag_finalize			(AnchorDragInfo		      *drag_info);
AnchorDragSelectionItem  *anchor_drag_selection_item_new		(void);
void			    anchor_drag_destroy_selection_list	(GList				      *selection_list);
GList			   *anchor_drag_build_selection_list		(GtkSelectionData		      *data);

char **			    anchor_drag_uri_array_from_selection_list (const GList			      *selection_list);
GList *			    anchor_drag_uri_list_from_selection_list	(const GList			      *selection_list);

char **			    anchor_drag_uri_array_from_list		(const GList			      *uri_list);
GList *			    anchor_drag_uri_list_from_array		(const char			     **uris);

gboolean		    anchor_drag_items_local			(const char			      *target_uri,
									 const GList			      *selection_list);
gboolean		    anchor_drag_uris_local			(const char			      *target_uri,
									 const GList			      *source_uri_list);
gboolean		    anchor_drag_items_in_trash		(const GList			      *selection_list);
gboolean		    anchor_drag_items_on_desktop		(const GList			      *selection_list);
void			    anchor_drag_default_drop_action_for_icons (GdkDragContext			      *context,
									 const char			      *target_uri,
									 const GList			      *items,
									 int				      *action);
GdkDragAction		    anchor_drag_default_drop_action_for_netscape_url (GdkDragContext			     *context);
GdkDragAction		    anchor_drag_default_drop_action_for_uri_list     (GdkDragContext			     *context,
										const char			     *target_uri_string);
gboolean		    anchor_drag_drag_data_get			(GtkWidget			      *widget,
									 GdkDragContext			      *context,
									 GtkSelectionData		      *selection_data,
									 guint				       info,
									 guint32			       time,
									 gpointer			       container_context,
									 AnchorDragEachSelectedItemIterator  each_selected_item_iterator);
GList			   *anchor_drag_create_selection_cache	(gpointer			       container_context,
									 AnchorDragEachSelectedItemIterator  each_selected_item_iterator);
gboolean		    anchor_drag_drag_data_get_from_cache	(GList				      *cache,
									 GdkDragContext			      *context,
									 GtkSelectionData		      *selection_data,
									 guint				       info,
									 guint32			       time);
int			    anchor_drag_modifier_based_action		(int				       default_action,
									 int				       non_default_action);

GdkDragAction		    anchor_drag_drop_action_ask		(GtkWidget			      *widget,
									 GdkDragAction			       possible_actions);

gboolean		    anchor_drag_autoscroll_in_scroll_region	(GtkWidget			      *widget);
void			    anchor_drag_autoscroll_calculate_delta	(GtkWidget			      *widget,
									 float				      *x_scroll_delta,
									 float				      *y_scroll_delta);
void			    anchor_drag_autoscroll_start		(AnchorDragInfo		      *drag_info,
									 GtkWidget			      *widget,
									 GSourceFunc			       callback,
									 gpointer			       user_data);
void			    anchor_drag_autoscroll_stop		(AnchorDragInfo		      *drag_info);

gboolean		    anchor_drag_selection_includes_special_link (GList			      *selection_list);

#endif
