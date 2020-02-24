/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13 - the patchwork13 core library
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

  export.h  -  Export patchwork to a file

*/
#ifndef PW13_EXPORT_H_INCLUDED
#define PW13_EXPORT_H_INCLUDED 1

#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include "dll.h"
#include "types.h"
#include "data.h"
#include "output.h"
#include "input.h"
#include "patch_class.h"
#include "patchwork.h"

#define PW13_XML_ENCODING "ISO-8859-1"


struct Pw13_Export
{
  xmlTextWriterPtr xml;
  float progress;
};


PW13_EXTERN
Pw13_Export * pw13_export_to_file (const char *path);

PW13_EXTERN
int pw13_export_end (Pw13_Export *x);

PW13_EXTERN
int pw13_export_patchwork (Pw13_Patchwork *pw, Pw13_Export *x);

PW13_EXTERN
int pw13_export_patch (Pw13_Patch *p, Pw13_Export *x);

PW13_EXTERN
int pw13_export_patch_class (Pw13_PatchClass *pc, Pw13_Export *x);

PW13_EXTERN
int pw13_export_input (Pw13_Input *i, Pw13_Export *x);

PW13_EXTERN
int pw13_export_output (Pw13_Output *o, Pw13_Export *x);

PW13_EXTERN
int pw13_export_data_type (Pw13_DataType *t, Pw13_Export *x);

PW13_EXTERN
int pw13_export_data (Pw13_Data *d, Pw13_Export *x);

PW13_EXTERN
int pw13_export_data_block (Pw13_Data b, Pw13_Export *x);

unsigned char * pw13_xml_convert (const char *in, const char *encoding);


#endif
/* ndef PW13_EXPORT_H_INCLUDED */
