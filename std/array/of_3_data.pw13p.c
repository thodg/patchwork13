/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13_cluster - the patchwork13 cluster
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

  std/array/of_3_data.pw13p.c - make an array with 3 datas

*/

#include <pw13/pw13.h>
#include <pw13/any.h>


static void patch_pump (Pw13_Patch *p, Pw13_Time *time)
{
  int i;
  Pw13_Data *res_array = p->output[0].data;
  pw13_pump_sources (p, time);
  
  for (i=0;i<3;i++) {
      Pw13_Data *d = pw13_input_val (p->input+i);
      res_array[i+1] = d[1];
  }   
}
    
  
static int patch_start (Pw13_Patch *p, Pw13_Time *time)
{
  time=time;
  p->output[0].data = pw13_data_array (3);
  return 1;
}


static void patch_stop (Pw13_Patch *p)
{
  pw13_data_free(p->output[0].data);
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init (name, p, 3, 1);
  pw13_any_input ("A", p->input, p);
  pw13_any_input ("B", p->input+1, p);
  pw13_any_input ("C", p->input+2, p);
  pw13_any_output ("{A,B,C}", p->output, p);
  p->pump = patch_pump;
  p->start = patch_start;
  p->stop = patch_stop;

  return 1;
}
