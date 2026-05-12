/*
 *  anchor-property-page-provider.c - Interface for Anchor extensions 
 *                                      that provide context menu items
 *                                      for files.
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

#include <config.h>
#include "anchor-menu-provider.h"

#include <glib-object.h>

static void
anchor_menu_provider_base_init (gpointer g_class)
{
	static gboolean initialized = FALSE;

	if (!initialized)
	{
		/* This signal should be emited each time the extension modify the list of menu items */
		g_signal_new ("items-updated",
			ANCHOR_TYPE_MENU_PROVIDER,
			G_SIGNAL_RUN_LAST,
			0,
			NULL, NULL,
			g_cclosure_marshal_VOID__VOID,
			G_TYPE_NONE, 0);
		initialized = TRUE;
	}
}

GType                   
anchor_menu_provider_get_type (void)
{
	static GType type = 0;

	if (!type) {
		const GTypeInfo info = {
			sizeof (AnchorMenuProviderIface),
			anchor_menu_provider_base_init,
			NULL,
			NULL,
			NULL,
			NULL,
			0,
			0,
			NULL
		};
		
		type = g_type_register_static (G_TYPE_INTERFACE, 
					       "AnchorMenuProvider",
					       &info, 0);
		g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
	}

	return type;
}

/**
 * anchor_menu_provider_get_file_items:
 * @provider: a #AnchorMenuProvider
 * @window: the parent #GtkWidget window
 * @files: (element-type AnchorFileInfo): a list of #AnchorFileInfo
 *
 * Returns: (element-type AnchorMenuItem) (transfer full): the provided list of #AnchorMenuItem
 */
GList *
anchor_menu_provider_get_file_items (AnchorMenuProvider *provider,
				       GtkWidget *window,
				       GList *files)
{
	g_return_val_if_fail (NAUTILUS_IS_MENU_PROVIDER (provider), NULL);

	if (ANCHOR_MENU_PROVIDER_GET_IFACE (provider)->get_file_items) {
		return ANCHOR_MENU_PROVIDER_GET_IFACE (provider)->get_file_items 
			(provider, window, files);
	} else {
		return NULL;
	}
}

/**
 * anchor_menu_provider_get_background_items:
 * @provider: a #AnchorMenuProvider
 * @window: the parent #GtkWidget window
 * @current_folder: the folder for which background items are requested
 *
 * Returns: (element-type AnchorMenuItem) (transfer full): the provided list of #AnchorMenuItem
 */
GList *
anchor_menu_provider_get_background_items (AnchorMenuProvider *provider,
					     GtkWidget *window,
					     AnchorFileInfo *current_folder)
{
	g_return_val_if_fail (NAUTILUS_IS_MENU_PROVIDER (provider), NULL);
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (current_folder), NULL);

	if (ANCHOR_MENU_PROVIDER_GET_IFACE (provider)->get_background_items) {
		return ANCHOR_MENU_PROVIDER_GET_IFACE (provider)->get_background_items 
			(provider, window, current_folder);
	} else {
		return NULL;
	}
}

/* This function emit a signal to inform anchor that its item list has changed */
void
anchor_menu_provider_emit_items_updated_signal (AnchorMenuProvider* provider)
{
	g_return_if_fail (NAUTILUS_IS_MENU_PROVIDER (provider));

	g_signal_emit_by_name (provider, "items-updated");
}

