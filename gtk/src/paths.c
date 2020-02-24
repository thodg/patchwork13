/*
  pw13_gtk - A Patchwork13! GTK+2 User Interface
  Copyright (C) 2005  Thomas de Grivel <billitch@yahoo.fr>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*
  
  main.c  -  pw13_gtk entry point
  
*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <pw13/pw13.h>

#ifdef WIN32
static char * dirname (char *path)
{
  char *c = strrchr (path, '\\');
  if (c)
    *c = 0;
  return path;
}
#else
#  include <libgen.h>
#endif

#include "paths.h"


#ifdef WIN32

static void init_paths_win32 (pw13_gtk_paths p)
{
  char *lib_dir = malloc (strlen (p->bindir) + 16);
  if (!lib_dir)
    exit (1);
  p->datadir = malloc (strlen (p->bindir) + 16);
  if (!p->datadir)
    exit (1);
  sprintf (lib_dir, "%s\\..\\lib", p->bindir);
  sprintf ((char*) p->datadir, "%s\\..", p->bindir);
  printf ("Setting pw13 library path to %s\n", lib_dir);
  pw13_set_library_path (lib_dir);
  free (lib_dir);
  printf ("pw13 library path is now %s\n", pw13_library_path ());
}

#else

static void init_paths_unix (Pw13_GtkPaths *p)
{
  p->datadir = PW13_GTK_DATA_DIR;
}

#endif /* def WIN32 */


void init_paths (Pw13_GtkPaths *p)
{
  p->bindir = dirname (pw13_strdup (p->bin));
#ifdef WIN32
  init_paths_win32 (p);
#else
  init_paths_unix (p);
#endif
}


char * pw13_gtk_data_path (Pw13_GtkPaths *paths, const char *file)
{
  char *r = malloc (strlen (paths->datadir) + strlen (file) + 4);
  if (!r)
    exit (1);
  sprintf (r, "%s/%s", paths->datadir, file);
  return r;
}


GladeXML * pw13_gtk_glade_xml (Pw13_GtkPaths *paths,
			       const char *file,
			       const char *root_widget)
{
  char *p = pw13_gtk_data_path (paths, file);
  GladeXML *xml = glade_xml_new (p, root_widget, NULL);
  free (p);
  return xml;
}
