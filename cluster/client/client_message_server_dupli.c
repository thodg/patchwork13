
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


#define LOOP(func) while (id) { \
 if ( id->connected ) { \
func \
} id = id->suiv; }



void pw13_cluster_client_dupli_create_path (char * classpath, char * name,
					    pident id)
{
  Pw13_Patch *patch;
  
  printf ("CREATE PATH \n");

  LOOP(
       send_int (id->server->socket, PW13_CREATE_PATCH );
       send_string (id->server->socket, classpath );
       send_string (id->server->socket, name );
       
       patch = (Pw13_Patch*) recv_int (id->server->socket);
       
       pw13_list_insert_head (&id->list_of_patch, patch);
       
       );
}


void pw13_cluster_client_dupli_destroy_path(Pw13_Patch *p, pident id)
{ 
  LOOP(
       send_int(id->server->socket, PW13_DESTROY_PATCH );
       send_int(id->server->socket, (int) p);
       );
  /* tenir la liste a jour */

}


void pw13_cluster_client_dupli_ask_patchwork_start (Pw13_Patchwork *pw,
						    Pw13_Time *tim, pident id)
{

  printf("dupli : PATCHWORK START");
  
  LOOP(
       send_int(id->server->socket, PW13_ASK_PW_START); 
       send (id->server->socket, (void*) tim, sizeof(Pw13_Time),0); 
       send_int(id->server->socket, (int) pw);
       );  
  
}

void pw13_cluster_client_dupli_ask_patchwork_stop (Pw13_Patchwork *pw,
						   pident id)
{
  
  printf("dupli : PATCHWORK STOP");
  
  LOOP( 
       send_int(id->server->socket, PW13_ASK_PW_STOP);
       send_int(id->server->socket, (int) pw);
       );  
  
}


void pw13_cluster_client_dupli_ask_patchwork_pump (Pw13_Patchwork *pw,
						   pident id)
{
  
  printf("dupli : PATCHWORK PUMP");
  
  LOOP( 
       send_int(id->server->socket, PW13_ASK_PW_PUMP);
       send_int(id->server->socket, (int) pw);
       );
  
}


void pw13_cluster_client_dupli_connect_path_local (Pw13_Patch *pdout, Pw13_Patch *pdin
			   , char *output, char *input, pident id)
{

  LOOP(
       send_int(id->server->socket, PW13_CONNECT_PATCH_LOCAL);
       
       send_int(id->server->socket,(int) pdout); /*patch output*/
       send_int(id->server->socket,(int) pdin); /*patch input*/
       
       send_string (id->server->socket, output);  /*output*/
       send_string (id->server->socket, input);  /*input*/
       );
}
