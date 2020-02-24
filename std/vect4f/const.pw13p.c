/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13_std - the patchwork13 standard patches library
  Copyright (C) 2005 Bruno Malaquin <broen.42@gmail.com>
  
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

  const.pw13p.c  -  vecteur of 4, constant patch

*/
#include <pw13/pw13.h>
#include <std/vect4f.pw13dt.h>


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 0, 1);
  pw13_vect4f_output ("k", p->output, p);
  pw13_vect4f_output_x ( p->output[0] ) = 0;
  pw13_vect4f_output_y ( p->output[0] ) = 0;
  pw13_vect4f_output_z ( p->output[0] ) = 0;
  pw13_vect4f_output_t ( p->output[0] ) = 0;
  
  return 1;
}

