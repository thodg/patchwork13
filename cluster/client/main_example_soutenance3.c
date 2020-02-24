
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
  
  int mode1,mode2,i;

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
	    /* MODE 1-1 : MUSIC */
	    
	    /* demande de creation du patch sound_out */
	    pw13_cluster_client_create_path ("/sdl/mixer/sound_out",
					     "sound_out.pw13.p", tmp);
	    
	    pw13_cluster_client_create_path ("/sdl/mixer/wav_out", 
					     "wav_out.pw13.p", tmp);	      
	    
	    pw13_cluster_client_connect_path_local 
	      (tmp->list_of_patch->suiv->elem, 
	       tmp->list_of_patch->suiv->suiv->elem,
	       "time", "next time", tmp);
	    
	    pw13_cluster_client_connect_path_local 
	      (tmp->list_of_patch->elem, 
	       tmp->list_of_patch->suiv->elem,  
	       "L", "L", tmp);
	    
	    pw13_cluster_client_connect_path_local 
	      (tmp->list_of_patch->elem, 
	       tmp->list_of_patch->suiv->elem,  
	       "R", "R", tmp);
	    
	    printf ("\n\ndemande de start du patchwork\n\n");
	    
	    pw13_cluster_client_dupli_ask_patchwork_start (tmp->pw, 0, tmp);
	    
	    
	  } else { 	    
	    /* MODE 1-0 : PRINTF-OSC */
	    
	    /* demande de creation du patch sin */
	    pw13_cluster_client_create_path ("/std/time/augend",
					     "augend.pw13.p", tmp);
	    
	    printf ("demande de patch print a cree\n");
	    pw13_cluster_client_create_path ("/std/float/print", 
					     "print.pw13.p", tmp);
	    printf ("demande de patch augend a cree\n");
	    pw13_cluster_client_create_path ("/std/float/osc", 
					     "osc.pw13.p", tmp);
	    
	    pw13_cluster_client_connect_path_local 
	      (tmp->list_of_patch->suiv->suiv->elem, 
	       tmp->list_of_patch->suiv->suiv->suiv->elem,
	       "t", "next time", tmp);
	    
	    pw13_cluster_client_connect_path_local 
	      (tmp->list_of_patch->suiv->elem, 
	       tmp->list_of_patch->suiv->suiv->elem,  
	       "()", "any", tmp);
	    
	    pw13_cluster_client_connect_path_local 
	      (tmp->list_of_patch->elem, 
	       tmp->list_of_patch->suiv->elem,  
	       "osc", "float", tmp);
	    
	    
	    printf ("\n\ndemande de start du patchwork\n\n");
	    
	    pw13_cluster_client_dupli_ask_patchwork_start (tmp->pw, 0, tmp);
	    /*
	      printf("->STOP \n");
	      pw13_cluster_client_ask_patch_stop(tmp->list_of_patch->elem,tmp);  
	    */
	    /* demande d'arret du premier patch */
	    /*  
		  }else {
		  printf("->STOP \n");
		  pw13_cluster_client_ask_patch_stop(tmp->list_of_patch->elem,tmp);
		  printf("->DESTROY \n");
		  pw13_cluster_client_destroy_path(tmp->list_of_patch->elem,tmp);
		  }
		  }
	    */
	      
	    
	    
	  } 
	}
      tmp=tmp->suiv;
    }
    
    tmp = pw13_serverlist;
    /*
      while (1)
      {
      if (tmp->connected) {
      pw13_cluster_client_ask_pump (tmp);
    */ /* printf ("Ask PumP \n"); *//*
	}
	tmp = tmp->suiv;
	if (tmp == NULL)
	  tmp = pw13_serverlist;
      }
				    */
  } else {
    /* MODE 0 : 2 serveurs... */
        
    /* MACHINE 1 = tmp | MACHINE 2 = tmp->suiv */

    /* creation des patch */
    printf("\n->CREATION DES PATCHS AUGEND ET PRINT SUR LA MACHINE 1\n");
    pw13_cluster_client_create_path ("/std/time/augend",
				     "augend.pw13.p", tmp);
    pw13_cluster_client_create_path ("/std/float/print", 
				     "print.pw13.p", tmp);
    
    printf("\n->CREATION DU PATCH OSC SUR LA MACHINE 2\n");
    pw13_cluster_client_create_path ("/std/float/osc", 
				     "osc.pw13.p", tmp->suiv);
    
    
    /* connection des patchs augend a master et print a augend en local */
    printf("\n->CONNECTION DU PATCH PRINT A AUGEND ET AUGEND A MASTER EN LOCAL SUR LA MACHINE 1\n");
    pw13_cluster_client_connect_path_local 
      (tmp->list_of_patch->suiv->elem, 
       tmp->list_of_patch->suiv->suiv->elem,
       "t", "next time", tmp); 
    pw13_cluster_client_connect_path_local 
      (tmp->list_of_patch->elem, 
       tmp->list_of_patch->suiv->elem,
       "()", "any", tmp);
    
    /* connection des patchs osc a print en passant par le output patch */
    printf("\n->CONNECTION DU PATCH OSC A PRINT EN PASSANT PAR LE OUTPUT PATCH\n");
    pw13_cluster_client_scat_bind_path_input(tmp->list_of_patch->elem, tmp->suiv->list_of_patch->elem, "float", "osc", tmp->suiv->adresse, tmp);
    
    
    /* start du patchwork sur la machine 1*/
    printf("\n->START DU PATCHWORK SUR LA MACHINE 1\n");
    pw13_cluster_client_dupli_ask_patchwork_start (tmp->pw, 0, tmp);
    
    
    /* start du patch osc  sur la machine 2*/
    printf("\n->START DU PATCH OSC SUR LA MACHINE 2\n");
    pw13_cluster_client_scat_ask_patch_start (tmp->suiv->list_of_patch->elem, 0, tmp);

    
    /* PUMP ifois SUR LA MACHINE 1 */
    for (i = 1; i>0; i--) {
      printf ("\n->ASK PUMP SUR LA MACHINE 1\n");
    }
    
  }
  
  return (0);
}


