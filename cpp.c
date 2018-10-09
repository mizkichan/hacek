#include "cpp.h"
#include "error.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>

#define TMP_PATHNAME "/tmp/hacek"

struct PreprocessingToken *preprocess(char *input) {
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

struct PreprocessingToken *cpp_tokenize(char *input) {}

// vim: set ft=c ts=2 sw=2 et:
