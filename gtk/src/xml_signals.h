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

  xml_signals.h  -  connect signals of glade xml widgets
  
*/
#ifndef PW13_GTK_XML_SIGNALS_H_INCLUDED
#define PW13_GTK_XML_SIGNALS_H_INCLUDED

#include <gtk/gtk.h>
#include <glade/glade.h>

/* connect signal */
#define xml_connect(xml,widget,signal,callback,param)                    \
  g_signal_connect (glade_xml_get_widget (xml, widget),                  \
			    signal, G_CALLBACK (callback), param)

/* connect signal */
#define xml_connect_after(xml,widget,signal,callback,param)              \
  g_signal_connect_after (glade_xml_get_widget (xml, widget),            \
			    signal, G_CALLBACK (callback), param)

/* connect swapped signal */
#define xml_connect_s(xml,widget,signal,callback,param)                  \
  g_signal_connect_swapped (glade_xml_get_widget (xml, widget),          \
			    signal, G_CALLBACK (callback), param)


#endif
/* ndef PW13_GTK_XML_SIGNALS_H_INCLUDED */
