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

  cluster_servers.h  -  Cluster servers list
  
*/
#ifndef PW13_GTK_CLUSTER_SERVERS_H_INCLUDED
#define PW13_GTK_CLUSTER_SERVERS_H_INCLUDED

#include <pthread.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <pw13_cluster/common/broadcast.h>
#include <pw13_cluster/client/servlist.h>
#include "types.h"

struct Pw13_GtkClusterServers
{
  GladeXML *xml;
  GtkWindow *window;
  GtkTreeView *treeview;
  GtkListStore *store;
  pthread_t bcast;
  pident servlist;
};

Pw13_GtkClusterServers * cluster_servers (Pw13_GtkToolbox *tb);


#endif
/* ndef PW13_GTK_CLUSTER_SERVERS_H_INCLUDED */
