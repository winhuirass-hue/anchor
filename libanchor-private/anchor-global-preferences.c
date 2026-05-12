/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* anchor-global-preferences.c - Anchor specific preference keys and
                                   functions.

   Copyright (C) 1999, 2000, 2001 Eazel, Inc.

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

   Authors: Ramiro Estrugo <ramiro@eazel.com>
*/

#include <config.h>
#include "anchor-global-preferences.h"

#include "anchor-file-utilities.h"
#include "anchor-file.h"
#include <eel/eel-glib-extensions.h>
#include <eel/eel-gtk-extensions.h>
#include <eel/eel-stock-dialogs.h>
#include <eel/eel-string.h>
#include <glib/gi18n.h>

GSettings *anchor_preferences;
GSettings *anchor_icon_view_preferences;
GSettings *anchor_list_view_preferences;
GSettings *anchor_desktop_preferences;
GSettings *anchor_window_state;
GSettings *gtk_filechooser_preferences;
GSettings *gnome_lockdown_preferences;
GSettings *gnome_background_preferences;
GSettings *gnome_interface_preferences;
GSettings *gnome_privacy_preferences;

/*
 * Public functions
 */
char *
anchor_global_preferences_get_default_folder_viewer_preference_as_iid (void)
{
	int preference_value;
	const char *viewer_iid;

	preference_value =
		g_settings_get_enum (anchor_preferences, NAUTILUS_PREFERENCES_DEFAULT_FOLDER_VIEWER);

	if (preference_value == NAUTILUS_DEFAULT_FOLDER_VIEWER_LIST_VIEW) {
		viewer_iid = NAUTILUS_LIST_VIEW_IID;
	} else {
		viewer_iid = NAUTILUS_CANVAS_VIEW_IID;
	}

	return g_strdup (viewer_iid);
}

void
anchor_global_preferences_init (void)
{
	static gboolean initialized = FALSE;

	if (initialized) {
		return;
	}

	initialized = TRUE;

	anchor_preferences = g_settings_new("org.gnome.anchor.preferences");
	anchor_window_state = g_settings_new("org.gnome.anchor.window-state");
	anchor_icon_view_preferences = g_settings_new("org.gnome.anchor.icon-view");
	anchor_list_view_preferences = g_settings_new("org.gnome.anchor.list-view");
	anchor_desktop_preferences = g_settings_new("org.gnome.anchor.desktop");
        /* Some settings such as show hidden files are shared between Anchor and GTK file chooser */
        gtk_filechooser_preferences = g_settings_new_with_path ("org.gtk.Settings.FileChooser",
                                                                "/org/gtk/settings/file-chooser/");
	gnome_lockdown_preferences = g_settings_new("org.gnome.desktop.lockdown");
	gnome_background_preferences = g_settings_new("org.gnome.desktop.background");
	gnome_interface_preferences = g_settings_new ("org.gnome.desktop.interface");
	gnome_privacy_preferences = g_settings_new ("org.gnome.desktop.privacy");
}
