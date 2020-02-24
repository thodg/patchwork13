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

  list.h  -  simple linked list and operators

*/
#ifndef PW13_LIST_H_INCLUDED
#define PW13_LIST_H_INCLUDED

#include "dll.h"
#include "error.h"
#include "types.h"


struct		Pw13_List {
  void		*elem;
  Pw13_List	*suiv;
};


/** Allocate and initialize a new list */
PW13_EXTERN
Pw13_List * pw13_list (void *E, Pw13_List *L);


/** Insert an element at the beginning of the list */
PW13_EXTERN
int pw13_list_insert_head (Pw13_List **L, void *E);

/** Insert an element at the end of the list (slow) */
PW13_EXTERN
int pw13_list_insert_tail (Pw13_List **L, void *E);

/** Insert an element in a sorted list */
PW13_EXTERN
int pw13_list_insert_sorted (Pw13_List **L, void *E,
			     int (*compare) (void*,void*));


/** Find the list beginning with an element */
PW13_EXTERN
Pw13_List * pw13_list_find (Pw13_List *L, void *E);

/** Find the link to the list beginning with an element */
PW13_EXTERN
Pw13_List ** pw13_list_find_p (Pw13_List **L, void *E);


/** Remove the first element of the list */
PW13_EXTERN
Pw13_Result pw13_list_remove_first (Pw13_List **L);

/** Remove the first occurence of an element in the list */
PW13_EXTERN
Pw13_Result pw13_list_remove_one (Pw13_List **L, void *E);

/** Remove all the occurences of an element in the list */
PW13_EXTERN
int pw13_list_remove_all (Pw13_List **L, void *E);


/** Push (insert) an element at the beginning of the list */
PW13_EXTERN
int pw13_list_push (Pw13_List **L, void *E);

/** Returns the first element after having removed it. */
PW13_EXTERN
void * pw13_list_pop (Pw13_List **L);


/** Free all nodes of a list (without freeing elements !) */
PW13_EXTERN
void pw13_list_free (Pw13_List *L);

/** Free all nodes calling a destructor for each element */
PW13_EXTERN
void pw13_list_destroy (Pw13_List *L, void (*destructor) (void*));


/** Returns the number of nodes in a list */
PW13_EXTERN
int pw13_list_length (Pw13_List *L);


#endif
/* ndef PW13_LIST_H_INCLUDED */
