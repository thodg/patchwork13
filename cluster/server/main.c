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

  main.c  -  cluster main

*/
#include <pthread.h>
#include "signals.h"
#include "server.h"
#include "tcp.h"
#include "broadcast.h"


int main ()
{
  pthread_t tcp_mode;

  printf("\n\n-------- SERVER_CLUSTER_PW13 START -------\n\n");

  server_signals ();
 
  pw13_dl_init();

  pthread_create(&tcp_mode, NULL, 
		 TCP_mode, NULL);
  Broadcast_mode();  
  return 0;
}
