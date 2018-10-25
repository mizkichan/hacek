#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>

char *clone_str_range(const char *, const char *)
    __attribute__((returns_nonnull, nonnull));
char *read_from_file(char *) __attribute__((nonnull));

void *checked_malloc(size_t) __attribute__((returns_nonnull, alloc_size(1)));
void *checked_realloc(void *, size_t)
    __attribute__((returns_nonnull, alloc_size(2)));

char *append_char(char *, char) __attribute__((returns_nonnull, nonnull));
char *append_str(char *, char *) __attribute__((returns_nonnull, nonnull));
bool starts_with(const char *, const char *) __attribute__((nonnull));
bool str_equals(const char *, const char *) __attribute__((nonnull));

void erase(void *, void *, void *) __attribute__((nonnull));

#define PUSH_BACK(type, ptr, n, value)                                         \
  do {                                                                         \
    (ptr) = (type *)checked_realloc((ptr), sizeof(type) * (++(n) + 1));        \
    (ptr)[n - 1] = (value);                                                    \
    (ptr)[n] = NULL;                                                           \
  } while (0)

#endif
// vim: set ft=c ts=2 sw=2 et:
