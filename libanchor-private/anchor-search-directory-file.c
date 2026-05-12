/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-search-directory-file.c: Subclass of AnchorFile to help implement the
   searches
 
   Copyright (C) 2005 Novell, Inc.
  
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
  
   Author: Anders Carlsson <andersca@imendio.com>
*/

#include <config.h>
#include "anchor-search-directory-file.h"

#include "anchor-directory-notify.h"
#include "anchor-directory-private.h"
#include "anchor-file-attributes.h"
#include "anchor-file-private.h"
#include "anchor-file-utilities.h"
#include "anchor-keyfile-metadata.h"
#include <eel/eel-glib-extensions.h>
#include "anchor-search-directory.h"
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <string.h>

struct AnchorSearchDirectoryFileDetails {
	gchar *metadata_filename;
};

G_DEFINE_TYPE(AnchorSearchDirectoryFile, anchor_search_directory_file, ANCHOR_TYPE_FILE);


static void
search_directory_file_monitor_add (AnchorFile *file,
				   gconstpointer client,
				   AnchorFileAttributes attributes)
{
	/* No need for monitoring, we always emit changed when files
	   are added/removed, and no other metadata changes */

	/* Update display name, in case this didn't happen yet */
	anchor_search_directory_file_update_display_name (NAUTILUS_SEARCH_DIRECTORY_FILE (file));
}

static void
search_directory_file_monitor_remove (AnchorFile *file,
				      gconstpointer client)
{
	/* Do nothing here, we don't have any monitors */
}

static void
search_directory_file_call_when_ready (AnchorFile *file,
				       AnchorFileAttributes file_attributes,
				       AnchorFileCallback callback,
				       gpointer callback_data)

{
	/* Update display name, in case this didn't happen yet */
	anchor_search_directory_file_update_display_name (NAUTILUS_SEARCH_DIRECTORY_FILE (file));
	
	/* All data for directory-as-file is always uptodate */
	(* callback) (file, callback_data);
}
 
static void
search_directory_file_cancel_call_when_ready (AnchorFile *file,
					       AnchorFileCallback callback,
					       gpointer callback_data)
{
	/* Do nothing here, we don't have any pending calls */
}

static gboolean
search_directory_file_check_if_ready (AnchorFile *file,
				      AnchorFileAttributes attributes)
{
	return TRUE;
}

static gboolean
search_directory_file_get_item_count (AnchorFile *file, 
				      guint *count,
				      gboolean *count_unreadable)
{
	GList *file_list;

	if (count) {
		file_list = anchor_directory_get_file_list (file->details->directory);

		*count = g_list_length (file_list);

		anchor_file_list_free (file_list);
	}

	return TRUE;
}

static AnchorRequestStatus
search_directory_file_get_deep_counts (AnchorFile *file,
				       guint *directory_count,
				       guint *file_count,
				       guint *unreadable_directory_count,
				       goffset *total_size)
{
	AnchorFile *dir_file;
	GList *file_list, *l;
	guint dirs, files;
	GFileType type;

	file_list = anchor_directory_get_file_list (file->details->directory);

	dirs = files = 0;
	for (l = file_list; l != NULL; l = l->next) {
		dir_file = NAUTILUS_FILE (l->data);
		type = anchor_file_get_file_type (dir_file);
		if (type == G_FILE_TYPE_DIRECTORY) {
			dirs++;
		} else {
			files++;
		}
	}

	if (directory_count != NULL) {
		*directory_count = dirs;
	}
	if (file_count != NULL) {
		*file_count = files;
	}
	if (unreadable_directory_count != NULL) {
		*unreadable_directory_count = 0;
	}
	if (total_size != NULL) {
		/* FIXME: Maybe we want to calculate this? */
		*total_size = 0;
	}
	
	anchor_file_list_free (file_list);
	
	return NAUTILUS_REQUEST_DONE;
}

static char *
search_directory_file_get_where_string (AnchorFile *file)
{
	return g_strdup (_("Search"));
}

static void
search_directory_file_set_metadata (AnchorFile *file,
                                    const char *key,
                                    const char *value)
{
	AnchorSearchDirectoryFile *search_file;

	search_file = NAUTILUS_SEARCH_DIRECTORY_FILE (file);
	anchor_keyfile_metadata_set_string (file,
	                                      search_file->details->metadata_filename,
	                                      "directory", key, value);
}

static void
search_directory_file_set_metadata_as_list (AnchorFile *file,
                                            const char *key,
                                            char **value)
{
	AnchorSearchDirectoryFile *search_file;

	search_file = NAUTILUS_SEARCH_DIRECTORY_FILE (file);
	anchor_keyfile_metadata_set_stringv (file,
	                                       search_file->details->metadata_filename,
	                                       "directory", key, (const gchar **) value);
}

void
anchor_search_directory_file_update_display_name (AnchorSearchDirectoryFile *search_file)
{
	AnchorFile *file;
	AnchorSearchDirectory *search_dir;
	AnchorQuery *query;
	char *display_name;
	gboolean changed;

	
	display_name = NULL;
	file = NAUTILUS_FILE (search_file);
	if (file->details->directory) {
		search_dir = NAUTILUS_SEARCH_DIRECTORY (file->details->directory);
		query = anchor_search_directory_get_query (search_dir);
	
		if (query != NULL) {
			display_name = anchor_query_to_readable_string (query);
			g_object_unref (query);
		} 
	}

	if (display_name == NULL) {
		display_name = g_strdup (_("Search"));
	}

	changed = anchor_file_set_display_name (file, display_name, NULL, TRUE);
	if (changed) {
		anchor_file_emit_changed (file);
	}

	g_free (display_name);
}

static void
anchor_search_directory_file_init (AnchorSearchDirectoryFile *search_file)
{
	AnchorFile *file;
	gchar *xdg_dir;

	file = NAUTILUS_FILE (search_file);

	search_file->details = G_TYPE_INSTANCE_GET_PRIVATE (search_file,
	                                                    ANCHOR_TYPE_SEARCH_DIRECTORY_FILE,
	                                                    AnchorSearchDirectoryFileDetails);

	xdg_dir = anchor_get_user_directory ();
	search_file->details->metadata_filename = g_build_filename (xdg_dir,
	                                                            "search-metadata",
	                                                            NULL);
	g_free (xdg_dir);

	file->details->got_file_info = TRUE;
	file->details->mime_type = eel_ref_str_get_unique ("x-directory/normal");
	file->details->type = G_FILE_TYPE_DIRECTORY;
	file->details->size = 0;

	file->details->file_info_is_up_to_date = TRUE;

	file->details->custom_icon = NULL;
	file->details->activation_uri = NULL;
	file->details->got_link_info = TRUE;
	file->details->link_info_is_up_to_date = TRUE;

	file->details->directory_count = 0;
	file->details->got_directory_count = TRUE;
	file->details->directory_count_is_up_to_date = TRUE;

	anchor_file_set_display_name (file, _("Search"), NULL, TRUE);
}

static void
anchor_search_directory_file_finalize (GObject *object)
{
	AnchorSearchDirectoryFile *search_file;

	search_file = NAUTILUS_SEARCH_DIRECTORY_FILE (object);

	g_free (search_file->details->metadata_filename);

	G_OBJECT_CLASS (anchor_search_directory_file_parent_class)->finalize (object);
}

static void
anchor_search_directory_file_class_init (AnchorSearchDirectoryFileClass *klass)
{
	GObjectClass *object_class;
	AnchorFileClass *file_class;

	object_class = G_OBJECT_CLASS (klass);
	file_class = NAUTILUS_FILE_CLASS (klass);

	object_class->finalize = anchor_search_directory_file_finalize;

	file_class->default_file_type = G_FILE_TYPE_DIRECTORY;

	file_class->monitor_add = search_directory_file_monitor_add;
	file_class->monitor_remove = search_directory_file_monitor_remove;
	file_class->call_when_ready = search_directory_file_call_when_ready;
	file_class->cancel_call_when_ready = search_directory_file_cancel_call_when_ready;
	file_class->check_if_ready = search_directory_file_check_if_ready;
	file_class->get_item_count = search_directory_file_get_item_count;
	file_class->get_deep_counts = search_directory_file_get_deep_counts;
	file_class->get_where_string = search_directory_file_get_where_string;
	file_class->set_metadata = search_directory_file_set_metadata;
	file_class->set_metadata_as_list = search_directory_file_set_metadata_as_list;

	g_type_class_add_private (object_class, sizeof(AnchorSearchDirectoryFileDetails));
}
