/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13_stk - the patchwork13 standard patches library
  Copyright (C) 2006 Maxime Louchart <maxime.louchart@gmail.com>

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

  float_out.pw13p.c  -  STK sound output patch

*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <stk/RtWvOut.h>
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>


struct	PatchParam
{
  RtWvOut	*wo;
  StkFrames	*f;

  PatchParam ()
  {
    wo = NULL;
    f = NULL;
  }

  bool init (unsigned int nchannels)
  {
    try {
      wo = new RtWvOut (nchannels);
    }
    catch (StkError&) {
      return false;
    }    
    f  = new StkFrames (1, nchannels);
    return true;
  }

  ~PatchParam ()
  {
    if (f)
      delete f;
    if (wo)
      delete wo;
  }
};


static void patch_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources (p, time);
  PatchParam	*pp = (PatchParam*) p->param;
  int		c = pp->f->channels ();
  while (c--)
    (*pp->f)[c] = pw13_float_input_val (p->input + c);
  pp->wo->tick (*pp->f);

  Pw13_Time	*t = pw13_time_output_val (p->output[0]);
  pw13_time_of_float (t, pp->wo->getTime ());
}

static int patch_start (Pw13_Patch *p, Pw13_Time *time)
{
  time = time;
  PatchParam	*pp = (PatchParam*) p->param;
  pp->wo->start ();
  return 0;
}

static void patch_stop (Pw13_Patch *p)
{
  PatchParam	*pp = (PatchParam*) p->param;
  pp->wo->stop ();
}

static void patch_destroy (Pw13_Patch *p)
{
  PatchParam	*pp = (PatchParam*) p->param;
  delete pp;
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  Stk::showWarnings (true);
  PatchParam	*pp;
  pp = new PatchParam ();
  if (!pp->init (2))
    return 0;

  param = param;
  pw13_patch_init (name, p, 2, 1);
  pw13_float_input ("L", p->input, p, 0);
  pw13_float_input ("R", p->input + 1, p, 0);
  pw13_time_output ("t", p->output, p);
  p->param = pp;
  p->pump = patch_pump;
  p->start = patch_start;
  p->stop = patch_stop;
  p->destroy = patch_destroy;
  return 1;
}
