/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13 - the patchwork13 core library
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

  time.h  -  give us some time

*/
#ifdef WIN32
#  include <windows.h>
#else
#  include <unistd.h>
#endif

#include <stdlib.h>
#include <string.h>
#include "data.h"
#include "time.h"


void pw13_time_of_float(Pw13_Time *t, double f)
{
  t->seconds = (long) f;
  t->frac = f - t->seconds;
  t->ticks = 0;
}


void pw13_time_zero(Pw13_Time *time)
{
  time->seconds = 0;
  time->frac = 0;
  time->ticks = 0;
}


void pw13_time_add(Pw13_Time *result, Pw13_Time *a, Pw13_Time *b)
{
  int i;
  result->frac = a->frac + b->frac;
  i = (int) result->frac;
  result->seconds = i + a->seconds + b->seconds;
  result->frac -= i;
  result->ticks = a->ticks + b->ticks;
}


void pw13_time_sub(Pw13_Time *result, Pw13_Time *a, Pw13_Time *b)
{
  int i;
  result->frac = a->frac - b->frac;
  i = (int) result->frac;
  result->seconds = i + a->seconds - b->seconds;
  result->frac -= i;
  result->ticks = a->ticks - b->ticks;
}


void pw13_time_increase(Pw13_Time *result, Pw13_Time *delta)
{
  int i;
  result->frac += delta->frac;
  i = (int) result->frac;
  result->frac -= i;
  result->seconds += i + delta->seconds;
  result->ticks += delta->ticks;
}


void pw13_time_decrease (Pw13_Time *result, Pw13_Time *delta)
{
  int i;
  result->frac -= delta->frac;
  i = (int) result->frac;
  result->frac -= i;
  result->seconds -= i + delta->seconds;
  result->ticks -= delta->ticks;
}


int pw13_time_compare (Pw13_Time *a, Pw13_Time *b)
{
  if (a->seconds < b->seconds)
    return -1;
  else if (a->seconds > b->seconds)
    return 1;
  else if (a->frac < b->frac)
    return -1;
  else if (a->frac > b->frac)
    return 1;
  else if (a->ticks < b->ticks)
    return -1;
  else if (a->ticks > b->ticks)
    return 1;
  return 0;
}


Pw13_Data * pw13_time_data (Pw13_Time *t)
{
  Pw13_Data *d = pw13_data_bytes (sizeof (Pw13_Time));
  if (t)
    memcpy (d+1, t, sizeof (Pw13_Time));
  else
    pw13_time_zero (pw13_data_time (d));
  return d;
}


void pw13_msleep (unsigned long milliseconds)
{
#ifdef WIN32
  Sleep (milliseconds);
#else
  usleep (milliseconds * 1000);
#endif
}
