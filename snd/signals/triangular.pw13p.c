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
  triangular.pw13p.c  -  Generates a triangular signal
*/

#include <math.h>
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>

static void pw13_triangular_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources (p, time);
  /*  
  float change = pw13_float_input_val (p->input);

  float coeff1 = ( 1 / (change) );
  float coeff2 = ( (-1) / (1 - change) );
  */
  /* FIXME
  if ( fmodf(time,1) < (pw13_float_input_val (p->input)) ) 
    pw13_float_output_val (p->output[0]) = coeff1* fmodf(time,1);
  else
    pw13_float_output_val (p->output[0]) = coeff2* fmodf(time,1) + (1-change*coeff2);
  */
}



PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init(name, p, 1, 1);
  pw13_float_input("Changing state (%)", p->input, p, 0.5);
  pw13_float_output("Signal value", p->output, p);
  p->pump = pw13_triangular_pump;
  return 1;
}
