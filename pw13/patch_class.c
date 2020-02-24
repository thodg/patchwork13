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

  patch_class.c  -  registered patch constructors

*/
#include <stdlib.h> /* malloc */
#include "patch.h"
#include "patch_class.h"
#include "error.h"


Pw13_List *pw13_loaded_patch_classes;


/* creates a patch of a given patch class */
Pw13_Result
pw13_make_patch_of_class (Pw13_Patch *p, Pw13_PatchClass *patch_class,
			  const char *patch_name)
{
  if (!patch_class)
    return pw13_error ("pw13_make_patch_of_class: NULL patch class");
  if ( ! patch_class->init (patch_name, p, patch_class->param) ) {
    free (p);
    return pw13_error ("pw13_make_patch_of_class: patch init failed");
  }
  p->patch_class = patch_class;
  return PW13_SUCCESS;
}


/* register a patch class */
int
pw13_patch_class_reg (Pw13_PatchClass *pc)
{
  if (!pc)
    return pw13_error ("pw13_patch_class_reg: pc is NULL\n");
  if (!pw13_list_insert_head (&pw13_loaded_patch_classes, (void*) pc))
    return pw13_error ("pw13_patch_class_reg: pw13_list_insert_head failed\n");
  return PW13_SUCCESS;
}
