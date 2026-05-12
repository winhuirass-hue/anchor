/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-directory-private.h: Anchor directory model.
 
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

#include <gio/gio.h>
#include <eel/eel-vfs-extensions.h>
#include <libanchor-private/anchor-directory.h>
#include <libanchor-private/anchor-file-queue.h>
#include <libanchor-private/anchor-file.h>
#include <libanchor-private/anchor-monitor.h>
#include <libanchor-extension/anchor-info-provider.h>
#include <libxml/tree.h>

typedef struct LinkInfoReadState LinkInfoReadState;
typedef struct TopLeftTextReadState TopLeftTextReadState;
typedef struct FileMonitors FileMonitors;
typedef struct DirectoryLoadState DirectoryLoadState;
typedef struct DirectoryCountState DirectoryCountState;
typedef struct DeepCountState DeepCountState;
typedef struct GetInfoState GetInfoState;
typedef struct NewFilesState NewFilesState;
typedef struct MimeListState MimeListState;
typedef struct ThumbnailState ThumbnailState;
typedef struct MountState MountState;
typedef struct FilesystemInfoState FilesystemInfoState;

typedef enum {
	REQUEST_LINK_INFO,
	REQUEST_DEEP_COUNT,
	REQUEST_DIRECTORY_COUNT,
	REQUEST_FILE_INFO,
	REQUEST_FILE_LIST, /* always FALSE if file != NULL */
	REQUEST_MIME_LIST,
	REQUEST_TOP_LEFT_TEXT,
	REQUEST_LARGE_TOP_LEFT_TEXT,
	REQUEST_EXTENSION_INFO,
	REQUEST_THUMBNAIL,
	REQUEST_MOUNT,
	REQUEST_FILESYSTEM_INFO,
	REQUEST_TYPE_LAST
} RequestType;

/* A request for information about one or more files. */
typedef guint32 Request;
typedef gint32 RequestCounter[REQUEST_TYPE_LAST];

#define REQUEST_WANTS_TYPE(request, type) ((request) & (1<<(type)))
#define REQUEST_SET_TYPE(request, type) (request) |= (1<<(type))

struct AnchorDirectoryDetails
{
	/* The location. */
	GFile *location;

	/* The file objects. */
	AnchorFile *as_file;
	GList *file_list;
	GHashTable *file_hash;

	/* Queues of files needing some I/O done. */
	AnchorFileQueue *high_priority_queue;
	AnchorFileQueue *low_priority_queue;
	AnchorFileQueue *extension_queue;

	/* These lists are going to be pretty short.  If we think they
	 * are going to get big, we can use hash tables instead.
	 */
	GList *call_when_ready_list;
	RequestCounter call_when_ready_counters;
	GList *monitor_list;
	RequestCounter monitor_counters;
	guint call_ready_idle_id;

	AnchorMonitor *monitor;
	gulong 		 mime_db_monitor;

	gboolean in_async_service_loop;
	gboolean state_changed;

	gboolean file_list_monitored;
	gboolean directory_loaded;
	gboolean directory_loaded_sent_notification;
	DirectoryLoadState *directory_load_in_progress;

	GList *pending_file_info; /* list of GnomeVFSFileInfo's that are pending */
	int confirmed_file_count;
        guint dequeue_pending_idle_id;

	GList *new_files_in_progress; /* list of NewFilesState * */

	DirectoryCountState *count_in_progress;

	AnchorFile *deep_count_file;
	DeepCountState *deep_count_in_progress;

	MimeListState *mime_list_in_progress;

	AnchorFile *get_info_file;
	GetInfoState *get_info_in_progress;

	AnchorFile *extension_info_file;
	AnchorInfoProvider *extension_info_provider;
	AnchorOperationHandle *extension_info_in_progress;
	guint extension_info_idle;

	ThumbnailState *thumbnail_state;

	MountState *mount_state;

	FilesystemInfoState *filesystem_info_state;
	
	TopLeftTextReadState *top_left_read_state;

	LinkInfoReadState *link_info_read_state;

	GList *file_operations_in_progress; /* list of FileOperation * */
};

AnchorDirectory *anchor_directory_get_existing                    (GFile                     *location);

/* async. interface */
void               anchor_directory_async_state_changed             (AnchorDirectory         *directory);
void               anchor_directory_call_when_ready_internal        (AnchorDirectory         *directory,
								       AnchorFile              *file,
								       AnchorFileAttributes     file_attributes,
								       gboolean                   wait_for_file_list,
								       AnchorDirectoryCallback  directory_callback,
								       AnchorFileCallback       file_callback,
								       gpointer                   callback_data);
gboolean           anchor_directory_check_if_ready_internal         (AnchorDirectory         *directory,
								       AnchorFile              *file,
								       AnchorFileAttributes     file_attributes);
