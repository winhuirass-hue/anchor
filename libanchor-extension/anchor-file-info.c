/*
 *  anchor-file-info.c - Information about a file 
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
 */

#include <config.h>
#include "anchor-file-info.h"
#include "anchor-extension-private.h"

AnchorFileInfo *(*anchor_file_info_getter) (GFile *location, gboolean create);

/**
 * anchor_file_info_list_copy:
 * @files: (element-type AnchorFileInfo): the files to copy
 *
 * Returns: (element-type AnchorFileInfo) (transfer full): a copy of @files.
 *  Use #anchor_file_info_list_free to free the list and unref its contents.
 */
GList *
anchor_file_info_list_copy (GList *files)
{
	GList *ret;
	GList *l;
	
	ret = g_list_copy (files);
	for (l = ret; l != NULL; l = l->next) {
		g_object_ref (G_OBJECT (l->data));
	}

	return ret;
}

/**
 * anchor_file_info_list_free:
 * @files: (element-type AnchorFileInfo): a list created with
 *   #anchor_file_info_list_copy
 *
 */
void              
anchor_file_info_list_free (GList *files)
{
	GList *l;
	
	for (l = files; l != NULL; l = l->next) {
		g_object_unref (G_OBJECT (l->data));
	}
	
	g_list_free (files);
}

static void
anchor_file_info_base_init (gpointer g_class)
{
}

GType                   
anchor_file_info_get_type (void)
{
	static GType type = 0;

	if (!type) {
		const GTypeInfo info = {
			sizeof (AnchorFileInfoIface),
			anchor_file_info_base_init,
			NULL,
			NULL,
			NULL,
			NULL,
			0,
			0,
			NULL
		};
		
		type = g_type_register_static (G_TYPE_INTERFACE, 
					       "AnchorFileInfo",
					       &info, 0);
		g_type_interface_add_prerequisite (type, G_TYPE_OBJECT);
	}

	return type;
}

gboolean
anchor_file_info_is_gone (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), FALSE);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->is_gone != NULL, FALSE);
	
	return ANCHOR_FILE_INFO_GET_IFACE (file)->is_gone (file);
}

GFileType
anchor_file_info_get_file_type (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), G_FILE_TYPE_UNKNOWN);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_file_type != NULL, G_FILE_TYPE_UNKNOWN);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_file_type (file);
}

char *
anchor_file_info_get_name (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_name != NULL, NULL);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_name (file);
}

/**
 * anchor_file_info_get_location:
 * @file: a #AnchorFileInfo
 *
 * Returns: (transfer full): a #GFile for the location of @file
 */
GFile *
anchor_file_info_get_location (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_location != NULL, NULL);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_location (file);
}
char *
anchor_file_info_get_uri (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_uri != NULL, NULL);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_uri (file);
}

char *
anchor_file_info_get_activation_uri (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_activation_uri != NULL, NULL);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_activation_uri (file);
}

/**
 * anchor_file_info_get_parent_location:
 * @file: a #AnchorFileInfo
 *
 * Returns: (allow-none) (transfer full): a #GFile for the parent location of @file, 
 *   or %NULL if @file has no parent
 */
GFile *
anchor_file_info_get_parent_location (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_parent_location != NULL, NULL);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_parent_location (file);
}

char *
anchor_file_info_get_parent_uri (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_parent_uri != NULL, NULL);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_parent_uri (file);
}

/**
 * anchor_file_info_get_parent_info:
 * @file: a #AnchorFileInfo
 *
 * Returns: (allow-none) (transfer full): a #AnchorFileInfo for the parent of @file, 
 *   or %NULL if @file has no parent
 */
AnchorFileInfo *
anchor_file_info_get_parent_info (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_parent_info != NULL, NULL);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_parent_info (file);
}

/**
 * anchor_file_info_get_mount:
 * @file: a #AnchorFileInfo
 *
 * Returns: (allow-none) (transfer full): a #GMount for the mount of @file, 
 *   or %NULL if @file has no mount
 */
GMount *
anchor_file_info_get_mount (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_mount != NULL, NULL);
    
	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_mount (file);
}

char *
anchor_file_info_get_uri_scheme (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_uri_scheme != NULL, NULL);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_uri_scheme (file);
}

char *
anchor_file_info_get_mime_type (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_mime_type != NULL, NULL);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_mime_type (file);
}

gboolean
anchor_file_info_is_mime_type (AnchorFileInfo *file,
				 const char *mime_type)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), FALSE);
	g_return_val_if_fail (mime_type != NULL, FALSE);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->is_mime_type != NULL, FALSE);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->is_mime_type (file,
								  mime_type);
}

gboolean
anchor_file_info_is_directory (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), FALSE);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->is_directory != NULL, FALSE);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->is_directory (file);
}

gboolean
anchor_file_info_can_write (AnchorFileInfo *file)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), FALSE);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->can_write != NULL, FALSE);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->can_write (file);
}

void
anchor_file_info_add_emblem (AnchorFileInfo *file,
			       const char *emblem_name)
{
	g_return_if_fail (NAUTILUS_IS_FILE_INFO (file));
	g_return_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->add_emblem != NULL);

	ANCHOR_FILE_INFO_GET_IFACE (file)->add_emblem (file, emblem_name);
}

char *
anchor_file_info_get_string_attribute (AnchorFileInfo *file,
					 const char *attribute_name)
{
	g_return_val_if_fail (NAUTILUS_IS_FILE_INFO (file), NULL);
	g_return_val_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->get_string_attribute != NULL, NULL);
	g_return_val_if_fail (attribute_name != NULL, NULL);

	return ANCHOR_FILE_INFO_GET_IFACE (file)->get_string_attribute 
		(file, attribute_name);
}

void
anchor_file_info_add_string_attribute (AnchorFileInfo *file,
					 const char *attribute_name,
					 const char *value)
{
	g_return_if_fail (NAUTILUS_IS_FILE_INFO (file));
	g_return_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->add_string_attribute != NULL);
	g_return_if_fail (attribute_name != NULL);
	g_return_if_fail (value != NULL);
	
	ANCHOR_FILE_INFO_GET_IFACE (file)->add_string_attribute 
		(file, attribute_name, value);
}

void
anchor_file_info_invalidate_extension_info (AnchorFileInfo *file)
{
	g_return_if_fail (NAUTILUS_IS_FILE_INFO (file));
	g_return_if_fail (ANCHOR_FILE_INFO_GET_IFACE (file)->invalidate_extension_info != NULL);
	
	ANCHOR_FILE_INFO_GET_IFACE (file)->invalidate_extension_info (file);
}

/**
 * anchor_file_info_lookup:
 * @location: the location to lookup the file info for
 *
 * Returns: (transfer full): a #AnchorFileInfo
 */
AnchorFileInfo *
anchor_file_info_lookup (GFile *location)
{
	return anchor_file_info_getter (location, FALSE);
}

/**
 * anchor_file_info_create:
 * @location: the location to create the file info for
 *
 * Returns: (transfer full): a #AnchorFileInfo
 */
AnchorFileInfo *
anchor_file_info_create (GFile *location)
{
	return anchor_file_info_getter (location, TRUE);
}

/**
 * anchor_file_info_lookup_for_uri:
 * @uri: the URI to lookup the file info for
 *
 * Returns: (transfer full): a #AnchorFileInfo
 */
AnchorFileInfo *
anchor_file_info_lookup_for_uri (const char *uri)
{
	GFile *location;
	AnchorFile *file;

	location = g_file_new_for_uri (uri);
	file = anchor_file_info_lookup (location);
	g_object_unref (location);

	return file;
}

/**
 * anchor_file_info_create_for_uri:
 * @uri: the URI to lookup the file info for
 *
 * Returns: (transfer full): a #AnchorFileInfo
 */
AnchorFileInfo *
anchor_file_info_create_for_uri (const char *uri)
{
	GFile *location;
	AnchorFile *file;

	location = g_file_new_for_uri (uri);
	file = anchor_file_info_create (location);
	g_object_unref (location);

	return file;
}
