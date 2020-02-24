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

  vbo_cube.pw13p.c  -  SDL  opengl vertex array pyramid patch

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>
#include "va.pw13dt.h"

float vertexData[] = {  1.0,  1.0, -1.0,
			-1.0,  1.0, -1.0,
			-1.0,  1.0,  1.0,
			1.0,  1.0,  1.0,

			1.0, -1.0,  1.0,
			-1.0, -1.0,  1.0,
			-1.0, -1.0, -1.0,
			1.0, -1.0, -1.0,

			1.0,  1.0,  1.0,
			-1.0,  1.0,  1.0,
			-1.0, -1.0,  1.0,
			1.0, -1.0,  1.0,

			1.0, -1.0, -1.0,
			-1.0, -1.0, -1.0,
			-1.0,  1.0, -1.0,
			1.0,  1.0, -1.0,

			-1.0,  1.0,  1.0,
			-1.0,  1.0, -1.0,
			-1.0, -1.0, -1.0,
			-1.0, -1.0,  1.0,

			1.0,  1.0, -1.0,
			1.0,  1.0,  1.0,
			1.0, -1.0,  1.0,
			1.0, -1.0, -1.0};


float colorData[] = {  0.0, 1.0, 0.0,
		       0.0, 1.0, 0.0,
		       0.0, 1.0, 0.0,
		       0.0, 1.0, 0.0,

		       1.0, 0.5, 0.0,
		       1.0, 0.5, 0.0,
		       1.0, 0.5, 0.0,
		       1.0, 0.5, 0.0,

		       1.0, 0.0, 0.0,
		       1.0, 0.0, 0.0,
		       1.0, 0.0, 0.0,
		       1.0, 0.0, 0.0,

		       1.0, 1.0, 0.0,
		       1.0, 1.0, 0.0,
		       1.0, 1.0, 0.0,
		       1.0, 1.0, 0.0,

		       0.0, 0.0, 1.0,
		       0.0, 0.0, 1.0,
		       0.0, 0.0, 1.0,
		       0.0, 0.0, 1.0,

		       1.0, 0.0, 1.0,
		       1.0, 0.0, 1.0,
		       1.0, 0.0, 1.0,
		       1.0, 0.0, 1.0};


static void pw13_sdl_va_cube_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources (p, time);
  p=p;
  time=time;

  memcpy(p->output->data+1, vertexData, sizeof(vertexData));
  memcpy((p->output+1)->data+1, colorData, sizeof(colorData));

}


static int pw13_sdl_va_cube_start (Pw13_Patch *p, Pw13_Time *time)
{
  p = p;
  time = time;

  p->output->data = pw13_data_array (sizeof(vertexData)/sizeof(float));
  (p->output+1)->data = pw13_data_array (sizeof(colorData)/sizeof(float));

  return 1;
}


static void pw13_sdl_va_cube_stop (Pw13_Patch *p)
{
  pw13_data_free (p->output->data);
  pw13_data_free ((p->output+1)->data);
}

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 1, 2);
  pw13_void_input ("()", p->input, p);
  pw13_va_output ("vertex", p->output, p);
  pw13_va_output ("color", p->output+1, p);
  p->pump = pw13_sdl_va_cube_pump;
  p->start = pw13_sdl_va_cube_start;
  p->stop = pw13_sdl_va_cube_stop;
  return 1;
}
