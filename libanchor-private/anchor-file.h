/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-file.h: Anchor file model.
 
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

#ifndef NAUTILUS_FILE_H
#define NAUTILUS_FILE_H

#include <gtk/gtk.h>
#include <gio/gio.h>
#include <libanchor-private/anchor-file-attributes.h>
#include <libanchor-private/anchor-icon-info.h>

/* AnchorFile is an object used to represent a single element of a
 * AnchorDirectory. It's lightweight and relies on AnchorDirectory
 * to do most of the work.
 */

/* AnchorFile is defined both here and in anchor-directory.h. */
#ifndef NAUTILUS_FILE_DEFINED
#define NAUTILUS_FILE_DEFINED
typedef struct AnchorFile AnchorFile;
#endif

#define ANCHOR_TYPE_FILE anchor_file_get_type()
#define NAUTILUS_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_FILE, AnchorFile))
#define NAUTILUS_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_FILE, AnchorFileClass))
#define NAUTILUS_IS_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_FILE))
#define NAUTILUS_IS_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_FILE))
#define NAUTILUS_FILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_FILE, AnchorFileClass))

typedef enum {
	NAUTILUS_FILE_SORT_NONE,
	NAUTILUS_FILE_SORT_BY_DISPLAY_NAME,
	NAUTILUS_FILE_SORT_BY_SIZE,
	NAUTILUS_FILE_SORT_BY_TYPE,
	NAUTILUS_FILE_SORT_BY_MTIME,
        NAUTILUS_FILE_SORT_BY_ATIME,
	NAUTILUS_FILE_SORT_BY_TRASHED_TIME,
	NAUTILUS_FILE_SORT_BY_SEARCH_RELEVANCE
} AnchorFileSortType;	

typedef enum {
	NAUTILUS_REQUEST_NOT_STARTED,
	NAUTILUS_REQUEST_IN_PROGRESS,
	NAUTILUS_REQUEST_DONE
} AnchorRequestStatus;

typedef enum {
	NAUTILUS_FILE_ICON_FLAGS_NONE = 0,
	NAUTILUS_FILE_ICON_FLAGS_USE_THUMBNAILS = (1<<0),
	NAUTILUS_FILE_ICON_FLAGS_IGNORE_VISITING = (1<<1),
	NAUTILUS_FILE_ICON_FLAGS_EMBEDDING_TEXT = (1<<2),
	NAUTILUS_FILE_ICON_FLAGS_FOR_DRAG_ACCEPT = (1<<3),
	NAUTILUS_FILE_ICON_FLAGS_FOR_OPEN_FOLDER = (1<<4),
	/* whether the thumbnail size must match the display icon size */
	NAUTILUS_FILE_ICON_FLAGS_FORCE_THUMBNAIL_SIZE = (1<<5),
	/* uses the icon of the mount if present */
	NAUTILUS_FILE_ICON_FLAGS_USE_MOUNT_ICON = (1<<6),
	/* render the mount icon as an emblem over the regular one */
	NAUTILUS_FILE_ICON_FLAGS_USE_MOUNT_ICON_AS_EMBLEM = (1<<7)
} AnchorFileIconFlags;	

/* Emblems sometimes displayed for AnchorFiles. Do not localize. */ 
#define NAUTILUS_FILE_EMBLEM_NAME_SYMBOLIC_LINK "symbolic-link"
#define NAUTILUS_FILE_EMBLEM_NAME_CANT_READ "unreadable"
#define NAUTILUS_FILE_EMBLEM_NAME_CANT_WRITE "readonly"
#define NAUTILUS_FILE_EMBLEM_NAME_TRASH "trash"
#define NAUTILUS_FILE_EMBLEM_NAME_NOTE "note"

typedef void (*AnchorFileCallback)          (AnchorFile  *file,
				               gpointer       callback_data);
typedef void (*AnchorFileListCallback)      (GList         *file_list,
				               gpointer       callback_data);
typedef void (*AnchorFileOperationCallback) (AnchorFile  *file,
					       GFile         *result_location,
					       GError        *error,
					       gpointer       callback_data);
typedef int (*AnchorWidthMeasureCallback)   (const char    *string,
					       void	     *context);
typedef char * (*AnchorTruncateCallback)    (const char    *string,
					       int	      width,
					       void	     *context);


#define NAUTILUS_FILE_ATTRIBUTES_FOR_ICON (NAUTILUS_FILE_ATTRIBUTE_INFO | NAUTILUS_FILE_ATTRIBUTE_LINK_INFO | NAUTILUS_FILE_ATTRIBUTE_THUMBNAIL)

typedef void AnchorFileListHandle;

/* GObject requirements. */
GType                   anchor_file_get_type                          (void);

/* Getting at a single file. */
AnchorFile *          anchor_file_get                               (GFile                          *location);
AnchorFile *          anchor_file_get_by_uri                        (const char                     *uri);

/* Get a file only if the anchor version already exists */
AnchorFile *          anchor_file_get_existing                      (GFile                          *location);
AnchorFile *          anchor_file_get_existing_by_uri               (const char                     *uri);

/* Covers for g_object_ref and g_object_unref that provide two conveniences:
 * 1) Using these is type safe.
 * 2) You are allowed to call these with NULL,
 */
AnchorFile *          anchor_file_ref                               (AnchorFile                   *file);
void                    anchor_file_unref                             (AnchorFile                   *file);

/* Monitor the file. */
void                    anchor_file_monitor_add                       (AnchorFile                   *file,
									 gconstpointer                   client,
									 AnchorFileAttributes          attributes);
void                    anchor_file_monitor_remove                    (AnchorFile                   *file,
									 gconstpointer                   client);

/* Waiting for data that's read asynchronously.
 * This interface currently works only for metadata, but could be expanded
 * to other attributes as well.
 */
void                    anchor_file_call_when_ready                   (AnchorFile                   *file,
									 AnchorFileAttributes          attributes,
									 AnchorFileCallback            callback,
									 gpointer                        callback_data);
void                    anchor_file_cancel_call_when_ready            (AnchorFile                   *file,
									 AnchorFileCallback            callback,
									 gpointer                        callback_data);
gboolean                anchor_file_check_if_ready                    (AnchorFile                   *file,
									 AnchorFileAttributes          attributes);
void                    anchor_file_invalidate_attributes             (AnchorFile                   *file,
									 AnchorFileAttributes          attributes);
void                    anchor_file_invalidate_all_attributes         (AnchorFile                   *file);

/* Basic attributes for file objects. */
gboolean                anchor_file_contains_text                     (AnchorFile                   *file);
char *                  anchor_file_get_display_name                  (AnchorFile                   *file);
char *                  anchor_file_get_edit_name                     (AnchorFile                   *file);
char *                  anchor_file_get_name                          (AnchorFile                   *file);
GFile *                 anchor_file_get_location                      (AnchorFile                   *file);
char *			 anchor_file_get_description			 (AnchorFile			 *file);
char *                  anchor_file_get_uri                           (AnchorFile                   *file);
char *                  anchor_file_get_uri_scheme                    (AnchorFile                   *file);
AnchorFile *          anchor_file_get_parent                        (AnchorFile                   *file);
GFile *                 anchor_file_get_parent_location               (AnchorFile                   *file);
char *                  anchor_file_get_parent_uri                    (AnchorFile                   *file);
char *                  anchor_file_get_parent_uri_for_display        (AnchorFile                   *file);
char *                  anchor_file_get_thumbnail_path                (AnchorFile                   *file);
gboolean                anchor_file_can_get_size                      (AnchorFile                   *file);
goffset                 anchor_file_get_size                          (AnchorFile                   *file);
time_t                  anchor_file_get_mtime                         (AnchorFile                   *file);
GFileType               anchor_file_get_file_type                     (AnchorFile                   *file);
char *                  anchor_file_get_mime_type                     (AnchorFile                   *file);
char *                  anchor_file_get_extension                     (AnchorFile                   *file);
gboolean                anchor_file_is_mime_type                      (AnchorFile                   *file,
									 const char                     *mime_type);
gboolean                anchor_file_is_launchable                     (AnchorFile                   *file);
gboolean                anchor_file_is_symbolic_link                  (AnchorFile                   *file);
gboolean                anchor_file_is_mountpoint                     (AnchorFile                   *file);
GMount *                anchor_file_get_mount                         (AnchorFile                   *file);
char *                  anchor_file_get_volume_free_space             (AnchorFile                   *file);
char *                  anchor_file_get_volume_name                   (AnchorFile                   *file);
char *                  anchor_file_get_symbolic_link_target_path     (AnchorFile                   *file);
char *                  anchor_file_get_symbolic_link_target_uri      (AnchorFile                   *file);
gboolean                anchor_file_is_broken_symbolic_link           (AnchorFile                   *file);
gboolean                anchor_file_is_anchor_link                  (AnchorFile                   *file);
gboolean                anchor_file_is_executable                     (AnchorFile                   *file);
gboolean                anchor_file_is_directory                      (AnchorFile                   *file);
gboolean                anchor_file_is_user_special_directory         (AnchorFile                   *file,
									 GUserDirectory                 special_directory);
gboolean		anchor_file_is_archive			(AnchorFile			*file);
gboolean                anchor_file_is_in_search                      (AnchorFile                   *file);
gboolean                anchor_file_is_in_trash                       (AnchorFile                   *file);
gboolean                anchor_file_is_in_recent                      (AnchorFile                   *file);
gboolean                anchor_file_is_in_desktop                     (AnchorFile                   *file);
gboolean                anchor_file_is_in_network                     (AnchorFile                   *file);
gboolean		anchor_file_is_home				(AnchorFile                   *file);
gboolean                anchor_file_is_desktop_directory              (AnchorFile                   *file);
GError *                anchor_file_get_file_info_error               (AnchorFile                   *file);
gboolean                anchor_file_get_directory_item_count          (AnchorFile                   *file,
									 guint                          *count,
									 gboolean                       *count_unreadable);
void                    anchor_file_recompute_deep_counts             (AnchorFile                   *file);
AnchorRequestStatus   anchor_file_get_deep_counts                   (AnchorFile                   *file,
									 guint                          *directory_count,
									 guint                          *file_count,
									 guint                          *unreadable_directory_count,
									 goffset               *total_size,
									 gboolean                        force);
gboolean                anchor_file_should_show_thumbnail             (AnchorFile                   *file);
gboolean                anchor_file_should_show_directory_item_count  (AnchorFile                   *file);
gboolean                anchor_file_should_show_type                  (AnchorFile                   *file);
GList *                 anchor_file_get_emblem_icons                  (AnchorFile                   *file);
char *                  anchor_file_get_top_left_text                 (AnchorFile                   *file);
char *                  anchor_file_peek_top_left_text                (AnchorFile                   *file,
									 gboolean                        need_large_text,
									 gboolean                       *got_top_left_text);
gboolean                anchor_file_get_directory_item_mime_types     (AnchorFile                   *file,
									 GList                         **mime_list);

void                    anchor_file_set_search_relevance              (AnchorFile                   *file,
									 gdouble                         relevance);
void                    anchor_file_set_attributes                    (AnchorFile                   *file, 
									 GFileInfo                      *attributes,
									 AnchorFileOperationCallback   callback,
									 gpointer                        callback_data);
GFilesystemPreviewType  anchor_file_get_filesystem_use_preview        (AnchorFile *file);

char *                  anchor_file_get_filesystem_id                 (AnchorFile                   *file);

AnchorFile *          anchor_file_get_trash_original_file           (AnchorFile                   *file);

/* Permissions. */
gboolean                anchor_file_can_get_permissions               (AnchorFile                   *file);
gboolean                anchor_file_can_set_permissions               (AnchorFile                   *file);
guint                   anchor_file_get_permissions                   (AnchorFile                   *file);
gboolean                anchor_file_can_get_owner                     (AnchorFile                   *file);
gboolean                anchor_file_can_set_owner                     (AnchorFile                   *file);
gboolean                anchor_file_can_get_group                     (AnchorFile                   *file);
gboolean                anchor_file_can_set_group                     (AnchorFile                   *file);
char *                  anchor_file_get_owner_name                    (AnchorFile                   *file);
char *                  anchor_file_get_group_name                    (AnchorFile                   *file);
GList *                 anchor_get_user_names                         (void);
GList *                 anchor_get_all_group_names                    (void);
GList *                 anchor_file_get_settable_group_names          (AnchorFile                   *file);
gboolean                anchor_file_can_get_selinux_context           (AnchorFile                   *file);
char *                  anchor_file_get_selinux_context               (AnchorFile                   *file);

/* "Capabilities". */
gboolean                anchor_file_can_read                          (AnchorFile                   *file);
gboolean                anchor_file_can_write                         (AnchorFile                   *file);
gboolean                anchor_file_can_execute                       (AnchorFile                   *file);
gboolean                anchor_file_can_rename                        (AnchorFile                   *file);
gboolean                anchor_file_can_delete                        (AnchorFile                   *file);
gboolean                anchor_file_can_trash                         (AnchorFile                   *file);

