/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13_gtk - A Patchwork13 GTK+2 User Interface
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

  xml.c  -  xml import / export

*/
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <pw13/pw13.h>
#include "input.h"
#include "output.h"
#include "patch.h"
#include "patchwork.h"
#include "connect.h"
#include "xml.h"


void pw13_gtk_patch_xml_export (Pw13_Method *m, void *call_param)
{
  Pw13_Export *x = (Pw13_Export*) call_param;
  Pw13_GtkPatch *p = (Pw13_GtkPatch*) m->param;
  int open = gtk_expander_get_expanded (GTK_EXPANDER (p->expander));
  xmlTextWriterStartElement (x->xml, BAD_CAST "pw13_gtk");

  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "x", "%i", p->x);
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "y", "%i", p->y);
  xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "open", "%i", open);

  xmlTextWriterEndElement (x->xml);
}


static void pw13_gtk_patch_build_connections (Pw13_GtkPatch *p)
{
  int i = p->p->nb_inputs;
  while (i--) {
    Pw13_Input *in;
    Pw13_Output *out;
    in = p->p->input + i;
    out = in->from;
    if (out) {
      Pw13_GtkOutput *gtk_out = (Pw13_GtkOutput*) out->custom_data;
      Pw13_GtkInput *gtk_in = (Pw13_GtkInput*) in->custom_data;
      pw13_gtk_connect (gtk_out, gtk_in, p->pw);
    }
  }
}


static void pw13_gtk_patch_xml_import_custom(Pw13_Method *m, void *call_param)
{
  Pw13_Import		*i = (Pw13_Import*) call_param;
  xmlNode		*node = i->node;
  Pw13_GtkPatch		*p = (Pw13_GtkPatch*) m->param;

  while (node && strcmp((char*) node->name, "pw13_gtk"))
    node = node->next;
  if (node) {
    int open = 0;
    xmlChar *c;
    if ((c = xmlGetProp(node, BAD_CAST "x")))
      p->x = atoi ((char*) c);
    if ((c = xmlGetProp(node, BAD_CAST "y")))
      p->y = atoi ((char*) c);
    if ((c = xmlGetProp(node, BAD_CAST "open")))
      open = atoi ((char*) c);
    gtk_layout_move(GTK_LAYOUT(p->pw->layout), p->widget, p->x, p->y);
    gtk_expander_set_expanded(GTK_EXPANDER (p->expander), open);
  }
  pw13_gtk_patch_expander_activate(GTK_EXPANDER (p->expander), p);
  pw13_gtk_patch_build_connections(p);
}

void pw13_gtk_patch_xml_import (Pw13_Method *m, void *call_param)
{
  Pw13_Import		*i = (Pw13_Import*) call_param;
  Pw13_GtkPatchwork	*pw = (Pw13_GtkPatchwork*) m->param;
  xmlNode		*node = i->node;
  Pw13_Patch		*patch = (Pw13_Patch*) node->_private;
  int			is_master = !strcmp(patch->name, "Master");
  Pw13_GtkPatch		*p = (Pw13_GtkPatch*) patch->custom_data;

  if (is_master)
    printf ("got xml import master\n");
  else {
    p = malloc(sizeof (Pw13_GtkPatch));
    p->p = patch;
    pw13_gtk_patch_init(p, pw);
    pw13_gtk_patchwork_add_patch(pw, p);
  }
  pw13_patch_method_add(patch, "xml import",
			pw13_gtk_patch_xml_import_custom, p);
}
