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

  output.c  -  gtk output widget

*/
#include <string.h>
#include <gtk/gtk.h>
#include <stdio.h>
#include "output.h"
#include "input.h"
#include "drag_and_drop.h"
#include "patch.h"
#include "patchwork.h"
#include "connect.h"

static GtkTargetEntry drag_from_output_targets[] = {PW13_DRAG_OUTPUT_TARGET};
static GtkTargetEntry drag_to_output_targets[] = {PW13_DRAG_INPUT_TARGET};


/* signal callbacks */
static void output_drag_data_get (GtkWidget *widget,
				  GdkDragContext *dc,
				  GtkSelectionData *sd,
				  guint info, guint time,
				  Pw13_GtkOutput *o)
{
  time = time;
  /*
  printf ("output drag_data_get signal received\n");
  */
  if (!widget || !dc)
    return;
  if (info == PW13_DRAG_OUTPUT) {
    char buf[16];
    sprintf (buf, "%x", (unsigned int) o);
    /*
    printf ("output data_set: %s\n", buf);
    */
    gtk_selection_data_set (sd, sd->target, 8, (guchar*) buf, strlen (buf));
  }
}

static void output_reconnect (Pw13_GtkOutput *o)
{
  Pw13_List *L = o->p->pw->connections;

  while (L) {
    Pw13_GtkConnect *c = (Pw13_GtkConnect*) L->elem;
    if (c->o == o && c->i->active) {
      printf("output reconnect \n");
      pw13_connect (o->o, c->i->i);
    }
    L = L->suiv;
  }
}

static void output_checkbox_toggled (GtkButton *checkbox, Pw13_GtkOutput *o)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(checkbox)) == TRUE) {
    o->active = 1;
    output_reconnect (o);
  }
  else {
    Pw13_List *L = o->o->connected_to;
    Pw13_Input *i;
    while (L) {
      printf ("output disconnect \n");
      fflush (stdout);
      i = (Pw13_Input*) L->elem;
      pw13_disconnect (i);
      L = L->suiv;
    }
    o->active = 0;
  }
  gtk_widget_queue_draw (GTK_WIDGET (o->p->pw->layout));
}

static void connect_output_signals (Pw13_GtkOutput *o)
{
  g_signal_connect (G_OBJECT (o->checkbox), "drag_data_get",
		    G_CALLBACK (output_drag_data_get), o);
  g_signal_connect_after (G_OBJECT (o->checkbox), "toggled",
			  G_CALLBACK (output_checkbox_toggled), o);
}


Pw13_GtkOutput * pw13_gtk_output_new (Pw13_Output *output, Pw13_GtkPatch *p)
{
  Pw13_GtkOutput *o = malloc (sizeof (Pw13_GtkOutput));
  if (!o)
    exit (1);
  o->o = output;
  o->o->custom_data = o;
  o->widget = gtk_hbox_new (FALSE, 1);
  o->name = gtk_label_new (output->name);
  o->type = gtk_label_new (output->data_type->name);
  o->checkbox = gtk_check_button_new ();
  gtk_box_pack_end (GTK_BOX (o->widget), o->checkbox, FALSE, FALSE, 0);
  gtk_box_pack_end (GTK_BOX (o->widget), o->name, FALSE, FALSE, 0);
  gtk_box_pack_end (GTK_BOX (o->widget), gtk_label_new(")  "), FALSE, FALSE,0);
  gtk_box_pack_end (GTK_BOX (o->widget), o->type, FALSE, FALSE, 0);
  gtk_box_pack_end (GTK_BOX (o->widget), gtk_label_new ("("), FALSE,FALSE,0);
  gtk_widget_show_all (o->widget);
  o->p = p;
  if (p)
    gtk_box_pack_start (GTK_BOX (p->outputs_box), o->widget, TRUE, TRUE, 0);

  gtk_drag_source_set (o->checkbox, GDK_BUTTON1_MASK,
		       drag_from_output_targets, 1, GDK_ACTION_LINK);
  gtk_drag_dest_set (o->checkbox, 0, drag_to_output_targets, 1,
		     GDK_ACTION_LINK);
  connect_output_signals (o);
  return o;
}
