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

  paths.h  -  paths
  
*/
#ifndef PW13_GTK_PATHS_H_INCLUDED
#define PW13_GTK_PATHS_H_INCLUDED

#include <glade/glade.h>
#include "types.h"

struct Pw13_GtkPaths
{
  const char *bin;
  const char *bindir;
  const char *datadir;
};

/* Initialise paths.
   paths->bin must be set to the program path (argv[0]) */
void init_paths (Pw13_GtkPaths *paths);

/* Builds the path of a file in the data dir */
char * pw13_gtk_data_path (Pw13_GtkPaths *paths, const char *file);

/* Load a glade xml file from the data dir */
GladeXML * pw13_gtk_glade_xml (Pw13_GtkPaths *paths,
			       const char *file,
			       const char *root_widget);


#endif
/* ndef PW13_GTK_PATHS_H_INCLUDED */
