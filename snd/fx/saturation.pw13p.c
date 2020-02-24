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

  saturation.pw13p.c  -  Generates saturation on samples

*/

#include <pw13/pw13.h>
#include <std/float.pw13dt.h>

#define	FLT_MIN	1.1754943508222875079688E-38F
#define	FLT_MAX	3.4028234663852885981170E+38F

static void pw13_saturation_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources (p, time);
  float res = pw13_float_input_val (p->input);
  
  if (res > pw13_float_input_val (p->input+1))
    res = pw13_float_input_val (p->input+1);
  else if (res < pw13_float_input_val (p->input+2))
    res= pw13_float_input_val (p->input+2);

  pw13_float_output_val (p->output[0]) = res;
}

PW13_EXPORT
int pw13_dl_patch_class_init(const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init(name, p, 3, 1);
  pw13_float_input("Sample Input", p->input, p, 0);
  pw13_float_input("Max", p->input+1, p, FLT_MAX);
  pw13_float_input("Min", p->input+2, p, (-FLT_MAX));
  pw13_float_output("Sample Output", p->output, p);
  p->pump = pw13_saturation_pump;
  return 1;
}
