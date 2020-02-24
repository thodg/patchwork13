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

  float.pw13p.c  -  float constant patch

*/
//#include <limits.h>
#include <math.h>
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <gtk/gtk.h>
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>


typedef struct {
  GtkAdjustment *adjustment;
  GtkWidget *current_control, *unused;
  struct {
    GtkSpinButton *min_spin, *max_spin;
    GtkComboBox *combo;
  } open;
  struct {
    GtkWidget *bin;
    GtkWidget *spinner;
    GtkWidget *hscale;
  } closed;
} s_patch_widgets, *p_patch_widgets;


static void combo_realize (GtkWidget *widget)
{
  GtkComboBox *combo = GTK_COMBO_BOX (widget);
  gtk_combo_box_set_active (combo, 0);
}


static void const_spinner_changed (GtkWidget *widget, Pw13_Patch *p)
{
  pw13_float_output_val (p->output[0]) =
    gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
}


static void min_spinner_changed (GtkWidget *widget, Pw13_Patch *p)
{
  p_patch_widgets w = (p_patch_widgets) p->param;
  float val = gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
  w->adjustment->lower = val;
  if (w->adjustment->upper < val) {
    w->adjustment->upper = val;
    gtk_spin_button_set_value (w->open.max_spin, val);
    gtk_spin_button_update (w->open.max_spin);
  }
  gtk_adjustment_changed (w->adjustment);
  gtk_adjustment_value_changed (w->adjustment);
  gtk_range_set_adjustment (GTK_RANGE (w->closed.hscale), w->adjustment);
  gtk_spin_button_update (GTK_SPIN_BUTTON (w->closed.spinner));
}


static void max_spinner_changed (GtkWidget *widget, Pw13_Patch *p)
{
  p_patch_widgets w = (p_patch_widgets) p->param;
  float val = 1 + gtk_spin_button_get_value (GTK_SPIN_BUTTON (widget));
  w->adjustment->upper = val;
  if (w->adjustment->lower > val) {
    w->adjustment->lower = val;
    gtk_spin_button_set_value (w->open.min_spin, val);
    gtk_spin_button_update (w->open.min_spin);
  }
  gtk_adjustment_changed (w->adjustment);
  gtk_adjustment_value_changed (w->adjustment);
  gtk_range_set_adjustment (GTK_RANGE (w->closed.hscale), w->adjustment);
  gtk_spin_button_update (GTK_SPIN_BUTTON (w->closed.spinner));
}


static void combo_changed (GtkComboBox *combo, Pw13_Patch *p)
{
  p_patch_widgets w = (p_patch_widgets) p->param;
  gchar *active = gtk_combo_box_get_active_text (combo);
  switch (active[0]) {
  case 'F':  // Fader
    if (w->current_control != GTK_WIDGET (w->closed.hscale)) {
      gtk_widget_reparent (w->current_control, w->unused);
      w->current_control = GTK_WIDGET (w->closed.hscale);
      gtk_widget_reparent (w->current_control, w->closed.bin);
      gtk_widget_show (w->current_control);
    }
    break;

  case 'S':  // Spinner
    if (w->current_control != GTK_WIDGET (w->closed.spinner)) {
      gtk_widget_reparent (w->current_control, w->unused);
      w->current_control = GTK_WIDGET (w->closed.spinner);
      gtk_widget_reparent (w->current_control, w->closed.bin);
      gtk_widget_show (w->current_control);
    }
    break;

  default:
    g_warning ("combo_changed: unknown control type !\n");
  }
}


static void build_open (Pw13_Method *m, void *call_param)
{
  GtkBox *box = (GtkBox*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  p_patch_widgets w = (p_patch_widgets) p->param;
  GtkWidget *vbox, *hbox, *label, *spinner;
  GtkComboBox *combo;
  vbox = gtk_vbox_new (FALSE, 0);

  // Min
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new ("Min");
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  spinner = gtk_spin_button_new_with_range (-FLT_MAX, FLT_MAX, 0.1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinner), -100);
  g_signal_connect (G_OBJECT (spinner), "changed",
		    G_CALLBACK (min_spinner_changed), p);
  gtk_box_pack_end (GTK_BOX (hbox), spinner, FALSE, FALSE, 0);
  w->open.min_spin = GTK_SPIN_BUTTON (spinner);

  // Max
  hbox = gtk_hbox_new (FALSE, 0);
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  label = gtk_label_new ("Max");
  gtk_box_pack_start (GTK_BOX (hbox), label, FALSE, FALSE, 0);
  spinner = gtk_spin_button_new_with_range (-FLT_MAX, FLT_MAX, 0.1);
  gtk_spin_button_set_value (GTK_SPIN_BUTTON (spinner), 100);
  g_signal_connect (G_OBJECT (spinner), "changed",
		    G_CALLBACK (max_spinner_changed), p);
  gtk_box_pack_end (GTK_BOX (hbox), spinner, FALSE, FALSE, 0);
  w->open.max_spin = GTK_SPIN_BUTTON (spinner);

  // Combo
  combo = GTK_COMBO_BOX (gtk_combo_box_new_text ());
  gtk_combo_box_append_text (combo, "Fader");
  gtk_combo_box_append_text (combo, "Spinner");
  g_signal_connect (G_OBJECT (combo), "realize",
		    G_CALLBACK (combo_realize), p);
  g_signal_connect (G_OBJECT (combo), "changed",
		    G_CALLBACK (combo_changed), p);
  gtk_box_pack_end (GTK_BOX (vbox), GTK_WIDGET (combo), FALSE, FALSE, 0);

  gtk_box_pack_end (box, vbox, TRUE, TRUE, 0);
}


static void build_closed (Pw13_Method *m, void *call_param)
{
  GtkBox *box = (GtkBox*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  p_patch_widgets w = (p_patch_widgets) p->param;
  w->closed.bin = gtk_frame_new (NULL);
  gtk_frame_set_shadow_type (GTK_FRAME (w->closed.bin), GTK_SHADOW_NONE);
  w->adjustment = GTK_ADJUSTMENT (gtk_adjustment_new (10, -100, 100,
						      0.1, 1, 1));

  w->unused = gtk_hbox_new (FALSE, 0);

  w->closed.spinner = gtk_spin_button_new (w->adjustment, 0.1, 2);
  g_signal_connect (G_OBJECT (w->closed.spinner), "changed",
		    G_CALLBACK (const_spinner_changed), p);
  gtk_container_add (GTK_CONTAINER (w->unused), w->closed.spinner);

  w->closed.hscale = gtk_hscale_new (w->adjustment);
  gtk_scale_set_draw_value (GTK_SCALE (w->closed.hscale), FALSE);
  GTK_WIDGET_UNSET_FLAGS (GTK_WIDGET (w->closed.hscale), GTK_CAN_FOCUS);
  gtk_widget_set_size_request (GTK_WIDGET (w->closed.hscale), 64, 8);
  gtk_container_add (GTK_CONTAINER (w->closed.bin), w->closed.hscale);

  w->current_control = GTK_WIDGET (w->closed.hscale);
  gtk_box_pack_end (box, w->closed.bin, TRUE, TRUE, 0);
}


static void import (Pw13_Method *m, void *call_param)
{
  Pw13_Import *i = (Pw13_Import*) call_param;
  xmlNode *node = i->node;
  m = m;
  while (node && strcmp ((char*) node->name, "pw13_gtk_const_float"))
    node = node->next;
  if (node) {
    xmlChar *c;
    Pw13_Patch *p = (Pw13_Patch*) node->parent->parent->_private;
    p_patch_widgets w = (p_patch_widgets) p->param;
    float min, max, value;
    if ((c = xmlGetProp (node, BAD_CAST "min")))
      sscanf ((char*) c, "%e", &min);
    if ((c = xmlGetProp (node, BAD_CAST "max")))
      sscanf ((char*) c, "%e", &max);
    if ((c = xmlGetProp (node, BAD_CAST "value")))
      sscanf ((char*) c, "%e", &value);
    w->adjustment->lower = min;
    w->adjustment->upper = max;
    w->adjustment->value = value;
    gtk_adjustment_changed (w->adjustment);
    gtk_adjustment_value_changed (w->adjustment);
    gtk_spin_button_set_value (w->open.min_spin, min);
    gtk_spin_button_set_value (w->open.max_spin, max);
    pw13_float_output_val (p->output[0]) = value;
  }
}


static void export (Pw13_Method *m, void *call_param)
{
  Pw13_Export *x = (Pw13_Export*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  p_patch_widgets w = (p_patch_widgets) p->param;
  float min = (float) w->adjustment->lower;
  float max = (float) w->adjustment->upper;
  float val = (float) w->adjustment->value;
  xmlTextWriterStartElement (x->xml, BAD_CAST "pw13_gtk_const_float");
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "min", "%99e", min);
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "max", "%99e", max);
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "value", "%99e", val);
  xmlTextWriterEndElement (x->xml);
}


static void patch_destroy (Pw13_Patch *p)
{
  p_patch_widgets w = (p_patch_widgets) p->param;
  gtk_widget_destroy (w->unused);
  free (p->param);
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 0, 1);
  pw13_float_output ("k", p->output, p);
  pw13_float_output_val (p->output[0]) = 10;
  p->destroy = patch_destroy;
  p->param = malloc (sizeof (s_patch_widgets));
  pw13_patch_method_add (p, "build_gtk_widget_open", build_open, NULL);
  pw13_patch_method_add (p, "build_gtk_widget_closed", build_closed, NULL);
  pw13_patch_method_add (p, "xml export", export, NULL);
  pw13_patch_method_add (p, "xml import", import, NULL);
  param = param;
  return 1;
}
