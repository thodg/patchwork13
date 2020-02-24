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

  pipe-out.pw13p.c  -  stream pipe out patch

*/
#include <stdio.h>
#include <pw13/pw13.h>
#include <std/stream.pw13dt.h>
#include <std/text.pw13dt.h>


static void patch_pump (Pw13_Patch *p, Pw13_Time *t)
{
  p = p;
  t = t;
}

static void patch_first_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources (p, time);

  pw13_stream_output_val (p->output[0]) =
    popen (pw13_text_input_val (p->input), "w");
  p->pump = patch_pump;
}

static int patch_start (Pw13_Patch *p, Pw13_Time *t)
{
  t = t;
  p->pump = patch_first_pump;
  return 1;
}

static void patch_stop (Pw13_Patch *p)
{
  pclose (pw13_stream_output_val (p->output[0]));
}

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 1, 1);
  pw13_text_input ("command", p->input, p, "");
  pw13_stream_output ("pipe out", p->output, p);
  p->start = patch_start;
  p->stop = patch_stop;
  param = param;
  return 1;
}

