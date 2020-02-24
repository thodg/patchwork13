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

  toolbox.h  -  toolbox header
  
*/
#ifndef PW13_GTK_TOOLBOX_H_INCLUDED
#define PW13_GTK_TOOLBOX_H_INCLUDED

#include <gtk/gtk.h>
#include <pw13/pw13.h>
#include "tb_tree.h"
#include "types.h"

struct Pw13_GtkToolbox
{
  Pw13_GtkPaths *paths;
  GladeXML *xml;
  GtkWindow *window;
  GtkWidget *tree_box;
  GtkWidget *view_menu;
  tb_tree_s tree;
  int patchwork_count;
  Pw13_List *pw_list;
  char *cluster_server_path;
  char *cluster_client_path;
  Pw13_GtkClusterServers *cluster_servers;
};

Pw13_GtkToolbox * build_toolbox (Pw13_GtkPaths * paths);

Pw13_Result pw13_gtk_open_file (Pw13_GtkToolbox *toolbox, const char *path);


#endif
/* ndef PW13_GTK_TOOLBOX_H_INCLUDED */
