#include"pw13_server.h"
#include<stdlib.h>

pw13cc_server pw13cc_new_server ( void )
{
  return ( malloc(sizeof(struct pw13cc_server_s)) );
}
