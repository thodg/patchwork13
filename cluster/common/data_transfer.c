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

  data_transfer.h  -  send and receive pw13_data over network

*/
#ifdef WIN32
#  include "win32.h"
#else
#  include <sys/ioctl.h>
#  include <sys/socket.h>
#endif

#include <stdio.h>
#include <pw13/pw13.h>
#include "transmit.h"
#include "data_transfer.h"


void send_data_block (int sockfd, Pw13_Data b)
{
  send (sockfd, (void*) &b, sizeof (b), 0);
}


void send_data (int sockfd, Pw13_Data *starting_block)
{  
  send (sockfd, (void*) starting_block,
       (starting_block->nblocks + 1) * sizeof (Pw13_Data), 0);
}


Pw13_Data * receive_data (int sockfd)
{
  Pw13_Data first;
  Pw13_Data *data_array;

  waitsock_data (sockfd, sizeof (Pw13_Data));
  recv (sockfd, (void*) &first, sizeof (Pw13_Data), 0);
  
  printf(" \n \n taille a recevoir 1: %u \n \n ", first.nblocks);

  data_array = pw13_data_array (first.nblocks);

  waitsock_data (sockfd, first.nblocks * sizeof (Pw13_Data));
  recv (sockfd, (void*) (data_array + 1),
	first.nblocks * sizeof (Pw13_Data), 0);

  printf ("data_array=%p   nblocks=%u\n", data_array, data_array->nblocks);

  return data_array;
}
