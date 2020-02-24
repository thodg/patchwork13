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

  dll.h  -  win32 dll support

*/
#ifndef PW13_DLL_H_INCLUDED
# define PW13_DLL_H_INCLUDED

# ifdef __cplusplus
#  define PW13_EXTERN_C "C"
# else
#  define PW13_EXTERN_C
# endif


# ifdef WIN32
#  define PW13_EXPORT __declspec(dllexport)
#  define PW13_IMPORT extern PW13_EXTERN_C __declspec(dllimport)
# else
#  define PW13_EXPORT extern PW13_EXTERN_C
#  define PW13_IMPORT extern PW13_EXTERN_C
# endif


# ifdef PW13_COMPILATION
#  define PW13_EXTERN PW13_EXPORT   /* the dll exports */
# else
#  define PW13_EXTERN PW13_IMPORT   /* the exe imports */
# endif

#endif
/* ndef PW13_DLL_H_INCLUDED */
