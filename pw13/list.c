/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13 - the patchwork13 standard patches library
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

  list.c  -  simple linked list and operators

*/
#include <stdlib.h> /* malloc */
#include "error.h"
#include "list.h"


/* allocate and initialize a new list */
Pw13_List * pw13_list (void *E, Pw13_List *L)
{
  Pw13_List *n = (Pw13_List*) malloc (sizeof (Pw13_List));
  if (n) {
    n->elem = E;
    n->suiv = L;
  }
  return n;
}

/* insert an element at the beginning of the list */
int pw13_list_insert_head (Pw13_List **L, void *E)
{
  Pw13_List *n;
  if ( !L || !(n = pw13_list (E, *L)) )
    return 0;
  *L = n;
  return 1;
}

/* insert an element at the end of the list (slow) */
int pw13_list_insert_tail (Pw13_List **L, void *E)
{
  Pw13_List *n;
  if ( !L || !(n = pw13_list (E, NULL)) )
    return 0;
  while (*L)
    L = &(*L)->suiv;
  *L = n;
  return 1;
}

/* insert an element in a sorted list */
int pw13_list_insert_sorted (Pw13_List **L, void *E,
			     int (*compare) (void*,void*))
{
  Pw13_List *n;
  if ( !L || !(n = pw13_list (E, NULL)) )
    return 0;
  while (*L && (compare((*L)->elem, E) < 0))
    L = &(*L)->suiv;
  n->suiv = *L;
  *L = n;
  return 1;
}


/* find the list beginning with an element */
Pw13_List *
pw13_list_find (Pw13_List *L, void *E)
{
  while (L && (L->elem != E))
    L = L->suiv;
  return L;
}

/* find the link to the list beginning with an element */
Pw13_List **
pw13_list_find_p (Pw13_List **L, void *E)
{
  if (!L)
    return NULL;
  while (*L && ((*L)->elem != E))
    L = &(*L)->suiv;
  return L;
}


/* remove the first element of the list */
Pw13_Result
pw13_list_remove_first (Pw13_List **L)
{
  Pw13_List *tmp;
  if (!L || !(*L))
    return PW13_ERROR;
  tmp = *L;
  *L = (*L)->suiv;
  free (tmp);
  return PW13_SUCCESS;
}

/* remove the first occurence of an element in the list */
Pw13_Result
pw13_list_remove_one (Pw13_List **L, void *E)
{
  Pw13_List **one = pw13_list_find_p (L, E);
  if (!one || !(*one))
    return PW13_ERROR;
  return pw13_list_remove_first (one);
}

/* remove all the occurences of an element in the list */
int
pw13_list_remove_all (Pw13_List **L, void *E)
{
  int nb = 0;
  Pw13_List **one = L;
  while ((one = pw13_list_find_p (one, E)) && (*one))
    nb += pw13_list_remove_first (one);
  return nb;
}


/* free all nodes of a list (without freeing elements !) */
void
pw13_list_free (Pw13_List *L)
{
  while (L) {
    Pw13_List *tmp = L;
    L = L->suiv;
    free (tmp);
  }
}

/* free all nodes calling a destructor for each element */
void
pw13_list_destroy (Pw13_List *L, void (*destructor) (void*))
{
  while (L) {
    Pw13_List *tmp = L;
    destructor (L->elem);
    L = L->suiv;
    free (tmp);
  }
}


int pw13_list_push (Pw13_List **L, void *E)
{
  return pw13_list_insert_head (L, E);
}


void * pw13_list_pop (Pw13_List **L)
{
  void *v;
  if ( ! *L )
    return NULL;
  v = (*L)->elem;
  pw13_list_remove_first (L);
  return v;
}
