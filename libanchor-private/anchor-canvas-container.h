/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* gnome-canvas-container.h - Canvas container widget.

   Copyright (C) 1999, 2000 Free Software Foundation
   Copyright (C) 2000 Eazel, Inc.

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

   Authors: Ettore Perazzoli <ettore@gnu.org>, Darin Adler <darin@bentspoon.com>
*/

#ifndef NAUTILUS_CANVAS_CONTAINER_H
#define NAUTILUS_CANVAS_CONTAINER_H

#include <eel/eel-canvas.h>
#include <libanchor-private/anchor-icon-info.h>

#define ANCHOR_TYPE_CANVAS_CONTAINER anchor_canvas_container_get_type()
#define NAUTILUS_CANVAS_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_CANVAS_CONTAINER, AnchorCanvasContainer))
#define NAUTILUS_CANVAS_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_CANVAS_CONTAINER, AnchorCanvasContainerClass))
#define NAUTILUS_IS_CANVAS_CONTAINER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_CANVAS_CONTAINER))
#define NAUTILUS_IS_CANVAS_CONTAINER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_CANVAS_CONTAINER))
#define NAUTILUS_CANVAS_CONTAINER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_CANVAS_CONTAINER, AnchorCanvasContainerClass))


#define NAUTILUS_CANVAS_ICON_DATA(pointer) \
	((AnchorCanvasIconData *) (pointer))

typedef struct AnchorCanvasIconData AnchorCanvasIconData;

typedef void (* AnchorCanvasCallback) (AnchorCanvasIconData *icon_data,
					 gpointer callback_data);

typedef struct {
	int x;
	int y;
	double scale;
} AnchorCanvasPosition;

typedef enum {
	NAUTILUS_CANVAS_LABEL_POSITION_UNDER,
} AnchorCanvasLabelPosition;

#define	NAUTILUS_CANVAS_CONTAINER_TYPESELECT_FLUSH_DELAY 1000000

typedef struct AnchorCanvasContainerDetails AnchorCanvasContainerDetails;

typedef struct {
	EelCanvas canvas;
	AnchorCanvasContainerDetails *details;
} AnchorCanvasContainer;

typedef struct {
	EelCanvasClass parent_slot;

	/* Operations on the container. */
	int          (* button_press) 	          (AnchorCanvasContainer *container,
						   GdkEventButton *event);
	void         (* context_click_background) (AnchorCanvasContainer *container,
						   GdkEventButton *event);
	void         (* middle_click) 		  (AnchorCanvasContainer *container,
						   GdkEventButton *event);

	/* Operations on icons. */
	void         (* activate)	  	  (AnchorCanvasContainer *container,
						   AnchorCanvasIconData *data);
	void         (* activate_alternate)       (AnchorCanvasContainer *container,
						   AnchorCanvasIconData *data);
	void         (* activate_previewer)       (AnchorCanvasContainer *container,
						   GList *files,
						   GArray *locations);
	void         (* context_click_selection)  (AnchorCanvasContainer *container,
						   GdkEventButton *event);
	void	     (* move_copy_items)	  (AnchorCanvasContainer *container,
						   const GList *item_uris,
						   GdkPoint *relative_item_points,
						   const char *target_uri,
						   GdkDragAction action,
						   int x,
						   int y);
	void	     (* handle_netscape_url)	  (AnchorCanvasContainer *container,
						   const char *url,
						   const char *target_uri,
						   GdkDragAction action,
						   int x,
						   int y);
	void	     (* handle_uri_list)    	  (AnchorCanvasContainer *container,
						   const char *uri_list,
						   const char *target_uri,
						   GdkDragAction action,
						   int x,
						   int y);
	void	     (* handle_text)		  (AnchorCanvasContainer *container,
						   const char *text,
						   const char *target_uri,
						   GdkDragAction action,
						   int x,
						   int y);
	void	     (* handle_raw)		  (AnchorCanvasContainer *container,
						   char *raw_data,
						   int length,
						   const char *target_uri,
						   const char *direct_save_uri,
						   GdkDragAction action,
						   int x,
						   int y);
	void	     (* handle_hover)		  (AnchorCanvasContainer *container,
						   const char *target_uri);

	/* Queries on the container for subclass/client.
	 * These must be implemented. The default "do nothing" is not good enough.
	 */
	char *	     (* get_container_uri)	  (AnchorCanvasContainer *container);

	/* Queries on icons for subclass/client.
	 * These must be implemented. The default "do nothing" is not
	 * good enough, these are _not_ signals.
	 */
	AnchorIconInfo *(* get_icon_images)     (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data,
						     int canvas_size,
						     char **embedded_text,
						     gboolean for_drag_accept,
						     gboolean need_large_embeddded_text,
						     gboolean *embedded_text_needs_loading,
						     gboolean *has_window_open);
	void         (* get_icon_text)            (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data,
						     char **editable_text,
						     char **additional_text,
						     gboolean include_invisible);
	char *       (* get_icon_description)     (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data);
	int          (* compare_icons)            (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *canvas_a,
						     AnchorCanvasIconData *canvas_b);
	int          (* compare_icons_by_name)    (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *canvas_a,
						     AnchorCanvasIconData *canvas_b);
	void         (* freeze_updates)           (AnchorCanvasContainer *container);
	void         (* unfreeze_updates)         (AnchorCanvasContainer *container);
	void         (* start_monitor_top_left)   (AnchorCanvasContainer *container,
						   AnchorCanvasIconData *data,
						   gconstpointer client,
						   gboolean large_text);
	void         (* stop_monitor_top_left)    (AnchorCanvasContainer *container,
						   AnchorCanvasIconData *data,
						   gconstpointer client);
	void         (* prioritize_thumbnailing)  (AnchorCanvasContainer *container,
						   AnchorCanvasIconData *data);

	/* Queries on icons for subclass/client.
	 * These must be implemented => These are signals !
	 * The default "do nothing" is not good enough.
	 */
	gboolean     (* can_accept_item)	  (AnchorCanvasContainer *container,
						   AnchorCanvasIconData *target, 
						   const char *item_uri);
	gboolean     (* get_stored_icon_position) (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data,
						     AnchorCanvasPosition *position);
	char *       (* get_icon_uri)             (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data);
	char *       (* get_icon_activation_uri)  (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data);
	char *       (* get_icon_drop_target_uri) (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data);

	/* If canvas data is NULL, the layout timestamp of the container should be retrieved.
	 * That is the time when the container displayed a fully loaded directory with
	 * all canvas positions assigned.
	 *
	 * If canvas data is not NULL, the position timestamp of the canvas should be retrieved.
	 * That is the time when the file (i.e. canvas data payload) was last displayed in a
	 * fully loaded directory with all canvas positions assigned.
	 */
	gboolean     (* get_stored_layout_timestamp) (AnchorCanvasContainer *container,
						      AnchorCanvasIconData *data,
						      time_t *time);
	/* If canvas data is NULL, the layout timestamp of the container should be stored.
	 * If canvas data is not NULL, the position timestamp of the container should be stored.
	 */
	gboolean     (* store_layout_timestamp) (AnchorCanvasContainer *container,
						 AnchorCanvasIconData *data,
						 const time_t *time);

	/* Notifications for the whole container. */
	void	     (* band_select_started)	  (AnchorCanvasContainer *container);
	void	     (* band_select_ended)	  (AnchorCanvasContainer *container);
	void         (* selection_changed) 	  (AnchorCanvasContainer *container);
	void         (* layout_changed)           (AnchorCanvasContainer *container);

	/* Notifications for icons. */
	void         (* icon_position_changed)    (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data,
						     const AnchorCanvasPosition *position);
	void         (* icon_rename_started)      (AnchorCanvasContainer *container,
						     GtkWidget *renaming_widget);
	void         (* icon_rename_ended)        (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data,
						     const char *text);
	void	     (* icon_stretch_started)     (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data);
	void	     (* icon_stretch_ended)       (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data);
	int	     (* preview)		  (AnchorCanvasContainer *container,
						   AnchorCanvasIconData *data,
						   gboolean start_flag);
        void         (* icon_added)               (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data);
        void         (* icon_removed)             (AnchorCanvasContainer *container,
						     AnchorCanvasIconData *data);
        void         (* cleared)                  (AnchorCanvasContainer *container);
	gboolean     (* start_interactive_search) (AnchorCanvasContainer *container);
} AnchorCanvasContainerClass;

/* GtkObject */
GType             anchor_canvas_container_get_type                      (void);
GtkWidget *       anchor_canvas_container_new                           (void);


/* adding, removing, and managing icons */
void              anchor_canvas_container_clear                         (AnchorCanvasContainer  *view);
gboolean          anchor_canvas_container_add                           (AnchorCanvasContainer  *view,
									   AnchorCanvasIconData       *data);
void              anchor_canvas_container_layout_now                    (AnchorCanvasContainer *container);
gboolean          anchor_canvas_container_remove                        (AnchorCanvasContainer  *view,
									   AnchorCanvasIconData       *data);
void              anchor_canvas_container_for_each                      (AnchorCanvasContainer  *view,
									   AnchorCanvasCallback    callback,
									   gpointer                callback_data);
void              anchor_canvas_container_request_update                (AnchorCanvasContainer  *view,
									   AnchorCanvasIconData       *data);
void              anchor_canvas_container_request_update_all            (AnchorCanvasContainer  *container);
void              anchor_canvas_container_reveal                        (AnchorCanvasContainer  *container,
									   AnchorCanvasIconData       *data);
gboolean          anchor_canvas_container_is_empty                      (AnchorCanvasContainer  *container);
AnchorCanvasIconData *anchor_canvas_container_get_first_visible_icon        (AnchorCanvasContainer  *container);
void              anchor_canvas_container_scroll_to_canvas                (AnchorCanvasContainer  *container,
									     AnchorCanvasIconData       *data);

void              anchor_canvas_container_begin_loading                 (AnchorCanvasContainer  *container);
void              anchor_canvas_container_end_loading                   (AnchorCanvasContainer  *container,
									   gboolean                all_icons_added);

/* control the layout */
gboolean          anchor_canvas_container_is_auto_layout                (AnchorCanvasContainer  *container);
void              anchor_canvas_container_set_auto_layout               (AnchorCanvasContainer  *container,
									   gboolean                auto_layout);

gboolean          anchor_canvas_container_is_keep_aligned               (AnchorCanvasContainer  *container);
void              anchor_canvas_container_set_keep_aligned              (AnchorCanvasContainer  *container,
									   gboolean                keep_aligned);
void              anchor_canvas_container_set_label_position            (AnchorCanvasContainer  *container,
									   AnchorCanvasLabelPosition pos);
void              anchor_canvas_container_sort                          (AnchorCanvasContainer  *container);
void              anchor_canvas_container_freeze_icon_positions         (AnchorCanvasContainer  *container);

int               anchor_canvas_container_get_max_layout_lines           (AnchorCanvasContainer  *container);
int               anchor_canvas_container_get_max_layout_lines_for_pango (AnchorCanvasContainer  *container);

void              anchor_canvas_container_set_highlighted_for_clipboard (AnchorCanvasContainer  *container,
									   GList                  *clipboard_canvas_data);

/* operations on all icons */
void              anchor_canvas_container_unselect_all                  (AnchorCanvasContainer  *view);
void              anchor_canvas_container_select_all                    (AnchorCanvasContainer  *view);


void              anchor_canvas_container_select_first                  (AnchorCanvasContainer  *view);


/* operations on the selection */
GList     *       anchor_canvas_container_get_selection                 (AnchorCanvasContainer  *view);
void			  anchor_canvas_container_invert_selection				(AnchorCanvasContainer  *view);
void              anchor_canvas_container_set_selection                 (AnchorCanvasContainer  *view,
									   GList                  *selection);
GArray    *       anchor_canvas_container_get_selected_icon_locations   (AnchorCanvasContainer  *view);
gboolean          anchor_canvas_container_has_stretch_handles           (AnchorCanvasContainer  *container);
gboolean          anchor_canvas_container_is_stretched                  (AnchorCanvasContainer  *container);
void              anchor_canvas_container_show_stretch_handles          (AnchorCanvasContainer  *container);
void              anchor_canvas_container_unstretch                     (AnchorCanvasContainer  *container);
void              anchor_canvas_container_start_renaming_selected_item  (AnchorCanvasContainer  *container,
									   gboolean                select_all);

/* options */
AnchorZoomLevel anchor_canvas_container_get_zoom_level                (AnchorCanvasContainer  *view);
void              anchor_canvas_container_set_zoom_level                (AnchorCanvasContainer  *view,
									   int                     new_zoom_level);
void              anchor_canvas_container_set_single_click_mode         (AnchorCanvasContainer  *container,
									   gboolean                single_click_mode);
void              anchor_canvas_container_enable_linger_selection       (AnchorCanvasContainer  *view,
									   gboolean                enable);
gboolean          anchor_canvas_container_get_is_fixed_size             (AnchorCanvasContainer  *container);
void              anchor_canvas_container_set_is_fixed_size             (AnchorCanvasContainer  *container,
									   gboolean                is_fixed_size);
gboolean          anchor_canvas_container_get_is_desktop                (AnchorCanvasContainer  *container);
void              anchor_canvas_container_set_is_desktop                (AnchorCanvasContainer  *container,
									   gboolean                is_desktop);
void              anchor_canvas_container_reset_scroll_region           (AnchorCanvasContainer  *container);
void              anchor_canvas_container_set_font                      (AnchorCanvasContainer  *container,
									   const char             *font); 
void              anchor_canvas_container_set_margins                   (AnchorCanvasContainer  *container,
									   int                     left_margin,
									   int                     right_margin,
									   int                     top_margin,
									   int                     bottom_margin);
void              anchor_canvas_container_set_use_drop_shadows          (AnchorCanvasContainer  *container,
									   gboolean                use_drop_shadows);
char*             anchor_canvas_container_get_icon_description          (AnchorCanvasContainer  *container,
									     AnchorCanvasIconData       *data);
gboolean          anchor_canvas_container_get_allow_moves               (AnchorCanvasContainer  *container);
void              anchor_canvas_container_set_allow_moves               (AnchorCanvasContainer  *container,
									   gboolean                allow_moves);

gboolean	  anchor_canvas_container_is_layout_rtl			(AnchorCanvasContainer  *container);
gboolean	  anchor_canvas_container_is_layout_vertical		(AnchorCanvasContainer  *container);

gboolean          anchor_canvas_container_get_store_layout_timestamps   (AnchorCanvasContainer  *container);
void              anchor_canvas_container_set_store_layout_timestamps   (AnchorCanvasContainer  *container,
									   gboolean                store_layout);

void              anchor_canvas_container_widget_to_file_operation_position (AnchorCanvasContainer *container,
									       GdkPoint              *position);

#define CANVAS_WIDTH(container,allocation) ((allocation.width		\
					     - container->details->left_margin \
					     - container->details->right_margin) \
					    /  EEL_CANVAS (container)->pixels_per_unit)

#define CANVAS_HEIGHT(container,allocation) ((allocation.height		\
					      - container->details->top_margin \
					      - container->details->bottom_margin) \
					     / EEL_CANVAS (container)->pixels_per_unit)

#endif /* NAUTILUS_CANVAS_CONTAINER_H */
