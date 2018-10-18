#include "error.h"
#include "preprocessor.h"
#include "token.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Args {
  char *input;
  char *output;
  bool eflag;
  bool sflag;
  bool cflag;
  bool help;
};

void usage(void);
bool parse_args(int, char **, struct Args *) __attribute__((nonnull));

void usage(void) {
  puts("Usage: hacek [options] file");
  puts("Options:");
  puts("  -h         Display this information.");
  puts("  -o <file>  Place the output into <file>.");
  puts("  -E         Preprocess only; do not compile, assemble or link.");
  puts("  -S         Compile only; do not assemble or link.");
  puts("  -c         Compile and assemble, but do not link.");
}

bool parse_args(int argc, char **argv, struct Args *args) {
  int opt;

  args->output = "a.out";
  args->eflag = false;
  args->sflag = false;
  args->cflag = false;
  args->help = false;

  while ((opt = getopt(argc, argv, "hESco:")) != -1) {
    switch (opt) {
    case 'h':
      args->help = true;
      break;

    case 'E':
      args->eflag = true;
      break;

    case 'S':
      args->sflag = true;
      break;

    case 'c':
      args->cflag = true;
      break;

    case 'o':
      args->output = optarg;
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
  char *buf;

  if (!parse_args(argc, argv, &args)) {
    return EXIT_FAILURE;
  }

  if (args.help) {
    usage();
    return EXIT_SUCCESS;
  }

  EXIT_MESSAGE_IF(!args.input, "no input file");

  buf = read_from_file(args.input);
  ERROR_IF(!buf, "%s", args.input);

  // preprocess
  preprocess(buf);
  if (args.eflag) {
    return EXIT_SUCCESS;
  }

  // compile
  if (args.sflag) {
    return EXIT_SUCCESS;
  }

  // assemble
  if (args.cflag) {
    return EXIT_SUCCESS;
  }

  // link

  return EXIT_SUCCESS;
}

// vim: set ft=c ts=2 sw=2 et:
