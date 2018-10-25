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

static void usage(void);
static bool parse_args(int, char **, struct Args *) __attribute__((nonnull));

static void usage(void) {
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
  char *source;
  struct PPToken **pp_tokens;
  struct Token **tokens;

  if (!parse_args(argc, argv, &args)) {
    return EXIT_FAILURE;
  }

  if (args.help) {
    usage();
    return EXIT_SUCCESS;
  }

  EXIT_MESSAGE_IF(!args.input, "no input file");

  // Phase 1.
  source = read_from_file(args.input);
  ERROR_IF(!source, "%s", args.input);

  // Phase 2. Line reconstruction.
  reconstruct_lines(source);

  // Phase 3. Tokenization of the source text into preprocessing tokens.
  pp_tokens = tokenize(source);

  // Phase 4. Execution of preprocessing directives.
  execute_pp_directives(pp_tokens);

  if (args.eflag) {
    // output preprocessed code
    return EXIT_SUCCESS;
  }

  // Phase 5. Escape sequences conversion.
  convert_escape_sequences(pp_tokens);

  // Phase 6. Concatenation adjacent string literals.
  concatenate_adjacent_string_literals(pp_tokens);

  // Phase 7. Conversion of preprocessing tokens into tokens, parsing,
  // translation and assembling.
  tokens = convert_pp_tokens_into_tokens(pp_tokens);
  ast = parse(tokens);
  assembly = translate(ast);

  if (args.sflag) {
    // output assembly code
    return EXIT_SUCCESS;
  }

  if (args.cflag) {
    // output object file
    return EXIT_SUCCESS;
  }

  // Phase 8. Linking
  // output executable file
  return EXIT_SUCCESS;
}

// vim: set ft=c ts=2 sw=2 et:
