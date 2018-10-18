#ifndef UTILS_H
#define UTILS_H

#include <stdbool.h>
#include <stddef.h>

char *clone_str_range(const char *, const char *) __attribute__((nonnull));
char *read_from_file(char *) __attribute__((nonnull));

void *checked_malloc(size_t) __attribute__((returns_nonnull, alloc_size(1)));
void *checked_realloc(void *, size_t)
    __attribute__((returns_nonnull, alloc_size(2)));

void *push_back(void *, size_t, void *, size_t)
    __attribute__((returns_nonnull, nonnull));
void *push_back_char(void *, size_t, char)
    __attribute__((returns_nonnull, nonnull));
char *append_str(char *, char *) __attribute__((returns_nonnull, nonnull));
bool starts_with(const char *, const char *) __attribute__((nonnull));
bool str_equals(const char *, const char *) __attribute__((nonnull));

void erase(void *, void *, void *) __attribute__((nonnull));

#endif
// vim: set ft=c ts=2 sw=2 et:
