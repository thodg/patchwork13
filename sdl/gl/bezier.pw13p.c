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

  vbo_cube.pw13p.c  -  SDL  opengl bezier patch

  patch_class_LDFLAGS = -lm
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_opengl.h>
#include <math.h>
#include <pw13/pw13.h>
#include <std/bool.pw13dt.h>

typedef struct point_3d {
  GLfloat x, y, z;
} POINT_3D;

POINT_3D grid[4][4];
GLuint dlB;
int divs = 7;


POINT_3D pointAdd(POINT_3D p, POINT_3D q)
{
  p.x += q.x;
  p.y += q.y;
  p.z += q.z;

  return p;
}


POINT_3D pointTimes (double c, POINT_3D p)
{
  p.x *= c;
  p.y *= c;
  p.z *= c;

  return p;
}


POINT_3D makePoint (double a, double b, double c)
{
  POINT_3D p;

  p.x = a;
  p.y = b;
  p.z = c;

  return p;
}


void initBezier( void ) {
  grid[0][0] = makePoint(-0.75,	-0.75,	-0.50);
  grid[0][1] = makePoint(-0.25,	-0.75,	 0.00);
  grid[0][2] = makePoint( 0.25,	-0.75,	 0.00);
  grid[0][3] = makePoint( 0.75,	-0.75,	-0.50);
  grid[1][0] = makePoint(-0.75,	-0.25,	-0.75);
  grid[1][1] = makePoint(-0.25,	-0.25,	 0.50);
  grid[1][2] = makePoint( 0.25,	-0.25,	 0.50);
  grid[1][3] = makePoint( 0.75,	-0.25,	-0.75);
  grid[2][0] = makePoint(-0.75,	 0.25,	 0.00);
  grid[2][1] = makePoint(-0.25,	 0.25,	-0.50);
  grid[2][2] = makePoint( 0.25,	 0.25,	-0.50);
  grid[2][3] = makePoint( 0.75,	 0.25,	 0.00);
  grid[3][0] = makePoint(-0.75,	 0.75,	-0.50);
  grid[3][1] = makePoint(-0.25,	 0.75,	-1.00);
  grid[3][2] = makePoint( 0.25,	 0.75,	-1.00);
  grid[3][3] = makePoint( 0.75,	 0.75,	-0.50);
}


POINT_3D Bernstein(float u, POINT_3D *p)
{
  POINT_3D a, b, c, d, r;

  a = pointTimes(pow(u,3), p[0]);
  b = pointTimes(3*pow(u,2)*(1-u), p[1]);
  c = pointTimes(3*u*pow((1-u),2), p[2]);
  d = pointTimes(pow((1-u),3), p[3]);

  r = pointAdd(pointAdd(a, b), pointAdd(c, d));

  return r;
}


GLuint genBezier( int divs)
{
  int u = 0, v;
  float py, px, pyold;
  GLuint drawlist = glGenLists(1);
  POINT_3D temp[4];
  POINT_3D *last = (POINT_3D*)malloc(sizeof(POINT_3D)*(divs+1));

  temp[0] = grid[0][3];
  temp[1] = grid[1][3];
  temp[2] = grid[2][3];
  temp[3] = grid[3][3];

  for (v=0;v<=divs;v++) {
    px = ((float)v)/((float)divs);
    last[v] = Bernstein(px, temp);
  }

  glNewList(drawlist, GL_COMPILE);
  for (u=1;u<=divs;u++) {
    py    = ((float)u)/((float)divs);
    pyold = ((float)u-1.0f)/((float)divs);

    temp[0] = Bernstein(py, grid[0]);
    temp[1] = Bernstein(py, grid[1]);
    temp[2] = Bernstein(py, grid[2]);
    temp[3] = Bernstein(py, grid[3]);

    glBegin(GL_TRIANGLE_STRIP);
    for (v=0;v<=divs;v++) {
      px = ((float)v)/((float)divs);

      glVertex3d(last[v].x, last[v].y, last[v].z);

      last[v] = Bernstein(px, temp);

      glVertex3d(last[v].x, last[v].y, last[v].z);
    }
    glEnd();
  }
  glEndList();

  free(last);

  return drawlist;
}



static void pw13_sdl_bezier_pump(Pw13_Patch *p, Pw13_Time *time)
{
  int i,j;

  pw13_pump_sources (p, time);

  glCallList (dlB);
  glColor3f(1.0f,0.0f,0.0f);
  if ( pw13_bool_input_val(p->input+1) ) {
    for(i=0;i<4;i++) {
      glBegin(GL_LINE_STRIP);
      for(j=0;j<4;j++)
	glVertex3d(grid[i][j].x, grid[i][j].y, grid[i][j].z);
      glEnd();
    }
    for(i=0;i<4;i++) {
      glBegin(GL_LINE_STRIP);
      for(j=0;j<4;j++)
	glVertex3d(grid[j][i].x, grid[j][i].y, grid[j][i].z);
      glEnd();
    }
  }
  glColor3f(0.0f,0.0f,1.0f);
}


static int pw13_sdl_bezier_start(Pw13_Patch *p, Pw13_Time *time)
{
  p = p;
  time = time;

  initBezier();
  dlB = genBezier(divs);
  return 1;
}


static void pw13_sdl_bezier_stop (Pw13_Patch *p)
{
  p=p;
  //TODO: CLEAN UP !?!!!
}

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 2, 1);
  pw13_void_input ("()", p->input, p);
  pw13_bool_input ("grid", p->input+1, p, 1);
  pw13_void_output ("()", p->output, p);
  p->pump = pw13_sdl_bezier_pump;
  p->start = pw13_sdl_bezier_start;
  p->stop = pw13_sdl_bezier_stop;
  return 1;
}
