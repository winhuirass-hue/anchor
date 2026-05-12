/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
   anchor-mime-application-chooser.c: Manages applications for mime types
 
   Copyright (C) 2004 Novell, Inc.
 
   The Gnome Library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Library General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.

   The Gnome Library is distributed in the hope that it will be useful,
   but APPLICATIONOUT ANY WARRANTY; applicationout even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Library General Public License for more details.

   You should have received a copy of the GNU Library General Public
   License along application the Gnome Library; see the file COPYING.LIB.  If not,
   see <http://www.gnu.org/licenses/>.

   Authors: Dave Camp <dave@novell.com>
*/

#ifndef NAUTILUS_MIME_APPLICATION_CHOOSER_H
#define NAUTILUS_MIME_APPLICATION_CHOOSER_H

#include <gtk/gtk.h>

#define ANCHOR_TYPE_MIME_APPLICATION_CHOOSER         (anchor_mime_application_chooser_get_type ())
#define NAUTILUS_MIME_APPLICATION_CHOOSER(obj)         (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_MIME_APPLICATION_CHOOSER, AnchorMimeApplicationChooser))
#define NAUTILUS_MIME_APPLICATION_CHOOSER_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_MIME_APPLICATION_CHOOSER, AnchorMimeApplicationChooserClass))
#define NAUTILUS_IS_MIME_APPLICATION_CHOOSER(obj)      (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_MIME_APPLICATION_CHOOSER)

typedef struct _NautilusMimeApplicationChooser        AnchorMimeApplicationChooser;
typedef struct _NautilusMimeApplicationChooserClass   AnchorMimeApplicationChooserClass;
typedef struct _NautilusMimeApplicationChooserDetails AnchorMimeApplicationChooserDetails;

struct _NautilusMimeApplicationChooser {
	GtkBox parent;
	AnchorMimeApplicationChooserDetails *details;
};

struct _NautilusMimeApplicationChooserClass {
	GtkBoxClass parent_class;
};

GType      anchor_mime_application_chooser_get_type (void);
GtkWidget * anchor_mime_application_chooser_new (GList *files,
						   const char *mime_type);

#endif /* NAUTILUS_MIME_APPLICATION_CHOOSER_H */
