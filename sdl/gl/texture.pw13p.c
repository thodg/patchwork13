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

  texture.pw13p.c  -  SDL  opengl texturing patch

*/
#include <pw13/pw13.h>
#include <std/vect4f.pw13dt.h>
#include <std/text.pw13dt.h>

#include <stdio.h>
#include <stdlib.h>
#include <SDL.h>
#include <SDL_opengl.h>

GLuint texture[1];

void  pw13_sdl_LoadGLTextures(Pw13_Patch *p)
{
  p = p;
  SDL_Surface * TextureImage[1];
  /*
  GLuint texture;
  */

  if ( ( TextureImage[0] = SDL_LoadBMP( "/goinfre/malaqu_b/patchwork13/trunk/pw13_sdl/gl/nehe.bmp" ) ) ){

    glGenTextures( 1, &texture[0] );
    glBindTexture( GL_TEXTURE_2D, texture[0] );

    glTexImage2D( GL_TEXTURE_2D, 0, 3, TextureImage[0]->w,
		  TextureImage[0]->h, 0, GL_BGR,
		  GL_UNSIGNED_BYTE, TextureImage[0]->pixels );

    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
  } else {
    printf("ERROR SDL_LOADBMP \n");
  }

  if ( TextureImage[0] ){
    SDL_FreeSurface( TextureImage[0] );
  }

  /*
  return texture;
  */
}

static void pw13_sdl_gl_texture_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources(p, time);
  p=p;
  time=time;

  glBindTexture( GL_TEXTURE_2D, texture[0]);


  glBegin(GL_QUADS);

  glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, 1.0f );
  glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, 1.0f );
  glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f,  1.0f, 1.0f );
  glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f,  1.0f, 1.0f );

  glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
  glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );
  glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f,  1.0f, -1.0f );
  glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f, -1.0f, -1.0f );

  glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );
  glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -1.0f,  1.0f,  1.0f );
  glTexCoord2f( 0.0f, 0.0f ); glVertex3f(  1.0f,  1.0f,  1.0f );
  glTexCoord2f( 0.0f, 1.0f ); glVertex3f(  1.0f,  1.0f, -1.0f );

  glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
  glTexCoord2f( 1.0f, 1.0f ); glVertex3f(  1.0f, -1.0f, -1.0f );
  glTexCoord2f( 1.0f, 0.0f ); glVertex3f(  1.0f, -1.0f,  1.0f );
  glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f,  1.0f );

  glTexCoord2f( 0.0f, 0.0f ); glVertex3f( 1.0f, -1.0f, -1.0f );
  glTexCoord2f( 0.0f, 1.0f ); glVertex3f( 1.0f,  1.0f, -1.0f );
  glTexCoord2f( 1.0f, 1.0f ); glVertex3f( 1.0f,  1.0f,  1.0f );
  glTexCoord2f( 1.0f, 0.0f ); glVertex3f( 1.0f, -1.0f,  1.0f );

  glTexCoord2f( 1.0f, 0.0f ); glVertex3f( -1.0f, -1.0f, -1.0f );
  glTexCoord2f( 0.0f, 0.0f ); glVertex3f( -1.0f, -1.0f,  1.0f );
  glTexCoord2f( 0.0f, 1.0f ); glVertex3f( -1.0f,  1.0f,  1.0f );
  glTexCoord2f( 1.0f, 1.0f ); glVertex3f( -1.0f,  1.0f, -1.0f );

  glEnd( );



  /*
  glTexGenfv( GL_S, GL_OBJECT_PLANE, pw13_vect4f_input_val(p->input+2) );
  glTexGenfv( GL_T, GL_OBJECT_PLANE, pw13_vect4f_input_val(p->input+3) );
  */
}

static int pw13_sdl_gl_texture_start (Pw13_Patch *p, Pw13_Time *time)
{
  p=p;
  time=time;

  /*
  p->param = (void*) pw13_sdl_LoadGLTextures( p );
  */

  return 1;
}

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 4, 1);
  pw13_void_input ("()", p->input, p);
  pw13_text_input ("name of texture", p->input+1, p, "nehe.bmp");
  pw13_vect4f_input ("S", p->input+2, p, pw13_vect4f(0,1,0,1) );
  pw13_vect4f_input ("T", p->input+3, p, pw13_vect4f(1,0,0,1) );
  pw13_void_output ("()", p->output, p);
  p->pump = pw13_sdl_gl_texture_pump;
  p->start = pw13_sdl_gl_texture_start;
  return 1;
}
