/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * anchor-application: main Anchor application class.
 *
 * Copyright (C) 2000 Red Hat, Inc.
 * Copyright (C) 2010 Cosimo Cecchi <cosimoc@gnome.org>
 *
 * Anchor is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Anchor is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef __NAUTILUS_APPLICATION_H__
#define __NAUTILUS_APPLICATION_H__

#include <gdk/gdk.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#include "anchor-bookmark-list.h"
#include "anchor-window.h"

#define NAUTILUS_DESKTOP_ICON_VIEW_IID	"OAFIID:Nautilus_File_Manager_Desktop_Canvas_View"

#define ANCHOR_TYPE_APPLICATION anchor_application_get_type()
#define NAUTILUS_APPLICATION(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_APPLICATION, AnchorApplication))
#define NAUTILUS_APPLICATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_APPLICATION, AnchorApplicationClass))
#define NAUTILUS_IS_APPLICATION(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_APPLICATION))
#define NAUTILUS_IS_APPLICATION_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_APPLICATION))
#define NAUTILUS_APPLICATION_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_APPLICATION, AnchorApplicationClass))

typedef struct _NautilusApplicationPriv AnchorApplicationPriv;

typedef struct {
	GtkApplication parent;

	AnchorApplicationPriv *priv;
} AnchorApplication;

typedef struct {
	GtkApplicationClass parent_class;
} AnchorApplicationClass;

GType anchor_application_get_type (void);

AnchorApplication * anchor_application_new (void);

AnchorWindow *     anchor_application_create_window (AnchorApplication *application,
							 GdkScreen           *screen);

void anchor_application_open_location (AnchorApplication *application,
					 GFile *location,
					 GFile *selection,
					 const char *startup_id);

void anchor_application_notify_unmount_show (AnchorApplication *application,
					       const gchar *message);

void anchor_application_notify_unmount_done (AnchorApplication *application,
					       const gchar *message);

AnchorBookmarkList *
     anchor_application_get_bookmarks  (AnchorApplication *application);
void anchor_application_edit_bookmarks (AnchorApplication *application,
					  AnchorWindow      *window);

GtkWidget * anchor_application_connect_server (AnchorApplication *application,
						 AnchorWindow      *window);

#endif /* __NAUTILUS_APPLICATION_H__ */
