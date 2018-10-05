#ifndef ERROR_H
#define ERROR_H

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>

void exit_message(const char *, ...);
void debug(char *, int, int, const char *, ...);

#define EXIT_MESSAGE(...) exit_message(__VA_ARGS__)
#define EXIT_MESSAGE_IF(cond, ...)                                             \
  if ((cond))                                                                  \
  EXIT_MESSAGE(__VA_ARGS__)

#define WARN(...) debug(__FILE__, __LINE__, (errno), __VA_ARGS__)
#define ERROR(...)                                                             \
  do {                                                                         \
    WARN(__VA_ARGS__);                                                         \
    exit(EXIT_FAILURE);                                                        \
  } while (0)
#define PANIC()                                                                \
  do {                                                                         \
    ERROR(NULL);                                                               \
    exit(EXIT_FAILURE);                                                        \
  } while (0)

#define WARN_IF(cond, ...)                                                     \
  if ((cond))                                                                  \
  WARN(__VA_ARGS__)
#define ERROR_IF(cond, ...)                                                    \
  if ((cond))                                                                  \
  ERROR(__VA_ARGS__)
#define PANIC_IF(cond)                                                         \
  if ((cond))                                                                  \
  PANIC()

#endif
// vim: set ft=c ts=2 sw=2 et:
