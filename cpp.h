#ifndef CPP_H
#define CPP_H

#include "tokens.h"
#include <stdbool.h>

struct PPTokenList preprocess(char *) __attribute__((nonnull));
struct PPTokenList cpp_tokenize(char *) __attribute__((nonnull));
bool is_nondigit(char) __attribute__((const));

struct PPTokenList {
  size_t length;
  struct PPToken *pp_tokens;
};

#endif
// vim: set ft=c ts=2 sw=2 et:
