/*
 *  Copyright © 2002 Christophe Fergeau
 *  Copyright © 2003 Marco Pesenti Gritti
 *  Copyright © 2003, 2004 Christian Persch
 *    (ephy-notebook.c)
 *
 *  Copyright © 2008 Free Software Foundation, Inc.
 *    (anchor-notebook.c)
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2, or (at your option)
 *  any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef NAUTILUS_NOTEBOOK_H
#define NAUTILUS_NOTEBOOK_H

#include <glib.h>
#include <gtk/gtk.h>

typedef struct _NautilusNotebookClass	AnchorNotebookClass;
typedef struct _NautilusNotebook	AnchorNotebook;
typedef struct _NautilusNotebookPrivate	AnchorNotebookPrivate;

#include "anchor-window-slot.h"

G_BEGIN_DECLS

#define ANCHOR_TYPE_NOTEBOOK		(anchor_notebook_get_type ())
#define NAUTILUS_NOTEBOOK(o)		(G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_NOTEBOOK, AnchorNotebook))
#define NAUTILUS_NOTEBOOK_CLASS(k)		(G_TYPE_CHECK_CLASS_CAST((k), ANCHOR_TYPE_NOTEBOOK, AnchorNotebookClass))
#define NAUTILUS_IS_NOTEBOOK(o)		(G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_NOTEBOOK))
#define NAUTILUS_IS_NOTEBOOK_CLASS(k)	(G_TYPE_CHECK_CLASS_TYPE ((k), ANCHOR_TYPE_NOTEBOOK))
#define NAUTILUS_NOTEBOOK_GET_CLASS(o)	(G_TYPE_INSTANCE_GET_CLASS ((o), ANCHOR_TYPE_NOTEBOOK, AnchorNotebookClass))

struct _NautilusNotebook
{
	GtkNotebook parent;

	/*< private >*/
        AnchorNotebookPrivate *priv;
};

struct _NautilusNotebookClass
{
        GtkNotebookClass parent_class;

	/* Signals */
	void	 (* tab_close_request)  (AnchorNotebook *notebook,
					 AnchorWindowSlot *slot);
};

GType		anchor_notebook_get_type		(void);

int		anchor_notebook_add_tab	(AnchorNotebook *nb,
						 AnchorWindowSlot *slot,
						 int position,
						 gboolean jump_to);
	
void		anchor_notebook_sync_tab_label (AnchorNotebook *nb,
						  AnchorWindowSlot *slot);
void		anchor_notebook_sync_loading   (AnchorNotebook *nb,
						  AnchorWindowSlot *slot);

void		anchor_notebook_reorder_current_child_relative (AnchorNotebook *notebook,
								  int offset);
gboolean        anchor_notebook_can_reorder_current_child_relative (AnchorNotebook *notebook,
								      int offset);
void            anchor_notebook_prev_page (AnchorNotebook *notebook);
void            anchor_notebook_next_page (AnchorNotebook *notebook);


G_END_DECLS

#endif /* NAUTILUS_NOTEBOOK_H */

