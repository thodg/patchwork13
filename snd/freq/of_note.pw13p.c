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

  of_notes.pw13p.c  -  notes to frequencies conversion patch

*/
#include <math.h>
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>
#include <snd/note.pw13dt.h>


static float freq_of_note (int note, float gamme)
{
  return exp (note / gamme * M_LN2);
}


static void patch_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources (p, time);
  float gamme = pw13_float_input_val (p->input+1);
  float tune = pw13_float_input_val (p->input+2);
  Pw13_Data *di = pw13_input_val (p->input);
  unsigned int n = di->nblocks;
  if (p->output->data->nblocks != n) {
    pw13_data_free (p->output->data);
    p->output->data = pw13_data_array (n);
  }
  while (n) {
    p->output->data[n].f = freq_of_note (di[n].i, gamme) + tune;
    n--;
  }
  time = time;
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 3, 1);
  pw13_notes_input ("note", p->input, p);
  pw13_float_input ("gamme", p->input+1, p, 12);
  pw13_float_input ("tune", p->input+2, p, 0);
  pw13_float_output ("freq", p->output, p);
  p->pump = patch_pump;
  param = param;
  return 1;
}
