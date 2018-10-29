#ifndef UTILS_H
#define UTILS_H

#include "alloc.h"
#include <stdbool.h>

char *read_from_file(char *) __attribute__((nonnull));
char *append_str(char *, char) __attribute__((returns_nonnull, nonnull));
char *remove_str(char *, char) __attribute__((returns_nonnull, nonnull));
void erase_str(char *, char *) __attribute__((nonnull));
char *search_str(const char *, const char *)
    __attribute__((returns_nonnull, nonnull));
char *search_char(const char *, char) __attribute__((nonnull));
bool starts_with(const char *, const char *) __attribute__((nonnull));
bool str_range_equals(const char *, const char *, const char *)
    __attribute__((nonnull));

#define PUSH_BACK(type, ptr, n, value)                                         \
  do {                                                                         \
    (ptr) = (type *)REALLOC((ptr), sizeof(type) * (++(n) + 1));                \
    (ptr)[n - 1] = (value);                                                    \
    (ptr)[n] = NULL;                                                           \
  } while (0)

#define DEL 0x7f

#endif
// vim: set ft=c ts=2 sw=2 et:
