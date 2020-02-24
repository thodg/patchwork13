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

  surface.pw13p.c  -  SDL surface opengl patch

  inspired by the code of Jeff Molofee ( http://nehe.gamedev.net/ )

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

const SDL_VideoInfo *videoInfo;
int videoFlags;
int isActive = TRUE;
int done = FALSE;
SDL_Event event;

SDL_Surface *surface;



  /*////////////////
 / INITIALISATION /
////////////////*/

static int gl_resizeWindow( int width, int height )
{
  GLfloat ratio;

  if ( height == 0 )
    height = 1;

  ratio = ( GLfloat )width / ( GLfloat )height;
  glViewport( 0, 0, ( GLint )width, ( GLint )height );
  glMatrixMode( GL_PROJECTION );
  gluPerspective( 45.0f, ratio, 0.1f, 100.0f );
  glMatrixMode( GL_MODELVIEW );
  glLoadIdentity( );
  /*glEnable( GL_LIGHTING );*/
  return( TRUE );
}


static int initGL( GLvoid )
{
  glEnable(GL_TEXTURE_2D);
  glShadeModel( GL_SMOOTH );
  glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );
  glClearDepth( 1.0f );
  glEnable( GL_DEPTH_TEST );
  glDepthFunc( GL_LEQUAL );
  glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
  glBlendFunc( GL_SRC_ALPHA, GL_ONE );
  return( TRUE );
}


static int drawGLScene( GLvoid )
{
  glFinish();
  SDL_GL_SwapBuffers( );
  return( TRUE );
}

static int gl_main( void )
{
  videoInfo = SDL_GetVideoInfo( );
  if ( !videoInfo ) {
    fprintf( stderr, "Video query failed: %s\n",
	     SDL_GetError( ) );
    SDL_Quit( );
  }

  videoFlags  = SDL_OPENGL;
  videoFlags |= SDL_GL_DOUBLEBUFFER;
  videoFlags |= SDL_HWPALETTE;
  videoFlags |= SDL_RESIZABLE;
  videoFlags |= SDL_HWSURFACE;
  videoFlags |= SDL_HWACCEL;
  videoFlags |= SDL_ANYFORMAT;
  /*
  if ( videoInfo->hw_available )
    videoFlags |= SDL_HWSURFACE;
  else
    videoFlags |= SDL_SWSURFACE;

  if ( videoInfo->blit_hw )
    videoFlags |= SDL_HWACCEL;
  */
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

  surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, videoFlags );

  if ( !surface ) {
    fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
    SDL_Quit( );
  }

  printf ("Vendor : %s\n"
	  "Renderer : %s\n"
	  "Version : %s\n"
	  "Extensions : %s\n",
	  glGetString (GL_VENDOR),
	  glGetString (GL_RENDERER),
	  glGetString (GL_VERSION),
	  glGetString (GL_EXTENSIONS));

  if ( initGL( ) == FALSE ) {
    fprintf( stderr, "Could not initialize OpenGL.\n" );
    SDL_Quit( );
  }
  gl_resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );
  return( 0 );
}


  /*////////////////////
 / FIN INITIALISATION /
////////////////////*/


static void pw13_sdl_gl_surface_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources (p, time);
  drawGLScene( );
}


static int pw13_sdl_gl_surface_start (Pw13_Patch *p, Pw13_Time *time)
{
  p=p;
  time=time;
  gl_main();
  return 1;
}

static void pw13_sdl_gl_surface_stop (Pw13_Patch *p)
{
  p=p;
  SDL_Quit ();
}

static void pw13_sdl_gl_surface_destroy (Pw13_Patch *p)
{
  p=p;
  SDL_Quit ();
}

PW13_EXPORT
int pw13_dl_patch_class_init(const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 1, 1);
  pw13_void_input ("()", p->input, p);
  pw13_void_output ("()", p->output, p);
  p->pump = pw13_sdl_gl_surface_pump;
  p->start = pw13_sdl_gl_surface_start;
  p->stop = pw13_sdl_gl_surface_stop;
  p->destroy = pw13_sdl_gl_surface_destroy;

  return 1;
}
