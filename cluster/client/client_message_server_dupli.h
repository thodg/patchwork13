#ifndef PW13_CLUSTER_CLIENT_MESSAGE_DUPLI_H_INCLUDED
#define PW13_CLUSTER_CLIENT_MESSAGE_DUPLI_H_INCLUDED

/* MODE DUPLICATE : */


void pw13_cluster_client_dupli_create_path (char * classpath,
					    char * name, pident id);
void pw13_cluster_client_dupli_destroy_path (Pw13_Patch *p, pident id);

void pw13_cluster_client_dupli_connect_path_local (Pw13_Patch *pdout,
						   Pw13_Patch *pdin,
						   char * output,
						   char * input,
						   pident id);

void pw13_cluster_client_dupli_ask_patchwork_start (Pw13_Patchwork *pw,
						    Pw13_Time *tim,
						    pident id);
void pw13_cluster_client_dupli_ask_patchwork_stop (Pw13_Patchwork *pw,
						   Pw13_Time *tim,
						   pident id);
void pw13_cluster_client_dupli_ask_patchwork_pump (Pw13_Patchwork *pw,
						   Pw13_Time *tim,
						   pident id);

#endif
/* ndef PW13_CLUSTER_CLIENT_MESSAGE_DUPLI_H_INCLUDED */

