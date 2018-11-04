#ifndef PARSER_H
#define PARSER_H

#include "defs.h"

struct AST *parse(struct Token **) __attribute__((returns_nonnull, nonnull));

#endif
// vim: set ft=c ts=2 sw=2 et:
