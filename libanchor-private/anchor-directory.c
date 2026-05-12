/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-directory.c: Anchor directory model.
 
   Copyright (C) 1999, 2000, 2001 Eazel, Inc.
  
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
  
   Author: Darin Adler <darin@bentspoon.com>
*/

#include <config.h>
#include "anchor-directory-private.h"

#include "anchor-directory-notify.h"
#include "anchor-file-attributes.h"
#include "anchor-file-private.h"
#include "anchor-file-utilities.h"
#include "anchor-search-directory.h"
#include "anchor-global-preferences.h"
#include "anchor-lib-self-check-functions.h"
#include "anchor-metadata.h"
#include "anchor-profile.h"
#include "anchor-desktop-directory.h"
#include "anchor-vfs-directory.h"
#include <eel/eel-glib-extensions.h>
#include <eel/eel-string.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

enum {
	FILES_ADDED,
	FILES_CHANGED,
	DONE_LOADING,
	LOAD_ERROR,
	LAST_SIGNAL
};

enum {
	PROP_LOCATION = 1,
	NUM_PROPERTIES
};

static guint signals[LAST_SIGNAL];
static GParamSpec *properties[NUM_PROPERTIES] = { NULL, };

static GHashTable *directories;

static void               anchor_directory_finalize         (GObject                *object);
static AnchorDirectory *anchor_directory_new              (GFile                  *location);
static GList *            real_get_file_list                  (AnchorDirectory      *directory);
static gboolean		  real_is_editable                    (AnchorDirectory      *directory);
static void               set_directory_location              (AnchorDirectory      *directory,
							       GFile                  *location);

G_DEFINE_TYPE (AnchorDirectory, anchor_directory, G_TYPE_OBJECT);

