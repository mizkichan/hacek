#ifndef UTILS_H
#define UTILS_H
#include <stdbool.h>
#include <unistd.h>

int get_file_size(char *);
bool read_from_file(char *, size_t, char *);

#endif
// vim: set ft=c ts=2 sw=2 et:
