#include "hacek.h"
#include "cpp.h"
#include "error.h"
#include "tokens.h"
#include "utils.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void usage(void) {
  PANIC_IF(puts("Usage: hacek [options] file...\n"
                "Options:\n"
                "  -h         Display this information.\n"
                "  -o <file>  Place the output into <file>.") == EOF);
}

bool parse_args(int argc, char **argv, struct Args *args) {
  int opt;
  args->output = "a.out";
  args->help = false;

  while ((opt = getopt(argc, argv, "ho:")) != -1) {
    switch (opt) {
    case 'o':
      args->output = optarg;
      break;

    case 'h':
      args->help = true;
      break;

    case '?':
      return false;
    }
  }

  args->input = argv[optind];
  return true;
}

int main(int argc, char **argv) {
  struct Args args;

  if (!parse_args(argc, argv, &args)) {
    return EXIT_FAILURE;
  }

  EXIT_MESSAGE_IF(args.input == (char *)NULL, "no input file\n");

  if (args.help) {
    usage();
    return EXIT_SUCCESS;
  }

  char *buf = read_from_file(args.input);
  ERROR_IF(buf == (char *)NULL, "%s", args.input);

  // do something

  return EXIT_SUCCESS;
}

// vim: set ft=c ts=2 sw=2 et:
