#include <stdio.h>
#include"client_of_server_common.h"

void display_list_output(client_of_server *cos)
{
  Pw13_List *current_elt;
  
  current_elt = cos->network_outputs;
  if (!current_elt) 
    printf("nul");
  
  while (current_elt) {
    printf ("%p \n", ((output_server *)current_elt)->output);
    current_elt = current_elt->suiv;
  }
}


void seek_and_destroy_patch_client_of_server (client_of_server *cos,
					      Pw13_Patch *patch)
{
  int i = 0;
  Pw13_List *pere_output;
  Pw13_List *output_courant;

  i = patch->nb_outputs;

  display_list_output(cos);
  
  while (i) {
    pere_output = NULL;
    output_courant = cos->network_outputs;
    while (output_courant &&
	   ((((output_server *)output_courant)->output) != (patch->output+i)) ){
      pere_output = output_courant; 
      output_courant = output_courant->suiv;
    }
    if (output_courant){
      pere_output->suiv = output_courant->suiv;
      /* pthread_exit(output_courant->*pthread_id); */  
      free(output_courant);
      output_courant = pere_output->suiv;
	} 
    i--;
  }
  
  /* regler son compte au different thread */
  
  display_list_output(cos);
}





