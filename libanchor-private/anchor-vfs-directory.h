/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-vfs-directory.h: Subclass of AnchorDirectory to implement the
   the case of a VFS directory.
 
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

#ifndef NAUTILUS_VFS_DIRECTORY_H
#define NAUTILUS_VFS_DIRECTORY_H

#include <libanchor-private/anchor-directory.h>

#define ANCHOR_TYPE_VFS_DIRECTORY anchor_vfs_directory_get_type()
#define NAUTILUS_VFS_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_VFS_DIRECTORY, AnchorVFSDirectory))
#define NAUTILUS_VFS_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_VFS_DIRECTORY, AnchorVFSDirectoryClass))
#define NAUTILUS_IS_VFS_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_VFS_DIRECTORY))
#define NAUTILUS_IS_VFS_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_VFS_DIRECTORY))
#define NAUTILUS_VFS_DIRECTORY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_VFS_DIRECTORY, AnchorVFSDirectoryClass))

typedef struct AnchorVFSDirectoryDetails AnchorVFSDirectoryDetails;

typedef struct {
	AnchorDirectory parent_slot;
} AnchorVFSDirectory;

typedef struct {
	AnchorDirectoryClass parent_slot;
} AnchorVFSDirectoryClass;

GType   anchor_vfs_directory_get_type (void);

#endif /* NAUTILUS_VFS_DIRECTORY_H */
