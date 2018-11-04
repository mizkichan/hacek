#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdnoreturn.h>

void debug(const char *, const char *, int, int, const char *, ...)
    __attribute__((format(printf, 5, 6), nonnull(1)));
noreturn void debug_abort(const char *, const char *, int, int, const char *,
                          ...)
    __attribute__((format(printf, 5, 6), nonnull(1)));

#define WARN(...) debug(__func__, __FILE__, __LINE__, errno, __VA_ARGS__)
#define ERROR(...) debug_abort(__func__, __FILE__, __LINE__, errno, __VA_ARGS__)
#define PANIC() debug_abort(__func__, __FILE__, __LINE__, errno, NULL)

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
