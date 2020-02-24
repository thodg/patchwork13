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

  import.c  -  import xml

*/
#include <stdarg.h>
#include <string.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include "error.h"
#include "classpath.h"
#include "names.h"
#include "import.h"


static void pw13_import_destroy (Pw13_Import *i)
{
  if (i) {
    if (i->doc)
      xmlFreeDoc (i->doc);
    i->doc = NULL;
  }
  xmlCleanupParser();
  xmlMemoryDump();
}


static Pw13_Result import_error (const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  fprintf (stderr, "pw13 xml import: ");
  vfprintf (stderr, fmt, ap);
  fprintf (stderr, "\n");
  va_end (ap);
  return PW13_ERROR;
}


static void node_down (Pw13_Import *i)
{
  i->parent = i->node;
  i->node = i->node->children;
}


static void node_up (Pw13_Import *i)
{
  i->node = i->parent;
  i->parent = i->node->parent;
}


static void next_node (Pw13_Import *i)
{
  i->node = i->node->next;
}


static void * node_id_ptr (xmlNode *node, const char *name)
{
  void *id = NULL;
  xmlChar *c = xmlGetProp (node, (xmlChar*) name);
  if (!c)
    //    import_error ("missing attribute id in node <%s>", node->name);
    ;
  else
    sscanf ((char*) c, "%p", &id);
  return id;
}


static void * node_id (xmlNode *node)
{
  return node_id_ptr (node, "id");
}


static int find_node (xmlNode *node, const char *name, void *id,
		      xmlNode **result)
{
  xmlNode *child;
  void *nid = node_id (node);
  //  printf ("find_node: <%s id=\"%p\">\n", node->name, nid);
  fflush (stdout);
  if ((id == nid) && (!strcmp ((char*) node->name, name))) {
    *result = node;
    return PW13_SUCCESS;
  }
  child = node->children;
  while (child) {
    if (find_node (child, name, id, result) == PW13_SUCCESS)
      return PW13_SUCCESS;
    child = child->next;
  }
  return PW13_ERROR;
}


static Pw13_Result check_node_name (Pw13_Import *i, const char *name)
{
  if (!i->node)
    return import_error ("NULL node (instead of <%s>)", name);
  if (strcmp ((char*) i->node->name, name))
    return import_error ("node is not a <%s>", name);
  return PW13_SUCCESS;
}


Pw13_Result pw13_import_from_file (Pw13_Import *i, const char *path)
{
  LIBXML_TEST_VERSION;

  i->progress = 0;

  i->doc = xmlReadFile (path, NULL, 0);
  if (!i->doc) {
    pw13_import_destroy (i);
    return import_error ("Failed to parse %s\n", path);
  }

  i->root = i->doc->children;
  i->node = i->root;

  printf ("root name=%s\n", i->root->name);

  if (check_node_name (i, "patchwork13") != PW13_SUCCESS)
    return PW13_ERROR;

  node_down (i);

  return PW13_SUCCESS;
}


void pw13_import_end (Pw13_Import *i)
{
  pw13_import_destroy (i);
}


Pw13_Result pw13_import_patchwork (Pw13_Import *i, Pw13_Patchwork *pw)
{
  if (check_node_name (i, "patchwork") != PW13_SUCCESS)
    return PW13_ERROR;
  node_down (i);
  while (i->node) {
    if (!strcmp((char*) i->node->name, "patch")) {
      Pw13_Patch *p = malloc(sizeof (Pw13_Patch));
      if (!p)
	exit (-1);
      if (pw13_import_patch(i, p, pw) != PW13_SUCCESS)
	return PW13_ERROR;
    }
    else if (!strcmp((char*) i->node->name, "master")) {
      /*
      printf ("<master>\n");
      */
      node_down(i);
      if (pw13_import_patch(i, pw->master, pw) != PW13_SUCCESS)
	return PW13_ERROR;
      node_up (i);
    }
    else
      import_error("unknown node <%s> in patchwork");
    next_node(i);
  }
  node_up(i);
  return PW13_SUCCESS;
}


static Pw13_Result pw13_import_patch_input (Pw13_Import *i, Pw13_Patch *p)
{
  Pw13_Input *in;
  xmlChar *name = xmlGetProp (i->node, (xmlChar*) "name");
  if (!name)
    return import_error ("input without name in patch \"%s\"", p->name);
  printf ("<patch input name %s>\n", name);
  in = pw13_find_input (p, (char*) name);
  if (!in)
    return import_error ("input \"%s\" not found in patch \"%s\"",
			 (char*) name, p->name);
  xmlFree (name);
  if (pw13_import_input (i, in) != PW13_SUCCESS)
    return PW13_ERROR;
  return PW13_SUCCESS;
}


static Pw13_Result pw13_import_patch_output (Pw13_Import *i, Pw13_Patch *p)
{
  Pw13_Output *out;
  xmlChar *name = xmlGetProp (i->node, (xmlChar*) "name");
  if (!name)
    return import_error ("output without name in patch \"%s\"", p->name);
  out = pw13_find_output (p, (char*) name);
  if (!out)
    return import_error ("output \"%s\" not found in patch \"%s\"",
			 (char*) name, p->name);
  xmlFree (name);
  if (pw13_import_output (i, out) != PW13_SUCCESS)
    return PW13_ERROR;
  return PW13_SUCCESS;
}


static Pw13_Result pw13_import_patch_custom (Pw13_Import *i, Pw13_Patch *p)
{
  node_down (i);
  pw13_patch_method_call (p, "xml import", i);
  node_up (i);
  return PW13_SUCCESS;
}


Pw13_Result pw13_import_patch (Pw13_Import *i, Pw13_Patch *p,
			       Pw13_Patchwork *pw)
{
  xmlChar	*c;
  char		*patch_name = NULL;
  char		*classpath = NULL;
  if (check_node_name(i, "patch") != PW13_SUCCESS)
    return PW13_ERROR;

  i->node->_private = p;
  c = xmlGetProp(i->node, (xmlChar*) "name");
  if (!c)
    return import_error("missing patch name");
  patch_name = strdup((char*) c);
  xmlFree(c);
  /*
  printf ("patch name=\"%s\"\n", (char*) patch_name);
  */
  c = xmlGetProp(i->node, (xmlChar*) "class_path");
  if (c) {
    classpath = strdup((char*) c);
    xmlFree (c);
    if (pw13_patch_of_classpath(p, classpath, patch_name) != PW13_SUCCESS)
      return PW13_ERROR;
    /*
    printf ("  classpath=\"%s\"\n", p->p_class->classpath);
    */
  }
  else
    p->name = patch_name;

  if (pw) {
    pw13_patchwork_add_patch(pw, p);
    pw13_patchwork_method_call(pw, "xml import patch", i);
  }

  node_down (i);
  while (i->node) {
    if (!strcmp((char*) i->node->name, "input")) {
      if (pw13_import_patch_input(i, p) != PW13_SUCCESS)
	return PW13_ERROR;
    }
    else if (!strcmp((char*) i->node->name, "output")) {
      if (pw13_import_patch_output(i, p) != PW13_SUCCESS)
	return PW13_ERROR;
    }
    else if (!strcmp((char*) i->node->name, "custom")) {
      pw13_import_patch_custom(i, p);
    }
    else {
      import_error("unknown node <%s> in patch \"%s\"",
		   (char*) i->node->name, p->name);
    }
    next_node(i);
  }
  node_up (i);
  return PW13_SUCCESS;
}


Pw13_Result pw13_import_patch_class (Pw13_Import *i, Pw13_PatchClass *pc)
{
  i=i;
  pc=pc;
  return PW13_SUCCESS;
}


Pw13_Result pw13_import_input (Pw13_Import *i, Pw13_Input *in)
{
  xmlChar *c;
  if (check_node_name (i, "input") != PW13_SUCCESS)
    return PW13_ERROR;
  i->node->_private = in;

  if (!in->name) {
    c = xmlGetProp (i->node, (xmlChar*) "name");
    if (!c)
      return import_error ("missing input name\n");
    in->name = strdup ((char*) c);
    xmlFree (c);
  }
  in->from = NULL;

  node_down (i);
  while (i->node) {
    if (!strcmp ((char*) i->node->name, "data_type")) {
      
    }
    else if (!strcmp ((char*) i->node->name, "defval")) {
      
    }
    else if (!strcmp ((char*) i->node->name, "from")) {
      void *ptr = node_id_ptr (i->node, "output_id");
      if (!ptr) {
	import_error ("missing output_id in node <from> of input \"%s\"",
		      in->name);
      }
      else {
	xmlNode *out_node;
	if (find_node (i->root, "output", ptr, &out_node) != PW13_SUCCESS) {
	  import_error ("output (id=%p) not found "
			"in node <from> of input \"%s\"", ptr, in->name);
	}
	else {
	  Pw13_Output *out = (Pw13_Output*) out_node->_private;
	  if (out) {
	    /*
	    printf ("connected \"%s\" to \"%s\"\n", out->name, in->name);
	    */
	    pw13_connect (out, in);
	    /*
	    pw13_output_print_infos (out);
	    pw13_input_print_infos (in);
	    */
	  }
	  else
	    import_error ("trying to connect NULL output to input \"%s\"\n",
			  in->name);
	}
      }
    }
    else
      import_error ("unknown node <%s> in input \"%s\"",
		    i->node->name, in->name);
    next_node (i);
  }
  node_up (i);
  return PW13_SUCCESS;
}


Pw13_Result pw13_import_output (Pw13_Import *i, Pw13_Output *out)
{
  xmlChar *c;
  if (check_node_name (i, "output") != PW13_SUCCESS)
    return PW13_ERROR;
  i->node->_private = out;

  if (!out->name) {
    c = xmlGetProp (i->node, (xmlChar*) "name");
    if (!c)
      return import_error ("missing output name\n");
    out->name = strdup ((char*) c);
    xmlFree (c);
  }
  out->connected_to = NULL;

  node_down (i);
  while (i->node) {
    next_node (i);
  }
  node_up (i);

  return PW13_SUCCESS;
}


Pw13_Result pw13_import_data_type (Pw13_Import *i, Pw13_DataType *t)
{
  if (check_node_name (i, "data_type") != PW13_SUCCESS)
    return PW13_ERROR;

  t=t;

  return PW13_SUCCESS;
}


Pw13_Result pw13_import_data (Pw13_Import *i, Pw13_Data *d)
{
  if (check_node_name (i, "data") != PW13_SUCCESS)
    return PW13_ERROR;

  d=d;

  return PW13_SUCCESS;
}


Pw13_Result pw13_import_data_block (Pw13_Import *i, Pw13_Data *db)
{
  if (check_node_name (i, "data_block") != PW13_SUCCESS)
    return PW13_ERROR;

  db=db;

  return PW13_SUCCESS;
}
