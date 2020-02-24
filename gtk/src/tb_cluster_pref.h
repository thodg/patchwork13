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

  tb_cluster_pref.h  -  Cluster preferences
  
*/
#ifndef PW13_GTK_TB_CLUSTER_PREF_H_INCLUDED
#define PW13_GTK_TB_CLUSTER_PREF_H_INCLUDED

#include <gtk/gtk.h>
#include <glade/glade.h>
#include "paths.h"


void tb_cluster_pref (Pw13_GtkToolbox *toolbox);
void tb_server_path_browse (Pw13_GtkToolbox *toolbox);
void tb_client_path_browse (Pw13_GtkToolbox *toolbox);
void tb_cluster_pref_ok (Pw13_GtkToolbox *toolbox);
void tb_cluster_pref_cancel (Pw13_GtkToolbox *toolbox);


#endif // ndef PW13_GTK_TB_CLUSTER_PREF_H_INCLUDED
