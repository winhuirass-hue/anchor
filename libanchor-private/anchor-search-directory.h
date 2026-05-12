/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-search-directory.h: Subclass of AnchorDirectory to implement
   a virtual directory consisting of the search directory and the search
   icons
 
   Copyright (C) 2005 Novell, Inc
  
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
*/

#ifndef NAUTILUS_SEARCH_DIRECTORY_H
#define NAUTILUS_SEARCH_DIRECTORY_H

#include <libanchor-private/anchor-directory.h>
#include <libanchor-private/anchor-query.h>

#define ANCHOR_TYPE_SEARCH_DIRECTORY anchor_search_directory_get_type()
#define NAUTILUS_SEARCH_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_SEARCH_DIRECTORY, AnchorSearchDirectory))
#define NAUTILUS_SEARCH_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_SEARCH_DIRECTORY, AnchorSearchDirectoryClass))
#define NAUTILUS_IS_SEARCH_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_SEARCH_DIRECTORY))
#define NAUTILUS_IS_SEARCH_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_SEARCH_DIRECTORY))
#define NAUTILUS_SEARCH_DIRECTORY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_SEARCH_DIRECTORY, AnchorSearchDirectoryClass))

typedef struct AnchorSearchDirectoryDetails AnchorSearchDirectoryDetails;

typedef struct {
	AnchorDirectory parent_slot;
	AnchorSearchDirectoryDetails *details;
} AnchorSearchDirectory;

typedef struct {
	AnchorDirectoryClass parent_slot;
} AnchorSearchDirectoryClass;

GType   anchor_search_directory_get_type             (void);

char   *anchor_search_directory_generate_new_uri     (void);

void           anchor_search_directory_set_saved_search (AnchorSearchDirectory *search,
							   GFile                   *saved_search);
gboolean       anchor_search_directory_is_saved_search (AnchorSearchDirectory *search);
gboolean       anchor_search_directory_is_modified     (AnchorSearchDirectory *search);
void           anchor_search_directory_save_search     (AnchorSearchDirectory *search);
void           anchor_search_directory_save_to_file    (AnchorSearchDirectory *search,
							  const char              *save_file_uri);

AnchorQuery *anchor_search_directory_get_query       (AnchorSearchDirectory *search);
void           anchor_search_directory_set_query       (AnchorSearchDirectory *search,
							  AnchorQuery           *query);

AnchorDirectory *
               anchor_search_directory_get_base_model (AnchorSearchDirectory  *search);
void           anchor_search_directory_set_base_model (AnchorSearchDirectory  *search,
							 AnchorDirectory        *base_model);

#endif /* NAUTILUS_SEARCH_DIRECTORY_H */
