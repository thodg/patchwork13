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

  random.pw13p.c  -  random int patch

*/
#ifdef WIN32
#  include <windows.h>
#endif

#include <pw13/pw13.h>
#include <sys/time.h>
#include <time.h>
#include <stdio.h> 
#include <std/int.pw13dt.h>

static void patch_pump (Pw13_Patch *p, Pw13_Time *time)
{
  int new_val;
  int * vals = (int*) p->param;

  pw13_pump_sources (p, time);

  if ( (pw13_int_input_val (p->input)) == 0 ) 
    new_val=0;
  else  
    new_val = (vals[0] + vals[1]) % (pw13_int_input_val (p->input));
  

  vals[1] = vals[0];
  vals[0] = new_val;
  
  pw13_int_output_val (p->output[0]) =
    new_val;
}

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  int *vals;
  pw13_patch_init (name, p, 1, 1);
  pw13_int_input ("Max", p->input, p, sizeof(int));
  pw13_int_output ("Random", p->output, p);
  p->pump = patch_pump;
  p->param = malloc (2 * sizeof (int));
  vals = (int*) p->param;
  vals[1] = 0;
  param = param;

#ifdef WIN32
  vals[0] = GetTickCount () * 2 + 1;
#else
  {
    struct timeval tp;
    gettimeofday (&tp,NULL);
    vals[0] = (int) tp.tv_usec % (tp.tv_usec + 1);
  }
#endif

  return 1;
}
