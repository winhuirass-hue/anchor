/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* anchor-file-undo-operations.h - Manages undo/redo of file operations
 *
 * Copyright (C) 2007-2011 Amos Brocco
 * Copyright (C) 2010 Red Hat, Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Amos Brocco <amos.brocco@gmail.com>
 *          Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#ifndef __NAUTILUS_FILE_UNDO_OPERATIONS_H__
#define __NAUTILUS_FILE_UNDO_OPERATIONS_H__

#include <gio/gio.h>
#include <gtk/gtk.h>

typedef enum {
	NAUTILUS_FILE_UNDO_OP_COPY,
	NAUTILUS_FILE_UNDO_OP_DUPLICATE,
	NAUTILUS_FILE_UNDO_OP_MOVE,
	NAUTILUS_FILE_UNDO_OP_RENAME,
	NAUTILUS_FILE_UNDO_OP_CREATE_EMPTY_FILE,
	NAUTILUS_FILE_UNDO_OP_CREATE_FILE_FROM_TEMPLATE,
	NAUTILUS_FILE_UNDO_OP_CREATE_FOLDER,
	NAUTILUS_FILE_UNDO_OP_MOVE_TO_TRASH,
	NAUTILUS_FILE_UNDO_OP_RESTORE_FROM_TRASH,
	NAUTILUS_FILE_UNDO_OP_CREATE_LINK,
	NAUTILUS_FILE_UNDO_OP_RECURSIVE_SET_PERMISSIONS,
	NAUTILUS_FILE_UNDO_OP_SET_PERMISSIONS,
	NAUTILUS_FILE_UNDO_OP_CHANGE_GROUP,
	NAUTILUS_FILE_UNDO_OP_CHANGE_OWNER,
	NAUTILUS_FILE_UNDO_OP_NUM_TYPES,
} AnchorFileUndoOp;

#define ANCHOR_TYPE_FILE_UNDO_INFO         (anchor_file_undo_info_get_type ())
#define NAUTILUS_FILE_UNDO_INFO(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_FILE_UNDO_INFO, AnchorFileUndoInfo))
#define NAUTILUS_FILE_UNDO_INFO_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_FILE_UNDO_INFO, AnchorFileUndoInfoClass))
#define NAUTILUS_IS_FILE_UNDO_INFO(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_FILE_UNDO_INFO))
#define NAUTILUS_IS_FILE_UNDO_INFO_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_FILE_UNDO_INFO))
#define NAUTILUS_FILE_UNDO_INFO_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_FILE_UNDO_INFO, AnchorFileUndoInfoClass))

typedef struct _NautilusFileUndoInfo      AnchorFileUndoInfo;
typedef struct _NautilusFileUndoInfoClass AnchorFileUndoInfoClass;
typedef struct _NautilusFileUndoInfoDetails AnchorFileUndoInfoDetails;

struct _NautilusFileUndoInfo {
	GObject parent;
	AnchorFileUndoInfoDetails *priv;
};

struct _NautilusFileUndoInfoClass {
	GObjectClass parent_class;

	void (* undo_func) (AnchorFileUndoInfo *self,
			    GtkWindow            *parent_window);
	void (* redo_func) (AnchorFileUndoInfo *self,
			    GtkWindow            *parent_window);

	void (* strings_func) (AnchorFileUndoInfo *self,
			       gchar **undo_label,
			       gchar **undo_description,
			       gchar **redo_label,
			       gchar **redo_description);
};

GType anchor_file_undo_info_get_type (void) G_GNUC_CONST;

void anchor_file_undo_info_apply_async (AnchorFileUndoInfo *self,
					  gboolean undo,
					  GtkWindow *parent_window,
					  GAsyncReadyCallback callback,
					  gpointer user_data);
gboolean anchor_file_undo_info_apply_finish (AnchorFileUndoInfo *self,
					       GAsyncResult *res,
					       gboolean *user_cancel,
					       GError **error);

void anchor_file_undo_info_get_strings (AnchorFileUndoInfo *self,
					  gchar **undo_label,
					  gchar **undo_description,
					  gchar **redo_label,
					  gchar **redo_description);

/* copy/move/duplicate/link/restore from trash */
#define ANCHOR_TYPE_FILE_UNDO_INFO_EXT         (anchor_file_undo_info_ext_get_type ())
#define NAUTILUS_FILE_UNDO_INFO_EXT(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_FILE_UNDO_INFO_EXT, AnchorFileUndoInfoExt))
#define NAUTILUS_FILE_UNDO_INFO_EXT_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_FILE_UNDO_INFO_EXT, AnchorFileUndoInfoExtClass))
#define NAUTILUS_IS_FILE_UNDO_INFO_EXT(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_FILE_UNDO_INFO_EXT))
#define NAUTILUS_IS_FILE_UNDO_INFO_EXT_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_FILE_UNDO_INFO_EXT))
#define NAUTILUS_FILE_UNDO_INFO_EXT_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_FILE_UNDO_INFO_EXT, AnchorFileUndoInfoExtClass))

typedef struct _NautilusFileUndoInfoExt      AnchorFileUndoInfoExt;
typedef struct _NautilusFileUndoInfoExtClass AnchorFileUndoInfoExtClass;
typedef struct _NautilusFileUndoInfoExtDetails AnchorFileUndoInfoExtDetails;

struct _NautilusFileUndoInfoExt {
	AnchorFileUndoInfo parent;
	AnchorFileUndoInfoExtDetails *priv;
};

struct _NautilusFileUndoInfoExtClass {
	AnchorFileUndoInfoClass parent_class;
};

GType anchor_file_undo_info_ext_get_type (void) G_GNUC_CONST;
AnchorFileUndoInfo *anchor_file_undo_info_ext_new (AnchorFileUndoOp op_type,
						       gint item_count,
						       GFile *src_dir,
						       GFile *target_dir);
void anchor_file_undo_info_ext_add_origin_target_pair (AnchorFileUndoInfoExt *self,
							 GFile                   *origin,
							 GFile                   *target);

/* create new file/folder */
#define ANCHOR_TYPE_FILE_UNDO_INFO_CREATE         (anchor_file_undo_info_create_get_type ())
#define NAUTILUS_FILE_UNDO_INFO_CREATE(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_FILE_UNDO_INFO_CREATE, AnchorFileUndoInfoCreate))
#define NAUTILUS_FILE_UNDO_INFO_CREATE_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_FILE_UNDO_INFO_CREATE, AnchorFileUndoInfoCreateClass))
#define NAUTILUS_IS_FILE_UNDO_INFO_CREATE(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_FILE_UNDO_INFO_CREATE))
#define NAUTILUS_IS_FILE_UNDO_INFO_CREATE_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_FILE_UNDO_INFO_CREATE))
#define NAUTILUS_FILE_UNDO_INFO_CREATE_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_FILE_UNDO_INFO_CREATE, AnchorFileUndoInfoCreateClass))

typedef struct _NautilusFileUndoInfoCreate      AnchorFileUndoInfoCreate;
typedef struct _NautilusFileUndoInfoCreateClass AnchorFileUndoInfoCreateClass;
typedef struct _NautilusFileUndoInfoCreateDetails AnchorFileUndoInfoCreateDetails;

struct _NautilusFileUndoInfoCreate {
	AnchorFileUndoInfo parent;
	AnchorFileUndoInfoCreateDetails *priv;
};

struct _NautilusFileUndoInfoCreateClass {
	AnchorFileUndoInfoClass parent_class;
};

GType anchor_file_undo_info_create_get_type (void) G_GNUC_CONST;
AnchorFileUndoInfo *anchor_file_undo_info_create_new (AnchorFileUndoOp op_type);
void anchor_file_undo_info_create_set_data (AnchorFileUndoInfoCreate *self,
					      GFile                      *file,
					      const char                 *template,
					      gint                        length);

/* rename */
#define ANCHOR_TYPE_FILE_UNDO_INFO_RENAME         (anchor_file_undo_info_rename_get_type ())
#define NAUTILUS_FILE_UNDO_INFO_RENAME(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_FILE_UNDO_INFO_RENAME, AnchorFileUndoInfoRename))
#define NAUTILUS_FILE_UNDO_INFO_RENAME_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_FILE_UNDO_INFO_RENAME, AnchorFileUndoInfoRenameClass))
#define NAUTILUS_IS_FILE_UNDO_INFO_RENAME(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_FILE_UNDO_INFO_RENAME))
#define NAUTILUS_IS_FILE_UNDO_INFO_RENAME_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_FILE_UNDO_INFO_RENAME))
#define NAUTILUS_FILE_UNDO_INFO_RENAME_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_FILE_UNDO_INFO_RENAME, AnchorFileUndoInfoRenameClass))

typedef struct _NautilusFileUndoInfoRename      AnchorFileUndoInfoRename;
typedef struct _NautilusFileUndoInfoRenameClass AnchorFileUndoInfoRenameClass;
typedef struct _NautilusFileUndoInfoRenameDetails AnchorFileUndoInfoRenameDetails;

struct _NautilusFileUndoInfoRename {
	AnchorFileUndoInfo parent;
	AnchorFileUndoInfoRenameDetails *priv;
};

struct _NautilusFileUndoInfoRenameClass {
	AnchorFileUndoInfoClass parent_class;
};

GType anchor_file_undo_info_rename_get_type (void) G_GNUC_CONST;
AnchorFileUndoInfo *anchor_file_undo_info_rename_new (void);
void anchor_file_undo_info_rename_set_data (AnchorFileUndoInfoRename *self,
					      GFile                      *old_file,
					      GFile                      *new_file);

/* trash */
#define ANCHOR_TYPE_FILE_UNDO_INFO_TRASH         (anchor_file_undo_info_trash_get_type ())
#define NAUTILUS_FILE_UNDO_INFO_TRASH(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_FILE_UNDO_INFO_TRASH, AnchorFileUndoInfoTrash))
#define NAUTILUS_FILE_UNDO_INFO_TRASH_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_FILE_UNDO_INFO_TRASH, AnchorFileUndoInfoTrashClass))
#define NAUTILUS_IS_FILE_UNDO_INFO_TRASH(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_FILE_UNDO_INFO_TRASH))
#define NAUTILUS_IS_FILE_UNDO_INFO_TRASH_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_FILE_UNDO_INFO_TRASH))
#define NAUTILUS_FILE_UNDO_INFO_TRASH_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_FILE_UNDO_INFO_TRASH, AnchorFileUndoInfoTrashClass))

typedef struct _NautilusFileUndoInfoTrash      AnchorFileUndoInfoTrash;
typedef struct _NautilusFileUndoInfoTrashClass AnchorFileUndoInfoTrashClass;
typedef struct _NautilusFileUndoInfoTrashDetails AnchorFileUndoInfoTrashDetails;

struct _NautilusFileUndoInfoTrash {
	AnchorFileUndoInfo parent;
	AnchorFileUndoInfoTrashDetails *priv;
};

struct _NautilusFileUndoInfoTrashClass {
	AnchorFileUndoInfoClass parent_class;
};

GType anchor_file_undo_info_trash_get_type (void) G_GNUC_CONST;
AnchorFileUndoInfo *anchor_file_undo_info_trash_new (gint item_count);
void anchor_file_undo_info_trash_add_file (AnchorFileUndoInfoTrash *self,
					     GFile                     *file);

/* recursive permissions */
#define ANCHOR_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS         (anchor_file_undo_info_rec_permissions_get_type ())
#define NAUTILUS_FILE_UNDO_INFO_REC_PERMISSIONS(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS, AnchorFileUndoInfoRecPermissions))
#define NAUTILUS_FILE_UNDO_INFO_REC_PERMISSIONS_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS, AnchorFileUndoInfoRecPermissionsClass))
#define NAUTILUS_IS_FILE_UNDO_INFO_REC_PERMISSIONS(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS))
#define NAUTILUS_IS_FILE_UNDO_INFO_REC_PERMISSIONS_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS))
#define NAUTILUS_FILE_UNDO_INFO_REC_PERMISSIONS_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_FILE_UNDO_INFO_REC_PERMISSIONS, AnchorFileUndoInfoRecPermissionsClass))

typedef struct _NautilusFileUndoInfoRecPermissions      AnchorFileUndoInfoRecPermissions;
typedef struct _NautilusFileUndoInfoRecPermissionsClass AnchorFileUndoInfoRecPermissionsClass;
typedef struct _NautilusFileUndoInfoRecPermissionsDetails AnchorFileUndoInfoRecPermissionsDetails;

struct _NautilusFileUndoInfoRecPermissions {
	AnchorFileUndoInfo parent;
	AnchorFileUndoInfoRecPermissionsDetails *priv;
};

struct _NautilusFileUndoInfoRecPermissionsClass {
	AnchorFileUndoInfoClass parent_class;
};

GType anchor_file_undo_info_rec_permissions_get_type (void) G_GNUC_CONST;
AnchorFileUndoInfo *anchor_file_undo_info_rec_permissions_new (GFile   *dest,
								   guint32 file_permissions,
								   guint32 file_mask,
								   guint32 dir_permissions,
								   guint32 dir_mask);
void anchor_file_undo_info_rec_permissions_add_file (AnchorFileUndoInfoRecPermissions *self,
						       GFile                              *file,
						       guint32                             permission);

/* single file change permissions */
#define ANCHOR_TYPE_FILE_UNDO_INFO_PERMISSIONS         (anchor_file_undo_info_permissions_get_type ())
#define NAUTILUS_FILE_UNDO_INFO_PERMISSIONS(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_FILE_UNDO_INFO_PERMISSIONS, AnchorFileUndoInfoPermissions))
#define NAUTILUS_FILE_UNDO_INFO_PERMISSIONS_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_FILE_UNDO_INFO_PERMISSIONS, AnchorFileUndoInfoPermissionsClass))
#define NAUTILUS_IS_FILE_UNDO_INFO_PERMISSIONS(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_FILE_UNDO_INFO_PERMISSIONS))
#define NAUTILUS_IS_FILE_UNDO_INFO_PERMISSIONS_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_FILE_UNDO_INFO_PERMISSIONS))
#define NAUTILUS_FILE_UNDO_INFO_PERMISSIONS_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_FILE_UNDO_INFO_PERMISSIONS, AnchorFileUndoInfoPermissionsClass))

typedef struct _NautilusFileUndoInfoPermissions      AnchorFileUndoInfoPermissions;
typedef struct _NautilusFileUndoInfoPermissionsClass AnchorFileUndoInfoPermissionsClass;
typedef struct _NautilusFileUndoInfoPermissionsDetails AnchorFileUndoInfoPermissionsDetails;

struct _NautilusFileUndoInfoPermissions {
	AnchorFileUndoInfo parent;
	AnchorFileUndoInfoPermissionsDetails *priv;
};

struct _NautilusFileUndoInfoPermissionsClass {
	AnchorFileUndoInfoClass parent_class;
};

GType anchor_file_undo_info_permissions_get_type (void) G_GNUC_CONST;
AnchorFileUndoInfo *anchor_file_undo_info_permissions_new (GFile   *file,
							       guint32  current_permissions,
							       guint32  new_permissions);

/* group and owner change */
#define ANCHOR_TYPE_FILE_UNDO_INFO_OWNERSHIP         (anchor_file_undo_info_ownership_get_type ())
#define NAUTILUS_FILE_UNDO_INFO_OWNERSHIP(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_FILE_UNDO_INFO_OWNERSHIP, AnchorFileUndoInfoOwnership))
#define NAUTILUS_FILE_UNDO_INFO_OWNERSHIP_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_FILE_UNDO_INFO_OWNERSHIP, AnchorFileUndoInfoOwnershipClass))
#define NAUTILUS_IS_FILE_UNDO_INFO_OWNERSHIP(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_FILE_UNDO_INFO_OWNERSHIP))
#define NAUTILUS_IS_FILE_UNDO_INFO_OWNERSHIP_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_FILE_UNDO_INFO_OWNERSHIP))
#define NAUTILUS_FILE_UNDO_INFO_OWNERSHIP_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_FILE_UNDO_INFO_OWNERSHIP, AnchorFileUndoInfoOwnershipClass))

typedef struct _NautilusFileUndoInfoOwnership      AnchorFileUndoInfoOwnership;
typedef struct _NautilusFileUndoInfoOwnershipClass AnchorFileUndoInfoOwnershipClass;
typedef struct _NautilusFileUndoInfoOwnershipDetails AnchorFileUndoInfoOwnershipDetails;

struct _NautilusFileUndoInfoOwnership {
	AnchorFileUndoInfo parent;
	AnchorFileUndoInfoOwnershipDetails *priv;
};

struct _NautilusFileUndoInfoOwnershipClass {
	AnchorFileUndoInfoClass parent_class;
};

GType anchor_file_undo_info_ownership_get_type (void) G_GNUC_CONST;
AnchorFileUndoInfo *anchor_file_undo_info_ownership_new (AnchorFileUndoOp  op_type,
							     GFile              *file,
							     const char         *current_data,
							     const char         *new_data);

#endif /* __NAUTILUS_FILE_UNDO_OPERATIONS_H__ */
