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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Darin Adler <darin@bentspoon.com>
 */

/* anchor-desktop-window.h
 */

#ifndef NAUTILUS_DESKTOP_WINDOW_H
#define NAUTILUS_DESKTOP_WINDOW_H

#include "anchor-window.h"

#define ANCHOR_TYPE_DESKTOP_WINDOW anchor_desktop_window_get_type()
#define NAUTILUS_DESKTOP_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_DESKTOP_WINDOW, AnchorDesktopWindow))
#define NAUTILUS_DESKTOP_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_DESKTOP_WINDOW, AnchorDesktopWindowClass))
#define NAUTILUS_IS_DESKTOP_WINDOW(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_DESKTOP_WINDOW))
#define NAUTILUS_IS_DESKTOP_WINDOW_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_DESKTOP_WINDOW))
#define NAUTILUS_DESKTOP_WINDOW_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_DESKTOP_WINDOW, AnchorDesktopWindowClass))

typedef struct AnchorDesktopWindowDetails AnchorDesktopWindowDetails;

typedef struct {
	AnchorWindow parent_spot;
	AnchorDesktopWindowDetails *details;
} AnchorDesktopWindow;

typedef struct {
	AnchorWindowClass parent_spot;
} AnchorDesktopWindowClass;

GType                  anchor_desktop_window_get_type            (void);
AnchorDesktopWindow *anchor_desktop_window_new                 (GtkApplication        *application,
								    GdkScreen             *screen);
gboolean               anchor_desktop_window_loaded              (AnchorDesktopWindow *window);

#endif /* NAUTILUS_DESKTOP_WINDOW_H */
