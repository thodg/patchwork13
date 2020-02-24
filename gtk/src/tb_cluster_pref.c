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

  tb_cluster_pref.c  -  Cluster preferences
  
*/
#include <stdlib.h>
#include <gtk/gtk.h>
#include <glade/glade.h>
#include "toolbox.h"
#include "paths.h"

static GtkWidget *dialog;
static GtkEntry *serv_path, *cli_path;


void tb_cluster_pref (Pw13_GtkToolbox *toolbox)
{
  GladeXML *xml = pw13_gtk_glade_xml (toolbox->paths,
				      "pw13_tb_cluster_pref.glade",
				      NULL);
  if (!xml) {
    g_warning ("tb_cluster_pref: glade_xml_new() failed");
    return;
  }
  dialog = glade_xml_get_widget (xml, "dialog");

  serv_path = GTK_ENTRY (glade_xml_get_widget (xml, "server_path"));
  gtk_entry_set_text (serv_path,
		      toolbox->cluster_server_path);

  cli_path = GTK_ENTRY (glade_xml_get_widget (xml, "client_path"));
  gtk_entry_set_text (cli_path, toolbox->cluster_client_path);

  glade_xml_signal_autoconnect (xml);
}

void tb_server_path_browse (Pw13_GtkToolbox *toolbox)
{
  GtkWidget *dlg;
  dlg = gtk_file_chooser_dialog_new ("Choose cluster server",
				     toolbox->window,
				     GTK_FILE_CHOOSER_ACTION_OPEN,
				     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				     GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				     NULL);
  if (gtk_dialog_run (GTK_DIALOG (dlg)) == GTK_RESPONSE_ACCEPT) {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dlg));
    gtk_entry_set_text (serv_path, filename);
    g_free (filename);
  }
  gtk_widget_destroy (dlg);
}

void tb_client_path_browse (Pw13_GtkToolbox *toolbox)
{
  GtkWidget *dlg;
  dlg = gtk_file_chooser_dialog_new ("Choose cluster client",
				     toolbox->window,
				     GTK_FILE_CHOOSER_ACTION_OPEN,
				     GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
				     GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT,
				     NULL);
  if (gtk_dialog_run (GTK_DIALOG (dlg)) == GTK_RESPONSE_ACCEPT) {
    char *filename;
    filename = gtk_file_chooser_get_filename (GTK_FILE_CHOOSER (dlg));
    gtk_entry_set_text (cli_path, filename);
    g_free (filename);
  }
  gtk_widget_destroy (dlg);
}

void tb_cluster_pref_ok (Pw13_GtkToolbox *toolbox)
{
  free (toolbox->cluster_server_path);
  toolbox->cluster_server_path = g_strdup (gtk_entry_get_text (serv_path));
  free (toolbox->cluster_client_path);
  toolbox->cluster_client_path = g_strdup (gtk_entry_get_text (cli_path));
  gtk_widget_destroy (dialog);
}

void tb_cluster_pref_cancel ()
{
  gtk_widget_destroy (dialog);
}
