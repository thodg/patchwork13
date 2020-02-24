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

  master_patch.c  -  Master patch for a patchwork, with a time input.

*/
#include <stdlib.h>
#include "patch.h"
#include "patchwork.h"
#include "time.h"
#include "any.h"
#include "master_patch.h"


static void pw13_master_patch_pump (Pw13_Patch *p, Pw13_Time *t)
{
  Pw13_Time	*next_time;

  pw13_force_pump_input (p->input, t);
  pw13_force_pump_input (p->input+1, t);

  next_time = pw13_time_input_val(p->input);
  *t = *next_time;
}

/*
static int pw13_master_patch_start (Pw13_Patch *p, Pw13_Time *time)
{
  p = p;
  time = time;
  return 1;
}
*/

Pw13_Patch * pw13_new_master_patch ()
{
  Pw13_Patch *p = malloc (sizeof (Pw13_Patch));
  if (!p)
    exit (-1);
  pw13_patch_init("Master", p, 2, 0);
  pw13_time_input("next time", p->input, p,
		   pw13_time_data (NULL));
  pw13_any_input("'a", p->input + 1, p);
  p->patch_class = NULL;
  p->pump = pw13_master_patch_pump;
  /*  p->start = pw13_master_patch_start; */
  return p;
}
