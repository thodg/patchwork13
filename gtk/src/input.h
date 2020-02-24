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

  input.h  -  gtk input widget

*/
#ifndef PW13_GTK_INPUT_H_INCLUDED
#define PW13_GTK_INPUT_H_INCLUDED

#include <gtk/gtk.h>
#include <pw13/pw13.h>
#include "types.h"

struct Pw13_GtkInput
{
  Pw13_Input *i;
  Pw13_GtkPatch *p;
  GtkWidget *widget;
  GtkWidget *checkbox;
  GtkWidget *name;
  GtkWidget *type;
  int x, y;
  int active;
  int drag_highlight;
  int drag_drop;
  GdkAtom drag_target;
};

Pw13_GtkInput * pw13_gtk_input_new (Pw13_Input *input, Pw13_GtkPatch *p);


#endif
/* ndef PW13_GTK_PATCH_H_INCLUDED */
