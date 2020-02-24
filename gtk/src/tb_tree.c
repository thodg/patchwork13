/*
  pw13_gtk - A Patchwork13! GTK User Interface
  Copyright (C) 2005  Thomas de Grivel <billitch@yahoo.fr>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  FUCKABILITY, MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the GNU General Public License for more freakish details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  *

  tb_tree.c - Toolbox treeview. Contains patch classes

*/
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <pw13/pw13.h>
#include "tb_tree.h"


static void mk_tree_item (tb_tree_item item,
			  tb_tree_item parent,
			  const char *name)
{
  int name_len = strlen (name);
  item->parent = parent;
  item->name = (char*) malloc (name_len + 1);
  strcpy (item->name, name);
  if (parent) {
    item->path = (char*) malloc (strlen (parent->path) + name_len + 2);
    sprintf (item->path, "%s/%s", parent->path, name);
  }
  else {
    item->path = (char*) malloc (strlen (name) + 1);
    strcpy (item->path, name);
  }
}


static void tb_tree_add_folder (tb_tree t, tb_tree_item item)
{
  GtkTreeIter *pi;
  if (item->parent)
    pi = &item->parent->iter;
  else
    pi = NULL;
  gtk_tree_store_append (t->tree_store,
			 &item->iter,
			 pi);
  gtk_tree_store_set (t->tree_store,
		      &item->iter,
		      TB_TREE_NAME_COLUMN, item->name,
		      -1);
}

static void tb_tree_add_file (tb_tree t, tb_tree_item item)
{
  GtkTreeIter *pi;
  if (item->parent)
    pi = &(item->parent->iter);
  else
    pi = NULL;
  gtk_tree_store_append (t->tree_store,
			 &(item->iter),
			 pi);
  gtk_tree_store_set (t->tree_store,
		      &(item->iter),
		      TB_TREE_NAME_COLUMN, item->name,
		      -1);
}


static int tb_tree_is_duplicate (GtkTreeModel *model, tb_tree_item item)
{
  GtkTreeIter iter, *pi;
  if (!item->parent)
    pi = NULL;
  else
    pi = &item->parent->iter;
  if (gtk_tree_model_iter_children (model, &iter, pi) == FALSE)
    return 0;
  do {
    char *name;
    gtk_tree_model_get (model, &iter, TB_TREE_NAME_COLUMN,
			&name, -1);
    if (!strcmp (name, item->name))
      return 1;
  } while (gtk_tree_model_iter_next (model, &iter) == TRUE);
  return 0;
}


static void tb_tree_parse_dir_r (tb_tree t, tb_tree_item item)
{
  if (g_file_test (item->path, G_FILE_TEST_IS_DIR)) {
    const char * n;
    GDir * d = g_dir_open(item->path, 0,NULL);
    tb_tree_add_folder(t, item);
    while ((n = (const char*) g_dir_read_name(d))) {
      tb_tree_item_s ni;
      mk_tree_item (&ni, item, n);
      tb_tree_parse_dir_r (t, &ni);
    }
    g_dir_close (d);
  }
  else if (pw13_dl_is_patch_class (item->path)) {
    if (!pw13_classpath_of_filepath (item->path) &&
        !pw13_classpath_of_filepath (item->name) &&
	!tb_tree_is_duplicate (GTK_TREE_MODEL (t->tree_store), item)) {
      tb_tree_add_file (t, item);
    }
  }
}

void tb_tree_parse_dir (tb_tree t, const char *path)
{
  tb_tree_item_s item;
  mk_tree_item (&item, NULL, path);
  tb_tree_parse_dir_r (t, &item);
  /*
  gtk_tree_view_expand_all (GTK_TREE_VIEW (t->tree_view));
  */
}


void tb_tree_build_widget (tb_tree t)
{
  GtkCellRenderer *cell;
  GtkTreeViewColumn *column;

  t->scrolled_window = GTK_SCROLLED_WINDOW (gtk_scrolled_window_new (NULL,
								     NULL));
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (t->scrolled_window),
				  GTK_POLICY_AUTOMATIC,
				  GTK_POLICY_AUTOMATIC);

  t->tree_store = gtk_tree_store_new (TB_TREE_NB_COLUMNS,
				      G_TYPE_STRING,
				      G_TYPE_STRING,
				      G_TYPE_STRING);

  t->tree_view = gtk_tree_view_new ();
  gtk_scrolled_window_add_with_viewport (t->scrolled_window,
					t->tree_view);
  gtk_tree_view_set_model (GTK_TREE_VIEW(t->tree_view),
			   GTK_TREE_MODEL(t->tree_store));
 
  cell = gtk_cell_renderer_text_new();
  column = gtk_tree_view_column_new_with_attributes("NOM",
						    cell,
						    "text",0,
						    NULL);
  gtk_tree_view_append_column (GTK_TREE_VIEW (t->tree_view),
			       GTK_TREE_VIEW_COLUMN (column));    

  g_object_unref (G_OBJECT(t->tree_store));
  gtk_widget_show (t->tree_view);
}


