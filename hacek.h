#ifndef HACEK_H
#define HACEK_H

#include <stdbool.h>

struct Args {
  char *input;
  char *output;
  bool help;
};

void usage(void);
bool parse_args(int, char **, struct Args *);

#endif
// vim: set ft=c ts=2 sw=2 et:
