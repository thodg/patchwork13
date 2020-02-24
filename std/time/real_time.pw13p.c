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

  real_time.pw13p.c  -  real time patch starting at given time

*/
#ifdef WIN32
#  include <windows.h>
#  define PW13_TIME_T DWORD
#else
#  define PW13_TIME_T struct timeval
#endif

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


void diff_time (Pw13_Time *r, PW13_TIME_T *a, PW13_TIME_T *b)
{
#ifdef WIN32
  PW13_TIME_T c = *a - *b;
  r->seconds = c / 1000;
  r->frac = (c % 1000) / 1000;
  while (r->frac < 0) {
    r->seconds--;
    r->frac += 1000;
  }
  while (r->frac >= 1000) {
    r->seconds++;
    r->frac -= 1000;
  }
#else
  r->seconds = a->tv_sec - b->tv_sec;
  r->frac = (a->tv_usec - b->tv_usec) / 1000000.0;
  while (r->frac < 0) {
    r->seconds--;
    r->frac += 1000000;
  }
  while (r->frac >= 1000000) {
    r->seconds++;
    r->frac -= 1000000;
  }
#endif
}


static void patch_pump (Pw13_Patch *p, Pw13_Time *t)
{
  Pw13_Time *out = pw13_time_output_val (p->output[0]);
  PW13_TIME_T new_t, *start_time = (PW13_TIME_T*) p->param;
  pw13_pump_input (p->input, t);
  get_time (&new_t);
  diff_time (out, &new_t, start_time);
  out->ticks++;
}


static int patch_start (Pw13_Patch *p, Pw13_Time *time)
{
  Pw13_Time *out = pw13_time_output_val (p->output[0]);
  PW13_TIME_T *start_time = (PW13_TIME_T*) p->param;
  get_time (start_time);
  *out = *time;
  return 1;
}


static void patch_destroy (Pw13_Patch *p)
{
  free (p->param);
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 1, 1);
  pw13_any_input ("'a", p->input, p);
  pw13_time_output ("t", p->output, p);
  p->param = malloc (sizeof (struct timeval));
  p->pump = patch_pump;
  p->destroy = patch_destroy;
  p->start = patch_start;
  param = param;
  return 1;
}
