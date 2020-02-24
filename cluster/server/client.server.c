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

  client.server.c  -  TODO: put description here

*/
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>
#include <strings.h>
#include <sys/types.h> 

#ifdef WIN32
#  include "../common/win32.h"
#else
#  include <netdb.h>
#  include <netinet/in.h> 
#  include <sys/socket.h>
#  include <arpa/inet.h>
#  include <sys/wait.h>
#  include <sys/select.h>
#endif

#define PW13_CLUSTER_CLIENT_PORT 7185

  /*//////////////////////////
 / Procedure d'envoi en TCP /
//////////////////////////*/

void TCPsend_interpatch(int *sockfd, void * paquet,int len) 
{
  if (send(*sockfd, paquet, len, 0) == -1)
    {
      perror("send");
    }
}

/*forker ou threader avant ca : */
/*la réception à faire machin comme sur le client avec une fct° équivalente pour les msg et la réception*/
/*
void TCPreceive_interpatch(int * socket
{}
*/
  /*////////////////////////
 / Fermeture de la socket /
////////////////////////*/
void closeTCPsocket_interpatch(int * sockfd)
{
  close(*sockfd);
}





  /*/////////////////////////////////////////
 / Initialisations d'une connection en TCP /
/////////////////////////////////////////*/
int TCPinterpatch (char * ip)
{
  int sock;
  struct sockaddr_in their_addr;
  struct hostent *he;
  
  if ((he=gethostbyname(ip)) == NULL)   /* Info de l'hôte */
    {
      herror("gethostbyname");
      exit(1);
    }
  
  if (( sock = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
      perror("socket");
      exit(1);
    }
  

  their_addr.sin_family = AF_INET;      /* host byte order */
  their_addr.sin_port = htons(PW13_CLUSTER_CLIENT_PORT);
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  memset (their_addr.sin_zero, 0, 8);     /* zero pour le reste de struct */
 
  if (connect (sock,
	       (struct sockaddr *)&their_addr,
	       sizeof(struct sockaddr)) == -1) 
    {
      perror("connect");
      printf("error connect tcp\n");
      exit(1);
    } else {
      printf("\n        !*! CONNECTION TCP INITIALISE !*!\n avec %s\n\n",ip);

      /*TCPrecv_interpatch(sock);*/
    }
  return sock;
}

