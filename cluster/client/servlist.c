#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "servlist.h"


pident pw13_serverlist = NULL;




void display (pident serverlist)
{
  pident tmp;
  int cpt = 1;
  tmp = serverlist;
  printf("\n");
  while(tmp) {
    printf("%i - %s : %i \n", cpt, tmp->adresse, tmp->score);
    cpt++;
    tmp = tmp->suiv;
  }
}


  /*///////////////////////////////////////////////////////
 / tri de la liste des servers en fonction de leur score /
///////////////////////////////////////////////////////*/

void triserverlist (pident elt, pident serverlist)
{
  pident tmp;
  tmp = serverlist;
  if (tmp && tmp->suiv) {
    if( (tmp->score) < (elt->score) ) {
      elt->suiv = tmp;
      serverlist = elt;
    } else {
    while( (tmp->suiv != NULL) && ((tmp->suiv->score) > (elt->score))) {
      tmp = tmp->suiv;
    }
    if(tmp->suiv != NULL) {
      elt->suiv = tmp->suiv;
      tmp->suiv = elt;
    } }
    tmp = elt->suiv;
    while(tmp->suiv != elt) {
      tmp = tmp->suiv;
    }
    tmp->suiv = NULL;
  }
}


  /*//////////////////////////////////////////////
 / ajout d un element dans la liste des servers /
//////////////////////////////////////////////*/

pident new_ident (char *addr, int sco)
{
  pident new_elt;
  if (! (new_elt = malloc(sizeof(ident))) )
    exit (1);

  new_elt->adresse = (char*) malloc (strlen (addr) + 1);
  strcpy (new_elt->adresse, addr);
  new_elt->score = sco;
  new_elt->suiv = NULL;
  new_elt->connected = 0;

  return new_elt;
}

pident addserverlist(pident *list, pident elt, int *dejadanslaliste)
{
  *dejadanslaliste = 1;
  while (*list && strcmp ((*list)->adresse, elt->adresse)) {
    list = &(*list)->suiv;
  }
  if (!*list) {
    *list = elt;
    *dejadanslaliste = 0;
    /*
      nbserver++;
      triserverlist(elt);
    */
  }
  /* display(); */
  return (elt);
}


  /*/////////////////////////////////////////////////
 / effacement d'un element de la liste des servers /
/////////////////////////////////////////////////*/

void deleltserverlist(char * addr, pident serverlist)
{
  pident tmp, elt_del;
  
  tmp = serverlist;
  while ( tmp->suiv->adresse != addr && tmp!=NULL) {
    tmp = tmp->suiv;
  }
  if ( tmp != NULL  ) {
    elt_del = tmp->suiv;
    tmp->suiv = elt_del->suiv;
    free(elt_del);
  }
  /*
    nbserver--;
  */
}    
