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

  augend.pw13p.c  -  time augmentation patch

*/
#include <pw13/pw13.h>


static void patch_pump (Pw13_Patch *p, Pw13_Time *time)
{
  Pw13_Time *res = pw13_time_output_val (p->output[0]);
  Pw13_Time *dt = pw13_time_input_val (p->input);
  pw13_pump_sources (p, time);
  pw13_time_add (res, time, dt);
}


static int patch_start (Pw13_Patch *p, Pw13_Time *time)
{
  Pw13_Time *res = pw13_time_output_val (p->output[0]);
  Pw13_Time *dt = pw13_time_input_val (p->input);
  pw13_time_add (res, time, dt);
  return 1;
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 2, 1);
  pw13_time_input ("dt", p->input, p, pw13_time_data (NULL));
  pw13_any_input ("any", p->input+1, p);
  pw13_time_output ("t", p->output, p);
  p->pump = patch_pump;
  p->start = patch_start;
  param = param;
  {
    Pw13_Time *dt = pw13_time_input_val (p->input);
    dt->seconds = 0;
    dt->frac = 0.01;
    dt->ticks = 1;
  }
  return 1;
}
