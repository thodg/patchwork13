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

  int.pw13dt.h  -  int inputs and outputs

*/
#ifndef PW13_STD_INT_H_INCLUDED
#define PW13_STD_INT_H_INCLUDED 1

#include <pw13/pw13.h>

/* input */
#define pw13_int_input(name, input, patch, defval) \
  pw13_input_init (name, input, patch, pw13_data_type("int"), \
                   pw13_data_i(defval))

#define pw13_int_input_val(input) ((pw13_input_val(input))[1].i)

/* output */
#define pw13_int_output(name, output, patch) \
  pw13_output_init (name, output, patch, pw13_data_type ("int"), \
		    pw13_data_i (0))

#define pw13_int_output_val(output) ((output).data[1].i)


#endif
/* not PW13_STD_INT_INCLUDED */
