#include "hacek.h"
#include "utils.h"
#include <getopt.h>
#include <stdio.h>
#include <stdlib.h>

void usage(void) {
  puts("Usage: hacek [options] file...\n"
       "Options:\n"
       "  -h         Display this information.\n"
       "  -o <file>  Place the output into <file>.");
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
  if (args->input == (char *)NULL) {
    fprintf(stderr, "no input file\n");
    return false;
  }

  return true;
}

int main(int argc, char **argv) {
  struct Args args;

  if (!parse_args(argc, argv, &args)) {
    return EXIT_FAILURE;
  }

  if (args.help) {
    usage();
    return EXIT_SUCCESS;
  }

  int bufsize = get_file_size(args.input);
  if (bufsize == -1) {
    return EXIT_FAILURE;
  }

  char buf[bufsize];
  if (!read_from_file(args.input, (size_t)bufsize, buf)) {
    return EXIT_FAILURE;
  }

  // do something

  return EXIT_SUCCESS;
}

// vim: set ft=c ts=2 sw=2 et:
