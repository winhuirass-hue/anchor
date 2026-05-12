/*
 *  anchor-file-info.h - Information about a file 
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

/* AnchorFileInfo is an interface to the AnchorFile object.  It 
 * provides access to the asynchronous data in the AnchorFile.
 * Extensions are passed objects of this type for operations. */

#ifndef ANCHOR_FILE_INFO_H
#define ANCHOR_FILE_INFO_H

#include <glib-object.h>
#include <gio/gio.h>

G_BEGIN_DECLS

#define ANCHOR_TYPE_FILE_INFO           (anchor_file_info_get_type ())
#define ANCHOR_FILE_INFO(obj)           (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_FILE_INFO, AnchorFileInfo))
#define NAUTILUS_IS_FILE_INFO(obj)        (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_FILE_INFO))
#define ANCHOR_FILE_INFO_GET_IFACE(obj) (G_TYPE_INSTANCE_GET_INTERFACE ((obj), ANCHOR_TYPE_FILE_INFO, AnchorFileInfoIface))


#ifndef NAUTILUS_FILE_DEFINED
#define NAUTILUS_FILE_DEFINED
/* Using AnchorFile for the vtable to make implementing this in 
 * AnchorFile easier */
typedef struct AnchorFile          AnchorFile;
#endif

typedef AnchorFile                  AnchorFileInfo;
typedef struct _AnchorFileInfoIface AnchorFileInfoIface;


struct _AnchorFileInfoIface 
{
	GTypeInterface g_iface;

	gboolean          (*is_gone)              (AnchorFileInfo *file);
	
	char *            (*get_name)             (AnchorFileInfo *file);
	char *            (*get_uri)              (AnchorFileInfo *file);
	char *            (*get_parent_uri)       (AnchorFileInfo *file);
	char *            (*get_uri_scheme)       (AnchorFileInfo *file);
	
	char *            (*get_mime_type)        (AnchorFileInfo *file);
	gboolean          (*is_mime_type)         (AnchorFileInfo *file,
						   const char       *mime_Type);
	gboolean          (*is_directory)         (AnchorFileInfo *file);
	
	void              (*add_emblem)           (AnchorFileInfo *file,
						   const char       *emblem_name);
	char *            (*get_string_attribute) (AnchorFileInfo *file,
						   const char       *attribute_name);
	void              (*add_string_attribute) (AnchorFileInfo *file,
						   const char       *attribute_name,
						   const char       *value);
	void              (*invalidate_extension_info) (AnchorFileInfo *file);
	
	char *            (*get_activation_uri)   (AnchorFileInfo *file);

	GFileType         (*get_file_type)        (AnchorFileInfo *file);
	GFile *           (*get_location)         (AnchorFileInfo *file);
	GFile *           (*get_parent_location)  (AnchorFileInfo *file);
	AnchorFileInfo* (*get_parent_info)      (AnchorFileInfo *file);
	GMount *          (*get_mount)            (AnchorFileInfo *file);
	gboolean          (*can_write)            (AnchorFileInfo *file);
  
};

GList            *anchor_file_info_list_copy            (GList            *files);
void              anchor_file_info_list_free            (GList            *files);
GType             anchor_file_info_get_type             (void);

/* Return true if the file has been deleted */
gboolean          anchor_file_info_is_gone              (AnchorFileInfo *file);

/* Name and Location */
GFileType         anchor_file_info_get_file_type        (AnchorFileInfo *file);
GFile *           anchor_file_info_get_location         (AnchorFileInfo *file);
char *            anchor_file_info_get_name             (AnchorFileInfo *file);
char *            anchor_file_info_get_uri              (AnchorFileInfo *file);
char *            anchor_file_info_get_activation_uri   (AnchorFileInfo *file);
GFile *           anchor_file_info_get_parent_location  (AnchorFileInfo *file);
char *            anchor_file_info_get_parent_uri       (AnchorFileInfo *file);
GMount *          anchor_file_info_get_mount            (AnchorFileInfo *file);
char *            anchor_file_info_get_uri_scheme       (AnchorFileInfo *file);
/* It's not safe to call this recursively multiple times, as it works
 * only for files already cached by Anchor.
 */
AnchorFileInfo* anchor_file_info_get_parent_info      (AnchorFileInfo *file);

/* File Type */
char *            anchor_file_info_get_mime_type        (AnchorFileInfo *file);
gboolean          anchor_file_info_is_mime_type         (AnchorFileInfo *file,
							   const char       *mime_type);
gboolean          anchor_file_info_is_directory         (AnchorFileInfo *file);
gboolean          anchor_file_info_can_write            (AnchorFileInfo *file);


/* Modifying the AnchorFileInfo */
void              anchor_file_info_add_emblem           (AnchorFileInfo *file,
							   const char       *emblem_name);
char *            anchor_file_info_get_string_attribute (AnchorFileInfo *file,
							   const char       *attribute_name);
void              anchor_file_info_add_string_attribute (AnchorFileInfo *file,
							   const char       *attribute_name,
							   const char       *value);

/* Invalidating file info */
void              anchor_file_info_invalidate_extension_info (AnchorFileInfo *file);

AnchorFileInfo *anchor_file_info_lookup                (GFile *location);
AnchorFileInfo *anchor_file_info_create                (GFile *location);
AnchorFileInfo *anchor_file_info_lookup_for_uri        (const char *uri);
AnchorFileInfo *anchor_file_info_create_for_uri        (const char *uri);

G_END_DECLS

#endif
