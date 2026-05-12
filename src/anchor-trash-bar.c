/* -*- Mode: C; tab-width: 8; indent-tabs-mode: t; c-basic-offset: 8 -*- */
/*
 * Copyright (C) 2006 Paolo Borelli <pborelli@katamail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Paolo Borelli <pborelli@katamail.com>
 *
 */

#include "config.h"

#include <glib/gi18n.h>
#include <gtk/gtk.h>

#include "anchor-trash-bar.h"

#include "anchor-view.h"
#include <libanchor-private/anchor-file-operations.h>
#include <libanchor-private/anchor-file-utilities.h>
#include <libanchor-private/anchor-file.h>
#include <libanchor-private/anchor-trash-monitor.h>

#define NAUTILUS_TRASH_BAR_GET_PRIVATE(o)\
	(G_TYPE_INSTANCE_GET_PRIVATE ((o), ANCHOR_TYPE_TRASH_BAR, AnchorTrashBarPrivate))

enum {
	PROP_VIEW = 1,
	NUM_PROPERTIES
};

enum {
	TRASH_BAR_RESPONSE_EMPTY = 1,
	TRASH_BAR_RESPONSE_RESTORE
};

struct AnchorTrashBarPrivate
{
	AnchorView *view;
	gulong selection_handler_id;
};

G_DEFINE_TYPE (AnchorTrashBar, anchor_trash_bar, GTK_TYPE_INFO_BAR);

static void
selection_changed_cb (AnchorView *view,
		      AnchorTrashBar *bar)
{
	int count;

	count = anchor_view_get_selection_count (view);

	gtk_info_bar_set_response_sensitive (GTK_INFO_BAR (bar),
					     TRASH_BAR_RESPONSE_RESTORE,
					     (count > 0));
}

static void
connect_view_and_update_button (AnchorTrashBar *bar)
{
	bar->priv->selection_handler_id =
		g_signal_connect (bar->priv->view, "selection-changed",
				  G_CALLBACK (selection_changed_cb), bar);

	selection_changed_cb (bar->priv->view, bar);
}

static void
anchor_trash_bar_set_property (GObject      *object,
				 guint         prop_id,
				 const GValue *value,
				 GParamSpec   *pspec)
{
	AnchorTrashBar *bar;

	bar = NAUTILUS_TRASH_BAR (object);

	switch (prop_id) {
	case PROP_VIEW:
		bar->priv->view = g_value_get_object (value);
		connect_view_and_update_button (bar);
		break;
	default:
		G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
		break;
	}
}

static void
anchor_trash_bar_dispose (GObject *obj)
{
	AnchorTrashBar *bar;

	bar = NAUTILUS_TRASH_BAR (obj);

	if (bar->priv->selection_handler_id) {
		g_signal_handler_disconnect (bar->priv->view, bar->priv->selection_handler_id);
		bar->priv->selection_handler_id = 0;
	}

	G_OBJECT_CLASS (anchor_trash_bar_parent_class)->dispose (obj);
}

static void
anchor_trash_bar_trash_state_changed (AnchorTrashMonitor *trash_monitor,
					gboolean              state,
					gpointer              data)
{
	AnchorTrashBar *bar;

	bar = NAUTILUS_TRASH_BAR (data);

	gtk_info_bar_set_response_sensitive (GTK_INFO_BAR (bar),
					     TRASH_BAR_RESPONSE_EMPTY,
					     !anchor_trash_monitor_is_empty ());
}

static void
anchor_trash_bar_class_init (AnchorTrashBarClass *klass)
{
	GObjectClass *object_class;

	object_class = G_OBJECT_CLASS (klass);

	object_class->set_property = anchor_trash_bar_set_property;
	object_class->dispose = anchor_trash_bar_dispose;

	g_object_class_install_property (object_class,
					 PROP_VIEW,
					 g_param_spec_object ("view",
							      "view",
							      "the AnchorView",
							      ANCHOR_TYPE_VIEW,
							      G_PARAM_WRITABLE |
							      G_PARAM_CONSTRUCT_ONLY |
							      G_PARAM_STATIC_STRINGS));

	g_type_class_add_private (klass, sizeof (AnchorTrashBarPrivate));
}

static void
trash_bar_response_cb (GtkInfoBar *infobar,
		       gint response_id,
		       gpointer user_data)
{
	AnchorTrashBar *bar;
	GtkWidget *window;
	GList *files;

	bar = NAUTILUS_TRASH_BAR (infobar);
	window = gtk_widget_get_toplevel (GTK_WIDGET (bar));

	switch (response_id) {
	case TRASH_BAR_RESPONSE_EMPTY:
		anchor_file_operations_empty_trash (window);
		break;
	case TRASH_BAR_RESPONSE_RESTORE:
		files = anchor_view_get_selection (bar->priv->view);
		anchor_restore_files_from_trash (files, GTK_WINDOW (window));
		anchor_file_list_free (files);
		break;
	default:
		break;
	}
}

static void
anchor_trash_bar_init (AnchorTrashBar *bar)
{
	GtkWidget *content_area, *action_area, *w;
	GtkWidget *label;
	PangoAttrList *attrs;

	bar->priv = NAUTILUS_TRASH_BAR_GET_PRIVATE (bar);
	content_area = gtk_info_bar_get_content_area (GTK_INFO_BAR (bar));
	action_area = gtk_info_bar_get_action_area (GTK_INFO_BAR (bar));

	gtk_orientable_set_orientation (GTK_ORIENTABLE (action_area),
					GTK_ORIENTATION_HORIZONTAL);

	attrs = pango_attr_list_new ();
	pango_attr_list_insert (attrs, pango_attr_weight_new (PANGO_WEIGHT_BOLD));
	label = gtk_label_new (_("Trash"));
	gtk_label_set_attributes (GTK_LABEL (label), attrs);
	pango_attr_list_unref (attrs);

	gtk_widget_show (label);
	gtk_container_add (GTK_CONTAINER (content_area), label);

	w = gtk_info_bar_add_button (GTK_INFO_BAR (bar),
				     _("Restore"),
				     TRASH_BAR_RESPONSE_RESTORE);
	gtk_widget_set_tooltip_text (w,
				     _("Restore selected items to their original position"));

	w = gtk_info_bar_add_button (GTK_INFO_BAR (bar),
	/* Translators: "Empty" is an action (for the trash) , not a state */
				     _("Empty"),
				     TRASH_BAR_RESPONSE_EMPTY);
	gtk_widget_set_tooltip_text (w,
				     _("Delete all items in the Trash"));

	g_signal_connect_object (anchor_trash_monitor_get (),
				 "trash-state-changed",
				 G_CALLBACK (anchor_trash_bar_trash_state_changed),
				 bar,
				 0);
	anchor_trash_bar_trash_state_changed (anchor_trash_monitor_get (),
						FALSE, bar);

	g_signal_connect (bar, "response",
			  G_CALLBACK (trash_bar_response_cb), bar);
}

GtkWidget *
anchor_trash_bar_new (AnchorView *view)
{
	return g_object_new (ANCHOR_TYPE_TRASH_BAR,
			     "view", view,
			     "message-type", GTK_MESSAGE_QUESTION,
			     NULL);
}
