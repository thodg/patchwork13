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

  bool.pw13dt.h  -  bool inputs and outputs
  MB
*/
#ifndef PW13_STD_BOOL_H_INCLUDED
#define PW13_STD_BOOL_H_INCLUDED 1

#include <pw13/pw13.h>

/* input */
#define pw13_bool_input(name, input, patch, defval) \
  pw13_input_init (name, input, patch, pw13_data_type ("bool"), \
		   pw13_data_b (defval))

#define pw13_bool_input_val(input) (((pw13_input_val(input))[1]).f)

/* output */
#define pw13_bool_output(name, output, patch) \
  pw13_output_init (name, output, patch, pw13_data_type ("bool"), \
                    pw13_data_b (0))

#define pw13_bool_output_val(output) ((output).data[1].f)

#endif
/* not PW13_STD_BOOL_INCLUDED */
