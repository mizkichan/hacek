#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdlib.h>

void debug(const char *, const char *, int, int, bool, const char *, ...)
    __attribute__((format(printf, 6, 7), nonnull(1)));

#define WARN(...) debug(__func__, __FILE__, __LINE__, errno, false, __VA_ARGS__)
#define ERROR(...) debug(__func__, __FILE__, __LINE__, errno, true, __VA_ARGS__)
#define PANIC() debug(__func__, __FILE__, __LINE__, errno, true, NULL)

#define WARN_IF(cond, ...)                                                     \
  if (cond)                                                                    \
  WARN(__VA_ARGS__)
#define ERROR_IF(cond, ...)                                                    \
  if (cond)                                                                    \
  ERROR(__VA_ARGS__)
#define PANIC_IF(cond)                                                         \
  if (cond)                                                                    \
  ERROR(#cond)

#endif
// vim: set ft=c ts=2 sw=2 et:
