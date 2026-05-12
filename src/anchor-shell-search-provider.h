/*
 * anchor-shell-search-provider.h - Implementation of a GNOME Shell
 *   search provider
 *
 * Copyright (C) 2012 Red Hat, Inc.
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
 * Authors: Cosimo Cecchi <cosimoc@gnome.org>
 *
 */

#ifndef __NAUTILUS_SHELL_SEARCH_PROVIDER_H__
#define __NAUTILUS_SHELL_SEARCH_PROVIDER_H__

#define ANCHOR_TYPE_SHELL_SEARCH_PROVIDER anchor_shell_search_provider_get_type()
#define NAUTILUS_SHELL_SEARCH_PROVIDER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_SHELL_SEARCH_PROVIDER, AnchorShellSearchProvider))

typedef struct _NautilusShellSearchProvider AnchorShellSearchProvider;
typedef GObjectClass AnchorShellSearchProviderClass;

GType anchor_shell_search_provider_get_type (void);
AnchorShellSearchProvider * anchor_shell_search_provider_new (void);

#endif /* __NAUTILUS_SHELL_SEARCH_PROVIDER_H__ */
