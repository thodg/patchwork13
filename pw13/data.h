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

  data.h  -  data, data types and operators

*/
#ifndef PW13_DATA_H_INCLUDED
#define PW13_DATA_H_INCLUDED

#include <stdarg.h>
#include "dll.h"
#include "types.h"
#include "list.h"


/**  Representation of the data to flow through patches  **/

union Pw13_Data {
  unsigned int nblocks;
  union {
    char c[4];
    unsigned char uc[4];
    short s[2];
    unsigned short us[2];
  } t;
  char c;
  unsigned char uc;
  short s;
  unsigned short us;
  int i;
  unsigned int ui;
  long l;
  unsigned long ul;
  float f;
  int b;
  char *pc;
  void *ptr;
};


extern Pw13_Data pw13_data_zero_s;


/* constructors */

/** Allocate a single data block containing zero.
    That means that the data segment contains no data. */
PW13_EXTERN Pw13_Data * pw13_data_zero ();

/** Allocate two blocks.
    The first contains 1 in nblocks because there is a
    single data block in the data segment.
    The second block contains user data.*/
PW13_EXTERN Pw13_Data * pw13_data_one ();

PW13_EXTERN Pw13_Data * pw13_data_one_init (Pw13_Data val);

/** Allocate an array of blocks.
    The first block contains the blocks count, and is followed by
    the counted blocks.
 */
PW13_EXTERN Pw13_Data * pw13_data_array (unsigned int nblocks);

/** Allocate an array of blocks with a given byte size.
    The first block contains the blocks count, and is followed by
    enough blocks to contain the given number of bytes.
 */
PW13_EXTERN Pw13_Data * pw13_data_bytes (unsigned int nbytes);

/** Allocate and initialize an array of blocks.
    The first block contains the blocks count, and is followed by
    the counted blocks.
 */
PW13_EXTERN Pw13_Data * pw13_data_array_init (unsigned int nblocks, ...);

/** Copy a data segment */
PW13_EXTERN Pw13_Data * pw13_data_copy (Pw13_Data *d);


/* operators */

/** Returns the size in bytes of a data segment */
PW13_EXTERN unsigned int pw13_data_size (Pw13_Data *d);

/** Free a data segment */
PW13_EXTERN void pw13_data_free (Pw13_Data *d);

/* Data constructors for convenience */
PW13_EXTERN Pw13_Data *pw13_data_c   (char val);
PW13_EXTERN Pw13_Data *pw13_data_uc  (unsigned char val);
PW13_EXTERN Pw13_Data *pw13_data_s   (short val);
PW13_EXTERN Pw13_Data *pw13_data_us  (unsigned short val);
PW13_EXTERN Pw13_Data *pw13_data_i   (int val);
PW13_EXTERN Pw13_Data *pw13_data_ui  (unsigned int val);
PW13_EXTERN Pw13_Data *pw13_data_l   (long val);
PW13_EXTERN Pw13_Data *pw13_data_ul  (unsigned long val);
PW13_EXTERN Pw13_Data *pw13_data_f   (float val);
PW13_EXTERN Pw13_Data *pw13_data_b   (int val);
PW13_EXTERN Pw13_Data *pw13_data_pc  (char *val);
PW13_EXTERN Pw13_Data *pw13_data_ptr (void *val);


#endif
/* ndef PW13_DATA_H_INCLUDED */
