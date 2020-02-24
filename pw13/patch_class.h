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

  patch_class.h  -  registered patch constructors

*/
#ifndef PW13_PATCH_CLASS_H_INCLUDED
#define PW13_PATCH_CLASS_H_INCLUDED 1

#include "dll.h"
#include "types.h"
#include "patch.h"
#include "patchwork.h"


struct Pw13_PatchClass {
  const char *name;
  const char *desc;
  void *param;
  Pw13_PatchConstructor *init;
  const char *filename;
  const char *classpath;
};


/** List of the patch classes that are loaded */
extern Pw13_List *pw13_loaded_patch_classes;


/** Allocate and init a new patch class */
PW13_EXTERN
Pw13_PatchClass pw13_patch_class (const char *name,
				  const char *description,
				  Pw13_PatchConstructor *init,
				  void *param);

/** Creates a patch of a given patch class */
PW13_EXTERN
Pw13_Result pw13_make_patch_of_class (Pw13_Patch *p,
				      Pw13_PatchClass *p_class,
				      const char *patch_name);


/** Register a patch class. Do not use **/
PW13_EXTERN
int pw13_patch_class_reg (Pw13_PatchClass *pc);


#endif
/* ndef PW13_PATCH_CLASS_H_INCLUDED */
