/*
  pw13_gtk - A Patchwork13! GTK User Interface
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

  quit.c - quit dialog

*/
#include <gtk/gtk.h>
#include <glade/glade.h>
#include "xml_signals.h"
#include "icons.h"
#include "quit.h"
#include "paths.h"

static void pw13_gtk_quit_response (GtkDialog *dialog, gint response)
{
  if (response == GTK_RESPONSE_YES)
    gtk_main_quit ();
  else
    gtk_widget_destroy (GTK_WIDGET (dialog));
}

/*
static void pw13_gtk_quit_no_clicked ()
{
  gtk_main_quit ();
}
*/

void pw13_gtk_quit_dialog (Pw13_GtkPaths *paths)
{
  GladeXML *xml;
  xml = pw13_gtk_glade_xml (paths, "pw13_quit.glade", NULL);
  if (!xml) {
    g_warning ("pw13_gtk_quit_dialog: glade_xml_new() failed");
    return;
  }
  xml_connect (xml, "quit", "response", pw13_gtk_quit_response, NULL);
}
