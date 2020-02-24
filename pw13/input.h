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

  input.h  -  data input

*/

#ifndef PW13_INPUT_H_INCLUDED
#define PW13_INPUT_H_INCLUDED

#include "dll.h"
#include "types.h"
#include "patch.h"
#include "data.h"
#include "output.h"
#include "error.h"

struct Pw13_Input {
  Pw13_Patch *patch;
  const char *name;
  Pw13_DataType *data_type;
  Pw13_Output *from;
  Pw13_Data *defval;
  void *custom_data;
};

/** initialize an allocated input */
PW13_EXTERN
void pw13_input_init (const char *name, Pw13_Input *input,
		      Pw13_Patch *patch, Pw13_DataType *data_type,
		      Pw13_Data *defval);

/** Destroy an input (without freeing it) */
PW13_EXTERN
void pw13_input_destroy (Pw13_Input *input);

/** Return the value of an input */
PW13_EXTERN
Pw13_Data *pw13_input_val (Pw13_Input *i);

/** Checks whether it is possible to connect a given output to an input */
PW13_EXTERN
int pw13_can_connect (Pw13_Output *from, Pw13_Input *to);

/** Connect an output to an input */
PW13_EXTERN
Pw13_Result pw13_connect (Pw13_Output *from, Pw13_Input *to);

/** Disconnect an input */
PW13_EXTERN
Pw13_Result pw13_disconnect (Pw13_Input *to);

/** Pump an input if it is connected */
PW13_EXTERN
int pw13_pump_input (Pw13_Input *i, Pw13_Time *t);

PW13_EXTERN
int pw13_force_pump_input (Pw13_Input *i, Pw13_Time *t);

PW13_EXTERN
void pw13_input_print_infos (Pw13_Input *i);


#endif
/* not PW13_INPUT_H_INCLUDED */