gboolean                anchor_file_can_mount                         (AnchorFile                   *file);
gboolean                anchor_file_can_unmount                       (AnchorFile                   *file);
gboolean                anchor_file_can_eject                         (AnchorFile                   *file);
gboolean                anchor_file_can_start                         (AnchorFile                   *file);
gboolean                anchor_file_can_start_degraded                (AnchorFile                   *file);
gboolean                anchor_file_can_stop                          (AnchorFile                   *file);
GDriveStartStopType     anchor_file_get_start_stop_type               (AnchorFile                   *file);
gboolean                anchor_file_can_poll_for_media                (AnchorFile                   *file);
gboolean                anchor_file_is_media_check_automatic          (AnchorFile                   *file);

void                    anchor_file_mount                             (AnchorFile                   *file,
									 GMountOperation                *mount_op,
									 GCancellable                   *cancellable,
									 AnchorFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    anchor_file_unmount                           (AnchorFile                   *file,
									 GMountOperation                *mount_op,
									 GCancellable                   *cancellable,
									 AnchorFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    anchor_file_eject                             (AnchorFile                   *file,
									 GMountOperation                *mount_op,
									 GCancellable                   *cancellable,
									 AnchorFileOperationCallback   callback,
									 gpointer                        callback_data);

void                    anchor_file_start                             (AnchorFile                   *file,
									 GMountOperation                *start_op,
									 GCancellable                   *cancellable,
									 AnchorFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    anchor_file_stop                              (AnchorFile                   *file,
									 GMountOperation                *mount_op,
									 GCancellable                   *cancellable,
									 AnchorFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    anchor_file_poll_for_media                    (AnchorFile                   *file);

/* Basic operations for file objects. */
void                    anchor_file_set_owner                         (AnchorFile                   *file,
									 const char                     *user_name_or_id,
									 AnchorFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    anchor_file_set_group                         (AnchorFile                   *file,
									 const char                     *group_name_or_id,
									 AnchorFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    anchor_file_set_permissions                   (AnchorFile                   *file,
									 guint32                         permissions,
									 AnchorFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    anchor_file_rename                            (AnchorFile                   *file,
									 const char                     *new_name,
									 AnchorFileOperationCallback   callback,
									 gpointer                        callback_data);
void                    anchor_file_cancel                            (AnchorFile                   *file,
									 AnchorFileOperationCallback   callback,
									 gpointer                        callback_data);

/* Return true if this file has already been deleted.
 * This object will be unref'd after sending the files_removed signal,
 * but it could hang around longer if someone ref'd it.
 */
gboolean                anchor_file_is_gone                           (AnchorFile                   *file);

/* Return true if this file is not confirmed to have ever really
 * existed. This is true when the AnchorFile object has been created, but no I/O
 * has yet confirmed the existence of a file by that name.
 */
gboolean                anchor_file_is_not_yet_confirmed              (AnchorFile                   *file);

/* Simple getting and setting top-level metadata. */
char *                  anchor_file_get_metadata                      (AnchorFile                   *file,
									 const char                     *key,
									 const char                     *default_metadata);
GList *                 anchor_file_get_metadata_list                 (AnchorFile                   *file,
									 const char                     *key);
void                    anchor_file_set_metadata                      (AnchorFile                   *file,
									 const char                     *key,
									 const char                     *default_metadata,
									 const char                     *metadata);
void                    anchor_file_set_metadata_list                 (AnchorFile                   *file,
									 const char                     *key,
									 GList                          *list);

/* Covers for common data types. */
gboolean                anchor_file_get_boolean_metadata              (AnchorFile                   *file,
									 const char                     *key,
									 gboolean                        default_metadata);
void                    anchor_file_set_boolean_metadata              (AnchorFile                   *file,
									 const char                     *key,
									 gboolean                        default_metadata,
									 gboolean                        metadata);
int                     anchor_file_get_integer_metadata              (AnchorFile                   *file,
									 const char                     *key,
									 int                             default_metadata);
void                    anchor_file_set_integer_metadata              (AnchorFile                   *file,
									 const char                     *key,
									 int                             default_metadata,
									 int                             metadata);

#define UNDEFINED_TIME ((time_t) (-1))

time_t                  anchor_file_get_time_metadata                 (AnchorFile                  *file,
									 const char                    *key);
void                    anchor_file_set_time_metadata                 (AnchorFile                  *file,
									 const char                    *key,
									 time_t                         time);


/* Attributes for file objects as user-displayable strings. */
char *                  anchor_file_get_string_attribute              (AnchorFile                   *file,
									 const char                     *attribute_name);
char *                  anchor_file_get_string_attribute_q            (AnchorFile                   *file,
									 GQuark                          attribute_q);
char *                  anchor_file_get_string_attribute_with_default (AnchorFile                   *file,
									 const char                     *attribute_name);
char *                  anchor_file_get_string_attribute_with_default_q (AnchorFile                  *file,
									 GQuark                          attribute_q);

/* Matching with another URI. */
gboolean                anchor_file_matches_uri                       (AnchorFile                   *file,
									 const char                     *uri);

/* Is the file local? */
gboolean                anchor_file_is_local                          (AnchorFile                   *file);

/* Comparing two file objects for sorting */
AnchorFileSortType    anchor_file_get_default_sort_type             (AnchorFile                   *file,
									 gboolean                       *reversed);
const gchar *           anchor_file_get_default_sort_attribute        (AnchorFile                   *file,
									 gboolean                       *reversed);

int                     anchor_file_compare_for_sort                  (AnchorFile                   *file_1,
									 AnchorFile                   *file_2,
									 AnchorFileSortType            sort_type,
									 gboolean			 directories_first,
									 gboolean		  	 reversed);
int                     anchor_file_compare_for_sort_by_attribute     (AnchorFile                   *file_1,
									 AnchorFile                   *file_2,
									 const char                     *attribute,
									 gboolean                        directories_first,
									 gboolean                        reversed);
int                     anchor_file_compare_for_sort_by_attribute_q   (AnchorFile                   *file_1,
									 AnchorFile                   *file_2,
									 GQuark                          attribute,
									 gboolean                        directories_first,
									 gboolean                        reversed);
gboolean                anchor_file_is_date_sort_attribute_q          (GQuark                          attribute);

int                     anchor_file_compare_display_name              (AnchorFile                   *file_1,
									 const char                     *pattern);
int                     anchor_file_compare_location                  (AnchorFile                    *file_1,
                                                                         AnchorFile                    *file_2);

/* filtering functions for use by various directory views */
gboolean                anchor_file_is_hidden_file                    (AnchorFile                   *file);
gboolean                anchor_file_should_show                       (AnchorFile                   *file,
									 gboolean                        show_hidden,
									 gboolean                        show_foreign);
GList                  *anchor_file_list_filter_hidden                (GList                          *files,
									 gboolean                        show_hidden);


/* Get the URI that's used when activating the file.
 * Getting this can require reading the contents of the file.
 */
gboolean                anchor_file_is_launcher                       (AnchorFile                   *file);
gboolean                anchor_file_is_foreign_link                   (AnchorFile                   *file);
gboolean                anchor_file_is_trusted_link                   (AnchorFile                   *file);
gboolean                anchor_file_has_activation_uri                (AnchorFile                   *file);
char *                  anchor_file_get_activation_uri                (AnchorFile                   *file);
GFile *                 anchor_file_get_activation_location           (AnchorFile                   *file);

char *                  anchor_file_get_drop_target_uri               (AnchorFile                   *file);

GIcon *                 anchor_file_get_gicon                         (AnchorFile                   *file,
									 AnchorFileIconFlags           flags);
AnchorIconInfo *      anchor_file_get_icon                          (AnchorFile                   *file,
									 int                             size,
									 int                             scale,
									 AnchorFileIconFlags           flags);
GdkPixbuf *             anchor_file_get_icon_pixbuf                   (AnchorFile                   *file,
									 int                             size,
									 gboolean                        force_size,
									 int                             scale,
									 AnchorFileIconFlags           flags);

gboolean                anchor_file_has_open_window                   (AnchorFile                   *file);
void                    anchor_file_set_has_open_window               (AnchorFile                   *file,
									 gboolean                        has_open_window);

/* Thumbnailing handling */
gboolean                anchor_file_is_thumbnailing                   (AnchorFile                   *file);

/* Convenience functions for dealing with a list of AnchorFile objects that each have a ref.
 * These are just convenient names for functions that work on lists of GtkObject *.
 */
GList *                 anchor_file_list_ref                          (GList                          *file_list);
void                    anchor_file_list_unref                        (GList                          *file_list);
void                    anchor_file_list_free                         (GList                          *file_list);
GList *                 anchor_file_list_copy                         (GList                          *file_list);
GList *                 anchor_file_list_from_uris                    (GList                          *uri_list);
GList *			anchor_file_list_sort_by_display_name		(GList				*file_list);
void                    anchor_file_list_call_when_ready              (GList                          *file_list,
									 AnchorFileAttributes          attributes,
									 AnchorFileListHandle        **handle,
									 AnchorFileListCallback        callback,
									 gpointer                        callback_data);
void                    anchor_file_list_cancel_call_when_ready       (AnchorFileListHandle         *handle);

/* Debugging */
void                    anchor_file_dump                              (AnchorFile                   *file);

typedef struct AnchorFileDetails AnchorFileDetails;

struct AnchorFile {
	GObject parent_slot;
	AnchorFileDetails *details;
};

/* This is actually a "protected" type, but it must be here so we can
 * compile the get_date function pointer declaration below.
 */
typedef enum {
	NAUTILUS_DATE_TYPE_MODIFIED,
	NAUTILUS_DATE_TYPE_ACCESSED,
	NAUTILUS_DATE_TYPE_TRASHED
} AnchorDateType;

typedef struct {
	GObjectClass parent_slot;

	/* Subclasses can set this to something other than G_FILE_TYPE_UNKNOWN and
	   it will be used as the default file type. This is useful when creating
	   a "virtual" AnchorFile subclass that you can't actually get real
	   information about. For exaple AnchorDesktopDirectoryFile. */
	GFileType default_file_type; 
	
	/* Called when the file notices any change. */
	void                  (* changed)                (AnchorFile *file);

	/* Called periodically while directory deep count is being computed. */
	void                  (* updated_deep_count_in_progress) (AnchorFile *file);

	/* Virtual functions (mainly used for trash directory). */
	void                  (* monitor_add)            (AnchorFile           *file,
							  gconstpointer           client,
							  AnchorFileAttributes  attributes);
	void                  (* monitor_remove)         (AnchorFile           *file,
							  gconstpointer           client);
	void                  (* call_when_ready)        (AnchorFile           *file,
							  AnchorFileAttributes  attributes,
							  AnchorFileCallback    callback,
							  gpointer                callback_data);
	void                  (* cancel_call_when_ready) (AnchorFile           *file,
							  AnchorFileCallback    callback,
							  gpointer                callback_data);
	gboolean              (* check_if_ready)         (AnchorFile           *file,
							  AnchorFileAttributes  attributes);
	gboolean              (* get_item_count)         (AnchorFile           *file,
							  guint                  *count,
							  gboolean               *count_unreadable);
	AnchorRequestStatus (* get_deep_counts)        (AnchorFile           *file,
							  guint                  *directory_count,
							  guint                  *file_count,
							  guint                  *unreadable_directory_count,
							  goffset       *total_size);
	gboolean              (* get_date)               (AnchorFile           *file,
							  AnchorDateType        type,
							  time_t                 *date);
	char *                (* get_where_string)       (AnchorFile           *file);

	void                  (* set_metadata)           (AnchorFile           *file,
							  const char             *key,
							  const char             *value);
	void                  (* set_metadata_as_list)   (AnchorFile           *file,
							  const char             *key,
							  char                  **value);
	
	void                  (* mount)                  (AnchorFile                   *file,
							  GMountOperation                *mount_op,
							  GCancellable                   *cancellable,
							  AnchorFileOperationCallback   callback,
							  gpointer                        callback_data);
	void                 (* unmount)                 (AnchorFile                   *file,
							  GMountOperation                *mount_op,
							  GCancellable                   *cancellable,
							  AnchorFileOperationCallback   callback,
							  gpointer                        callback_data);
	void                 (* eject)                   (AnchorFile                   *file,
							  GMountOperation                *mount_op,
							  GCancellable                   *cancellable,
							  AnchorFileOperationCallback   callback,
							  gpointer                        callback_data);

	void                  (* start)                  (AnchorFile                   *file,
							  GMountOperation                *start_op,
							  GCancellable                   *cancellable,
							  AnchorFileOperationCallback   callback,
							  gpointer                        callback_data);
	void                 (* stop)                    (AnchorFile                   *file,
							  GMountOperation                *mount_op,
							  GCancellable                   *cancellable,
							  AnchorFileOperationCallback   callback,
							  gpointer                        callback_data);

	void                 (* poll_for_media)          (AnchorFile                   *file);
} AnchorFileClass;

#endif /* NAUTILUS_FILE_H */
