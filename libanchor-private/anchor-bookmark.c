/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* anchor-bookmark.c - implementation of individual bookmarks.
 *
 * Copyright (C) 1999, 2000 Eazel, Inc.
 * Copyright (C) 2011, Red Hat, Inc.
 *
 * The Gnome Library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * The Gnome Library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with the Gnome Library; see the file COPYING.LIB.  If not,
 * see <http://www.gnu.org/licenses/>.
 *
 * Authors: John Sullivan <sullivan@eazel.com>
 *          Cosimo Cecchi <cosimoc@redhat.com>
 */

#include <config.h>

#include "anchor-bookmark.h"

#include <eel/eel-vfs-extensions.h>
#include <gio/gio.h>
#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include <libanchor-private/anchor-file.h>
#include <libanchor-private/anchor-file-utilities.h>
#include <libanchor-private/anchor-global-preferences.h>
#include <libanchor-private/anchor-icon-names.h>

#define DEBUG_FLAG NAUTILUS_DEBUG_BOOKMARKS
#include <libanchor-private/anchor-debug.h>

enum {
	CONTENTS_CHANGED,
	LAST_SIGNAL
};

enum {
	PROP_NAME = 1,
	PROP_CUSTOM_NAME,
	PROP_LOCATION,
	PROP_ICON,
	PROP_SYMBOLIC_ICON,
	NUM_PROPERTIES
};

#define ELLIPSISED_MENU_ITEM_MIN_CHARS  32

static GParamSpec* properties[NUM_PROPERTIES] = { NULL };
static guint signals[LAST_SIGNAL];

struct AnchorBookmarkDetails
{
	char *name;
	gboolean has_custom_name;
	GFile *location;
	GIcon *icon;
	GIcon *symbolic_icon;
	AnchorFile *file;
	
	char *scroll_file;

	gboolean exists;
	guint exists_id;
	GCancellable *cancellable;
};

static void	  anchor_bookmark_disconnect_file	  (AnchorBookmark	 *file);

G_DEFINE_TYPE (AnchorBookmark, anchor_bookmark, G_TYPE_OBJECT);

static void
anchor_bookmark_set_name_internal (AnchorBookmark *bookmark,
				     const char *new_name)
{
	if (g_strcmp0 (bookmark->details->name, new_name) != 0) {
		g_free (bookmark->details->name);
		bookmark->details->name = g_strdup (new_name);

		g_object_notify_by_pspec (G_OBJECT (bookmark), properties[PROP_NAME]);
	}
}

static void
bookmark_set_name_from_ready_file (AnchorBookmark *self,
				   AnchorFile *file)
{
	gchar *display_name;

	if (self->details->has_custom_name) {
		return;
	}

	display_name = anchor_file_get_display_name (self->details->file);

	if (anchor_file_is_home (self->details->file)) {
		anchor_bookmark_set_name_internal (self, _("Home"));
	} else if (g_strcmp0 (self->details->name, display_name) != 0) {
		anchor_bookmark_set_name_internal (self, display_name);
		DEBUG ("%s: name changed to %s", anchor_bookmark_get_name (self), display_name);
	}

	g_free (display_name);
}

static void
bookmark_file_changed_callback (AnchorFile *file,
				AnchorBookmark *bookmark)
{
	GFile *location;

	g_assert (file == bookmark->details->file);

	DEBUG ("%s: file changed", anchor_bookmark_get_name (bookmark));

	location = anchor_file_get_location (file);

	if (!g_file_equal (bookmark->details->location, location) &&
	    !anchor_file_is_in_trash (file)) {
		DEBUG ("%s: file got moved", anchor_bookmark_get_name (bookmark));

		g_object_unref (bookmark->details->location);
		bookmark->details->location = g_object_ref (location);

		g_object_notify_by_pspec (G_OBJECT (bookmark), properties[PROP_LOCATION]);
		g_signal_emit (bookmark, signals[CONTENTS_CHANGED], 0);
	}

	g_object_unref (location);

	if (anchor_file_is_gone (file) ||
	    anchor_file_is_in_trash (file)) {
		/* The file we were monitoring has been trashed, deleted,
		 * or moved in a way that we didn't notice. We should make 
		 * a spanking new AnchorFile object for this 
		 * location so if a new file appears in this place 
		 * we will notice. However, we can't immediately do so
		 * because creating a new AnchorFile directly as a result
		 * of noticing a file goes away may trigger i/o on that file
		 * again, noticeing it is gone, leading to a loop.
		 * So, the new AnchorFile is created when the bookmark
		 * is used again. However, this is not really a problem, as
		 * we don't want to change the icon or anything about the
		 * bookmark just because its not there anymore.
		 */
		DEBUG ("%s: trashed", anchor_bookmark_get_name (bookmark));
		anchor_bookmark_disconnect_file (bookmark);
	} else {
		bookmark_set_name_from_ready_file (bookmark, file);
	}
}