static void
anchor_directory_set_property (GObject *object,
				 guint property_id,
				 const GValue *value,
				 GParamSpec *pspec)
{
	AnchorDirectory *directory = NAUTILUS_DIRECTORY (object);

	switch (property_id) {
	case PROP_LOCATION:
		set_directory_location (directory, g_value_get_object (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void
anchor_directory_get_property (GObject *object,
				 guint property_id,
				 GValue *value,
				 GParamSpec *pspec)
{
	AnchorDirectory *directory = NAUTILUS_DIRECTORY (object);

	switch (property_id) {
	case PROP_LOCATION:
		g_value_set_object (value, directory->details->location);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void
anchor_directory_class_init (AnchorDirectoryClass *klass)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (klass);
	
	object_class->finalize = anchor_directory_finalize;
	object_class->set_property = anchor_directory_set_property;
	object_class->get_property = anchor_directory_get_property;

	signals[FILES_ADDED] =
		g_signal_new ("files-added",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (AnchorDirectoryClass, files_added),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__POINTER,
		              G_TYPE_NONE, 1, G_TYPE_POINTER);
	signals[FILES_CHANGED] =
		g_signal_new ("files-changed",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (AnchorDirectoryClass, files_changed),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__POINTER,
		              G_TYPE_NONE, 1, G_TYPE_POINTER);
	signals[DONE_LOADING] =
		g_signal_new ("done-loading",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (AnchorDirectoryClass, done_loading),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__VOID,
		              G_TYPE_NONE, 0);
	signals[LOAD_ERROR] =
		g_signal_new ("load-error",
		              G_TYPE_FROM_CLASS (object_class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (AnchorDirectoryClass, load_error),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__POINTER,
		              G_TYPE_NONE, 1, G_TYPE_POINTER);

	properties[PROP_LOCATION] =
		g_param_spec_object ("location",
				     "The location",
				     "The location of this directory",
				     G_TYPE_FILE,
				     G_PARAM_CONSTRUCT_ONLY | G_PARAM_READWRITE);

	klass->get_file_list = real_get_file_list;
	klass->is_editable = real_is_editable;

	g_type_class_add_private (klass, sizeof (AnchorDirectoryDetails));
	g_object_class_install_properties (object_class, NUM_PROPERTIES, properties);
}

static void
anchor_directory_init (AnchorDirectory *directory)
{
	directory->details = G_TYPE_INSTANCE_GET_PRIVATE ((directory), ANCHOR_TYPE_DIRECTORY, AnchorDirectoryDetails);
	directory->details->file_hash = g_hash_table_new (g_str_hash, g_str_equal);
	directory->details->high_priority_queue = anchor_file_queue_new ();
	directory->details->low_priority_queue = anchor_file_queue_new ();
	directory->details->extension_queue = anchor_file_queue_new ();
}

AnchorDirectory *
anchor_directory_ref (AnchorDirectory *directory)
{
	if (directory == NULL) {
		return directory;
	}

	g_return_val_if_fail (NAUTILUS_IS_DIRECTORY (directory), NULL);

	g_object_ref (directory);
	return directory;
}

void
anchor_directory_unref (AnchorDirectory *directory)
{
	if (directory == NULL) {
		return;
	}

	g_return_if_fail (NAUTILUS_IS_DIRECTORY (directory));

	g_object_unref (directory);
}

static void
anchor_directory_finalize (GObject *object)
{
	AnchorDirectory *directory;

	directory = NAUTILUS_DIRECTORY (object);

	g_hash_table_remove (directories, directory->details->location);

	anchor_directory_cancel (directory);
	g_assert (directory->details->count_in_progress == NULL);
	g_assert (directory->details->top_left_read_state == NULL);

	if (directory->details->monitor_list != NULL) {
		g_warning ("destroying a AnchorDirectory while it's being monitored");
		g_list_free_full (directory->details->monitor_list, g_free);
	}

	if (directory->details->monitor != NULL) {
		anchor_monitor_cancel (directory->details->monitor);
	}

	if (directory->details->dequeue_pending_idle_id != 0) {
		g_source_remove (directory->details->dequeue_pending_idle_id);
	}

	if (directory->details->call_ready_idle_id != 0) {
		g_source_remove (directory->details->call_ready_idle_id);
	}

	if (directory->details->location) {
		g_object_unref (directory->details->location);
	}

	g_assert (directory->details->file_list == NULL);
	g_hash_table_destroy (directory->details->file_hash);

	anchor_file_queue_destroy (directory->details->high_priority_queue);
	anchor_file_queue_destroy (directory->details->low_priority_queue);
	anchor_file_queue_destroy (directory->details->extension_queue);
	g_assert (directory->details->directory_load_in_progress == NULL);
	g_assert (directory->details->count_in_progress == NULL);
	g_assert (directory->details->dequeue_pending_idle_id == 0);
	g_list_free_full (directory->details->pending_file_info, g_object_unref);

	G_OBJECT_CLASS (anchor_directory_parent_class)->finalize (object);
}

static void
collect_all_directories (gpointer key, gpointer value, gpointer callback_data)
{
	AnchorDirectory *directory;
	GList **dirs;

	directory = NAUTILUS_DIRECTORY (value);
	dirs = callback_data;

	*dirs = g_list_prepend (*dirs, anchor_directory_ref (directory));
}

static void
filtering_changed_callback (gpointer callback_data)
{
	GList *dirs, *l;
	AnchorDirectory *directory;

	g_assert (callback_data == NULL);

	dirs = NULL;
	g_hash_table_foreach (directories, collect_all_directories, &dirs);

	/* Preference about which items to show has changed, so we
	 * can't trust any of our precomputed directory counts.
	 */
	for (l = dirs; l != NULL; l = l->next) {
		directory = NAUTILUS_DIRECTORY (l->data);
		anchor_directory_invalidate_count_and_mime_list (directory);
	}

	anchor_directory_list_unref (dirs);
}

void
emit_change_signals_for_all_files (AnchorDirectory *directory)
{
	GList *files;

	files = g_list_copy (directory->details->file_list);
	if (directory->details->as_file != NULL) {
		files = g_list_prepend (files, directory->details->as_file);
	}

	anchor_file_list_ref (files);
	anchor_directory_emit_change_signals (directory, files);

	anchor_file_list_free (files);
}

void
emit_change_signals_for_all_files_in_all_directories (void)
{
	GList *dirs, *l;
	AnchorDirectory *directory;

	dirs = NULL;
	g_hash_table_foreach (directories,
			      collect_all_directories,
			      &dirs);

	for (l = dirs; l != NULL; l = l->next) {
		directory = NAUTILUS_DIRECTORY (l->data);
		emit_change_signals_for_all_files (directory);
		anchor_directory_unref (directory);
	}

	g_list_free (dirs);
}

static void
async_state_changed_one (gpointer key, gpointer value, gpointer user_data)
{
	AnchorDirectory *directory;

	g_assert (key != NULL);
	g_assert (NAUTILUS_IS_DIRECTORY (value));
	g_assert (user_data == NULL);

	directory = NAUTILUS_DIRECTORY (value);
	
	anchor_directory_async_state_changed (directory);
	emit_change_signals_for_all_files (directory);
}

static void
async_data_preference_changed_callback (gpointer callback_data)
{
	g_assert (callback_data == NULL);

	/* Preference involving fetched async data has changed, so
	 * we have to kick off refetching all async data, and tell
	 * each file that it (might have) changed.
	 */
	g_hash_table_foreach (directories, async_state_changed_one, NULL);
}

static void
add_preferences_callbacks (void)
{
	anchor_global_preferences_init ();

	g_signal_connect_swapped (gtk_filechooser_preferences,
				  "changed::" NAUTILUS_PREFERENCES_SHOW_HIDDEN_FILES,
				  G_CALLBACK(filtering_changed_callback),
				  NULL);
	g_signal_connect_swapped (anchor_preferences,
				  "changed::" NAUTILUS_PREFERENCES_SHOW_DIRECTORY_ITEM_COUNTS,
				  G_CALLBACK (async_data_preference_changed_callback),
				  NULL);
}

/**
 * anchor_directory_get_by_uri:
 * @uri: URI of directory to get.
 *
 * Get a directory given a uri.
 * Creates the appropriate subclass given the uri mappings.
 * Returns a referenced object, not a floating one. Unref when finished.
 * If two windows are viewing the same uri, the directory object is shared.
 */
AnchorDirectory *
anchor_directory_get_internal (GFile *location, gboolean create)
{
	AnchorDirectory *directory;
	
	/* Create the hash table first time through. */
	if (directories == NULL) {
		directories = g_hash_table_new (g_file_hash, (GCompareFunc) g_file_equal);
		add_preferences_callbacks ();
	}

	/* If the object is already in the hash table, look it up. */

	directory = g_hash_table_lookup (directories,
					 location);
	if (directory != NULL) {
		anchor_directory_ref (directory);
	} else if (create) {
		/* Create a new directory object instead. */
		directory = anchor_directory_new (location);
		if (directory == NULL) {
			return NULL;
		}

		/* Put it in the hash table. */
		g_hash_table_insert (directories,
				     directory->details->location,
				     directory);
	}

	return directory;
}

AnchorDirectory *
anchor_directory_get (GFile *location)
{
	if (location == NULL) {
    		return NULL;
	}

	return anchor_directory_get_internal (location, TRUE);
}

AnchorDirectory *
anchor_directory_get_existing (GFile *location)
{
	if (location == NULL) {
    		return NULL;
	}

	return anchor_directory_get_internal (location, FALSE);
}


AnchorDirectory *
anchor_directory_get_by_uri (const char *uri)
{
	AnchorDirectory *directory;
	GFile *location;

	if (uri == NULL) {
    		return NULL;
	}

	location = g_file_new_for_uri (uri);

	directory = anchor_directory_get_internal (location, TRUE);
	g_object_unref (location);
	return directory;
}

AnchorDirectory *
anchor_directory_get_for_file (AnchorFile *file)
{
	char *uri;
	AnchorDirectory *directory;

	g_return_val_if_fail (NAUTILUS_IS_FILE (file), NULL);

	uri = anchor_file_get_uri (file);
	directory = anchor_directory_get_by_uri (uri);
	g_free (uri);
	return directory;
}

/* Returns a reffed AnchorFile object for this directory.
 */
AnchorFile *
anchor_directory_get_corresponding_file (AnchorDirectory *directory)
{
	AnchorFile *file;
	char *uri;

	file = anchor_directory_get_existing_corresponding_file (directory);
	if (file == NULL) {
		uri = anchor_directory_get_uri (directory);
		file = anchor_file_get_by_uri (uri);
		g_free (uri);
	}

	return file;
}

/* Returns a reffed AnchorFile object for this directory, but only if the
 * AnchorFile object has already been created.
 */
AnchorFile *
anchor_directory_get_existing_corresponding_file (AnchorDirectory *directory)
{
	AnchorFile *file;
	char *uri;
	
	file = directory->details->as_file;
	if (file != NULL) {
		anchor_file_ref (file);
		return file;
	}

	uri = anchor_directory_get_uri (directory);
	file = anchor_file_get_existing_by_uri (uri);
	g_free (uri);
	return file;
}

/* anchor_directory_get_name_for_self_as_new_file:
 * 
 * Get a name to display for the file representing this
 * directory. This is called only when there's no VFS
 * directory for this AnchorDirectory.
 */
char *
anchor_directory_get_name_for_self_as_new_file (AnchorDirectory *directory)
{
	GFile *file;
	char *directory_uri;
	char *scheme;
	char *name;
	char *hostname = NULL;

	directory_uri = anchor_directory_get_uri (directory);
	file = g_file_new_for_uri (directory_uri);
	scheme = g_file_get_uri_scheme (file);
	g_object_unref (file);

	anchor_uri_parse (directory_uri, &hostname, NULL, NULL);
	if (hostname == NULL || (strlen (hostname) == 0)) {
		name = g_strdup (directory_uri);
	} else if (scheme == NULL) {
		name = g_strdup (hostname);
	} else {
		/* Translators: this is of the format "hostname (uri-scheme)" */
		name = g_strdup_printf (_("%s (%s)"), hostname, scheme);
	}

	g_free (directory_uri);
	g_free (scheme);
	g_free (hostname);

	return name;
}

char *
anchor_directory_get_uri (AnchorDirectory *directory)
{
	g_return_val_if_fail (NAUTILUS_IS_DIRECTORY (directory), NULL);

	return g_file_get_uri (directory->details->location);
}

GFile *
anchor_directory_get_location (AnchorDirectory  *directory)
{
	g_return_val_if_fail (NAUTILUS_IS_DIRECTORY (directory), NULL);

	return g_object_ref (directory->details->location);
}

static AnchorDirectory *
anchor_directory_new (GFile *location)
{
	AnchorDirectory *directory;
	GType type;
	char *uri;
	gboolean is_saved_search;

	uri = g_file_get_uri (location);
	is_saved_search = g_str_has_suffix (uri, NAUTILUS_SAVED_SEARCH_EXTENSION);

	if (eel_uri_is_desktop (uri)) {
		type = ANCHOR_TYPE_DESKTOP_DIRECTORY;
	} else if (eel_uri_is_search (uri) || is_saved_search) {
		type = ANCHOR_TYPE_SEARCH_DIRECTORY;
	} else {
		type = ANCHOR_TYPE_VFS_DIRECTORY;
	}

	g_free (uri);

	directory = g_object_new (type, "location", location, NULL);
	if (is_saved_search) {
		anchor_search_directory_set_saved_search (NAUTILUS_SEARCH_DIRECTORY (directory), location);
	}

	return directory;
}

gboolean
anchor_directory_is_local (AnchorDirectory *directory)
{
	g_return_val_if_fail (NAUTILUS_IS_DIRECTORY (directory), FALSE);
	
	if (directory->details->location == NULL) {
		return TRUE;
	}

	return anchor_directory_is_in_trash (directory) ||
	       g_file_is_native (directory->details->location);
}

gboolean
anchor_directory_is_in_trash (AnchorDirectory *directory)
{
	g_assert (NAUTILUS_IS_DIRECTORY (directory));
	
	if (directory->details->location == NULL) {
		return FALSE;
	}

	return g_file_has_uri_scheme (directory->details->location, "trash");
}

gboolean
anchor_directory_is_in_recent (AnchorDirectory *directory)
{
	g_assert (NAUTILUS_IS_DIRECTORY (directory));

	if (directory->details->location == NULL) {
		return FALSE;
	}

	return g_file_has_uri_scheme (directory->details->location, "recent");
}

gboolean
anchor_directory_is_in_network (AnchorDirectory *directory)
{
	g_assert (NAUTILUS_IS_DIRECTORY (directory));

	if (directory->details->location == NULL) {
		return FALSE;
	}

	return g_file_has_uri_scheme (directory->details->location, "network") ||
		g_file_has_uri_scheme (directory->details->location, "dns-sd");
}

gboolean
anchor_directory_are_all_files_seen (AnchorDirectory *directory)
{
	g_return_val_if_fail (NAUTILUS_IS_DIRECTORY (directory), FALSE);

	return NAUTILUS_DIRECTORY_CLASS (G_OBJECT_GET_CLASS (directory))->are_all_files_seen (directory);
}

static void
add_to_hash_table (AnchorDirectory *directory, AnchorFile *file, GList *node)
{
	const char *name;

	name = eel_ref_str_peek (file->details->name);

	g_assert (node != NULL);
	g_assert (g_hash_table_lookup (directory->details->file_hash,
				       name) == NULL);
	g_hash_table_insert (directory->details->file_hash, (char *) name, node);
}

static GList *
extract_from_hash_table (AnchorDirectory *directory, AnchorFile *file)
{
	const char *name;
	GList *node;

	name = eel_ref_str_peek (file->details->name);
	if (name == NULL) {
		return NULL;
	}

	/* Find the list node in the hash table. */
	node = g_hash_table_lookup (directory->details->file_hash, name);
	g_hash_table_remove (directory->details->file_hash, name);

	return node;
}

void
anchor_directory_add_file (AnchorDirectory *directory, AnchorFile *file)
{
	GList *node;
	gboolean add_to_work_queue;

	g_assert (NAUTILUS_IS_DIRECTORY (directory));
	g_assert (NAUTILUS_IS_FILE (file));
	g_assert (file->details->name != NULL);

	/* Add to list. */
	node = g_list_prepend (directory->details->file_list, file);
	directory->details->file_list = node;

	/* Add to hash table. */
	add_to_hash_table (directory, file, node);

	directory->details->confirmed_file_count++;

	add_to_work_queue = FALSE;
	if (anchor_directory_is_file_list_monitored (directory)) {
		/* Ref if we are monitoring, since monitoring owns the file list. */
		anchor_file_ref (file);
		add_to_work_queue = TRUE;
	} else if (anchor_directory_has_active_request_for_file (directory, file)) {
		/* We're waiting for the file in a call_when_ready. Make sure
		   we add the file to the work queue so that said waiter won't
		   wait forever for e.g. all files in the directory to be done */
		add_to_work_queue = TRUE;
	}
	
	if (add_to_work_queue) {
		anchor_directory_add_file_to_work_queue (directory, file);
	}
}

void
anchor_directory_remove_file (AnchorDirectory *directory, AnchorFile *file)
{
	GList *node;

	g_assert (NAUTILUS_IS_DIRECTORY (directory));
	g_assert (NAUTILUS_IS_FILE (file));
	g_assert (file->details->name != NULL);

	/* Find the list node in the hash table. */
	node = extract_from_hash_table (directory, file);
	g_assert (node != NULL);
	g_assert (node->data == file);

	/* Remove the item from the list. */
	directory->details->file_list = g_list_remove_link
		(directory->details->file_list, node);
	g_list_free_1 (node);

	anchor_directory_remove_file_from_work_queue (directory, file);

	if (!file->details->unconfirmed) {
		directory->details->confirmed_file_count--;
	}

	/* Unref if we are monitoring. */
	if (anchor_directory_is_file_list_monitored (directory)) {
		anchor_file_unref (file);
	}
}

GList *
anchor_directory_begin_file_name_change (AnchorDirectory *directory,
					   AnchorFile *file)
{
	/* Find the list node in the hash table. */
	return extract_from_hash_table (directory, file);
}

void
anchor_directory_end_file_name_change (AnchorDirectory *directory,
					 AnchorFile *file,
					 GList *node)
{
	/* Add the list node to the hash table. */
	if (node != NULL) {
		add_to_hash_table (directory, file, node);
	}
}

AnchorFile *
anchor_directory_find_file_by_name (AnchorDirectory *directory,
				      const char *name)
{
	GList *node;

	g_return_val_if_fail (NAUTILUS_IS_DIRECTORY (directory), NULL);
	g_return_val_if_fail (name != NULL, NULL);

	node = g_hash_table_lookup (directory->details->file_hash,
				    name);
	return node == NULL ? NULL : NAUTILUS_FILE (node->data);
}

/* "." for the directory-as-file, otherwise the filename */
AnchorFile *
anchor_directory_find_file_by_internal_filename (AnchorDirectory *directory,
						   const char *internal_filename)
{
	AnchorFile *result;

	if (g_strcmp0 (internal_filename, ".") == 0) {
		result = anchor_directory_get_existing_corresponding_file (directory);
		if (result != NULL) {
			anchor_file_unref (result);
		}
	} else {
		result = anchor_directory_find_file_by_name (directory, internal_filename);
	}

	return result;
}

void
anchor_directory_emit_files_added (AnchorDirectory *directory,
				     GList *added_files)
{
	anchor_profile_start (NULL);
	if (added_files != NULL) {
		g_signal_emit (directory,
				 signals[FILES_ADDED], 0,
				 added_files);
	}
	anchor_profile_end (NULL);
}

void
anchor_directory_emit_files_changed (AnchorDirectory *directory,
				       GList *changed_files)
{
	anchor_profile_start (NULL);
	if (changed_files != NULL) {
		g_signal_emit (directory,
				 signals[FILES_CHANGED], 0,
				 changed_files);
	}
	anchor_profile_end (NULL);
}

void
anchor_directory_emit_change_signals (AnchorDirectory *directory,
					     GList *changed_files)
{
	GList *p;

	anchor_profile_start (NULL);
	for (p = changed_files; p != NULL; p = p->next) {
		anchor_file_emit_changed (p->data);
	}
	anchor_directory_emit_files_changed (directory, changed_files);
	anchor_profile_end (NULL);
}

void
anchor_directory_emit_done_loading (AnchorDirectory *directory)
{
	g_signal_emit (directory,
			 signals[DONE_LOADING], 0);
}

void
anchor_directory_emit_load_error (AnchorDirectory *directory,
				    GError *error)
{
	g_signal_emit (directory,
			 signals[LOAD_ERROR], 0,
			 error);
}

/* Return a directory object for this one's parent. */
static AnchorDirectory *
get_parent_directory (GFile *location)
{
	AnchorDirectory *directory;
	GFile *parent;

	parent = g_file_get_parent (location);
	if (parent) {
		directory = anchor_directory_get_internal (parent, TRUE);
		g_object_unref (parent);
		return directory;
	}
	return NULL;
}

/* If a directory object exists for this one's parent, then
 * return it, otherwise return NULL.
 */
static AnchorDirectory *
get_parent_directory_if_exists (GFile *location)
{
	AnchorDirectory *directory;
	GFile *parent;

	parent = g_file_get_parent (location);
	if (parent) {
		directory = anchor_directory_get_internal (parent, FALSE);
		g_object_unref (parent);
		return directory;
	}
	return NULL;
}

static void
hash_table_list_prepend (GHashTable *table, gconstpointer key, gpointer data)
{
	GList *list;

	list = g_hash_table_lookup (table, key);
	list = g_list_prepend (list, data);
	g_hash_table_insert (table, (gpointer) key, list);
}

static void
call_files_added_free_list (gpointer key, gpointer value, gpointer user_data)
{
	g_assert (NAUTILUS_IS_DIRECTORY (key));
	g_assert (value != NULL);
	g_assert (user_data == NULL);

	g_signal_emit (key,
			 signals[FILES_ADDED], 0,
			 value);
	g_list_free (value);
}

static void
call_files_changed_common (AnchorDirectory *directory, GList *file_list)
{
	GList *node;
	AnchorFile *file;

	for (node = file_list; node != NULL; node = node->next) {
		file = node->data;
		if (file->details->directory == directory) {
			anchor_directory_add_file_to_work_queue (directory, 
								   file);
		}
	}
	anchor_directory_async_state_changed (directory);
	anchor_directory_emit_change_signals (directory, file_list);
}

static void
call_files_changed_free_list (gpointer key, gpointer value, gpointer user_data)
{
	g_assert (value != NULL);
	g_assert (user_data == NULL);

	call_files_changed_common (NAUTILUS_DIRECTORY (key), value);
	g_list_free (value);
}

static void
call_files_changed_unref_free_list (gpointer key, gpointer value, gpointer user_data)
{
	g_assert (value != NULL);
	g_assert (user_data == NULL);

	call_files_changed_common (NAUTILUS_DIRECTORY (key), value);
	anchor_file_list_free (value);
}

static void
call_get_file_info_free_list (gpointer key, gpointer value, gpointer user_data)
{
	AnchorDirectory *directory;
	GList *files;
	
	g_assert (NAUTILUS_IS_DIRECTORY (key));
	g_assert (value != NULL);
	g_assert (user_data == NULL);

	directory = key;
	files = value;
	
	anchor_directory_get_info_for_new_files (directory, files);
	g_list_foreach (files, (GFunc) g_object_unref, NULL);
	g_list_free (files);
}

static void
invalidate_count_and_unref (gpointer key, gpointer value, gpointer user_data)
{
	g_assert (NAUTILUS_IS_DIRECTORY (key));
	g_assert (value == key);
	g_assert (user_data == NULL);

	anchor_directory_invalidate_count_and_mime_list (key);
	anchor_directory_unref (key);
}

static void
collect_parent_directories (GHashTable *hash_table, AnchorDirectory *directory)
{
	g_assert (hash_table != NULL);
	g_assert (NAUTILUS_IS_DIRECTORY (directory));

	if (g_hash_table_lookup (hash_table, directory) == NULL) {
		anchor_directory_ref (directory);
		g_hash_table_insert  (hash_table, directory, directory);
	}
}

void
anchor_directory_notify_files_added (GList *files)
{
	GHashTable *added_lists;
	GList *p;
	AnchorDirectory *directory;
	GHashTable *parent_directories;
	AnchorFile *file;
	GFile *location, *parent;

	anchor_profile_start (NULL);

	/* Make a list of added files in each directory. */
	added_lists = g_hash_table_new (NULL, NULL);

	/* Make a list of parent directories that will need their counts updated. */
	parent_directories = g_hash_table_new (NULL, NULL);

	for (p = files; p != NULL; p = p->next) {
		location = p->data;

		/* See if the directory is already known. */
		directory = get_parent_directory_if_exists (location);
		if (directory == NULL) {
			/* In case the directory is not being
			 * monitored, but the corresponding file is,
			 * we must invalidate it's item count.
			 */


			file = NULL;
			parent = g_file_get_parent (location);
			if (parent) {
				file = anchor_file_get_existing (parent);
				g_object_unref (parent);
			}

			if (file != NULL) {
				anchor_file_invalidate_count_and_mime_list (file);
				anchor_file_unref (file);
			}

			continue;
		}

		collect_parent_directories (parent_directories, directory);

		/* If no one is monitoring files in the directory, nothing to do. */
		if (!anchor_directory_is_file_list_monitored (directory)) {
			anchor_directory_unref (directory);
			continue;
		}

		file = anchor_file_get_existing (location);
		/* We check is_added here, because the file could have been added
		 * to the directory by a anchor_file_get() but not gotten 
		 * files_added emitted
		 */
		if (file && file->details->is_added) {
			/* A file already exists, it was probably renamed.
			 * If it was renamed this could be ignored, but 
			 * queue a change just in case */
			anchor_file_changed (file);
			anchor_file_unref (file);
		} else {
			hash_table_list_prepend (added_lists, 
						 directory, 
						 g_object_ref (location));
		}
		anchor_directory_unref (directory);
	}

	/* Now get file info for the new files. This creates AnchorFile
	 * objects for the new files, and sends out a files_added signal. 
	 */
	g_hash_table_foreach (added_lists, call_get_file_info_free_list, NULL);
	g_hash_table_destroy (added_lists);

	/* Invalidate count for each parent directory. */
	g_hash_table_foreach (parent_directories, invalidate_count_and_unref, NULL);
	g_hash_table_destroy (parent_directories);

	anchor_profile_end (NULL);
}

static void
g_file_pair_free (GFilePair *pair)
{
	g_object_unref (pair->to);
	g_object_unref (pair->from);
	g_free (pair);
}

static GList *
uri_pairs_to_file_pairs (GList *uri_pairs)
{
	GList *l, *file_pair_list;
	GFilePair *file_pair;
	URIPair *uri_pair;
	
	file_pair_list = NULL;

	for (l = uri_pairs; l != NULL; l = l->next) {
		uri_pair = l->data;
		file_pair = g_new (GFilePair, 1);
		file_pair->from = g_file_new_for_uri (uri_pair->from_uri);
		file_pair->to = g_file_new_for_uri (uri_pair->to_uri);
		
		file_pair_list = g_list_prepend (file_pair_list, file_pair);
	}
	return g_list_reverse (file_pair_list);
}

void
anchor_directory_notify_files_added_by_uri (GList *uris)
{
	GList *files;

	files = anchor_file_list_from_uris (uris);
	anchor_directory_notify_files_added (files);
	g_list_free_full (files, g_object_unref);
}

void
anchor_directory_notify_files_changed (GList *files)
{
	GHashTable *changed_lists;
	GList *node;
	GFile *location;
	AnchorFile *file;

	/* Make a list of changed files in each directory. */
	changed_lists = g_hash_table_new (NULL, NULL);

	/* Go through all the notifications. */
	for (node = files; node != NULL; node = node->next) {
		location = node->data;

		/* Find the file. */
		file = anchor_file_get_existing (location);
		if (file != NULL) {
			/* Tell it to re-get info now, and later emit
			 * a changed signal.
			 */
			file->details->file_info_is_up_to_date = FALSE;
			file->details->top_left_text_is_up_to_date = FALSE;
			file->details->link_info_is_up_to_date = FALSE;
			anchor_file_invalidate_extension_info_internal (file);

			hash_table_list_prepend (changed_lists,
						 file->details->directory,
						 file);
		}
	}

	/* Now send out the changed signals. */
	g_hash_table_foreach (changed_lists, call_files_changed_unref_free_list, NULL);
	g_hash_table_destroy (changed_lists);
}

void
anchor_directory_notify_files_changed_by_uri (GList *uris)
{
	GList *files;

	files = anchor_file_list_from_uris (uris);
	anchor_directory_notify_files_changed (files);
	g_list_free_full (files, g_object_unref);
}

void
anchor_directory_notify_files_removed (GList *files)
{
	GHashTable *changed_lists;
	GList *p;
	AnchorDirectory *directory;
	GHashTable *parent_directories;
	AnchorFile *file;
	GFile *location;

	/* Make a list of changed files in each directory. */
	changed_lists = g_hash_table_new (NULL, NULL);

	/* Make a list of parent directories that will need their counts updated. */
	parent_directories = g_hash_table_new (NULL, NULL);

	/* Go through all the notifications. */
	for (p = files; p != NULL; p = p->next) {
		location = p->data;

		/* Update file count for parent directory if anyone might care. */
		directory = get_parent_directory_if_exists (location);
		if (directory != NULL) {
			collect_parent_directories (parent_directories, directory);
			anchor_directory_unref (directory);
		}

		/* Find the file. */
		file = anchor_file_get_existing (location);
		if (file != NULL && !anchor_file_rename_in_progress (file)) {
			/* Mark it gone and prepare to send the changed signal. */
			anchor_file_mark_gone (file);
			hash_table_list_prepend (changed_lists,
						 file->details->directory,
						 anchor_file_ref (file));
		}
		anchor_file_unref (file);
	}

	/* Now send out the changed signals. */
	g_hash_table_foreach (changed_lists, call_files_changed_unref_free_list, NULL);
	g_hash_table_destroy (changed_lists);

	/* Invalidate count for each parent directory. */
	g_hash_table_foreach (parent_directories, invalidate_count_and_unref, NULL);
	g_hash_table_destroy (parent_directories);
}

void
anchor_directory_notify_files_removed_by_uri (GList *uris)
{
	GList *files;

	files = anchor_file_list_from_uris (uris);
	anchor_directory_notify_files_changed (files);
	g_list_free_full (files, g_object_unref);
}

static void
set_directory_location (AnchorDirectory *directory,
			GFile *location)
{
	if (directory->details->location) {
		g_object_unref (directory->details->location);
	}
	directory->details->location = g_object_ref (location);

	g_object_notify_by_pspec (G_OBJECT (directory), properties[PROP_LOCATION]);
}

static void
change_directory_location (AnchorDirectory *directory,
			   GFile *new_location)
{
	/* I believe it's impossible for a self-owned file/directory
	 * to be moved. But if that did somehow happen, this function
	 * wouldn't do enough to handle it.
	 */
	g_assert (directory->details->as_file == NULL);

	g_hash_table_remove (directories,
			     directory->details->location);

	set_directory_location (directory, new_location);

	g_hash_table_insert (directories,
			     directory->details->location,
			     directory);
}

typedef struct {
	GFile *container;
	GList *directories;
} CollectData;

static void
collect_directories_by_container (gpointer key, gpointer value, gpointer callback_data)
{
	AnchorDirectory *directory;
	CollectData *collect_data;
	GFile *location;

	location = (GFile *) key;
	directory = NAUTILUS_DIRECTORY (value);
	collect_data = (CollectData *) callback_data;

	if (g_file_has_prefix (location, collect_data->container) ||
	    g_file_equal (collect_data->container, location)) {
		anchor_directory_ref (directory);
		collect_data->directories =
			g_list_prepend (collect_data->directories,
					directory);
	}
}

static GList *
anchor_directory_moved_internal (GFile *old_location,
				   GFile *new_location)
{
	CollectData collection;
	AnchorDirectory *directory;
	GList *node, *affected_files;
	GFile *new_directory_location;
	char *relative_path;

	collection.container = old_location;
	collection.directories = NULL;

	g_hash_table_foreach (directories,
			      collect_directories_by_container,
			      &collection);

	affected_files = NULL;

	for (node = collection.directories; node != NULL; node = node->next) {
		directory = NAUTILUS_DIRECTORY (node->data);
		new_directory_location = NULL;

		if (g_file_equal (directory->details->location, old_location)) {
			new_directory_location = g_object_ref (new_location);
		} else {
			relative_path = g_file_get_relative_path (old_location,
								  directory->details->location);
			if (relative_path != NULL) {
				new_directory_location = g_file_resolve_relative_path (new_location, relative_path);
				g_free (relative_path);
				
			}
		}
		
		if (new_directory_location) {
			change_directory_location (directory, new_directory_location);
			g_object_unref (new_directory_location);
		
			/* Collect affected files. */
			if (directory->details->as_file != NULL) {
				affected_files = g_list_prepend
					(affected_files,
					 anchor_file_ref (directory->details->as_file));
			}
			affected_files = g_list_concat
				(affected_files,
				 anchor_file_list_copy (directory->details->file_list));
		}
		
		anchor_directory_unref (directory);
	}

	g_list_free (collection.directories);

	return affected_files;
}

void
anchor_directory_moved (const char *old_uri,
			  const char *new_uri)
{
	GList *list, *node;
	GHashTable *hash;
	AnchorFile *file;
	GFile *old_location;
	GFile *new_location;

	hash = g_hash_table_new (NULL, NULL);

	old_location = g_file_new_for_uri (old_uri);
	new_location = g_file_new_for_uri (new_uri);
	
	list = anchor_directory_moved_internal (old_location, new_location);
	for (node = list; node != NULL; node = node->next) {
		file = NAUTILUS_FILE (node->data);
		hash_table_list_prepend (hash,
					 file->details->directory,
					 anchor_file_ref (file));
	}
	anchor_file_list_free (list);
	
	g_object_unref (old_location);
	g_object_unref (new_location);

	g_hash_table_foreach (hash, call_files_changed_unref_free_list, NULL);
	g_hash_table_destroy (hash);
}

void
anchor_directory_notify_files_moved (GList *file_pairs)
{
	GList *p, *affected_files, *node;
	GFilePair *pair;
	AnchorFile *file;
	AnchorDirectory *old_directory, *new_directory;
	GHashTable *parent_directories;
	GList *new_files_list, *unref_list;
	GHashTable *added_lists, *changed_lists;
	char *name;
	AnchorFileAttributes cancel_attributes;
	GFile *to_location, *from_location;
	
	/* Make a list of added and changed files in each directory. */
	new_files_list = NULL;
	added_lists = g_hash_table_new (NULL, NULL);
	changed_lists = g_hash_table_new (NULL, NULL);
	unref_list = NULL;

	/* Make a list of parent directories that will need their counts updated. */
	parent_directories = g_hash_table_new (NULL, NULL);

	cancel_attributes = anchor_file_get_all_attributes ();

	for (p = file_pairs; p != NULL; p = p->next) {
		pair = p->data;
		from_location = pair->from;
		to_location = pair->to;

		/* Handle overwriting a file. */
		file = anchor_file_get_existing (to_location);
		if (file != NULL) {
			/* Mark it gone and prepare to send the changed signal. */
			anchor_file_mark_gone (file);
			new_directory = file->details->directory;
			hash_table_list_prepend (changed_lists,
						 new_directory,
						 file);
			collect_parent_directories (parent_directories,
						    new_directory);
		}

		/* Update any directory objects that are affected. */
		affected_files = anchor_directory_moved_internal (from_location,
								    to_location);
		for (node = affected_files; node != NULL; node = node->next) {
			file = NAUTILUS_FILE (node->data);
			hash_table_list_prepend (changed_lists,
						 file->details->directory,
						 file);
		}
		unref_list = g_list_concat (unref_list, affected_files);

		/* Move an existing file. */
		file = anchor_file_get_existing (from_location);
		if (file == NULL) {
			/* Handle this as if it was a new file. */
			new_files_list = g_list_prepend (new_files_list,
							 to_location);
		} else {
			/* Handle notification in the old directory. */
			old_directory = file->details->directory;
			collect_parent_directories (parent_directories, old_directory);

			/* Cancel loading of attributes in the old directory */
			anchor_directory_cancel_loading_file_attributes
				(old_directory, file, cancel_attributes);

			/* Locate the new directory. */
			new_directory = get_parent_directory (to_location);
			collect_parent_directories (parent_directories, new_directory);
			/* We can unref now -- new_directory is in the
			 * parent directories list so it will be
			 * around until the end of this function
			 * anyway.
			 */
			anchor_directory_unref (new_directory);

			/* Update the file's name and directory. */
			name = g_file_get_basename (to_location);
			anchor_file_update_name_and_directory 
				(file, name, new_directory);
			g_free (name);

			/* Update file attributes */
			anchor_file_invalidate_attributes (file, NAUTILUS_FILE_ATTRIBUTE_INFO);

			hash_table_list_prepend (changed_lists,
						 old_directory,
						 file);
			if (old_directory != new_directory) {
				hash_table_list_prepend	(added_lists,
							 new_directory,
							 file);
			}

			/* Unref each file once to balance out anchor_file_get_by_uri. */
			unref_list = g_list_prepend (unref_list, file);
		}
	}

	/* Now send out the changed and added signals for existing file objects. */
	g_hash_table_foreach (changed_lists, call_files_changed_free_list, NULL);
	g_hash_table_destroy (changed_lists);
	g_hash_table_foreach (added_lists, call_files_added_free_list, NULL);
	g_hash_table_destroy (added_lists);

	/* Let the file objects go. */
	anchor_file_list_free (unref_list);

	/* Invalidate count for each parent directory. */
	g_hash_table_foreach (parent_directories, invalidate_count_and_unref, NULL);
	g_hash_table_destroy (parent_directories);

	/* Separate handling for brand new file objects. */
	anchor_directory_notify_files_added (new_files_list);
	g_list_free (new_files_list);
}

void
anchor_directory_notify_files_moved_by_uri (GList *uri_pairs)
{
	GList *file_pairs;

	file_pairs = uri_pairs_to_file_pairs (uri_pairs);
	anchor_directory_notify_files_moved (file_pairs);
	g_list_foreach (file_pairs, (GFunc)g_file_pair_free, NULL);
	g_list_free (file_pairs);
}

void
anchor_directory_schedule_position_set (GList *position_setting_list)
{
	GList *p;
	const AnchorFileChangesQueuePosition *item;
	AnchorFile *file;
	char str[64];
	time_t now;

	time (&now);

	for (p = position_setting_list; p != NULL; p = p->next) {
		item = (AnchorFileChangesQueuePosition *) p->data;

		file = anchor_file_get (item->location);
		
		if (item->set) {
			g_snprintf (str, sizeof (str), "%d,%d", item->point.x, item->point.y);
		} else {
			str[0] = 0;
		}
		anchor_file_set_metadata
			(file,
			 NAUTILUS_METADATA_KEY_ICON_POSITION,
			 NULL,
			 str);

		if (item->set) {
			anchor_file_set_time_metadata
				(file,
				 NAUTILUS_METADATA_KEY_ICON_POSITION_TIMESTAMP,
				 now);
		} else {
			anchor_file_set_time_metadata
				(file,
				 NAUTILUS_METADATA_KEY_ICON_POSITION_TIMESTAMP,
				 UNDEFINED_TIME);
		}

		if (item->set) {
			g_snprintf (str, sizeof (str), "%d", item->screen);
		} else {
			str[0] = 0;
		}
		anchor_file_set_metadata
			(file,
			 NAUTILUS_METADATA_KEY_SCREEN,
			 NULL,
			 str);
		
		anchor_file_unref (file);
	}
}

gboolean
anchor_directory_contains_file (AnchorDirectory *directory,
				  AnchorFile *file)
{
	g_return_val_if_fail (NAUTILUS_IS_DIRECTORY (directory), FALSE);
	g_return_val_if_fail (NAUTILUS_IS_FILE (file), FALSE);

	if (anchor_file_is_gone (file)) {
		return FALSE;
	}

	return NAUTILUS_DIRECTORY_CLASS (G_OBJECT_GET_CLASS (directory))->contains_file (directory, file);
}

char *
anchor_directory_get_file_uri (AnchorDirectory *directory,
				 const char *file_name)
{
	GFile *child;
	char *result;

	g_return_val_if_fail (NAUTILUS_IS_DIRECTORY (directory), NULL);
	g_return_val_if_fail (file_name != NULL, NULL);

	result = NULL;

	child = g_file_get_child (directory->details->location, file_name);
	result = g_file_get_uri (child);
	g_object_unref (child);
	
	return result;
}

void
anchor_directory_call_when_ready (AnchorDirectory *directory,
				    AnchorFileAttributes file_attributes,
				    gboolean wait_for_all_files,
				    AnchorDirectoryCallback callback,
				    gpointer callback_data)
{
	g_return_if_fail (NAUTILUS_IS_DIRECTORY (directory));
	g_return_if_fail (callback != NULL);

	NAUTILUS_DIRECTORY_CLASS (G_OBJECT_GET_CLASS (directory))->call_when_ready 
		(directory, file_attributes, wait_for_all_files,
		 callback, callback_data);
}

void
anchor_directory_cancel_callback (AnchorDirectory *directory,
				    AnchorDirectoryCallback callback,
				    gpointer callback_data)
{
	g_return_if_fail (NAUTILUS_IS_DIRECTORY (directory));
	g_return_if_fail (callback != NULL);

	NAUTILUS_DIRECTORY_CLASS (G_OBJECT_GET_CLASS (directory))->cancel_callback 
		(directory, callback, callback_data);
}

void
anchor_directory_file_monitor_add (AnchorDirectory *directory,
				     gconstpointer client,
				     gboolean monitor_hidden_files,
				     AnchorFileAttributes file_attributes,
				     AnchorDirectoryCallback callback,
				     gpointer callback_data)
{
	g_return_if_fail (NAUTILUS_IS_DIRECTORY (directory));
	g_return_if_fail (client != NULL);

	NAUTILUS_DIRECTORY_CLASS (G_OBJECT_GET_CLASS (directory))->file_monitor_add 
		(directory, client,
		 monitor_hidden_files,
		 file_attributes,
		 callback, callback_data);
}

void
anchor_directory_file_monitor_remove (AnchorDirectory *directory,
					gconstpointer client)
{
	g_return_if_fail (NAUTILUS_IS_DIRECTORY (directory));
	g_return_if_fail (client != NULL);

	NAUTILUS_DIRECTORY_CLASS (G_OBJECT_GET_CLASS (directory))->file_monitor_remove
		(directory, client);
}

void
anchor_directory_force_reload (AnchorDirectory *directory)
{
	g_return_if_fail (NAUTILUS_IS_DIRECTORY (directory));

	NAUTILUS_DIRECTORY_CLASS (G_OBJECT_GET_CLASS (directory))->force_reload (directory);
}

gboolean
anchor_directory_is_not_empty (AnchorDirectory *directory)
{
	g_return_val_if_fail (NAUTILUS_IS_DIRECTORY (directory), FALSE);

	return NAUTILUS_DIRECTORY_CLASS (G_OBJECT_GET_CLASS (directory))->is_not_empty (directory);
}

static gboolean
is_tentative (gpointer data, gpointer callback_data)
{
	AnchorFile *file;

	g_assert (callback_data == NULL);

	file = NAUTILUS_FILE (data);
	/* Avoid returning files with !is_added, because these
	 * will later be sent with the files_added signal, and a
	 * user doing get_file_list + files_added monitoring will
	 * then see the file twice */
	return !file->details->got_file_info || !file->details->is_added;
}

GList *
anchor_directory_get_file_list (AnchorDirectory *directory)
{
	return NAUTILUS_DIRECTORY_CLASS (G_OBJECT_GET_CLASS (directory))->get_file_list (directory);
}

static GList *
real_get_file_list (AnchorDirectory *directory)
{
	GList *tentative_files, *non_tentative_files;

	tentative_files = eel_g_list_partition
		(g_list_copy (directory->details->file_list),
		 is_tentative, NULL, &non_tentative_files);
	g_list_free (tentative_files);

	anchor_file_list_ref (non_tentative_files);
	return non_tentative_files;
}

static gboolean
real_is_editable (AnchorDirectory *directory)
{
	return TRUE;
}

gboolean
anchor_directory_is_editable (AnchorDirectory *directory)
{
	return NAUTILUS_DIRECTORY_CLASS (G_OBJECT_GET_CLASS (directory))->is_editable (directory);
}

GList *
anchor_directory_match_pattern (AnchorDirectory *directory, const char *pattern)
{
	GList *files, *l, *ret;
	GPatternSpec *spec;


	ret = NULL;
	spec = g_pattern_spec_new (pattern);
	
	files = anchor_directory_get_file_list (directory);
	for (l = files; l; l = l->next) {
		AnchorFile *file;
		char *name;
	       
	        file = NAUTILUS_FILE (l->data);
		name = anchor_file_get_display_name (file);

		if (g_pattern_match_string (spec, name)) {
			ret = g_list_prepend(ret, anchor_file_ref (file));	
		}

		g_free (name);
	}

	g_pattern_spec_free (spec);
	anchor_file_list_free (files);

	return ret;
}

/**
 * anchor_directory_list_ref
 *
 * Ref all the directories in a list.
 * @list: GList of directories.
 **/
GList *
anchor_directory_list_ref (GList *list)
{
	g_list_foreach (list, (GFunc) anchor_directory_ref, NULL);
	return list;
}

/**
 * anchor_directory_list_unref
 *
 * Unref all the directories in a list.
 * @list: GList of directories.
 **/
void
anchor_directory_list_unref (GList *list)
{
	g_list_foreach (list, (GFunc) anchor_directory_unref, NULL);
}

/**
 * anchor_directory_list_free
 *
 * Free a list of directories after unrefing them.
 * @list: GList of directories.
 **/
void
anchor_directory_list_free (GList *list)
{
	anchor_directory_list_unref (list);
	g_list_free (list);
}

/**
 * anchor_directory_list_copy
 *
 * Copy the list of directories, making a new ref of each,
 * @list: GList of directories.
 **/
GList *
anchor_directory_list_copy (GList *list)
{
	return g_list_copy (anchor_directory_list_ref (list));
}

static int
compare_by_uri (AnchorDirectory *a, AnchorDirectory *b)
{
	char *uri_a, *uri_b;
	int res;

	uri_a = g_file_get_uri (a->details->location);
	uri_b = g_file_get_uri (b->details->location);
	
	res = strcmp (uri_a, uri_b);

	g_free (uri_a);
	g_free (uri_b);
	
	return res;
}

static int
compare_by_uri_cover (gconstpointer a, gconstpointer b)
{
	return compare_by_uri (NAUTILUS_DIRECTORY (a), NAUTILUS_DIRECTORY (b));
}

/**
 * anchor_directory_list_sort_by_uri
 * 
 * Sort the list of directories by directory uri.
 * @list: GList of directories.
 **/
GList *
anchor_directory_list_sort_by_uri (GList *list)
{
	return g_list_sort (list, compare_by_uri_cover);
}

gboolean
anchor_directory_is_desktop_directory (AnchorDirectory   *directory)
{
	if (directory->details->location == NULL) {
		return FALSE;
	}

	return anchor_is_desktop_directory (directory->details->location);
}

#if !defined (NAUTILUS_OMIT_SELF_CHECK)

#include <eel/eel-debug.h>
#include "anchor-file-attributes.h"

static int data_dummy;
static gboolean got_files_flag;

static void
got_files_callback (AnchorDirectory *directory, GList *files, gpointer callback_data)
{
	g_assert (NAUTILUS_IS_DIRECTORY (directory));
	g_assert (g_list_length (files) > 10);
	g_assert (callback_data == &data_dummy);

	got_files_flag = TRUE;
}

/* Return the number of extant AnchorDirectories */
int
anchor_directory_number_outstanding (void)
{
        return directories ? g_hash_table_size (directories) : 0;
}

void
anchor_self_check_directory (void)
{
	AnchorDirectory *directory;
	AnchorFile *file;

	directory = anchor_directory_get_by_uri ("file:///etc");
	file = anchor_file_get_by_uri ("file:///etc/passwd");

	EEL_CHECK_INTEGER_RESULT (g_hash_table_size (directories), 1);

	anchor_directory_file_monitor_add
		(directory, &data_dummy,
		 TRUE, 0, NULL, NULL);

	/* FIXME: these need to be updated to the new metadata infrastructure
	 *  as make check doesn't pass.
	anchor_file_set_metadata (file, "test", "default", "value");
	EEL_CHECK_STRING_RESULT (anchor_file_get_metadata (file, "test", "default"), "value");

	anchor_file_set_boolean_metadata (file, "test_boolean", TRUE, TRUE);
	EEL_CHECK_BOOLEAN_RESULT (anchor_file_get_boolean_metadata (file, "test_boolean", TRUE), TRUE);
	anchor_file_set_boolean_metadata (file, "test_boolean", TRUE, FALSE);
	EEL_CHECK_BOOLEAN_RESULT (anchor_file_get_boolean_metadata (file, "test_boolean", TRUE), FALSE);
	EEL_CHECK_BOOLEAN_RESULT (anchor_file_get_boolean_metadata (NULL, "test_boolean", TRUE), TRUE);

	anchor_file_set_integer_metadata (file, "test_integer", 0, 17);
	EEL_CHECK_INTEGER_RESULT (anchor_file_get_integer_metadata (file, "test_integer", 0), 17);
	anchor_file_set_integer_metadata (file, "test_integer", 0, -1);
	EEL_CHECK_INTEGER_RESULT (anchor_file_get_integer_metadata (file, "test_integer", 0), -1);
	anchor_file_set_integer_metadata (file, "test_integer", 42, 42);
	EEL_CHECK_INTEGER_RESULT (anchor_file_get_integer_metadata (file, "test_integer", 42), 42);
	EEL_CHECK_INTEGER_RESULT (anchor_file_get_integer_metadata (NULL, "test_integer", 42), 42);
	EEL_CHECK_INTEGER_RESULT (anchor_file_get_integer_metadata (file, "nonexistent_key", 42), 42);
	*/

	EEL_CHECK_BOOLEAN_RESULT (anchor_directory_get_by_uri ("file:///etc") == directory, TRUE);
	anchor_directory_unref (directory);

	EEL_CHECK_BOOLEAN_RESULT (anchor_directory_get_by_uri ("file:///etc/") == directory, TRUE);
	anchor_directory_unref (directory);

	EEL_CHECK_BOOLEAN_RESULT (anchor_directory_get_by_uri ("file:///etc////") == directory, TRUE);
	anchor_directory_unref (directory);

	anchor_file_unref (file);

	anchor_directory_file_monitor_remove (directory, &data_dummy);

	anchor_directory_unref (directory);

	while (g_hash_table_size (directories) != 0) {
		gtk_main_iteration ();
	}

	EEL_CHECK_INTEGER_RESULT (g_hash_table_size (directories), 0);

	directory = anchor_directory_get_by_uri ("file:///etc");

	got_files_flag = FALSE;

	anchor_directory_call_when_ready (directory,
					    NAUTILUS_FILE_ATTRIBUTE_INFO |
					    NAUTILUS_FILE_ATTRIBUTE_DEEP_COUNTS,
					    TRUE,
					    got_files_callback, &data_dummy);

	while (!got_files_flag) {
		gtk_main_iteration ();
	}

	EEL_CHECK_BOOLEAN_RESULT (directory->details->file_list == NULL, TRUE);

	EEL_CHECK_INTEGER_RESULT (g_hash_table_size (directories), 1);

	file = anchor_file_get_by_uri ("file:///etc/passwd");

	/* EEL_CHECK_STRING_RESULT (anchor_file_get_metadata (file, "test", "default"), "value"); */
	
	anchor_file_unref (file);

	anchor_directory_unref (directory);

	EEL_CHECK_INTEGER_RESULT (g_hash_table_size (directories), 0);
}

#endif /* !NAUTILUS_OMIT_SELF_CHECK */
