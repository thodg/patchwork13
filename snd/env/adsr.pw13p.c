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

  osc.pw13-patch.c  -  oscillator patch

*/
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>


static void	pw13_sound_enveloppe_adsr_pump(Pw13_Patch	*p,
					       Pw13_Time	*time)
{
  float		a;
  float		d;
  float		s;
  float		r;
  float		ta;
  float		td;
  float		ts;
  float		tr;
  float		level;

  pw13_pump_sources(p, time);

  ta = time->seconds + time->frac - (a = pw13_float_input_val(p->input + 0));
  td = ta - (d = pw13_float_input_val(p->input + 1));
  ts = td - (s = pw13_float_input_val(p->input + 2));
  level = pw13_float_input_val(p->input + 3);
  tr = ts - (r = pw13_float_input_val(p->input + 4));

  if (time->seconds < 0 || time->frac < 0 || tr > 0)
    pw13_float_output_val(p->output[0]) = 0;
  else if (ts > 0)
    pw13_float_output_val(p->output[0]) = (1 - ts / r) * level;
  else if (td > 0)
    pw13_float_output_val(p->output[0]) = level;
  else if (ta > 0)
    pw13_float_output_val(p->output[0]) = 1 - ta / d;
  else
    pw13_float_output_val(p->output[0]) = ta / d;
}


PW13_EXPORT
Pw13_Result pw13_dl_patch_class_init(const char		*name,
				     Pw13_Patch		*p,
				     void		*param)
{
  pw13_patch_init (name, p, 5, 1);

  pw13_float_input("attack", p->input, p, 0.05);
  pw13_float_input("decay", p->input + 1, p, 0.1);
  pw13_float_input("sustain", p->input + 2, p, 1);
  pw13_float_input("level", p->input + 3, p, 0.6);
  pw13_float_input("release", p->input + 4, p, 0.5);

  pw13_float_output("enveloppe", p->output, p);

  p->pump = pw13_sound_enveloppe_adsr_pump;

  param = param;

  return (PW13_SUCCESS);
}
