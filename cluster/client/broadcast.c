
#include <stdio.h> 
#include <stdlib.h>
#include <errno.h> 
#include <string.h>
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

#include "../common/broadcast.h"
#include "../common/nieme_str.h"
#include "iplist.h"
#include "servlist.h"

#define MAXDATASIZE 100 /* Tampon d'entrée */

int nbserver = 0;


  /*//////////////////////////////////////////////////////////////////////////
 / Envoi d'un paquet en Broadcast et reception du paquet d'authentification /
//////////////////////////////////////////////////////////////////////////*/

int pw13cc_Bcast_init()
{
  int socketDesc;    /* Socket Descriptor */
  
  /* Create socket from which to send */
  if ((socketDesc = socket(AF_INET, SOCK_DGRAM, 0)) < 0) 
    {
      perror("open error on socket");
      exit(1);
    }

  return(socketDesc);
}


void * pw13cc_Bcast (void *servlist,
		     void (*callback) (pident, void*),
		     void *param)
{
  pident *serverlist = (pident*) servlist;
  int socketDesc;
  int addrLength;
  struct sockaddr_in destinationAddr, myAddr, responseAddr;
  struct hostent *hostNameBufPtr;
  char outMessageBuf[128];
  char inMessageBuf[128];
  struct timeval timeOut;
  fd_set readReadySet;
  int msgCount;
  int trueFlag = 0x1;
  pident elt;
  int nb_bcast = 100;
  
  socketDesc = pw13cc_Bcast_init ();
  display (*serverlist);
    
  /* BIND the socket on our end to any available port (i.e.
     leave it up to the system to assign a port (used for
     replys */
  myAddr.sin_family = AF_INET;
  myAddr.sin_addr.s_addr = htonl(INADDR_ANY);
  myAddr.sin_port = htons (PW13_BROADCAST_CLIENT_PORT);
  if (bind(socketDesc, (struct sockaddr *) &myAddr, sizeof(myAddr)) 
      < 0)
    {
      perror("bind error");
      goto end_Bcast;
    }
  
  /* And build BROADCAST address */
  destinationAddr.sin_family = AF_INET;
  destinationAddr.sin_addr.s_addr = htonl(INADDR_BROADCAST);
  destinationAddr.sin_port = htons(PW13_BROADCAST_SERVER_PORT);
  
  /* Set the socket to permit multicasts on the local LAN */
  if (setsockopt(socketDesc, SOL_SOCKET, SO_BROADCAST,
		 (void*) &trueFlag, sizeof(trueFlag)) != 0) 
    {
      perror("setsockopt error");
      goto end_Bcast;
    }
  
  while(nb_bcast--)
    {
      /* Broadcast message */
      strcpy(outMessageBuf, "#1#1#");
      if (sendto(socketDesc, outMessageBuf, strlen(outMessageBuf)+1, 0,
		 (struct sockaddr *) &destinationAddr,
		 sizeof (destinationAddr)) < 0) 
	{
	  perror("socket send error");
	  printf("1");
	}
      
      
      /* And wait for up to 1 seconds for replys to come back */
      /* Set timeOut value for receive to 1 seconds */
      timeOut.tv_sec = 1L;
      timeOut.tv_usec = 0L;
      
      /* Read responses from hosts accepting the multicast until a read 
	 finally times-out. */
      FD_ZERO(&readReadySet);
      FD_SET(socketDesc, &readReadySet);
      msgCount = 0;
      while (select(socketDesc+1,&readReadySet, NULL, NULL, &timeOut) > 0) 
	{
	  addrLength= sizeof(responseAddr);
	  if (recvfrom(socketDesc, inMessageBuf,
		       sizeof(inMessageBuf), 0, 
		       (struct sockaddr *)&responseAddr,
		       (socklen_t*)&addrLength) < 0) 
	    {
	      perror("socket read error");
	      printf("2");
	      goto end_Bcast;
	    }
	  
	  /* Convert the address of the responding machine to its symbolic
	     format */
	  if( !strncmp(inMessageBuf,"#1#2#",4) )
	    {
	      int dejadanslaliste;
	      if ((hostNameBufPtr =
		   gethostbyaddr ((void*) &responseAddr.sin_addr.s_addr,
				  sizeof(responseAddr.sin_addr.s_addr),
				  AF_INET)) == NULL) 
		{
		  perror("error gethostbyaddr");
		}
	      else 
		{
		  printf("Message received from %s\n", hostNameBufPtr->h_name);
		}
	      printf ("recu un paquet de %s\n",
		     inet_ntoa(responseAddr.sin_addr));
	      elt = addserverlist (serverlist,
				   new_ident (inet_ntoa(responseAddr.sin_addr),
					      atoi(nieme_str(inMessageBuf,3))),
				   &dejadanslaliste);
	      if (!dejadanslaliste && callback)
		callback (elt, param);
	      
	      printf("Received Message=%s\n\n",inMessageBuf);
	      msgCount++;
	    }
	  
	  FD_SET (socketDesc, &readReadySet);
	  
	  sleep (1);
	} /* while */
      
      printf ("Received %d messages\n",msgCount);
    }

 end_Bcast:
  close (socketDesc);
  return NULL;
}
