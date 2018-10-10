#ifndef CPP_H
#define CPP_H

#include "tokens.h"

struct PreprocessingToken *preprocess(char *)
    __attribute__((returns_nonnull, nonnull));
struct PreprocessingToken *cpp_tokenize(char *)
    __attribute__((returns_nonnull, nonnull));

#endif
// vim: set ft=c ts=2 sw=2 et:
