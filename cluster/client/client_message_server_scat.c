
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



int pw13_cluster_client_scat_create_path (char * classpath, char * name,
					  pident id)
{
  Pw13_Patch *patch;
  
  printf ("CREATE PATH \n");

  send_int (id->server->socket, PW13_CREATE_PATCH );
  send_string (id->server->socket, classpath );
  send_string (id->server->socket, name );
  
  patch = (Pw13_Patch*) recv_int (id->server->socket);
  
  printf("adresse du patch : %p \n", patch);
  if (patch) {
    pw13_list_insert_head (&id->list_of_patch, patch);
    return 1;
  }
  return 0;
}


void pw13_cluster_client_scat_destroy_path (Pw13_Patch *p, pident id)
{ 
  send_int(id->server->socket, PW13_DESTROY_PATCH );
  send_int(id->server->socket, (int) p);
  
  /* tenir la liste a jour */

}


int pw13_cluster_client_scat_create_path_output (pident id)
{
  Pw13_Patch *patch;
  
  printf ("CREATE PATH OUTPUT \n");
  
  send_int (id->server->socket, PW13_CREATE_PATCH_OUTPUT );
  
  patch = (Pw13_Patch*) recv_int (id->server->socket);
  
  printf("adresse du patch_output : %p \n", patch);
  if (patch) {
    pw13_list_insert_head (&id->list_of_patch, patch);
    return 1;
  }
  return 0;
}


void pw13_cluster_client_scat_connect_path_local (Pw13_Patch *pdout,
						  Pw13_Patch *pdin,
						  char *output,
						  char *input, pident id)
{
  send_int(id->server->socket, PW13_CONNECT_PATCH_LOCAL);
	   
  send_int(id->server->socket,(int) pdout); /*patch output*/
  send_int(id->server->socket,(int) pdin); /*patch input*/
  
  send_string (id->server->socket, output);  /*output*/
  send_string (id->server->socket, input);  /*input*/

}


int pw13_cluster_client_scat_bind_path_input (Pw13_Patch *pd1, Pw13_Patch *pd2,
					      char * input,
					      char * output,
					      char * ip, pident id)
{
  send_int(id->server->socket, PW13_BIND_PATCH_INPUT);
  
  send_int(id->server->socket, (int) pd1);  /* input patch */
  send_int(id->server->socket, (int) pd2);  /* output patch */
  send_string(id->server->socket, input);  /* input */
  send_string(id->server->socket, output);  /* output */
  send_string(id->server->socket, ip); /* ip de l'autre server */
 
  return 1;
}


int pw13_cluster_client_scat_ask_patch_start (Pw13_Patch *pd,
					      Pw13_Time *tim, pident id)
{
  send_int(id->server->socket, PW13_ASK_PATCH_START);
  send (id->server->socket, (void*) tim, sizeof(Pw13_Time),0);
  send_int(id->server->socket, (int) pd);
  
  return 1;
}


int pw13_cluster_client_scat_ask_patch_stop (Pw13_Patch *pd, pident id)
{
  send_int(id->server->socket, PW13_ASK_PATCH_STOP);
  send_int(id->server->socket, (int) pd);
  
  return 1;
}


int pw13_cluster_client_scat_ask_patch_pump (Pw13_Patch *pd,
					     Pw13_Time *tim, pident id)
{
  send_int(id->server->socket, PW13_ASK_PATCH_PUMP);
  send_int(id->server->socket, (int) pd);
  send_int(id->server->socket, (int) tim); /* ptet pas en int... */
  /* le time a envoyer aussi ... es-ce que le patch augend est sur le client ou pas aussi...*/

  return 1;
}
