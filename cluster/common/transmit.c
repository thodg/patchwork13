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

  transmit.c  -  send and receive basic types over network

*/
#ifdef WIN32
#  include "win32.h"
#else
#  include <sys/ioctl.h>
#  include <sys/socket.h>
#endif

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <pw13/pw13.h>
#include "transmit.h"


int pf(char * msg) 
{
  printf ("%s \n", msg);
  return 1;
}


void waitsock_data (int socket, int data)
{
  int taille = 0;
  do {
    ioctl (socket, FIONREAD, (void*) &taille);
    pw13_msleep (0);
  } while (taille < data);
}


char * recv_string (int socket)
{
  int nb_char;
  char * res;
  
  waitsock_data  (socket, sizeof (int));
  recv (socket, (void*) &nb_char, sizeof(int), 0);
  
  printf ("taille de la chaine a recevoir : %i \n", nb_char);
  
  res = malloc (nb_char * sizeof (char));
  waitsock_data  (socket, nb_char * sizeof (char));
  recv(socket, (void*) res, nb_char * sizeof (char), 0);
  
  return res;
}


int send_string (int socket,char * msg)
{
  int len;
  
  len = strlen(msg);
  len += 1;
  
  if (send (socket, (void*) &len, sizeof(int), 0) == -1)
    return 0;
  if (send (socket, msg, len*sizeof(char), 0) == -1)
    return 0;
  
  return 1;
}


int recv_int(int socket)
{
  int res;
  
  waitsock_data  (socket, sizeof (int));
  recv (socket, (void*) &res, sizeof(int),0);
    
  return res;
}


int send_int(int socket, int i)
{
  if( send(socket, (void*) &i, sizeof(int), 0 )==-1 ){
    return 0;
  }
  return 1;
}


int pw13_send (int sock_desc, void* data)
{
  unsigned int pos=0;
  int error;
  while( pos < sizeof(*data) ){    
    error = send(sock_desc, data+pos, sizeof(*data),0);
    if( error==-1 )
      return 0;
    pos+=error;
  }
  return 1;
}




