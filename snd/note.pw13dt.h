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

  notes.pw13dt.h  -  notes inputs and outputs

*/
#ifndef PW13_SOUND_NOTES_H_INCLUDED
#define PW13_SOUND_NOTES_H_INCLUDED 1

#include <pw13/pw13.h>
#include <std/int.pw13dt.h>

/* input */
#define pw13_note_input pw13_int_input
#define pw13_note_input_val pw13_int_input_val
#define pw13_notes_input(name, input, patch)	  \
  pw13_input_init (name, input, patch,		  \
                   pw13_data_type("int"),	  \
		   pw13_data_zero())

/* output */
#define pw13_note_output pw13_int_output
#define pw13_note_output_val pw13_int_output_val
#define pw13_notes_output(name, output, patch)	\
  pw13_output_init (name, output, patch,	\
		    pw13_data_type("int"),	\
		    pw13_data_zero())


#endif
/* not PW13_SOUND_NOTES_INCLUDED */
