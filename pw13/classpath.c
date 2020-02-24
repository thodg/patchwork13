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

  classpath.c  -  loading of patch classes through classpath

*/
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "list.h"
#include "patchwork.h"
#include "dl.h"
#include "patch_class.h"
#include "classpath.h"
#include "str.h"
#include "library_path.h"
#include "pw13-config.h"


Pw13_PatchClass *
pw13_find_classpath (const char *classpath)
{
  Pw13_List *cl = pw13_loaded_patch_classes;
  while (cl) {
    Pw13_PatchClass *pc = (Pw13_PatchClass*) cl->elem;
    if (!strcmp (classpath, pc->classpath))
      return pc;
    cl = cl->suiv;
  }
  return NULL;
}

Pw13_PatchClass *
pw13_load_classpath (const char *path)
{
  Pw13_PatchClass *pc = NULL;
  const char *libpath = pw13_library_path ();
  char *buf = (char*) malloc (strlen (libpath)
			      + strlen (path)
			      + 16);
  if (!buf)
    exit (-1);
  sprintf (buf, "%s/%s.pw13p", libpath, path);
  pc = pw13_dl_patch_class (buf);
  if (pc)
    pc->classpath = pw13_strdup (path);
  free (buf);
  return pc;
}

Pw13_PatchClass *
pw13_register_classpath (const char *classpath)
{
  Pw13_PatchClass *pc = pw13_find_classpath (classpath);
  if (!pc)
    pc = pw13_load_classpath (classpath);
  return pc;
}


Pw13_Result
pw13_patch_of_classpath (Pw13_Patch *p, const char *classpath, const char *name)
{
  Pw13_PatchClass *pc = pw13_register_classpath (classpath);
  if (pc)
    return pw13_make_patch_of_class (p, pc, name);
  return pw13_error ("Failed to register classpath \"%s\"", classpath);
}


int pw13_classpath_of_filepath (char *path)
{
  int r = 0;
  char *dot = path;
  while (*dot)
    dot++;
  while (!r && dot > path) {
    while (dot > path && *dot != '.')
      dot--;
    if (!strcmp (dot, ".pw13p"))
      r = 1;
    *dot = 0;
  }
  return !r;
}
