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

#include "tcp.h"
#include "../common/transmit.h"

#define MAXDATASIZE 1024

  /*/////////////////////////////////////////
 / Initialisations d'une connection en TCP /
/////////////////////////////////////////*/
void TCPinit (pident id)
{
  struct sockaddr_in their_addr;
  struct hostent *he;
  Pw13_Patch *master;
  int is_client=1;
  
  if ((he=gethostbyname(id->adresse)) == NULL)   /* Info de l'hote */
    {
      herror("gethostbyname");
      exit(1);
    }
  
  if (( id->socket = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
    {
      perror("socket");
      exit(1);
    }
  

  their_addr.sin_family = AF_INET;      /* host byte order */
  their_addr.sin_port = htons(PW13_CLUSTER_SERVER_PORT);
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  memset (their_addr.sin_zero, 0, 8);     /* zero pour le reste de struct */
 
  if (connect (id->socket,
	       (struct sockaddr *)&their_addr,
	       sizeof(struct sockaddr)) == -1) 
    {
      perror("connect");
      printf("error connect tcp\n");
      exit(1);
    } else {
      id->connected = 1;
      printf("\n\nCONNECTION TCP INITIALISE \navec %s\n\n",id->adresse);
      id->server = malloc(sizeof(pw13cc_server_s));
      id->server->socket = id->socket;

      send_int (id->socket, is_client);

      id->pw = (Pw13_Patchwork*) recv_int (id->socket);
      master = (Pw13_Patch*) recv_int (id->socket);
      
      pw13_list_insert_head (&id->list_of_patch, master);
    }
}



  /*////////////////////////
 / Fermeture de la socket /
////////////////////////*/
void closeTCPsocket(int * sockfd)
{
  close(*sockfd);
}