static void
apply_warning_emblem (GIcon **base,
		      gboolean symbolic)
{
	GIcon *warning, *emblemed_icon;
	GEmblem *emblem;

	if (symbolic) {
		warning = g_themed_icon_new ("dialog-warning-symbolic");
	} else {
		warning = g_themed_icon_new ("dialog-warning");
	}

	emblem = g_emblem_new (warning);
	emblemed_icon = g_emblemed_icon_new (*base, emblem);

	g_object_unref (emblem);
	g_object_unref (warning);
	g_object_unref (*base);

	*base = emblemed_icon;
}

gboolean
anchor_bookmark_get_is_builtin (AnchorBookmark *bookmark)
{
	GUserDirectory xdg_type;

	/* if this is not an XDG dir, it's never builtin */
	if (!anchor_bookmark_get_xdg_type (bookmark, &xdg_type)) {
		return FALSE;
	}

	/* exclude XDG locations which are not in our builtin list */
	if (xdg_type == G_USER_DIRECTORY_DESKTOP &&
	    !g_settings_get_boolean (gnome_background_preferences, NAUTILUS_PREFERENCES_SHOW_DESKTOP)) {
		return FALSE;
	}

	return (xdg_type != G_USER_DIRECTORY_TEMPLATES) && (xdg_type != G_USER_DIRECTORY_PUBLIC_SHARE);
}

gboolean
anchor_bookmark_get_xdg_type (AnchorBookmark *bookmark,
				GUserDirectory   *directory)
{
	gboolean match;
	GFile *location;
	const gchar *path;
	GUserDirectory dir;

	match = FALSE;

	for (dir = 0; dir < G_USER_N_DIRECTORIES; dir++) {
		path = g_get_user_special_dir (dir);
		if (!path) {
			continue;
		}

		location = g_file_new_for_path (path);
		match = g_file_equal (location, bookmark->details->location);
		g_object_unref (location);

		if (match) {
			break;
		}
	}

	if (match && directory != NULL) {
		*directory = dir;
	}

	return match;
}

static GIcon *
get_native_icon (AnchorBookmark *bookmark,
		 gboolean symbolic)
{
	GUserDirectory xdg_type;
	GIcon *icon = NULL;

	if (bookmark->details->file == NULL) {
		goto out;
	}

	if (!anchor_bookmark_get_xdg_type (bookmark, &xdg_type)) {
		goto out;
	}

	if (xdg_type < G_USER_N_DIRECTORIES) {
		if (symbolic) {
			icon = anchor_special_directory_get_symbolic_icon (xdg_type);
		} else {
			icon = anchor_special_directory_get_icon (xdg_type);
		}
	}

 out:
	if (icon == NULL) {
		if (symbolic) {
			icon = g_themed_icon_new (NAUTILUS_ICON_FOLDER);
		} else {
			icon = g_themed_icon_new (NAUTILUS_ICON_FULLCOLOR_FOLDER);
		}
	}

	return icon;
}

