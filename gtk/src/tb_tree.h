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

  tb_tree.h  -  toolbox tree header
  
*/
#ifndef PW13_GTK_TB_TREE_H_INCLUDED
#define PW13_GTK_TB_TREE_H_INCLUDED

#include <gtk/gtk.h>

enum {
  TB_TREE_NAME_COLUMN,
  TB_TREE_DESCRIPTION_COLUMN,
  TB_TREE_AUTHOR_COLUMN,
  TB_TREE_NB_COLUMNS
};

typedef struct tb_tree_s
{
  GtkScrolledWindow *scrolled_window;
  GtkWidget *tree_view;
  GtkTreeStore *tree_store;
} tb_tree_s, *tb_tree;


typedef struct tb_tree_item_s *tb_tree_item;
typedef struct tb_tree_item_s
{
  tb_tree_item parent;
  GtkTreeIter iter;
  char *name;
  char *path;
} tb_tree_item_s;


void tb_tree_build_widget (tb_tree t);
void tb_tree_parse_dir (tb_tree t, const char *path);


#endif
/* ndef PW13_GTK_TB_TREE_H_INCLUDED */
