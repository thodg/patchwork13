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

  xml.h  -  xml import / export

*/
#ifndef PW13_GTK_XML_H_INCLUDED
#define PW13_GTK_XML_H_INCLUDED

#include <pw13/pw13.h>


void pw13_gtk_patch_xml_export (Pw13_Method *m, void *call_param);
void pw13_gtk_patch_xml_import (Pw13_Method *m, void *call_param);


#endif
/* ndef PW13_GTK_XML_H_INCLUDED */