static void
anchor_bookmark_set_icon_to_default (AnchorBookmark *bookmark)
{
	GIcon *icon, *symbolic_icon;
	char *uri;

	if (g_file_is_native (bookmark->details->location)) {
		symbolic_icon = get_native_icon (bookmark, TRUE);
		icon = get_native_icon (bookmark, FALSE);
	} else {
		uri = anchor_bookmark_get_uri (bookmark);
		if (g_str_has_prefix (uri, EEL_SEARCH_URI)) {
			symbolic_icon = g_themed_icon_new (NAUTILUS_ICON_FOLDER_SAVED_SEARCH);
			icon = g_themed_icon_new (NAUTILUS_ICON_FULLCOLOR_FOLDER_SAVED_SEARCH);
		} else {
			symbolic_icon = g_themed_icon_new (NAUTILUS_ICON_FOLDER_REMOTE);
			icon = g_themed_icon_new (NAUTILUS_ICON_FULLCOLOR_FOLDER_REMOTE);
		}
		g_free (uri);
	}

	if (!bookmark->details->exists) {
		DEBUG ("%s: file does not exist, add emblem", anchor_bookmark_get_name (bookmark));

		apply_warning_emblem (&icon, FALSE);
		apply_warning_emblem (&symbolic_icon, TRUE);
	}

	DEBUG ("%s: setting icon to default", anchor_bookmark_get_name (bookmark));

	g_object_set (bookmark,
		      "icon", icon,
		      "symbolic-icon", symbolic_icon,
		      NULL);

	g_object_unref (icon);
	g_object_unref (symbolic_icon);
}

static void
anchor_bookmark_disconnect_file (AnchorBookmark *bookmark)
{
	if (bookmark->details->file != NULL) {
		DEBUG ("%s: disconnecting file",
		       anchor_bookmark_get_name (bookmark));

		g_signal_handlers_disconnect_by_func (bookmark->details->file,
						      G_CALLBACK (bookmark_file_changed_callback),
						      bookmark);
		g_clear_object (&bookmark->details->file);
	}

	if (bookmark->details->cancellable != NULL) {
		g_cancellable_cancel (bookmark->details->cancellable);
		g_clear_object (&bookmark->details->cancellable);
	}

	if (bookmark->details->exists_id != 0) {
		g_source_remove (bookmark->details->exists_id);
		bookmark->details->exists_id = 0;
	}
}

static void
anchor_bookmark_connect_file (AnchorBookmark *bookmark)
{
	if (bookmark->details->file != NULL) {
		DEBUG ("%s: file already connected, returning",
		       anchor_bookmark_get_name (bookmark));
		return;
	}

	if (bookmark->details->exists) {
		DEBUG ("%s: creating file", anchor_bookmark_get_name (bookmark));

		bookmark->details->file = anchor_file_get (bookmark->details->location);
		g_assert (!anchor_file_is_gone (bookmark->details->file));

		g_signal_connect_object (bookmark->details->file, "changed",
					 G_CALLBACK (bookmark_file_changed_callback), bookmark, 0);
	}

	if (bookmark->details->icon == NULL ||
	    bookmark->details->symbolic_icon == NULL) {
		anchor_bookmark_set_icon_to_default (bookmark);
	}

	if (bookmark->details->file != NULL &&
	    anchor_file_check_if_ready (bookmark->details->file, NAUTILUS_FILE_ATTRIBUTE_INFO)) {
		bookmark_set_name_from_ready_file (bookmark, bookmark->details->file);
	}

	if (bookmark->details->name == NULL) {
		bookmark->details->name = anchor_compute_title_for_location (bookmark->details->location);
	}
}

static void
anchor_bookmark_set_exists (AnchorBookmark *bookmark,
			      gboolean exists)
{
	if (bookmark->details->exists == exists) {
		return;
	}

	bookmark->details->exists = exists;
	DEBUG ("%s: setting bookmark to exist: %d\n",
	       anchor_bookmark_get_name (bookmark), exists);

	/* refresh icon */
	anchor_bookmark_set_icon_to_default (bookmark);
}

static gboolean
exists_non_native_idle_cb (gpointer user_data)
{
	AnchorBookmark *bookmark = user_data;
	bookmark->details->exists_id = 0;
	anchor_bookmark_set_exists (bookmark, FALSE);

	return FALSE;
}

static void
exists_query_info_ready_cb (GObject *source,
			    GAsyncResult *res,
			    gpointer user_data)
{
	GFileInfo *info;
	AnchorBookmark *bookmark;
	GError *error = NULL;
	gboolean exists = FALSE;

	info = g_file_query_info_finish (G_FILE (source), res, &error);
	if (!info && g_error_matches (error, G_IO_ERROR, G_IO_ERROR_CANCELLED)) {
		g_clear_error (&error);
		return;
	}

	g_clear_error (&error);
	bookmark = user_data;

	if (info) {
		exists = TRUE;

		g_object_unref (info);
		g_clear_object (&bookmark->details->cancellable);
	}

	anchor_bookmark_set_exists (bookmark, exists);
}

static void
anchor_bookmark_update_exists (AnchorBookmark *bookmark)
{
	/* Convert to a path, returning FALSE if not local. */
	if (!g_file_is_native (bookmark->details->location) &&
	    bookmark->details->exists_id == 0) {
		bookmark->details->exists_id =
			g_idle_add (exists_non_native_idle_cb, bookmark);
		return;
	}

	if (bookmark->details->cancellable != NULL) {
		return;
	}

	bookmark->details->cancellable = g_cancellable_new ();
	g_file_query_info_async (bookmark->details->location,
				 G_FILE_ATTRIBUTE_STANDARD_TYPE,
				 0, G_PRIORITY_DEFAULT,
				 bookmark->details->cancellable,
				 exists_query_info_ready_cb, bookmark);
}

/* GObject methods */

