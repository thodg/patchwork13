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

  input.c  -  gtk input widget

*/
#include <string.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include "input.h"
#include "output.h"
#include "connect.h"
#include "patch.h"
#include "patchwork.h"
#include "drag_and_drop.h"

static GtkTargetEntry drag_to_input_targets[] = {PW13_DRAG_OUTPUT_TARGET};
static GtkTargetEntry drag_from_input_targets[] = {PW13_DRAG_INPUT_TARGET};

static void input_reconnect (Pw13_GtkInput	*i)
{
  Pw13_GtkConnect *c = NULL;
  Pw13_List *L = i->p->pw->connections;

  while (L && (c = (Pw13_GtkConnect*) L->elem)->i != i)
    L = L->suiv;
  if (L && c->o->active) {
    printf("input reconnect \n");
    pw13_connect (c->o->o, i->i);
  }
}

/* signal callbacks */
static void input_checkbox_toggled (GtkButton		*checkbox,
				    Pw13_GtkInput	*i)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON(checkbox)) == TRUE) {
    i->active = 1;
    input_reconnect (i);
  }
  else {
    i->active = 0;
    if (i->i->from)
      pw13_disconnect (i->i);
  }
  gtk_widget_queue_draw (GTK_WIDGET (i->p->pw->layout));
}

/* drag source */
static void input_drag_data_get (GtkWidget *widget,
				 GdkDragContext *dc,
				 GtkSelectionData *sd,
				 guint info, guint time,
				 Pw13_GtkInput *i)
{
  time = time;
  /*
  printf ("input drag_data_get signal received\n");
  */
  if (!widget || !dc)
    return;
  if (info == PW13_DRAG_INPUT) {
    char buf[16];
    sprintf (buf, "%x", (unsigned int) i);
    printf ("input data_set: %s\n", buf);
    gtk_selection_data_set (sd, sd->target, 8, (guchar*) buf, strlen (buf));
  }
}

/* drag dest */
static gboolean input_drag_motion (GtkWidget *widget,
				    GdkDragContext *dc,
				    gint x, gint y, guint time,
				    Pw13_GtkInput *i)
{
  GtkTargetList *targetlist;
  /*
  printf ("input drag motion\n");
  */
  targetlist = gtk_drag_dest_get_target_list (widget);
  i->drag_target = gtk_drag_dest_find_target (widget, dc, targetlist);
  if (i->drag_target == GDK_NONE)
    gdk_drag_status (dc, 0, time);
  else {
    i->drag_drop = 0;
    gtk_drag_get_data (widget, dc, i->drag_target, time);
  }
  x = x;
  y = y;
  return TRUE;
}


static gboolean input_drag_drop (GtkWidget *widget, GdkDragContext *dc,
				 gint x, gint y, guint time,
				 Pw13_GtkInput *i)
{
  /*
  printf ("input drag_drop signal received\n");
  */
  i->drag_drop = 1;
  gtk_drag_get_data (widget, dc, i->drag_target, time);
  x = x;
  y = y;
  return TRUE;
}


static void input_drag_data_received (GtkWidget *widget,
				      GdkDragContext *dc,
				      gint x, gint y,
				      GtkSelectionData *sd,
				      guint info, guint time,
				      Pw13_GtkInput *i)
{
  x = x;
  y = y;
  widget = widget;
  /*
    printf ("input drag data received !\n");
  */
  if (!sd->data || sd->length < 0) {
    printf ("invalid drag data\n");
    return;
  }
  else if (info == PW13_DRAG_OUTPUT) {
    Pw13_GtkOutput *o;
    union {
      Pw13_GtkOutput **o;
      unsigned int *ui;
    } po;
    po.o = &o;
    /*
      printf ("addr: %s\n", addr);
    */
    if ((sscanf ((char*) sd->data, "%x", po.ui) != 1) || !o) {
      printf ("NULL data !\n");
      if (i->drag_drop)
	gtk_drag_finish (dc, FALSE, FALSE, time);
    }
    else if (pw13_can_connect (o->o, i->i)) {
      if (!i->drag_drop) {
	gdk_drag_status (dc, GDK_ACTION_LINK, time);
	if (!i->drag_highlight) {
	  i->drag_highlight = 1;
	  gtk_drag_highlight (widget);
	}
      }
      else {
	if (pw13_connect (o->o, i->i) == PW13_SUCCESS) {
	  pw13_gtk_connect (o, i, o->p->pw);
	  gtk_drag_finish (dc, TRUE, FALSE, time);
	}
	else
	  gtk_drag_finish (dc, FALSE, FALSE, time);
      }
    }
    else {
      if (!i->drag_drop)
	gdk_drag_status (dc, 0, time);
      else {
	printf ("can't connect : %s\n", o->o->name);
	gtk_drag_finish (dc, FALSE, FALSE, time);
      }
    }
  }
  if (i->drag_drop) {
    if (i->drag_highlight) {
      i->drag_highlight = 0;
      gtk_drag_unhighlight (widget);
    }
    i->drag_drop = 0;
    i->drag_target = GDK_NONE;
  }
}

void input_drag_leave (GtkWidget *widget, GdkDragContext *dc,
		       guint time, Pw13_GtkInput *i)
{
  dc = dc;
  time = time;
  if (i->drag_highlight) {
    i->drag_highlight = 0;
    gtk_drag_unhighlight (widget);
  }
}

/*
static void input_drag_end (GtkWidget *widget, GdkDragContext *dc,
                            pw13_gtk_input i)
{
  printf ("input drag_end signal\n");
  dc = dc;
}
*/

static void connect_input_signals (Pw13_GtkInput *i)
{
  g_signal_connect_after (G_OBJECT (i->checkbox), "toggled",
			  G_CALLBACK (input_checkbox_toggled), i);
  /* drag source */
  g_signal_connect (G_OBJECT (i->checkbox), "drag_data_get",
		    G_CALLBACK (input_drag_data_get), i);
  /* drag dest */
  g_signal_connect (i->checkbox, "drag_motion",
		    G_CALLBACK (input_drag_motion), i);
  g_signal_connect (i->checkbox, "drag_drop",
		    G_CALLBACK (input_drag_drop), i);
  g_signal_connect (i->checkbox, "drag_data_received",
		    G_CALLBACK (input_drag_data_received), i);
  g_signal_connect (i->checkbox, "drag_leave",
		    G_CALLBACK (input_drag_leave), i);
  /*g_signal_connect (i->checkbox, "drag_end",
    G_CALLBACK (input_drag_end), i);*/
}


Pw13_GtkInput * pw13_gtk_input_new (Pw13_Input *input, Pw13_GtkPatch *p)
{
  Pw13_GtkInput *i = malloc (sizeof (Pw13_GtkInput));
  if (!i)
    exit (1);
  i->i = input;
  i->i->custom_data = i;
  i->active = 0;
  i->drag_highlight = 0;
  i->widget = gtk_hbox_new (FALSE, 1);
  i->checkbox = GTK_WIDGET (gtk_check_button_new ());
  i->name = gtk_label_new (input->name);
  i->type = gtk_label_new (input->data_type->name);
  gtk_box_pack_start (GTK_BOX (i->widget), i->checkbox, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (i->widget), i->name, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (i->widget), gtk_label_new("  ("),FALSE,FALSE,0);
  gtk_box_pack_start (GTK_BOX (i->widget), i->type, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (i->widget), gtk_label_new (")"), FALSE,FALSE,0);
  i->p = p;
  if (p)
    gtk_box_pack_start (GTK_BOX (p->inputs_box), i->widget, TRUE, TRUE, 0);
  gtk_widget_show_all (i->widget);

  gtk_drag_source_set (i->checkbox, GDK_BUTTON1_MASK,
		       drag_from_input_targets, 1, GDK_ACTION_LINK);
  gtk_drag_dest_set (i->checkbox, 0, drag_to_input_targets, 1,
		     GDK_ACTION_LINK);

  connect_input_signals (i);
  /*
  printf ("/new_pw13_gtk_input: input %p\n", input);
  */
  return i;
}
