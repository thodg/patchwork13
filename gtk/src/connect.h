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

  connect.h  -  output -> input connections

*/
#ifndef PW13_GTK_CONNECT_H_INCLUDED
#define PW13_GTK_CONNECT_H_INCLUDED

#include <gtk/gtk.h>
#include <pw13/pw13.h>
#include "types.h"

struct			Pw13_GtkConnect
{
  Pw13_GtkOutput	*o;
  Pw13_GtkInput		*i;
  int			active;
};

void pw13_gtk_get_check_pos (int	*x,
			     int	*y,
			     GtkWidget	*main,
			     GtkWidget	*slave);

void pw13_gtk_disconnect_input (Pw13_List	**L,
				Pw13_GtkInput	*i);

Pw13_GtkConnect * pw13_gtk_connect (Pw13_GtkOutput *output,
				    Pw13_GtkInput *input,
				    Pw13_GtkPatchwork *pw);

#endif
/* ndef PW13_GTK_CONNECT_H_INCLUDED */
