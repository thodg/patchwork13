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

*

  patch_class_LDFLAGS = -lm

*/
#include <math.h>
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>


typedef struct	PatchParam
{
  double	x;
  Pw13_Time	t;
}		PatchParam;


static void pw13_std_float_osc_pump(Pw13_Patch *p, Pw13_Time *time)
{
  PatchParam *pp = (PatchParam*) p->param;
  pw13_pump_sources (p, time);

  pp->x += (time->frac - pp->t.frac + (time->seconds - pp->t.seconds))
    * pw13_float_input_val(p->input);      /* frequency */
  pp->x -= (int) pp->x;

  pw13_float_output_val (p->output[0]) =
    (sin(2.0 * M_PI * pp->x
	 + pw13_float_input_val(p->input+1))  /* phase */
     * pw13_float_input_val(p->input+2)     /* amplitude */
     + pw13_float_input_val(p->input+3));  /* offset */
  pp->t = *time;
}


static int pw13_std_float_osc_start(Pw13_Patch *p, Pw13_Time *time)
{
  PatchParam *pp = (PatchParam*) p->param;
  pp->x = 0;
  pp->t = *time;
  return 1;
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 4, 1);

  pw13_float_input("frequency", p->input, p, 220);
  pw13_float_input("phase", p->input+1, p, 0);
  pw13_float_input("amplitude", p->input+2, p, 1);
  pw13_float_input("offset", p->input+3, p, 0);

  pw13_float_output("osc", p->output, p);

  p->param = malloc(sizeof (PatchParam));
  p->pump = pw13_std_float_osc_pump;
  p->start = pw13_std_float_osc_start;

  param = param;
  return 1;
}
