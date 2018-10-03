#ifndef HACEK_H
#include <stdbool.h>

struct Args {
  char *input;
  char *output;
  bool help;
};

void usage(void);
bool parse_args(int, char **, struct Args *);

#define HACEK_H
#endif
// vim: set ft=c ts=2 sw=2 et:
