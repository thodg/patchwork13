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

  bool.pw13p.c  -  gtk boolean constant patch

*/
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <gtk/gtk.h>
#include <pw13/pw13.h>
#include <std/bool.pw13dt.h>


typedef struct {
  GtkToggleButton *button;
} s_patch_widgets;


static void toggled (GtkToggleButton *btn,
		     Pw13_Patch *p)
{
  pw13_bool_output_val (p->output[0]) =
    gtk_toggle_button_get_active (btn);
}


static void build_closed (Pw13_Method *m, void *call_param)
{
  GtkBox *box = (GtkBox*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  s_patch_widgets *w = (s_patch_widgets*) p->param;
  GtkWidget *btn = gtk_toggle_button_new_with_label ("   ");
  g_signal_connect (G_OBJECT (btn), "toggled",  
                    G_CALLBACK (toggled), p);
  gtk_box_pack_end (box, btn, TRUE, TRUE, 0);
  w->button = GTK_TOGGLE_BUTTON (btn);
}


static void import (Pw13_Method *m, void *call_param)
{
  Pw13_Import *i = (Pw13_Import*) call_param;
  xmlNode *node = i->node;
  m = m;
  while (node && strcmp ((char*) node->name, "pw13_gtk_const_bool"))
    node = node->next;
  if (node) {
    xmlChar *c;
    Pw13_Patch *p = (Pw13_Patch*) node->parent->parent->_private;
    s_patch_widgets *w = (s_patch_widgets*) p->param;
    int b;
    if ((c = xmlGetProp (node, BAD_CAST "bool")))
      sscanf ((char*) c, "%i", &b);
    pw13_bool_output_val (p->output[0]) = b;
    gtk_toggle_button_set_active (w->button, b);
  }
}


static void export (Pw13_Method *m, void *call_param)
{
  Pw13_Export *x = (Pw13_Export*) call_param;
  Pw13_Patch *p = (Pw13_Patch*) m->name->object;
  int b = pw13_bool_output_val (p->output[0]);
  xmlTextWriterStartElement (x->xml, BAD_CAST "pw13_gtk_const_bool");
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "bool", "%i", b);
  xmlTextWriterEndElement (x->xml);
}


static void patch_destroy (Pw13_Patch *p)
{
  free (p->param);
}


PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 0, 1);
  pw13_bool_output ("b", p->output, p);
  p->destroy = patch_destroy;
  pw13_patch_method_add (p, "build_gtk_widget_closed", build_closed, NULL);
  pw13_patch_method_add (p, "xml export", export, NULL);
  pw13_patch_method_add (p, "xml import", import, NULL);  
  pw13_bool_output_val (p->output[0]) = 0;
  p->param = malloc (sizeof (s_patch_widgets));
  param = param;
  return 1;
}
