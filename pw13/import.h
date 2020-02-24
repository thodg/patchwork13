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
#ifndef PW13_IMPORT_H_INCLUDED
#define PW13_IMPORT_H_INCLUDED 1

#include <stdio.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "dll.h"
#include "types.h"
#include "data.h"
#include "output.h"
#include "input.h"
#include "patch_class.h"
#include "patchwork.h"
#include "export.h"
#include "error.h"


struct Pw13_Import
{
  xmlTextWriterPtr xml;
  float progress;
  xmlDocPtr doc;
  xmlNodePtr root, node, parent;
};


PW13_EXTERN
Pw13_Result pw13_import_from_file (Pw13_Import *i, const char *path);

PW13_EXTERN
void pw13_import_end (Pw13_Import *i);

PW13_EXTERN
Pw13_Result pw13_import_patchwork (Pw13_Import *i, Pw13_Patchwork *pw);

PW13_EXTERN
Pw13_Result pw13_import_patch (Pw13_Import *i, Pw13_Patch *p,
			       Pw13_Patchwork *pw);

PW13_EXTERN
Pw13_Result pw13_import_patch_class (Pw13_Import *i, Pw13_PatchClass *pc);

PW13_EXTERN
Pw13_Result pw13_import_input (Pw13_Import *i, Pw13_Input *in);

PW13_EXTERN
Pw13_Result pw13_import_output (Pw13_Import *i, Pw13_Output *o);

PW13_EXTERN
Pw13_Result pw13_import_data_type (Pw13_Import *i, Pw13_DataType *t);

PW13_EXTERN
Pw13_Result pw13_import_data (Pw13_Import *i, Pw13_Data *t);


#endif
/* ndef PW13_PATCH_H_INCLUDED */
