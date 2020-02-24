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

  input.c  -  data input

*/
#include <stdlib.h> /* malloc */
#include "patch.h"
#include "output.h"
#include "input.h"
#include "any.h"
#include "error.h"


/* initialize an allocated input */
void
pw13_input_init (const char *name, Pw13_Input *input, Pw13_Patch *patch,
		 Pw13_DataType *data_type, Pw13_Data *defval)
{
  input->name = name;
  input->patch = patch;
  input->data_type = data_type;
  input->from = NULL;
  input->defval = defval;
  input->custom_data = NULL;
}

/* destruct an input, without freeing it */
void
pw13_input_destroy (Pw13_Input *input)
{
  if (input && input->from)
    pw13_disconnect (input);
}

/* return the value of an input */
Pw13_Data * pw13_input_val (Pw13_Input *i)
{
  if (i->from)
    return i->from->data;
  return i->defval;
}


int pw13_can_connect (Pw13_Output *from, Pw13_Input *to)
{
  if (!from) {
    pw13_error ("pw13_connect: NULL output");
    return 0;
  }
  if (!to) {
    pw13_error ("pw13_connect: NULL input");
    return 0;
  }
  if (from->data_type != to->data_type
      && from->data_type->name[0] != '\''
      && to->data_type->name[0] != '\'') {
    pw13_error ("pw13_connect: incompatible data types");
    return 0;
  }
  if (to->from == from) {
    pw13_error ("pw13_connect: already connected");
    return 0;
  }
  return 1;
}

/* connect an output to an input */
Pw13_Result
pw13_connect (Pw13_Output *from, Pw13_Input *to)
{
  Pw13_Output *old_from;
  if ((old_from = to->from))
    pw13_list_remove_one (&old_from->connected_to, to);
  to->from = from;
  from->connected_to = pw13_list (to, from->connected_to);
  return PW13_SUCCESS;
}


/* disconnect an input */
Pw13_Result
pw13_disconnect (Pw13_Input *B)
{
  Pw13_Output *A;
  if (!B)
    return pw13_error ("pw13_disconnect: NULL input");
  A = B->from;
  if (pw13_list_remove_one (& A->connected_to, (void*) B) != PW13_SUCCESS)
    pw13_fatal_error ("pw13_disconnect: list_remove_one");
  B->from = NULL;

  return PW13_SUCCESS;
}


/* pump an input if it is connected */
int pw13_pump_input(Pw13_Input *i, Pw13_Time *t)
{
  if (i->from) {
    pw13_patch_pump(i->from->patch, t);
    return 1;
  }
  return 0;
}


/* force pump an input if it is connected */
int pw13_force_pump_input(Pw13_Input *i, Pw13_Time *t)
{
  if (i->from) {
    pw13_patch_force_pump(i->from->patch, t);
    return 1;
  }
  return 0;
}


void
pw13_input_print_infos (Pw13_Input *i)
{
  printf ("input %p\n", i);
  if (i) {
    printf ("  name        \"%s\"\n"
	    "  from        %p\n"
	    "  custom data %p\n",
	    i->name,
	    i->from,
	    i->custom_data);
  }
  printf ("/input\n\n");
}
