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

  file-open.pw13p.c  -  Opens a file and sends its descriptor

*/

#include <pw13/pw13.h>
#include <std/stream.pw13dt.h>


typedef struct {
  FILE* f;
} s_file_open, *p_file_open;


static int patch_start (Pw13_Patch *p, Pw13_Time *time)
{
  time = time;
  p_file_open f = malloc(sizeof (s_file_open));
  p->param = f;
  f->f = fopen("/goinfre/pwet","r");
  pw13_stream_output_val (p->output[0])  =   f->f;
  return 1;
}

static void patch_stop (Pw13_Patch *p)
{
  free (p->param);
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init(name, p, 0, 1);
  pw13_stream_output ("Stream", p->output, p);
  p->start = patch_start;
  p->stop = patch_stop;
  return 1;
}
