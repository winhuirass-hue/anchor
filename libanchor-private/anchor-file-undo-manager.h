/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* anchor-file-undo-manager.h - Manages the undo/redo stack
 *
 * Copyright (C) 2007-2011 Amos Brocco
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
 * Author: Amos Brocco <amos.brocco@gmail.com>
 */

#ifndef __NAUTILUS_FILE_UNDO_MANAGER_H__
#define __NAUTILUS_FILE_UNDO_MANAGER_H__

#include <glib.h>
#include <glib-object.h>
#include <gtk/gtk.h>
#include <gio/gio.h>

#include <libanchor-private/anchor-file-undo-operations.h>

typedef struct _NautilusFileUndoManager AnchorFileUndoManager;
typedef struct _NautilusFileUndoManagerClass AnchorFileUndoManagerClass;
typedef struct _NautilusFileUndoManagerPrivate AnchorFileUndoManagerPrivate;

#define ANCHOR_TYPE_FILE_UNDO_MANAGER\
	(anchor_file_undo_manager_get_type())
#define NAUTILUS_FILE_UNDO_MANAGER(object)\
	(G_TYPE_CHECK_INSTANCE_CAST((object), ANCHOR_TYPE_FILE_UNDO_MANAGER,\
				    AnchorFileUndoManager))
#define NAUTILUS_FILE_UNDO_MANAGER_CLASS(klass)\
	(G_TYPE_CHECK_CLASS_CAST((klass), ANCHOR_TYPE_FILE_UNDO_MANAGER,\
				 AnchorFileUndoManagerClass))
#define NAUTILUS_IS_FILE_UNDO_MANAGER(object)\
	(G_TYPE_CHECK_INSTANCE_TYPE((object), ANCHOR_TYPE_FILE_UNDO_MANAGER))
#define NAUTILUS_IS_FILE_UNDO_MANAGER_CLASS(klass)\
	(G_TYPE_CHECK_CLASS_TYPE((klass), ANCHOR_TYPE_FILE_UNDO_MANAGER))
#define NAUTILUS_FILE_UNDO_MANAGER_GET_CLASS(object)\
	(G_TYPE_INSTANCE_GET_CLASS((object), ANCHOR_TYPE_FILE_UNDO_MANAGER,\
				   AnchorFileUndoManagerClass))

typedef enum {
	NAUTILUS_FILE_UNDO_MANAGER_STATE_NONE,
	NAUTILUS_FILE_UNDO_MANAGER_STATE_UNDO,
	NAUTILUS_FILE_UNDO_MANAGER_STATE_REDO
} AnchorFileUndoManagerState;

struct _NautilusFileUndoManager {
	GObject parent_instance;

	/* < private > */
	AnchorFileUndoManagerPrivate* priv;
};

struct _NautilusFileUndoManagerClass {
	GObjectClass parent_class;
};

GType anchor_file_undo_manager_get_type (void) G_GNUC_CONST;

AnchorFileUndoManager * anchor_file_undo_manager_get (void);

void anchor_file_undo_manager_set_action (AnchorFileUndoInfo *info);
AnchorFileUndoInfo *anchor_file_undo_manager_get_action (void);

AnchorFileUndoManagerState anchor_file_undo_manager_get_state (void);

void anchor_file_undo_manager_undo (GtkWindow *parent_window);
void anchor_file_undo_manager_redo (GtkWindow *parent_window);

void anchor_file_undo_manager_push_flag (void);
gboolean anchor_file_undo_manager_pop_flag (void);

#endif /* __NAUTILUS_FILE_UNDO_MANAGER_H__ */
