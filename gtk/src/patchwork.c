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

  patchwork.c  -  patchwork window

*/
#define _GNU_SOURCE
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <glib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include <pw13/pw13.h>
#include <pw13/export.h>
#include <cairo.h>
#include "patchwork.h"
#include "pw_close.h"
#include "patch.h"
#include "play.h"
#include "input.h"
#include "output.h"
#include "connect.h"
#include "toolbox.h"
#include "pw_draw.h"
#include "xml_signals.h"
#include "drag_and_drop.h"
#include "paths.h"

static GtkTargetEntry drop_targets[] =
  { PW13_DRAG_TEXT_TARGET,
    PW13_DRAG_PATCH_TARGET,
    PW13_DRAG_INPUT_TARGET
  };

/* signal callbacks */
/* window */
static gboolean pw13_gtk_pw_delete (Pw13_GtkPatchwork *pw)
{
  pw13_gtk_pw_close_dialog (pw);
  return TRUE;
}

void pw13_gtk_pw_destroy (Pw13_GtkPatchwork *pw)
{
  pw = pw;
}

/* menu */

static void pw13_gtk_pw_file_sel (Pw13_GtkPatchwork *pw)
{
  const char *newpath = gtk_file_selection_get_filename (pw->filesel);
  pw13_gtk_patchwork_set_filepath (pw, newpath);
  gtk_widget_destroy (GTK_WIDGET (pw->filesel));
  pw->filesel = NULL;
  pw->new = 0;
  pw13_gtk_pw_file_save (pw);
}

static void pw13_gtk_pw_filesel_destroy (Pw13_GtkPatchwork *pw)
{
  if (pw->must_close)
    {
      pw13_gtk_play_stop (pw->play);
      gtk_widget_destroy (GTK_WIDGET (pw->view_menu_item));
      gtk_widget_destroy (GTK_WIDGET (pw->window));
    }
}

static void pw13_gtk_pw_file_save_as (Pw13_GtkPatchwork *pw)
{
  pw->filesel =
    GTK_FILE_SELECTION (gtk_file_selection_new ("Save patchwork"));
  g_signal_connect_swapped (pw->filesel, "destroy_event",
  			    G_CALLBACK (pw13_gtk_pw_filesel_destroy), pw);
  g_signal_connect_swapped (G_OBJECT (pw->filesel->ok_button), "clicked",
			    G_CALLBACK (pw13_gtk_pw_file_sel), pw);
  g_signal_connect_swapped (G_OBJECT (pw->filesel->cancel_button),
			    "clicked", G_CALLBACK (gtk_widget_destroy),
			    pw->filesel);
  gtk_widget_show (GTK_WIDGET (pw->filesel));
}

void pw13_gtk_pw_file_save (Pw13_GtkPatchwork *pw)
{
  if (pw->new)
    {
      pw13_gtk_pw_file_save_as (pw);
    }
  else
    {
      Pw13_Export *x = pw13_export_to_file (pw->filepath);
      pw13_export_patchwork (&pw->pw, x);
      pw13_export_end (x);
      if (pw->must_close)
	pw13_gtk_patchwork_destroy (pw);
    }
}

static void pw13_gtk_pw_file_close (Pw13_GtkPatchwork *pw)
{
  pw13_gtk_pw_delete (pw);
}


/* patchwork drawing area */

static gboolean layout_expose (GtkWidget *widget,
			       GdkEventExpose *event,
			       Pw13_GtkPatchwork *pw)
{
  event = event;
  pw13_gtk_patchwork_draw (pw, widget);
  return TRUE;
}
/*
static gboolean pwdraw_expose (GtkWidget *widget,
			       GdkEventExpose *event,
			       Pw13_GtkPatchwork *pw)
{
  event = event;
  pw_draw (pw, widget);
  return TRUE;
}
*/
static void pwdraw_button_press (GtkWidget *widget,
				 GdkEventButton *event,
				 Pw13_GtkPatchwork *pw)
{
  pw->drag_x = -1;
  pw->drag_y = -1;
  event = event;
  widget = widget;
}

static void pwdraw_button_release (GtkWidget *widget,
				   GdkEventButton *event,
				   Pw13_GtkPatchwork *pw)
{
  pw = pw;
  event = event;
  widget = widget;
}


static gboolean pwdraw_motion_notify (GtkWidget *widget,
				      GdkEventMotion *event,
				      Pw13_GtkPatchwork *pw)
{
  widget = widget;
  if (pw->moving_patch) {
    Pw13_GtkPatch *p = pw->moving_patch;
    gint rx, ry;
    gdk_window_get_origin (pw->pwdraw, &rx, &ry);
    p->x = event->x_root - rx - pw->moving_patch_dx;
    p->y = event->y_root - ry - pw->moving_patch_dy;;
    /*
    printf ("Moving patch %s x %i y %i\n",
	    p->p->name, p->x, p->y);
    */
    gtk_layout_move (GTK_LAYOUT(pw->layout), p->widget, p->x, p->y);
    pw13_gtk_patchwork_redraw (pw);
    return TRUE;
  }
  return FALSE;
}


static gboolean pwdraw_drag_motion (GtkWidget *widget,
				    GdkDragContext *drag_context,
				    gint x, gint y, guint time,
				    Pw13_GtkPatchwork *pw)
{
  GtkTargetList *targetlist;
/*
  printf ("pwdraw drag motion\n");
*/
  if (!pw->drag_highlight) {
    pw->drag_highlight = 1;
    gtk_drag_highlight (widget);
  }

  targetlist = gtk_drag_dest_get_target_list (GTK_WIDGET (pw->layout));
  pw->drag_target = gtk_drag_dest_find_target (widget, drag_context,
					       targetlist);
  if (pw->drag_target == GDK_NONE)
    gdk_drag_status (drag_context, 0, time);
  else {
    pw->drag_drop = 0;
    gtk_drag_get_data (widget, drag_context, pw->drag_target, time);
  }
  x = x;
  y = y;
  return TRUE;
}


static gboolean pwdraw_drag_drop (GtkWidget *widget, GdkDragContext *dc,
				  gint x, gint y, guint time,
				  Pw13_GtkPatchwork *pw)
{
  printf ("pwdraw drag_drop signal received\n");
  fflush (stdout);
  pw->drag_drop = 1;
  gtk_drag_get_data (widget, dc, pw->drag_target, time);
  x = x;
  y = y;
  return TRUE;
}


static void drag_data_received_text(GdkDragContext	*dc,
				    gint		x,
				    gint		y,
				    GtkSelectionData	*selection_data,
				    guint		time,
				    Pw13_GtkPatchwork	*pw)
{
  if (!pw->drag_drop)
    gdk_drag_status (dc, GDK_ACTION_COPY, time);
  else {
    Pw13_GtkPatch *p;
    char *classpath = (char*) selection_data->data;
    printf ("Text drop successful : %s\n", classpath);
    p = pw13_gtk_patch_new_of_classpath (pw, classpath);
    if (!p) {
      printf ("new_pw13_gtk_patch failed\n");
      gtk_drag_finish (dc, FALSE, FALSE, time);
      return;
    }
    p->x = x;
    p->y = y;
    pw13_patchwork_add_patch(&pw->pw, p->p);
    pw13_gtk_patchwork_add_patch (pw, p);
    gtk_drag_finish (dc, TRUE, FALSE, time);
  }
}


static void drag_data_received_patch(GdkDragContext	*dc,
				     gint		x,
				     gint		y,
				     GtkSelectionData	*selection_data,
				     guint		time,
				     Pw13_GtkPatchwork	*pw)
{
  Pw13_GtkPatch		*p;
  union {
    Pw13_GtkPatch	**p;
    unsigned int	*ui;
  }			pp;
  pp.p = &p;
  char * addr = (char*) selection_data->data;
  if (!sscanf (addr, "%x", pp.ui) || !p) {
    printf ("NULL data !\n");
    if (pw->drag_drop)
      gtk_drag_finish (dc, FALSE, FALSE, time);
    return;
  }
  gtk_layout_move (pw->layout, p->widget, x, y);
  pw13_gtk_patchwork_redraw (pw);
  if (pw->drag_drop) {
    printf ("Patch drop successful : %s\n", p->p->name);
    p->x = x;
    p->y = y;
    gtk_drag_finish (dc, TRUE, FALSE, time);
  }
  else
    gdk_drag_status (dc, GDK_ACTION_MOVE, time);
  return;
}


static void drag_data_received_input(GdkDragContext	*dc,
				     GtkSelectionData	*selection_data,
				     guint		time,
				     Pw13_GtkPatchwork	*pw)
{
  if (!pw->drag_drop)
    gdk_drag_status (dc, GDK_ACTION_MOVE, time);
  else {
    Pw13_GtkInput	*i;
    union {
      Pw13_GtkInput	**i;
      unsigned int	*ui;
    }			pi;
    pi.i = &i;
    if ((sscanf ((char*) selection_data->data, "%x", pi.ui) != 1) || !i) {
      printf ("NULL data !\n");
      if (pw->drag_drop)
	gtk_drag_finish (dc, FALSE, FALSE, time);
    }
    else
      if (i->i->from) {
	printf ("Disconnecting input \"%s\"\n", i->i->name);

	/* store output list address linked if only one input linked to it */
	Pw13_GtkOutput *o = NULL;
	if (i->i->from->connected_to->suiv == NULL) {
	  o = (Pw13_GtkOutput*) i->i->from->custom_data;
	}
	
	/* disconnect the patch at the core level */
	pw13_disconnect (i->i);
	
	/* disengage the output checkbox if it is the only link (cant be done before the core disconnection) */
	if(o) {
	  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (o->checkbox),FALSE);
	}
	/* disengage the input checkbox (cant be done before the core disconnection) */ 
	gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (i->checkbox), FALSE);
	
	/* disconnect the patch at gtk level */
	pw13_gtk_disconnect_input (&pw->connections, i);
	/* redraw patchwork */
	pw13_gtk_patchwork_redraw (pw);
      }
  }
}


static void pwdraw_drag_data_received (GtkWidget	 *widget,
				       GdkDragContext	 *dc,
				       gint		 x,
				       gint		 y,
				       GtkSelectionData	 *selection_data,
				       guint		 info,
				       guint		 time,
				       Pw13_GtkPatchwork *pw)
{
  widget = widget;
  x = (x / 8) * 8;
  y = (y / 8) * 8;
  if (!selection_data->data || selection_data->length < 0) {
    printf ("invalid drag data\n");
    return;
  }
  if (info == PW13_DRAG_TEXT)
    drag_data_received_text (dc, x, y, selection_data, time, pw);
  else if (info == PW13_DRAG_PATCH)
    drag_data_received_patch (dc, x, y, selection_data, time, pw);
  else if (info == PW13_DRAG_INPUT)
    drag_data_received_input (dc, selection_data, time, pw);
}

static void pwdraw_drag_begin (Pw13_GtkPatchwork *pw)
{
  pw = pw;
}

static void pwdraw_drag_end (GtkWidget *widget, GdkDragContext *dc,
			     Pw13_GtkPatchwork *pw)
{
  dc = dc;
/*
  printf ("patchwork drag_end signal received\n");
*/
  pw->drag_drop = 0;
  pw->drag_target = GDK_NONE;
  pw->drag_highlight = 0;
  gtk_drag_unhighlight (widget);
}


/* controls */
static void pw13_gtk_pw_play (GtkWidget *widget, Pw13_GtkPatchwork *pw)
{
  if (gtk_toggle_button_get_active (GTK_TOGGLE_BUTTON (widget)) == TRUE) {
    if (!pw13_gtk_play_start (pw->play)) {
      printf ("Failed to create play thread\n");
      gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (widget), FALSE);
    }
  }
  else
    pw13_gtk_play_stop (pw->play);
}


static void connect_patchwork_signals (Pw13_GtkPatchwork *pw)
{
  /* signals */
  /*   window   */
  g_signal_connect_swapped (pw->window, "delete_event",
 			    G_CALLBACK (pw13_gtk_pw_delete), pw);
  g_signal_connect_swapped (pw->window, "destroy_event",
  			    G_CALLBACK (pw13_gtk_pw_destroy), pw);
  /*   file   */
  xml_connect_s(pw->xml, "file_save", "activate", pw13_gtk_pw_file_save, pw);
  xml_connect_s(pw->xml, "file_save_as","activate",pw13_gtk_pw_file_save_as,pw);
  xml_connect_s (pw->xml, "file_close", "activate", pw13_gtk_pw_file_close, pw);
  /*   draw area   */
  g_signal_connect (pw->layout, "expose_event",
			  G_CALLBACK (layout_expose), pw);
  g_signal_connect (pw->layout, "no_expose_event",
			  G_CALLBACK (layout_expose), pw);
  g_signal_connect (pw->layout, "button_press_event",
		    G_CALLBACK (pwdraw_button_press), pw);
  g_signal_connect (pw->layout, "button_release_event",
		    G_CALLBACK (pwdraw_button_release), pw);
  g_signal_connect (pw->layout, "motion_notify_event",
		    G_CALLBACK (pwdraw_motion_notify), pw);

  /* drag n drop */
  g_signal_connect_after (pw->layout, "drag_begin",
			    G_CALLBACK (pwdraw_drag_begin), pw);
  g_signal_connect (pw->layout, "drag_motion",
		    G_CALLBACK (pwdraw_drag_motion), pw);
  g_signal_connect (pw->layout, "drag_drop",
		    G_CALLBACK (pwdraw_drag_drop), pw);
  g_signal_connect (pw->layout, "drag_data_received",
		    G_CALLBACK (pwdraw_drag_data_received), pw);
  g_signal_connect (pw->layout, "drag_end",
		    G_CALLBACK (pwdraw_drag_end), pw);

  /* controls */
  xml_connect (pw->xml, "play", "clicked", pw13_gtk_pw_play, pw);
}


static void build_patchwork_widget (Pw13_GtkPatchwork *p)
{
  p->xml = pw13_gtk_glade_xml (p->paths, "pw13_patchwork.glade", NULL);
  if (!p->xml) {
    g_warning ("build_patchwork_widget: glade_xml_new () failed");
    p->window = NULL;
    return;
  }
  p->window = GTK_WINDOW (glade_xml_get_widget (p->xml, "patchwork"));
  p->layout = GTK_LAYOUT (glade_xml_get_widget (p->xml, "pwlayout"));
  p->pwdraw = p->layout->bin_window;
  gtk_drag_dest_set (GTK_WIDGET (p->layout),
		     0, drop_targets, 3,
		     GDK_ACTION_COPY | GDK_ACTION_MOVE);
  connect_patchwork_signals (p);
  gtk_widget_set_app_paintable (GTK_WIDGET (p->layout), TRUE);
  gtk_widget_set_redraw_on_allocate (GTK_WIDGET (p->layout), TRUE);
}


Pw13_GtkPatchwork * pw13_gtk_patchwork_new (Pw13_GtkPaths *paths)
{
  Pw13_GtkPatchwork *pw;
  pw = (Pw13_GtkPatchwork*) malloc (sizeof (Pw13_GtkPatchwork));
  if (!pw)
    exit (-1);
  pw13_patchwork_init (&pw->pw);
  pw->paths = paths;
  pw->new = 1;
  pw->modified = 0;
  pw->filepath = pw->filename = NULL;
  pw->filesel = NULL;
  pw->width = pw->height = 400;
  build_patchwork_widget (pw);
  if (!pw->window) {
    pw13_patchwork_destroy (&pw->pw);
    free (pw);
    return NULL;
  }
  pw->connections = NULL;

  pw->master = malloc (sizeof (Pw13_GtkPatch));
  if (!pw->master)
    exit (-1);
  pw->master->p = pw->pw.master;
  pw13_gtk_patch_init (pw->master, pw);
  pw->master->x = 500;
  pw->master->y = 260;
  pw13_gtk_patchwork_add_patch (pw, pw->master);

  pw->play = pw13_gtk_play_new (pw);
  pw->must_close = 0;

  pw->moving_patch = NULL;
  return pw;
}


void pw13_gtk_patchwork_set_filepath (Pw13_GtkPatchwork *p, const char *path)
{
  p->filepath = (char*) realloc (p->filepath, strlen (path) + 1);
  strcpy (p->filepath, path);
  p->filename = g_path_get_basename (p->filepath);

  gtk_window_set_title (p->window, p->filename);
}


/*
void pw13_gtk_patchwork_destroy (Pw13_GtkPatchwork *pw)
{
  pwd_delete (pw);
}
*/

void pw13_gtk_patchwork_add_patch (Pw13_GtkPatchwork *pw, Pw13_GtkPatch *p)
{
  gtk_layout_put (pw->layout, p->widget, p->x, p->y);
}


void pw13_gtk_patchwork_destroy (Pw13_GtkPatchwork *pw)
{
  printf ("Destroying GTK patchwork :\n");
  if (pw->play->should_play)
    pw13_gtk_play_stop (pw->play);
  printf (" * patchwork\n");
  pw13_patchwork_destroy (&pw->pw);
  printf (" * cairo connections\n");
  pw13_list_free (pw->connections);
  printf (" * view menu item\n");
  gtk_widget_destroy (GTK_WIDGET (pw->view_menu_item));
  printf (" * patchwork window\n");
  gtk_widget_destroy (GTK_WIDGET (pw->window));
  free(pw);
}


void pw13_gtk_patchwork_redraw (Pw13_GtkPatchwork *pw)
{
  gtk_widget_queue_draw (GTK_WIDGET (pw->layout));
}