static void
anchor_bookmark_set_property (GObject *object,
				guint property_id,
				const GValue *value,
				GParamSpec *pspec)
{
	AnchorBookmark *self = NAUTILUS_BOOKMARK (object);
	GIcon *new_icon;

	switch (property_id) {
	case PROP_ICON:
		new_icon = g_value_get_object (value);

		if (new_icon != NULL && !g_icon_equal (self->details->icon, new_icon)) {
			g_clear_object (&self->details->icon);
			self->details->icon = g_object_ref (new_icon);
		}

		break;
	case PROP_SYMBOLIC_ICON:
		new_icon = g_value_get_object (value);

		if (new_icon != NULL && !g_icon_equal (self->details->symbolic_icon, new_icon)) {
			g_clear_object (&self->details->symbolic_icon);
			self->details->symbolic_icon = g_object_ref (new_icon);
		}

		break;
	case PROP_LOCATION:
		self->details->location = g_value_dup_object (value);
		break;
	case PROP_CUSTOM_NAME:
		self->details->has_custom_name = g_value_get_boolean (value);
		break;
	case PROP_NAME:
		anchor_bookmark_set_name_internal (self, g_value_get_string (value));
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void
anchor_bookmark_get_property (GObject *object,
				guint property_id,
				GValue *value,
				GParamSpec *pspec)
{
	AnchorBookmark *self = NAUTILUS_BOOKMARK (object);

	switch (property_id) {
	case PROP_NAME:
		g_value_set_string (value, self->details->name);
		break;
	case PROP_ICON:
		g_value_set_object (value, self->details->icon);
		break;
	case PROP_SYMBOLIC_ICON:
		g_value_set_object (value, self->details->symbolic_icon);
		break;
	case PROP_LOCATION:
		g_value_set_object (value, self->details->location);
		break;
	case PROP_CUSTOM_NAME:
		g_value_set_boolean (value, self->details->has_custom_name);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, property_id, pspec);
		break;
	}
}

static void
anchor_bookmark_finalize (GObject *object)
{
	AnchorBookmark *bookmark;

	g_assert (NAUTILUS_IS_BOOKMARK (object));

	bookmark = NAUTILUS_BOOKMARK (object);

	anchor_bookmark_disconnect_file (bookmark);	

	g_object_unref (bookmark->details->location);
	g_clear_object (&bookmark->details->icon);
	g_clear_object (&bookmark->details->symbolic_icon);

	g_free (bookmark->details->name);
	g_free (bookmark->details->scroll_file);

	G_OBJECT_CLASS (anchor_bookmark_parent_class)->finalize (object);
}

static void
anchor_bookmark_constructed (GObject *obj)
{
	AnchorBookmark *self = NAUTILUS_BOOKMARK (obj);

	anchor_bookmark_connect_file (self);
	anchor_bookmark_update_exists (self);
}

static void
anchor_bookmark_class_init (AnchorBookmarkClass *class)
{
	GObjectClass *oclass = G_OBJECT_CLASS (class);

	oclass->finalize = anchor_bookmark_finalize;
	oclass->get_property = anchor_bookmark_get_property;
	oclass->set_property = anchor_bookmark_set_property;
	oclass->constructed = anchor_bookmark_constructed;

	signals[CONTENTS_CHANGED] =
		g_signal_new ("contents-changed",
		              G_TYPE_FROM_CLASS (class),
		              G_SIGNAL_RUN_LAST,
		              G_STRUCT_OFFSET (AnchorBookmarkClass, contents_changed),
		              NULL, NULL,
		              g_cclosure_marshal_VOID__VOID,
		              G_TYPE_NONE, 0);

	properties[PROP_NAME] =
		g_param_spec_string ("name",
				     "Bookmark's name",
				     "The name of this bookmark",
				     NULL,
				     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT);

	properties[PROP_CUSTOM_NAME] =
		g_param_spec_boolean ("custom-name",
				      "Whether the bookmark has a custom name",
				      "Whether the bookmark has a custom name",
				      FALSE,
				      G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT);

	properties[PROP_LOCATION] =
		g_param_spec_object ("location",
				     "Bookmark's location",
				     "The location of this bookmark",
				     G_TYPE_FILE,
				     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS | G_PARAM_CONSTRUCT_ONLY);

	properties[PROP_ICON] =
		g_param_spec_object ("icon",
				     "Bookmark's icon",
				     "The icon of this bookmark",
				     G_TYPE_ICON,
				     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	properties[PROP_SYMBOLIC_ICON] =
		g_param_spec_object ("symbolic-icon",
				     "Bookmark's symbolic icon",
				     "The symbolic icon of this bookmark",
				     G_TYPE_ICON,
				     G_PARAM_READWRITE | G_PARAM_STATIC_STRINGS);

	g_object_class_install_properties (oclass, NUM_PROPERTIES, properties);

	g_type_class_add_private (class, sizeof (AnchorBookmarkDetails));
}

static void
anchor_bookmark_init (AnchorBookmark *bookmark)
{
	bookmark->details = G_TYPE_INSTANCE_GET_PRIVATE (bookmark, ANCHOR_TYPE_BOOKMARK,
							 AnchorBookmarkDetails);

	bookmark->details->exists = TRUE;
}

const gchar *
anchor_bookmark_get_name (AnchorBookmark *bookmark)
{
	g_return_val_if_fail (NAUTILUS_IS_BOOKMARK (bookmark), NULL);

	return bookmark->details->name;
}

gboolean
anchor_bookmark_get_has_custom_name (AnchorBookmark *bookmark)
{
	g_return_val_if_fail(NAUTILUS_IS_BOOKMARK (bookmark), FALSE);

	return (bookmark->details->has_custom_name);
}

/**
 * anchor_bookmark_set_custom_name:
 *
 * Change the user-displayed name of a bookmark.
 * @new_name: The new user-displayed name for this bookmark, mustn't be NULL.
 *
 **/
void
anchor_bookmark_set_custom_name (AnchorBookmark *bookmark,
				   const char *new_name)
{
	g_return_if_fail (new_name != NULL);
	g_return_if_fail (NAUTILUS_IS_BOOKMARK (bookmark));

	g_object_set (bookmark,
		      "custom-name", TRUE,
		      "name", new_name,
		      NULL);

	g_signal_emit (bookmark, signals[CONTENTS_CHANGED], 0);
}

/**
 * anchor_bookmark_compare_with:
 *
 * Check whether two bookmarks are considered identical.
 * @a: first AnchorBookmark*.
 * @b: second AnchorBookmark*.
 * 
 * Return value: 0 if @a and @b have same name and uri, 1 otherwise 
 * (GCompareFunc style)
 **/
int		    
anchor_bookmark_compare_with (gconstpointer a, gconstpointer b)
{
	AnchorBookmark *bookmark_a;
	AnchorBookmark *bookmark_b;

	g_return_val_if_fail (NAUTILUS_IS_BOOKMARK (a), 1);
	g_return_val_if_fail (NAUTILUS_IS_BOOKMARK (b), 1);

	bookmark_a = NAUTILUS_BOOKMARK (a);
	bookmark_b = NAUTILUS_BOOKMARK (b);

	if (!g_file_equal (bookmark_a->details->location,
			   bookmark_b->details->location)) {
		return 1;
	}
	
	if (g_strcmp0 (bookmark_a->details->name,
		       bookmark_b->details->name) != 0) {
		return 1;
	}
	
	return 0;
}

GIcon *
anchor_bookmark_get_symbolic_icon (AnchorBookmark *bookmark)
{
	g_return_val_if_fail (NAUTILUS_IS_BOOKMARK (bookmark), NULL);

	/* Try to connect a file in case file exists now but didn't earlier. */
	anchor_bookmark_connect_file (bookmark);

	if (bookmark->details->symbolic_icon) {
		return g_object_ref (bookmark->details->symbolic_icon);
	}
	return NULL;
}

GIcon *
anchor_bookmark_get_icon (AnchorBookmark *bookmark)
{
	g_return_val_if_fail (NAUTILUS_IS_BOOKMARK (bookmark), NULL);

	/* Try to connect a file in case file exists now but didn't earlier. */
	anchor_bookmark_connect_file (bookmark);

	if (bookmark->details->icon) {
		return g_object_ref (bookmark->details->icon);
	}
	return NULL;
}

GFile *
anchor_bookmark_get_location (AnchorBookmark *bookmark)
{
	g_return_val_if_fail(NAUTILUS_IS_BOOKMARK (bookmark), NULL);

	/* Try to connect a file in case file exists now but didn't earlier.
	 * This allows a bookmark to update its image properly in the case
	 * where a new file appears with the same URI as a previously-deleted
	 * file. Calling connect_file here means that attempts to activate the 
	 * bookmark will update its image if possible. 
	 */
	anchor_bookmark_connect_file (bookmark);

	return g_object_ref (bookmark->details->location);
}

char *
anchor_bookmark_get_uri (AnchorBookmark *bookmark)
{
	GFile *file;
	char *uri;

	file = anchor_bookmark_get_location (bookmark);
	uri = g_file_get_uri (file);
	g_object_unref (file);
	return uri;
}

AnchorBookmark *
anchor_bookmark_new (GFile *location,
		       const gchar *custom_name)
{
	AnchorBookmark *new_bookmark;

	new_bookmark = NAUTILUS_BOOKMARK (g_object_new (ANCHOR_TYPE_BOOKMARK,
							"location", location,
							"name", custom_name,
							"custom-name", custom_name != NULL,
							NULL));

	return new_bookmark;
}				 

/**
 * anchor_bookmark_menu_item_new:
 * 
 * Return a menu item representing a bookmark.
 * @bookmark: The bookmark the menu item represents.
 * Return value: A newly-created bookmark, not yet shown.
 **/ 
GtkWidget *
anchor_bookmark_menu_item_new (AnchorBookmark *bookmark)
{
	GtkWidget *menu_item;
	GtkLabel *label;
	const char *name;

	name = anchor_bookmark_get_name (bookmark);
	menu_item = gtk_menu_item_new_with_label (name);
	label = GTK_LABEL (gtk_bin_get_child (GTK_BIN (menu_item)));
	gtk_label_set_use_underline (label, FALSE);
	gtk_label_set_ellipsize (label, PANGO_ELLIPSIZE_END);
	gtk_label_set_max_width_chars (label, ELLIPSISED_MENU_ITEM_MIN_CHARS);

	return menu_item;
}

void
anchor_bookmark_set_scroll_pos (AnchorBookmark      *bookmark,
				  const char            *uri)
{
	g_free (bookmark->details->scroll_file);
	bookmark->details->scroll_file = g_strdup (uri);
}

char *
anchor_bookmark_get_scroll_pos (AnchorBookmark      *bookmark)
{
	return g_strdup (bookmark->details->scroll_file);
}

gboolean
anchor_bookmark_get_exists (AnchorBookmark *bookmark)
{
	return bookmark->details->exists;
}
