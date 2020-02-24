#ifndef PW13_CLUSTER_CC_SERVER_H_INCLUDED
#define PW13_CLUSTER_CC_SERVER_H_INCLUDED 1

typedef struct pw13cc_server_s * pw13cc_server;

typedef struct pw13cc_server_s {
  int socket; 
}pw13cc_server_s;


pw13cc_server pw13cc_new_server( void );

#endif
/* ndef PW13_CLUSTER_CC_SERVER_H_INCLUDED */
