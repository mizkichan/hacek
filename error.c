#include "error.h"
#include <stdio.h>
#include <string.h>

void debug(const char *func, const char *file, int line, int errnum, bool exit,
           const char *format, ...) {
  va_list ap;

  fprintf(stderr, "%s:%s:%d", func, file, line);

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

  if (exit) {
    abort();
  }
}

// vim: set ft=c ts=2 sw=2 et:
