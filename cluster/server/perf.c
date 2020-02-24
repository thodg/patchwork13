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

  perf.c  -  estimation of server performances

*/
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <pw13/pw13.h>
#include "perf.h"

/* void usleep (unsigned long usec); */


/* calcul de la charge du cpu, la fonction renvoit un pourcentage
   de l'utilisation */
double load_cpu()
{
  double * cpuload;
  unsigned long long load, total, ototal, oload;
  unsigned long long ab, ac, ad, ae;
  int cpt;
  double res;
  FILE * stat;

  cpuload = malloc( (NBR_BCL)*sizeof(double) );
  ototal = oload = 0;

  /* compteur pour etalonner les donnees sur la charge du cpu */  
  for(cpt = NBR_BCL; cpt; cpt-- ) {
    
    /* donnees sur la charge du cpu */
    stat = fopen("/proc/stat", "r");
    if (!stat)
      return 0;
    fscanf(stat, "%*s %llu %llu %llu %llu", &ab, &ac, &ad, &ae);
    fclose(stat);
    
    load = ab + ac + ad;
    total = ab + ac + ad + ae;
    
    /* comparaison avec les donnees precedentes */
    if (ototal == 0) {
      cpuload[cpt-1] = 0.1;
    } else { 
      if ((total - ototal) <= 0) {
	cpuload[cpt-1] = 99;
	
      } else {
	cpuload[cpt-1] = ((load - oload)) / (double)(total - ototal);
      }}
    
    /* stockage de la charge du cpu */
    oload = load;
    ototal = total;
    
    pw13_msleep (30);
  }
  
  /* calcul de la moyenne des differentes valeurs */
  res = 0;
  for( ; cpt < NBR_BCL; cpt++)
    {
      res += cpuload[cpt];
    }
  
  return (res / NBR_BCL);
}

/* calcul de l'espace utilise dans la RAM, la fonction renvoit un pourcentage de l'utilisation */
double load_memory()
{
  unsigned long long total, free;
  FILE * meminfo;

  meminfo = fopen("/proc/meminfo", "r");
  if (!meminfo)
    return 0;
  fscanf(meminfo, "%*s %Ld %*s %*s  %Ld ", &total, &free);
  fclose(meminfo);

  return ( (total - free) / (double)(total) );
}

/* attribution d'un score en fonction du cpu, de la charge du cpu, la quantite de RAM et son utilisation  */
int score()
{
  double lcpu, tcpu = 0, lmem, tmem;
  FILE * file;
  char ligne[301];
  char * pos;
  int c;

  lcpu = 1 - load_cpu();
  lmem = 1 - load_memory();

  /* recuperation du nombre de Mhz maximum du cpu */
  c = 0;
  file = fopen("/proc/cpuinfo", "r");
  if (!file)
    return 0;
  while (c)
  {
    c = (int)fgets (ligne, sizeof(ligne), file);
    if (!strncmp(ligne, "bogomips", strlen("bogomips")))
    {
      pos = strrchr (ligne, ':') +2;
      if (pos[strlen(pos)-1] == '\n') pos[strlen(pos)-1] = '\0';
      strcpy (ligne, pos);
      tcpu = atof (ligne);
      break;
    }
  }
  fclose (file); 

  /* recuperation de la quantite de RAM du system */
  file = fopen("/proc/meminfo", "r");
  fscanf(file, "%*s %lf", &tmem);
  fclose(file);
  
  printf("%lf %lf %lf %lf \n",lcpu, tcpu, lmem, tmem);

  return ((int)(lcpu * tcpu * lmem * tmem));
}
