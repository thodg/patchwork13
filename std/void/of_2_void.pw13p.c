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

  of_2_void.pw13p.c  -  connect 2 void into 1

*/
#include <pw13/pw13.h>
#include <stdio.h>
#include <stdlib.h>


static void patch_pump(Pw13_Patch *p, Pw13_Time *t)
{
  pw13_pump_sources(p, t);
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 2, 1);
  pw13_void_input ("()", p->input, p);
  pw13_void_input ("()", p->input+1, p);
  pw13_void_output ("()", p->output, p);
  p->pump = patch_pump;
  return 1;
}
