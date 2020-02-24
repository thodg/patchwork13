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

  sdl_init.pw13p.c  -  SDL init opengl patch

*/
#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <pw13/pw13.h>

#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16

#define TRUE  1
#define FALSE 0

static int pw13_sdl_init_start (Pw13_Patch *p, Pw13_Time *time)
{
  p = p;
  time = time;

  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    fprintf( stderr, "Video initialization failed: %s\n", SDL_GetError( ) );
    SDL_Quit ();
  }
  return 1;
}

static void pw13_sdl_init_stop (Pw13_Patch *p)
{
  p = p;
  SDL_Quit ();
}

static void pw13_sdl_init_destroy (Pw13_Patch *p)
{
  p = p;
  SDL_Quit ();
}

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 1, 1);
  pw13_void_input ("()", p->input, p);
  pw13_void_output ("()", p->output, p);
  p->start = pw13_sdl_init_start;
  p->stop = pw13_sdl_init_stop;
  p->destroy = pw13_sdl_init_destroy;

  return 1;
}
