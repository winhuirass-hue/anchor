/*
 *  anchor-menu.h - Menus exported by AnchorMenuProvider objects.
 *
 *  Copyright (C) 2005 Raffaele Sandrini
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
 *           Raffaele Sandrini <rasa@gmx.ch>
 *
 */

#ifndef ANCHOR_MENU_H
#define ANCHOR_MENU_H

#include <glib-object.h>
#include "anchor-extension-types.h"


G_BEGIN_DECLS

/* AnchorMenu defines */
#define ANCHOR_TYPE_MENU         (anchor_menu_get_type ())
#define ANCHOR_MENU(o)           (G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_MENU, AnchorMenu))
#define ANCHOR_MENU_CLASS(k)     (G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_MENU, AnchorMenuClass))
#define NAUTILUS_IS_MENU(o)        (G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_MENU))
#define NAUTILUS_IS_MENU_CLASS(k)  (G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_MENU))
#define ANCHOR_MENU_GET_CLASS(o) (G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_MENU, AnchorMenuClass))
/* AnchorMenuItem defines */
#define ANCHOR_TYPE_MENU_ITEM            (anchor_menu_item_get_type())
#define ANCHOR_MENU_ITEM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_MENU_ITEM, AnchorMenuItem))
#define ANCHOR_MENU_ITEM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_MENU_ITEM, AnchorMenuItemClass))
#define ANCHOR_MENU_IS_ITEM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_MENU_ITEM))
#define ANCHOR_MENU_IS_ITEM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((obj), ANCHOR_TYPE_MENU_ITEM))
#define ANCHOR_MENU_ITEM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj), ANCHOR_TYPE_MENU_ITEM, AnchorMenuItemClass))


/* AnchorMenu types */
typedef struct _AnchorMenu		AnchorMenu;
typedef struct _AnchorMenuPrivate	AnchorMenuPrivate;
typedef struct _AnchorMenuClass	AnchorMenuClass;
/* AnchorMenuItem types */
typedef struct _AnchorMenuItem        AnchorMenuItem;
typedef struct _AnchorMenuItemDetails AnchorMenuItemDetails;
typedef struct _AnchorMenuItemClass   AnchorMenuItemClass;


/* AnchorMenu structs */
struct _AnchorMenu {
	GObject parent;
	AnchorMenuPrivate *priv;
};

struct _AnchorMenuClass {
	GObjectClass parent_class;
};

/* AnchorMenuItem structs */
struct _AnchorMenuItem {
	GObject parent;

	AnchorMenuItemDetails *details;
};

struct _AnchorMenuItemClass {
	GObjectClass parent;

	void (*activate) (AnchorMenuItem *item);
};


/* AnchorMenu methods */
GType		anchor_menu_get_type	(void);
AnchorMenu *	anchor_menu_new	(void);

void	anchor_menu_append_item	(AnchorMenu      *menu,
					 AnchorMenuItem  *item);
GList*	anchor_menu_get_items		(AnchorMenu *menu);
void	anchor_menu_item_list_free	(GList *item_list);

/* AnchorMenuItem methods */
GType             anchor_menu_item_get_type      (void);
AnchorMenuItem *anchor_menu_item_new           (const char       *name,
						    const char       *label,
						    const char       *tip,
						    const char       *icon);

void              anchor_menu_item_activate      (AnchorMenuItem *item);
void              anchor_menu_item_set_submenu   (AnchorMenuItem *item,
						    AnchorMenu     *menu);
/* AnchorMenuItem has the following properties:
 *   name (string)        - the identifier for the menu item
 *   label (string)       - the user-visible label of the menu item
 *   tip (string)         - the tooltip of the menu item 
 *   icon (string)        - the name of the icon to display in the menu item
 *   sensitive (boolean)  - whether the menu item is sensitive or not
 *   priority (boolean)   - used for toolbar items, whether to show priority
 *                          text.
 *   menu (AnchorMenu)  - The menu belonging to this item. May be null.
 */

G_END_DECLS

#endif /* ANCHOR_MENU_H */
