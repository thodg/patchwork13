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

  output.h  -  data output

*/
#ifndef PW13_OUTPUT_H_INCLUDED
#define PW13_OUTPUT_H_INCLUDED 1

#include "dll.h"
#include "types.h"
#include "patch.h"
#include "data.h"
#include "data_type.h"
#include "list.h"


struct Pw13_Output {
  Pw13_Patch	*patch;
  const char	*name;
  Pw13_DataType	*data_type;
  Pw13_Data	*data;
  Pw13_List	*connected_to;
  void		*custom_data;
};


/* initialize an allocated output */
PW13_EXTERN
void pw13_output_init (const char *name, Pw13_Output *output,
		       Pw13_Patch *patch, Pw13_DataType *data_type,
		       Pw13_Data *data);

/* destroy the contents of an output */
PW13_EXTERN
void pw13_output_destroy (Pw13_Output *o);


PW13_EXTERN
void pw13_output_print_infos (Pw13_Output *o);


#endif
/* ndef PW13_OUTPUT_H_INCLUDED */
