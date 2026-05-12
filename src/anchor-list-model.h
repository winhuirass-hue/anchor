/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-list-model.h - a GtkTreeModel for file lists. 

   Copyright (C) 2001, 2002 Anders Carlsson

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   see <http://www.gnu.org/licenses/>.

   Authors: Anders Carlsson <andersca@gnu.org>
*/

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <libanchor-private/anchor-file.h>
#include <libanchor-private/anchor-directory.h>
#include <libanchor-extension/anchor-column.h>

#ifndef NAUTILUS_LIST_MODEL_H
#define NAUTILUS_LIST_MODEL_H

#define ANCHOR_TYPE_LIST_MODEL anchor_list_model_get_type()
#define NAUTILUS_LIST_MODEL(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_LIST_MODEL, AnchorListModel))
#define NAUTILUS_LIST_MODEL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_LIST_MODEL, AnchorListModelClass))
#define NAUTILUS_IS_LIST_MODEL(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_LIST_MODEL))
#define NAUTILUS_IS_LIST_MODEL_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_LIST_MODEL))
#define NAUTILUS_LIST_MODEL_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_LIST_MODEL, AnchorListModelClass))

enum {
	NAUTILUS_LIST_MODEL_FILE_COLUMN,
	NAUTILUS_LIST_MODEL_SUBDIRECTORY_COLUMN,
	NAUTILUS_LIST_MODEL_SMALLEST_ICON_COLUMN,
	NAUTILUS_LIST_MODEL_SMALLER_ICON_COLUMN,
	NAUTILUS_LIST_MODEL_SMALL_ICON_COLUMN,
	NAUTILUS_LIST_MODEL_STANDARD_ICON_COLUMN,
	NAUTILUS_LIST_MODEL_LARGE_ICON_COLUMN,
	NAUTILUS_LIST_MODEL_LARGER_ICON_COLUMN,
	NAUTILUS_LIST_MODEL_LARGEST_ICON_COLUMN,
	NAUTILUS_LIST_MODEL_FILE_NAME_IS_EDITABLE_COLUMN,
	NAUTILUS_LIST_MODEL_NUM_COLUMNS
};

typedef struct AnchorListModelDetails AnchorListModelDetails;

typedef struct AnchorListModel {
	GObject parent_instance;
	AnchorListModelDetails *details;
} AnchorListModel;

typedef struct {
	GObjectClass parent_class;

	void (* subdirectory_unloaded)(AnchorListModel *model,
				       AnchorDirectory *subdirectory);
} AnchorListModelClass;

GType    anchor_list_model_get_type                          (void);
gboolean anchor_list_model_add_file                          (AnchorListModel          *model,
								AnchorFile         *file,
								AnchorDirectory    *directory);
void     anchor_list_model_file_changed                      (AnchorListModel          *model,
								AnchorFile         *file,
								AnchorDirectory    *directory);
gboolean anchor_list_model_is_empty                          (AnchorListModel          *model);
guint    anchor_list_model_get_length                        (AnchorListModel          *model);
void     anchor_list_model_remove_file                       (AnchorListModel          *model,
								AnchorFile         *file,
								AnchorDirectory    *directory);
void     anchor_list_model_clear                             (AnchorListModel          *model);
gboolean anchor_list_model_get_tree_iter_from_file           (AnchorListModel          *model,
								AnchorFile         *file,
								AnchorDirectory    *directory,
								GtkTreeIter          *iter);
GList *  anchor_list_model_get_all_iters_for_file            (AnchorListModel          *model,
								AnchorFile         *file);
gboolean anchor_list_model_get_first_iter_for_file           (AnchorListModel          *model,
								AnchorFile         *file,
								GtkTreeIter          *iter);
void     anchor_list_model_set_should_sort_directories_first (AnchorListModel          *model,
								gboolean              sort_directories_first);

int      anchor_list_model_get_sort_column_id_from_attribute (AnchorListModel *model,
								GQuark       attribute);
GQuark   anchor_list_model_get_attribute_from_sort_column_id (AnchorListModel *model,
								int sort_column_id);
void     anchor_list_model_sort_files                        (AnchorListModel *model,
								GList **files);

AnchorZoomLevel anchor_list_model_get_zoom_level_from_column_id (int               column);
int               anchor_list_model_get_column_id_from_zoom_level (AnchorZoomLevel zoom_level);

AnchorFile *    anchor_list_model_file_for_path (AnchorListModel *model, GtkTreePath *path);
gboolean          anchor_list_model_load_subdirectory (AnchorListModel *model, GtkTreePath *path, AnchorDirectory **directory);
void              anchor_list_model_unload_subdirectory (AnchorListModel *model, GtkTreeIter *iter);

void              anchor_list_model_set_drag_view (AnchorListModel *model,
						     GtkTreeView *view,
						     int begin_x, 
						     int begin_y);
GtkTreeView *     anchor_list_model_get_drag_view (AnchorListModel *model,
						     int *drag_begin_x,
						     int *drag_begin_y);

GtkTargetList *   anchor_list_model_get_drag_target_list (void);

int               anchor_list_model_compare_func (AnchorListModel *model,
						    AnchorFile *file1,
						    AnchorFile *file2);


int               anchor_list_model_add_column (AnchorListModel *model,
						  AnchorColumn *column);
int               anchor_list_model_get_column_number (AnchorListModel *model,
							 const char *column_name);

void              anchor_list_model_subdirectory_done_loading (AnchorListModel       *model,
								 AnchorDirectory *directory);

void              anchor_list_model_set_highlight_for_files (AnchorListModel *model,
							       GList *files);
						   
#endif /* NAUTILUS_LIST_MODEL_H */
