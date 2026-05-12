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

#ifndef __NAUTILUS_DBUS_MANAGER_H__
#define __NAUTILUS_DBUS_MANAGER_H__

#include <glib-object.h>
#include <gio/gio.h>

typedef struct _NautilusDBusManager AnchorDBusManager;
typedef struct _NautilusDBusManagerClass AnchorDBusManagerClass;

GType anchor_dbus_manager_get_type (void);
AnchorDBusManager * anchor_dbus_manager_new (void);

#endif /* __NAUTILUS_DBUS_MANAGER_H__ */
