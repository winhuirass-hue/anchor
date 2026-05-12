/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-empty-view.c - implementation of empty view of directory.

   Copyright (C) 2006 Free Software Foundation, Inc.
   
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

   Authors: Christian Neumair <chris@gnome-de.org>
*/

#include <config.h>

#include "anchor-empty-view.h"

#include "anchor-view.h"

#include <string.h>
#include <libanchor-private/anchor-file-utilities.h>
#include <eel/eel-vfs-extensions.h>

struct AnchorEmptyViewDetails {
	int number_of_files;
};

static GList *anchor_empty_view_get_selection                   (AnchorView   *view);
static GList *anchor_empty_view_get_selection_for_file_transfer (AnchorView   *view);
static void   anchor_empty_view_scroll_to_file                  (AnchorView      *view,
								   const char        *uri);

G_DEFINE_TYPE (AnchorEmptyView, anchor_empty_view, ANCHOR_TYPE_VIEW)

static void
anchor_empty_view_add_file (AnchorView *view, AnchorFile *file, AnchorDirectory *directory)
{
	static GTimer *timer = NULL;
	static gdouble cumu = 0, elaps;
	NAUTILUS_EMPTY_VIEW (view)->details->number_of_files++;
	GdkPixbuf *icon;

	if (!timer) timer = g_timer_new ();

	g_timer_start (timer);
	icon = anchor_file_get_icon_pixbuf (file, anchor_get_icon_size_for_zoom_level (NAUTILUS_ZOOM_LEVEL_STANDARD), TRUE, 0);

	elaps = g_timer_elapsed (timer, NULL);
	g_timer_stop (timer);

	g_object_unref (icon);
	
	cumu += elaps;
	g_message ("entire loading: %.3f, cumulative %.3f", elaps, cumu);
}


static void
anchor_empty_view_begin_loading (AnchorView *view)
{
}

static void
anchor_empty_view_clear (AnchorView *view)
{
}


static void
anchor_empty_view_file_changed (AnchorView *view, AnchorFile *file, AnchorDirectory *directory)
{
}

static GList *
anchor_empty_view_get_selection (AnchorView *view)
{
	return NULL;
}


static GList *
anchor_empty_view_get_selection_for_file_transfer (AnchorView *view)
{
	return NULL;
}

static gboolean
anchor_empty_view_is_empty (AnchorView *view)
{
	return NAUTILUS_EMPTY_VIEW (view)->details->number_of_files == 0;
}

static void
anchor_empty_view_end_file_changes (AnchorView *view)
{
}

static void
anchor_empty_view_remove_file (AnchorView *view, AnchorFile *file, AnchorDirectory *directory)
{
	NAUTILUS_EMPTY_VIEW (view)->details->number_of_files--;
	g_assert (NAUTILUS_EMPTY_VIEW (view)->details->number_of_files >= 0);
}

static void
anchor_empty_view_set_selection (AnchorView *view, GList *selection)
{
	anchor_view_notify_selection_changed (view);
}

static void
anchor_empty_view_select_all (AnchorView *view)
{
}

static void
anchor_empty_view_reveal_selection (AnchorView *view)
{
}

static void
anchor_empty_view_merge_menus (AnchorView *view)
{
	NAUTILUS_VIEW_CLASS (anchor_empty_view_parent_class)->merge_menus (view);
}

static void
anchor_empty_view_update_menus (AnchorView *view)
{
	NAUTILUS_VIEW_CLASS (anchor_empty_view_parent_class)->update_menus (view);
}

/* Reset sort criteria and zoom level to match defaults */
static void
anchor_empty_view_reset_to_defaults (AnchorView *view)
{
}

static void
anchor_empty_view_bump_zoom_level (AnchorView *view, int zoom_increment)
{
}

static AnchorZoomLevel
anchor_empty_view_get_zoom_level (AnchorView *view)
{
	return NAUTILUS_ZOOM_LEVEL_STANDARD;
}

static void
anchor_empty_view_zoom_to_level (AnchorView *view,
			    AnchorZoomLevel zoom_level)
{
}

static void
anchor_empty_view_restore_default_zoom_level (AnchorView *view)
{
}

static gboolean 
anchor_empty_view_can_zoom_in (AnchorView *view) 
{
	return FALSE;
}

static gboolean 
anchor_empty_view_can_zoom_out (AnchorView *view) 
{
	return FALSE;
}

static void
anchor_empty_view_start_renaming_file (AnchorView *view,
				  AnchorFile *file,
				  gboolean select_all)
{
}

static void
anchor_empty_view_click_policy_changed (AnchorView *directory_view)
{
}


static int
anchor_empty_view_compare_files (AnchorView *view, AnchorFile *file1, AnchorFile *file2)
{
	if (file1 < file2) {
		return -1;
	}

	if (file1 > file2) {
		return +1;
	}

	return 0;
}

static gboolean
anchor_empty_view_using_manual_layout (AnchorView *view)
{
	return FALSE;
}

static void
anchor_empty_view_end_loading (AnchorView *view,
			   gboolean all_files_seen)
{
}

static char *
anchor_empty_view_get_first_visible_file (AnchorView *view)
{
	return NULL;
}

static void
anchor_empty_view_scroll_to_file (AnchorView *view,
			      const char *uri)
{
}

static void
anchor_empty_view_sort_directories_first_changed (AnchorView *view)
{
}

static const char *
anchor_empty_view_get_id (AnchorView *view)
{
	return NAUTILUS_EMPTY_VIEW_ID;
}

static void
anchor_empty_view_class_init (AnchorEmptyViewClass *class)
{
	AnchorViewClass *anchor_view_class;

	g_type_class_add_private (class, sizeof (AnchorEmptyViewDetails));

	anchor_view_class = NAUTILUS_VIEW_CLASS (class);

	anchor_view_class->add_file = anchor_empty_view_add_file;
	anchor_view_class->begin_loading = anchor_empty_view_begin_loading;
	anchor_view_class->bump_zoom_level = anchor_empty_view_bump_zoom_level;
	anchor_view_class->can_zoom_in = anchor_empty_view_can_zoom_in;
	anchor_view_class->can_zoom_out = anchor_empty_view_can_zoom_out;
        anchor_view_class->click_policy_changed = anchor_empty_view_click_policy_changed;
	anchor_view_class->clear = anchor_empty_view_clear;
	anchor_view_class->file_changed = anchor_empty_view_file_changed;
	anchor_view_class->get_selection = anchor_empty_view_get_selection;
	anchor_view_class->get_selection_for_file_transfer = anchor_empty_view_get_selection_for_file_transfer;
	anchor_view_class->is_empty = anchor_empty_view_is_empty;
	anchor_view_class->remove_file = anchor_empty_view_remove_file;
	anchor_view_class->merge_menus = anchor_empty_view_merge_menus;
	anchor_view_class->update_menus = anchor_empty_view_update_menus;
	anchor_view_class->reset_to_defaults = anchor_empty_view_reset_to_defaults;
	anchor_view_class->restore_default_zoom_level = anchor_empty_view_restore_default_zoom_level;
	anchor_view_class->reveal_selection = anchor_empty_view_reveal_selection;
	anchor_view_class->select_all = anchor_empty_view_select_all;
	anchor_view_class->set_selection = anchor_empty_view_set_selection;
	anchor_view_class->compare_files = anchor_empty_view_compare_files;
	anchor_view_class->sort_directories_first_changed = anchor_empty_view_sort_directories_first_changed;
	anchor_view_class->start_renaming_file = anchor_empty_view_start_renaming_file;
	anchor_view_class->get_zoom_level = anchor_empty_view_get_zoom_level;
	anchor_view_class->zoom_to_level = anchor_empty_view_zoom_to_level;
	anchor_view_class->end_file_changes = anchor_empty_view_end_file_changes;
	anchor_view_class->using_manual_layout = anchor_empty_view_using_manual_layout;
	anchor_view_class->end_loading = anchor_empty_view_end_loading;
	anchor_view_class->get_view_id = anchor_empty_view_get_id;
	anchor_view_class->get_first_visible_file = anchor_empty_view_get_first_visible_file;
	anchor_view_class->scroll_to_file = anchor_empty_view_scroll_to_file;
}

static void
anchor_empty_view_init (AnchorEmptyView *empty_view)
{
	empty_view->details = G_TYPE_INSTANCE_GET_PRIVATE (empty_view, ANCHOR_TYPE_EMPTY_VIEW,
							   AnchorEmptyViewDetails);
}

AnchorView *
anchor_empty_view_new (AnchorWindowSlot *slot)
{
	return g_object_new (ANCHOR_TYPE_EMPTY_VIEW,
			     "window-slot", slot,
			     NULL);
}
