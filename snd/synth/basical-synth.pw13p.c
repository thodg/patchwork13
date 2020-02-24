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
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, LIKE SEX OR DRUG USE.
  See the GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
*
  basical-synth.pw13p.c  -  A basical synthetiser
*
  patch_class_LDFLAGS = -lm
*/

#include <math.h>
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>
#include <stdio.h>

static void	pw13_sound_basical_synth_pump(Pw13_Patch	*p,
					      Pw13_Time		*time)
{
  unsigned int	i;
  float		res;
  Pw13_Data	*volumes;
  Pw13_Data	*freqs;

  pw13_pump_input(p->input + 1, time);
  freqs = pw13_input_val(p->input + 1);

  pw13_pump_input(p->input + 2, time);
  volumes = pw13_input_val(p->input + 2);

  res = 0;
  for (i = 0; i < freqs->nblocks; i++) {
    float	wave;
    Pw13_Time	t;
    int		v;

    t.seconds = 0;
    t.frac = time->frac * freqs[i + 1].f;
    t.frac -= (int) t.frac;
    t.ticks = time->ticks;
    pw13_pump_input(p->input, &t);
    wave = pw13_float_input_val(p->input);

    v = i % volumes->nblocks;
    res += (volumes[v + 1].f) * wave;
  }

  pw13_float_output_val (p->output[0]) = res;
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init(name, p, 3, 1);

  pw13_float_input("wave", p->input, p, 0);
  pw13_float_input("freqs", p->input + 1, p, 0);
  pw13_float_input("volumes", p->input + 2, p, 0);

  pw13_float_output("signal", p->output, p);

  p->pump = pw13_sound_basical_synth_pump;

  return 1;
}
