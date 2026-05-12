/*
 *  Anchor-sendto
 *
 *  Copyright (C) 2004 Free Software Foundation, Inc.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU General Public
 *  License as published by the Free Software Foundation; either
 *  version 2 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Library General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 *  Author: Roberto Majadas <roberto.majadas@openshine.com>
 *
 */

#include <config.h>
#include <string.h>
#include <glib/gi18n-lib.h>
#include <libanchor-extension/anchor-extension-types.h>
#include <libanchor-extension/anchor-file-info.h>
#include <libanchor-extension/anchor-menu-provider.h>
#include "anchor-nste.h"


static GObjectClass *parent_class;

static void
sendto_callback (AnchorMenuItem *item,
		 gpointer          user_data)
{
	GList            *files, *scan;
	gchar            *uri;
	GString          *cmd;

	files = g_object_get_data (G_OBJECT (item), "files");
	cmd = g_string_new ("anchor-sendto");

	for (scan = files; scan; scan = scan->next) {
		AnchorFileInfo *file = scan->data;

		uri = anchor_file_info_get_uri (file);
		g_string_append_printf (cmd, " \"%s\"", uri);
		g_free (uri);
	}

	g_spawn_command_line_async (cmd->str, NULL);

	g_string_free (cmd, TRUE);
}

static gboolean
check_available_mailer ()
{
	GAppInfo *app_info;

	app_info = g_app_info_get_default_for_uri_scheme ("mailto");
	if (app_info) {
		g_clear_object (&app_info);
		return TRUE;
	}
	else
		return FALSE;
}

static GList *
anchor_nste_get_file_items (AnchorMenuProvider *provider,
			      GtkWidget            *window,
			      GList                *files)
{
	GList    *items = NULL;
	gboolean  one_item;
	AnchorMenuItem *item;
	AnchorNste *nste;

	nste = ANCHOR_NSTE (provider);
	if (!nste->nst_present)
		return NULL;

	if (files == NULL)
		return NULL;
  
	if (!check_available_mailer ())
		return NULL;

	one_item = (files != NULL) && (files->next == NULL);
	if (one_item &&
	    !anchor_file_info_is_directory ((AnchorFileInfo *)files->data)) {
		item = anchor_menu_item_new ("AnchorNste::sendto",
					       _("Email…"),
					       _("Send file by mail…"),
					       "document-send");
	} else {
		item = anchor_menu_item_new ("AnchorNste::sendto",
					       _("Email…"),
					       _("Send files by mail…"),
					       "document-send");
	}
	g_signal_connect (item,
			  "activate",
			  G_CALLBACK (sendto_callback),
			  provider);
	g_object_set_data_full (G_OBJECT (item),
				"files",
				anchor_file_info_list_copy (files),
				(GDestroyNotify) anchor_file_info_list_free);

	items = g_list_append (items, item);

	return items;
}

static void
anchor_nste_menu_provider_iface_init (AnchorMenuProviderIface *iface)
{
	iface->get_file_items = anchor_nste_get_file_items;
}

static void
anchor_nste_instance_init (AnchorNste *nste)
{
	char *path;

	path = g_find_program_in_path ("anchor-sendto");
	nste->nst_present = (path != NULL);
	g_free (path);
}

static void
anchor_nste_class_init (AnchorNsteClass *class)
{
	parent_class = g_type_class_peek_parent (class);
}

static GType nste_type = 0;

GType
anchor_nste_get_type (void)
{
	return nste_type;
}

void
anchor_nste_register_type (GTypeModule *module)
{
	static const GTypeInfo info = {
		sizeof (AnchorNsteClass),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) anchor_nste_class_init,
		NULL,
		NULL,
		sizeof (AnchorNste),
		0,
		(GInstanceInitFunc) anchor_nste_instance_init,
	};

	static const GInterfaceInfo menu_provider_iface_info = {
		(GInterfaceInitFunc) anchor_nste_menu_provider_iface_init,
		NULL,
		NULL
	};

	nste_type = g_type_module_register_type (module,
						 G_TYPE_OBJECT,
						 "AnchorNste",
						 &info, 0);

	g_type_module_add_interface (module,
				     nste_type,
				     ANCHOR_TYPE_MENU_PROVIDER,
				     &menu_provider_iface_info);
}
