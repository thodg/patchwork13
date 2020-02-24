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

  names.c  -  functions related to patch names

*/
#include <stdlib.h>
#include <string.h>
#include "types.h"
#include "list.h"
#include "patchwork.h"
#include "names.h"


Pw13_Patch * pw13_find_patch (Pw13_Patchwork *pw, const char *name)
{
  Pw13_List *L = pw->patches;
  while (L) {
    Pw13_Patch *p = (Pw13_Patch*) L->elem;
    if (!strcmp (p->name, name))
      return p;
    L = L->suiv;
  }
  return NULL;
}


char * pw13_unique_patch_name (Pw13_Patchwork *pw, const char *classpath)
{
  char *name, *n;
  const char *classname = strrchr (classpath, '/');
  int classnamelen;
  unsigned int i;
  if (classname)
    classname++;
  else
    classname = classpath;
  classnamelen = strlen (classname);
  name = malloc (classnamelen + 16);
  if (!name)
    exit (1);
  strcpy (name, classname);
  n = name + classnamelen;
  *(n++) = ' ';
  for (i=1; i>0; i++) {
    sprintf (n, "%u", i);
    if (!pw13_find_patch (pw, name))
      return name;
  }
  free (name);
  return NULL;
}


Pw13_Input * pw13_find_input (Pw13_Patch *p, const char *name)
{
  if (p) {
    int i = p->nb_inputs;
    while (i--) {
      if (!strcmp (name, p->input[i].name))
	return p->input + i;
    }
  }
  return NULL;
}


Pw13_Output * pw13_find_output (Pw13_Patch *p, const char *name)
{
  if (p) {
    int i = p->nb_outputs;
    while (i--) {
      if (!strcmp (name, p->output[i].name))
	return p->output + i;
    }
  }
  return NULL;
}
