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

  pw_close.c - patchwork dialog, ask wether to save when a pw is closed.

*/
#include <gtk/gtk.h>
#include <glade/glade.h>
#include "xml_signals.h"
#include "icons.h"
#include "pw_close.h"
#include "paths.h"
#include "play.h"
#include "patchwork.h"

static void pw13_gtk_pw_close_response (GtkDialog *dialog, gint response,
					Pw13_GtkPatchwork *pw)
{
  if (response == GTK_RESPONSE_YES)
    {
      /* save; */
      pw->must_close = 1;
      pw13_gtk_pw_file_save (pw);
    }
  if (response == GTK_RESPONSE_NO)
    pw13_gtk_patchwork_destroy (pw);
  gtk_widget_destroy (GTK_WIDGET (dialog));
}


void pw13_gtk_pw_close_dialog (Pw13_GtkPatchwork *pw)
{
  GladeXML *xml;
  xml = pw13_gtk_glade_xml (pw->paths, "pw13_pw_close.glade", NULL);
  if (!xml) {
    g_warning ("pw13_gtk_quit_dialog: pw13_gtk_glade_xml () failed");
    return;
  }
  xml_connect (xml, "pw_close", "response", pw13_gtk_pw_close_response, pw);
}
