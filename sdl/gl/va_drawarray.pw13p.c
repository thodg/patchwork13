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

  vbo_cube.pw13p.c  -  SDL  opengl vertex array drawarray vertex patch

*/
#include <pw13/pw13.h>
#include "va.pw13dt.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_opengl.h>

static void patch_pump (Pw13_Patch *p, Pw13_Time *time)
{
  Pw13_Data *i_vertex;
  Pw13_Data *i_color;

  pw13_pump_sources (p, time);

  i_vertex = pw13_input_val (p->input);
  i_color = pw13_input_val (p->input+1);

  glVertexPointer ( 3, GL_FLOAT,  0,  i_vertex+1 );
  glColorPointer ( 3, GL_FLOAT,  0, i_color+1 );

  glEnableClientState ( GL_VERTEX_ARRAY);
  glEnableClientState ( GL_COLOR_ARRAY);

  if( i_vertex->nblocks < i_color->nblocks ) {
    glDrawArrays ( GL_QUADS, 0, i_vertex->nblocks/3 );
  } else {
    glDrawArrays ( GL_QUADS, 0, i_color->nblocks/3 );
  }

  glDisableClientState ( GL_VERTEX_ARRAY);
  glDisableClientState ( GL_COLOR_ARRAY);

}

/*
static int patch_start (Pw13_Patch *p, Pw13_Time *time)
{
  p=p;
  time=time;

  return 1;
}


static void patch_stop (Pw13_Patch *p)
{
  p=p;

}
*/

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 2, 1);
  pw13_va_input ("vertex", p->input, p);
  pw13_va_input ("color", p->input+1, p);
  pw13_void_output ("()", p->output, p);
  p->pump = patch_pump;
  /*
  p->start = patch_start;
  p->stop = patch_stop;
  */
  return 1;
}
