/*
  
  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13_std - the patchwork13 standard patches library
  Copyright (C) 2005 Malaquin Bruno <broen.42@gmail.com>
  
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

  add.pw13p.c  -  vecteur of 4, addition patch

*/
#include <pw13/pw13.h>
#include <std/vect4f.pw13dt.h>


static void pw13_std_vect4f_add_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources (p, time);
  
  pw13_vect4f_output_x (p->output[0]) =
    pw13_vect4f_input_x (p->input)
    + pw13_vect4f_input_x (p->input+1);
  
  pw13_vect4f_output_y (p->output[0]) =
    pw13_vect4f_input_y (p->input)
    + pw13_vect4f_input_y (p->input+1);
  
  pw13_vect4f_output_z (p->output[0]) =
    pw13_vect4f_input_z (p->input)
    + pw13_vect4f_input_z (p->input+1);
  
  pw13_vect4f_output_t (p->output[0]) =
    pw13_vect4f_input_t (p->input)
    + pw13_vect4f_input_t (p->input+1);
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 2, 1);
  pw13_vect4f_input ("A", p->input, p, pw13_vect4f(0,0,0,0));
  pw13_vect4f_input ("B", p->input+1, p, pw13_vect4f(0,0,0,0));
  pw13_vect4f_output ("A+B", p->output, p);
  p->pump = pw13_std_vect4f_add_pump;
  return 1;
}

