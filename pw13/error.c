#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "error.h"

Pw13_Result pw13_error (const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  vfprintf (stderr, fmt, ap);
  fputs ("\n", stderr);
  va_end (ap);
  return PW13_ERROR;
}

void pw13_fatal_error (const char *fmt, ...)
{
  va_list ap;
  va_start (ap, fmt);
  vfprintf (stderr, fmt, ap);
  fputs ("\n", stderr);
  va_end (ap);
  exit (-1);
}
