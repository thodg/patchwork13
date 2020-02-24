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

  icons.h  -  icons loading
  
*/
#ifndef PW13_GTK_ICONS_H_INCLUDED
#define PW13_GTK_ICONS_H_INCLUDED

#include <gtk/gtk.h>

extern char *icon_path_prefix;
extern int icon_path_prefix_len;

char * icon_path (const char *icon_xpm);
GtkWidget * load_icon (const char *icon_xpm);

#endif
/* ndef PW13_GTK_ICONS_H_INCLUDED */
