/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* AnchorEntry: one-line text editing widget. This consists of bug fixes
 * and other improvements to GtkEntry, and all the changes could be rolled
 * into GtkEntry some day.
 *
 * Copyright (C) 2000 Eazel, Inc.
 *
 * Author: John Sullivan <sullivan@eazel.com>
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

#ifndef NAUTILUS_ENTRY_H
#define NAUTILUS_ENTRY_H

#include <gtk/gtk.h>

G_BEGIN_DECLS

#define ANCHOR_TYPE_ENTRY anchor_entry_get_type()
#define NAUTILUS_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_ENTRY, AnchorEntry))
#define NAUTILUS_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_ENTRY, AnchorEntryClass))
#define NAUTILUS_IS_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_ENTRY))
#define NAUTILUS_IS_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_ENTRY))
#define NAUTILUS_ENTRY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_ENTRY, AnchorEntryClass))

typedef struct AnchorEntryDetails AnchorEntryDetails;

typedef struct {
	GtkEntry parent;
	AnchorEntryDetails *details;
} AnchorEntry;

typedef struct {
	GtkEntryClass parent_class;

	void (*selection_changed) (AnchorEntry *entry);
} AnchorEntryClass;

GType       anchor_entry_get_type                 (void);
GtkWidget  *anchor_entry_new                      (void);
GtkWidget  *anchor_entry_new_with_max_length      (guint16        max);
void        anchor_entry_set_text                 (AnchorEntry *entry,
						     const char    *text);
void        anchor_entry_select_all               (AnchorEntry *entry);
void        anchor_entry_select_all_at_idle       (AnchorEntry *entry);
void        anchor_entry_set_special_tab_handling (AnchorEntry *entry,
						     gboolean       special_tab_handling);

G_END_DECLS

#endif /* NAUTILUS_ENTRY_H */
