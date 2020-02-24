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

  delay.pw13p.c  -  Generates a delay on buffers

*/

#include <stdlib.h>
#include <stdio.h>
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>
#include <std/int.pw13dt.h>


typedef struct s_delay{
  int nb_samples;
  int absolute_position;
  float* samples_buffer;
} s_delay;


static void pw13_delay_pump (Pw13_Patch *p, Pw13_Time *time)
{
  float feedback;
  s_delay* local_param;

  int i,delay;
 
  pw13_pump_sources (p, time);

  feedback=pw13_float_input_val (p->input+2);
  local_param = (s_delay*) p->param;

  delay = pw13_int_input_val (p->input+1);
  if (delay < 0)
    {
      delay = 0;
    }


  if ( (local_param->nb_samples) != (delay)  )
    {     
      if( local_param->samples_buffer != NULL)
	{
	  local_param->samples_buffer=realloc(local_param->samples_buffer, 
					      (delay)*sizeof(float));
	  
	  if ( ((local_param->nb_samples) < (delay))  && (delay != 0 ))
	    {
	      for (i=(local_param->nb_samples-1);i<(delay);i++)
		{
		  local_param->samples_buffer[i]=local_param->samples_buffer[i % (local_param->nb_samples)];
		}
	    }
	}      
      else
	{
	  local_param->samples_buffer=malloc(sizeof(float)*(delay));
	}

      local_param->nb_samples = delay;

    }

  
  if( local_param->samples_buffer != NULL)
    {
      local_param->absolute_position ++ ;
      local_param->absolute_position %= local_param->nb_samples;
      
      local_param->samples_buffer[local_param->absolute_position] =
	local_param->samples_buffer[local_param->absolute_position] * feedback
	+ pw13_float_input_val (p->input);
      pw13_float_output_val (p->output[0]) =
	local_param->samples_buffer[local_param->absolute_position];
    }
}  


static int pw13_delay_start (Pw13_Patch *p, Pw13_Time *time)
{
  time=time;
  s_delay* start_param=malloc(sizeof(s_delay*));
  start_param->nb_samples=pw13_float_input_val (p->input+1);
  start_param->absolute_position=-1;
  start_param->samples_buffer=NULL;
  (p->param)=start_param;
  return 1;
}

static void pw13_delay_stop (Pw13_Patch *p)
  {
    free(p->param);
  }

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init(name, p, 3, 1);
  pw13_float_input("Sample Input", p->input, p, 0);
  pw13_int_input("Delay", p->input+1, p, 1000);
  pw13_float_input("Feedback", p->input+2, p, 0.5);
  pw13_float_output("Sample Output", p->output, p);
  p->pump = pw13_delay_pump;
  p->start = pw13_delay_start;
  p->stop = pw13_delay_stop;
  return 1;
}
