#ifndef __OUTPUT_SERVER_H
#define __OUTPUT_SERVER_H

#include <pthread.h>
#include <pw13/pw13.h>

typedef struct input_server input_server;
typedef struct output_server output_server;

#include "client_of_server.h"

struct input_server {
  Pw13_Input *input;
  Pw13_Patch *dest_patch; /* pointeur sur le patch sur lequel il est connecte */
  char * output_name; /* nom de l'output sur lequel il est connecter */
  int socket;
  client_of_server *client;
};
 

struct output_server {
  Pw13_Output *output;
  int socket;
  pthread_t *thread_rcv;
  client_of_server *client;
};


void output_server_traitement_message (output_server *os);

void output_server_connect (output_server *os);
void ask_pump_from_server (output_server *os);

int pw13_cluster_output_patch_class_init (const char *name, Pw13_Patch *p,
					  void *param);

#endif
/* ndef __OUTPUT_SERVER_H */
