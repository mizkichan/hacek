#include "alloc.h"
#include "error.h"
#include "lexer.h"
#include "parser.h"
#include "preprocessor.h"
#include "utils.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

struct Pathname {
  char *pathname;
  char **include_dirs;
  char **lib_dirs;
  char **libs;
};

struct Definition {
  char *name;
  char *value;
};

struct Args {
  bool help : 1;
  bool compile_only : 1;
  struct Definition **definitions;
  bool preprocess_only : 1;
  bool debug : 1;
  char *optlevel;
  char *outfile;
  bool strip : 1;
  char **undefines;
  struct Pathname **pathnames;
};

static void usage(void);
static bool parse_args(int, char **, struct Args *) __attribute__((nonnull));

static void usage(void) {
  puts(" _            \\\\//    _");
  puts("| |__   __ _  _\\/ ___| | __");
  puts("| '_ \\ / _` |/ __/ _ \\ |/ /");
  puts("| | | | (_| | (_|  __/   <");
  puts("|_| |_|\\__,_|\\___\\___|_|\\_\\");
  puts("");
  puts("Usage: hacek [options...] pathname [[pathname] [-I directory]");
  puts("             [-L directory] [-l library]]...");
  puts("Options:");
  puts("  -h               Show this information.");
  puts("  -c               Compile but do not link.");
  puts("  -D name[=value]  Define name to value or 1 if value is omitted.");
  puts("  -E               Preprocess but do not compile.");
  puts("  -g               Produce debug information.");
  puts("  -I directory     Add directory to include search paths.");
  puts("  -L directory     Add directory to library search paths.");
  puts("  -l library       Add library.");
  puts("  -O optlevel      Set optimization level (default is 0).");
  puts("  -o outfile       Write output to outfile.");
  puts("  -s               Produce stripped binary.");
  puts("  -U name          Undefine name.");
}

bool parse_args(int argc, char **argv, struct Args *args) {
  int opt;
  size_t pathnames_count = 0, d_count = 0, u_count = 0;

  args->help = false;
  args->compile_only = false;
  args->definitions = NULL;
  args->preprocess_only = false;
  args->debug = false;
  args->optlevel = "0";
  args->outfile = "a.out";
  args->strip = false;
  args->undefines = NULL;
  args->pathnames = NULL;

  while ((opt = getopt(argc, argv, "hcD:EgO:o:sU:")) > 0) {
    struct Definition *buf;
    char *equal;

    switch (opt) {
    case 'h':
      args->help = true;
      break;
    case 'c':
      args->compile_only = true;
      break;
    case 'D':
      buf = MALLOC(sizeof(struct Definition));
      if ((equal = search_char(optarg, '='))) {
        buf->name = clone_str(optarg, equal);
        buf->value = equal + 1;
      } else {
        buf->name = optarg;
        buf->value = "1";
      }
      PUSH_BACK(struct Definition *, args->definitions, d_count, buf);
      break;
    case 'E':
      args->preprocess_only = true;
      break;
    case 'g':
      args->debug = true;
      break;
    case 'O':
      args->optlevel = optarg;
      break;
    case 'o':
      args->outfile = optarg;
      break;
    case 's':
      args->strip = true;
      break;
    case 'U':
      PUSH_BACK(char *, args->undefines, u_count, optarg);
      break;
    case '?':
      return false;
    default:
      ERROR("opt == %c, optopt == %c", opt, optopt);
    }
  }

  argc -= optind;
  argv += optind;
  optind = 1;
  while (argv[optind - 1]) {
    struct Pathname *buf;
    size_t include_dirs_count = 0;
    size_t lib_dirs_count = 0;
    size_t libs_count = 0;

    buf = MALLOC(sizeof(struct Pathname));
    buf->pathname = argv[optind - 1];
    buf->include_dirs = NULL;
    buf->lib_dirs = NULL;
    buf->libs = NULL;

    while ((opt = getopt(argc, argv, "I:L:l:")) > 0) {
      switch (opt) {
      case 'I':
        PUSH_BACK(char *, buf->include_dirs, include_dirs_count, optarg);
        break;
      case 'L':
        PUSH_BACK(char *, buf->lib_dirs, lib_dirs_count, optarg);
        break;
      case 'l':
        PUSH_BACK(char *, buf->libs, libs_count, optarg);
        break;
      default:
        ERROR("opt == %c, optopt == %c", opt, optopt);
      }
    }

    PUSH_BACK(struct Pathname *, args->pathnames, pathnames_count, buf);
    ++optind;
  }

  return true;
}

int main(int argc, char **argv) {
  struct Args args;
  char *source;
  struct Line **lines;
  struct PPTokenLine **pp_token_lines;
  struct Token **tokens;
  struct AST *ast;

  alloc_init();
  setenv("POSIXLY_CORRECT", "", false);

  if (!parse_args(argc, argv, &args)) {
    return EXIT_FAILURE;
  }

  if (args.help) {
    usage();
    return EXIT_SUCCESS;
  }

  if (!args.pathnames) {
    WARN("no input file");
    return EXIT_FAILURE;
  }

  for (size_t i = 0; args.pathnames[i]; ++i) {
    struct Pathname *pathname = args.pathnames[i];

    // Phase 1.
    source = read_from_file(pathname->pathname);
    ERROR_IF(!source, "%s", pathname->pathname);
    lines = split_source_into_lines(source);

    // Phase 2. Line reconstruction.
    reconstruct_lines(lines);

    // Phase 3. Tokenization of the source text into preprocessing tokens.
    replace_comments(lines);
    pp_token_lines = tokenize(lines);

    // Phase 4. Execution of preprocessing directives.
    execute_pp_directives(pp_token_lines);

    if (args.preprocess_only) {
      // output preprocessed code
      FREE(source);
      return EXIT_SUCCESS;
    }

    // Phase 5. Escape sequences conversion.
    convert_escape_sequences(pp_token_lines);

    // Phase 6. Concatenation adjacent string literals.
    concatenate_adjacent_string_literals(pp_token_lines);

    // Phase 7. Conversion of preprocessing tokens into tokens,
    // parsing, translation and assembling.
    tokens = convert_pp_tokens_into_tokens(pp_token_lines);
    ast = parse(tokens);
    /*
       assembly = translate(ast);
       */

    if (args.compile_only) {
      // output object file
      FREE(source);
      return EXIT_SUCCESS;
    }

    // Phase 8. Linking
    // output executable file

    FREE(source);
  }

  return EXIT_SUCCESS;
}

// vim: set ft=c ts=2 sw=2 et:
