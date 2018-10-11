#include "cpp.h"
#include "error.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>

#define TMP_PATHNAME "/tmp/hacek"

struct PPTokenList preprocess(char *input) {
  FILE *fp = popen(
      "clang -E -std=c17 -Weverything - | sed 's/^#.*$//' > " TMP_PATHNAME,
      "w");
  PANIC_IF(fp == (FILE *)NULL);

  for (char *c = input; *c != '\0'; ++c) {
    ERROR_IF((fputc(*c, fp) == EOF), "fputc() returned EOF");
  }

  WARN_IF(pclose(fp) == -1, "failed to close pipe");

  char *buf = read_from_file(TMP_PATHNAME);
  ERROR_IF(buf == (char *)NULL, TMP_PATHNAME);

  WARN_IF(unlink(TMP_PATHNAME) == -1, TMP_PATHNAME);

  return cpp_tokenize(buf);
}

struct PPTokenList cpp_tokenize(char *input) {
  size_t i = 0;
  size_t token_count = 0;
  struct PPToken *pp_tokens = NULL;

  while (input[i] != '\0') {
    switch (input[i]) {
    case '#':
      push_back(pp_tokens, token_count,
                &(struct PPToken){.kind = PP_PUNCTUATOR, .punctuator = SIGN},
                sizeof(struct PPToken));
      ++token_count;
      ++i;
      break;

    default:
      if (is_nondigit(input[i])) {
        // now it is identifier
      }

      ERROR("unexpected char: %c", input[i]);
    }
  }

  return (struct PPTokenList){.length = token_count, .pp_tokens = pp_tokens};
}

bool is_nondigit(char x) {
  return x == '_' || ('a' <= x && x <= 'z') || ('A' <= x && x <= 'Z');
}

// vim: set ft=c ts=2 sw=2 et:
