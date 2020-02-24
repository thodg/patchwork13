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

  about.c - source file for the about box

  by Vivien Ravet
  
*/
#include <gtk/gtk.h>
#include "icons.h"

void about_pw13_gtk ()
{  
  /* Widgets declarations */
  GtkWidget *window;
  GtkWidget *label;
  GtkWidget *vbox;
  GtkWidget *hbox;
  GtkWidget *button;
  GtkWidget *image;
  
  /* create the window*/ 
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  
  /* title of the window  */
  gtk_window_set_title (GTK_WINDOW (window), "About Patchwork13!");
  gtk_window_set_resizable (GTK_WINDOW (window), FALSE);
  gtk_container_set_border_width (GTK_CONTAINER (window), 10);
  
  /* create a vbox to organise the widgets of the window */
  vbox = gtk_vbox_new (TRUE, 10);
  
  /* create a label to display informations about the software */
  label = gtk_label_new ("Patchwork13 about box !\n"
			 "Danger, chantier interdit au public !");
  
  /* pack the label into the vbox */
  gtk_box_pack_start (GTK_BOX (vbox), label, TRUE, FALSE, 0);
  
  /* show the label */
  gtk_widget_show (label);
  
  /* create an image */
  image = load_icon ("constr.xpm");
  
  /* pack the image into the vbox */
  gtk_box_pack_start (GTK_BOX (vbox), image, TRUE, FALSE, 0);
  
  /* show the image */
  gtk_widget_show (image);
  
  /* create a hbox to control the width of the button */
  hbox = gtk_hbox_new(TRUE,0);
  
  /* a quit button. */
  button = gtk_button_new_with_label ("  OK  ");
  
  /* setup the signal to destroy the window when the button is clicked */
  g_signal_connect_swapped (G_OBJECT (button), "clicked",
			    G_CALLBACK (gtk_widget_destroy),
			    G_OBJECT (window));
  
  /* pack the button in the hbox  */
  gtk_box_pack_start (GTK_BOX (hbox), button, FALSE, FALSE, 0);
  
  /* pack the hbox in the vbox  */
  gtk_box_pack_start (GTK_BOX (vbox), hbox, FALSE, FALSE, 0);
  
  /* pack the vbox in the window */ 
  gtk_container_add (GTK_CONTAINER (window), vbox);
  
  /* show everything */
  gtk_widget_show (button);
  gtk_widget_show (vbox);
  gtk_widget_show (hbox);
  gtk_widget_show (window);
}
