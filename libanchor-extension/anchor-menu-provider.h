/*
 *  anchor-menu-provider.h - Interface for Anchor extensions that 
 *                             provide context menu items.
 *
 *  Copyright (C) 2003 Novell, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 * 
 *  Author:  Dave Camp <dave@ximian.com>
 *
 */

/* This interface is implemented by Anchor extensions that want to
 * add context menu entries to files.  Extensions are called when
 * Anchor constructs the context menu for a file.  They are passed a
 * list of AnchorFileInfo objects which holds the current selection */

#ifndef ANCHOR_MENU_PROVIDER_H
#define ANCHOR_MENU_PROVIDER_H

#include <glib-object.h>
#include <gtk/gtk.h>
#include "anchor-extension-types.h"
#include "anchor-file-info.h"
#include "anchor-menu.h"

G_BEGIN_DECLS

#define ANCHOR_TYPE_MENU_PROVIDER           (anchor_menu_provider_get_type ())
#define ANCHOR_MENU_PROVIDER(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_MENU_PROVIDER, AnchorMenuProvider))
#define NAUTILUS_IS_MENU_PROVIDER(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_MENU_PROVIDER))
#define ANCHOR_MENU_PROVIDER_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), ANCHOR_TYPE_MENU_PROVIDER, AnchorMenuProviderIface))

typedef struct _AnchorMenuProvider       AnchorMenuProvider;
typedef struct _AnchorMenuProviderIface  AnchorMenuProviderIface;

struct _AnchorMenuProviderIface {
	GTypeInterface g_iface;

	GList *(*get_file_items)       (AnchorMenuProvider *provider,
					GtkWidget            *window,
					GList                *files);
	GList *(*get_background_items) (AnchorMenuProvider *provider,
					GtkWidget            *window,
					AnchorFileInfo     *current_folder);
};

/* Interface Functions */
GType                   anchor_menu_provider_get_type             (void);
GList                  *anchor_menu_provider_get_file_items       (AnchorMenuProvider *provider,
								     GtkWidget            *window,
								     GList                *files);
GList                  *anchor_menu_provider_get_background_items (AnchorMenuProvider *provider,
								     GtkWidget            *window,
								     AnchorFileInfo     *current_folder);

/* This function emit a signal to inform anchor that its item list has changed. */
void                    anchor_menu_provider_emit_items_updated_signal (AnchorMenuProvider *provider);

G_END_DECLS

#endif
