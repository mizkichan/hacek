#include "error.h"
#include <stdio.h>
#include <string.h>

void debug(char *file, int line, int errnum, const char *format, ...) {
  va_list ap;

  fprintf(stderr, "%s:%d", file, line);

  if (format) {
    fputs(": ", stderr);
    va_start(ap, format);
    vfprintf(stderr, format, ap);
    va_end(ap);
  }

  if (errnum) {
    fprintf(stderr, ": %s", strerror(errnum));
  }

  fputc('\n', stderr);
}

// vim: set ft=c ts=2 sw=2 et:
