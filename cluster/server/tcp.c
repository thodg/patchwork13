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

  tcp.c  -  mode tcp

*/
#ifdef WIN32
#  include "../common/win32.h"
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <unistd.h>
#endif

#include <pw13/pw13.h>
#include "server.h"
#include "tcp.h"
#include "output_server.h"
#include "../common/transmit.h"

void *TCP_mode(void *param)
{
  Pw13_Patchwork *pw;
  struct sockaddr_in my_addr;
  struct sockaddr_in their_addr;
  int sockTCP;
  s_server *server_structure;
  
  pw = malloc ( sizeof(Pw13_Patchwork));
  pw13_patchwork_init (pw);

  my_addr.sin_family = AF_INET;
  my_addr.sin_port = htons (PW13_CLUSTER_SERVER_PORT);
  my_addr.sin_addr.s_addr = INADDR_ANY;
  memset (&(my_addr.sin_zero), 0, 8);
  
  if ((sockTCP = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }

  server_structure = create_init_s_server (sockTCP);

  if (bind (sockTCP, (struct sockaddr *) &my_addr,
	    sizeof(struct sockaddr)) == -1) {
    perror("bind");
    exit(1);
  }
  if (listen (sockTCP, MAXBUFLEN) == -1) {
    perror ("listen");
    exit (1);
  }
  

  TCPaccept (sockTCP, &their_addr, pw, server_structure);
  return param;
}


/* boucle d'acceptation */
int TCPaccept (int sockTCP, struct sockaddr_in *their_addr,
	       Pw13_Patchwork *pw,
	       s_server *server_structure)
{
  int sin_size, new_socket;
  int num;    
  
  while (1) {  /* main accept() loop */
    sin_size = sizeof(struct sockaddr_in);
    if ((new_socket = accept (sockTCP, (struct sockaddr *) their_addr,
			      (socklen_t*) &sin_size)) == -1) {
      printf("error : accept \n");
      continue;
    }
    printf ("serveur: Recu connection de : %s\n",
	    inet_ntoa (their_addr->sin_addr));    
    

    /* num sert a differentier client/output */
    waitsock_data (new_socket, sizeof(int));
    printf("buffer received \n");
    recv (new_socket, (void*) &num, sizeof (num), 0);
    
    if (num) {  /* is client */
      
      send_int (new_socket, (int) pw);
      send_int (new_socket, (int) pw->master);
      
      printf("TCP_loop_receive_client_of_server \n");
      TCP_loop_receive_client_of_server (pw, new_socket, server_structure);
      
    } else {   /* is server */ 

      /* il faudrait regler le remplissable correct de pw, pb si plusieurs pw */
      printf ("\n\n\n              TCP_loop_connect_from_server_to_server\n\n\n");
      TCP_loop_receive_server (pw,  new_socket);
    }
    /* pw13_msleep (100); */
  }
  return 0;
}


/* creation du thread pour recevoir les msg venant du CLIENT */
void TCP_loop_receive_client_of_server (Pw13_Patchwork *pw,
					int sockfd,
					s_server *server_structure)
{
  pthread_t thread_receive;
  client_of_server *cos;
  
  /* cree le client associe et l'ajoute dans la liste des clients du server */

  cos = create_init_s_client_of_server (pw, sockfd, server_structure);

  pthread_create (&thread_receive, NULL, 
		 boucle_message_reseau_client, 
		 cos);

  cos->thread_rcv = &thread_receive;
  
  pw13_list_insert_tail (&server_structure->clients,
			 cos);
  
}


/* creation du thread pour recevoir les msg venant du SERVER */
void TCP_loop_receive_server (Pw13_Patchwork *pw,
			      int sockfd)
{
  pthread_t thread_receive;
  output_server *os;
  pw=pw;
  
  os = malloc (sizeof (output_server));
  os->socket = sockfd;

  pthread_create(&thread_receive, NULL, 
		 boucle_output_server_traitement_message, 
		 os);
  
  os->thread_rcv = &thread_receive;
}


