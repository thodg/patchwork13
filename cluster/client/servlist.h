#ifndef PW13_CLUSTER_SERVLIST_H_INCLUDED
#define PW13_CLUSTER_SERVLIST_H_INCLUDED 1

#include <pw13/pw13.h>
#include "pw13_server.h"

typedef struct ident * pident;
typedef struct ident {
  char * adresse;
  int score;
  pident suiv;
  int socket;
  int connected;
  Pw13_Patchwork *pw;
  pw13cc_server server;
  Pw13_List *list_of_patch;
} ident;

void display (pident serverlist);
void triserverlist (pident elt, pident serverlist);
pident new_ident (char *addr, int sco);
pident addserverlist (pident *list, pident elt, int *dejadanslaliste);
void deleltserverlist (char *addr, pident serverlist);

extern pident pw13_serverlist;

#endif
/* ndef PW13_CLUSTER_SERVLIST_H_INCLUDED */
