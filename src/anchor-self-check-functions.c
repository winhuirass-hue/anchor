/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*- */

/*
 * Anchor
 *
 * Copyright (C) 1999, 2000 Eazel, Inc.
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
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, see <http://www.gnu.org/licenses/>.
 *
 * Author: Darin Adler <darin@bentspoon.com>
 */
   
/* anchor-self-check-functions.c: Wrapper for all self check functions
 * in Anchor proper.
 */

#include <config.h>

#if ! defined (NAUTILUS_OMIT_SELF_CHECK)

#include "anchor-self-check-functions.h"

void anchor_run_self_checks(void)
{
	NAUTILUS_FOR_EACH_SELF_CHECK_FUNCTION (NAUTILUS_CALL_SELF_CHECK_FUNCTION)
}

#endif /* ! NAUTILUS_OMIT_SELF_CHECK */
