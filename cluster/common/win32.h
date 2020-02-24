/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13_cluster - the patchwork13 cluster
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

  win32.h  -  Definition of missing unix functions in win32

*/
#ifndef PW13_CLUSTER_WIN32_H_INCLUDED
#define PW13_CLUSTER_WIN32_H_INCLUDED

#include <windows.h>

typedef int socklen_t;

#define ioctl(s, cmd, argp) ioctlsocket (s, cmd, argp)
#define close(s) closesocket (s)

#define herror(str) fprintf (stderr, "%s failed\n", str)

/*
#define sleep(n) Sleep ((n)*1000)
#define usleep(n) Sleep ((n)/1000+1)
*/

#endif
/* ndef PW13_CLUSTER_WIN32_H_INCLUDED */
