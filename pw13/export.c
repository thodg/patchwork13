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
#include <string.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include "data_type.h"
#include "export.h"

#ifdef _DEBUG
#  include <stdio.h>
#  define debug_log(str) puts (str); fflush (stdout)
#else
#  define debug_log(str) 
#endif


unsigned char * pw13_xml_convert (const char *in, const char *encoding)
{
  xmlChar *out;
  int ret;
  int size;
  int out_size;
  int temp;
  xmlCharEncodingHandlerPtr handler;
  if (!in)
    return NULL;
  handler = xmlFindCharEncodingHandler (encoding);
  if (!handler) {
    printf("pw13_xml_convert: no encoding handler found for '%s'\n",
	   encoding ? encoding : "");
    return 0;
  }
  size = (int) strlen(in) + 1;
  out_size = size * 2 - 1;
  out = (unsigned char *) xmlMalloc((size_t) out_size);
  if (out != 0) {
    temp = size - 1;
    ret = handler->input(out, &out_size, (const xmlChar *) in, &temp);
    if ((ret < 0) || (temp - size + 1)) {
      if (ret < 0) {
	printf("pw13_xml_convert: conversion not successful.\n");
      } else {
	printf
	  ("pw13_xml_convert: conversion not successful."
	   "converted: %i octets.\n", temp);
      }
      xmlFree(out);
      out = 0;
    } else {
      out = (unsigned char *) xmlRealloc (out, out_size + 1);
      out[out_size] = 0;  /*null terminating out */
    }
  } else {
    printf("ConvertInput: no mem\n");
  }
  return out;
}


static void pw13_export_destroy (Pw13_Export *x)
{
  if (x) {
    if (x->xml)
      xmlFreeTextWriter (x->xml);
    x->xml = NULL;
  }
  xmlCleanupParser();
  xmlMemoryDump();
}


Pw13_Export *pw13_export_to_file (const char *path)
{
  Pw13_Export *x;
  LIBXML_TEST_VERSION;
  x = malloc (sizeof (Pw13_Export));
  if (!x)
    exit (1);
  x->progress = 0;
  if (!(x->xml = xmlNewTextWriterFilename (path, 0))) {
    free (x);
    return NULL;
  }
  if (xmlTextWriterStartDocument(x->xml, NULL, PW13_XML_ENCODING, NULL)<0) {
    pw13_export_destroy (x);
    return NULL;
  }
  if (xmlTextWriterStartElement (x->xml, BAD_CAST "patchwork13") < 0) {
    pw13_export_destroy (x);
    return NULL;
  }
  if (xmlTextWriterWriteAttribute (x->xml, BAD_CAST "version",
				   BAD_CAST "0.1") < 0) {
    pw13_export_destroy (x);
    return NULL;
  }
  return x;
}


int pw13_export_end (Pw13_Export *x)
{
  int res = xmlTextWriterEndDocument (x->xml);
  pw13_export_destroy (x);
  xmlCleanupParser();
  xmlMemoryDump();
  if (res < 0)
    return 0;
  return 1;
}


static int export_id_ptr (Pw13_Export *x, const char *name, void *ptr)
{
  debug_log ("<export_id_ptr>\n");
  return xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST name, "%p", ptr);
}


static int export_id (Pw13_Export *x, void *ptr)
{
  debug_log ("<export_id>\n");
  return export_id_ptr (x, "id", ptr);
}


int pw13_export_patchwork (Pw13_Patchwork *pw, Pw13_Export *x)
{
  Pw13_List *L;
  debug_log ("<patchwork>\n");
  if (xmlTextWriterStartElement (x->xml, BAD_CAST "patchwork") < 0)
    return -1;
  export_id (x, pw);
  pw13_patchwork_topo_sort (pw);
  L = pw->patches;
  while (L) {
    Pw13_Patch	*p = (Pw13_Patch*) L->elem;
    if ((p != pw->master) && pw13_export_patch (p, x) < 0)
      return -1;
    L = L->suiv;
  }
  if (xmlTextWriterStartElement (x->xml, BAD_CAST "master") < 0)
    return -1;
  if (pw13_export_patch (pw->master, x) < 0)
    return -1;
  if (xmlTextWriterEndElement (x->xml) < 0)
    return -1;
  if (xmlTextWriterEndElement (x->xml) < 0) /* </patchwork> */
    return -1;
  return 0;
}


int pw13_export_patch (Pw13_Patch *p, Pw13_Export *x)
{
  int r;
  unsigned i;
  xmlChar *tmp;
  debug_log ("<patch>\n");
  if (xmlTextWriterStartElement (x->xml, BAD_CAST "patch") < 0)
    return -1;
  export_id (x, p);
  if (p->patch_class) {
    debug_log ("<patch_class>\n");
    tmp = pw13_xml_convert (p->patch_class->classpath, PW13_XML_ENCODING);
    r = xmlTextWriterWriteAttribute (x->xml, BAD_CAST "class_path", tmp);
    xmlFree (tmp);
    debug_log ("</patch_class>\n");
  }
  debug_log ("<name>\n");
  tmp = pw13_xml_convert (p->name, PW13_XML_ENCODING);
  r = xmlTextWriterWriteAttribute (x->xml, BAD_CAST "name", tmp);
  xmlFree (tmp);
  if (r < 0)
    return -1;
  debug_log ("</name>\n");
  /*
  if (xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "nb_inputs",
					 "%u", p->nb_inputs) < 0)
    return -1;
  if (xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "nb_outputs",
					 "%u", p->nb_outputs) < 0)
    return -1;
  */
  i = p->nb_inputs;
  for (i=0; i<p->nb_inputs; i++)
    pw13_export_input (p->input+i, x);
  for (i=0; i<p->nb_outputs; i++)
    pw13_export_output (p->output+i, x);

  if (pw13_patch_method_find (p, "xml export")) {
    if (xmlTextWriterStartElement (x->xml, BAD_CAST "custom") < 0)
      return -1;
    pw13_patch_method_call (p, "xml export", x);
    if (xmlTextWriterEndElement (x->xml) < 0)
      return -1;
  }

  if (xmlTextWriterEndElement (x->xml) < 0)
    return -1;
  return 0;
}


int pw13_export_patch_class (Pw13_PatchClass *pc, Pw13_Export *x)
{
  int r;
  xmlChar *tmp;
  debug_log ("<patch_class>\n");
  if (xmlTextWriterStartElement (x->xml, BAD_CAST "patch_class") < 0)
    return -1;
  export_id (x, pc);
  tmp = pw13_xml_convert (pc->name, PW13_XML_ENCODING);
  r = xmlTextWriterWriteAttribute (x->xml, BAD_CAST "name", tmp);
  xmlFree (tmp);
  if (r < 0)
    return -1;
  if (xmlTextWriterEndElement (x->xml) < 0)
    return -1;
  return 0;
}


int pw13_export_input (Pw13_Input *i, Pw13_Export *x)
{
  int r;
  xmlChar *tmp;
  debug_log ("<input>\n");
  if (xmlTextWriterStartElement (x->xml, BAD_CAST "input") < 0)
    return -1;
  export_id (x, i);
  tmp = pw13_xml_convert (i->name, PW13_XML_ENCODING);
  r = xmlTextWriterWriteAttribute (x->xml, BAD_CAST "name", tmp);
  xmlFree (tmp);
  if (r < 0)
    return -1;
  if (pw13_export_data_type (i->data_type, x))
    return -1;
  if (xmlTextWriterStartElement (x->xml, BAD_CAST "defval") < 0)
    return -1;
  if (pw13_export_data (i->defval, x))
    return -1;
  if (xmlTextWriterEndElement (x->xml) < 0)  /* </defval> */
    return -1;
  if (i->from) {
    if (xmlTextWriterStartElement (x->xml, BAD_CAST "from") < 0)
      return -1;
    if (export_id_ptr (x, "output_id", i->from) < 0)
      return -1;
    if (xmlTextWriterEndElement (x->xml) < 0)  /* </from> */
      return -1;
  }
  if (xmlTextWriterEndElement (x->xml) < 0)  /* </input> */
    return -1;
  return 0;
}


int pw13_export_output (Pw13_Output *o, Pw13_Export *x)
{
  xmlChar *tmp;
  int r;
  debug_log ("<output>\n");
  if (xmlTextWriterStartElement (x->xml, BAD_CAST "output") < 0)
    return -1;
  export_id (x, o);
  tmp = pw13_xml_convert (o->name, PW13_XML_ENCODING);
  r = xmlTextWriterWriteAttribute (x->xml, BAD_CAST "name", tmp);
  xmlFree (tmp);
  if (r < 0)
    return -1;
  if (pw13_export_data_type (o->data_type, x))
    return -1;
  if (pw13_export_data (o->data, x))
    return -1;
  if (xmlTextWriterEndElement (x->xml) < 0)
    return -1;
  return 0;
}


int pw13_export_data_type (Pw13_DataType *t, Pw13_Export *x)
{
  xmlChar *tmp;
  int r;
  debug_log ("<data_type>\n");
  if (xmlTextWriterStartElement (x->xml, BAD_CAST "data_type") < 0)
    return -1;
  export_id (x, t);
  debug_log ("</export_id_ptr>\n");
  tmp = pw13_xml_convert (t->name, PW13_XML_ENCODING);
  r = xmlTextWriterWriteAttribute (x->xml, BAD_CAST "name", tmp);
  xmlFree (tmp);
  if (r < 0)
    return -1;
  if (xmlTextWriterEndElement (x->xml) < 0)
    return -1;
  return 0;
}


int pw13_export_data (Pw13_Data *d, Pw13_Export *x)
{
  unsigned int i;
  debug_log ("<data>\n");
  if (xmlTextWriterStartElement (x->xml, BAD_CAST "data") < 0)
    return -1;
  export_id (x, d);
  if (xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "nblocks",
					 "%u", d->nblocks) < 0)
    return -1;
  for (i = 1; i <= d->nblocks; i++) {
    if (pw13_export_data_block (d[i], x) < 0)
      return -1;
  }
  if (xmlTextWriterEndElement (x->xml) < 0)
    return -1;
  return 0;
}


int pw13_export_data_block (Pw13_Data b, Pw13_Export *x)
{
  debug_log ("<data_block>\n");
  if (xmlTextWriterStartElement (x->xml, BAD_CAST "data_block") < 0)
    return -1;
  export_id (x, NULL);
  if (xmlTextWriterWriteFormatAttribute (x->xml, BAD_CAST "hex",
					 "%x", b.ui) < 0)
    return -1;
  if (xmlTextWriterEndElement (x->xml) < 0)
    return -1;
  return 0;
}
