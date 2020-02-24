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

  patch.h  -  patchwork window

*/
#ifndef PW13_GTK_PATCH_H_INCLUDED
#define PW13_GTK_PATCH_H_INCLUDED

#include <gtk/gtk.h>
#include <glade/glade.h>
#include <pw13/pw13.h>
#include "types.h"

struct Pw13_GtkPatch
{
  Pw13_Patch *p;
  Pw13_GtkPatchwork *pw;
  GladeXML *xml;
  GtkWidget *widget;
  GtkLabel *title;
  GtkWidget *handle;
  GtkWidget *expander;
  GtkWidget *main_box, *closed_box, *open_box;
  GtkWidget *inputs_box, *outputs_box;
  GtkWidget *close_patch_button;
  GtkMenu *menu;
  int x, y;
};

/* create a gtk patch widget from a pw13_patch */
Pw13_Result pw13_gtk_patch_init (Pw13_GtkPatch *p, Pw13_GtkPatchwork *pw);

/* create and init a patch of a given classpath */
Pw13_GtkPatch * pw13_gtk_patch_new_of_classpath (Pw13_GtkPatchwork *pw,
						 const char *classpath);

/* create a master patch widget
pw13_gtk_patch pw13_gtk_new_master_patch (pw13_gtk_pw pw);
*/

void pw13_gtk_patch_expander_activate (GtkExpander *expander,
				       Pw13_GtkPatch *p);

void pw13_gtk_patch_destroy (Pw13_GtkPatch *p);

#endif
/* ndef PW13_GTK_PATCH_H_INCLUDED */
