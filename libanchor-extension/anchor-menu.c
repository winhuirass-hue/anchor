/*
 *  anchor-menu.h - Menus exported by AnchorMenuProvider objects.
 *
 *  Copyright (C) 2005 Raffaele Sandrini
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
 *  Author:  Raffaele Sandrini <rasa@gmx.ch>
 *
 */

#include <config.h>
#include "anchor-menu.h"
#include "anchor-extension-i18n.h"

#include <glib.h>

#define ANCHOR_MENU_GET_PRIVATE(o) (G_TYPE_INSTANCE_GET_PRIVATE ((o), ANCHOR_TYPE_MENU, AnchorMenuPrivate))
G_DEFINE_TYPE (AnchorMenu, anchor_menu, G_TYPE_OBJECT);

struct _AnchorMenuPrivate {
	GList *item_list;
};

void
anchor_menu_append_item (AnchorMenu *menu, AnchorMenuItem *item)
{
	g_return_if_fail (menu != NULL);
	g_return_if_fail (item != NULL);
	
	menu->priv->item_list = g_list_append (menu->priv->item_list, g_object_ref (item));
}

/**
 * anchor_menu_get_items:
 * @menu: a #AnchorMenu
 *
 * Returns: (element-type AnchorMenuItem) (transfer full): the provided #AnchorMenuItem list
 */
GList *
anchor_menu_get_items (AnchorMenu *menu)
{
	GList *item_list;

	g_return_val_if_fail (menu != NULL, NULL);
	
	item_list = g_list_copy (menu->priv->item_list);
	g_list_foreach (item_list, (GFunc)g_object_ref, NULL);
	
	return item_list;
}

/**
 * anchor_menu_item_list_free:
 * @item_list: (element-type AnchorMenuItem): a list of #AnchorMenuItem
 *
 */
void
anchor_menu_item_list_free (GList *item_list)
{
	g_return_if_fail (item_list != NULL);
	
	g_list_foreach (item_list, (GFunc)g_object_unref, NULL);
	g_list_free (item_list);
}

/* Type initialization */

static void
anchor_menu_finalize (GObject *object)
{
	AnchorMenu *menu = ANCHOR_MENU (object);

	if (menu->priv->item_list) {
		g_list_free (menu->priv->item_list);
	}

	G_OBJECT_CLASS (anchor_menu_parent_class)->finalize (object);
}

static void
anchor_menu_init (AnchorMenu *menu)
{
	menu->priv = ANCHOR_MENU_GET_PRIVATE (menu);

	menu->priv->item_list = NULL;
}

static void
anchor_menu_class_init (AnchorMenuClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);
	
	g_type_class_add_private (klass, sizeof (AnchorMenuPrivate));
	
	object_class->finalize = anchor_menu_finalize;
}

/* public constructors */

AnchorMenu *
anchor_menu_new (void)
{
	AnchorMenu *obj;
	
	obj = ANCHOR_MENU (g_object_new (ANCHOR_TYPE_MENU, NULL));
	
	return obj;
}
