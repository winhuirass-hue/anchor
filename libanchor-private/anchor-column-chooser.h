/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* anchor-column-choose.h - A column chooser widget

   Copyright (C) 2004 Novell, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the column COPYING.LIB.  If not,
   see <http://www.gnu.org/licenses/>.

   Authors: Dave Camp <dave@ximian.com>
*/

#ifndef NAUTILUS_COLUMN_CHOOSER_H
#define NAUTILUS_COLUMN_CHOOSER_H

#include <gtk/gtk.h>
#include <libanchor-private/anchor-file.h>

#define ANCHOR_TYPE_COLUMN_CHOOSER anchor_column_chooser_get_type()
#define NAUTILUS_COLUMN_CHOOSER(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_COLUMN_CHOOSER, AnchorColumnChooser))
#define NAUTILUS_COLUMN_CHOOSER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_COLUMN_CHOOSER, AnchorColumnChooserClass))
#define NAUTILUS_IS_COLUMN_CHOOSER(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_COLUMN_CHOOSER))
#define NAUTILUS_IS_COLUMN_CHOOSER_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_COLUMN_CHOOSER))
#define NAUTILUS_COLUMN_CHOOSER_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_COLUMN_CHOOSER, AnchorColumnChooserClass))

typedef struct _NautilusColumnChooserDetails AnchorColumnChooserDetails;

typedef struct {
	GtkBox parent;
	
	AnchorColumnChooserDetails *details;
} AnchorColumnChooser;

typedef struct {
        GtkBoxClass parent_slot;

	void (*changed) (AnchorColumnChooser *chooser);
	void (*use_default) (AnchorColumnChooser *chooser);
} AnchorColumnChooserClass;

GType      anchor_column_chooser_get_type            (void);
GtkWidget *anchor_column_chooser_new                 (AnchorFile *file);
void       anchor_column_chooser_set_settings    (AnchorColumnChooser   *chooser,
						    char                   **visible_columns, 
						    char                   **column_order);
void       anchor_column_chooser_get_settings    (AnchorColumnChooser *chooser,
						    char                  ***visible_columns, 
						    char                  ***column_order);

#endif /* NAUTILUS_COLUMN_CHOOSER_H */
