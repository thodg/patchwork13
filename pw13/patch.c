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

  patch.c  -  polymorphic structure to generate outputs data
              from inputs data
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "patch.h"
#include "patchwork.h"
#include "error.h"
#include "str.h"
#include "method.h"


/* initialize a patch */
void
pw13_patch_init(const char	*name,
		Pw13_Patch	*p,
		unsigned int	nb_inputs,
		unsigned int	nb_outputs)
{
  p->pw = NULL;
  p->name = NULL;
  pw13_patch_set_name (p, name);
  p->nb_inputs = nb_inputs;
  p->input = (Pw13_Input*) malloc (nb_inputs * sizeof(Pw13_Input));
  p->nb_outputs = nb_outputs;
  p->output = (Pw13_Output*) malloc (nb_outputs * sizeof(Pw13_Output));
  p->started = 0;
  p->start = &pw13_patch_start_default;
  p->stop = &pw13_patch_stop_default;
  p->pump = &pw13_patch_pump_default;
  p->destroy = &pw13_patch_destroy_default;
  p->method_names = NULL;
  p->param = NULL;
  p->custom_data = NULL;
}


/* change the name of a patch */
void
pw13_patch_set_name (Pw13_Patch *p, const char *name)
{
  free ((void*) p->name);
  p->name = (const char *) pw13_strdup (name);
}

/* start a patch and its sources */
int pw13_patch_start (Pw13_Patch *p, Pw13_Time *t)
{
  printf ("starting `%s' (%x)\n", p->name, (unsigned) p);
  if (p->start (p, t) != PW13_SUCCESS)
    return pw13_error ("pw13_patch_start: p->start() failed");
  p->last_pump_tick = t->ticks;
  p->started = 1;
  printf ("started `%s' (%x)\n", p->name, (unsigned) p);
  return PW13_SUCCESS;
}

/* pump a patch data (compute the outputs data from the inputs data) */
void pw13_patch_force_pump (Pw13_Patch *p, Pw13_Time *t)
{
  if (PW13_DEBUG && p)
    printf ("pw13_patch_force_pump (`%s'): t=%li,%99f[%lu]"
	    " last_pump_tick=%lu\n",
	    p->name, t->seconds, t->frac, t->ticks, p->last_pump_tick);
  p->last_pump_tick = t->ticks;
  p->pump (p, t);
}


/* pump a patch data only if time changed */
int
pw13_patch_pump (Pw13_Patch *p, Pw13_Time *t)
{
  if (PW13_DEBUG && p)
    printf ("pw13_patch_pump (`%s'): t=%li,%f[%lu] last_pump_tick=%lu\n",
	    p->name, t->seconds, t->frac, t->ticks, p->last_pump_tick);
  if (t->ticks != p->last_pump_tick) {
    p->last_pump_tick = t->ticks; /* do not change order */
    p->pump (p, t);               /* it would loop if there is a cycle */
    return 1;
  }
  return 0;
}

/* Low level function to stop a patch. */
void pw13_patch_stop (Pw13_Patch *p)
{
  p->stop (p);
  p->started = 0;
}


void
pw13_patch_disconnect (Pw13_Patch *p)
{
  int n;
  n = p->nb_outputs;
  while (n--) {
    Pw13_Output *o = p->output + n;
    Pw13_List *L = o->connected_to;
    while (L) {
      Pw13_Input *i = (Pw13_Input*) L->elem;
      pw13_disconnect (i);
      L = L->suiv;
    }
  }
  n = p->nb_inputs;
  while (n--) {
    Pw13_Input *i = p->input + n;
    Pw13_Output *o = i->from;
    if (o)
      pw13_disconnect (i);
  }
}



/* Low level function to destroy a patch. */
void pw13_patch_destroy (Pw13_Patch *p)
{
  int i;
  pw13_patch_disconnect (p);
  if (p->pw) {
    pw13_patchwork_remove_patch (p->pw, p);
    p->pw = NULL;
  }
  if (p->started)
    p->stop (p);
  p->destroy (p);
  if(!p)
    return;
  free ((void*) p->name);
  i = p->nb_outputs;
  while (i--)
    pw13_output_destroy (&p->output[i]);
  i = p->nb_inputs;
  while (i--)
    pw13_input_destroy (&p->input[i]);
  free (p->input);
  free (p->output);
}


/* default patch methods */

int
pw13_patch_start_default (Pw13_Patch *p, Pw13_Time *time)
{
  p = p;
  time = time;
  return 1;  /* success */
}

void
pw13_patch_pump_default (Pw13_Patch *p, Pw13_Time *time)
{
  p = p;
  time = time;
}

void
pw13_patch_stop_default (Pw13_Patch *p)
{
  p = p;
}


void
pw13_patch_destroy_default (Pw13_Patch *p)
{
  p = p;
}


int pw13_pump_sources (Pw13_Patch *p, Pw13_Time *t)
{
  int pumped = 0;
  int i = p->nb_inputs;
  while (i--)
    if (pw13_pump_input(p->input + i, t) == PW13_SUCCESS)
      pumped++;
  return pumped;
}


int pw13_nb_connected_inputs (Pw13_Patch *p)
{
  int c = 0;
  int i = p->nb_inputs;
  while (i--)
    if (p->input[i].from)
      c++;
  return c;
}


void pw13_patch_print_infos (Pw13_Patch *p)
{
  printf ("patch %p\n", p);
  if (p) {
    unsigned int i;
    printf ("  name        %s\n"
	    "  nb inputs   %u\n"
	    "  nb outputs  %u\n"
	    "\n",
	    p->name,
	    p->nb_inputs,
	    p->nb_outputs);

    for (i=0; i<p->nb_inputs; i++)
      pw13_input_print_infos (p->input + i);

    for (i=0; i<p->nb_outputs; i++)
      pw13_output_print_infos (p->output + i);
  }
  printf ("/patch\n\n");
}


Pw13_Method * pw13_patch_method_add (Pw13_Patch *p, const char *name,
				     Pw13_MethodFunc *f, void *method_param)
{
  return pw13_method_add (&p->method_names, name, f, p, method_param);
}


Pw13_MethodName * pw13_patch_method_find (Pw13_Patch *p, const char *name)
{
  return * pw13_method_name_find (&p->method_names, name);
}


int pw13_patch_method_call (Pw13_Patch *p, const char *name,
			    void *call_param)
{
  int count = 0;
  Pw13_MethodName *n = * pw13_method_name_find (&p->method_names, name);
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
