/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   anchor-desktop-link-monitor.c: singleton thatn manages the links
    
   Copyright (C) 2003 Red Hat, Inc.
  
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
  
   Author: Alexander Larsson <alexl@redhat.com>
*/

#include <config.h>
#include "anchor-desktop-link-monitor.h"
#include "anchor-desktop-link.h"
#include "anchor-desktop-icon-file.h"
#include "anchor-directory.h"
#include "anchor-desktop-directory.h"
#include "anchor-global-preferences.h"

#include <eel/eel-debug.h>
#include <eel/eel-vfs-extensions.h>
#include <eel/eel-stock-dialogs.h>
#include <gtk/gtk.h>
#include <glib/gi18n.h>
#include <gio/gio.h>
#include <libanchor-private/anchor-trash-monitor.h>
#include <string.h>

struct AnchorDesktopLinkMonitorDetails {
	GVolumeMonitor *volume_monitor;
	AnchorDirectory *desktop_dir;
	
	AnchorDesktopLink *home_link;
	AnchorDesktopLink *trash_link;
	AnchorDesktopLink *network_link;

	gulong mount_id;
	gulong unmount_id;
	gulong changed_id;
	
	GList *mount_links;
};

G_DEFINE_TYPE (AnchorDesktopLinkMonitor, anchor_desktop_link_monitor, G_TYPE_OBJECT);

static AnchorDesktopLinkMonitor *the_link_monitor = NULL;

void
anchor_desktop_link_monitor_shutdown (void)
{
	g_clear_object (&the_link_monitor);
}

AnchorDesktopLinkMonitor *
anchor_desktop_link_monitor_get (void)
{
	if (the_link_monitor == NULL) {
		g_object_new (ANCHOR_TYPE_DESKTOP_LINK_MONITOR, NULL);
		eel_debug_call_at_shutdown (anchor_desktop_link_monitor_shutdown);
	}
	return the_link_monitor;
}

static void
volume_delete_dialog (GtkWidget *parent_view,
                      AnchorDesktopLink *link)
{
	GMount *mount;
	char *dialog_str;
	char *display_name;

	mount = anchor_desktop_link_get_mount (link);

	if (mount != NULL) {
		display_name = anchor_desktop_link_get_display_name (link);
		dialog_str = g_strdup_printf (_("You cannot move the volume “%s” to the trash."),
					      display_name);
		g_free (display_name);

		if (g_mount_can_eject (mount)) {
			eel_run_simple_dialog
				(parent_view, 
				 FALSE,
				 GTK_MESSAGE_ERROR,
				 dialog_str,
				 _("If you want to eject the volume, please use Eject in the "
				   "popup menu of the volume."),
				 _("_OK"), NULL);
		} else {
			eel_run_simple_dialog
				(parent_view, 
				 FALSE,
				 GTK_MESSAGE_ERROR,
				 dialog_str,
				 _("If you want to unmount the volume, please use Unmount Volume in the "
				   "popup menu of the volume."),
				 _("_OK"), NULL);
		}

		g_object_unref (mount);
		g_free (dialog_str);
	}
}

void
anchor_desktop_link_monitor_delete_link (AnchorDesktopLinkMonitor *monitor,
					   AnchorDesktopLink *link,
					   GtkWidget *parent_view)
{
	switch (anchor_desktop_link_get_link_type (link)) {
	case NAUTILUS_DESKTOP_LINK_HOME:
	case NAUTILUS_DESKTOP_LINK_TRASH:
	case NAUTILUS_DESKTOP_LINK_NETWORK:
		/* just ignore. We don't allow you to delete these */
		break;
	default:
		volume_delete_dialog (parent_view, link);
		break;
	}
}

static gboolean
volume_file_name_used (AnchorDesktopLinkMonitor *monitor,
		       const char *name)
{
	GList *l;
	char *other_name;
	gboolean same;

	for (l = monitor->details->mount_links; l != NULL; l = l->next) {
		other_name = anchor_desktop_link_get_file_name (l->data);
		same = strcmp (name, other_name) == 0;
		g_free (other_name);

		if (same) {
			return TRUE;
		}
	}

	return FALSE;
}

char *
anchor_desktop_link_monitor_make_filename_unique (AnchorDesktopLinkMonitor *monitor,
						    const char *filename)
{
	char *unique_name;
	int i;
	
	i = 2;
	unique_name = g_strdup (filename);
	while (volume_file_name_used (monitor, unique_name)) {
		g_free (unique_name);
		unique_name = g_strdup_printf ("%s.%d", filename, i++);
	}
	return unique_name;
}

static gboolean
has_mount (AnchorDesktopLinkMonitor *monitor,
	   GMount                     *mount)
{
	gboolean ret;
	GMount *other_mount;
	GList *l;

	ret = FALSE;

	for (l = monitor->details->mount_links; l != NULL; l = l->next) {
		other_mount = anchor_desktop_link_get_mount (l->data);
		if (mount == other_mount) {
			g_object_unref (other_mount);
			ret = TRUE;
			break;
		}
		g_object_unref (other_mount);
	}

	return ret;
}

static void
create_mount_link (AnchorDesktopLinkMonitor *monitor,
		   GMount *mount)
{
	AnchorDesktopLink *link;

	if (has_mount (monitor, mount))
		return;

	if ((!g_mount_is_shadowed (mount)) &&
	    g_settings_get_boolean (anchor_desktop_preferences,
				    NAUTILUS_PREFERENCES_DESKTOP_VOLUMES_VISIBLE)) {
		link = anchor_desktop_link_new_from_mount (mount);
		monitor->details->mount_links = g_list_prepend (monitor->details->mount_links, link);
	}
}

static void
remove_mount_link (AnchorDesktopLinkMonitor *monitor,
		   GMount *mount)
{
	GList *l;
	AnchorDesktopLink *link;
	GMount *other_mount;

	link = NULL;
	for (l = monitor->details->mount_links; l != NULL; l = l->next) {
		other_mount = anchor_desktop_link_get_mount (l->data);
		if (mount == other_mount) {
			g_object_unref (other_mount);
			link = l->data;
			break;
		}
		g_object_unref (other_mount);
	}

	if (link) {
		monitor->details->mount_links = g_list_remove (monitor->details->mount_links, link);
		g_object_unref (link);
	}
}



static void
mount_added_callback (GVolumeMonitor *volume_monitor,
		      GMount *mount, 
		      AnchorDesktopLinkMonitor *monitor)
{
	create_mount_link (monitor, mount);
}


static void
mount_removed_callback (GVolumeMonitor *volume_monitor,
			GMount *mount, 
			AnchorDesktopLinkMonitor *monitor)
{
	remove_mount_link (monitor, mount);
}

static void
mount_changed_callback (GVolumeMonitor *volume_monitor,
			GMount *mount, 
			AnchorDesktopLinkMonitor *monitor)
{
	/* TODO: update the mount with other details */

	/* remove a mount if it goes into the shadows */
	if (g_mount_is_shadowed (mount) && has_mount (monitor, mount)) {
		remove_mount_link (monitor, mount);
	}}

static void
update_link_visibility (AnchorDesktopLinkMonitor *monitor,
			AnchorDesktopLink       **link_ref,
			AnchorDesktopLinkType     link_type,
			const char                 *preference_key)
{
	if (g_settings_get_boolean (anchor_desktop_preferences, preference_key)) {
		if (*link_ref == NULL) {
			*link_ref = anchor_desktop_link_new (link_type);
		}
	} else {
		if (*link_ref != NULL) {
			g_object_unref (*link_ref);
			*link_ref = NULL;
		}
	}
}

static void
desktop_home_visible_changed (gpointer callback_data)
{
	AnchorDesktopLinkMonitor *monitor;

	monitor = NAUTILUS_DESKTOP_LINK_MONITOR (callback_data);

	update_link_visibility (NAUTILUS_DESKTOP_LINK_MONITOR (monitor),
				&monitor->details->home_link,
				NAUTILUS_DESKTOP_LINK_HOME,
				NAUTILUS_PREFERENCES_DESKTOP_HOME_VISIBLE);
}

static void
desktop_trash_visible_changed (gpointer callback_data)
{
	AnchorDesktopLinkMonitor *monitor;

	monitor = NAUTILUS_DESKTOP_LINK_MONITOR (callback_data);

	update_link_visibility (NAUTILUS_DESKTOP_LINK_MONITOR (callback_data),
				&monitor->details->trash_link,
				NAUTILUS_DESKTOP_LINK_TRASH,
				NAUTILUS_PREFERENCES_DESKTOP_TRASH_VISIBLE);
}

static void
desktop_network_visible_changed (gpointer callback_data)
{
	AnchorDesktopLinkMonitor *monitor;

	monitor = NAUTILUS_DESKTOP_LINK_MONITOR (callback_data);

	update_link_visibility (NAUTILUS_DESKTOP_LINK_MONITOR (callback_data),
				&monitor->details->network_link,
				NAUTILUS_DESKTOP_LINK_NETWORK,
				NAUTILUS_PREFERENCES_DESKTOP_NETWORK_VISIBLE);
}

static void
desktop_volumes_visible_changed (gpointer callback_data)
{
	AnchorDesktopLinkMonitor *monitor;
	GList *l, *mounts;

	monitor = NAUTILUS_DESKTOP_LINK_MONITOR (callback_data);

	if (g_settings_get_boolean (anchor_desktop_preferences,
				    NAUTILUS_PREFERENCES_DESKTOP_VOLUMES_VISIBLE)) {
		if (monitor->details->mount_links == NULL) {
			mounts = g_volume_monitor_get_mounts (monitor->details->volume_monitor);
			for (l = mounts; l != NULL; l = l->next) {
				create_mount_link (monitor, l->data);
				g_object_unref (l->data);
			}
			g_list_free (mounts);
		}
	} else {
		g_list_foreach (monitor->details->mount_links, (GFunc)g_object_unref, NULL);
		g_list_free (monitor->details->mount_links);
		monitor->details->mount_links = NULL;
	}
}

static void
create_link_and_add_preference (AnchorDesktopLink   **link_ref,
				AnchorDesktopLinkType link_type,
				const char             *preference_key,
				GCallback               callback,
				gpointer                callback_data)
{
	char *detailed_signal;

	if (g_settings_get_boolean (anchor_desktop_preferences, preference_key)) {
		*link_ref = anchor_desktop_link_new (link_type);
	}

	detailed_signal = g_strconcat ("changed::", preference_key, NULL);
	g_signal_connect_swapped (anchor_desktop_preferences,
				  detailed_signal,
				  callback, callback_data);

	g_free (detailed_signal);
}

static void
anchor_desktop_link_monitor_init (AnchorDesktopLinkMonitor *monitor)
{
	GList *l, *mounts;
	GMount *mount;

	monitor->details = G_TYPE_INSTANCE_GET_PRIVATE (monitor, ANCHOR_TYPE_DESKTOP_LINK_MONITOR,
							AnchorDesktopLinkMonitorDetails);

	the_link_monitor = monitor;
	monitor->details->volume_monitor = g_volume_monitor_get ();

	/* We keep around a ref to the desktop dir */
	monitor->details->desktop_dir = anchor_directory_get_by_uri (EEL_DESKTOP_URI);

	/* Default links */

	create_link_and_add_preference (&monitor->details->home_link,
					NAUTILUS_DESKTOP_LINK_HOME,
					NAUTILUS_PREFERENCES_DESKTOP_HOME_VISIBLE,
					G_CALLBACK (desktop_home_visible_changed),
					monitor);

	create_link_and_add_preference (&monitor->details->trash_link,
					NAUTILUS_DESKTOP_LINK_TRASH,
					NAUTILUS_PREFERENCES_DESKTOP_TRASH_VISIBLE,
					G_CALLBACK (desktop_trash_visible_changed),
					monitor);

	create_link_and_add_preference (&monitor->details->network_link,
					NAUTILUS_DESKTOP_LINK_NETWORK,
					NAUTILUS_PREFERENCES_DESKTOP_NETWORK_VISIBLE,
					G_CALLBACK (desktop_network_visible_changed),
					monitor);

	/* Mount links */

	mounts = g_volume_monitor_get_mounts (monitor->details->volume_monitor);
	for (l = mounts; l != NULL; l = l->next) {
		mount = l->data;
		create_mount_link (monitor, mount);
		g_object_unref (mount);
	}
	g_list_free (mounts);

	g_signal_connect_swapped (anchor_desktop_preferences,
				  "changed::" NAUTILUS_PREFERENCES_DESKTOP_VOLUMES_VISIBLE,
				  G_CALLBACK (desktop_volumes_visible_changed),
				  monitor);

	monitor->details->mount_id =
		g_signal_connect_object (monitor->details->volume_monitor, "mount-added",
					 G_CALLBACK (mount_added_callback), monitor, 0);
	monitor->details->unmount_id =
		g_signal_connect_object (monitor->details->volume_monitor, "mount-removed",
					 G_CALLBACK (mount_removed_callback), monitor, 0);
	monitor->details->changed_id =
		g_signal_connect_object (monitor->details->volume_monitor, "mount-changed",
					 G_CALLBACK (mount_changed_callback), monitor, 0);

}

static void
remove_link_and_preference (AnchorDesktopLink   **link_ref,
			    const char             *preference_key,
			    GCallback               callback,
			    gpointer                callback_data)
{
	if (*link_ref != NULL) {
		g_object_unref (*link_ref);
		*link_ref = NULL;
	}

	g_signal_handlers_disconnect_by_func (anchor_desktop_preferences,
					      callback, callback_data);
}

static void
desktop_link_monitor_finalize (GObject *object)
{
	AnchorDesktopLinkMonitor *monitor;

	monitor = NAUTILUS_DESKTOP_LINK_MONITOR (object);

	g_object_unref (monitor->details->volume_monitor);

	/* Default links */

	remove_link_and_preference (&monitor->details->home_link,
				    NAUTILUS_PREFERENCES_DESKTOP_HOME_VISIBLE,
				    G_CALLBACK (desktop_home_visible_changed),
				    monitor);

	remove_link_and_preference (&monitor->details->trash_link,
				    NAUTILUS_PREFERENCES_DESKTOP_TRASH_VISIBLE,
				    G_CALLBACK (desktop_trash_visible_changed),
				    monitor);

	remove_link_and_preference (&monitor->details->network_link,
				    NAUTILUS_PREFERENCES_DESKTOP_NETWORK_VISIBLE,
				    G_CALLBACK (desktop_network_visible_changed),
				    monitor);

	/* Mounts */

	g_list_foreach (monitor->details->mount_links, (GFunc)g_object_unref, NULL);
	g_list_free (monitor->details->mount_links);
	monitor->details->mount_links = NULL;

	anchor_directory_unref (monitor->details->desktop_dir);
	monitor->details->desktop_dir = NULL;

	g_signal_handlers_disconnect_by_func (anchor_desktop_preferences,
					      desktop_volumes_visible_changed,
					      monitor);

	if (monitor->details->mount_id != 0) {
		g_source_remove (monitor->details->mount_id);
	}
	if (monitor->details->unmount_id != 0) {
		g_source_remove (monitor->details->unmount_id);
	}
	if (monitor->details->changed_id != 0) {
		g_source_remove (monitor->details->changed_id);
	}

	G_OBJECT_CLASS (anchor_desktop_link_monitor_parent_class)->finalize (object);
}

static void
anchor_desktop_link_monitor_class_init (AnchorDesktopLinkMonitorClass *klass)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (klass);
	object_class->finalize = desktop_link_monitor_finalize;

	g_type_class_add_private (klass, sizeof (AnchorDesktopLinkMonitorDetails));
}
