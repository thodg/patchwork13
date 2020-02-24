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

  error.h  -  Error codes and error reporting functions

  TODO:
  
  * use pw13_result as return type of all library functions

*/
#ifndef PW13_ERROR_H_INCLUDED
#define PW13_ERROR_H_INCLUDED

#include <stdarg.h>
#include "dll.h"
#include "types.h"

#ifndef PW13_DEBUG
#  define PW13_DEBUG 0
#endif


/** Print a formated error message on stderr */
PW13_EXTERN
Pw13_Result pw13_error (const char *format, ...);

/** Print a formated error message and exit */
PW13_EXTERN
void pw13_fatal_error (const char *format, ...);


#endif
/* ndef PW13_ERROR_H_INCLUDED */
