/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Red Hat, Inc.
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
 * Author: Alexander Larsson <alexl@redhat.com>
 *
 */

#ifndef NAUTILUS_QUERY_EDITOR_H
#define NAUTILUS_QUERY_EDITOR_H

#include <gtk/gtk.h>

#include <libanchor-private/anchor-query.h>

#define ANCHOR_TYPE_QUERY_EDITOR anchor_query_editor_get_type()
#define NAUTILUS_QUERY_EDITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_QUERY_EDITOR, AnchorQueryEditor))
#define NAUTILUS_QUERY_EDITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_QUERY_EDITOR, AnchorQueryEditorClass))
#define NAUTILUS_IS_QUERY_EDITOR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_QUERY_EDITOR))
#define NAUTILUS_IS_QUERY_EDITOR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_QUERY_EDITOR))
#define NAUTILUS_QUERY_EDITOR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_QUERY_EDITOR, AnchorQueryEditorClass))

typedef struct AnchorQueryEditorDetails AnchorQueryEditorDetails;

typedef struct AnchorQueryEditor {
	GtkBox parent;
	AnchorQueryEditorDetails *details;
} AnchorQueryEditor;

typedef struct {
	GtkBoxClass parent_class;

	void (* changed)   (AnchorQueryEditor  *editor,
			    AnchorQuery        *query,
			    gboolean              reload);
	void (* cancel)    (AnchorQueryEditor *editor);
	void (* activated) (AnchorQueryEditor *editor);
} AnchorQueryEditorClass;

#include "anchor-window-slot.h"

GType      anchor_query_editor_get_type     	   (void);
GtkWidget* anchor_query_editor_new          	   (void);

gboolean       anchor_query_editor_handle_event (AnchorQueryEditor *editor,
						   GdkEventKey         *event);

AnchorQuery *anchor_query_editor_get_query   (AnchorQueryEditor *editor);
void           anchor_query_editor_set_query   (AnchorQueryEditor *editor,
						  AnchorQuery       *query);
GFile *        anchor_query_editor_get_location (AnchorQueryEditor *editor);
void           anchor_query_editor_set_location (AnchorQueryEditor *editor,
						   GFile               *location);

#endif /* NAUTILUS_QUERY_EDITOR_H */
