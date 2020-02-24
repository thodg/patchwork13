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
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA

*

  connect.c  -  output -> input connections

*/
#include <pw13/pw13.h>
#include "connect.h"
#include "output.h"
#include "input.h"
#include "patchwork.h"


void pw13_gtk_get_check_pos (int *x, int *y, GtkWidget *main, GtkWidget *slave)
{
  int xt=0, yt=0;
  GtkWidget *temp;

  temp = slave;

  while (temp != main)
  {
    xt += temp->allocation.x;
    yt += temp->allocation.y;
    temp = gtk_widget_get_parent (temp);
  }
  *x = xt;
  *y = yt;
}


void pw13_gtk_disconnect_input (Pw13_List **L,
				Pw13_GtkInput *i)
{
  while (*L) {
    Pw13_GtkConnect *c = (Pw13_GtkConnect*) (*L)->elem;
    if (c->i == i) {
      Pw13_List *tmp = *L;
      *L = (*L)->suiv;
      free (c);
      free (tmp);
      return;
    }
    L = &(*L)->suiv;
  }
}


Pw13_GtkConnect * pw13_gtk_connect (Pw13_GtkOutput *output,
				    Pw13_GtkInput *input,
				    Pw13_GtkPatchwork *pw)
{
  Pw13_GtkConnect *c = malloc (sizeof (Pw13_GtkConnect));
  if (!c)
    exit (1);
  c->o = output;
  c->i = input;
  c->active = 1;
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (c->o->checkbox), TRUE);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (c->i->checkbox), TRUE);
  if (pw) {
    pw13_gtk_disconnect_input (&pw->connections, input);
    pw->connections = pw13_list (c, pw->connections);
    gtk_widget_queue_draw (GTK_WIDGET (pw->layout));
  }
  printf ("connected %s to %s\n", c->o->o->name, c->i->i->name);
  return c;
}
