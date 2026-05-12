/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/* anchor-view.h
 *
 * Copyright (C) 1999, 2000  Free Software Foundaton
 * Copyright (C) 2000, 2001  Eazel, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Ettore Perazzoli
 * 	    Darin Adler <darin@bentspoon.com>
 * 	    John Sullivan <sullivan@eazel.com>
 *          Pavel Cisler <pavel@eazel.com>
 */

#ifndef NAUTILUS_VIEW_H
#define NAUTILUS_VIEW_H

#include <gtk/gtk.h>
#include <gio/gio.h>

#include <libanchor-private/anchor-directory.h>
#include <libanchor-private/anchor-file.h>
#include <libanchor-private/anchor-link.h>

typedef struct AnchorView AnchorView;
typedef struct AnchorViewClass AnchorViewClass;

#include "anchor-window.h"
#include "anchor-window-slot.h"

#if ENABLE_EMPTY_VIEW
#define NAUTILUS_EMPTY_VIEW_ID "OAFIID:Nautilus_File_Manager_Empty_View"
#endif

#define NAUTILUS_CANVAS_VIEW_ID "OAFIID:Nautilus_File_Manager_Canvas_View"
#define NAUTILUS_DESKTOP_CANVAS_VIEW_ID "OAFIID:Nautilus_File_Manager_Desktop_Canvas_View"
#define NAUTILUS_LIST_VIEW_ID "OAFIID:Nautilus_File_Manager_List_View"

#define ANCHOR_TYPE_VIEW anchor_view_get_type()
#define NAUTILUS_VIEW(obj)\
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_VIEW, AnchorView))
#define NAUTILUS_VIEW_CLASS(klass)\
	(G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_VIEW, AnchorViewClass))
#define NAUTILUS_IS_VIEW(obj)\
	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_VIEW))
#define NAUTILUS_IS_VIEW_CLASS(klass)\
	(G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_VIEW))
#define NAUTILUS_VIEW_GET_CLASS(obj)\
	(G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_VIEW, AnchorViewClass))

typedef struct AnchorViewDetails AnchorViewDetails;

struct AnchorView {
	GtkScrolledWindow parent;

	AnchorViewDetails *details;
};

struct AnchorViewClass {
	GtkScrolledWindowClass parent_class;

	/* The 'clear' signal is emitted to empty the view of its contents.
	 * It must be replaced by each subclass.
	 */
	void 	(* clear) 		 (AnchorView *view);
	
	/* The 'begin_file_changes' signal is emitted before a set of files
	 * are added to the view. It can be replaced by a subclass to do any 
	 * necessary preparation for a set of new files. The default
	 * implementation does nothing.
	 */
	void 	(* begin_file_changes) (AnchorView *view);
	
	/* The 'add_file' signal is emitted to add one file to the view.
	 * It must be replaced by each subclass.
	 */
	void    (* add_file) 		 (AnchorView *view, 
					  AnchorFile *file,
					  AnchorDirectory *directory);
	void    (* remove_file)		 (AnchorView *view, 
					  AnchorFile *file,
					  AnchorDirectory *directory);

	/* The 'file_changed' signal is emitted to signal a change in a file,
	 * including the file being removed.
	 * It must be replaced by each subclass.
	 */
	void 	(* file_changed)         (AnchorView *view, 
					  AnchorFile *file,
					  AnchorDirectory *directory);

	/* The 'end_file_changes' signal is emitted after a set of files
	 * are added to the view. It can be replaced by a subclass to do any 
	 * necessary cleanup (typically, cleanup for code in begin_file_changes).
	 * The default implementation does nothing.
	 */
	void 	(* end_file_changes)    (AnchorView *view);
	
	/* The 'begin_loading' signal is emitted before any of the contents
	 * of a directory are added to the view. It can be replaced by a 
	 * subclass to do any necessary preparation to start dealing with a
	 * new directory. The default implementation does nothing.
	 */
	void 	(* begin_loading) 	 (AnchorView *view);

	/* The 'end_loading' signal is emitted after all of the contents
	 * of a directory are added to the view. It can be replaced by a 
	 * subclass to do any necessary clean-up. The default implementation 
	 * does nothing.
	 *
	 * If all_files_seen is true, the handler may assume that
	 * no load error ocurred, and all files of the underlying
	 * directory were loaded.
	 *
	 * Otherwise, end_loading was emitted due to cancellation,
	 * which usually means that not all files are available.
	 */
	void 	(* end_loading) 	 (AnchorView *view,
					  gboolean all_files_seen);

	/* Function pointers that don't have corresponding signals */

        /* reset_to_defaults is a function pointer that subclasses must 
         * override to set sort order, zoom level, etc to match default
         * values. 
         */
        void     (* reset_to_defaults)	         (AnchorView *view);

	/* get_backing uri is a function pointer for subclasses to
	 * override. Subclasses may replace it with a function that
	 * returns the URI for the location where to create new folders,
	 * files, links and paste the clipboard to.
	 */

	char *	(* get_backing_uri)		(AnchorView *view);

	/* get_selection is not a signal; it is just a function pointer for
	 * subclasses to replace (override). Subclasses must replace it
	 * with a function that returns a newly-allocated GList of
	 * AnchorFile pointers.
	 */
	GList *	(* get_selection) 	 	(AnchorView *view);
	
	/* get_selection_for_file_transfer  is a function pointer for
	 * subclasses to replace (override). Subclasses must replace it
	 * with a function that returns a newly-allocated GList of
	 * AnchorFile pointers. The difference from get_selection is
	 * that any files in the selection that also has a parent folder
	 * in the selection is not included.
	 */
	GList *	(* get_selection_for_file_transfer)(AnchorView *view);
	
        /* select_all is a function pointer that subclasses must override to
         * select all of the items in the view */
        void     (* select_all)	         	(AnchorView *view);

        /* select_first is a function pointer that subclasses must override to
         * select the first item in the view */
        void     (* select_first)	      	(AnchorView *view);

        /* set_selection is a function pointer that subclasses must
         * override to select the specified items (and unselect all
         * others). The argument is a list of AnchorFiles. */

        void     (* set_selection)	 	(AnchorView *view, 
        					 GList *selection);
        					 
        /* invert_selection is a function pointer that subclasses must
         * override to invert selection. */

        void     (* invert_selection)	 	(AnchorView *view);        					 

	/* Return an array of locations of selected icons in their view. */
	GArray * (* get_selected_icon_locations) (AnchorView *view);

        /* bump_zoom_level is a function pointer that subclasses must override
         * to change the zoom level of an object. */
        void    (* bump_zoom_level)      	(AnchorView *view,
					  	 int zoom_increment);

        /* zoom_to_level is a function pointer that subclasses must override
         * to set the zoom level of an object to the specified level. */
        void    (* zoom_to_level) 		(AnchorView *view, 
        				         AnchorZoomLevel level);

        AnchorZoomLevel (* get_zoom_level)    (AnchorView *view);

	/* restore_default_zoom_level is a function pointer that subclasses must override
         * to restore the zoom level of an object to a default setting. */
        void    (* restore_default_zoom_level) (AnchorView *view);

        /* can_zoom_in is a function pointer that subclasses must override to
         * return whether the view is at maximum size (furthest-in zoom level) */
        gboolean (* can_zoom_in)	 	(AnchorView *view);

        /* can_zoom_out is a function pointer that subclasses must override to
         * return whether the view is at minimum size (furthest-out zoom level) */
        gboolean (* can_zoom_out)	 	(AnchorView *view);
        
        /* reveal_selection is a function pointer that subclasses may
         * override to make sure the selected items are sufficiently
         * apparent to the user (e.g., scrolled into view). By default,
         * this does nothing.
         */
        void     (* reveal_selection)	 	(AnchorView *view);

        /* merge_menus is a function pointer that subclasses can override to
         * add their own menu items to the window's menu bar.
         * If overridden, subclasses must call parent class's function.
         */
        void    (* merge_menus)         	(AnchorView *view);
        void    (* unmerge_menus)         	(AnchorView *view);

        /* update_menus is a function pointer that subclasses can override to
         * update the sensitivity or wording of menu items in the menu bar.
         * It is called (at least) whenever the selection changes. If overridden, 
         * subclasses must call parent class's function.
         */
        void    (* update_menus)         	(AnchorView *view);

	/* sort_files is a function pointer that subclasses can override
	 * to provide a sorting order to determine which files should be
	 * presented when only a partial list is provided.
	 */
	int     (* compare_files)              (AnchorView *view,
						AnchorFile    *a,
						AnchorFile    *b);

	/* using_manual_layout is a function pointer that subclasses may
	 * override to control whether or not items can be freely positioned
	 * on the user-visible area.
	 * Note that this value is not guaranteed to be constant within the
	 * view's lifecycle. */
	gboolean (* using_manual_layout)     (AnchorView *view);

	/* is_read_only is a function pointer that subclasses may
	 * override to control whether or not the user is allowed to
	 * change the contents of the currently viewed directory. The
	 * default implementation checks the permissions of the
	 * directory.
	 */
	gboolean (* is_read_only)	        (AnchorView *view);

	/* is_empty is a function pointer that subclasses must
	 * override to report whether the view contains any items.
	 */
	gboolean (* is_empty)                   (AnchorView *view);

	gboolean (* can_rename_file)            (AnchorView *view,
						 AnchorFile *file);
	/* select_all specifies whether the whole filename should be selected
	 * or only its basename (i.e. everything except the extension)
	 * */
	void	 (* start_renaming_file)        (AnchorView *view,
					  	 AnchorFile *file,
						 gboolean select_all);

	/* convert *point from widget's coordinate system to a coordinate
	 * system used for specifying file operation positions, which is view-specific.
	 *
	 * This is used by the the icon view, which converts the screen position to a zoom
	 * level-independent coordinate system.
	 */
	void (* widget_to_file_operation_position) (AnchorView *view,
						    GdkPoint     *position);

	/* Preference change callbacks, overriden by icon and list views. 
	 * Icon and list views respond by synchronizing to the new preference
	 * values and forcing an update if appropriate.
	 */
	void	(* click_policy_changed)	   (AnchorView *view);
	void	(* sort_directories_first_changed) (AnchorView *view);

	/* Get the id string for this view. Its a constant string, not memory managed */
	const char *   (* get_view_id)            (AnchorView          *view);

	/* Return the uri of the first visible file */	
	char *         (* get_first_visible_file) (AnchorView          *view);
	/* Scroll the view so that the file specified by the uri is at the top
	   of the view */
	void           (* scroll_to_file)	  (AnchorView          *view,
						   const char            *uri);

        /* Signals used only for keybindings */
        gboolean (* trash)                         (AnchorView *view);
        gboolean (* delete)                        (AnchorView *view);
};

/* GObject support */
GType               anchor_view_get_type                         (void);

AnchorView *      anchor_view_new                              (const gchar *id,
								    AnchorWindowSlot *slot);

/* Functions callable from the user interface and elsewhere. */
AnchorWindowSlot *anchor_view_get_anchor_window_slot         (AnchorView  *view);
char *              anchor_view_get_uri                          (AnchorView  *view);

void                anchor_view_display_selection_info           (AnchorView  *view);

GdkAtom	            anchor_view_get_copied_files_atom            (AnchorView  *view);
gboolean            anchor_view_get_active                       (AnchorView  *view);

/* Wrappers for signal emitters. These are normally called 
 * only by AnchorView itself. They have corresponding signals
 * that observers might want to connect with.
 */
gboolean            anchor_view_get_loading                      (AnchorView  *view);

/* Hooks for subclasses to call. These are normally called only by 
 * AnchorView and its subclasses 
 */
void                anchor_view_activate_files                   (AnchorView        *view,
								    GList                  *files,
								    AnchorWindowOpenFlags flags,
								    gboolean                confirm_multiple);
void                anchor_view_preview_files                    (AnchorView        *view,
								    GList               *files,
								    GArray              *locations);
void                anchor_view_start_batching_selection_changes (AnchorView  *view);
void                anchor_view_stop_batching_selection_changes  (AnchorView  *view);
void                anchor_view_notify_selection_changed         (AnchorView  *view);
GtkUIManager *      anchor_view_get_ui_manager                   (AnchorView  *view);
AnchorDirectory  *anchor_view_get_model                        (AnchorView  *view);
AnchorFile       *anchor_view_get_directory_as_file            (AnchorView  *view);
void                anchor_view_pop_up_background_context_menu   (AnchorView  *view,
								    GdkEventButton   *event);
void                anchor_view_pop_up_selection_context_menu    (AnchorView  *view,
								    GdkEventButton   *event); 
gboolean            anchor_view_should_show_file                 (AnchorView  *view,
								    AnchorFile     *file);
gboolean	    anchor_view_should_sort_directories_first    (AnchorView  *view);
void                anchor_view_ignore_hidden_file_preferences   (AnchorView  *view);
void                anchor_view_set_show_foreign                 (AnchorView  *view,
								    gboolean          show_foreign);
gboolean            anchor_view_handle_scroll_event              (AnchorView  *view,
								    GdkEventScroll   *event);

void                anchor_view_freeze_updates                   (AnchorView  *view);
void                anchor_view_unfreeze_updates                 (AnchorView  *view);
gboolean            anchor_view_get_is_renaming                  (AnchorView  *view);
void                anchor_view_set_is_renaming                  (AnchorView  *view,
								    gboolean       renaming);
void                anchor_view_add_subdirectory                (AnchorView  *view,
								   AnchorDirectory*directory);
void                anchor_view_remove_subdirectory             (AnchorView  *view,
								   AnchorDirectory*directory);

gboolean            anchor_view_is_editable                     (AnchorView *view);

/* AnchorView methods */
const char *      anchor_view_get_view_id                (AnchorView      *view);

/* file operations */
char *            anchor_view_get_backing_uri            (AnchorView      *view);
void              anchor_view_move_copy_items            (AnchorView      *view,
							    const GList       *item_uris,
							    GArray            *relative_item_points,
							    const char        *target_uri,
							    int                copy_action,
							    int                x,
							    int                y);
void              anchor_view_new_file_with_initial_contents (AnchorView *view,
								const char *parent_uri,
								const char *filename,
								const char *initial_contents,
								int length,
								GdkPoint *pos);

/* selection handling */
void              anchor_view_activate_selection         (AnchorView      *view);
int               anchor_view_get_selection_count        (AnchorView      *view);
GList *           anchor_view_get_selection              (AnchorView      *view);
void              anchor_view_set_selection              (AnchorView      *view,
							    GList             *selection);


void              anchor_view_load_location              (AnchorView      *view,
							    GFile             *location);
void              anchor_view_stop_loading               (AnchorView      *view);

char *            anchor_view_get_first_visible_file     (AnchorView      *view);
void              anchor_view_scroll_to_file             (AnchorView      *view,
							    const char        *uri);
char *            anchor_view_get_title                  (AnchorView      *view);
gboolean          anchor_view_supports_zooming           (AnchorView      *view);
void              anchor_view_bump_zoom_level            (AnchorView      *view,
							    int                zoom_increment);
void              anchor_view_zoom_to_level              (AnchorView      *view,
							    AnchorZoomLevel  level);
void              anchor_view_restore_default_zoom_level (AnchorView      *view);
gboolean          anchor_view_can_zoom_in                (AnchorView      *view);
gboolean          anchor_view_can_zoom_out               (AnchorView      *view);
AnchorZoomLevel anchor_view_get_zoom_level             (AnchorView      *view);
void              anchor_view_pop_up_location_context_menu (AnchorView    *view,
							      GdkEventButton  *event,
							      const char      *location);
void              anchor_view_grab_focus                 (AnchorView      *view);
void              anchor_view_update_menus               (AnchorView      *view);

gboolean          anchor_view_get_show_hidden_files      (AnchorView      *view);

#endif /* NAUTILUS_VIEW_H */
