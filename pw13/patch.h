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

  patch.h  -  Polymorphic structure to generate outputs data from inputs data.

*/
#ifndef PW13_PATCH_H_INCLUDED
#define PW13_PATCH_H_INCLUDED 1

#include "dll.h"
#include "types.h"
#include "error.h"
#include "time.h"
#include "data.h"
#include "list.h"
#include "output.h"
#include "input.h"
#include "patch_class.h"

/**
   Structure of a patch.

   This structure holds all the informations needed to make a patch work.
   You can instanciate a patch from a patch class with patch_of_class.
   See pw_new_patch if the class is registered with a patchwork you want the
   patch added to.

   On a lower level, you would call patch_init to manually create the patch.
*/

struct		    Pw13_Patch {
  const char	    *name;	    /** Name of the patch (malloc'ed). */
  Pw13_PatchClass   *patch_class;   /** Pointer to the class instanciated. */
  int		    started;	    /** Has the patch been started ? */
  Pw13_PatchPump    *pump;	    /** Pointer to pump method. */
  Pw13_PatchStart   *start;	    /** Pointer to start method. */
  Pw13_PatchStop    *stop;	    /** Pointer to stop method. */
  Pw13_PatchDestroy *destroy;	    /** Pointer to destroy method. */
  Pw13_MethodName   *method_names;  /** List of patch specific methods */
  void		    *param;	    /** You can store additional data here. */
  Pw13_Input	    *input;	    /** Malloc'ed array of t_input. */
  unsigned int	    nb_inputs;	    /** Number of inputs . */
  Pw13_Output	    *output;	    /** Malloc'ed array of t_output. */
  unsigned int	    nb_outputs;	    /** Number of outputs. */
  Pw13_Patchwork    *pw;	    /** Pointer to the parent patchwork. */
  unsigned long     last_pump_tick; /** Needed to avoid pumping twice. */
  int		    topo;	    /** Needed for topological sort */
  void		    *custom_data;   /** Custom data (for the application,
					not for the patch) */
};


/** Initialize a patch.
    This is a low level function to initialize a patch, you should
    only use this if you are writing a patch or if you want to write
    your own higher level functions.
    It allocates the inputs and outputs arrays, however you
    must initialize the elements of the arrays yourself.
    The pointer to the patch must be valid (ie it can be static or dynamic
    but you're in charge).
 */
PW13_EXTERN
void pw13_patch_init (const char *name, Pw13_Patch *p,
		      unsigned int nb_inputs, unsigned int nb_outputs);

/** Change the name of a patch.
    Call this function to change the name of an existing patch.
**/
PW13_EXTERN
void pw13_patch_set_name (Pw13_Patch *p, const char *name);

/** Low level function to start a patch.
    You must call this before pumping the patch (you may then
    pump the patch as many times as you want before calling patch_stop).

    It actually calls the function pointed by patch::init.
    This is called on the master patch by a patchwork
    when starting all the patches.
*/
PW13_EXTERN
int pw13_patch_start (Pw13_Patch *p, Pw13_Time *t);

/** Low level function to pump the patch data.
    Call this to pump the data from the inputs and compute
    the outputs data.

    It actually calls the function patch::pump points to, but only if
    the time is different from the previous pump time.

    This avoids to pump twice the same data and thus allows us to
    use a deep first search to pump a patchwork.

    Returns 1 if the patch was pumped, 0 otherwise.
*/
PW13_EXTERN
int pw13_patch_pump (Pw13_Patch *p, Pw13_Time *time);

PW13_EXTERN
void pw13_patch_force_pump (Pw13_Patch *p, Pw13_Time *time);

/** Low level function to stop a patch.
    Use this to cleanup after pumps (close files, free memory...).
    It calls the patch stop member function (patch::stop).
*/
PW13_EXTERN
void pw13_patch_stop (Pw13_Patch *p);

/** Function to destroy a patch.
    Use this function to destroy a patch, when you don't need it anymore.
    It calls the function pointed by the patch
    destroy member (patch::destroy).
*/
PW13_EXTERN
void pw13_patch_destroy (Pw13_Patch *p);


/** Default patch start method.
    The default patch does nothing when starting.
    patch_init sets the patch start method to this.

    You are free to change it when you write a patch init function,
    because it does nothing.
*/
PW13_EXTERN
int pw13_patch_start_default (Pw13_Patch *p, Pw13_Time *time);

/** Default patch pump method.
    The default patch pumps nothing : it has no input/output.
    You are free to change it when you write a patch init function,
    because it does nothing.
*/
PW13_EXTERN
void pw13_patch_pump_default (Pw13_Patch *p, Pw13_Time *time);

/** Default patch stop method.
    The default patch does nothing when stoping.

    You are free to change it when you write a patch init function,
    because it does nothing.
*/
PW13_EXTERN
void pw13_patch_stop_default (Pw13_Patch *p);

/** Default patch destroy method.
    You must change it in a patch init function to destroy your
    patch params.
*/
PW13_EXTERN
void pw13_patch_destroy_default (Pw13_Patch *p);


/** Low level macro to pump patch sources.
    Call patch_pump on every patch connected to an input of a given patch.

    Returns 1 if any source patch was pumped, 0 otherwise.
*/
PW13_EXTERN
int pw13_pump_sources (Pw13_Patch *p, Pw13_Time *t);


/** Count the connected inputs of a patch */
PW13_EXTERN
int pw13_nb_connected_inputs (Pw13_Patch *p);


PW13_EXTERN
void pw13_patch_print_infos (Pw13_Patch *p);


PW13_EXTERN
Pw13_Method * pw13_patch_method_add (Pw13_Patch *p, const char *name,
				    Pw13_MethodFunc *f, void *method_param);


PW13_EXTERN
Pw13_MethodName * pw13_patch_method_find (Pw13_Patch *p, const char *name);


/** Call a patch specific method from its name and returns
    how many functions have been called by the method.
*/
PW13_EXTERN
int pw13_patch_method_call (Pw13_Patch *p, const char *method_name,
			    void *call_param);


#endif
/* ndef PW13_PATCH_H_INCLUDED */
