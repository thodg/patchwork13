/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13_sdl - the patchwork13 standard patches library
  Copyright (C) 2005 Bruno Malaquin <broen.42@gmail.com>

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

  sound_out.pw13p.c  -  SDL sound output patch

*/
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <SDL.h>
#include <SDL_audio.h>
#include <pw13/pw13.h>
#include <std/short.pw13dt.h>

#define TRUE  1
#define FALSE 0
#define NB_BUFFERS 5
#define FMT_FREQ 44100
#define FMT_FORMAT AUDIO_S16
#define FMT_CHANNELS 2
#define FMT_SAMPLES 1024

typedef struct lstream
{
  struct lstream * suiv;
  short * buff;
}lstream;

typedef struct sound_out
{
  lstream * read;
  lstream * write;
  int first;
  int wpos;
  int nb_buff;
  SDL_AudioSpec fmt;
  Pw13_Time delta_time;
}sound_out;

static void add_buff(lstream *blist)
{
  lstream *buff = malloc(sizeof (lstream));
  short *biff = malloc((FMT_SAMPLES * FMT_CHANNELS) *sizeof (short));
  buff->buff = biff;
  buff->suiv = blist->suiv;
  blist->suiv = buff;
}

static void sound_callback(void *userdata, Uint8 *stream, int len)
{
  sound_out *so = (sound_out*) userdata;

  while (so->read == so->write)
    {
      SDL_Delay(1);
    }
  memcpy(stream, so->read->buff, len);
  so->read = so->read->suiv;
}

static void pw13_sdl_sound_out_pump (Pw13_Patch *p, Pw13_Time *time)
{
  sound_out *so = p->param;
  Pw13_Time *out_time;
  pw13_pump_sources (p, time);
  out_time = pw13_time_output_val (p->output[0]);
 
  if (so->wpos == (FMT_CHANNELS * FMT_SAMPLES))
    {
      if (so->first)
	{
	  so->first = 0;
	  so->read = so->write;
	  SDL_PauseAudio(0);
	}
      while (so->read == so->write->suiv)
	{
	  SDL_Delay(1);
	}
      so->write = so->write->suiv;
      so->wpos = 0;
    }
  so->write->buff[so->wpos++] =  pw13_short_input_val (p->input);
  so->write->buff[so->wpos++] =  pw13_short_input_val (p->input + 1);

  pw13_time_increase (out_time, &so->delta_time);
}

static int pw13_sdl_sound_out_start (Pw13_Patch *p, Pw13_Time *time)
{
  int i;
  sound_out *so = p->param;
  Pw13_Time *out_time;
  pw13_pump_sources (p, time);

  out_time = pw13_time_output_val (p->output[0]);
  pw13_time_increase (out_time, &so->delta_time);

  so->wpos = 0;
  so->first = 1;

  so->fmt.freq = FMT_FREQ;
  so->fmt.format = FMT_FORMAT;
  so->fmt.channels = FMT_CHANNELS;
  so->fmt.samples = FMT_SAMPLES;
  so->fmt.callback = sound_callback;
  so->fmt.userdata = so;
  so->read = NULL;
  so->wpos = 0;
  so->first = 1;

  so->delta_time.seconds = 0;
  so->delta_time.ticks = 1;
  so->delta_time.frac = 1.0 / so->fmt.freq;

  lstream *bluff = malloc(sizeof (lstream));
  short *bff = malloc((FMT_SAMPLES * FMT_CHANNELS) *sizeof (short));
  bluff->buff = bff;
  bluff->suiv = NULL;
  so->write = bluff;
  so->write->suiv = so->write;

  for (i = 0; i < NB_BUFFERS; ++i)
    {
      add_buff(so->write);
    }


  if(SDL_Init(SDL_INIT_AUDIO) == -1)
    {
      printf("SDL_Init: %s\n", SDL_GetError());
      return 0;
    }

  if (SDL_OpenAudio(&so->fmt, NULL) < 0)
    {
      fprintf(stderr, "Impossible d'acceder a l'audio: %s\n", SDL_GetError());
      return 0;
    }

  if (!(so->first))
    SDL_PauseAudio(0);

  printf("sound_out_start ok ... \n");
  return 1;
}

static void pw13_sdl_sound_out_stop (Pw13_Patch *p)
{
  p=p;
  SDL_PauseAudio(1);
  SDL_CloseAudio();

}

static void pw13_sdl_sound_out_destroy (Pw13_Patch *p)
{
  free(p->param);
}

PW13_EXPORT
int pw13_dl_patch_class_init (const char *name, Pw13_Patch *p, void *param)
{
  param = param;
  pw13_patch_init(name, p, 2, 1);
  pw13_short_input("L", p->input, p, 0);
  pw13_short_input("R", p->input+1, p, 0);
  pw13_time_output("time", p->output, p);
  p->pump = pw13_sdl_sound_out_pump;
  p->start = pw13_sdl_sound_out_start;
  p->stop = pw13_sdl_sound_out_stop;
  p->destroy = pw13_sdl_sound_out_destroy;
  p->param = malloc (sizeof (sound_out));

   printf("init sound ok........ \n");
  return 1;
}
