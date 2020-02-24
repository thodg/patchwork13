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

  names.h  -  functions related to string names

*/
#ifndef PW13_NAMES_H_INCLUDED
#define PW13_NAMES_H_INCLUDED 1

#include "dll.h"
#include "types.h"

/** Find a patch in a patchwork from its name */
PW13_EXTERN
Pw13_Patch * pw13_find_patch (Pw13_Patchwork *pw, const char *name);

/** Returns a unique patch name */
PW13_EXTERN
char * pw13_unique_patch_name (Pw13_Patchwork *pw, const char *classpath);


/** Find an input in a patch from its name */
PW13_EXTERN
Pw13_Input * pw13_find_input (Pw13_Patch *p, const char *name);

/** Find an output in a patch from its name */
PW13_EXTERN
Pw13_Output * pw13_find_output (Pw13_Patch *p, const char *name);


#endif
/* ndef PW13_NAMES_H_INCLUDED */
