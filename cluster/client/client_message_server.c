
#ifdef WIN32
#  include "../common/win32.h"
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#endif

#include <pw13/pw13.h>
#include "../common/type_message.h"
#include "../common/transmit.h"
#include "pw13_server.h"
#include "servlist.h"
#include "broadcast.h"
#include "client.h"
#include "client_message_server_dupli.h"
#include "client_message_server_scat.h"

/* pour le mode scat faire l'appel au server chosit */


int pw13_cluster_client_create_path (char * classpath, char * name, pclient tcl)
{
  
  if( tcl->mode ){
    pw13_cluster_client_dupli_create_path( classpath, name, tcl->servers);
  } else {
    pw13_cluster_client_scat_create_path( classpath, name, tcl->servers);
  }
  
  return 1;
}


void pw13_cluster_client_destroy_path (Pw13_Patch *p, pclient tcl)
{ 
  
  if( tcl->mode ){
    pw13_cluster_client_dupli_destroy_path( p, tcl->servers);
  } else {
    pw13_cluster_client_scat_destroy_path(p, tcl->servers);
  }
  

}

void pw13_cluster_client_connect_path_local (Pw13_Patch *pdout,
					     Pw13_Patch *pdin,
					     char *output,
					     char *input, pclient tcl)
{
 
  if( tcl->mode ){
    pw13_cluster_client_dupli_connect_path_local (pdout, pdin, output,
						  input, tcl->servers);
  } else {
    pw13_cluster_client_scat_connect_path_local (pdout, pdin, output,
						 input, tcl->servers);
  }
  
}


void pw13_cluster_client_ask_patchwork_start (Pw13_Patchwork *pw,
					      Pw13_Time *tim, pclient tcl)
{
  if( tcl->mode ){
    pw13_cluster_client_dupli_ask_patchwork_start (pw, tim, tcl->servers);
  } else {
    /* bon a faire le meme truc que ds la lib avec un appel reseau au moment du patch start*/
    printf("pouet\n");
  }

}


void pw13_cluster_client_ask_patchwork_stop (Pw13_Patchwork *pw, Pw13_Time *tim, pclient tcl)
{

  if( tcl->mode ){
    pw13_cluster_client_dupli_ask_patchwork_stop (pw, tim, tcl->servers);
  } else {
    /* bon a faire le meme truc que ds la lib avec un appel reseau au moment du patch stop*/
    printf("pouet\n");
  }

}


void pw13_cluster_client_ask_patchwork_pump (Pw13_Patchwork *pw, Pw13_Time *tim, pclient tcl)
{

  if( tcl->mode ){
    pw13_cluster_client_dupli_ask_patchwork_pump (pw, tim, tcl->servers);
  } else {
    /* bon a faire le meme truc que ds la lib avec un appel reseau au moment du patch pump*/
    printf("pouet\n");
  }

}


