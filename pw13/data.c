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

  data.c  -  data constructors

*/
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include "data.h"
#include "types.h"

Pw13_Data pw13_data_zero_s = {0};


Pw13_Data * pw13_data_zero ()
{
  return &pw13_data_zero_s;
}


Pw13_Data * pw13_data_one ()
{
  Pw13_Data *s = malloc (2 * sizeof (Pw13_Data));
  s->nblocks = 1;
  return s;
}


Pw13_Data * pw13_data_array (unsigned int nblocks)
{
  Pw13_Data *s = malloc ((nblocks + 1) * sizeof (Pw13_Data));
  s->nblocks = nblocks;
  return s;
}


Pw13_Data * pw13_data_bytes (unsigned int nbytes)
{
  unsigned int nblocks = (nbytes + sizeof (Pw13_Data) - 1) / sizeof (Pw13_Data);
  return pw13_data_array (nblocks);
}


Pw13_Data * pw13_data_array_init (unsigned int nblocks, ...)
{
  va_list ap;
  va_start (ap, nblocks);
  Pw13_Data *a = pw13_data_array (nblocks);
  unsigned int i;
  for (i = 1; i <= nblocks; i++)
    a[i] = va_arg (ap, Pw13_Data);
  va_end (ap);
  return a;
}


Pw13_Data * pw13_data_copy (Pw13_Data *d)
{
  int size = (d->nblocks + 1) * sizeof (Pw13_Data);
  Pw13_Data *s = malloc (size);
  memcpy (s, d, size);
  return s;
}


unsigned int pw13_data_size (Pw13_Data *d)
{
  return (d->nblocks + 1) * sizeof (Pw13_Data);
}


void pw13_data_free (Pw13_Data *d)
{
  free (d);
}


/* data constructors for convenience */
#define PW13_DATA_CONSTRUCTOR(name, type, field) \
  Pw13_Data * name (type val)			 \
  {						 \
    Pw13_Data *d = pw13_data_one ();		 \
    d[1].field = val;				 \
    return d;					 \
  }

PW13_DATA_CONSTRUCTOR (pw13_data_c,  char, c)
PW13_DATA_CONSTRUCTOR (pw13_data_uc, unsigned char, uc)
PW13_DATA_CONSTRUCTOR (pw13_data_s,  short, s)
PW13_DATA_CONSTRUCTOR (pw13_data_us, unsigned short, us)
PW13_DATA_CONSTRUCTOR (pw13_data_i,  int, i)
PW13_DATA_CONSTRUCTOR (pw13_data_ui, unsigned int, ui)
PW13_DATA_CONSTRUCTOR (pw13_data_l,  long, l)
PW13_DATA_CONSTRUCTOR (pw13_data_ul, unsigned long, ul)
PW13_DATA_CONSTRUCTOR (pw13_data_f,  float, f)
PW13_DATA_CONSTRUCTOR (pw13_data_b,  int, b)
PW13_DATA_CONSTRUCTOR (pw13_data_pc, char*, pc)
PW13_DATA_CONSTRUCTOR (pw13_data_ptr,void*, ptr)

#undef PW13_DATA_CONSTRUCTOR
