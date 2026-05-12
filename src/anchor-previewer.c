/*
 * anchor-previewer: anchor previewer DBus wrapper
 *
 * Copyright (C) 2011, Red Hat, Inc.
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

#include "config.h"

#include "anchor-previewer.h"

#define DEBUG_FLAG NAUTILUS_DEBUG_PREVIEWER
#include <libanchor-private/anchor-debug.h>

#include <gio/gio.h>

G_DEFINE_TYPE (AnchorPreviewer, anchor_previewer, G_TYPE_OBJECT);

#define PREVIEWER_DBUS_NAME "org.gnome.AnchorPreviewer"
#define PREVIEWER_DBUS_IFACE "org.gnome.AnchorPreviewer"
#define PREVIEWER_DBUS_PATH "/org/gnome/AnchorPreviewer"

static AnchorPreviewer *singleton = NULL;

struct _NautilusPreviewerPriv {
  GDBusConnection *connection;
};

static void
anchor_previewer_dispose (GObject *object)
{
  AnchorPreviewer *self = NAUTILUS_PREVIEWER (object);

  DEBUG ("%p", self);

  g_clear_object (&self->priv->connection);

  G_OBJECT_CLASS (anchor_previewer_parent_class)->dispose (object);
}

static GObject *
anchor_previewer_constructor (GType type,
                                guint n_construct_params,
                                GObjectConstructParam *construct_params)
{
  GObject *retval;

  if (singleton != NULL)
    return G_OBJECT (singleton);

  retval = G_OBJECT_CLASS (anchor_previewer_parent_class)->constructor
    (type, n_construct_params, construct_params);

  singleton = NAUTILUS_PREVIEWER (retval);
  g_object_add_weak_pointer (retval, (gpointer) &singleton);

  return retval;
}

static void
anchor_previewer_init (AnchorPreviewer *self)
{
  GError *error = NULL;

  self->priv = G_TYPE_INSTANCE_GET_PRIVATE (self, ANCHOR_TYPE_PREVIEWER,
                                            AnchorPreviewerPriv);

  self->priv->connection = g_bus_get_sync (G_BUS_TYPE_SESSION,
                                           NULL, &error);

  if (error != NULL) {
    g_printerr ("Unable to initialize DBus connection: %s", error->message);
    g_error_free (error);
    return;
  }
}

static void
anchor_previewer_class_init (AnchorPreviewerClass *klass)
{
  GObjectClass *oclass;

  oclass = G_OBJECT_CLASS (klass);
  oclass->constructor = anchor_previewer_constructor;
  oclass->dispose = anchor_previewer_dispose;

  g_type_class_add_private (klass, sizeof (AnchorPreviewerPriv));
}

static void
previewer_show_file_ready_cb (GObject *source,
                              GAsyncResult *res,
                              gpointer user_data)
{
  AnchorPreviewer *self = user_data;
  GError *error = NULL;

  g_dbus_connection_call_finish (self->priv->connection,
                                 res, &error);

  if (error != NULL) {
    DEBUG ("Unable to call ShowFile on AnchorPreviewer: %s",
           error->message);
    g_error_free (error);
  }

  g_object_unref (self);
}

static void
previewer_close_ready_cb (GObject *source,
                          GAsyncResult *res,
                          gpointer user_data)
{
  AnchorPreviewer *self = user_data;
  GError *error = NULL;

  g_dbus_connection_call_finish (self->priv->connection,
                                 res, &error);

  if (error != NULL) {
    DEBUG ("Unable to call Close on AnchorPreviewer: %s",
           error->message);
    g_error_free (error);
  }

  g_object_unref (self);
}

AnchorPreviewer *
anchor_previewer_get_singleton (void)
{
  return g_object_new (ANCHOR_TYPE_PREVIEWER, NULL);
}

void
anchor_previewer_call_show_file (AnchorPreviewer *self,
                                   const gchar *uri,
                                   guint xid,
				   gboolean close_if_already_visible)
{
  GVariant *variant;

  variant = g_variant_new ("(sib)",
                           uri, xid, close_if_already_visible);

  if (self->priv->connection == NULL) {
    g_printerr ("No DBus connection available");
    return;
  }

  g_dbus_connection_call (self->priv->connection,
                          PREVIEWER_DBUS_NAME,
                          PREVIEWER_DBUS_PATH,
                          PREVIEWER_DBUS_IFACE,
                          "ShowFile",
                          variant,
                          NULL,
                          G_DBUS_CALL_FLAGS_NONE,
                          -1,
                          NULL,
                          previewer_show_file_ready_cb,
                          g_object_ref (self));
}

void
anchor_previewer_call_close (AnchorPreviewer *self)
{
  if (self->priv->connection == NULL) {
    g_printerr ("No DBus connection available");
    return;
  }

  /* don't autostart the previewer if it's not running */
  g_dbus_connection_call (self->priv->connection,
                          PREVIEWER_DBUS_NAME,
                          PREVIEWER_DBUS_PATH,
                          PREVIEWER_DBUS_IFACE,
                          "Close",
                          NULL,
                          NULL,
                          G_DBUS_CALL_FLAGS_NO_AUTO_START,
                          -1,
                          NULL,
                          previewer_close_ready_cb,
                          g_object_ref (self));
}
