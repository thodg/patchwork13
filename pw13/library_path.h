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

  library_path.h  -  loading of patch classes through classpath

*/
#ifndef PW13_LIBRARY_PATH_H_INCLUDED
#define PW13_LIBRARY_PATH_H_INCLUDED

#include "dll.h"

extern char * pw13_cur_library_path;

/** Returns the current library path */
PW13_EXTERN
const char * pw13_library_path ();

/** Change the current library path */
PW13_EXTERN
void pw13_set_library_path (const char *path);


#endif
/* ndef PW13_LIBRARY_PATH_H_INCLUDED */
