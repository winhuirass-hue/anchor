/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Anchor
 *
 * Copyright (C) 2000 Eazel, Inc.
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
 * Author: Maciej Stachowiak <mjs@eazel.com>
 *         Ettore Perazzoli <ettore@gnu.org>
 */

#ifndef NAUTILUS_LOCATION_ENTRY_H
#define NAUTILUS_LOCATION_ENTRY_H

#include <libanchor-private/anchor-entry.h>

#define ANCHOR_TYPE_LOCATION_ENTRY anchor_location_entry_get_type()
#define NAUTILUS_LOCATION_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_LOCATION_ENTRY, AnchorLocationEntry))
#define NAUTILUS_LOCATION_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_LOCATION_ENTRY, AnchorLocationEntryClass))
#define NAUTILUS_IS_LOCATION_ENTRY(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_LOCATION_ENTRY))
#define NAUTILUS_IS_LOCATION_ENTRY_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_LOCATION_ENTRY))
#define NAUTILUS_LOCATION_ENTRY_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_LOCATION_ENTRY, AnchorLocationEntryClass))

typedef struct AnchorLocationEntryDetails AnchorLocationEntryDetails;

typedef struct AnchorLocationEntry {
	AnchorEntry parent;
	AnchorLocationEntryDetails *details;
} AnchorLocationEntry;

typedef struct {
	AnchorEntryClass parent_class;
	/* for GtkBindingSet */
	void         (* cancel)           (AnchorLocationEntry *entry);
} AnchorLocationEntryClass;

typedef enum {
	NAUTILUS_LOCATION_ENTRY_ACTION_GOTO,
	NAUTILUS_LOCATION_ENTRY_ACTION_CLEAR
} AnchorLocationEntryAction;

GType      anchor_location_entry_get_type     	(void);

GtkWidget* anchor_location_entry_new          	(void);
void       anchor_location_entry_set_special_text     (AnchorLocationEntry *entry,
							 const char            *special_text);
void       anchor_location_entry_set_secondary_action (AnchorLocationEntry *entry,
							 AnchorLocationEntryAction secondary_action);
void       anchor_location_entry_set_location         (AnchorLocationEntry *entry,
							 GFile                 *location);
void       anchor_location_entry_focus                (AnchorLocationEntry *entry);

#endif /* NAUTILUS_LOCATION_ENTRY_H */
