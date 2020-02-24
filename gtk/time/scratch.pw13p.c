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

  scratch.pw13p.c  -  scratching  patch

*/

#include <gtk/gtk.h>
#include <pw13/pw13.h>


typedef struct {
  double time;
  struct{
    GtkWidget *hscale;
  } open;
  struct{
    GtkWidget *button;
  } closed;
} PatchParam;

static void button_pressed (GtkButton *button, Pw13_Patch *p)
{
  button=button;
  PatchParam *w = (PatchParam*) (p->param);
  w->time = gtk_range_get_value (GTK_RANGE (w->open.hscale));
  gtk_range_set_value (GTK_RANGE (w->open.hscale),0);
}

    
static void build_closed (Pw13_Method *m, void *call_param)
{
  GtkBox *box = (GtkBox*) call_param;
  GtkRequisition button_alloc;

  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  PatchParam *w = (PatchParam*) p->param;

  w->closed.button = gtk_button_new ();

  gtk_box_pack_start(box,w->closed.button,FALSE,FALSE,0);

  button_alloc.width=30;
  button_alloc.height=15;
  
  gtk_widget_size_request (w->closed.button,&button_alloc);

  g_signal_connect (G_OBJECT (w->closed.button), "clicked",
		    G_CALLBACK (button_pressed), p);
}


static void build_open (Pw13_Method *m, void *call_param)
{
  GtkBox *box = (GtkBox*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  PatchParam *w = (PatchParam*) p->param;
  w->open.hscale =  gtk_hscale_new_with_range (-0.5, 0.5, 0.00001);
  gtk_range_set_value (GTK_RANGE (w->open.hscale), 0);
  gtk_box_pack_start (box, w->open.hscale, TRUE, TRUE, 0);
}


static void patch_destroy (Pw13_Patch *p)
{
  PatchParam *w = (PatchParam*) p->param;
  gtk_widget_destroy (w->closed.button);
  gtk_widget_destroy (w->open.hscale);
  
  free (p->param);
}


static int patch_start (Pw13_Patch *p, Pw13_Time *time)
{
  time = time;
  PatchParam *w = (PatchParam*) p->param;
  w->time = 0;
  return 1;
}


static void patch_pump (Pw13_Patch *p, Pw13_Time *time)
{
  PatchParam *w = (PatchParam*) p->param;
  Pw13_Time t = *time;
  t.seconds += (int) w->time;
  t.frac += w->time - (int) w->time;
  pw13_pump_sources (p, &t);
  p->output[0].data = pw13_input_val (p->input);
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 1, 1);
  pw13_any_input("'a", p->input, p);
  pw13_any_output ("'a", p->output, p);
  p->pump = patch_pump;
  p->start = patch_start;
  p->destroy = patch_destroy;
  p->param = malloc (sizeof (PatchParam));
  pw13_patch_method_add (p, "build_gtk_widget_open", build_open, NULL);
  pw13_patch_method_add (p, "build_gtk_widget_closed", build_closed, NULL);
  param = param;
  return 1;
}
