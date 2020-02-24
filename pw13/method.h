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

  method.h  -  Patch methods

*/
#ifndef PW13_METHOD_H_INCLUDED
#define PW13_METHOD_H_INCLUDED

#include "dll.h"
#include "types.h"


struct Pw13_Method
{
  Pw13_MethodName *name;
  /** Method parameter (not the same as the calling parameter) */
  void *param;
  /** Pointer to the function */
  Pw13_MethodFunc *func;
  /** Link to next method function (to chain them) */
  Pw13_Method *next;
};

PW13_EXTERN
Pw13_Method * pw13_method_add (Pw13_MethodName **method_names, const char *name,
			       Pw13_MethodFunc *f, void *object,
			       void *method_param);

PW13_EXTERN
void pw13_method_remove (Pw13_Method *method, Pw13_MethodName **method_names);



struct Pw13_MethodName
{
  const char *name;
  void *object;
  Pw13_MethodName *next;
  Pw13_Method *methods;
};

/** Finds a patch method from its name. */
PW13_EXTERN
Pw13_MethodName ** pw13_method_name_find (Pw13_MethodName **method_names,
					  const char *name);

/** Returns the patch method with a given name, creating it if necessary. */
PW13_EXTERN
Pw13_MethodName * pw13_method_name_register (Pw13_MethodName **method_names,
					    void *object, const char *name);

/** Removes a method from a patch methods list. */
PW13_EXTERN
void pw13_method_name_remove (Pw13_MethodName *n,
			      Pw13_MethodName **method_names);


#endif
/* ndef PW13_METHOD_H_INCLUDED */
