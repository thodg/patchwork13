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

  transmit.h  -  send and receive basic types over network

*/
#ifndef PW13_CLUSTER_TRANSMIT_H_INCLUDED
#define PW13_CLUSTER_TRANSMIT_H_INCLUDED


int pf (char * msg);

void waitsock_data (int socket, int data);

int send_string (int sockfd, char *msg);
char * recv_string (int sockfd);

int send_int (int sock, int i);
int recv_int (int socket);


#endif
/* ndef PW13_CLUSTER_TRANSMIT_H_INCLUDED */

