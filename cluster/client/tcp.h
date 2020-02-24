#ifndef PW13_CLUSTER_CLIENT_TCP_H_INCLUDED
#define PW13_CLUSTER_CLIENT_TCP_H_INCLUDED

#include "servlist.h"

#define PW13_CLUSTER_SERVER_PORT 7185

void TCPinit (pident id);
int TCPreceive(int *sockfd, void* s,int len);
void TCPsend(int *sockfd, void * paquet,int len);

#endif
/* ndef PW13_CLUSTER_CLIENT_TCP_H_INCLUDED */
