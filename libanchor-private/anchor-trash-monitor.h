/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* 
   anchor-trash-monitor.h: Anchor trash state watcher.
 
   Copyright (C) 2000 Eazel, Inc.
  
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
  
   Author: Pavel Cisler <pavel@eazel.com>
*/

#ifndef NAUTILUS_TRASH_MONITOR_H
#define NAUTILUS_TRASH_MONITOR_H

#include <gtk/gtk.h>
#include <gio/gio.h>

typedef struct AnchorTrashMonitor AnchorTrashMonitor;
typedef struct AnchorTrashMonitorClass AnchorTrashMonitorClass;
typedef struct AnchorTrashMonitorDetails AnchorTrashMonitorDetails;

#define ANCHOR_TYPE_TRASH_MONITOR anchor_trash_monitor_get_type()
#define NAUTILUS_TRASH_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_TRASH_MONITOR, AnchorTrashMonitor))
#define NAUTILUS_TRASH_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_TRASH_MONITOR, AnchorTrashMonitorClass))
#define NAUTILUS_IS_TRASH_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_TRASH_MONITOR))
#define NAUTILUS_IS_TRASH_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_TRASH_MONITOR))
#define NAUTILUS_TRASH_MONITOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_TRASH_MONITOR, AnchorTrashMonitorClass))

struct AnchorTrashMonitor {
	GObject object;
	AnchorTrashMonitorDetails *details;
};

struct AnchorTrashMonitorClass {
	GObjectClass parent_class;

	void (* trash_state_changed)		(AnchorTrashMonitor 	*trash_monitor,
				      		 gboolean 		 new_state);
};

GType			anchor_trash_monitor_get_type				(void);

AnchorTrashMonitor   *anchor_trash_monitor_get 				(void);
gboolean		anchor_trash_monitor_is_empty 			(void);
GIcon                  *anchor_trash_monitor_get_icon                         (void);

#endif
