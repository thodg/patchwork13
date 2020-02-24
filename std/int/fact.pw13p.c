/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13_std - the patchwork13 standard patches library
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

  fact.pw13p.c  -  int factorial patch

*/
#include <pw13/pw13.h>
#include <std/int.pw13dt.h>


#define PW13_FACT_MAX_N 12

int pw13_std_int_fact_values[PW13_FACT_MAX_N+1] = {
/*0  1  2  3   4    5    6     7      8       9  */
  1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880,
  /*   10        11         12  */
  3628800, 39916800, 479001600
};


static void patch_pump (Pw13_Patch *p, Pw13_Time *time)
{
  int n;
  pw13_pump_sources (p, time);

  n = pw13_int_input_val(p->input);
  if (0<n && n<=PW13_FACT_MAX_N)
    pw13_int_output_val (p->output[0]) = pw13_std_int_fact_values[n];
  else
    pw13_int_output_val (p->output[0]) = 1;
}

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 1, 1);
  pw13_int_input ("n", p->input, p, 0);
  pw13_int_output ("n!", p->output, p);
  p->pump = patch_pump;
  param = param;
  return 1;
}

