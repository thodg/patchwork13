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

  types.h  -  forward types definitions to make the compiler happy

*/
#ifndef PW13_TYPES_H_INCLUDED
#define PW13_TYPES_H_INCLUDED

typedef enum Pw13_Result {
  PW13_ERROR = 0,
  PW13_SUCCESS = 1,
  PW13_FAILURE = 2
} Pw13_Result;

struct Pw13_List;
typedef struct Pw13_List Pw13_List;

struct Pw13_Queue;
typedef struct Pw13_Queue Pw13_Queue;

union Pw13_Data;
typedef union Pw13_Data Pw13_Data;

struct Pw13_Operator;
typedef struct Pw13_Operator Pw13_Operator;

struct Pw13_DataType;
typedef struct Pw13_DataType Pw13_DataType;

struct Pw13_Input;
typedef struct Pw13_Input Pw13_Input;

struct Pw13_Output;
typedef struct Pw13_Output Pw13_Output;

struct Pw13_PatchClass;
typedef struct Pw13_PatchClass Pw13_PatchClass;

struct Pw13_Patch;
typedef struct Pw13_Patch Pw13_Patch;

struct Pw13_Patchwork;
typedef struct Pw13_Patchwork Pw13_Patchwork;

struct Pw13_Time;
typedef struct Pw13_Time Pw13_Time;

struct Pw13_Export;
typedef struct Pw13_Export Pw13_Export;

struct Pw13_Import;
typedef struct Pw13_Import Pw13_Import;


/** Patch constructor callback function */
typedef int (Pw13_PatchConstructor) (const char *name, Pw13_Patch *patch,
				     void *param);

/** Patch start function callback */
typedef int (Pw13_PatchStart) (Pw13_Patch*, Pw13_Time*);

/** Pointer to a patch pump method. */
typedef void (Pw13_PatchPump) (Pw13_Patch*, Pw13_Time*);

/** Pointer to a patch stop method. */
typedef void (Pw13_PatchStop) (Pw13_Patch*);

/** Pointer to a patch destroy method. */
typedef void (Pw13_PatchDestroy) (Pw13_Patch*);


/* Methods */
typedef struct Pw13_MethodName Pw13_MethodName;
typedef struct Pw13_Method Pw13_Method;
typedef void (Pw13_MethodFunc) (Pw13_Method*, void*);


#endif
/* ndef PW13_TYPES_H_INCLUDED */
