#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void *checked_malloc(size_t) __attribute__((returns_nonnull, alloc_size(1)));
void *checked_realloc(void *, size_t)
    __attribute__((returns_nonnull, alloc_size(2)));
void *push_back(void *, size_t, void *, size_t)
    __attribute__((returns_nonnull, nonnull));
char *read_from_file(char *) __attribute__((nonnull));

#endif
// vim: set ft=c ts=2 sw=2 et:
