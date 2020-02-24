/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13_std - the patchwork13 standard patches library
  Copyright (C) 2005 Thomas de Grivel <billitch@yahoo.fr>
  
  This program is free software; you can redistribute it and/or
  modify it under the terms of the GNU General Public License
  as published by the Free Software Foundation; either version 2
  of the License, or (at your option) any later version.
  
  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.
  
  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.

*

  replay.pw13p.c  -  replay patch

*/
#include <limits.h>
#include <gtk/gtk.h>
#include <pw13/pw13.h>
#include <pw13/std/int.pw13dt.h>
/* #include <pw13/std/float.pw13dt.h> */


typedef struct {
  GtkWidget *button;
  Pw13_Time time, last_pump_time;
} s_patch_widgets;


static void button_pressed (GtkButton *button, Pw13_Patch *p)
{
  s_patch_widgets *w = (s_patch_widgets*) (p->param);
  w->time = w->last_pump_time;
  button = button;
}



static void pw13_time_replay_pump (Pw13_Patch *p, Pw13_Time *time)
{
  s_patch_widgets *w = (s_patch_widgets*) p->param;
  Pw13_Time diff_time;
  w->last_pump_time = *time;
  pw13_time_sub (&diff_time, time, &w->time);

  pw13_pump_sources (p, &diff_time);

  p->output[0].data = pw13_input_val (p->input);
}


static void build_closed (Pw13_Method *m, void *call_param)
{
  GtkBox *box = (GtkBox*) call_param;
  GtkRequisition button_alloc;

  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  s_patch_widgets *w = (s_patch_widgets*) p->param;

  w->button = gtk_button_new ();
  gtk_box_pack_start (box, w->button, FALSE, FALSE, 0);

  button_alloc.width = 30;
  button_alloc.height = 15;
  
  gtk_widget_size_request (w->button, &button_alloc);

  g_signal_connect (G_OBJECT (w->button), "clicked",
		    G_CALLBACK (button_pressed), p);
}


static void patch_destroy (Pw13_Patch *p)
{
  s_patch_widgets *w = (s_patch_widgets*) p->param;
  gtk_widget_destroy (w->button);
  
  free (p->param);
}


static int pw13_time_replay_start (Pw13_Patch *p, Pw13_Time *time)
{
  time = time;
  s_patch_widgets *w = (s_patch_widgets*) p->param;
  w->last_pump_time = *time;
  return 1;
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 1, 1);
  pw13_any_input("'a", p->input, p);
  pw13_any_output ("'a", p->output, p);
  p->pump = pw13_time_replay_pump;
  p->start = pw13_time_replay_start;
  p->destroy = patch_destroy;
  p->param = malloc (sizeof (s_patch_widgets));
  pw13_patch_method_add (p, "build_gtk_widget_closed", build_closed, NULL);
  param = param;
  return 1;
}
