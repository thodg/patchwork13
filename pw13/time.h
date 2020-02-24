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
#ifndef PW13_TIME_H_INCLUDED
#define PW13_TIME_H_INCLUDED

#include "types.h"
#include "dll.h"


struct Pw13_Time
{
  long seconds;
  double frac;
  unsigned long ticks;
};


PW13_EXTERN
void pw13_time_of_float (Pw13_Time *t, double d);
PW13_EXTERN
double pw13_float_of_time (Pw13_Time *t);


/* operations */
PW13_EXTERN
void pw13_time_zero (Pw13_Time *time);
PW13_EXTERN
void pw13_time_add (Pw13_Time *result, Pw13_Time *a, Pw13_Time *b);
PW13_EXTERN
void pw13_time_sub (Pw13_Time *result, Pw13_Time *a, Pw13_Time *b);
PW13_EXTERN
void pw13_time_increase (Pw13_Time *result, Pw13_Time *delta);
PW13_EXTERN
void pw13_time_decrease (Pw13_Time *result, Pw13_Time *delta);

/* observers */
PW13_EXTERN
int pw13_time_compare (Pw13_Time *a, Pw13_Time *b);


/* time as patch data */
PW13_EXTERN
Pw13_Data * pw13_time_data (Pw13_Time *t);
#define pw13_data_time(d) ((Pw13_Time*) ((d)+1))

/* time input */
#define pw13_time_input(name, input, patch, defval)		\
  pw13_input_init (name, input, patch, pw13_data_type ("time"),	\
                   defval)

#define pw13_time_input_val(input) (pw13_data_time (pw13_input_val (input)))

/* time output */
#define pw13_time_output(name, output, patch)				\
  pw13_output_init (name, output, patch, pw13_data_type ("time"),	\
                    pw13_time_data (0))

#define pw13_time_output_val(output) (pw13_data_time ((output).data))


/** Portable sleep function, in milliseconds */
PW13_EXTERN
void pw13_msleep (unsigned long milliseconds);



#endif
/* ndef PW13_TIME_H_INCLUDED */
