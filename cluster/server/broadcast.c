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

  broadcast.c  -  server broadcast mode

*/
#ifdef WIN32
#  include "../common/win32.h"
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#endif

#include "server.h"
#include "broadcast.h"


int Broadcast_mode()
{ 
  int sockfd;
  struct sockaddr_in my_addr;    /* mon adresse */
  struct sockaddr_in their_addr; /* Adresse du connecté */
  int addr_len, numbytes;
  char buf[MAXBUFLEN];

  if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1) {
    perror("socket");
    exit(1);
  }
  my_addr.sin_family = AF_INET;         /* host byte order */
  my_addr.sin_port = htons(PW13_BROADCAST_SERVER_PORT);  /* short, network byte order */
  my_addr.sin_addr.s_addr = INADDR_ANY; /* auto-fill with my IP */
  memset (&(my_addr.sin_zero), 0, 8);        /* zero pour le reste de struct */

  if (bind(sockfd, (struct sockaddr *)&my_addr, sizeof(struct sockaddr)) == -1) {
    perror("bind");
    exit(1);
  }  
  while (1)
    {
      addr_len = sizeof(struct sockaddr);
      if ((numbytes=recvfrom(sockfd, buf, MAXBUFLEN, 0,
			     (struct sockaddr *)&their_addr,
			     (socklen_t*)&addr_len)) == -1) {
	perror("recvfrom");
	exit(1);
      }
      buf[numbytes] = '\0';    
      printf("** Recu un paquet de : %s - contenant : %s **\n",
	     inet_ntoa(their_addr.sin_addr), buf);
      
      if ( !strncmp(buf, "#1#1#", 4) ) {
	/* Reponse au paquet de Broadcast */
	Broadcast_reponse(sockfd, &their_addr, numbytes);
      }
    }
  return 0;
}


/* envoit du packet de reponde de broadcast #1#2#+score */
int Broadcast_reponse (int sockfd,
		       struct sockaddr_in * their_addr,
		       int numbytes)
{ 
  char * msg; /* le message qui va etre envoye */
  msg = malloc(42*sizeof(char));
  
  printf ("avant score\n");
  sprintf(msg, "#1#2#%i#", score());
  printf("Reponse du #1#1# : %s \n", msg);
  if ((numbytes=sendto (sockfd, msg, strlen(msg)+1, 0,
			(struct sockaddr *)their_addr,
			sizeof(struct sockaddr))) == -1) {
    perror("sendto");
    exit(1);
  }
  printf("Send %d octets a %s\n",numbytes,inet_ntoa(their_addr->sin_addr));
  return 1;
}
