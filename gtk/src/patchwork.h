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

  patchwork.h  -  patchwork window

*/
#ifndef PW13_GTK_PATCHWORK_H_INCLUDED
#define PW13_GTK_PATCHWORK_H_INCLUDED

#include <gtk/gtk.h>
#include <gdk/gdk.h>
#include <glade/glade.h>
#include <pw13/pw13.h>
#include "types.h"

struct Pw13_GtkPatchwork
{
  Pw13_Patchwork pw;
  Pw13_GtkPatch *master;
  Pw13_List *connections;
  Pw13_GtkPaths *paths;
  GladeXML *xml;
  char *filepath, *filename;
  GtkWindow *window;
  GtkWidget *view_menu_item;
  GtkLayout *layout;
  GdkWindow *pwdraw;
  int new;
  int modified;
  int width, height;
  int x1,x2,y1,y2;
  GtkFileSelection *filesel;
  Pw13_GtkPatch *moving_patch;
  int moving_patch_dx, moving_patch_dy;
  int drag_highlight;
  int drag_drop;
  int drag_x, drag_y;
  GdkAtom drag_target;
  Pw13_GtkPlay *play;
  int must_close;
};

Pw13_GtkPatchwork * pw13_gtk_patchwork_new (Pw13_GtkPaths *paths);
void pw13_gtk_patchwork_destroy (Pw13_GtkPatchwork *pw);

void pw13_gtk_patchwork_set_filepath (Pw13_GtkPatchwork *p, const char *path);

void pw13_gtk_patchwork_redraw (Pw13_GtkPatchwork *pw);

void pw13_gtk_patchwork_add_patch (Pw13_GtkPatchwork *pw, Pw13_GtkPatch *p);
void pw13_gtk_patchwork_build_patches (Pw13_GtkPatchwork *pw);
void pw13_gtk_pw_file_save (Pw13_GtkPatchwork *pw);

#endif
/* ndef PW13_GTK_PATCHWORK_H_INCLUDED */
