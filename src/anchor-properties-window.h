/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* fm-properties-window.h - interface for window that lets user modify 
                            icon properties

   Copyright (C) 2000 Eazel, Inc.

   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along with the Gnome Library; see the file COPYING.LIB.  If not,
   see <http://www.gnu.org/licenses/>.

   Authors: Darin Adler <darin@bentspoon.com>
*/

#ifndef NAUTILUS_PROPERTIES_WINDOW_H
#define NAUTILUS_PROPERTIES_WINDOW_H

#include <gtk/gtk.h>
#include <libanchor-private/anchor-file.h>

typedef struct AnchorPropertiesWindow AnchorPropertiesWindow;

#define ANCHOR_TYPE_PROPERTIES_WINDOW anchor_properties_window_get_type()
#define NAUTILUS_PROPERTIES_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_PROPERTIES_WINDOW, AnchorPropertiesWindow))
#define NAUTILUS_PROPERTIES_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_PROPERTIES_WINDOW, AnchorPropertiesWindowClass))
#define NAUTILUS_IS_PROPERTIES_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_PROPERTIES_WINDOW))
#define NAUTILUS_IS_PROPERTIES_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_PROPERTIES_WINDOW))
#define NAUTILUS_PROPERTIES_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_PROPERTIES_WINDOW, AnchorPropertiesWindowClass))

typedef struct AnchorPropertiesWindowDetails AnchorPropertiesWindowDetails;

struct AnchorPropertiesWindow {
	GtkDialog window;
	AnchorPropertiesWindowDetails *details;	
};

struct AnchorPropertiesWindowClass {
	GtkDialogClass parent_class;
	
	/* Keybinding signals */
	void (* close)    (AnchorPropertiesWindow *window);
};

typedef struct AnchorPropertiesWindowClass AnchorPropertiesWindowClass;

GType   anchor_properties_window_get_type   (void);

void 	anchor_properties_window_present    (GList       *files,
					       GtkWidget   *parent_widget,
					       const gchar *startup_id);

#endif /* NAUTILUS_PROPERTIES_WINDOW_H */
