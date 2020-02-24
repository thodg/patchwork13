
#include <sys/types.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#ifdef WIN32
#  include "../common/win32.h"
#else
#  include <sys/socket.h>
#endif

#include "tcp.h"
#include "../common/broadcast.h"
#include "../common/transmit.h"
#include "servlist.h"
#include "client_message_server.h"
#include "client_message_server_scat.h"
#include "client_message_server_dupli.h"
#include "broadcast.h"


void * Bcast_thread (void *param)
{
  pident *servlist = (pident*) param;
  pw13cc_Bcast (servlist, NULL, NULL);
  return NULL;
}


int main ()
{
  FILE * ip_list;
  
  int mode1,mode2;

  ip_list = fopen("IP_server","w+"); 
  /* TODO : recuperation de la liste des anciens serveurs */
  fclose (ip_list);

  pident tmp;
 

  pthread_t thread_Bcast;
  pthread_create(&thread_Bcast, NULL, Bcast_thread, &pw13_serverlist);

  sleep (2);
 
  tmp = pw13_serverlist;
  while (tmp) {
    if ( !tmp->connected ) {
      tmp->connected = 1;
      TCPinit (tmp);
    }
    tmp = tmp->suiv;
  }
  
  sleep(1);
  
  tmp = pw13_serverlist;
  
  mode1 = 1;
  mode2 = 0;
  /*
    MODE 1 = 
    0 : 2 serveurs reagissent entre eux 
    1 : tous les serveurs font la meme serie de chose
    
    MODE 2 = 
    0 : lisage d'un son sur tous les pc 
    1 : printf de l'osc sur tous les pc
  */
  if (mode1) {
    /* MODE 1 : tous les serveurs... */
    while (tmp) {
      
      if(tmp->connected)
	{
	  printf("socket : %i server->socket : %i  \n", tmp->socket, tmp->server->socket);
	  
	  if (!mode2) {
	    	    
	  } else { 	    
	    	    
	  } 
	}
      tmp=tmp->suiv;
    }
    
  } else {
    /* MODE 0 : 2 serveurs... */
        
    /* MACHINE 1 = tmp | MACHINE 2 = tmp->suiv */

  }
  
  
  
  return 0;
}


