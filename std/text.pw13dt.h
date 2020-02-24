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

  text.pw13dt.h  -  text data type, inputs and outputs.

*/
#ifndef PW13_STD_TEXT_H_INCLUDED
#define PW13_STD_TEXT_H_INCLUDED 1

#include <string.h>
#include <pw13/pw13.h>

/* data type */
#define pw13_data_text(data) \
  ((char*) (data)[1].t.c)

#define pw13_text_data(text)						\
  (((Pw13_Data*)							\
    strcpy (pw13_data_text (pw13_data_array (strlen (text)		\
					     / sizeof(Pw13_Data))	\
			    ),						\
	    text)							\
    ) - 1)


/* input */
#define pw13_text_input(name, input, patch, defval) \
  pw13_input_init (name, input, patch, pw13_data_type ("text"), \
                   pw13_text_data(defval))

#define pw13_text_input_val(input) \
  ((char*)((pw13_input_val(input))[1].t.c))

#define pw13_text_input_len(input) \
  ((pw13_input_val(input))->nblocks * sizeof (pw13_data_t))

/* output */
#define pw13_text_output(name, output, patch) \
  pw13_output_init (name, output, patch, pw13_data_type ("text"), \
                    pw13_data_i (0))

#define pw13_text_output_val(output) \
  ((output).data[1].t.c)

#endif
/* not PW13_STD_TEXT_INCLUDED */
