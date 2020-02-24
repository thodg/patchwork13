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

  drag_and_drop.h  -  Drag and drop
  
*/
#ifndef PW13_GTK_DRAG_AND_DROP_H_INCLUDED
#define PW13_GTK_DRAG_AND_DROP_H_INCLUDED

enum {
  PW13_DRAG_TEXT,
  PW13_DRAG_PATCH,
  PW13_DRAG_OUTPUT,
  PW13_DRAG_INPUT
};

#define PW13_DRAG_TEXT_TARGET  {"text/plain", 0, PW13_DRAG_TEXT}
#define PW13_DRAG_PATCH_TARGET \
  {"patchwork13/pw13-gtk-patch", 0, PW13_DRAG_PATCH}
#define PW13_DRAG_OUTPUT_TARGET \
  {"patchwork13/pw13-gtk-output", 0, PW13_DRAG_OUTPUT}
#define PW13_DRAG_INPUT_TARGET \
  {"patchwork13/pw13-gtk-input", 0, PW13_DRAG_INPUT}


#endif
/* ndef PW13_GTK_DRAG_AND_DROP_H_INCLUDED */
