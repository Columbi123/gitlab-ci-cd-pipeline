#ifndef CAT_H
#define CAT_H

#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  int b;
  int e;
  int n;
  int s;
  int t;
  int v;
} Options;

void par_options(int argc, char *argv[], Options *options);
void process_file(FILE *file, Options options);

#endif
