/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* anchor-file-conflict-dialog: dialog that handles file conflicts
   during transfer operations.

   Copyright (C) 2008, Cosimo Cecchi

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.
   
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
   
   You should have received a copy of the GNU General Public
   License along with this program; if not, see <http://www.gnu.org/licenses/>.
   
   Authors: Cosimo Cecchi <cosimoc@gnome.org>
*/

#ifndef NAUTILUS_FILE_CONFLICT_DIALOG_H
#define NAUTILUS_FILE_CONFLICT_DIALOG_H

#include <glib-object.h>
#include <gio/gio.h>
#include <gtk/gtk.h>

#define ANCHOR_TYPE_FILE_CONFLICT_DIALOG \
	(anchor_file_conflict_dialog_get_type ())
#define NAUTILUS_FILE_CONFLICT_DIALOG(o) \
	(G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_FILE_CONFLICT_DIALOG,\
				     AnchorFileConflictDialog))
#define NAUTILUS_FILE_CONFLICT_DIALOG_CLASS(k) \
	(G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_FILE_CONFLICT_DIALOG,\
				 AnchorFileConflictDialogClass))
#define NAUTILUS_IS_FILE_CONFLICT_DIALOG(o) \
	(G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_FILE_CONFLICT_DIALOG))
#define NAUTILUS_IS_FILE_CONFLICT_DIALOG_CLASS(k) \
	(G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_FILE_CONFLICT_DIALOG))
#define NAUTILUS_FILE_CONFLICT_DIALOG_GET_CLASS(o) \
	(G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_FILE_CONFLICT_DIALOG,\
				    AnchorFileConflictDialogClass))

typedef struct _NautilusFileConflictDialog        AnchorFileConflictDialog;
typedef struct _NautilusFileConflictDialogClass   AnchorFileConflictDialogClass;
typedef struct _NautilusFileConflictDialogDetails AnchorFileConflictDialogDetails;

struct _NautilusFileConflictDialog {
	GtkDialog parent;
	AnchorFileConflictDialogDetails *details;
};

struct _NautilusFileConflictDialogClass {
	GtkDialogClass parent_class;
};

enum
{
	CONFLICT_RESPONSE_SKIP = 1,
	CONFLICT_RESPONSE_REPLACE = 2,
	CONFLICT_RESPONSE_RENAME = 3,
};

GType anchor_file_conflict_dialog_get_type (void) G_GNUC_CONST;

GtkWidget* anchor_file_conflict_dialog_new              (GtkWindow *parent,
							   GFile *source,
							   GFile *destination,
							   GFile *dest_dir);
char*      anchor_file_conflict_dialog_get_new_name     (AnchorFileConflictDialog *dialog);
gboolean   anchor_file_conflict_dialog_get_apply_to_all (AnchorFileConflictDialog *dialog);

#endif /* NAUTILUS_FILE_CONFLICT_DIALOG_H */
