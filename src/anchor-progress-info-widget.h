/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * anchor-progress-info-widget.h: file operation progress user interface.
 *
 * Copyright (C) 2007, 2011 Red Hat, Inc.
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Alexander Larsson <alexl@redhat.com>
 *          Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#ifndef __NAUTILUS_PROGRESS_INFO_WIDGET_H__
#define __NAUTILUS_PROGRESS_INFO_WIDGET_H__

#include <gtk/gtk.h>

#include <libanchor-private/anchor-progress-info.h>

#define ANCHOR_TYPE_PROGRESS_INFO_WIDGET anchor_progress_info_widget_get_type()
#define NAUTILUS_PROGRESS_INFO_WIDGET(obj) \
	(G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_PROGRESS_INFO_WIDGET, AnchorProgressInfoWidget))
#define NAUTILUS_PROGRESS_INFO_WIDGET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_PROGRESS_INFO_WIDGET, AnchorProgressInfoWidgetClass))
#define NAUTILUS_IS_PROGRESS_INFO_WIDGET(obj) \
	(G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_PROGRESS_INFO_WIDGET))
#define NAUTILUS_IS_PROGRESS_INFO_WIDGET_CLASS(klass) \
	(G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_PROGRESS_INFO_WIDGET))
#define NAUTILUS_PROGRESS_INFO_WIDGET_GET_CLASS(obj) \
	(G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_PROGRESS_INFO_WIDGET, AnchorProgressInfoWidgetClass))

typedef struct _NautilusProgressInfoWidgetPriv AnchorProgressInfoWidgetPriv;

typedef struct {
	GtkBox parent;

	/* private */
	AnchorProgressInfoWidgetPriv *priv;
} AnchorProgressInfoWidget;

typedef struct {
	GtkBoxClass parent_class;
} AnchorProgressInfoWidgetClass;

GType anchor_progress_info_widget_get_type (void);

GtkWidget * anchor_progress_info_widget_new (AnchorProgressInfo *info);

#endif /* __NAUTILUS_PROGRESS_INFO_WIDGET_H__ */
