/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-desktop-link-monitor.h: singleton that manages the desktop links
    
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

#ifndef NAUTILUS_DESKTOP_LINK_MONITOR_H
#define NAUTILUS_DESKTOP_LINK_MONITOR_H

#include <gtk/gtk.h>
#include <libanchor-private/anchor-desktop-link.h>

#define ANCHOR_TYPE_DESKTOP_LINK_MONITOR anchor_desktop_link_monitor_get_type()
#define NAUTILUS_DESKTOP_LINK_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_DESKTOP_LINK_MONITOR, AnchorDesktopLinkMonitor))
#define NAUTILUS_DESKTOP_LINK_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_DESKTOP_LINK_MONITOR, AnchorDesktopLinkMonitorClass))
#define NAUTILUS_IS_DESKTOP_LINK_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_DESKTOP_LINK_MONITOR))
#define NAUTILUS_IS_DESKTOP_LINK_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_DESKTOP_LINK_MONITOR))
#define NAUTILUS_DESKTOP_LINK_MONITOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_DESKTOP_LINK_MONITOR, AnchorDesktopLinkMonitorClass))

typedef struct AnchorDesktopLinkMonitorDetails AnchorDesktopLinkMonitorDetails;

typedef struct {
	GObject parent_slot;
	AnchorDesktopLinkMonitorDetails *details;
} AnchorDesktopLinkMonitor;

typedef struct {
	GObjectClass parent_slot;
} AnchorDesktopLinkMonitorClass;

GType   anchor_desktop_link_monitor_get_type (void);

AnchorDesktopLinkMonitor *   anchor_desktop_link_monitor_get (void);
void anchor_desktop_link_monitor_shutdown (void);

void anchor_desktop_link_monitor_delete_link (AnchorDesktopLinkMonitor *monitor,
						AnchorDesktopLink *link,
						GtkWidget *parent_view);

/* Used by anchor-desktop-link.c */
char * anchor_desktop_link_monitor_make_filename_unique (AnchorDesktopLinkMonitor *monitor,
							   const char *filename);

#endif /* NAUTILUS_DESKTOP_LINK_MONITOR_H */
