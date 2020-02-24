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

  output.c  -  data output

*/

#include <stdlib.h> /* malloc */
#include "patch.h"
#include "output.h"


/* initialize an allocated output */
void
pw13_output_init (const char *name, Pw13_Output *output,
		  Pw13_Patch *patch, Pw13_DataType *data_type,
		  Pw13_Data *data)
{
  output->name = name;
  output->patch = patch;
  output->data_type = data_type;
  output->connected_to = NULL;
  output->data = data;
  output->custom_data = NULL;
}


/* destroy the contents of an output */
void pw13_output_destroy (Pw13_Output *o)
{
  pw13_list_free (o->connected_to);
}


void
pw13_output_print_infos (Pw13_Output *o)
{
  printf ("output %p\n", o);
  if (o) {
    printf ("  name   \"%s\"\n"
	    "  connected_to %p\n"
	    "  custom data  %p\n",
	    o->name,
	    o->connected_to,
	    o->custom_data);
  }
  printf ("/output\n\n");
}
