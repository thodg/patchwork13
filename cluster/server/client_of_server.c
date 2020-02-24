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

  client_of_server.c  -  structure for a client

*/
#ifdef WIN32
#  include "../common/win32.h"
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#endif

#include <unistd.h>
#include <stdio.h>
#include <pthread.h>

#include <pw13/pw13.h>
#include "server.h"
#include "tcp.h"
#include "../common/data_transfer.h"
#include "../common/type_message.h"
#include "client_of_server.h"


void create_patch_client_of_server (client_of_server *cos)
{
  Pw13_Patch *patch_created;
  char *classpath;
  char *name;
 
  printf ("\n\n create patch requested \n\n");

  classpath = recv_string (cos->socketwc);
  name = recv_string (cos->socketwc);

  patch_created = malloc (sizeof (Pw13_Patch));

  printf ("classpath : %s \n", classpath);
  printf ("name : %s \n", name);

  if (pw13_patch_of_classpath (patch_created, classpath, name) 
      == PW13_SUCCESS) {
    /* renvoye le pointeur sur le patch cree */
    printf ("patch created : %p \n", patch_created);
    send_int (cos->socketwc, (int) patch_created); 
  }
}


void create_patch_client_of_server_output (client_of_server *cos)
{
  /* en faite ne sert a rien car on le cree des que ya un bind a distance */
  cos=cos;
  /*
    Pw13_Patch *patch_created;
  char * name;
  
  printf ("\n\n create patch output requested \n\n");

  patch_created = malloc (sizeof (pw13_patch_t));
  
  */
    /* renvoye le pointeur sur le patch cree */
  /*
    printf ("patch_output created : %p \n", patch_created);
    send_int (cos->socketwc, (int) patch_created); 
    }
  */
}


void ask_patch_start_client_of_server (client_of_server *cos)
{
  Pw13_Patch *pd;
  Pw13_Time time;
  
  printf ("\n\n ask_patch_start \n\n");
  
  waitsock_data (cos->socketwc, sizeof (Pw13_Time));
  recv(cos->socketwc, (void*) &time, sizeof (Pw13_Time), 0);
  
  pd = (Pw13_Patch*) recv_int (cos->socketwc);  
  
  if ( pw13_patch_start (pd, &time) )
    printf("patch started \n\n");
}


void ask_patch_stop_client_of_server (client_of_server *cos)
{
  Pw13_Patch *patch;
 
  printf ("\n\n ask_patch_stop \n\n");

  patch = (Pw13_Patch*) recv_int(cos->socketwc);
  printf("\n\n patch to stop :  %p \n", patch);

  pw13_patch_stop (patch);
}


void ask_patch_pump_client_of_server (client_of_server *cos)
{
  Pw13_Patch *patch;
  Pw13_Time time;
  
  printf ("\n\n ask_patch_pump \n\n");
  
  patch = (Pw13_Patch*) recv_int(cos->socketwc);
  printf("\n\n patch to pump :  %p \n", patch);
  
  waitsock_data (cos->socketwc, sizeof (Pw13_Time));
  recv(cos->socketwc, (void*) &time, sizeof (Pw13_Time), 0);
  printf("at time :  %li,%lf \n", time.seconds, time.frac);  

  pw13_patch_pump (patch, &time);
}


void ask_patchwork_start_client_of_server(client_of_server *cos)
{
  Pw13_Time time;
  Pw13_Patchwork *pw;
 
  printf ("\n\n ask_patchwork_start \n\n");
 
  waitsock_data (cos->socketwc, sizeof (Pw13_Time));
  recv(cos->socketwc, (void*) &time, sizeof (Pw13_Time), 0);

  pw = (Pw13_Patchwork*) recv_int (cos->socketwc);
  
  printf ("start patchwork : %p \n", pw);
  printf ("master : %s \n\n", pw->master->name);

  pw13_patchwork_start (pw, &time);
}


void ask_patchwork_stop_client_of_server (client_of_server *cos)
{
  printf("PATCHWORK STOP\n");
 
  pw13_patchwork_stop (cos->pw);
}


void ask_patchwork_pump_client_of_server (client_of_server *cos)
{
  printf("PATCHWORK PUMP\n");
 
  pw13_patchwork_pump (cos->pw);
}


void destroy_patch_client_of_server (client_of_server *cos)
{
  Pw13_Patch *patch;
  
  printf ("\n\n ask patch destroy \n\n");
  
  patch = (Pw13_Patch*) recv_int (cos->socketwc);
  printf ("\n\n patch to destroy : %p \n", patch);

  /* mise a jour de la liste*/
  /* A VERIFIER TOUT CA ... */ 
  seek_and_destroy_patch_client_of_server (cos,patch);

  pw13_patch_destroy (patch);
}



int connect_patch_local_client_of_server (client_of_server *cos)
{
  pw13_cluster_connect_local s_connect_local;
 
  Pw13_Patch *patch_output;
  char* output_name;
  Pw13_Patch *patch_input;
  char* input_name;

  printf (" \n\n connect patch requested \n\n");

  patch_output = (Pw13_Patch*) recv_int (cos->socketwc);
  patch_input = (Pw13_Patch*) recv_int (cos->socketwc);
  
  output_name = recv_string (cos->socketwc);
  input_name = recv_string (cos->socketwc);

  s_connect_local.from = pw13_find_output(patch_output,output_name);
  s_connect_local.to = pw13_find_input(patch_input,input_name);
  
  printf ("output name : %s \n", output_name);
  printf ("input name : %s \n", input_name);
  
  if (! (PW13_SUCCESS == 
	 pw13_connect (s_connect_local.from, s_connect_local.to)))
    return 0;
  
  printf ("\n\n patch connected \n\n");
    
  return 1;
}


void bind_patch_input_client_of_server (client_of_server *cos)
{
  input_server *is; 

  Pw13_Patch *local_patch;
  Pw13_Patch *dest_patch;
  Pw13_Patch *output_patch;
  char *input_id;
  char *output_id;
  char *output_ip;
  char *name;

  struct sockaddr_in their_addr;
  struct hostent *he;
  int socket_input;

  printf ("\n\n bind patch input \n\n");

  is = malloc (sizeof (input_server));  
  
  local_patch = (Pw13_Patch*) recv_int (cos->socketwc); /* input patch */
  dest_patch = (Pw13_Patch*) recv_int (cos->socketwc); /* output patch */
  
  input_id = recv_string (cos->socketwc); /* input */
  output_id = recv_string (cos->socketwc); /* output */
  output_ip = recv_string (cos->socketwc); /* ip de l'autre server */
  
  if ((he = gethostbyname(output_ip)) == NULL) {
    herror ("gethostbyname");
    exit (1);}

  if ((socket_input = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
    perror ("socket");
    exit (1);}
  
  their_addr.sin_family = AF_INET;      /* host byte order */
  their_addr.sin_port = htons(PW13_CLUSTER_SERVER_PORT);
  their_addr.sin_addr = *((struct in_addr *)he->h_addr);
  memset (their_addr.sin_zero, 0, 8);     /* zero pour le reste de struct */
  
  if (connect (socket_input,
               (struct sockaddr *)&their_addr,
               sizeof(struct sockaddr)) == -1){
    perror("connect");
    printf("error connect tcp\n");
    exit (1);
  } else {

    /* envoit a l autre server les infos pour connecter les input/output */
    send_int (socket_input, 0);
    send_int (socket_input, PW13_CONNECT_PATCH_DISTANT);
    send_int (socket_input, (int) dest_patch);
    send_string (socket_input, output_id);
    
    pw13_find_input (local_patch, input_id);  

    /* creation du patch d'output */
    output_patch = malloc (sizeof (Pw13_Patch));
    name = pw13_unique_patch_name (cos->pw, "output_patch");
    if (pw13_cluster_output_patch_class_init (name, output_patch, NULL)
	== PW13_SUCCESS) {
      printf("name output_patch : %s\n", name);
      
      /* connect entre le patch d'output et le patch local de l'input */
      pw13_connect (pw13_find_output (output_patch, "'a"),
		    pw13_find_input (local_patch, input_id));
    }

    printf("test 3 \n");

    /* mise a jour dans le parametre du patch */
    local_patch->param = (void *) is;

    /* ce champ input ne sert a rien j crois en faite */
    is->input = pw13_find_input (local_patch, input_id);  
    is->dest_patch = dest_patch;
    is->output_name = output_id;
    is->socket = socket_input;
    is->client=cos;
    
    pw13_list_insert_tail (&(cos->network_outputs), is);
  }
}


void traitement_message_client_of_server (client_of_server *cos)
{
  int num;
  
  /*printf ("\n\n traitement_message_client_of_server de : %i \n\n", cos->socketwc);*/
  
  num = recv_int (cos->socketwc);
  
  switch (num)
    {
    case PW13_CREATE_PATCH :
      create_patch_client_of_server (cos);
      break;
    case PW13_CREATE_PATCH_OUTPUT :
      create_patch_client_of_server_output (cos);
      break;
    case PW13_DESTROY_PATCH :
      destroy_patch_client_of_server (cos);
      break;
    case PW13_CONNECT_PATCH_LOCAL :
      connect_patch_local_client_of_server (cos);
      break;
    case PW13_BIND_PATCH_INPUT :
      bind_patch_input_client_of_server (cos);
      break;
    case PW13_ASK_PATCH_START:
      ask_patch_start_client_of_server (cos);
      break;
    case PW13_ASK_PATCH_STOP: 
      ask_patch_stop_client_of_server (cos);
      break;
    case PW13_ASK_PATCH_PUMP:
      ask_patch_pump_client_of_server (cos);
      break;
    case PW13_ASK_PW_START:
      ask_patchwork_start_client_of_server (cos);
      break;
    case PW13_ASK_PW_STOP:
      ask_patchwork_stop_client_of_server (cos);
      break;
    case PW13_ASK_PW_PUMP:
      ask_patchwork_pump_client_of_server (cos);
      break;
    default :
      fprintf (stderr,"\ntraitement_message_client_of_server : message inconnu de la socket : %i | num : %i \n", cos->socketwc, num);
    };
}
