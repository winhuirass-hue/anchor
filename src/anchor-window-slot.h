/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-window-slot.h: Anchor window slot
 
   Copyright (C) 2008 Free Software Foundation, Inc.
  
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
  
   Author: Christian Neumair <cneumair@gnome.org>
*/

#ifndef NAUTILUS_WINDOW_SLOT_H
#define NAUTILUS_WINDOW_SLOT_H

#include "anchor-query-editor.h"

typedef struct AnchorWindowSlot AnchorWindowSlot;
typedef struct AnchorWindowSlotClass AnchorWindowSlotClass;
typedef struct AnchorWindowSlotDetails AnchorWindowSlotDetails;

#include "anchor-view.h"
#include "anchor-window.h"

#define ANCHOR_TYPE_WINDOW_SLOT	 (anchor_window_slot_get_type())
#define NAUTILUS_WINDOW_SLOT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_WINDOW_SLOT, AnchorWindowSlotClass))
#define NAUTILUS_WINDOW_SLOT(obj)	 (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_WINDOW_SLOT, AnchorWindowSlot))
#define NAUTILUS_IS_WINDOW_SLOT(obj)      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_WINDOW_SLOT))
#define NAUTILUS_IS_WINDOW_SLOT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_WINDOW_SLOT))
#define NAUTILUS_WINDOW_SLOT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_WINDOW_SLOT, AnchorWindowSlotClass))

typedef enum {
	NAUTILUS_LOCATION_CHANGE_STANDARD,
	NAUTILUS_LOCATION_CHANGE_BACK,
	NAUTILUS_LOCATION_CHANGE_FORWARD,
	NAUTILUS_LOCATION_CHANGE_RELOAD
} AnchorLocationChangeType;

struct AnchorWindowSlotClass {
	GtkBoxClass parent_class;

	/* wrapped AnchorWindowInfo signals, for overloading */
	void (* active)   (AnchorWindowSlot *slot);
	void (* inactive) (AnchorWindowSlot *slot);
};

/* Each AnchorWindowSlot corresponds to a location in the window
 * for displaying a AnchorView, i.e. a tab.
 */
struct AnchorWindowSlot {
	GtkBox parent;

	AnchorWindowSlotDetails *details;
};

GType   anchor_window_slot_get_type (void);

AnchorWindowSlot * anchor_window_slot_new              (AnchorWindow     *window);

AnchorWindow * anchor_window_slot_get_window           (AnchorWindowSlot *slot);
void             anchor_window_slot_set_window           (AnchorWindowSlot *slot,
							    AnchorWindow     *window);

/* convenience wrapper without selection and callback/user_data */
#define anchor_window_slot_open_location(slot, location, flags)\
	anchor_window_slot_open_location_full(slot, location, flags, NULL, NULL, NULL)

void anchor_window_slot_open_location_full              (AnchorWindowSlot *slot,
							   GFile	      *location,
							   AnchorWindowOpenFlags flags,
							   GList	      *new_selection,
							   AnchorWindowGoToCallback callback,
							   gpointer	       user_data);

GFile * anchor_window_slot_get_location		   (AnchorWindowSlot *slot);
char *  anchor_window_slot_get_location_uri		   (AnchorWindowSlot *slot);

AnchorFile *    anchor_window_slot_get_file            (AnchorWindowSlot *slot);
AnchorBookmark *anchor_window_slot_get_bookmark        (AnchorWindowSlot *slot);
AnchorView *    anchor_window_slot_get_view            (AnchorWindowSlot *slot);

AnchorView * anchor_window_slot_get_current_view       (AnchorWindowSlot *slot);
char *         anchor_window_slot_get_current_uri        (AnchorWindowSlot *slot);

GList * anchor_window_slot_get_back_history              (AnchorWindowSlot *slot);
GList * anchor_window_slot_get_forward_history           (AnchorWindowSlot *slot);

GFile * anchor_window_slot_get_query_editor_location     (AnchorWindowSlot *slot);
void    anchor_window_slot_set_search_visible            (AnchorWindowSlot *slot,
							    gboolean            visible);

gboolean anchor_window_slot_get_allow_stop               (AnchorWindowSlot *slot);
void     anchor_window_slot_set_allow_stop		   (AnchorWindowSlot *slot,
							    gboolean	        allow_stop);
void     anchor_window_slot_stop_loading                 (AnchorWindowSlot *slot);

const gchar *anchor_window_slot_get_title                (AnchorWindowSlot *slot);
void         anchor_window_slot_update_title		   (AnchorWindowSlot *slot);

gboolean anchor_window_slot_handle_event       	   (AnchorWindowSlot *slot,
							    GdkEventKey        *event);

void    anchor_window_slot_queue_reload		   (AnchorWindowSlot *slot);

void	 anchor_window_slot_set_content_view	           (AnchorWindowSlot *slot,
							    const char		*id);

void    anchor_window_slot_go_home			   (AnchorWindowSlot *slot,
							    AnchorWindowOpenFlags flags);
void    anchor_window_slot_go_up                         (AnchorWindowSlot *slot,
							    AnchorWindowOpenFlags flags);

void    anchor_window_slot_set_status			   (AnchorWindowSlot *slot,
							    const char         *primary_status,
							    const char         *detail_status);

#endif /* NAUTILUS_WINDOW_SLOT_H */
