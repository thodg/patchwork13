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

  clearbuffer.pw13p.c  -  SDL opengl clear buffer color and depth patch

*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <pw13/pw13.h>

static void pw13_sdl_clearbuffer_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources (p, time);

  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 1, 1);
  pw13_void_input ("()", p->input, p);
  pw13_void_output ("()", p->output, p);
  p->pump = pw13_sdl_clearbuffer_pump;
  return 1;
}
