#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void *checked_malloc(size_t) __attribute__((returns_nonnull, alloc_size(1)));
char *read_from_file(char *) __attribute__((nonnull));

#endif
// vim: set ft=c ts=2 sw=2 et:
