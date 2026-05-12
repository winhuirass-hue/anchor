/*
 *  anchor-property-page.h - Property pages exported by 
 *                             AnchorPropertyProvider objects.
 *
 *  Copyright (C) 2003 Novell, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Library General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU Library General Public
 *  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 * 
 *  Author:  Dave Camp <dave@ximian.com>
 *
 */

#ifndef NAUTILUS_PROPERTY_PAGE_H
#define NAUTILUS_PROPERTY_PAGE_H

#include <glib-object.h>
#include <gtk/gtk.h>
#include "anchor-extension-types.h"

G_BEGIN_DECLS

#define ANCHOR_TYPE_PROPERTY_PAGE            (anchor_property_page_get_type())
#define NAUTILUS_PROPERTY_PAGE(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_PROPERTY_PAGE, AnchorPropertyPage))
#define NAUTILUS_PROPERTY_PAGE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_PROPERTY_PAGE, AnchorPropertyPageClass))
#define NAUTILUS_IS_PROPERTY_PAGE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_PROPERTY_PAGE))
#define NAUTILUS_IS_PROPERTY_PAGE_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), ANCHOR_TYPE_PROPERTY_PAGE))
#define NAUTILUS_PROPERTY_PAGE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), ANCHOR_TYPE_PROPERTY_PAGE, AnchorPropertyPageClass))

typedef struct _NautilusPropertyPage        AnchorPropertyPage;
typedef struct _NautilusPropertyPageDetails AnchorPropertyPageDetails;
typedef struct _NautilusPropertyPageClass   AnchorPropertyPageClass;

struct _NautilusPropertyPage
{
	GObject parent;

	AnchorPropertyPageDetails *details;
};

struct _NautilusPropertyPageClass 
{
	GObjectClass parent;
};

GType                 anchor_property_page_get_type  (void);
AnchorPropertyPage *anchor_property_page_new       (const char           *name,
							GtkWidget            *label,
							GtkWidget            *page);

/* AnchorPropertyPage has the following properties:
 *   name (string)        - the identifier for the property page
 *   label (widget)       - the user-visible label of the property page
 *   page (widget)        - the property page to display
 */

G_END_DECLS

#endif
