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

#ifndef NAUTILUS_WINDOW_PRIVATE_H
#define NAUTILUS_WINDOW_PRIVATE_H

#include "anchor-window.h"
#include "anchor-window-slot.h"
#include "anchor-bookmark-list.h"

#include <libanchor-private/anchor-directory.h>

/* FIXME bugzilla.gnome.org 42575: Migrate more fields into here. */
struct AnchorWindowDetails
{
        GtkUIManager *ui_manager;
        GtkActionGroup *main_action_group; /* owned by ui_manager */

        /* Menus. */
        guint extensions_menu_merge_id;
        GtkActionGroup *extensions_menu_action_group;

        GtkWidget *notebook;

        /* available slots, and active slot.
         * Both of them may never be NULL.
         */
        GList *slots;
        AnchorWindowSlot *active_slot;

        GtkWidget *content_paned;
        
        /* Side Pane */
        int side_pane_width;
        GtkWidget *sidebar;        /* container for the GtkPlacesSidebar */
        GtkWidget *places_sidebar; /* the actual GtkPlacesSidebar */

        /* Main view */
        GtkWidget *main_view;

        /* Toolbar */
        GtkWidget *toolbar;
        gboolean temporary_navigation_bar;

        /* focus widget before the location bar has been shown temporarily */
        GtkWidget *last_focus_widget;

        gboolean disable_chrome;

        guint sidebar_width_handler_id;
        guint app_menu_visibility_id;
        guint bookmarks_id;
};

/* window geometry */
/* Min values are very small, and a Anchor window at this tiny size is *almost*
 * completely unusable. However, if all the extra bits (sidebar, location bar, etc)
 * are turned off, you can see an icon or two at this size. See bug 5946.
 */

#define NAUTILUS_WINDOW_MIN_WIDTH		200
#define NAUTILUS_WINDOW_MIN_HEIGHT		200
#define NAUTILUS_WINDOW_DEFAULT_WIDTH		855
#define NAUTILUS_WINDOW_DEFAULT_HEIGHT		550

typedef void (*AnchorBookmarkFailedCallback) (AnchorWindow *window,
                                                AnchorBookmark *bookmark);

void               anchor_window_load_extension_menus                  (AnchorWindow    *window);

void                 anchor_window_set_active_slot                     (AnchorWindow    *window,
									  AnchorWindowSlot *slot);

void                 anchor_window_prompt_for_location                 (AnchorWindow *window,
                                                                          GFile          *location);

/* sync window GUI with current slot. Used when changing slots,
 * and when updating the slot state.
 */
void anchor_window_sync_allow_stop       (AnchorWindow *window,
					    AnchorWindowSlot *slot);
void anchor_window_sync_title            (AnchorWindow *window,
					    AnchorWindowSlot *slot);
void anchor_window_sync_zoom_widgets     (AnchorWindow *window);
void anchor_window_sync_up_button        (AnchorWindow *window);

/* window menus */
void               anchor_window_initialize_actions                    (AnchorWindow    *window);
void               anchor_window_initialize_menus                      (AnchorWindow    *window);
void               anchor_window_finalize_menus                        (AnchorWindow    *window);

void               anchor_window_update_show_hide_menu_items           (AnchorWindow     *window);

#endif /* NAUTILUS_WINDOW_PRIVATE_H */
