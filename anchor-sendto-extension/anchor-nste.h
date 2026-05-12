/*
 *  Anchor SendTo extension
 *
 *  Copyright (C) 2005 Roberto Majadas
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

#ifndef ANCHOR_NSTE_H
#define ANCHOR_NSTE_H

#include <glib-object.h>

G_BEGIN_DECLS

#define ANCHOR_TYPE_NSTE  (anchor_nste_get_type ())
#define ANCHOR_NSTE(o)    (G_TYPE_CHECK_INSTANCE_CAST ((o), ANCHOR_TYPE_NSTE, AnchorNste))
#define NAUTILUS_IS_NSTE(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), ANCHOR_TYPE_NSTE))

typedef struct _AnchorNste      AnchorNste;
typedef struct _AnchorNsteClass AnchorNsteClass;

struct _AnchorNste {
	GObject  __parent;
	gboolean nst_present;
};

struct _AnchorNsteClass {
	GObjectClass __parent;
};

GType anchor_nste_get_type      (void);
void  anchor_nste_register_type (GTypeModule *module);

G_END_DECLS

#endif /* ANCHOR_NSTE_H */
