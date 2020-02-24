
  /*////////////////
 /STOCKAGE EXTERNE/
////////////////*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "iplist.h"

void write_iplist(char * ip)
{
  FILE * ip_list;
  
  ip_list = fopen("IP_server","a");
  if( ip_list==NULL ) exit(1);
  
  fputs(ip, ip_list);
  fputc('\n',ip_list);
  fclose(ip_list);
}

char * read_iplist(int nb)
{
  FILE * ip_list;
  char * ip;

  ip = malloc(17*sizeof(char));

  ip_list = fopen("IP_server","r");
  if( ip_list==NULL ) exit(1);
  
  while(nb)
    {
      fgets(ip, 17, ip_list);
      nb--;
    }
  fclose(ip_list);
      
  ip[strlen(ip)-1] = '\0';

  return ip;
}
