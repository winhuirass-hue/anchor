/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-desktop-file.h: Subclass of AnchorFile to implement the
   the case of a desktop icon file
 
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

#ifndef NAUTILUS_DESKTOP_ICON_FILE_H
#define NAUTILUS_DESKTOP_ICON_FILE_H

#include <libanchor-private/anchor-file.h>
#include <libanchor-private/anchor-desktop-link.h>

#define ANCHOR_TYPE_DESKTOP_ICON_FILE anchor_desktop_icon_file_get_type()
#define NAUTILUS_DESKTOP_ICON_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_DESKTOP_ICON_FILE, AnchorDesktopIconFile))
#define NAUTILUS_DESKTOP_ICON_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_DESKTOP_ICON_FILE, AnchorDesktopIconFileClass))
#define NAUTILUS_IS_DESKTOP_ICON_FILE(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_DESKTOP_ICON_FILE))
#define NAUTILUS_IS_DESKTOP_ICON_FILE_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_DESKTOP_ICON_FILE))
#define NAUTILUS_DESKTOP_ICON_FILE_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_DESKTOP_ICON_FILE, AnchorDesktopIconFileClass))

typedef struct AnchorDesktopIconFileDetails AnchorDesktopIconFileDetails;

typedef struct {
	AnchorFile parent_slot;
	AnchorDesktopIconFileDetails *details;
} AnchorDesktopIconFile;

typedef struct {
	AnchorFileClass parent_slot;
} AnchorDesktopIconFileClass;

GType   anchor_desktop_icon_file_get_type (void);

AnchorDesktopIconFile *anchor_desktop_icon_file_new      (AnchorDesktopLink     *link);
void                     anchor_desktop_icon_file_update   (AnchorDesktopIconFile *icon_file);
void                     anchor_desktop_icon_file_remove   (AnchorDesktopIconFile *icon_file);
AnchorDesktopLink     *anchor_desktop_icon_file_get_link (AnchorDesktopIconFile *icon_file);

#endif /* NAUTILUS_DESKTOP_ICON_FILE_H */
