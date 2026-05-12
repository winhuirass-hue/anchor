/*
 *  anchor-property-page-provider.c - Interface for Anchor extensions
 *                                      that provide property pages for 
 *                                      files.
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
#include "anchor-property-page-provider.h"

#include <glib-object.h>

static void
anchor_property_page_provider_base_init (gpointer g_class)
{
}

GType                   
anchor_property_page_provider_get_type (void)
{
	static GType type = 0;

	if (!type) {
		const GTypeInfo info = {
			sizeof (AnchorPropertyPageProviderIface),
			anchor_property_page_provider_base_init,
			NULL,
			NULL,
			NULL,
			NULL,
			0,
			0,
			NULL
		};
		
		type = g_type_register_static (G_TYPE_INTERFACE, 
					       "AnchorPropertyPageProvider",
					       &info, 0);
		g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
	}

	return type;
}

/**
 * anchor_property_page_provider_get_pages:
 * @provider: a #AnchorPropertyPageProvider
 * @files: (element-type AnchorFileInfo): a #GList of #AnchorFileInfo
 *
 * This function is called by Anchor when it wants property page
 * items from the extension.
 *
 * This function is called in the main thread before a property page
 * is shown, so it should return quickly.
 *
 * Returns: (element-type AnchorPropertyPage) (transfer full): A #GList of allocated #AnchorPropertyPage items.
 */
GList *
anchor_property_page_provider_get_pages (AnchorPropertyPageProvider *provider,
					   GList *files)
{
	g_return_val_if_fail (NAUTILUS_IS_PROPERTY_PAGE_PROVIDER (provider), NULL);
	g_return_val_if_fail (NAUTILUS_PROPERTY_PAGE_PROVIDER_GET_IFACE (provider)->get_pages != NULL, NULL);

	return NAUTILUS_PROPERTY_PAGE_PROVIDER_GET_IFACE (provider)->get_pages 
		(provider, files);	
}

					       
