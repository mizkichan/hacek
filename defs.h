#ifndef DEFS_H
#define DEFS_H

#include <err.h>
#include <stdlib.h>

#define WARN(msg) warn("%s:%d: %s", __FILE__, __LINE__, (msg))
#define FAILURE(msg) errx(EXIT_FAILURE, "%s:%d: %s", __FILE__, __LINE__, (msg))
#define ERROR(msg) err(EXIT_FAILURE, "%s:%d: %s", __FILE__, __LINE__, (msg))
#define PANIC() err(EXIT_FAILURE, "%s:%d", __FILE__, __LINE__)

#define FAILURE_IF(cond, msg)                                                  \
  if (cond)                                                                    \
  FAILURE(msg)
#define WARN_IF(cond, msg)                                                     \
  if (cond)                                                                    \
  WARN(msg)
#define ERROR_IF(cond, msg)                                                    \
  if (cond)                                                                    \
  ERROR(msg)
#define PANIC_IF(cond)                                                         \
  if (cond)                                                                    \
  PANIC()

#endif
// vim: set ft=c ts=2 sw=2 et:
