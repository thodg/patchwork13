/*

  This is part of Patchwork13! the real-time data synthesis toolkit.
  http://patchwork13.sourceforge.net/

  pw13_std - the patchwork13 standard patches library
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

  of_samples.pw13p.c  -  extracts the frequencies of a signal

*/
#include <fftw3.h>
#include <pw13/pw13.h>
#include <std/float.pw13dt.h>
#include <math.h>

#define NB_SAMPLES 20
#define NB_PUMP 0

int pos=0,abs_pos=0;


typedef struct	patch_fft
{
  double	*in;
  fftw_complex	*out;
  fftw_plan	plan;
}		s_patch_fft;


static void patch_pump (Pw13_Patch *p, Pw13_Time *time)
{
  int i;
  s_patch_fft *f = (s_patch_fft *) (p->param);
  Pw13_Data *res_r = p->output[1].data;
  Pw13_Data *res_c = p->output[2].data;

  pw13_pump_sources (p, time);

  if (abs_pos > NB_PUMP) {
    if (pos < NB_SAMPLES) {
      f->in[pos]=pw13_float_input_val (p->input);
      pos+=1;
    }
    else {
      for(i=0;i<NB_SAMPLES;i++) {
	res_r[i+1].f=f->out[i][0];
	res_c[i+1].f=f->out[i][1];
      }
      fftw_execute(f->plan);
      for(i=0;i<NB_SAMPLES;i++) {
	abs_pos=0;
	pos=0;
      }
    }
    abs_pos+=1;
  }
}

static void patch_destroy (Pw13_Patch *p)
{
  s_patch_fft *f = (s_patch_fft *) p->param;
  fftw_destroy_plan(f->plan);
  fftw_free(f->in);
  fftw_free(f->out);
}

static void patch_stop(Pw13_Patch *p)
{
  s_patch_fft *f = (s_patch_fft *) p->param;

  fftw_destroy_plan(f->plan);
  fftw_free(f->in);
  fftw_free(f->out);
}

static int patch_start(Pw13_Patch *p, Pw13_Time *time)
{
  s_patch_fft *f;
  pos = 0;
  abs_pos = 0;
  time = time;
  f = malloc(sizeof (s_patch_fft));
  f->in = (double*) fftw_malloc(sizeof(double) * NB_SAMPLES);
  f->out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * NB_SAMPLES);
  f->plan = fftw_plan_dft_r2c_1d(NB_SAMPLES, f->in, f->out, FFTW_ESTIMATE);
  p->param = f;

  return 1;
}

static void p_init_params (Pw13_Patch *p)
{
  int i;
  Pw13_Data *d;
  p->output[0].data = pw13_data_array (NB_SAMPLES);
  p->output[1].data = pw13_data_array (NB_SAMPLES);
  p->output[2].data = pw13_data_array (NB_SAMPLES);
  d = p->output[0].data;
  for (i = 1; i <= NB_SAMPLES; i++)
    d[i].f = i / (2 * M_PI);
}

PW13_EXPORT
int pw13_dl_patch_class_init(const char *name, Pw13_Patch *p, void *param)
{
  pw13_patch_init (name, p, 1, 3);
  pw13_float_input ("Signal", p->input, p, 0);
  pw13_any_output ("Frequencies", p->output, p);
  pw13_any_output ("Real Coeff.", p->output+1, p);
  pw13_any_output ("Complex Coeff.", p->output+2, p);
  p->pump = patch_pump;
  p->destroy = patch_destroy;
  p->start = patch_start;
  p->stop = patch_stop;
  param = param;
  p_init_params(p);
  return 1;
}
