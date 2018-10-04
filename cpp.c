#include "cpp.h"
#include "utils.h"
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define TMP_PATHNAME "/tmp/hacek"

bool write_to_cpp(char *input) {
  FILE *fp = popen(
      "clang -E -std=c17 -Weverything - | sed 's/^#.*$//' > " TMP_PATHNAME,
      "w");
  if (fp == (FILE *)NULL) {
    fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, strerror(errno));
    return false;
  }

  for (char *c = input; *c != '\0'; ++c) {
    if (fputc(*c, fp) == EOF) {
      fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__,
              "fputc() returned EOF");
      return false;
    }
  }

  if (pclose(fp) == -1) {
    fprintf(stderr, "%s:%d: %s\n", __FILE__, __LINE__, strerror(errno));
    return false;
  }

  return true;
}

char *preprocess(char *input) {
  if (!write_to_cpp(input)) {
    return NULL;
  }

  char *result = read_from_file(TMP_PATHNAME);
  if (result == (char *)NULL) {
    return NULL;
  }

  if (unlink(TMP_PATHNAME) == -1) {
    fprintf(stderr, "%s:%d: %s: %s\n", __FILE__, __LINE__, TMP_PATHNAME,
            strerror(errno));
    // return NULL;
  }

  return result;
}

// vim: set ft=c ts=2 sw=2 et:
