#ifndef PW13_CLUSTER_CLIENT_MESSAGE_SCAT_H_INCLUDED
#define PW13_CLUSTER_CLIENT_MESSAGE_SCAT_H_INCLUDED

/* MODE SCATTER : */

int pw13_cluster_client_scat_create_path (char * classpath,
					  char * name,
					  pident id);
void pw13_cluster_client_scat_destroy_path (Pw13_Patch *p, pident id);

int pw13_cluster_client_scat_create_path_output (pident id);

void pw13_cluster_client_scat_connect_path_local (Pw13_Patch *pdout,
						  Pw13_Patch *pdin,
						  char * output,
						  char * input,
						  pident id);
int pw13_cluster_client_scat_bind_path_input (Pw13_Patch *pd1,
					      Pw13_Patch *pd2,
					      char * input,
					      char * output,
					      char * ip,
					      pident id);

int pw13_cluster_client_scat_ask_patch_start (Pw13_Patch *pd,
					      Pw13_Time *tim,
					      pident id);
int pw13_cluster_client_scat_ask_patch_stop (Pw13_Patch *pd,
					     pident id);
int pw13_cluster_client_scat_ask_patch_pump (pident id, Pw13_Time *tim);

#endif
/* ndef PW13_CLUSTER_CLIENT_MESSAGE_SCAT_H_INCLUDED */

