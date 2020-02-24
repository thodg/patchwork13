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
#include <string.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <pw13/pw13.h>
#include "toolbox.h"
#include "patchwork.h"
#include "paths.h"

int main (int argc, char **argv)
{
  Pw13_GtkToolbox *tb;
  Pw13_GtkPaths paths;
  gtk_init (&argc, &argv);
  glade_init ();

  paths.bin = argv[0];
  init_paths (&paths);

  pw13_dl_init ();
  tb = build_toolbox (&paths);
  if (!tb)
    return 1;
  if (argc == 2)
    pw13_gtk_open_file (tb, argv[1]);
  gtk_main();
  pw13_dl_exit ();
  return 0;
}
