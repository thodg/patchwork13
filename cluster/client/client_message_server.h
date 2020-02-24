#ifndef PW13_CLUSTER_CLIENT_MESSAGE_H_INCLUDED
#define PW13_CLUSTER_CLIENT_MESSAGE_H_INCLUDED

#include "client.h"

void pw13_cluster_client_create_path (char * classpath,
				      char * name, pclient tcl);
void pw13_cluster_client_destroy_path (Pw13_Patch *p, pclient tcl);

void pw13_cluster_client_connect_path_local (Pw13_Patch *pdout,
					     Pw13_Patch *pdin,
					     char * output,
					     char * input, pclient tcl);

void pw13_cluster_client_ask_patchwork_start (Pw13_Patchwork *pw,
					      Pw13_Time *tim, pclient tcl);
void pw13_cluster_client_ask_patchwork_stop (Pw13_Patchwork *pw,
					     Pw13_Time *tim, pclient tcl);
void pw13_cluster_client_ask_patchwork_pump (Pw13_Patchwork *pw,
					     Pw13_Time *tim, pclient tcl);


#endif
/* ndef PW13_CLUSTER_CLIENT_MESSAGE_H_INCLUDED */

