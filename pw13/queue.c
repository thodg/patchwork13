/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13 - the patchwork13 core library
  Copyright (C) 2005 Thomas de Grivel <billitch@yahoo.fr>
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*

  queue.c  -  queue (FIFO) and operators

*/
#include <stdlib.h> /* NULL */
#include "list.h"
#include "queue.h"


/* initialize an allocated queue */
void
pw13_queue_init (Pw13_Queue *q)
{
  q->head = q->tail = NULL;
  q->len = 0;
}

/* free all the elements but not the queue */
void
pw13_queue_clear (Pw13_Queue *q)
{
  pw13_list_free (q->head);
  q->head = q->tail = NULL;
  q->len = 0;
}

/* returns wether the queue is empty or not */
int
pw13_queue_empty (Pw13_Queue *q)
{
  return !q->len;
}

/* destruct each element but not the queue */
void
pw13_queue_destruct (Pw13_Queue *q, void (*destructor) (void*))
{
  pw13_list_destroy (q->head, destructor);
  q->head = q->tail = NULL;
  q->len = 0;
}

/* queue an element */
int pw13_queue_push (Pw13_Queue *q, void *elem)
{
  Pw13_List *n = pw13_list (elem, NULL);
  if (!n)
    return 0;
  if (q->tail)
    q->tail->suiv = n;
  else
    q->head = n;
  q->tail = n;
  q->len++;
  return 1; 
}

/* return and remove the first element in the queue */
void *
pw13_queue_pull (Pw13_Queue *q)
{
  void *e;
  if (!q->head)
    return NULL;
  e = q->head->elem;
  pw13_list_remove_first (&q->head);
  if (!q->head)
    q->tail = NULL;
  q->len--;
  return e;
}
