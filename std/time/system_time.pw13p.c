/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13_std - the patchwork13 standard patches library
  Copyright (C) 2005 Thomas de Grivel <billitch@yahoo.fr>
  
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

  system_time.pw13p.c  -  system time patch

*/
#ifdef WIN32
#  include <windows.h>
#  define PW13_TIME_T DWORD
#else
#  define PW13_TIME_T struct timeval
#endif

#include <unistd.h>
#include <sys/time.h>
#include <pw13/pw13.h>


void get_time (PW13_TIME_T *t)
{
#ifdef WIN32
  *t = GetTickCount (); 
#else
  gettimeofday (t, NULL);
#endif
}


void conv_time (Pw13_Time *res, PW13_TIME_T *t)
{
#ifdef WIN32
  res->seconds = *t / 1000;
  res->frac = (*t % 1000) / 1000.0;
#else
  res->seconds = t->tv_sec;
  res->frac = t->tv_usec / 1000000.0;
#endif
}


static void patch_pump (Pw13_Patch *p, Pw13_Time *time)
{
  PW13_TIME_T	t;
  Pw13_Time*	o;

  pw13_pump_input (p->input, time);
  get_time (&t);
  o = pw13_time_output_val (p->output[0]);
  conv_time (o, &t);
  o->ticks++;
}


static int patch_start (Pw13_Patch *p, Pw13_Time *time)
{
  PW13_TIME_T t;
  get_time (&t);
  conv_time (pw13_time_output_val (p->output[0]), &t);
  time = time;
  return 1;
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 1, 1);
  pw13_any_input ("'a", p->input, p);
  pw13_time_output ("t", p->output, p);
  p->pump = patch_pump;
  p->start = patch_start;
  param = param;
  return 1;
}
