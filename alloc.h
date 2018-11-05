#ifndef ALLOC_H
#define ALLOC_H

#include <stddef.h>

#ifdef DEBUG
void alloc_init(void);
void *checked_malloc(const char *, const char *, int, size_t)
    __attribute__((returns_nonnull, alloc_size(4), nonnull(1)));
void *checked_realloc(const char *, const char *, int, void *, size_t)
    __attribute__((returns_nonnull, alloc_size(5), nonnull(1)));
void checked_free(const char *, const char *, int, void *)
    __attribute__((nonnull(1)));

#define MALLOC(size) checked_malloc(__func__, __FILE__, __LINE__, size)
#define REALLOC(ptr, size)                                                     \
  checked_realloc(__func__, __FILE__, __LINE__, ptr, size)
#define FREE(ptr) checked_free(__func__, __FILE__, __LINE__, ptr)

#else
#include <stdlib.h>
#define MALLOC malloc
#define REALLOC realloc
#define FREE free

#endif

#endif
// vim: set ft=c ts=2 sw=2 et:
