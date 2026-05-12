/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Copyright (C) 2005 Novell, Inc.
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
 * Author: Anders Carlsson <andersca@imendio.com>
 *
 */

#ifndef NAUTILUS_QUERY_H
#define NAUTILUS_QUERY_H

#include <glib-object.h>

#define ANCHOR_TYPE_QUERY		(anchor_query_get_type ())
#define NAUTILUS_QUERY(obj)		(G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_QUERY, AnchorQuery))
#define NAUTILUS_QUERY_CLASS(klass)	(G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_QUERY, AnchorQueryClass))
#define NAUTILUS_IS_QUERY(obj)		(G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_QUERY))
#define NAUTILUS_IS_QUERY_CLASS(klass)	(G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_QUERY))
#define NAUTILUS_QUERY_GET_CLASS(obj)    (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_QUERY, AnchorQueryClass))

typedef struct AnchorQueryDetails AnchorQueryDetails;

typedef struct AnchorQuery {
	GObject parent;
	AnchorQueryDetails *details;
} AnchorQuery;

typedef struct {
	GObjectClass parent_class;
} AnchorQueryClass;

GType          anchor_query_get_type (void);

AnchorQuery* anchor_query_new      (void);

char *         anchor_query_get_text           (AnchorQuery *query);
void           anchor_query_set_text           (AnchorQuery *query, const char *text);

gboolean       anchor_query_get_show_hidden_files (AnchorQuery *query);
void           anchor_query_set_show_hidden_files (AnchorQuery *query, gboolean show_hidden);

char *         anchor_query_get_location       (AnchorQuery *query);
void           anchor_query_set_location       (AnchorQuery *query, const char *uri);

GList *        anchor_query_get_mime_types     (AnchorQuery *query);
void           anchor_query_set_mime_types     (AnchorQuery *query, GList *mime_types);
void           anchor_query_add_mime_type      (AnchorQuery *query, const char *mime_type);

gdouble        anchor_query_matches_string     (AnchorQuery *query, const gchar *string);

char *         anchor_query_to_readable_string (AnchorQuery *query);
AnchorQuery *anchor_query_load               (char *file);
gboolean       anchor_query_save               (AnchorQuery *query, char *file);

#endif /* NAUTILUS_QUERY_H */
