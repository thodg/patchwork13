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

  server.c  -  cluster server

*/
#ifdef WIN32
#  include "../common/win32.h"
#else
#  include <sys/types.h>
#  include <sys/socket.h>
#  include <netinet/in.h>
#  include <arpa/inet.h>
#endif

#include <stdio.h>
#include "server.h"
#include "output_server.h"
#include "../common/transmit.h"
#include "client_of_server.h"


int nbclient = 0;
p_noeud_c clientlist = NULL;

Pw13_Patchwork *pw;


void *boucle_message_reseau_client (void *param)
{
  client_of_server *cos;

  cos = (client_of_server *) param;
  while (1){
    traitement_message_client_of_server (cos);
  }
}


void *boucle_output_server_traitement_message (void *param)
{
  output_server *os; 

  os = (output_server *) param;
  while (1){
    output_server_traitement_message (os);
  }
}
      

s_server *create_init_s_server (int socket)
{
  s_server *server_structure;
  
  server_structure = malloc (sizeof (s_server));
  if (!server_structure)
    return NULL;
  server_structure->sockfd = socket;
  
  return server_structure;
}


client_of_server * create_init_s_client_of_server (Pw13_Patchwork *pw,
						  int socket,
						  s_server *server)
{
  client_of_server *cos;
  if ( !(cos = malloc (sizeof (client_of_server)))) {
    return NULL ;
  }
  
  cos->pw = pw;
  cos->socketwc = socket;
  cos->network_outputs = NULL;
  cos->network_inputs = NULL;
  cos->server = server;

  return cos;
}



void display_liste_c()
{
  p_noeud_c tmp;
  int cpt = 1;
  
  tmp = clientlist;
  printf ("\n");
  while (tmp != NULL) {
    printf ("%i - %s \n",cpt, tmp->adresse);
    cpt++;
    tmp = tmp->suiv;
  }
}


  /*/////////////////////////////////
 / Gestion de la liste des clients /
/////////////////////////////////*/

/* construit un noveau client */
p_noeud_c new_noeud_c (char *addr)
{
  p_noeud_c new_elt;
  if (! (new_elt = malloc (sizeof (t_noeud_c))) )
    exit (1);

  new_elt->adresse = (char*) malloc (strlen (addr) + 1);
  strcpy (new_elt->adresse, addr);
  new_elt->socket = malloc(sizeof(int));
  new_elt->suiv = NULL;

  return new_elt;
}

/* ajoute un client qui n'est pas dans la liste à la fin de la liste */
p_noeud_c addclientlist(p_noeud_c *list, p_noeud_c elt)
{
  while (*list && strcmp((*list)->adresse, elt->adresse)) {
    list = &(*list)->suiv;
  }
  if (!*list) {
    *list = elt;
    nbclient++;
    printf("** addclientlist : %s :-: %i **\n",elt->adresse,*(elt->socket));
  }
  /* display(); */
  return (elt); 
}

/* est ou est pas dans la liste des clients ? */
int is_in_client_list(p_noeud_c *list, p_noeud_c elt)
{
  while( *list && strcmp((*list)->adresse, elt->adresse)) {
    list = &(*list)->suiv;
  }
  if(!*list) {
    return 0; /* pas ds la liste */
  } 
  return 1; /* ds la liste */
}


/* effacement d'un element de la liste des servers */
void deleltclientlist(char * addr)
{
  p_noeud_c tmp, elt_del;

  tmp = clientlist;
  while ( tmp->suiv->adresse != addr && tmp!=NULL) {
      tmp = tmp->suiv;
    }
  if ( tmp != NULL  ) {
    elt_del = tmp->suiv;
    tmp->suiv = elt_del->suiv;
    free(elt_del);
  }
  nbclient--;
}
