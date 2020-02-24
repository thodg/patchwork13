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

  print.pw13p.c  -  Prints the input signal

*/


#include <math.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <gtk/gtk.h>
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>
#include <stdio.h>

#define DEFAULT_INTERVAL 8

#define DEFAULT_MIN -2
#define DEFAULT_MAX 2

#define AREA_WIDTH 128
#define AREA_HEIGHT 128


typedef struct {
  float minY, maxY;
  int w, h;
  unsigned interval, new_interval;
  int print_counter, print_pos;
  GdkPoint points[AREA_WIDTH];
  float val;
  struct {
    GtkSpinButton *interval_spin, *minY_spin, *maxY_spin;
  } open;
  struct {
    GtkWidget *area;
  } closed;
} s_patch_widgets, *p_patch_widgets;


static gboolean area_expose_event (GtkWidget *widget,
			      GdkEventExpose *event,
			      Pw13_Patch *p)
{
  p_patch_widgets w = (p_patch_widgets) p->param;

  event = event;
  gdk_draw_lines (widget->window,
		   widget->style->fg_gc[GTK_WIDGET_STATE (widget)],
		   w->points, AREA_WIDTH);
  return TRUE;
}


static void interval_spinner_changed (GtkWidget *widget, Pw13_Patch *p)
{
  p_patch_widgets w = (p_patch_widgets) p->param;
  w->new_interval = gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
}


static void minY_spinner_changed (GtkWidget *widget, Pw13_Patch *p)
{
  p_patch_widgets w = (p_patch_widgets) p->param;
  w->minY = gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
  if (w->maxY < w->minY) {
    w->maxY = w->minY;
    gtk_spin_button_set_value (w->open.maxY_spin, w->maxY);
    gtk_spin_button_update (w->open.maxY_spin);
  }
}


static void maxY_spinner_changed (GtkWidget *widget, Pw13_Patch *p)
{
  p_patch_widgets w = (p_patch_widgets) p->param;
  w->maxY = gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
  if (w->maxY < w->minY) {
    w->minY = w->maxY;
    gtk_spin_button_set_value (w->open.minY_spin, w->minY);
    gtk_spin_button_update (w->open.minY_spin);
  }
}


static void build_open (Pw13_Method *m, void *call_param)
{
  GtkBox *box = (GtkBox*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;

  p_patch_widgets w = (p_patch_widgets) p->param;
  GtkWidget *vbox, *hbox, *label, *spinner;
  vbox = gtk_vbox_new (FALSE, 0);

  // Max
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new ("Max");
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  spinner = gtk_spin_button_new_with_range (-FLT_MAX, FLT_MAX, 0.1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinner), DEFAULT_MAX);
  g_signal_connect (G_OBJECT (spinner), "changed",
		    G_CALLBACK (maxY_spinner_changed), p);
  gtk_box_pack_end (GTK_BOX (hbox), spinner, FALSE, FALSE, 0);
  w->open.maxY_spin = GTK_SPIN_BUTTON (spinner);

  // Min
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new ("Min");
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  spinner = gtk_spin_button_new_with_range (-FLT_MAX, FLT_MAX, 0.1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinner), DEFAULT_MIN);
  g_signal_connect (G_OBJECT (spinner), "changed",
		    G_CALLBACK (minY_spinner_changed), p);
  gtk_box_pack_end (GTK_BOX (hbox), spinner, FALSE, FALSE, 0);
  w->open.minY_spin = GTK_SPIN_BUTTON (spinner);

  // Interval
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new ("Interval");
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  spinner = gtk_spin_button_new_with_range (1, INT_MAX, 1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinner), DEFAULT_INTERVAL);
  g_signal_connect (G_OBJECT (spinner), "changed",
		    G_CALLBACK (interval_spinner_changed), p);
  gtk_box_pack_end (GTK_BOX (hbox), spinner, FALSE, FALSE, 0);
  w->open.interval_spin = GTK_SPIN_BUTTON (spinner);

  gtk_box_pack_end (box, vbox, TRUE, TRUE, 0);
}


