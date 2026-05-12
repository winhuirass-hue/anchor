/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-search-directory-file.h: Subclass of AnchorFile to implement the
   the case of the search directory
 
   Copyright (C) 2003 Red Hat, Inc.
  
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
  
   Author: Alexander Larsson <alexl@redhat.com>
*/

#ifndef NAUTILUS_SEARCH_DIRECTORY_FILE_H
#define NAUTILUS_SEARCH_DIRECTORY_FILE_H

#include <libanchor-private/anchor-file.h>

#define ANCHOR_TYPE_SEARCH_DIRECTORY_FILE anchor_search_directory_file_get_type()
#define NAUTILUS_SEARCH_DIRECTORY_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_SEARCH_DIRECTORY_FILE, AnchorSearchDirectoryFile))
#define NAUTILUS_SEARCH_DIRECTORY_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_SEARCH_DIRECTORY_FILE, AnchorSearchDirectoryFileClass))
#define NAUTILUS_IS_SEARCH_DIRECTORY_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_SEARCH_DIRECTORY_FILE))
#define NAUTILUS_IS_SEARCH_DIRECTORY_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_SEARCH_DIRECTORY_FILE))
#define NAUTILUS_SEARCH_DIRECTORY_FILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_SEARCH_DIRECTORY_FILE, AnchorSearchDirectoryFileClass))

typedef struct AnchorSearchDirectoryFileDetails AnchorSearchDirectoryFileDetails;

typedef struct {
	AnchorFile parent_slot;
	AnchorSearchDirectoryFileDetails *details;
} AnchorSearchDirectoryFile;

typedef struct {
	AnchorFileClass parent_slot;
} AnchorSearchDirectoryFileClass;

GType   anchor_search_directory_file_get_type (void);
void    anchor_search_directory_file_update_display_name (AnchorSearchDirectoryFile *search_file);

#endif /* NAUTILUS_SEARCH_DIRECTORY_FILE_H */
