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

  print.pw13p.c  -  float print (as text) to stream patch

*/
#include <stdio.h>
#include <pw13/pw13.h>
#include <std/bool.pw13dt.h>
#include <std/stream.pw13dt.h>


static void patch_pump (Pw13_Patch *p, Pw13_Time *time)
{
  unsigned int n;
  Pw13_Data *d;
  FILE *f;
  pw13_pump_sources (p, time);
  f = pw13_stream_input_val (p->input+1);
  d = pw13_input_val (p->input);
  for (n=1; n<=d->nblocks; n++)
    fprintf (f, "%s ", d[n].i ? "true" : "false");
  fputs ("\n", f);
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 2, 1);
  pw13_bool_input ("b", p->input, p, 0.0f);
  pw13_stream_input ("stream", p->input+1, p, stdout);
  pw13_void_output ("()", p->output, p);
  p->pump = patch_pump;
  param = param;
  return 1;
}
