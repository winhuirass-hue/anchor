/* -*- Mode: C; tab-width: 8; indent-tabs-mode: nil; c-basic-offset: 8 -*- */

/*
 *  Anchor
 *
 *  Copyright (C) 1999, 2000 Red Hat, Inc.
 *  Copyright (C) 1999, 2000, 2001 Eazel, Inc.
 *
 *  Anchor is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public License as
 *  published by the Free Software Foundation; either version 2 of the
 *  License, or (at your option) any later version.
 *
 *  Anchor is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Authors: Elliot Lee <sopwith@redhat.com>
 *           Darin Adler <darin@bentspoon.com>
 *
 */
/* anchor-window.h: Interface of the main window object */

#ifndef NAUTILUS_WINDOW_H
#define NAUTILUS_WINDOW_H

#include <gtk/gtk.h>
#include <eel/eel-glib-extensions.h>
#include <libanchor-private/anchor-bookmark.h>
#include <libanchor-private/anchor-search-directory.h>

typedef struct AnchorWindow AnchorWindow;
typedef struct AnchorWindowClass AnchorWindowClass;
typedef struct AnchorWindowDetails AnchorWindowDetails;

typedef enum {
        NAUTILUS_WINDOW_OPEN_FLAG_CLOSE_BEHIND = 1 << 0,
        NAUTILUS_WINDOW_OPEN_FLAG_NEW_WINDOW = 1 << 1,
        NAUTILUS_WINDOW_OPEN_FLAG_NEW_TAB = 1 << 2,
        NAUTILUS_WINDOW_OPEN_FLAG_USE_DEFAULT_LOCATION = 1 << 3
} AnchorWindowOpenFlags;

typedef enum {
	NAUTILUS_WINDOW_OPEN_SLOT_NONE = 0,
	NAUTILUS_WINDOW_OPEN_SLOT_APPEND = 1
}  AnchorWindowOpenSlotFlags;

typedef gboolean (* AnchorWindowGoToCallback) (AnchorWindow *window,
                                                 GFile *location,
                                                 GError *error,
                                                 gpointer user_data);

#include "anchor-view.h"
#include "anchor-window-slot.h"

#define ANCHOR_TYPE_WINDOW anchor_window_get_type()
#define NAUTILUS_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_WINDOW, AnchorWindow))
#define NAUTILUS_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_WINDOW, AnchorWindowClass))
#define NAUTILUS_IS_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_WINDOW))
#define NAUTILUS_IS_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_WINDOW))
#define NAUTILUS_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_WINDOW, AnchorWindowClass))

#define NAUTILUS_WINDOW_SIDEBAR_PLACES "places"
#define NAUTILUS_WINDOW_SIDEBAR_TREE "tree"

struct AnchorWindowClass {
        GtkApplicationWindowClass parent_spot;

	/* Function pointers for overriding, without corresponding signals */
        void   (* sync_title) (AnchorWindow *window,
			       AnchorWindowSlot *slot);
        void   (* close) (AnchorWindow *window);
};

struct AnchorWindow {
        GtkApplicationWindow parent_object;
        
        AnchorWindowDetails *details;
};

GType            anchor_window_get_type             (void);
AnchorWindow * anchor_window_new                  (GdkScreen         *screen);
void             anchor_window_close                (AnchorWindow    *window);

void             anchor_window_connect_content_view (AnchorWindow    *window,
						       AnchorView      *view);
void             anchor_window_disconnect_content_view (AnchorWindow    *window,
							  AnchorView      *view);

void             anchor_window_go_to                (AnchorWindow    *window,
                                                       GFile             *location);
void             anchor_window_go_to_full           (AnchorWindow    *window,
                                                       GFile             *location,
                                                       AnchorWindowGoToCallback callback,
                                                       gpointer           user_data);
void             anchor_window_new_tab              (AnchorWindow    *window);

GtkUIManager *   anchor_window_get_ui_manager       (AnchorWindow    *window);
GtkActionGroup * anchor_window_get_main_action_group (AnchorWindow   *window);

void                 anchor_window_view_visible          (AnchorWindow *window,
                                                            AnchorView *view);
AnchorWindowSlot * anchor_window_get_active_slot       (AnchorWindow *window);
GList *              anchor_window_get_slots             (AnchorWindow *window);
AnchorWindowSlot * anchor_window_open_slot             (AnchorWindow *window,
                                                            AnchorWindowOpenSlotFlags flags);
void                 anchor_window_slot_close            (AnchorWindow *window,
                                                            AnchorWindowSlot *slot);

GtkWidget *          anchor_window_ensure_location_entry (AnchorWindow *window);
void                 anchor_window_sync_location_widgets (AnchorWindow *window);
void                 anchor_window_grab_focus            (AnchorWindow *window);

void     anchor_window_hide_sidebar         (AnchorWindow *window);
void     anchor_window_show_sidebar         (AnchorWindow *window);
void     anchor_window_back_or_forward      (AnchorWindow *window,
                                               gboolean        back,
                                               guint           distance,
                                               AnchorWindowOpenFlags flags);


gboolean anchor_window_disable_chrome_mapping (GValue *value,
                                                 GVariant *variant,
                                                 gpointer user_data);

AnchorWindowOpenFlags anchor_event_get_window_open_flags   (void);
void     anchor_window_show_about_dialog    (AnchorWindow *window);

#endif
