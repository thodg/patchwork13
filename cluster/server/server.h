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

  server.h  -  cluster server

*/
#ifndef _SERVER_H_INCLUDED_
#define _SERVER_H_INCLUDED_ 

#include <pw13/pw13.h>
#include <pthread.h>

typedef struct s_server s_server;

#include "perf.h" 
#include "../common/broadcast.h"
#include "../common/nieme_str.h"
#include "client_of_server.h"
#include "../common/transmit.h"


#define PW13_CLUSTER_SERVER_PORT 7185
#define MAXBUFLEN 100

struct s_server {
  int sockfd;
  Pw13_List *clients;
};

void *boucle_message_reseau_client (void *param);
void *boucle_output_server_traitement_message(void *param);


s_server *create_init_s_server (int sockfd);
client_of_server *create_init_s_client_of_server (Pw13_Patchwork *pw,
						  int socket,
						  s_server *server);

#endif /* _SERVER_H_INCLUDED_ */
