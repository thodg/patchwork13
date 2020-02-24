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

  perf.h  -  estimation of server performances

*/
#ifndef PW13_CLUSTER_PERF_H_INCLUDED
#define PW13_CLUSTER_PERF_H_INCLUDED

#ifdef WIN32
#  include "../common/win32.h"
#else
#  include <netinet/in.h>
#endif

#define NBR_BCL 5

typedef struct t_noeud_c * p_noeud_c;
typedef struct t_noeud_c {
  char * adresse;
  int * socket;
  p_noeud_c suiv;
} t_noeud_c;

double load_cpu(void);

double load_memory(void);

int score(void);

int Broadcast_reponse(int, struct sockaddr_in *, int);

void display_liste_c ();

p_noeud_c new_noeud_c (char *);

p_noeud_c addclientlist (p_noeud_c *, p_noeud_c);

int is_in_client_list(p_noeud_c *, p_noeud_c);

void deleltclientlist(char *);

int *TCPinterpatch (char *server);

#endif
/* ndef PW13_CLUSTER_PERF_H_INCLUDED */
