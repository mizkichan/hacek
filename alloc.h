#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>

void alloc_init(void);
void *checked_malloc(const char *, int, size_t)
    __attribute__((returns_nonnull, alloc_size(3), nonnull(1)));
void *checked_realloc(const char *, int, void *, size_t)
    __attribute__((returns_nonnull, alloc_size(4), nonnull(1)));
void checked_free(const char *, int, void *) __attribute__((nonnull(1)));

#define MALLOC(size) checked_malloc(__FILE__, __LINE__, size)
#define REALLOC(ptr, size) checked_realloc(__FILE__, __LINE__, ptr, size)
#define FREE(ptr) checked_free(__FILE__, __LINE__, ptr)

#endif
// vim: set ft=c ts=2 sw=2 et: