/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-desktop-directory.h: Subclass of AnchorDirectory to implement
   a virtual directory consisting of the desktop directory and the desktop
   icons
 
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

#ifndef NAUTILUS_DESKTOP_DIRECTORY_H
#define NAUTILUS_DESKTOP_DIRECTORY_H

#include <libanchor-private/anchor-directory.h>

#define ANCHOR_TYPE_DESKTOP_DIRECTORY anchor_desktop_directory_get_type()
#define NAUTILUS_DESKTOP_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_DESKTOP_DIRECTORY, AnchorDesktopDirectory))
#define NAUTILUS_DESKTOP_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_DESKTOP_DIRECTORY, AnchorDesktopDirectoryClass))
#define NAUTILUS_IS_DESKTOP_DIRECTORY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_DESKTOP_DIRECTORY))
#define NAUTILUS_IS_DESKTOP_DIRECTORY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_DESKTOP_DIRECTORY))
#define NAUTILUS_DESKTOP_DIRECTORY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_DESKTOP_DIRECTORY, AnchorDesktopDirectoryClass))

typedef struct AnchorDesktopDirectoryDetails AnchorDesktopDirectoryDetails;

typedef struct {
	AnchorDirectory parent_slot;
	AnchorDesktopDirectoryDetails *details;
} AnchorDesktopDirectory;

typedef struct {
	AnchorDirectoryClass parent_slot;

} AnchorDesktopDirectoryClass;

GType   anchor_desktop_directory_get_type             (void);
AnchorDirectory * anchor_desktop_directory_get_real_directory   (AnchorDesktopDirectory *desktop_directory);

#endif /* NAUTILUS_DESKTOP_DIRECTORY_H */
