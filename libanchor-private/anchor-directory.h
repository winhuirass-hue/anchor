/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-directory.h: Anchor directory model.
 
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

#ifndef NAUTILUS_DIRECTORY_H
#define NAUTILUS_DIRECTORY_H

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <libanchor-private/anchor-file-attributes.h>

/* AnchorDirectory is a class that manages the model for a directory,
   real or virtual, for Anchor, mainly the file-manager component. The directory is
   responsible for managing both real data and cached metadata. On top of
   the file system independence provided by gio, the directory
   object also provides:
  
       1) A synchronization framework, which notifies via signals as the
          set of known files changes.
       2) An abstract interface for getting attributes and performing
          operations on files.
*/

#define ANCHOR_TYPE_DIRECTORY anchor_directory_get_type()
#define NAUTILUS_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_DIRECTORY, AnchorDirectory))
#define NAUTILUS_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_DIRECTORY, AnchorDirectoryClass))
#define NAUTILUS_IS_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_DIRECTORY))
#define NAUTILUS_IS_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_DIRECTORY))
#define NAUTILUS_DIRECTORY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_DIRECTORY, AnchorDirectoryClass))

/* AnchorFile is defined both here and in anchor-file.h. */
#ifndef NAUTILUS_FILE_DEFINED
#define NAUTILUS_FILE_DEFINED
typedef struct AnchorFile AnchorFile;
#endif

typedef struct AnchorDirectoryDetails AnchorDirectoryDetails;

typedef struct
{
	GObject object;
	AnchorDirectoryDetails *details;
} AnchorDirectory;

typedef void (*AnchorDirectoryCallback) (AnchorDirectory *directory,
					   GList             *files,
					   gpointer           callback_data);

typedef struct
{
	GObjectClass parent_class;

	/*** Notification signals for clients to connect to. ***/

	/* The files_added signal is emitted as the directory model 
	 * discovers new files.
	 */
	void     (* files_added)         (AnchorDirectory          *directory,
					  GList                      *added_files);

	/* The files_changed signal is emitted as changes occur to
	 * existing files that are noticed by the synchronization framework,
	 * including when an old file has been deleted. When an old file
	 * has been deleted, this is the last chance to forget about these
	 * file objects, which are about to be unref'd. Use a call to
	 * anchor_file_is_gone () to test for this case.
	 */
	void     (* files_changed)       (AnchorDirectory         *directory,
					  GList                     *changed_files);

	/* The done_loading signal is emitted when a directory load
	 * request completes. This is needed because, at least in the
	 * case where the directory is empty, the caller will receive
	 * no kind of notification at all when a directory load
	 * initiated by `anchor_directory_file_monitor_add' completes.
	 */
	void     (* done_loading)        (AnchorDirectory         *directory);

	void     (* load_error)          (AnchorDirectory         *directory,
					  GError                    *error);

	/*** Virtual functions for subclasses to override. ***/
	gboolean (* contains_file)       (AnchorDirectory         *directory,
					  AnchorFile              *file);
	void     (* call_when_ready)     (AnchorDirectory         *directory,
					  AnchorFileAttributes     file_attributes,
					  gboolean                   wait_for_file_list,
					  AnchorDirectoryCallback  callback,
					  gpointer                   callback_data);
	void     (* cancel_callback)     (AnchorDirectory         *directory,
					  AnchorDirectoryCallback  callback,
					  gpointer                   callback_data);
	void     (* file_monitor_add)    (AnchorDirectory          *directory,
					  gconstpointer              client,
					  gboolean                   monitor_hidden_files,
					  AnchorFileAttributes     monitor_attributes,
					  AnchorDirectoryCallback  initial_files_callback,
					  gpointer                   callback_data);
	void     (* file_monitor_remove) (AnchorDirectory         *directory,
					  gconstpointer              client);
	void     (* force_reload)        (AnchorDirectory         *directory);
	gboolean (* are_all_files_seen)  (AnchorDirectory         *directory);
	gboolean (* is_not_empty)        (AnchorDirectory         *directory);

	/* get_file_list is a function pointer that subclasses may override to
	 * customize collecting the list of files in a directory.
	 * For example, the AnchorDesktopDirectory overrides this so that it can
	 * merge together the list of files in the $HOME/Desktop directory with
	 * the list of standard icons (Home, Trash) on the desktop.
	 */
	GList *	 (* get_file_list)	 (AnchorDirectory *directory);

	/* Should return FALSE if the directory is read-only and doesn't
	 * allow setting of metadata.
	 * An example of this is the search directory.
	 */
	gboolean (* is_editable)         (AnchorDirectory *directory);
} AnchorDirectoryClass;

/* Basic GObject requirements. */
GType              anchor_directory_get_type                 (void);

/* Get a directory given a uri.
 * Creates the appropriate subclass given the uri mappings.
 * Returns a referenced object, not a floating one. Unref when finished.
 * If two windows are viewing the same uri, the directory object is shared.
 */
AnchorDirectory *anchor_directory_get                      (GFile                     *location);
AnchorDirectory *anchor_directory_get_by_uri               (const char                *uri);
AnchorDirectory *anchor_directory_get_for_file             (AnchorFile              *file);

/* Covers for g_object_ref and g_object_unref that provide two conveniences:
 * 1) Using these is type safe.
 * 2) You are allowed to call these with NULL,
 */
AnchorDirectory *anchor_directory_ref                      (AnchorDirectory         *directory);
void               anchor_directory_unref                    (AnchorDirectory         *directory);

/* Access to a URI. */
char *             anchor_directory_get_uri                  (AnchorDirectory         *directory);
GFile *            anchor_directory_get_location             (AnchorDirectory         *directory);

/* Is this file still alive and in this directory? */
gboolean           anchor_directory_contains_file            (AnchorDirectory         *directory,
								AnchorFile              *file);

/* Get the uri of the file in the directory, NULL if not found */
char *             anchor_directory_get_file_uri             (AnchorDirectory         *directory,
								const char                *file_name);

/* Get (and ref) a AnchorFile object for this directory. */
AnchorFile *     anchor_directory_get_corresponding_file   (AnchorDirectory         *directory);

/* Waiting for data that's read asynchronously.
 * The file attribute and metadata keys are for files in the directory.
 */
void               anchor_directory_call_when_ready          (AnchorDirectory         *directory,
								AnchorFileAttributes     file_attributes,
								gboolean                   wait_for_all_files,
								AnchorDirectoryCallback  callback,
								gpointer                   callback_data);
void               anchor_directory_cancel_callback          (AnchorDirectory         *directory,
								AnchorDirectoryCallback  callback,
								gpointer                   callback_data);


/* Monitor the files in a directory. */
void               anchor_directory_file_monitor_add         (AnchorDirectory         *directory,
								gconstpointer              client,
								gboolean                   monitor_hidden_files,
								AnchorFileAttributes     attributes,
								AnchorDirectoryCallback  initial_files_callback,
								gpointer                   callback_data);
void               anchor_directory_file_monitor_remove      (AnchorDirectory         *directory,
								gconstpointer              client);
void               anchor_directory_force_reload             (AnchorDirectory         *directory);

/* Get a list of all files currently known in the directory. */
GList *            anchor_directory_get_file_list            (AnchorDirectory         *directory);

GList *            anchor_directory_match_pattern            (AnchorDirectory         *directory,
							        const char *glob);


/* Return true if the directory has information about all the files.
 * This will be false until the directory has been read at least once.
 */
gboolean           anchor_directory_are_all_files_seen       (AnchorDirectory         *directory);

/* Return true if the directory is local. */
gboolean           anchor_directory_is_local                 (AnchorDirectory         *directory);

gboolean           anchor_directory_is_in_trash              (AnchorDirectory         *directory);
gboolean           anchor_directory_is_in_recent             (AnchorDirectory         *directory);
gboolean           anchor_directory_is_in_network            (AnchorDirectory         *directory);

/* Return false if directory contains anything besides a Anchor metafile.
 * Only valid if directory is monitored. Used by the Trash monitor.
 */
gboolean           anchor_directory_is_not_empty             (AnchorDirectory         *directory);

/* Convenience functions for dealing with a list of AnchorDirectory objects that each have a ref.
 * These are just convenient names for functions that work on lists of GtkObject *.
 */
GList *            anchor_directory_list_ref                 (GList                     *directory_list);
void               anchor_directory_list_unref               (GList                     *directory_list);
void               anchor_directory_list_free                (GList                     *directory_list);
GList *            anchor_directory_list_copy                (GList                     *directory_list);
GList *            anchor_directory_list_sort_by_uri         (GList                     *directory_list);

/* Fast way to check if a directory is the desktop directory */
gboolean           anchor_directory_is_desktop_directory     (AnchorDirectory         *directory);

gboolean           anchor_directory_is_editable              (AnchorDirectory         *directory);


#endif /* NAUTILUS_DIRECTORY_H */
