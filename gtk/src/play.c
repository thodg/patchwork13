/*
  pw13_gtk - A Patchwork13! GTK+2 User Interface
  Copyright (C) 2005  Thomas de Grivel <billitch@yahoo.fr>

  This program is free software; you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation; either version 2 of the License, or
  (at your option) any later version.

  This program is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with this program; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

*

  patchwork.c  -  patchwork window

*/
#include <unistd.h>
#include <glib.h>
#include <pw13/pw13.h>
#include "patchwork.h"
#include "play.h"

#ifdef WIN32
#  include <pw13_cluster/common/win32.h>
#endif

static gpointer pw13_gtk_play_thread (gpointer param)
{
  Pw13_GtkPlay *play = (Pw13_GtkPlay*) param;
  Pw13_Patchwork *pw = &play->pw->pw;
  Pw13_Time time;
  time.seconds = 0;
  time.frac = 0;
  time.ticks = 0;
  if (pw13_patchwork_start (pw, &time) == PW13_SUCCESS)
    printf ("Started patchwork\n");
  else {
    printf ("Failed to start patchwork\n");
    return NULL;
  }
  play->should_play = 1;
  play->playing = 1;
  while (play->should_play) {
    /* debug
    printf ("  pump (time = %f)\n", pw->time);
    play->should_play = 0;
    */
    pw13_patchwork_pump (pw);
  }
  pw13_patchwork_stop (pw);
  printf ("patchwork stopped\n");
  play->playing = 0;
  return NULL;
}


Pw13_GtkPlay * pw13_gtk_play_new (Pw13_GtkPatchwork *pw)
{
  Pw13_GtkPlay *play;
  if (!g_thread_supported ())
    g_thread_init (NULL);
  play = malloc (sizeof (Pw13_GtkPlay));
  if (!play)
    exit (-1);
  play->thread = NULL;
  play->pw = pw;
  play->should_play = 0;
  play->playing = 0;
  return play;
}

int pw13_gtk_play_start (Pw13_GtkPlay *play)
{
  play->thread = g_thread_create (pw13_gtk_play_thread,
				  (gpointer) play,
				  TRUE, NULL);
  if (!play->thread)
    return 0;
  while (!play->playing)
    pw13_msleep(1);
  return 1;
}

void pw13_gtk_play_stop (Pw13_GtkPlay *play)
{
  printf ("Stopping thread... ");
  play->should_play = 0;
  while (play->playing)
    pw13_msleep (1);
}

