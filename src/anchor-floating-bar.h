/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/* Anchor - Floating status bar.
 *
 * Copyright (C) 2011 Red Hat Inc.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Library General Public
 * License as published by the Free Software Foundation; either
 * version 2 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Library General Public License for more details.
 *
 * You should have received a copy of the GNU Library General Public
 * License along with this library; if not, see <http://www.gnu.org/licenses/>.
 *
 * Authors: Cosimo Cecchi <cosimoc@redhat.com>
 *
 */

#ifndef __NAUTILUS_FLOATING_BAR_H__
#define __NAUTILUS_FLOATING_BAR_H__

#include <gtk/gtk.h>

#define NAUTILUS_FLOATING_BAR_ACTION_ID_STOP 1

#define ANCHOR_TYPE_FLOATING_BAR anchor_floating_bar_get_type()
#define NAUTILUS_FLOATING_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_CAST ((obj), ANCHOR_TYPE_FLOATING_BAR, AnchorFloatingBar))
#define NAUTILUS_FLOATING_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_CAST ((klass), ANCHOR_TYPE_FLOATING_BAR, AnchorFloatingBarClass))
#define NAUTILUS_IS_FLOATING_BAR(obj) \
  (G_TYPE_CHECK_INSTANCE_TYPE ((obj), ANCHOR_TYPE_FLOATING_BAR))
#define NAUTILUS_IS_FLOATING_BAR_CLASS(klass) \
  (G_TYPE_CHECK_CLASS_TYPE ((klass), ANCHOR_TYPE_FLOATING_BAR))
#define NAUTILUS_FLOATING_BAR_GET_CLASS(obj) \
  (G_TYPE_INSTANCE_GET_CLASS ((obj), ANCHOR_TYPE_FLOATING_BAR, AnchorFloatingBarClass))

typedef struct _NautilusFloatingBar AnchorFloatingBar;
typedef struct _NautilusFloatingBarClass AnchorFloatingBarClass;
typedef struct _NautilusFloatingBarDetails AnchorFloatingBarDetails;

struct _NautilusFloatingBar {
	GtkBox parent;
	AnchorFloatingBarDetails *priv;
};

struct _NautilusFloatingBarClass {
	GtkBoxClass parent_class;
};

/* GObject */
GType       anchor_floating_bar_get_type  (void);

GtkWidget * anchor_floating_bar_new              (const gchar *primary_label,
						    const gchar *details_label,
						    gboolean show_spinner);

void       anchor_floating_bar_set_primary_label (AnchorFloatingBar *self,
						    const gchar *label);
void       anchor_floating_bar_set_details_label (AnchorFloatingBar *self,
						    const gchar *label);
void        anchor_floating_bar_set_labels        (AnchorFloatingBar *self,
						     const gchar *primary,
						     const gchar *detail);
void        anchor_floating_bar_set_show_spinner (AnchorFloatingBar *self,
						    gboolean show_spinner);

void        anchor_floating_bar_add_action       (AnchorFloatingBar *self,
						    const gchar *icon_name,
						    gint action_id);
void        anchor_floating_bar_cleanup_actions  (AnchorFloatingBar *self);

#endif /* __NAUTILUS_FLOATING_BAR_H__ */

