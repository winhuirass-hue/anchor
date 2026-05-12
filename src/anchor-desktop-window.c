/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Anchor
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Anchor is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Anchor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Darin Adler <darin@bentspoon.com>
 */

#include <config.h>
#include "anchor-desktop-window.h"
#include "anchor-window-private.h"
#include "anchor-actions.h"

#include <X11/Xatom.h>
#include <gdk/gdkx.h>
#include <gtk/gtk.h>
#include <gio/gio.h>
#include <glib/gi18n.h>

#include <eel/eel-vfs-extensions.h>
#include <libanchor-private/anchor-file-utilities.h>
#include <libanchor-private/anchor-icon-names.h>
#include <libanchor-private/anchor-global-preferences.h>

struct AnchorDesktopWindowDetails {
	gulong size_changed_id;

	gboolean loaded;
};

G_DEFINE_TYPE (AnchorDesktopWindow, anchor_desktop_window, 
	       ANCHOR_TYPE_WINDOW);

static void
anchor_desktop_window_update_directory (AnchorDesktopWindow *window)
{
	GFile *location;

	g_assert (NAUTILUS_IS_DESKTOP_WINDOW (window));

	window->details->loaded = FALSE;
	location = g_file_new_for_uri (EEL_DESKTOP_URI);
	anchor_window_go_to (NAUTILUS_WINDOW (window), location);
	window->details->loaded = TRUE;

	g_object_unref (location);
}

static void
anchor_desktop_window_constructed (GObject *obj)
{
	GtkActionGroup *action_group;
	GtkAction *action;
	AtkObject *accessible;
	AnchorDesktopWindow *window = NAUTILUS_DESKTOP_WINDOW (obj);
	AnchorWindow *nwindow = NAUTILUS_WINDOW (obj);

	G_OBJECT_CLASS (anchor_desktop_window_parent_class)->constructed (obj);

	action_group = anchor_window_get_main_action_group (nwindow);

	/* Don't allow close action on desktop */
	action = gtk_action_group_get_action (action_group,
					      NAUTILUS_ACTION_CLOSE);
	gtk_action_set_sensitive (action, FALSE);

	/* Don't allow new tab on desktop */
	action = gtk_action_group_get_action (action_group,
					      NAUTILUS_ACTION_NEW_TAB);
	gtk_action_set_sensitive (action, FALSE);

	/* Don't allow search on desktop */
	action = gtk_action_group_get_action (action_group,
					      NAUTILUS_ACTION_SEARCH);
	gtk_action_set_sensitive (action, FALSE);

	/* Set the accessible name so that it doesn't inherit the cryptic desktop URI. */
	accessible = gtk_widget_get_accessible (GTK_WIDGET (window));

	if (accessible) {
		atk_object_set_name (accessible, _("Desktop"));
	}
}

static void
anchor_desktop_window_init (AnchorDesktopWindow *window)
{
	window->details = G_TYPE_INSTANCE_GET_PRIVATE (window, ANCHOR_TYPE_DESKTOP_WINDOW,
						       AnchorDesktopWindowDetails);

	gtk_window_move (GTK_WINDOW (window), 0, 0);

	/* shouldn't really be needed given our semantic type
	 * of _NET_WM_TYPE_DESKTOP, but why not
	 */
	gtk_window_set_resizable (GTK_WINDOW (window),
				  FALSE);
	gtk_window_set_decorated (GTK_WINDOW (window),
				  FALSE);

	g_object_set_data (G_OBJECT (window), "is_desktop_window", 
			   GINT_TO_POINTER (1));
}

static void
anchor_desktop_window_screen_size_changed (GdkScreen             *screen,
					     AnchorDesktopWindow *window)
{
	int width_request, height_request;

	width_request = gdk_screen_get_width (screen);
	height_request = gdk_screen_get_height (screen);
	
	g_object_set (window,
		      "width_request", width_request,
		      "height_request", height_request,
		      NULL);
}

AnchorDesktopWindow *
anchor_desktop_window_new (GtkApplication *application,
			     GdkScreen      *screen)
{
	AnchorDesktopWindow *window;
	int width_request, height_request;
        GdkRGBA transparent = {0, 0, 0, 0};

	width_request = gdk_screen_get_width (screen);
	height_request = gdk_screen_get_height (screen);

	window = g_object_new (ANCHOR_TYPE_DESKTOP_WINDOW,
			       "application", application,
			       "disable-chrome", TRUE,
			       "width_request", width_request,
			       "height_request", height_request,
			       "screen", screen,
			       NULL);

	/* Special sawmill setting*/
	gtk_window_set_wmclass (GTK_WINDOW (window), "desktop_window", "Anchor");

	/* Point window at the desktop folder.
	 * Note that anchor_desktop_window_init is too early to do this.
	 */
	anchor_desktop_window_update_directory (window);
        gtk_widget_override_background_color (GTK_WIDGET (window), 0, &transparent);


	return window;
}

static gboolean
anchor_desktop_window_delete_event (GtkWidget *widget,
				      GdkEventAny *event)
{
	/* Returning true tells GTK+ not to delete the window. */
	return TRUE;
}

static void
map (GtkWidget *widget)
{
	/* Chain up to realize our children */
	GTK_WIDGET_CLASS (anchor_desktop_window_parent_class)->map (widget);
	gdk_window_lower (gtk_widget_get_window (widget));
}

static void
unrealize (GtkWidget *widget)
{
	AnchorDesktopWindow *window;
	AnchorDesktopWindowDetails *details;

	window = NAUTILUS_DESKTOP_WINDOW (widget);
	details = window->details;

	if (details->size_changed_id != 0) {
		g_signal_handler_disconnect (gtk_window_get_screen (GTK_WINDOW (window)),
					     details->size_changed_id);
		details->size_changed_id = 0;
	}

	GTK_WIDGET_CLASS (anchor_desktop_window_parent_class)->unrealize (widget);
}

static void
set_wmspec_desktop_hint (GdkWindow *window)
{
	GdkAtom atom;

	atom = gdk_atom_intern ("_NET_WM_WINDOW_TYPE_DESKTOP", FALSE);
        
	gdk_property_change (window,
			     gdk_atom_intern ("_NET_WM_WINDOW_TYPE", FALSE),
			     gdk_x11_xatom_to_atom (XA_ATOM), 32,
			     GDK_PROP_MODE_REPLACE, (guchar *) &atom, 1);
}

static void
realize (GtkWidget *widget)
{
	AnchorDesktopWindow *window;
	AnchorDesktopWindowDetails *details;
	GdkVisual *visual;

	window = NAUTILUS_DESKTOP_WINDOW (widget);
	details = window->details;

	/* Make sure we get keyboard events */
	gtk_widget_set_events (widget, gtk_widget_get_events (widget) 
			      | GDK_KEY_PRESS_MASK | GDK_KEY_RELEASE_MASK);
			      
	visual = gdk_screen_get_rgba_visual (gtk_widget_get_screen (widget));
	if (visual) {
		gtk_widget_set_visual (widget, visual);
	}

	/* Do the work of realizing. */
	GTK_WIDGET_CLASS (anchor_desktop_window_parent_class)->realize (widget);

	/* This is the new way to set up the desktop window */
	set_wmspec_desktop_hint (gtk_widget_get_window (widget));

	details->size_changed_id =
		g_signal_connect (gtk_window_get_screen (GTK_WINDOW (window)), "size-changed",
				  G_CALLBACK (anchor_desktop_window_screen_size_changed), window);
}

static void
real_sync_title (AnchorWindow *window,
		 AnchorWindowSlot *slot)
{
	/* hardcode "Desktop" */
	gtk_window_set_title (GTK_WINDOW (window), _("Desktop"));
}

static void
real_window_close (AnchorWindow *window)
{
	/* stub, does nothing */
	return;
}

static void
anchor_desktop_window_class_init (AnchorDesktopWindowClass *klass)
{
	GtkWidgetClass *wclass = GTK_WIDGET_CLASS (klass);
	AnchorWindowClass *nclass = NAUTILUS_WINDOW_CLASS (klass);
	GObjectClass *oclass = G_OBJECT_CLASS (klass);

	oclass->constructed = anchor_desktop_window_constructed;

	wclass->realize = realize;
	wclass->unrealize = unrealize;
	wclass->map = map;
	wclass->delete_event = anchor_desktop_window_delete_event;

	nclass->sync_title = real_sync_title;
	nclass->close = real_window_close;

	g_type_class_add_private (klass, sizeof (AnchorDesktopWindowDetails));
}

gboolean
anchor_desktop_window_loaded (AnchorDesktopWindow *window)
{
	return window->details->loaded;
}
