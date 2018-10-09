#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void *checked_malloc(size_t) __attribute__((alloc_size(1)));
void *checked_realloc(void *, size_t) __attribute__((alloc_size(2)));
char *read_from_file(char *) __attribute__((nonnull));

struct String {
  size_t length;
  char *chars;
};

#endif
// vim: set ft=c ts=2 sw=2 et:
