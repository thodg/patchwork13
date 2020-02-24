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

  xy.pw13p.c  -  X and Y float constant patch

*/
#include <math.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <gtk/gtk.h>
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>

#define DEFAULT_MIN 0
#define DEFAULT_MAX 100

typedef struct {
  float minX, maxX, minY, maxY;
  int ix, iy, w, h;
  struct {
    GtkSpinButton *minX_spin, *maxX_spin, *minY_spin, *maxY_spin;
  } open;
  struct {
    GtkWidget *area;
  } closed;
} s_patch_widgets;


static gboolean area_expose_event (GtkWidget *widget,
			      GdkEventExpose *event,
			      Pw13_Patch *p)
{
  s_patch_widgets *w = (s_patch_widgets *) p->param;

  event = event;
  gdk_draw_line (widget->window,
		 widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		 w->ix, 0,
		 w->ix, widget->allocation.height);
  gdk_draw_line (widget->window,
		 widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		 0, w->iy,
		 widget->allocation.width, w->iy);
  return TRUE;
}


static void calc (GtkWidget *widget, Pw13_Patch *p)
{
  s_patch_widgets *w = (s_patch_widgets *) p->param;
  float fx, fy;
  if (w->ix < 0)
    w->ix = 0;
  if (w->ix > w->w)
    w->ix = w->w;
  if (w->iy < 0)
    w->iy = 0;
  if (w->iy > w->h)
    w->iy = w->h;
  fx = w->ix * (w->maxX - w->minX) / w->w + w->minX;
  fy = (w->h - w->iy) * (w->maxY - w->minY) / w->h + w->minY;
  pw13_float_output_val (p->output[0]) = fx;
  pw13_float_output_val (p->output[1]) = fy;
  /*
    printf ("x %f\ty %f\n", fx, fy);
  */
  widget = widget;
}


static gboolean area_button_press (GtkWidget *widget,
			    GdkEventButton *event,
			    Pw13_Patch *p)
{
  s_patch_widgets *w = (s_patch_widgets *) p->param;

  w->ix = event->x;
  w->iy = event->y;
  calc (widget, p);
  gtk_widget_queue_draw (w->closed.area);
  return TRUE;
}


static gboolean area_motion (GtkWidget *widget,
			     GdkEventMotion *event,
			     Pw13_Patch *p)
{
  s_patch_widgets *w = (s_patch_widgets *) p->param;

  w->ix = event->x;
  w->iy = event->y;
  calc (widget, p);
  gtk_widget_queue_draw (w->closed.area);
  return TRUE;
}


static void update (Pw13_Patch *p)
{
  float fx = pw13_float_output_val (p->output[0]);
  float fy = pw13_float_output_val (p->output[1]);
  s_patch_widgets *w = (s_patch_widgets *) p->param;

  w->ix = (fx - w->minX) * w->w / (w->maxX - w->minX);
  w->iy = (fy - w->minY) * w->h / (w->maxY - w->minY);
}


static void minX_spinner_changed (GtkWidget *widget, Pw13_Patch *p)
{
  s_patch_widgets *w = (s_patch_widgets *) p->param;

  w->minX = gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
  if (pw13_float_output_val (p->output[0]) < w->minX)
    pw13_float_output_val (p->output[0]) = w->minX;
  if (w->maxX < w->minX) {
    w->maxX = w->minX;
    gtk_spin_button_set_value (w->open.maxX_spin, w->maxX);
    gtk_spin_button_update (w->open.maxX_spin);
  }
  update (p);
  gtk_widget_queue_draw (w->closed.area);
}


static void maxX_spinner_changed (GtkWidget *widget, Pw13_Patch *p)
{
  s_patch_widgets *w = (s_patch_widgets *) p->param;

  w->maxX = gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
  if (pw13_float_output_val (p->output[0]) > w->maxX)
    pw13_float_output_val (p->output[0]) = w->maxX;
  if (w->maxX < w->minX) {
    w->minX = w->maxX;
    gtk_spin_button_set_value (w->open.minX_spin, w->minX);
    gtk_spin_button_update (w->open.minX_spin);
  }
  update (p);
  gtk_widget_queue_draw (w->closed.area);
}


static void minY_spinner_changed (GtkWidget *widget, Pw13_Patch *p)
{
  s_patch_widgets *w = (s_patch_widgets *) p->param;

  w->minY = gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
  if (pw13_float_output_val (p->output[0]) < w->minY)
    pw13_float_output_val (p->output[0]) = w->minY;
  if (w->maxY < w->minY) {
    w->maxY = w->minY;
    gtk_spin_button_set_value (w->open.maxY_spin, w->maxY);
    gtk_spin_button_update (w->open.maxY_spin);
  }
  update (p);
  gtk_widget_queue_draw (w->closed.area);
}


static void maxY_spinner_changed (GtkWidget *widget, Pw13_Patch *p)
{
  s_patch_widgets *w = (s_patch_widgets *) p->param;

  w->maxY = gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
  if (pw13_float_output_val (p->output[0]) > w->maxY)
    pw13_float_output_val (p->output[0]) = w->maxY;
  if (w->maxY < w->minY) {
    w->minY = w->maxY;
    gtk_spin_button_set_value (w->open.minY_spin, w->minY);
    gtk_spin_button_update (w->open.minY_spin);
  }
  update (p);
  gtk_widget_queue_draw (w->closed.area);
}


static void build_open (Pw13_Method *m, void *call_param)
{
  GtkBox *box = (GtkBox*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  s_patch_widgets *w = (s_patch_widgets *) p->param;
  GtkWidget *vbox, *hbox, *label, *spinner;

  vbox = gtk_vbox_new (FALSE, 0);

  // Max X
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new ("Max X");
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  spinner = gtk_spin_button_new_with_range (-FLT_MAX, FLT_MAX, 0.1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinner), DEFAULT_MAX);
  g_signal_connect (G_OBJECT (spinner), "changed",
		    G_CALLBACK (maxX_spinner_changed), p);
  gtk_box_pack_end (GTK_BOX (hbox), spinner, FALSE, FALSE, 0);
  w->open.maxX_spin = GTK_SPIN_BUTTON (spinner);

  // Min X
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new ("Min X");
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  spinner = gtk_spin_button_new_with_range (-FLT_MAX, FLT_MAX, 0.1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinner), DEFAULT_MIN);
  g_signal_connect (G_OBJECT (spinner), "changed",
		    G_CALLBACK (minX_spinner_changed), p);
  gtk_box_pack_end (GTK_BOX (hbox), spinner, FALSE, FALSE, 0);
  w->open.minX_spin = GTK_SPIN_BUTTON (spinner);

  // Max Y
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new ("Max Y");
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  spinner = gtk_spin_button_new_with_range (-FLT_MAX, FLT_MAX, 0.1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinner), DEFAULT_MAX);
  g_signal_connect (G_OBJECT (spinner), "changed",
		    G_CALLBACK (maxY_spinner_changed), p);
  gtk_box_pack_end (GTK_BOX (hbox), spinner, FALSE, FALSE, 0);
  w->open.maxY_spin = GTK_SPIN_BUTTON (spinner);

  // Min Y
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new ("Min Y");
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  spinner = gtk_spin_button_new_with_range (-FLT_MAX, FLT_MAX, 0.1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinner), DEFAULT_MIN);
  g_signal_connect (G_OBJECT (spinner), "changed",
		    G_CALLBACK (minY_spinner_changed), p);
  gtk_box_pack_end (GTK_BOX (hbox), spinner, FALSE, FALSE, 0);
  w->open.minY_spin = GTK_SPIN_BUTTON (spinner);

  gtk_box_pack_end (box, vbox, TRUE, TRUE, 0);
}


static void build_closed (Pw13_Method *m, void *call_param)
{
  GtkWidget *area;
  GtkBox *box = (GtkBox*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  s_patch_widgets *w = (s_patch_widgets *) p->param;

  area = gtk_drawing_area_new ();
  gtk_drawing_area_size (GTK_DRAWING_AREA (area), w->w+1, w->h+1);
  gtk_widget_add_events (area,
			 GDK_EXPOSURE_MASK |
			 GDK_BUTTON1_MOTION_MASK |
			 GDK_BUTTON_PRESS_MASK);
  g_signal_connect (G_OBJECT (area), "expose-event",  
                    G_CALLBACK (area_expose_event), p);
  g_signal_connect (G_OBJECT (area), "button-press-event",  
                    G_CALLBACK (area_button_press), p);
  g_signal_connect (G_OBJECT (area), "motion-notify-event",
                    G_CALLBACK (area_motion), p);
  gtk_box_pack_end (box, area, TRUE, TRUE, 0);
  w->closed.area = area;
}


static void import (Pw13_Method *m, void *call_param)
{
  Pw13_Import *i = (Pw13_Import*) call_param;
  xmlNode *node = i->node;
  m = m;
  while (node && strcmp ((char*) node->name, "pw13_gtk_const_float_xy"))
    node = node->next;
  if (node) {
    xmlChar *c;
    Pw13_Patch *p = (Pw13_Patch*) node->parent->parent->_private;
    s_patch_widgets *w = (s_patch_widgets *) p->param;
    float fx, fy;
    if ((c = xmlGetProp (node, BAD_CAST "minX")))
      sscanf ((char*) c, "%e", &w->minX);
    if ((c = xmlGetProp (node, BAD_CAST "maxX")))
      sscanf ((char*) c, "%e", &w->maxX);
    if ((c = xmlGetProp (node, BAD_CAST "minY")))
      sscanf ((char*) c, "%e", &w->minY);
    if ((c = xmlGetProp (node, BAD_CAST "maxY")))
      sscanf ((char*) c, "%e", &w->maxY);
    if ((c = xmlGetProp (node, BAD_CAST "x")))
      sscanf ((char*) c, "%e", &fx);
    if ((c = xmlGetProp (node, BAD_CAST "y")))
      sscanf ((char*) c, "%e", &fy);
    pw13_float_output_val (p->output[0]) = fx;
    pw13_float_output_val (p->output[1]) = fy;
    if (w->open.minX_spin) {
      gtk_spin_button_set_value (w->open.minX_spin, w->minX);
      gtk_spin_button_set_value (w->open.maxX_spin, w->maxX);
      gtk_spin_button_set_value (w->open.minY_spin, w->minY);
      gtk_spin_button_set_value (w->open.maxY_spin, w->maxY);
      update (p);
    }
  }
}


static void export (Pw13_Method *m, void *call_param)
{
  Pw13_Export *x = (Pw13_Export*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  s_patch_widgets *w = (s_patch_widgets *) p->param;

  float fx = pw13_float_output_val (p->output[0]);
  float fy = pw13_float_output_val (p->output[1]);
  xmlTextWriterStartElement (x->xml, BAD_CAST "pw13_gtk_const_float_xy");
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "minX", "%99e", w->minX);
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "maxX", "%99e", w->maxX);
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "minY", "%99e", w->minY);
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "maxY", "%99e", w->maxY);
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "x", "%99e", fx);
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "y", "%99e", fy);
  xmlTextWriterEndElement (x->xml);
}


static void patch_destroy (Pw13_Patch *p)
{
  free (p->param);
}


static void init_param (Pw13_Patch *p)
{
  s_patch_widgets *w = malloc (sizeof (s_patch_widgets));

  p->param = w;
  memset(w, 0, sizeof (s_patch_widgets));
  w->w = 100;
  w->h = 100;
  w->minX = DEFAULT_MIN;
  w->maxX = DEFAULT_MAX;
  w->minY = DEFAULT_MIN;
  w->maxY = DEFAULT_MAX;
  update (p);
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 0, 2);
  pw13_float_output ("x", p->output, p);
  pw13_float_output ("y", p->output+1, p);
  p->destroy = patch_destroy;
  pw13_patch_method_add (p, "build_gtk_widget_open", build_open, NULL);
  pw13_patch_method_add (p, "build_gtk_widget_closed", build_closed, NULL);
  pw13_patch_method_add (p, "xml export", export, NULL);
  pw13_patch_method_add (p, "xml import", import, NULL);
  pw13_float_output_val (p->output[0]) = (DEFAULT_MIN+DEFAULT_MAX) / 2.0;
  pw13_float_output_val (p->output[1]) = (DEFAULT_MIN+DEFAULT_MAX) / 2.0;
  init_param (p);
  param = param;
  return 1;
}
