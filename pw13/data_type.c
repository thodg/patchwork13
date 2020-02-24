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

  data_type.c  -  strong types for data

*/
#include <stdlib.h> /* malloc */
#include <string.h>
#include <stdarg.h>
#include "data.h"
#include "data_type.h"
#include "types.h"
#include "list.h"


Pw13_List *pw13_registered_data_types = NULL;


/* register or return a registered data type */
Pw13_DataType * pw13_data_type (const char *name)
{
  Pw13_List *L;
  if (!name)
    return NULL;
  L = pw13_registered_data_types;
  while (L && strcmp (name, ((Pw13_DataType*) L->elem)->name))
    L = L->suiv;
  if (!L) {
    Pw13_DataType *dt = (Pw13_DataType*) malloc (sizeof(Pw13_DataType));
    dt->name = strdup (name);
    L = pw13_list (dt, pw13_registered_data_types);
    pw13_registered_data_types = L;
  }
  return (Pw13_DataType*) L->elem;
}
