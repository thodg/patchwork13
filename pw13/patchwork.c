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

  patchwork.c  -  graph of patches, registers data types and patch classes

*/
#include <stdlib.h> /* malloc */
#include <stdarg.h>
#include <string.h>
#include "patch.h"
#include "patchwork.h"
#include "queue.h"
#include "time.h"
#include "error.h"
#include "master_patch.h"
#include "method.h"

#define MAX(a,b) ((a)>(b) ? (a) : (b))

/* Initialize an allocated patchwork. */
void
pw13_patchwork_init (Pw13_Patchwork *pw)
{
  pw->nb_patches = 0;
  pw->patches = NULL;
  pw->time.ticks = 0;
  pw->time.seconds = 0;
  pw->time.frac = 0;
  pw->method_names = NULL;
  pw->master = pw13_new_master_patch (pw);
}


/* Destroy a patchwork (without freeing it). */
void
pw13_patchwork_destroy (Pw13_Patchwork *pw)
{
  if (pw) {
    if (pw->master->started)
      pw13_patchwork_stop (pw);
    pw13_patch_destroy (pw->master);
    pw13_list_free (pw->patches);
  }
}


/* add an existing patch to the patchwork */
void
pw13_patchwork_add_patch (Pw13_Patchwork *pw, Pw13_Patch *patch)
{
  patch->pw = pw;
  if (pw->master->started)
    pw13_patch_start (patch, &pw->time);
  pw->patches = pw13_list (patch, pw->patches);
  pw->nb_patches++;
  pw13_patchwork_method_call (pw, "add patch", patch);
}

/* remove a patch from the patchwork */
int
pw13_patchwork_remove_patch (Pw13_Patchwork *pw, Pw13_Patch *p)
{
  Pw13_Result	res;

  if (!pw || !p)
    return 0;
  if (p->started)
    pw13_patch_stop (p);
  res = pw13_list_remove_one (&pw->patches, (void*) p);
  if (res == PW13_SUCCESS)
    pw->nb_patches--;
  return (res);
}

/* start all the patches */
Pw13_Result
pw13_patchwork_start (Pw13_Patchwork *pw, Pw13_Time *time)
{
  Pw13_List *L;
  int res = 1;
  if (!pw)
    return pw13_error ("pw13_patchwork_start: NULL patchwork");
  pw->time = *time;
  pw13_patchwork_topo_sort (pw);
  L = pw->patches;
  while (L && res) {
    Pw13_Patch *p = (Pw13_Patch*) L->elem;
    res = pw13_patch_start (p, time);
    L = L->suiv;
  }
  if (res)
    res = pw13_patch_start (pw->master, time);
  return res ? PW13_SUCCESS : PW13_FAILURE;
}

/* pump data through all the patches */
void
pw13_patchwork_pump (Pw13_Patchwork *pw)
{
  if (PW13_DEBUG) {
    printf ("pw13_pw_pump: pre pump time=%li,%99f[%lu]\n",
	    pw->time.seconds, pw->time.frac, pw->time.ticks);
    pw13_msleep (10);
  }
  pw13_patch_force_pump (pw->master, &pw->time);
}

/* stop all the patches */
void
pw13_patchwork_stop (Pw13_Patchwork *pw)
{
  Pw13_List *L;
  if (!pw)
    return;
  L = pw->patches;
  while (L) {
    Pw13_Patch *p = (Pw13_Patch*) L->elem;
    pw13_patch_stop (p);
    L = L->suiv;
  }
  pw13_patch_stop (pw->master);
}


void pw13_patchwork_topo_sort (Pw13_Patchwork *pw)
{
  Pw13_List	*l;
  Pw13_List	*stack = NULL;
  Pw13_List	*result = NULL;
  Pw13_Patch	*p;
  Pw13_Input	*succ;
  unsigned int	i;

  for (l = pw->patches; l != NULL; l = l->suiv)
  {
    p = (Pw13_Patch *) l->elem;
    if (!(p->topo = pw13_nb_connected_inputs(p)))
      pw13_list_push(&stack, p);
  }
  while (stack)
  {
    p = pw13_list_pop(&stack);
    pw13_list_insert_tail(&result, p);
    for (i = 0; i < p->nb_outputs; i++)
    {
      for (l = p->output[i].connected_to; l != NULL; l = l->suiv)
      {
	succ = (Pw13_Input *) l->elem;
	if (!(--succ->patch->topo))
	  pw13_list_push(&stack, succ->patch);
      }
    }
  }
  l = pw->patches;
  pw->patches = result;
  pw13_list_free(l);
}


/* methods */

Pw13_Method * pw13_patchwork_method_add (Pw13_Patchwork *pw, const char *name,
					 Pw13_MethodFunc *f, void *method_param)
{
  return pw13_method_add (&pw->method_names, name, f, pw, method_param);
}


Pw13_MethodName * pw13_patchwork_method_find (Pw13_Patchwork *pw,
					      const char *name)
{
  return * pw13_method_name_find (&pw->method_names, name);
}


int pw13_patchwork_method_call (Pw13_Patchwork *pw, const char *name,
				void *call_param)
{
  int count = 0;
  Pw13_MethodName *n = * pw13_method_name_find (&pw->method_names, name);
  if (n) {
    Pw13_Method *method = n->methods;
    while (method) {
      method->func (method, call_param);
      method = method->next;
      count++;
    }
  }
  return count;
}
