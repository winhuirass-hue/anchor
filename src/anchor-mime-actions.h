/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* anchor-mime-actions.h - uri-specific versions of mime action functions

   Copyright (C) 2000 Eazel, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   see <http://www.gnu.org/licenses/>.

   Authors: Maciej Stachowiak <mjs@eazel.com>
*/

#ifndef NAUTILUS_MIME_ACTIONS_H
#define NAUTILUS_MIME_ACTIONS_H

#include <gio/gio.h>

#include <libanchor-private/anchor-file.h>

#include "anchor-window.h"

AnchorFileAttributes anchor_mime_actions_get_required_file_attributes (void);

GAppInfo *             anchor_mime_get_default_application_for_file     (AnchorFile            *file);
GList *                anchor_mime_get_applications_for_file            (AnchorFile            *file);

GAppInfo *             anchor_mime_get_default_application_for_files    (GList                   *files);
GList *                anchor_mime_get_applications_for_files           (GList                   *file);

gboolean               anchor_mime_file_opens_in_view                   (AnchorFile            *file);
gboolean               anchor_mime_file_opens_in_external_app           (AnchorFile            *file);
gboolean               anchor_mime_file_launches                        (AnchorFile            *file);
void                   anchor_mime_activate_files                       (GtkWindow               *parent_window,
									   AnchorWindowSlot      *slot,
									   GList                   *files,
									   const char              *launch_directory,
									   AnchorWindowOpenFlags  flags,
									   gboolean                 user_confirmation);
void                   anchor_mime_activate_file                        (GtkWindow               *parent_window,
									   AnchorWindowSlot      *slot_info,
									   AnchorFile            *file,
									   const char              *launch_directory,
									   AnchorWindowOpenFlags  flags);


#endif /* NAUTILUS_MIME_ACTIONS_H */
