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

  patch.c  -  patch widget

*/
#include <stdlib.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <pw13/pw13.h>
#include "patch.h"
#include "patchwork.h"
#include "connect.h"
#include "input.h"
#include "output.h"
#include "xml_signals.h"
#include "drag_and_drop.h"
#include "paths.h"
#include "xml.h"


static GtkTargetEntry drag_patch_targets[] =
  { PW13_DRAG_PATCH_TARGET
  };


static void update_patch_size (Pw13_GtkPatch *p)
{
  GtkRequisition req;
  GtkAllocation alloc;
  gtk_widget_size_request (p->widget, &req);
  alloc.x = p->widget->allocation.x;
  alloc.y = p->widget->allocation.y;
  alloc.width = req.width;
  alloc.height = req.height;
  gtk_widget_size_allocate (p->widget, &alloc);
  gtk_widget_queue_draw (GTK_WIDGET (p->pw->layout));
}


void pw13_gtk_patch_expander_activate (GtkExpander *expander, Pw13_GtkPatch *p)
{
  if (gtk_expander_get_expanded (expander) == TRUE) {
    gtk_widget_show_all (p->open_box);
    printf ("patch %s open\n", p->p->name);
  }
  else {
    gtk_widget_hide_all (p->open_box);
    printf ("patch %s close\n", p->p->name);
  }
  update_patch_size (p);
}

/*
static void patch_drag_data_get (GtkWidget *widget, GdkDragContext *dc,
				 GtkSelectionData *selection_data,
				 guint info, guint time,
				 pw13_gtk_patch p)
{
  time = time;
  if (!widget || !dc)
    return;
  if (info == PW13_DRAG_PATCH) {
    char buf[16];
    sprintf (buf, "%x", (unsigned int) p);
    gtk_selection_data_set (selection_data, selection_data->target,
			    8, (guchar*) buf, strlen (buf));
  }
}
*/
static gboolean close_button_release (GtkWidget *widget,
				    GdkEventButton *event,
				    Pw13_GtkPatch *p)
{
widget = widget;
if((event->button == 1) && (p->p != p->pw->pw.master))
	{
	pw13_gtk_patch_destroy (p);
	return TRUE;
	}
return FALSE;
}


static gboolean patch_button_press (GtkWidget *widget,
				    GdkEventButton *event,
				    Pw13_GtkPatch *p)
{
  widget = widget;
  if ((event->button == 1) && p->pw) {
    int rx, ry;
    gdk_window_get_origin (p->widget->window, &rx, &ry);
    p->pw->moving_patch_dx = event->x_root - rx;
    p->pw->moving_patch_dy = event->y_root - ry;
    p->pw->moving_patch = p;
    return TRUE;
  }
  if ((event->button == 2) && (p->p != p->pw->pw.master)) {
    pw13_gtk_patch_destroy (p);
    return TRUE;
  }
 if((event->button == 3) && p->pw){
gtk_menu_popup (GTK_MENU(p->menu),NULL,NULL,NULL,NULL,
					event->button, event->time);
return TRUE;
}
  return FALSE;
}


static gboolean patch_button_release (GtkWidget *widget,
				      GdkEventButton *event,
				      Pw13_GtkPatch *p)
{
  widget = widget;
  if ((event->button == 1) && p->pw) {
    p->pw->moving_patch = NULL;
    return TRUE;
  }
  return FALSE;
}


static void patch_size_allocate (GtkWidget *widget,
				 GtkAllocation *allocation,
				 Pw13_GtkPatch *p)
{
  widget = widget;
  allocation = allocation;

  update_patch_size (p);
}


static void connect_patch_signals (Pw13_GtkPatch *p)
{
  /*
  g_signal_connect (G_OBJECT (p->handle), "drag_data_get",
		    G_CALLBACK (patch_drag_data_get), p);
  */
  g_signal_connect (G_OBJECT (p->handle), "button_press_event",
		    G_CALLBACK (patch_button_press), p);
  g_signal_connect (G_OBJECT (p->handle), "button_release_event",
		    G_CALLBACK (patch_button_release), p);
  g_signal_connect (G_OBJECT (p->close_patch_button),"button_release_event",
		    G_CALLBACK (close_button_release), p);
  g_signal_connect_after (G_OBJECT (p->expander), "activate",
			  G_CALLBACK (pw13_gtk_patch_expander_activate), p);
  g_signal_connect (G_OBJECT (p->main_box), "size-allocate",
		    G_CALLBACK (patch_size_allocate), p);
}

GtkMenu * build_patch_menu()
{
  GtkMenu * menu;
  GtkWidget * menu_items;
  menu = GTK_MENU (gtk_menu_new ());
  menu_items = gtk_menu_item_new_with_label("help");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu),menu_items);
  gtk_widget_show(menu_items);
  menu_items = gtk_menu_item_new_with_label("properties");
  gtk_menu_shell_append(GTK_MENU_SHELL(menu),menu_items);
  gtk_widget_show(menu_items);
  return menu;
}

static int build_patch_widget (Pw13_GtkPatch *p)
{
  p->xml = pw13_gtk_glade_xml (p->pw->paths, "pw13_patch.glade", "patch");
  if (!p->xml) {
    g_warning ("build_patch_widget: pw13_gtk_glade_xml () failed");
    return 0;
  }
  p->widget = glade_xml_get_widget (p->xml, "patch");
  p->main_box = glade_xml_get_widget (p->xml, "main_box");
  p->expander = glade_xml_get_widget (p->xml, "expander");
  p->title = GTK_LABEL (glade_xml_get_widget (p->xml, "title"));
  p->close_patch_button = glade_xml_get_widget (p->xml,"close_patch");
  p->handle = glade_xml_get_widget (p->xml, "handle");
  p->closed_box = glade_xml_get_widget (p->xml, "closed_box");
  p->open_box = glade_xml_get_widget (p->xml, "open_box");
  p->inputs_box = glade_xml_get_widget (p->xml, "inputs_box");
  p->outputs_box = glade_xml_get_widget (p->xml, "outputs_box");
  p->menu = build_patch_menu();
  connect_patch_signals (p);
  return 1;
}


void pw13_gtk_patch_build_controls (Pw13_GtkPatch *p)
{
  GtkWidget *separator;
  GtkBox *box;

  box = GTK_BOX (p->open_box);
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (box, separator, TRUE, TRUE, 0);
  if (!pw13_patch_method_call (p->p, "build_gtk_widget_open", box))
    gtk_widget_destroy (separator);

  box = GTK_BOX (p->closed_box);
  separator = gtk_vseparator_new ();
  gtk_box_pack_start (box, separator, TRUE, TRUE, 0);
  if (!pw13_patch_method_call (p->p, "build_gtk_widget_closed", box))
    gtk_widget_destroy (separator);
}


static void pw13_gtk_patch_add_methods (Pw13_GtkPatch *p)
{
  pw13_patch_method_add (p->p, "xml export", pw13_gtk_patch_xml_export, p);
}


Pw13_Result pw13_gtk_patch_init (Pw13_GtkPatch *p, Pw13_GtkPatchwork *pw)
{
  unsigned int i;
  printf ("pw13_gtk_init_patch  %p\n", p);   /* DEBUG */
  if (!p)
    exit (-1);
  /* pw13_patch_print_infos (p->p); */
  p->x = p->y = 0;
  p->p->custom_data = p;
  p->pw = pw;
  if (!build_patch_widget (p))
  {
    free (p);
    return PW13_ERROR;
  }
  gtk_label_set_text (p->title, p->p->name);
  for (i = 0; i < p->p->nb_inputs; i++)
    pw13_gtk_input_new (p->p->input + i, p);
  for (i = 0; i < p->p->nb_outputs; i++)
    pw13_gtk_output_new (p->p->output + i, p);

  gtk_drag_source_set (p->handle, GDK_BUTTON1_MASK,
		       drag_patch_targets, 1, GDK_ACTION_MOVE);
  pw13_gtk_patch_build_controls (p);
  gtk_widget_show_all (p->widget);

  pw13_gtk_patch_add_methods (p);
  printf ("/pw13_gtk_init_patch  %p\n\n", p);  /* DEBUG */
  return PW13_SUCCESS;
}


Pw13_GtkPatch * pw13_gtk_patch_new_of_classpath (Pw13_GtkPatchwork *pw,
						 const char *classpath)
{
  Pw13_GtkPatch *p;
  char *name;
  p = malloc (sizeof (Pw13_GtkPatch));
  if (!p)
    exit (-1);
  p->p = malloc (sizeof (Pw13_Patch));
  if (!p->p)
    exit (-1);

  name = pw13_unique_patch_name (&pw->pw, classpath);
  if (!name)
    return NULL;

  if (pw13_patch_of_classpath (p->p, classpath, name)
      != PW13_SUCCESS) {
    free (name);
    return NULL;
  }
  if (pw13_gtk_patch_init (p, pw) != PW13_SUCCESS) {
    free (name);
    pw13_patch_destroy (p->p);
    return NULL;
  }
  return p;
}


void pw13_gtk_patch_destroy (Pw13_GtkPatch *p)
{
  printf ("destroying patch '%s'\n", p->p->name);
  unsigned int ni = p->p->nb_inputs;
  while (ni--) {
    Pw13_Input *i = p->p->input + ni;
    pw13_gtk_disconnect_input (&p->pw->connections,
			       (Pw13_GtkInput*) i->custom_data);
  }
  unsigned int no = p->p->nb_outputs;
  while (no--) {
    Pw13_Output *o = p->p->output + no;
    Pw13_List *L = o->connected_to;
    while (L) {
      Pw13_Input *i;
      i = (Pw13_Input*) L->elem;
      pw13_gtk_disconnect_input (&p->pw->connections,
				 (Pw13_GtkInput*) i->custom_data);
      L = L->suiv;
    }
  }
  gtk_widget_destroy (p->widget);
  pw13_patch_destroy (p->p);
  if (p->pw)
    pw13_gtk_patchwork_redraw (p->pw);
}
