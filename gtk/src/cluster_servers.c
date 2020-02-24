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

  cluster_servers.c  -  Cluster servers list
  
*/
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <pw13_cluster/client/broadcast.h>
#include <pw13_cluster/client/servlist.h>
#include "toolbox.h"
#include "cluster_servers.h"
#include "paths.h"

enum {
  ADDRESS_COL,
  SCORE_COL,
  CONNECTED_COL,
  NB_COL
};


/* add server to treeview */
static void add_server (pident elt, void *param)
{
  Pw13_GtkClusterServers *cs = (Pw13_GtkClusterServers*) param;
  GtkTreeIter iter;
  gtk_list_store_append (cs->store, &iter);  /* Acquire an iterator */
  gtk_list_store_set (cs->store, &iter,
		      ADDRESS_COL, elt->adresse,
		      SCORE_COL, g_strdup_printf ("%u", elt->score),
		      CONNECTED_COL, FALSE,
		      -1);
}

/* broadcast thread */
static void * bcast_thread (void *param)
{
  Pw13_GtkClusterServers *cs = (Pw13_GtkClusterServers*) param;
  pw13cc_Bcast (&cs->servlist, add_server, cs);
  return NULL;
}


/* signal callbacks */
/* window */
static void cluster_servers_destroy (Pw13_GtkToolbox *tb)
{
  pthread_join (tb->cluster_servers->bcast, NULL);
  free (tb->cluster_servers);
  tb->cluster_servers = NULL;
}

/* toolbar */

static void cluster_servers_refresh (Pw13_GtkClusterServers *cs)
{
  pthread_create (&cs->bcast, NULL, bcast_thread, cs);  
}

static void cluster_servers_block (Pw13_GtkClusterServers *cs)
{
  cs = cs;
}

/* treeview */
/* message */
static void cluster_servers_send_message (Pw13_GtkClusterServers *cs)
{
  cs = cs;
}


/* create list */

void cs_build_list_widget (Pw13_GtkClusterServers *cs)
{
  GtkTreeViewColumn *column;
  GtkCellRenderer *renderer;

  cs->servlist = NULL;
  cs->store = gtk_list_store_new (NB_COL,
				  G_TYPE_STRING,   /* address   */
				  G_TYPE_INT,      /* score     */
				  G_TYPE_BOOLEAN); /* connected */
  gtk_tree_view_set_model (cs->treeview, GTK_TREE_MODEL (cs->store));
  g_object_unref (G_OBJECT (cs->store));

  /* address */
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Address",
						     renderer,
						     "text", ADDRESS_COL,
						     NULL);
  gtk_tree_view_append_column (cs->treeview, column);
  /* score */
  renderer = gtk_cell_renderer_text_new ();
  column = gtk_tree_view_column_new_with_attributes ("Score",
						     renderer,
						     "text", SCORE_COL,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (cs->treeview), column);
  /* connected */
  renderer = gtk_cell_renderer_toggle_new ();
  column = gtk_tree_view_column_new_with_attributes ("Connected",
						     renderer,
						     "radio", CONNECTED_COL,
						     NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (cs->treeview), column);
}


/* create window */

Pw13_GtkClusterServers * cluster_servers (Pw13_GtkToolbox *tb)
{
  Pw13_GtkClusterServers *cs =
    malloc (sizeof (Pw13_GtkClusterServers));
  if (!cs)
    exit (-1);
  
  cs->xml = pw13_gtk_glade_xml (tb->paths, "pw13_cluster_servers.glade", NULL);
  if (!cs->xml) {
    g_warning ("cluster_servers: glade_xml_new() failed");
    return NULL;
  }
  cs->window = GTK_WINDOW (glade_xml_get_widget (cs->xml, "cluster_servers"));
  cs->treeview = GTK_TREE_VIEW (glade_xml_get_widget (cs->xml, "treeview"));

  cs_build_list_widget (cs);

  /* signals */
  /* window */
  g_signal_connect_swapped (cs->window, "destroy",
			    G_CALLBACK (cluster_servers_destroy), tb);  
  /*   toolbar   */
  g_signal_connect_swapped (glade_xml_get_widget (cs->xml, "refresh"),
			    "clicked", G_CALLBACK (cluster_servers_refresh),
			    cs);
  g_signal_connect_swapped (glade_xml_get_widget (cs->xml, "block"),
			    "clicked", G_CALLBACK (cluster_servers_block),
			    cs);
  /*   messages   */
  g_signal_connect_swapped (glade_xml_get_widget (cs->xml, "send_message"),
			    "clicked",
			    G_CALLBACK (cluster_servers_send_message), cs);

  pthread_create (&cs->bcast, NULL, bcast_thread, cs);  
  return cs;
}
