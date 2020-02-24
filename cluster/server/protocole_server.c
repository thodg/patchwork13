#include <pw13/pw13.h>
#include"../common/type_message.h"
#include"protocole_server.h"
#include"../client/tcp.h"
#include"../common/fct_racc.h"
#include"perf.h"

pw13_list liste_patch;
pw13_list liste_patch_local;

/* FICHIER A ENLEVER DE LA SVN A PRIORI IL SERT A RIEN  */


/* remove avec une recherche en itératif + adresses du pére->suiv, ptet caster le list->elem avec (s_succ_pred*) */
void remove_patch_from_list(pw13_list list_patch,pw13_patch p)
{
  pw13_list liste;
  pw13_list * pere_suiv;
 
 

  while( liste && ( liste->elem != p ) ){
    pere_suiv = &liste->suiv;
    liste = liste->suiv;
  }
  
  if (!list){
  pere_suiv = &liste->suiv;
  free(list);
  }

}


pw13_list seek_from_list(pw13_list liste, ,pw13_patch p)
{
  if(liste)
    {
      s_succ_pred *e = (s_succ_pred*) liste->elem;

      if((e->patch) == p)
	{
	  return(liste);
	}
      else
	seek_from_list(liste->suiv,p);
      
    }

  return(NULL);
}
	  


void edit_patch_from_list(pw13_list list_patch,s_succ_pred* s_patch)
{
  pw13_list liste;

  liste = seek_from_list(list_patch, s_patch->patch);
  liste->elem = s_patch;
}







  /*/////////////////////////////////////////
 / Gestion des messages reseau cote server /
/////////////////////////////////////////*/



int message_reseau(pw13_patchwork pw,
		    int * sockfd)
{
  waitsock_data (sockfd,sizeof (int));
  int num;
  recv(*sockfd,&num, sizeof(num),0);
    
  
  switch(num)
    {
    case PW13_CREATE_PATCH :
      {
	pw13_patch s_patch;
	pw13_cluster_create_patch s;
	s_succ_pred l;
	
	waitsock_data (sockfd,sizeof (pw13_cluster_create_patch));
	recv (*sockfd,&s, sizeof (s),0);
	s_patch=pw13_patch_of_classpath(pw,
					s.classpath,
					s.name
					);
	l.patch=s_patch;
	l.succ=NULL;
	l.pred=NULL;
	pw13_list_insert_tail(&liste_patch,&l);
      }
      
    case PW13_DESTROY_PATCH : 
      {
	pw13_cluster_destroy_patch s;
	
	waitsock_data (sockfd,sizeof (pw13_cluster_destroy_patch));
	recv (*sockfd,&s, sizeof (s),0);
	remove_patch_from_list(liste_patch, s.patch);
	pw13_patch_destroy(s.patch);
      }
      
    case PW13_CONNECT_PATCH_LOCAL :
      {
	pw13_cluster_connect_local s;
	pw13_list l;
	s_output_liste_local ol;
	s_input_liste_local il;
	
	waitsock_data (sockfd,sizeof (pw13_cluster_connect_local));
	recv (*sockfd,&s, sizeof (s),0);
	
	if (!(PW13_SUCCESS==
	     pw13_pw_connect (   	
			      s.source,
			      s.output_id,
			      s.dest,
			      s.input_id
			      )  	
	     ))
	  return(0);
	
	/* remplir ol et il */
	

	l=seek_from_list(liste_patch_local,s.source);
	
	s_succ_pred *e = (s_succ_pred*) l->elem;
	
	pw13_list_insert_tail (&(e->succ), &ol);
	pw13_list_insert_tail (&(e->pred), &il);
	
			
      }
      
    case PW13_CONNECT_PATCH_DISTANT :
      {
	pw13_cluster_connect_distant s;
	int* sock_nb;
	int bind_patch_input=PW13_BIND_PATCH_INPUT;
	pw13_list l;
	s_succ_pred *e = (s_succ_pred*) l->elem;
	s_output_liste ol;
	
	waitsock_data  (sockfd,sizeof (pw13_cluster_connect_distant));
	recv (*sockfd,&s, sizeof (s),0);
	sock_nb=TCPinterpatch(s.dest_serv);
	
	ol.o=s.output_id;
	ol.socket=sock_nb;
	
	l=(seek_from_list(liste_patch,s.source));
	pw13_list_insert_tail(&(e->succ),&ol); 
	
	edit_patch_from_list(liste_patch,e);
	
	
	TCPsend(sock_nb,&bind_patch_input,sizeof(int));
	TCPsend(sock_nb,&s.input_id,sizeof(s.input_id));
      }
      
      
      
    case PW13_BIND_PATCH_INPUT :
      {
	pw13_cluster_bind_patch_input s;
	pw13_list l;
	s_succ_pred *e = (s_succ_pred*) l->elem;
	s_input_liste il;
	
	waitsock_data  (sockfd,sizeof (pw13_cluster_bind_patch_input));
	recv (*sockfd,&s, sizeof (s),0);
		
	il.i=s.input_id;
	il.socket=sockfd;

	l=seek_from_list(liste_patch,s.dest);
	pw13_list_insert_tail(&(e->succ),&il);
	
	edit_patch_from_list(liste_patch,e);
		
      }
	
       
    default : return(0);
    }
      
  return 0;
}
