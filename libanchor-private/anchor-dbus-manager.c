/*
 * anchor-dbus-manager: anchor DBus interface
 *
 * Copyright (C) 2010, Red Hat, Inc.
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
 * Author: Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#include <config.h>

#include "anchor-dbus-manager.h"
#include "anchor-generated.h"

#include "anchor-file-operations.h"

#define DEBUG_FLAG NAUTILUS_DEBUG_DBUS
#include "anchor-debug.h"

#include <gio/gio.h>

struct _NautilusDBusManager {
  GObject parent;

  GDBusObjectManagerServer *object_manager;
  AnchorDBusFileOperations *file_operations;
};

struct _NautilusDBusManagerClass {
  GObjectClass parent_class;
};

G_DEFINE_TYPE (AnchorDBusManager, anchor_dbus_manager, G_TYPE_OBJECT);

static void
anchor_dbus_manager_dispose (GObject *object)
{
  AnchorDBusManager *self = (AnchorDBusManager *) object;

  if (self->file_operations) {
    g_dbus_interface_skeleton_unexport (G_DBUS_INTERFACE_SKELETON (self->file_operations));
    g_object_unref (self->file_operations);
    self->file_operations = NULL;
  }

  if (self->object_manager) {
    g_object_unref (self->object_manager);
    self->object_manager = NULL;
  }

  G_OBJECT_CLASS (anchor_dbus_manager_parent_class)->dispose (object);
}

static gboolean
handle_copy_file (AnchorDBusFileOperations *object,
		  GDBusMethodInvocation *invocation,
		  const gchar *source_uri,
		  const gchar *source_display_name,
		  const gchar *dest_dir_uri,
		  const gchar *dest_name)
{
  GFile *source_file, *target_dir;
  const gchar *target_name = NULL, *source_name = NULL;

  source_file = g_file_new_for_uri (source_uri);
  target_dir = g_file_new_for_uri (dest_dir_uri);

  if (dest_name != NULL && dest_name[0] != '\0')
    target_name = dest_name;

  if (source_display_name != NULL && source_display_name[0] != '\0')
    source_name = source_display_name;

  anchor_file_operations_copy_file (source_file, target_dir, source_name, target_name,
				      NULL, NULL, NULL);

  g_object_unref (source_file);
  g_object_unref (target_dir);

  anchor_dbus_file_operations_complete_copy_file (object, invocation);
  return TRUE; /* invocation was handled */
}

static gboolean
handle_copy_uris (AnchorDBusFileOperations *object,
		  GDBusMethodInvocation *invocation,
		  const gchar **sources,
		  const gchar *destination)
{
  GList *source_files = NULL;
  GFile *dest_dir;
  gint idx;

  dest_dir = g_file_new_for_uri (destination);

  for (idx = 0; sources[idx] != NULL; idx++)
    source_files = g_list_prepend (source_files,
                                   g_file_new_for_uri (sources[idx]));

  anchor_file_operations_copy (source_files, NULL,
                                 dest_dir,
                                 NULL, NULL, NULL);

  g_list_free_full (source_files, g_object_unref);
  g_object_unref (dest_dir);

  anchor_dbus_file_operations_complete_copy_uris (object, invocation);
  return TRUE; /* invocation was handled */
}

static gboolean
handle_empty_trash (AnchorDBusFileOperations *object,
		    GDBusMethodInvocation *invocation)
{
  anchor_file_operations_empty_trash (NULL);

  anchor_dbus_file_operations_complete_empty_trash (object, invocation);
  return TRUE; /* invocation was handled */
}

static void
anchor_dbus_manager_init (AnchorDBusManager *self)
{
  GDBusConnection *connection;

  connection = g_application_get_dbus_connection (g_application_get_default ());

  self->object_manager = g_dbus_object_manager_server_new ("/org/gnome/Anchor");
  self->file_operations = anchor_dbus_file_operations_skeleton_new ();

  g_signal_connect (self->file_operations,
		    "handle-copy-uris",
		    G_CALLBACK (handle_copy_uris),
		    self);
  g_signal_connect (self->file_operations,
		    "handle-copy-file",
		    G_CALLBACK (handle_copy_file),
		    self);
  g_signal_connect (self->file_operations,
		    "handle-empty-trash",
		    G_CALLBACK (handle_empty_trash),
		    self);

  g_dbus_interface_skeleton_export (G_DBUS_INTERFACE_SKELETON (self->file_operations), connection,
				    "/org/gnome/Anchor", NULL);

  g_dbus_object_manager_server_set_connection (self->object_manager, connection);
}

static void
anchor_dbus_manager_class_init (AnchorDBusManagerClass *klass)
{
  GObjectClass *oclass = G_OBJECT_CLASS (klass);

  oclass->dispose = anchor_dbus_manager_dispose;
}

AnchorDBusManager *
anchor_dbus_manager_new (void)
{
  return g_object_new (anchor_dbus_manager_get_type (),
                       NULL);
}
