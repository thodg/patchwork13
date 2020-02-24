#define _GNU_SOURCE
#include <stdio.h>
#include <signal.h>
#include "signals.h"

static void server_signal_handler (int signum)
{
  switch (signum) {
  case SIGINT:
    printf ("SIGINT caught\n");
    break;
  }
}

void server_signals ()
{
  signal (SIGINT, server_signal_handler);
}
