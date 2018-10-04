#include "cpp.h"
#include "defs.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>

#define TMP_PATHNAME "/tmp/hacek"

char *preprocess(char *input) {
  FILE *fp = popen(
      "clang -E -std=c17 -Weverything - | sed 's/^#.*$//' > " TMP_PATHNAME,
      "w");
  PANIC_IF(fp == (FILE *)NULL);

  for (char *c = input; *c != '\0'; ++c) {
    ERROR_IF((fputc(*c, fp) == EOF), "fputc() returned EOF");
  }

  PANIC_IF(pclose(fp) == -1);

  char *buf = read_from_file(TMP_PATHNAME);
  PANIC_IF(buf == (char *)NULL);

  WARN_IF(unlink(TMP_PATHNAME) == -1, TMP_PATHNAME);

  return buf;
}

// vim: set ft=c ts=2 sw=2 et:
