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

  queue.h  -  queue (FIFO) and operators

*/
#ifndef PW13_QUEUE_H_INCLUDED
#define PW13_QUEUE_H_INCLUDED 1

#include "dll.h"
#include "types.h"
#include "list.h"

struct Pw13_Queue {
  Pw13_List *head;
  Pw13_List *tail;
  int len;
};


/** Initialize an allocated queue 
 **/
PW13_EXTERN
void pw13_queue_init (Pw13_Queue *q);

/** Free all the elements but not the queue 
 **/
PW13_EXTERN
void pw13_queue_clear (Pw13_Queue *q);

/** Returns wether the queue is empty or not
 **/
PW13_EXTERN
int pw13_queue_empty (Pw13_Queue *q);

/** Destruct each element but not the queue 
 **/
PW13_EXTERN
void pw13_queue_destruct (Pw13_Queue *q, void (*destructor) (void*));

/** Queue an element 
 **/
PW13_EXTERN
int pw13_queue_push (Pw13_Queue *q, void *elem);

/** Return and remove the first element in the queue 
 **/
PW13_EXTERN
void * pw13_queue_pull (Pw13_Queue *q);


#endif
/* ndef PW13_QUEUE_H_INCLUDED */
