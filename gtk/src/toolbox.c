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

  toolbox.c  -  main window for pw13_gtk

*/
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <pw13/pw13.h>
#include "toolbox.h"
#include "tb_tree.h"
//#include "tb_cluster_pref.h"
#include "patchwork.h"
#include "about.h"
#include "quit.h"
#include "xml_signals.h"
#include "drag_and_drop.h"
#include "paths.h"
#include "xml.h"
//#include "cluster_servers.h"


static GtkTargetEntry drag_patch_class_targets[] =
  { PW13_DRAG_TEXT_TARGET
  };

static gboolean tb_delete (Pw13_GtkToolbox *tb)
{
  pw13_gtk_quit_dialog (tb->paths);
  return TRUE;
}

static void tb_destroy ()
{
  gtk_main_quit ();
}

/*   view   */
static void tb_view_cluster_servers ()
{
}

static void tb_view_local_server ()
{
}

static void tb_view_menu_item_clicked (Pw13_GtkPatchwork *p)
{
  gtk_window_present (p->window);
}

static void tb_add_to_view_menu (Pw13_GtkToolbox *tb, Pw13_GtkPatchwork *p)
{
  p->view_menu_item = gtk_menu_item_new_with_label (p->filename);
  gtk_menu_shell_append (GTK_MENU_SHELL (tb->view_menu),
			 p->view_menu_item);
  g_signal_connect_swapped (G_OBJECT (p->view_menu_item), "activate",
			    G_CALLBACK (tb_view_menu_item_clicked),
			    (gpointer) p);
  gtk_widget_show (p->view_menu_item);
}

/* file */
static void tb_file_new (Pw13_GtkToolbox *toolbox)
{
  Pw13_GtkPatchwork *pw;
  char buf[32];
  sprintf (buf, "New patchwork %i", toolbox->patchwork_count++);
  pw = pw13_gtk_patchwork_new (toolbox->paths);
  pw13_gtk_patchwork_set_filepath (pw, buf);
  tb_add_to_view_menu (toolbox, pw);
  pw13_list_push (&toolbox->pw_list, pw);
}


Pw13_Result pw13_gtk_open_file (Pw13_GtkToolbox *toolbox, const char *path)
{
  Pw13_Import i;
  Pw13_GtkPatchwork *pw;
  printf ("pw13_gtk: Opening %s\n", path);

  if (pw13_import_from_file (&i, path) != PW13_SUCCESS) {
    fprintf (stderr, "Failed to import from %s\n", path);
    return PW13_ERROR;
  }
  pw = pw13_gtk_patchwork_new (toolbox->paths);
  pw13_gtk_patchwork_set_filepath (pw, path);

  pw13_patchwork_method_add (&pw->pw, "xml import patch",
		      pw13_gtk_patch_xml_import, pw);

  if (pw13_import_patchwork (&i, &pw->pw) != PW13_SUCCESS) {
    fprintf (stderr, "Failed import patchwork from %s\n", path);
    gtk_widget_destroy (GTK_WIDGET (pw->window));
    return PW13_ERROR;
  }
  pw13_import_end (&i);
  printf ("import end\n");
  /*
  pw13_gtk_pw_build_patches (pw);
  */
  tb_add_to_view_menu (toolbox, pw);
  pw13_list_push (&toolbox->pw_list, pw);

  return PW13_SUCCESS;
}


static void tb_file_open (Pw13_GtkToolbox *toolbox)
{
  GtkWidget *dialog;
  dialog = gtk_file_chooser_dialog_new ("Open patchwork",
				        toolbox->window,
					GTK_FILE_CHOOSER_ACTION_OPEN,
					GTK_STOCK_CANCEL,
					GTK_RESPONSE_CANCEL,
					GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
					NULL);
  if (gtk_dialog_run (GTK_DIALOG (dialog)) == GTK_RESPONSE_ACCEPT) {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dialog));
    pw13_gtk_open_file (toolbox, filename);
    g_free (filename);
  }
  gtk_widget_destroy (dialog);
}


static void tb_file_quit (Pw13_GtkToolbox *tb)
{
  tb_delete (tb);
}

/*   cluster   */
/*
static void tb_cluster_servers (Pw13_GtkToolbox *tb)
{
  if (tb->cluster_servers)
    gtk_window_present (tb->cluster_servers->window);
  else
    tb->cluster_servers = cluster_servers (tb);
}

static void tb_cluster_create_server ()
{
}
*/

/*   help   */
static void tb_help_about ()
{
  about_pw13_gtk ();
}

/* treeview */

static char * get_iter_path (GtkTreeModel *model,
			     GtkTreeIter *iter)
{
  char *res = NULL;
  int len = 0, namelen;
  char *pathstring = gtk_tree_model_get_string_from_iter (model, iter);
  char *c = strchr (pathstring, ':');
  do {
    gchar *name;
    GtkTreeIter i;
    c = strchr (c+1, ':');
    if (c)
      *c = 0;
    gtk_tree_model_get_iter_from_string (model, &i, pathstring);
    gtk_tree_model_get (model, &i, TB_TREE_NAME_COLUMN, &name, -1);
    namelen = strlen (name);
    res = realloc (res, len + namelen + 2);
    if (!res)
      exit (1);
    if (len)
      res[len++] = '/';
    strcpy (res + len, name);
    len += namelen;
    if (c)
      *c = ':';
  } while (c);
  free (pathstring);
  return res;
}

static void treeview_drag_data_get (GtkWidget *widget, GdkDragContext *dc,
				    GtkSelectionData *selection_data,
				    guint info, guint time,
				    Pw13_GtkToolbox *tb)
{
  time = time;
/*
  printf ("treeview drag_data_get signal received\n");
*/
  if (!widget || !tb || !dc)
    return;
  if (info == PW13_DRAG_TEXT) {
    GtkTreeIter iter;
    char *classpath;
    GtkTreeSelection *ts;
    ts = gtk_tree_view_get_selection (GTK_TREE_VIEW(tb->tree.tree_view));
    gtk_tree_selection_get_selected (ts, NULL, &iter);
    classpath = get_iter_path (GTK_TREE_MODEL (tb->tree.tree_store),
			       &iter);
    if (classpath) {
/*
      printf ("DnD classpath : %s\n", classpath);
*/
      fflush (stdout);
      gtk_selection_data_set (selection_data, selection_data->target,
			      8, (guchar*) classpath,
			      strlen (classpath));
      free (classpath);
    }
  }
}


static void connect_toolbox_signals (Pw13_GtkToolbox *tb)
{
  /* signals */
  /*   toolbox   */
  g_signal_connect_swapped (tb->window, "delete_event",
			    G_CALLBACK (tb_delete), tb);
  g_signal_connect_swapped (tb->window, "destroy_event",
			    G_CALLBACK (tb_destroy), tb);
  /* treeview */
  g_signal_connect (tb->tree.tree_view, "drag_data_get",
		    G_CALLBACK (treeview_drag_data_get), tb);
  /*   file   */
  xml_connect_s (tb->xml, "file_new", "activate", tb_file_new, tb);
  xml_connect_s (tb->xml, "file_open", "activate", tb_file_open, tb);
  xml_connect_s (tb->xml, "file_quit", "activate", tb_file_quit, tb);
  /*   cluster   */
  /*
  xml_connect_s (tb->xml, "cluster_create_server", "activate",
		 tb_cluster_create_server, tb);
  xml_connect_s (tb->xml, "cluster_servers", "activate",
		 tb_cluster_servers, tb);
  xml_connect_s (tb->xml, "cluster_pref", "activate", tb_cluster_pref, tb);
  */
  /*   view   */
  xml_connect_s (tb->xml, "view_cluster_servers", "activate",
		 tb_view_cluster_servers, tb);
  xml_connect_s (tb->xml, "view_local_server", "activate",
		 tb_view_local_server, tb);
  /*   help   */
  xml_connect_s (tb->xml, "help_about", "activate", tb_help_about, tb);
}


Pw13_GtkToolbox * build_toolbox (Pw13_GtkPaths *paths)
{
  Pw13_GtkToolbox *tb = malloc (sizeof (Pw13_GtkToolbox));
  if (!tb)
    exit (-1);

  tb->paths = paths;
  tb->patchwork_count = 1;
  tb->pw_list = NULL;
  tb->cluster_server_path = g_strdup ("pw13_cluster_s");
  tb->cluster_client_path = g_strdup ("pw13_cluster_c");
  tb->cluster_servers = NULL;

  tb->xml = pw13_gtk_glade_xml (paths, "pw13_toolbox.glade", NULL);

  if (!tb->xml) {
    g_warning ("build_toolbox: glade_xml_new() failed");
    return NULL;
  }

  tb->window = GTK_WINDOW (glade_xml_get_widget (tb->xml, "toolbox"));
  tb->tree_box = glade_xml_get_widget (tb->xml, "tree_box");
  tb->view_menu = glade_xml_get_widget (tb->xml, "view_menu");

  tb_tree_build_widget (&tb->tree);
  tb_tree_parse_dir (&tb->tree, pw13_library_path ());
  gtk_box_pack_start (GTK_BOX(tb->tree_box),
		      GTK_WIDGET (tb->tree.scrolled_window),
		      TRUE, TRUE, 0);
  gtk_widget_show (GTK_WIDGET (tb->tree.scrolled_window));
  gtk_tree_view_enable_model_drag_source
    (GTK_TREE_VIEW (tb->tree.tree_view), GDK_BUTTON1_MASK,
     drag_patch_class_targets, 1, GDK_ACTION_COPY);
  
  connect_toolbox_signals (tb);
  return tb;
}
