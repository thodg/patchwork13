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

  classpath.h  -  loading of patch classes through classpath

*/
#ifndef PW13_CLASSPATH_H_INCLUDED
#define PW13_CLASSPATH_H_INCLUDED 1

#include "dll.h"
#include "types.h"


/** Find a patch class in a patchwork from its class path */
PW13_EXTERN
Pw13_PatchClass *pw13_find_classpath (const char *path);

/** Load a patch class from its class path (eg "std/float/pi") */
PW13_EXTERN
Pw13_PatchClass *pw13_load_classpath (const char *path);


/** Register or return registered patch class from its path.
    Use this every time you can ! */
PW13_EXTERN
Pw13_PatchClass *pw13_register_classpath (const char *classpath);

/** Returns a patch of the given class path, and adds it to the patchwork.
 */
PW13_EXTERN
Pw13_Result pw13_patch_of_classpath (Pw13_Patch *p, const char *classpath,
				     const char *name);

/** Transforms classpath corresponding to a given file path.
    It returns 0 if the path is valid and was transformed into a classpath.
*/
PW13_EXTERN
int pw13_classpath_of_filepath (char *path);


#endif
/* ndef PW13_CLASSPATH_H_INCLUDED */
