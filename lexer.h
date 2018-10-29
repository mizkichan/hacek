#ifndef LEXER_H
#define LEXER_H

#include "token.h"

void reconstruct_lines(char *) __attribute__((nonnull));
struct PPToken **tokenize(char *) __attribute__((returns_nonnull, nonnull));
void replace_comments(char *, struct PPToken **) __attribute__((nonnull));

#endif
// vim: set ft=c ts=2 sw=2 et:
