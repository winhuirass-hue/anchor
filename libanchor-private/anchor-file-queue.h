/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 8; tab-width: 8 -*-

   Copyright (C) 2001 Maciej Stachowiak
  
   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; either version 2 of the
   License, or (at your option) any later version.
  
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.
  
   You should have received a copy of the GNU General Public
   License along with this program; if not, see <http://www.gnu.org/licenses/>.

   Author: Maciej Stachowiak <mjs@noisehavoc.org>
*/

#ifndef NAUTILUS_FILE_QUEUE_H
#define NAUTILUS_FILE_QUEUE_H

#include <libanchor-private/anchor-file.h>

typedef struct AnchorFileQueue AnchorFileQueue;

AnchorFileQueue *anchor_file_queue_new      (void);
void               anchor_file_queue_destroy  (AnchorFileQueue *queue);

/* Add a file to the tail of the queue, unless it's already in the queue */
void               anchor_file_queue_enqueue  (AnchorFileQueue *queue,
						 AnchorFile      *file);

/* Return the file at the head of the queue after removing it from the
 * queue. This is dangerous unless you have another ref to the file,
 * since it will unref it.  
 */
AnchorFile *     anchor_file_queue_dequeue  (AnchorFileQueue *queue);

/* Remove a file from an arbitrary point in the queue in constant time. */
void               anchor_file_queue_remove   (AnchorFileQueue *queue,
						 AnchorFile      *file);

/* Get the file at the head of the queue without removing or unrefing it. */
AnchorFile *     anchor_file_queue_head     (AnchorFileQueue *queue);

gboolean           anchor_file_queue_is_empty (AnchorFileQueue *queue);

#endif /* NAUTILUS_FILE_CHANGES_QUEUE_H */
