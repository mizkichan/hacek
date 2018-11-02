#ifndef LEXER_H
#define LEXER_H

#include <stddef.h>

struct Line {
  size_t linenum;
  char *value;
};

struct PPTokenLine {
  size_t linenum;
  struct PPToken **pp_tokens;
};

struct Line **split_source_into_lines(const char *) __attribute__((nonnull));
void reconstruct_lines(struct Line **) __attribute__((nonnull));
void replace_comments(struct Line **) __attribute__((nonnull));
struct PPTokenLine **tokenize(struct Line **)
    __attribute__((returns_nonnull, nonnull));

#endif
// vim: set ft=c ts=2 sw=2 et:
