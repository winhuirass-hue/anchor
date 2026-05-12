/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-progress-info.h: file operation progress info.
 
   Copyright (C) 2007 Red Hat, Inc.
  
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

#ifndef NAUTILUS_PROGRESS_INFO_H
#define NAUTILUS_PROGRESS_INFO_H

#include <glib-object.h>
#include <gio/gio.h>

#define ANCHOR_TYPE_PROGRESS_INFO         (anchor_progress_info_get_type ())
#define NAUTILUS_PROGRESS_INFO(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_PROGRESS_INFO, AnchorProgressInfo))
#define NAUTILUS_PROGRESS_INFO_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_PROGRESS_INFO, AnchorProgressInfoClass))
#define NAUTILUS_IS_PROGRESS_INFO(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_PROGRESS_INFO))
#define NAUTILUS_IS_PROGRESS_INFO_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_PROGRESS_INFO))
#define NAUTILUS_PROGRESS_INFO_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_PROGRESS_INFO, AnchorProgressInfoClass))

typedef struct _NautilusProgressInfo      AnchorProgressInfo;
typedef struct _NautilusProgressInfoClass AnchorProgressInfoClass;

GType anchor_progress_info_get_type (void) G_GNUC_CONST;

/* Signals:
   "changed" - status or details changed
   "progress-changed" - the percentage progress changed (or we pulsed if in activity_mode
   "started" - emited on job start
   "finished" - emitted when job is done
   
   All signals are emitted from idles in main loop.
   All methods are threadsafe.
 */

AnchorProgressInfo *anchor_progress_info_new (void);

GList *       anchor_get_all_progress_info (void);

char *        anchor_progress_info_get_status      (AnchorProgressInfo *info);
char *        anchor_progress_info_get_details     (AnchorProgressInfo *info);
double        anchor_progress_info_get_progress    (AnchorProgressInfo *info);
GCancellable *anchor_progress_info_get_cancellable (AnchorProgressInfo *info);
void          anchor_progress_info_cancel          (AnchorProgressInfo *info);
gboolean      anchor_progress_info_get_is_started  (AnchorProgressInfo *info);
gboolean      anchor_progress_info_get_is_finished (AnchorProgressInfo *info);
gboolean      anchor_progress_info_get_is_paused   (AnchorProgressInfo *info);

void          anchor_progress_info_start           (AnchorProgressInfo *info);
void          anchor_progress_info_finish          (AnchorProgressInfo *info);
void          anchor_progress_info_pause           (AnchorProgressInfo *info);
void          anchor_progress_info_resume          (AnchorProgressInfo *info);
void          anchor_progress_info_set_status      (AnchorProgressInfo *info,
						      const char           *status);
void          anchor_progress_info_take_status     (AnchorProgressInfo *info,
						      char                 *status);
void          anchor_progress_info_set_details     (AnchorProgressInfo *info,
						      const char           *details);
void          anchor_progress_info_take_details    (AnchorProgressInfo *info,
						      char                 *details);
void          anchor_progress_info_set_progress    (AnchorProgressInfo *info,
						      double                current,
						      double                total);
void          anchor_progress_info_pulse_progress  (AnchorProgressInfo *info);



#endif /* NAUTILUS_PROGRESS_INFO_H */
