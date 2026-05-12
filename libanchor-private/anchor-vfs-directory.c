/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-vfs-directory.c: Subclass of AnchorDirectory to help implement the
   virtual trash directory.
 
   Copyright (C) 1999, 2000 Eazel, Inc.
  
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
#include "anchor-vfs-directory.h"

#include "anchor-directory-private.h"
#include "anchor-file-private.h"

G_DEFINE_TYPE (AnchorVFSDirectory, anchor_vfs_directory, ANCHOR_TYPE_DIRECTORY);

static void
anchor_vfs_directory_init (AnchorVFSDirectory *directory)
{

}

static gboolean
vfs_contains_file (AnchorDirectory *directory,
		   AnchorFile *file)
{
	g_assert (NAUTILUS_IS_VFS_DIRECTORY (directory));
	g_assert (NAUTILUS_IS_FILE (file));

	return file->details->directory == directory;
}

static void
vfs_call_when_ready (AnchorDirectory *directory,
		     AnchorFileAttributes file_attributes,
		     gboolean wait_for_file_list,
		     AnchorDirectoryCallback callback,
		     gpointer callback_data)
{
	g_assert (NAUTILUS_IS_VFS_DIRECTORY (directory));

	anchor_directory_call_when_ready_internal
		(directory,
		 NULL,
		 file_attributes,
		 wait_for_file_list,
		 callback,
		 NULL,
		 callback_data);
}

static void
vfs_cancel_callback (AnchorDirectory *directory,
		     AnchorDirectoryCallback callback,
		     gpointer callback_data)
{
	g_assert (NAUTILUS_IS_VFS_DIRECTORY (directory));

	anchor_directory_cancel_callback_internal
		(directory,
		 NULL,
		 callback,
		 NULL,
		 callback_data);
}

static void
vfs_file_monitor_add (AnchorDirectory *directory,
		      gconstpointer client,
		      gboolean monitor_hidden_files,
		      AnchorFileAttributes file_attributes,
		      AnchorDirectoryCallback callback,
		      gpointer callback_data)
{
	g_assert (NAUTILUS_IS_VFS_DIRECTORY (directory));
	g_assert (client != NULL);

	anchor_directory_monitor_add_internal
		(directory, NULL,
		 client,
		 monitor_hidden_files,
		 file_attributes,
		 callback, callback_data);
}

static void
vfs_file_monitor_remove (AnchorDirectory *directory,
			 gconstpointer client)
{
	g_assert (NAUTILUS_IS_VFS_DIRECTORY (directory));
	g_assert (client != NULL);
	
	anchor_directory_monitor_remove_internal (directory, NULL, client);
}

static void
vfs_force_reload (AnchorDirectory *directory)
{
	AnchorFileAttributes all_attributes;

	g_assert (NAUTILUS_IS_DIRECTORY (directory));

	all_attributes = anchor_file_get_all_attributes ();
	anchor_directory_force_reload_internal (directory,
						  all_attributes);
}

static gboolean
vfs_are_all_files_seen (AnchorDirectory *directory)
{
	g_assert (NAUTILUS_IS_VFS_DIRECTORY (directory));
	
	return directory->details->directory_loaded;
}

static gboolean
vfs_is_not_empty (AnchorDirectory *directory)
{
	g_assert (NAUTILUS_IS_VFS_DIRECTORY (directory));
	g_assert (anchor_directory_is_anyone_monitoring_file_list (directory));

	return directory->details->file_list != NULL;
}

static void
anchor_vfs_directory_class_init (AnchorVFSDirectoryClass *klass)
{
	AnchorDirectoryClass *directory_class = NAUTILUS_DIRECTORY_CLASS (klass);

	directory_class->contains_file = vfs_contains_file;
	directory_class->call_when_ready = vfs_call_when_ready;
	directory_class->cancel_callback = vfs_cancel_callback;
	directory_class->file_monitor_add = vfs_file_monitor_add;
	directory_class->file_monitor_remove = vfs_file_monitor_remove;
	directory_class->force_reload = vfs_force_reload;
	directory_class->are_all_files_seen = vfs_are_all_files_seen;
	directory_class->is_not_empty = vfs_is_not_empty;
}
