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

  tcp.h  -  tcp mode

*/
#ifndef PW13_CLUSTER_TCP_H_INCLUDED_
#define PW13_CLUSTER_TCP_H_INCLUDED_ 

#ifdef WIN32
#  include "../common/win32.h"
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#endif

#include <pw13/pw13.h>

void * TCP_mode (void *param);

int TCPaccept (int sockTCP,
	       struct sockaddr_in *their_addr,
	       Pw13_Patchwork *pw,
	       s_server *server_structure);

void TCP_loop_receive_client_of_server(Pw13_Patchwork *pw,
				       int sockfd,
				       s_server *server_structure);


void TCP_loop_receive_server (Pw13_Patchwork *pw,
			      int sockfd);


#endif
/* ndef PW13_CLUSTER_TCP_H_INCLUDED */
