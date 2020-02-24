#ifndef _CLIENT_OF_SERVER_H_INCLUDED_
#define _CLIENT_OF_SERVER_H_INCLUDED_ 

typedef struct client_of_server client_of_server;

#include <pw13/pw13.h>
#include "client_of_server_common.h"
#include "server.h"
#include "output_server.h"
#include "../common/transmit.h"

struct client_of_server{
  Pw13_Patchwork *pw;
  int socketwc;  /* socket connected with the client */
  pthread_t *thread_rcv; /* for the receiving loop message of the client */
  Pw13_List *network_outputs;  
  Pw13_List *network_inputs;
  s_server *server; 
};


void traitement_message_client_of_server (client_of_server *cos);

void create_patch_client_of_server (client_of_server *cos);
void create_patch_client_of_server_output (client_of_server *cos);

void destroy_patch_client_of_server (client_of_server *cos);

int connect_patch_local_client_of_server (client_of_server *cos);
void bind_patch_input_client_of_server (client_of_server *cos);

void ask_patch_start_client_of_server (client_of_server *cos);
void ask_patch_stop_client_of_server (client_of_server *cos);
void ask_patch_pump_client_of_server (client_of_server *cos);

void ask_patchwork_start_client_of_server (client_of_server *cos);
void ask_patchwork_stop_client_of_server (client_of_server *cos);
void ask_patchwork_pump_client_of_server (client_of_server *cos);

     
#endif /* _CLIENT_OF_SERVER_H_INCLUDED_ */

