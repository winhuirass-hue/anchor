/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Anchor
 *
 * Copyright (C) 2003 Red Hat, Inc.
 * Copyright (C) 2010 Cosimo Cecchi <cosimoc@gnome.org>
 *
 * Anchor is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Anchor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * see <http://www.gnu.org/licenses/>.
 */

#ifndef NAUTILUS_CONNECT_SERVER_DIALOG_H
#define NAUTILUS_CONNECT_SERVER_DIALOG_H

#include <gio/gio.h>
#include <gtk/gtk.h>

#include "anchor-application.h"
#include "anchor-window.h"

#define ANCHOR_TYPE_CONNECT_SERVER_DIALOG\
	(anchor_connect_server_dialog_get_type ())
#define NAUTILUS_CONNECT_SERVER_DIALOG(obj)\
        (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_CONNECT_SERVER_DIALOG,\
				     AnchorConnectServerDialog))
#define NAUTILUS_CONNECT_SERVER_DIALOG_CLASS(klass)\
	(G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_CONNECT_SERVER_DIALOG,\
				  AnchorConnectServerDialogClass))
#define NAUTILUS_IS_CONNECT_SERVER_DIALOG(obj)\
	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_CONNECT_SERVER_DIALOG)

typedef struct _NautilusConnectServerDialog AnchorConnectServerDialog;
typedef struct _NautilusConnectServerDialogClass AnchorConnectServerDialogClass;
typedef struct _NautilusConnectServerDialogDetails AnchorConnectServerDialogDetails;

struct _NautilusConnectServerDialog {
	GtkDialog parent;
	AnchorConnectServerDialogDetails *details;
};

struct _NautilusConnectServerDialogClass {
	GtkDialogClass parent_class;
};

GType anchor_connect_server_dialog_get_type (void);

GtkWidget * anchor_connect_server_dialog_new             (AnchorWindow *window);
GFile *     anchor_connect_server_dialog_get_location    (AnchorConnectServerDialog *dialog);
void        anchor_connect_server_dialog_set_show_browse (AnchorConnectServerDialog *dialog,
							    gboolean                     show);

#endif /* NAUTILUS_CONNECT_SERVER_DIALOG_H */
