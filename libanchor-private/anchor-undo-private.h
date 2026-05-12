/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* xxx
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Author: Gene Z. Ragan <gzr@eazel.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 */

#ifndef NAUTILUS_UNDO_PRIVATE_H
#define NAUTILUS_UNDO_PRIVATE_H

#include <libanchor-private/anchor-undo.h>
#include <libanchor-private/anchor-undo-manager.h>
#include <glib-object.h>

AnchorUndoManager * anchor_undo_get_undo_manager    (GObject               *attached_object);
void                  anchor_undo_attach_undo_manager (GObject               *object,
							 AnchorUndoManager   *manager);

#endif /* NAUTILUS_UNDO_PRIVATE_H */
