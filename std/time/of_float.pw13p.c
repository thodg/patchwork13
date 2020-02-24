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

  of_float.pw13p.c  -  float to time conversion patch

*/
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>


static void pw13_std_time_of_float_pump (Pw13_Patch *p, Pw13_Time *time)
{
  Pw13_Time *out = pw13_time_output_val (p->output[0]);
  float f = pw13_float_input_val (p->input);
  pw13_pump_sources (p, time);
  out->seconds = (long) f;
  out->frac = f - (long) f;
  out->ticks++;
}

static int patch_start (Pw13_Patch *p, Pw13_Time *time)
{
  Pw13_Time *out = pw13_time_output_val (p->output[0]);
  out->seconds = 0;
  out->frac = 0;
  out->ticks = 0;
  time = time;
  return 1;
}

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 1, 1);
  pw13_float_input ("f", p->input, p, 0);
  pw13_time_output ("t", p->output, p);
  p->pump = pw13_std_time_of_float_pump;
  p->start = patch_start;
  return 1;
}