static void build_closed (Pw13_Method *m, void *call_param)
{
  GtkWidget *area;
  GtkBox *box = (GtkBox*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;

  p_patch_widgets w = (p_patch_widgets) p->param;
  area = gtk_drawing_area_new ();
  gtk_drawing_area_size (GTK_DRAWING_AREA (area), w->w+1, w->h+1);
  gtk_widget_add_events (area,
			 GDK_EXPOSURE_MASK |
			 GDK_BUTTON1_MOTION_MASK |
			 GDK_BUTTON_PRESS_MASK);
  g_signal_connect (G_OBJECT (area), "expose-event",  
                    G_CALLBACK (area_expose_event), p);
  gtk_box_pack_end (box, area, TRUE, TRUE, 0);
  w->closed.area = area;
}


static void init_param (Pw13_Patch *p)
{
  int i;
  p_patch_widgets w = malloc (sizeof (s_patch_widgets));
  p->param = w;
  
  w->w = AREA_WIDTH;
  w->h = AREA_HEIGHT;
  w->minY = DEFAULT_MIN;
  w->maxY = DEFAULT_MAX;
  w->interval = w->new_interval = DEFAULT_INTERVAL;
  for (i=0; i<AREA_WIDTH; i++) {
    w->points[i].x = i;
    w->points[i].y = AREA_HEIGHT / 2;
  }
}


static void import (Pw13_Method *m, void *call_param)
{
  Pw13_Import *i = (Pw13_Import*) call_param;
  xmlNode *node = i->node;
  m = m;
  while (node && strcmp ((char*) node->name, "pw13_gtk_display_oscilloscope"))
    node = node->next;
  if (node) {
    xmlChar *c;
    Pw13_Patch *p = (Pw13_Patch*) node->parent->parent->_private;
    p_patch_widgets w = (p_patch_widgets) p->param;
    if ((c = xmlGetProp (node, BAD_CAST "interval")))
      sscanf ((char*) c, "%u", &w->new_interval);
    if ((c = xmlGetProp (node, BAD_CAST "minY")))
      sscanf ((char*) c, "%e", &w->minY);
    if ((c = xmlGetProp (node, BAD_CAST "maxY")))
      sscanf ((char*) c, "%e", &w->maxY);
    gtk_spin_button_set_value (w->open.interval_spin, w->new_interval);
    gtk_spin_button_set_value (w->open.minY_spin, w->minY);
    gtk_spin_button_set_value (w->open.maxY_spin, w->maxY);
  }
}


static void export (Pw13_Method *m, void *call_param)
{
  Pw13_Export *x = (Pw13_Export*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  p_patch_widgets w = (p_patch_widgets) p->param;
  xmlTextWriterStartElement (x->xml, BAD_CAST "pw13_gtk_display_oscilloscope");
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "interval",
				     "%i", w->new_interval);
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "minY", "%99e", w->minY);
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "maxY", "%99e", w->maxY);
  xmlTextWriterEndElement (x->xml);
}


static void pw13_print_signal_pump (Pw13_Patch *p, Pw13_Time *time)
{
  pw13_pump_sources (p, time);
  p_patch_widgets w = (p_patch_widgets) p->param;
  int x = w->print_pos;
  GtkWidget *wid = w->closed.area; /* not weed =D       wid, wid, i love wid */
  w->val += pw13_float_input_val (p->input);
  if (!w->print_counter--) {
    float v = w->val / w->interval;
    w->points[x].y = (v - w->minY) * w->h / (w->maxY - w->minY);
    w->print_counter = w->interval = w->new_interval;
    w->val = 0;
    if (++w->print_pos == AREA_WIDTH) {
      w->print_pos = 0;
      gtk_widget_queue_draw (wid);
    }
  }
  time=time;
}


static int patch_start (Pw13_Patch *p, Pw13_Time *t)
{
  int i;
  p_patch_widgets w = (p_patch_widgets) p->param;
  w->print_pos = 0;
  w->print_counter = w->interval = w->new_interval;
  for (i=0; i<AREA_WIDTH; i++) {
    w->points[i].x = i;
    w->points[i].y = AREA_HEIGHT / 2;
  }
  w->val = 0;
  t = t;
  return 1;
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 1, 1);
  pw13_float_input("Signal", p->input, p, 0);
  pw13_void_output ("Void", p->output, p);
  pw13_patch_method_add (p, "build_gtk_widget_open", build_open, NULL);
  pw13_patch_method_add (p, "build_gtk_widget_closed", build_closed, NULL);
  pw13_patch_method_add (p, "xml export", export, NULL);
  pw13_patch_method_add (p, "xml import", import, NULL);
  p->start = patch_start;
  p->pump = pw13_print_signal_pump;
  init_param (p);
  param = param;
  return 1;
}
