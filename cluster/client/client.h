#ifndef PW13_CLUSTER_CLIENT_H_INCLUDED
#define PW13_CLUSTER_CLIENT_H_INCLUDED 1

#include <pw13/pw13.h>
#include "servlist.h"

typedef struct client * pclient;
typedef struct client {
  int mode;
  pident servers;
} client;

/* tcl = tete client : pcq on choisit a la tete du client... */

#endif
/* ndef PW13_CLUSTER_CLIENT_H_INCLUDED */
