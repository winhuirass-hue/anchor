/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Anchor
 *
 * Copyright (C) 2011 Red Hat, Inc.
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
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * see <http://www.gnu.org/licenses/>.
 *
 * Author: Cosimo Cecchi <cosimoc@redhat.com>
 */

#ifndef __NAUTILUS_PROGRESS_INFO_MANAGER_H__
#define __NAUTILUS_PROGRESS_INFO_MANAGER_H__

#include <glib-object.h>

#include <libanchor-private/anchor-progress-info.h>

#define ANCHOR_TYPE_PROGRESS_INFO_MANAGER anchor_progress_info_manager_get_type()
#define NAUTILUS_PROGRESS_INFO_MANAGER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_PROGRESS_INFO_MANAGER, AnchorProgressInfoManager))
#define NAUTILUS_PROGRESS_INFO_MANAGER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_PROGRESS_INFO_MANAGER, AnchorProgressInfoManagerClass))
#define NAUTILUS_IS_PROGRESS_INFO_MANAGER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_PROGRESS_INFO_MANAGER))
#define NAUTILUS_IS_PROGRESS_INFO_MANAGER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_PROGRESS_INFO_MANAGER))
#define NAUTILUS_PROGRESS_INFO_MANAGER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_PROGRESS_INFO_MANAGER, AnchorProgressInfoManagerClass))

typedef struct _NautilusProgressInfoManager AnchorProgressInfoManager;
typedef struct _NautilusProgressInfoManagerClass AnchorProgressInfoManagerClass;
typedef struct _NautilusProgressInfoManagerPriv AnchorProgressInfoManagerPriv;

struct _NautilusProgressInfoManager {
  GObject parent;

  /* private */
  AnchorProgressInfoManagerPriv *priv;
};

struct _NautilusProgressInfoManagerClass {
  GObjectClass parent_class;
};

GType anchor_progress_info_manager_get_type (void);

AnchorProgressInfoManager* anchor_progress_info_manager_new (void);

void anchor_progress_info_manager_add_new_info (AnchorProgressInfoManager *self,
                                                  AnchorProgressInfo *info);
GList *anchor_progress_info_manager_get_all_infos (AnchorProgressInfoManager *self);

G_END_DECLS

#endif /* __NAUTILUS_PROGRESS_INFO_MANAGER_H__ */
