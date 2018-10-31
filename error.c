#include "error.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <string.h>

static void vdebug(const char *, const char *, int, int, const char *, va_list)
    __attribute__((format(printf, 5, 0), nonnull(1)));

static void vdebug(const char *func, const char *file, int line, int errnum,
                   const char *format, va_list ap) {
  fprintf(stderr, "%s: %s:%d", func, file, line);

  if (format) {
    fputs(": ", stderr);
    vfprintf(stderr, format, ap);
  }

  if (errnum) {
    fprintf(stderr, ": %s", strerror(errnum));
  }

  fputc('\n', stderr);
}

void debug(const char *func, const char *file, int line, int errnum,
           const char *format, ...) {
  va_list ap;

  va_start(ap, format);
  vdebug(func, file, line, errnum, format, ap);
  va_end(ap);
}

noreturn void debug_abort(const char *func, const char *file, int line,
                          int errnum, const char *format, ...) {
  va_list ap;

  va_start(ap, format);
  vdebug(func, file, line, errnum, format, ap);
  va_end(ap);

  abort();
}

// vim: set ft=c ts=2 sw=2 et:
