#include <pthread.h>

#include "../common/type_message.h"
#include "../common/data_transfer.h"
#include "../common/transmit.h"
#include "output_server.h"


void output_server_traitement_message (output_server *os)
{
  int num;

  printf(" \n\n output_server_traitement_message de : %i \n\n",
	 os->socket);

  num = recv_int (os->socket);
  
  switch (num)
    {
    case PW13_CONNECT_PATCH_DISTANT :
      output_server_connect (os);
      break; 
    case PW13_ASK_PATCH_PUMP :
      ask_pump_from_server (os); 
      break;
    default :
      fprintf (stderr,
	       "traitement_message_client_of_server: "
	       "message inconnu de la socket : %i | msg -> : %i \n",
	       os->socket, num);
    }
}


void output_server_connect (output_server *os)
{ 
  Pw13_Patch *output;
  char *output_id;

  printf("\n\n connect output distant \n\n");
  
  output = (Pw13_Patch*) recv_int (os->socket);
  
  output_id = recv_string (os->socket);

  
  os->output = pw13_find_output(output, output_id);
  
  /* il faudrait remplir le chanp client avec un cos de output_server mais j vois pas trop comment... et un peu osef */
  /*
  pw13_list_insert_tail (&(cos->network_outputs), o);
  */
}


void ask_pump_from_server (output_server *os)
{
  Pw13_Patch *patch;
  Pw13_Time t;
  
  printf ("pump requested\n");
  
  patch = (Pw13_Patch*) recv_int (os->socket);
  waitsock_data (os->socket, sizeof (Pw13_Time));
  recv (os->socket, (void *) &t, sizeof (Pw13_Time), 0);

  if (!pw13_patch_pump (patch, &t))
    printf ("patch non pumped\n\n");
  printf ("patch pumped\n\n");
  
  send_data (os->socket, os->output->data);
  
}


static void pw13_cluster_output_pump (Pw13_Patch *p, Pw13_Time *time)
{ 
  pw13_pump_sources (p, time);
  
  input_server *is;
  is = (input_server *) p->param;
  
  send_int (is->socket, PW13_ASK_PATCH_PUMP);
  send_int (is->socket, (int) is->dest_patch);
  send (is->socket, (void *) time, sizeof(Pw13_Time), 0);

  p->output[0].data = receive_data (is->socket);
  p=p;
  time=time;
}

static int pw13_cluster_output_start (Pw13_Patch *p, Pw13_Time *time)
{
  p=p;
  time=time;
  /* a mettre la conncection tcp qui est dans le bind voir tout le bind_patch_machin */
  return 1;
}


int pw13_cluster_output_patch_class_init (const char *name, Pw13_Patch *p,
					  void *param)
{
  param = param;
  pw13_patch_init (name, p, 0, 1);
  pw13_any_output("'a", p->output, p);
  p->pump = pw13_cluster_output_pump;
  p->start = pw13_cluster_output_start;
  p->param = param;
  return 1;
}
