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

  dl.c  -  dynamic loading of patches from .pw13p files

*/
#include <stdlib.h>
#include <string.h>
#include <ltdl.h>
#include "dl.h"
#include "patchwork.h"
#include "error.h"
#include "str.h"

void
pw13_dl_init (void)
{
  lt_dlinit ();
}

void
pw13_dl_exit (void)
{
  lt_dlexit ();
}


Pw13_PatchClass *
pw13_dl_patch_class (const char *path)
{
  lt_dlhandle h;
  Pw13_PatchClass *pc;

  if ( ! (h = lt_dlopenext (path)) ) {
    pw13_error ("dl_patch_class: lt_dlopenext '%s' failed", path);
    return NULL;
  }
  if ( ! (pc = (Pw13_PatchClass*) malloc (sizeof (Pw13_PatchClass))) )
    exit (-1);
  pc->init = (Pw13_PatchConstructor*) lt_dlsym (h, "pw13_dl_patch_class_init");
  if (!pc->init) {
    pw13_error ("pw13_dl_class: patch init not found !");
    free (pc);
    lt_dlclose (h);
    return NULL;
  }
  pc->filename = pw13_strdup (path);
  pc->classpath = NULL;
  pw13_patch_class_reg (pc);
  return pc;
}


int pw13_dl_is_patch_class (const char *path)
{
  lt_dlhandle h;
  if (!(h = lt_dlopenext (path))) {
    return 0;
  }
  if (!lt_dlsym (h, "pw13_dl_patch_class_init")) {
    lt_dlclose (h);
    return 0;
  }
  lt_dlclose (h);
  return 1;
}
