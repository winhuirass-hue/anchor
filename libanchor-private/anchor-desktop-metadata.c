/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */
/*
 * Anchor
 *
 * Copyright (C) 2011 Red Hat, Inc.
 *
 * Anchor is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * Anchor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; see the file COPYING.  If not,
 * see <http://www.gnu.org/licenses/>.
 *
 * Authors: Cosimo Cecchi <cosimoc@redhat.com>
 */

#include <config.h>

#include "anchor-desktop-metadata.h"

#include "anchor-file-utilities.h"
#include "anchor-keyfile-metadata.h"

static gchar *
get_keyfile_path (void)
{
	gchar *xdg_dir, *retval;

	xdg_dir = anchor_get_user_directory ();
	retval = g_build_filename (xdg_dir, "desktop-metadata", NULL);

	g_free (xdg_dir);

	return retval;
}

void
anchor_desktop_set_metadata_string (AnchorFile *file,
                                      const gchar *name,
                                      const gchar *key,
                                      const gchar *string)
{
	gchar *keyfile_filename;

	keyfile_filename = get_keyfile_path ();

	anchor_keyfile_metadata_set_string (file, keyfile_filename,
	                                      name, key, string);

	g_free (keyfile_filename);
}

void
anchor_desktop_set_metadata_stringv (AnchorFile *file,
                                       const char *name,
                                       const char *key,
                                       const char * const *stringv)
{
	gchar *keyfile_filename;

	keyfile_filename = get_keyfile_path ();

	anchor_keyfile_metadata_set_stringv (file, keyfile_filename,
	                                       name, key, stringv);

	g_free (keyfile_filename);
}

gboolean
anchor_desktop_update_metadata_from_keyfile (AnchorFile *file,
					       const gchar *name)
{
	gchar *keyfile_filename;
	gboolean result;

	keyfile_filename = get_keyfile_path ();

	result = anchor_keyfile_metadata_update_from_keyfile (file,
	                                                        keyfile_filename,
	                                                        name);

	g_free (keyfile_filename);
	return result;
}
