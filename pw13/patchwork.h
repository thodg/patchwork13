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

  patchwork.h  -  graph of patches, registers data types and patch classes

*/
#ifndef PW13_PATCHWORK_H_INCLUDED
#define PW13_PATCHWORK_H_INCLUDED 1

#include <stdarg.h> /* log */
#include <stdio.h>  /* log */
#include "dll.h"
#include "types.h"
#include "patch.h"
#include "patch_class.h"
#include "input.h"
#include "list.h"
#include "time.h"

struct Pw13_Patchwork {
  Pw13_Patch		*master;	/** Master patch, controls time */
  int			nb_patches;	/** Number of patches */
  Pw13_List		*patches;	/** List of patches */
  Pw13_Time		time;		/** Current time */
  Pw13_MethodName	*method_names;	/** Patchwork specific methods */
};


/** Initialize an allocated patchwork. **/
PW13_EXTERN
void pw13_patchwork_init (Pw13_Patchwork *pw);

/** Destroy a patchwork (without freeing it). **/
PW13_EXTERN
void pw13_patchwork_destroy (Pw13_Patchwork *pw);


/** Write to patchwork log using printf format. **/
PW13_EXTERN
void pw13_patchwork_log (Pw13_Patchwork *pw, const char *msg, ...);


/** Add an existing patch to the patchwork. **/
PW13_EXTERN
void pw13_patchwork_add_patch (Pw13_Patchwork *pw, Pw13_Patch *patch);

/** Safely disconnect and remove a patch from the patchwork. **/
PW13_EXTERN
int pw13_patchwork_remove_patch (Pw13_Patchwork *pw, Pw13_Patch *patch);


/** Start all the patches. **/
PW13_EXTERN
Pw13_Result pw13_patchwork_start (Pw13_Patchwork *pw, Pw13_Time *time);

/** Pump data through all the patches. **/
PW13_EXTERN
void pw13_patchwork_pump (Pw13_Patchwork *pw);

/** Stop all the patches. */
PW13_EXTERN
void pw13_patchwork_stop (Pw13_Patchwork *pw);


/** Topological sort of the patches */
PW13_EXTERN
void pw13_patchwork_topo_sort (Pw13_Patchwork *pw);


/* methods */

PW13_EXTERN
Pw13_Method * pw13_patchwork_method_add (Pw13_Patchwork *pw,
					 const char *name,
					 Pw13_MethodFunc *f,
					 void *method_param);


PW13_EXTERN
Pw13_MethodName * pw13_patchwork_method_find (Pw13_Patchwork *pw,
					      const char *name);


/** Call a patchwork specific method from its name and returns
    how many functions have been called by the method.
*/
PW13_EXTERN
int pw13_patchwork_method_call (Pw13_Patchwork *pw, const char *method_name,
				void *call_param);


#endif
/* not PW13_PATCHWORK_H_INCLUDED */