void               anchor_directory_cancel_callback_internal        (AnchorDirectory         *directory,
								       AnchorFile              *file,
								       AnchorDirectoryCallback  directory_callback,
								       AnchorFileCallback       file_callback,
								       gpointer                   callback_data);
void               anchor_directory_monitor_add_internal            (AnchorDirectory         *directory,
								       AnchorFile              *file,
								       gconstpointer              client,
								       gboolean                   monitor_hidden_files,
								       AnchorFileAttributes     attributes,
								       AnchorDirectoryCallback  callback,
								       gpointer                   callback_data);
void               anchor_directory_monitor_remove_internal         (AnchorDirectory         *directory,
								       AnchorFile              *file,
								       gconstpointer              client);
void               anchor_directory_get_info_for_new_files          (AnchorDirectory         *directory,
								       GList                     *vfs_uris);
AnchorFile *     anchor_directory_get_existing_corresponding_file (AnchorDirectory         *directory);
void               anchor_directory_invalidate_count_and_mime_list  (AnchorDirectory         *directory);
gboolean           anchor_directory_is_file_list_monitored          (AnchorDirectory         *directory);
gboolean           anchor_directory_is_anyone_monitoring_file_list  (AnchorDirectory         *directory);
gboolean           anchor_directory_has_active_request_for_file     (AnchorDirectory         *directory,
								       AnchorFile              *file);
void               anchor_directory_remove_file_monitor_link        (AnchorDirectory         *directory,
								       GList                     *link);
void               anchor_directory_schedule_dequeue_pending        (AnchorDirectory         *directory);
void               anchor_directory_stop_monitoring_file_list       (AnchorDirectory         *directory);
void               anchor_directory_cancel                          (AnchorDirectory         *directory);
void               anchor_async_destroying_file                     (AnchorFile              *file);
void               anchor_directory_force_reload_internal           (AnchorDirectory         *directory,
								       AnchorFileAttributes     file_attributes);
void               anchor_directory_cancel_loading_file_attributes  (AnchorDirectory         *directory,
								       AnchorFile              *file,
								       AnchorFileAttributes     file_attributes);

/* Calls shared between directory, file, and async. code. */
void               anchor_directory_emit_files_added                (AnchorDirectory         *directory,
								       GList                     *added_files);
void               anchor_directory_emit_files_changed              (AnchorDirectory         *directory,
								       GList                     *changed_files);
void               anchor_directory_emit_change_signals             (AnchorDirectory         *directory,
								       GList                     *changed_files);
void               emit_change_signals_for_all_files		      (AnchorDirectory	 *directory);
void               emit_change_signals_for_all_files_in_all_directories (void);
void               anchor_directory_emit_done_loading               (AnchorDirectory         *directory);
void               anchor_directory_emit_load_error                 (AnchorDirectory         *directory,
								       GError                    *error);
AnchorDirectory *anchor_directory_get_internal                    (GFile                     *location,
								       gboolean                   create);
char *             anchor_directory_get_name_for_self_as_new_file   (AnchorDirectory         *directory);
Request            anchor_directory_set_up_request                  (AnchorFileAttributes     file_attributes);

/* Interface to the file list. */
AnchorFile *     anchor_directory_find_file_by_name               (AnchorDirectory         *directory,
								       const char                *filename);
AnchorFile *     anchor_directory_find_file_by_internal_filename  (AnchorDirectory         *directory,
								       const char                *internal_filename);

void               anchor_directory_add_file                        (AnchorDirectory         *directory,
								       AnchorFile              *file);
void               anchor_directory_remove_file                     (AnchorDirectory         *directory,
								       AnchorFile              *file);
FileMonitors *     anchor_directory_remove_file_monitors            (AnchorDirectory         *directory,
								       AnchorFile              *file);
void               anchor_directory_add_file_monitors               (AnchorDirectory         *directory,
								       AnchorFile              *file,
								       FileMonitors              *monitors);
void               anchor_directory_add_file                        (AnchorDirectory         *directory,
								       AnchorFile              *file);
GList *            anchor_directory_begin_file_name_change          (AnchorDirectory         *directory,
								       AnchorFile              *file);
void               anchor_directory_end_file_name_change            (AnchorDirectory         *directory,
								       AnchorFile              *file,
								       GList                     *node);
void               anchor_directory_moved                           (const char                *from_uri,
								       const char                *to_uri);
/* Interface to the work queue. */

void               anchor_directory_add_file_to_work_queue          (AnchorDirectory *directory,
								       AnchorFile *file);
void               anchor_directory_remove_file_from_work_queue     (AnchorDirectory *directory,
								       AnchorFile *file);


/* debugging functions */
int                anchor_directory_number_outstanding              (void);
