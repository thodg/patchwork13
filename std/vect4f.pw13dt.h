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

  vect4f.pw13dt.h  -  float inputs and outputs

*/
#ifndef PW13_STD_VECT4F_H_INCLUDED
#define PW13_STD_VECT4F_H_INCLUDED 1

#include <pw13/pw13.h>

#define pw13_vect4f(x, y, z, t) \
  pw13_data_array_init (4, (Pw13_Data) x, \
                           (Pw13_Data) y, \
                           (Pw13_Data) z, \
                           (Pw13_Data) t)

/* input */
#define pw13_vect4f_input(name, input, patch, defval) \
  pw13_input_init (name, input, patch, \
                   pw13_data_type("vect4f"), \
                   defval)
#define pw13_vect4f_input_val(input) (&(pw13_input_val(input)+1)[0].f)

#define pw13_vect4f_input_x(input) (pw13_input_val(input)+1)[0].f
#define pw13_vect4f_input_y(input) (pw13_input_val(input)+1)[1].f
#define pw13_vect4f_input_z(input) (pw13_input_val(input)+1)[2].f
#define pw13_vect4f_input_t(input) (pw13_input_val(input)+1)[3].f

/* output */
#define pw13_vect4f_output(name, output, patch) \
  pw13_output_init (name, output, patch, pw13_data_type ("vect4f"),	\
		    pw13_data_f (0))

#define pw13_vect4f_output_x(output) ((output).data[1].f)
#define pw13_vect4f_output_y(output) ((output).data[2].f)
#define pw13_vect4f_output_z(output) ((output).data[3].f)
#define pw13_vect4f_output_t(output) ((output).data[4].f)

#endif
/* ndef PW13_STD_VECT4F_INCLUDED */
