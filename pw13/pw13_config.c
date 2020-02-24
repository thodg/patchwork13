#include <stdio.h>
#include <string.h>
#include "pw13-config.h"


int usage (char *argv0)
{
  fprintf (stderr,
	   "%s\n"
	   "Usage: %s (--pw13-library-path | --cflags | --ldflags | --libs)\n",
	     PACKAGE_STRING,
	     argv0);
    return 1;
}


int main (int argc, char *argv[])
{
  if (argc != 2) {
    return usage (argv[0]);
  }
  if (!strcmp (argv[1], "--pw13-library-path"))
    printf ("%s\n", PW13_LIBRARY_PATH);
  else if (!strcmp (argv[1], "--cflags"))
    printf ("%s\n", PW13_CFLAGS);
  else if (!strcmp (argv[1], "--ldflags"))
    printf ("%s\n", PW13_LDFLAGS);
  else if (!strcmp (argv[1], "--libs"))
    printf ("%s\n", PW13_LIBS);
  else
    return usage (argv[0]);

  return 0;
}
