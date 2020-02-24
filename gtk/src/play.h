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

  play.h  -  patchwork playing thread
  
*/
#ifndef PW13_GTK_PLAY_H_INCLUDED
#define PW13_GTK_PLAY_H_INCLUDED

#include <glib.h>
#include <pw13/pw13.h>
#include "patchwork.h"

struct Pw13_GtkPlay
{
  GThread *thread;
  Pw13_GtkPatchwork *pw;
  int should_play;
  int playing;
};

Pw13_GtkPlay * pw13_gtk_play_new (Pw13_GtkPatchwork *pw);
int pw13_gtk_play_start (Pw13_GtkPlay *play);
void pw13_gtk_play_stop (Pw13_GtkPlay *play);


#endif
/* ndef PW13_GTK_PLAY_H_INCLUDED */
