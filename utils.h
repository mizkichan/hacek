#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>

void *checked_malloc(size_t);
void *checked_realloc(void *, size_t);
char *read_from_file(char *);

#endif
// vim: set ft=c ts=2 sw=2 et:
