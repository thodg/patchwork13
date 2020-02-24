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

  method.h  -  Lists of methods

*/
#include <stdlib.h>
#include <string.h>
#include "patch.h"
#include "method.h"


Pw13_Method * pw13_method_add (Pw13_MethodName **method_names,
			       const char *name,
			       Pw13_MethodFunc *f, void *object,
			       void *method_param)
{
  Pw13_MethodName *n = pw13_method_name_register (method_names, object, name);
  Pw13_Method *method = malloc (sizeof (Pw13_Method));
  Pw13_Method **prev = &n->methods;
  while (*prev)
    prev = &(*prev)->next;
  if (!method)
    exit (-1);
  method->name = n;
  method->param = method_param;
  method->func = f;
  method->next = NULL;
  *prev = method;
  return method;
}


void pw13_method_remove (Pw13_Method *method, Pw13_MethodName **method_names)
{
  Pw13_MethodName *name = method->name;
  Pw13_Method **L = &name->methods;
  while (*L && *L != method)
    L = &(*L)->next;
  if (!*L) {
    fprintf (stderr, "pw13_method_remove: method not found !\n");
    exit (1);
  }
  *L = (*L)->next;
  free (method);
  if (name->methods == NULL)
    pw13_method_name_remove (name, method_names);
}


Pw13_MethodName ** pw13_method_name_find (Pw13_MethodName **method_names,
					  const char *name)
{
  Pw13_MethodName **L = method_names;
  while (*L && strcmp ((*L)->name, name))
    L = &(*L)->next;
  return L;
}


Pw13_MethodName * pw13_method_name_register (Pw13_MethodName **method_names,
					     void *object, const char *name)
{
  Pw13_MethodName **L = pw13_method_name_find (method_names, name);
  if (!*L) {
    *L = malloc (sizeof (Pw13_MethodName));
    if (!*L)
      exit (-1);
    (*L)->name = strdup (name);
    (*L)->object = object;
    (*L)->next = NULL;
    (*L)->methods = NULL;
  }
  return *L;
}


void pw13_method_name_remove (Pw13_MethodName *name,
			      Pw13_MethodName **method_names)
{
  Pw13_MethodName **L = method_names;
  while (*L && (*L) != name)
    L = &(*L)->next;
  *L = (*L)->next;
  if (name->methods) {
    fprintf (stderr, "pw13_method_name_remove: not empty !\n");
    exit (1);
  }
  free (name);
}
