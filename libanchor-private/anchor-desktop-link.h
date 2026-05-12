/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-desktop-link.h: Class that handles the links on the desktop
    
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

#ifndef NAUTILUS_DESKTOP_LINK_H
#define NAUTILUS_DESKTOP_LINK_H

#include <libanchor-private/anchor-file.h>
#include <gio/gio.h>

#define ANCHOR_TYPE_DESKTOP_LINK anchor_desktop_link_get_type()
#define NAUTILUS_DESKTOP_LINK(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_DESKTOP_LINK, AnchorDesktopLink))
#define NAUTILUS_DESKTOP_LINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_DESKTOP_LINK, AnchorDesktopLinkClass))
#define NAUTILUS_IS_DESKTOP_LINK(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_DESKTOP_LINK))
#define NAUTILUS_IS_DESKTOP_LINK_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_DESKTOP_LINK))
#define NAUTILUS_DESKTOP_LINK_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_DESKTOP_LINK, AnchorDesktopLinkClass))

typedef struct AnchorDesktopLinkDetails AnchorDesktopLinkDetails;

typedef struct {
	GObject parent_slot;
	AnchorDesktopLinkDetails *details;
} AnchorDesktopLink;

typedef struct {
	GObjectClass parent_slot;
} AnchorDesktopLinkClass;

typedef enum {
	NAUTILUS_DESKTOP_LINK_HOME,
	NAUTILUS_DESKTOP_LINK_TRASH,
	NAUTILUS_DESKTOP_LINK_MOUNT,
	NAUTILUS_DESKTOP_LINK_NETWORK
} AnchorDesktopLinkType;

GType   anchor_desktop_link_get_type (void);

AnchorDesktopLink *   anchor_desktop_link_new                     (AnchorDesktopLinkType  type);
AnchorDesktopLink *   anchor_desktop_link_new_from_mount          (GMount                 *mount);
AnchorDesktopLinkType anchor_desktop_link_get_link_type           (AnchorDesktopLink     *link);
char *                  anchor_desktop_link_get_file_name           (AnchorDesktopLink     *link);
char *                  anchor_desktop_link_get_display_name        (AnchorDesktopLink     *link);
GIcon *                 anchor_desktop_link_get_icon                (AnchorDesktopLink     *link);
GFile *                 anchor_desktop_link_get_activation_location (AnchorDesktopLink     *link);
char *                  anchor_desktop_link_get_activation_uri      (AnchorDesktopLink     *link);
gboolean                anchor_desktop_link_get_date                (AnchorDesktopLink     *link,
								       AnchorDateType         date_type,
								       time_t                  *date);
GMount *                anchor_desktop_link_get_mount               (AnchorDesktopLink     *link);
gboolean                anchor_desktop_link_can_rename              (AnchorDesktopLink     *link);
gboolean                anchor_desktop_link_rename                  (AnchorDesktopLink     *link,
								       const char              *name);


#endif /* NAUTILUS_DESKTOP_LINK_H */
