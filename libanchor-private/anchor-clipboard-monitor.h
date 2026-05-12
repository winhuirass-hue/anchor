/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-clipboard-monitor.h: lets you notice clipboard changes.
    
   Copyright (C) 2004 Red Hat, Inc.
  
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

#ifndef NAUTILUS_CLIPBOARD_MONITOR_H
#define NAUTILUS_CLIPBOARD_MONITOR_H

#include <gtk/gtk.h>

#define ANCHOR_TYPE_CLIPBOARD_MONITOR anchor_clipboard_monitor_get_type()
#define NAUTILUS_CLIPBOARD_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_CLIPBOARD_MONITOR, AnchorClipboardMonitor))
#define NAUTILUS_CLIPBOARD_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_CLIPBOARD_MONITOR, AnchorClipboardMonitorClass))
#define NAUTILUS_IS_CLIPBOARD_MONITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_CLIPBOARD_MONITOR))
#define NAUTILUS_IS_CLIPBOARD_MONITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_CLIPBOARD_MONITOR))
#define NAUTILUS_CLIPBOARD_MONITOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_CLIPBOARD_MONITOR, AnchorClipboardMonitorClass))

typedef struct AnchorClipboardMonitorDetails AnchorClipboardMonitorDetails;
typedef struct AnchorClipboardInfo AnchorClipboardInfo;

typedef struct {
	GObject parent_slot;

	AnchorClipboardMonitorDetails *details;
} AnchorClipboardMonitor;

typedef struct {
	GObjectClass parent_slot;
  
	void (* clipboard_changed) (AnchorClipboardMonitor *monitor);
	void (* clipboard_info) (AnchorClipboardMonitor *monitor,
	                         AnchorClipboardInfo *info);
} AnchorClipboardMonitorClass;

struct AnchorClipboardInfo {
	GList *files;
	gboolean cut;
};

GType   anchor_clipboard_monitor_get_type (void);

AnchorClipboardMonitor *   anchor_clipboard_monitor_get (void);
void anchor_clipboard_monitor_set_clipboard_info (AnchorClipboardMonitor *monitor,
                                                    AnchorClipboardInfo *info);
AnchorClipboardInfo * anchor_clipboard_monitor_get_clipboard_info (AnchorClipboardMonitor *monitor);
void anchor_clipboard_monitor_emit_changed (void);

void anchor_clear_clipboard_callback (GtkClipboard *clipboard,
                                        gpointer      user_data);
void anchor_get_clipboard_callback   (GtkClipboard     *clipboard,
                                        GtkSelectionData *selection_data,
                                        guint             info,
                                        gpointer          user_data);



#endif /* NAUTILUS_CLIPBOARD_MONITOR_H */

