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

  vbo_cube.pw13p.c  -  SDL  opengl vbo cube patch

*/
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_opengl.h>
/*#include "glext.h"*/

#define BUFFER_OFFSET(offset) ((char *)NULL + (offset))

GLAPI void APIENTRY glBindBufferARB (GLenum, GLuint);
GLAPI void APIENTRY glDeleteBuffersARB (GLsizei, const GLuint *);
GLAPI void APIENTRY glGenBuffersARB (GLsizei, GLuint *);
GLAPI void APIENTRY glBufferDataARB (GLenum, GLsizeiptrARB, const GLvoid *, GLenum);

int g_fVBOSupported = 1;

GLuint vertexBuffer, colorBuffer;

GLfloat vertexData[] = { 1.0, 1.0, -1.0,
			 -1.0, 1.0, -1.0,
			 -1.0, 1.0, 1.0,
			 1.0, 1.0, 1.0,

			 1.0, -1.0, 1.0,
			 -1.0, -1.0, 1.0,
			 -1.0, -1.0, -1.0,
			 1.0, -1.0, -1.0,

			 1.0, 1.0, 1.0,
			 -1.0, 1.0, 1.0,
			 -1.0, -1.0, 1.0,
			 1.0, -1.0, 1.0,

			 1.0, -1.0, -1.0,
			 -1.0, -1.0, -1.0,
			 -1.0, 1.0, -1.0,
			 1.0, 1.0, -1.0,

			 -1.0, 1.0, 1.0,
			 -1.0, 1.0, -1.0,
			 -1.0, -1.0, -1.0,
			 -1.0, -1.0, 1.0,

			 1.0, 1.0, -1.0,
			 1.0, 1.0, 1.0,
			 1.0, -1.0, 1.0,
			 1.0, -1.0, -1.0};


GLfloat colorData[] = {  0.0, 1.0, 0.0,
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


int IsExtensionSupported( char* szTargetExtension )
{
  const unsigned char *pszExtensions = NULL;
  const unsigned char *pszStart;
  unsigned char *pszWhere, *pszTerminator;

  pszWhere = (unsigned char *) strchr( szTargetExtension, ' ' );
  if( pszWhere || *szTargetExtension == '\0' )
    return 0;

  pszExtensions = glGetString( GL_EXTENSIONS );

  pszStart = pszExtensions;

  for(;;)
    {
      pszWhere = (unsigned char *) strstr( (const char *) pszStart, szTargetExtension );
      if( !pszWhere )
	break;
      pszTerminator = pszWhere + strlen( szTargetExtension );
      if( pszWhere == pszStart || *( pszWhere - 1 ) == ' ' )
	if( *pszTerminator == ' ' || *pszTerminator == '\0' )
	  return 1;
      pszStart = pszTerminator;
    }
  return 0;
}


static void pw13_sdl_vbo_cube_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources (p, time);
  p=p;
  time=time;

  if ( g_fVBOSupported ) {

    glDrawArrays(GL_QUADS, 0, 6*4);

  } else {
    printf("VBO not supported");
  }

}


void BuildVBOs()
{
  glGenBuffersARB(1, &vertexBuffer);
  glGenBuffersARB(1, &colorBuffer);

  glBindBufferARB(GL_ARRAY_BUFFER, vertexBuffer);
  glBufferDataARB(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

  glBindBufferARB(GL_ARRAY_BUFFER, colorBuffer);
  glBufferDataARB(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);

  glBindBufferARB(GL_ARRAY_BUFFER, vertexBuffer);
  glVertexPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));

  glBindBufferARB(GL_ARRAY_BUFFER, colorBuffer);
  glColorPointer(3, GL_FLOAT, 0, BUFFER_OFFSET(0));

  glBindBufferARB(GL_ARRAY_BUFFER, 0);

}


static int pw13_sdl_vbo_cube_start (Pw13_Patch *p, Pw13_Time *time)
{
  p=p;
  time=time;

  /* Check For VBOs Supported */

  g_fVBOSupported = IsExtensionSupported( "GL_ARB_vertex_buffer_object" );

  if( g_fVBOSupported )
    {
      BuildVBOs();
    }

  return 1;
}


static void pw13_sdl_vbo_cube_stop (Pw13_Patch *p)
{
  p=p;

}

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init(name, p, 1, 1);
  pw13_void_input("()", p->input, p);
  pw13_void_output("()", p->output, p);
  p->pump = pw13_sdl_vbo_cube_pump;
  p->start = pw13_sdl_vbo_cube_start;
  p->stop = pw13_sdl_vbo_cube_stop;
  return 1;
}
