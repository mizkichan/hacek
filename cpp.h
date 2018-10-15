#ifndef CPP_H
#define CPP_H

#include "tokens.h"
#include <stdbool.h>

struct PPTokenList preprocess(char *) __attribute__((nonnull));
struct PPTokenList cpp_tokenize(char *) __attribute__((nonnull));
void cpp_concat_string_literals(struct PPTokenList *);

void skip_whitespaces(char **);

bool match_header_name(char **, struct PPToken *);
bool match_identifier(char **, struct PPToken *);
bool match_pp_number(char **, struct PPToken *);
bool match_character_constant(char **, struct PPToken *);
bool match_string_literal(char **, struct PPToken *);
bool match_punctuator(char **, struct PPToken *);

char read_char(char **);

bool is_nondigit(char);
bool is_digit(char);
bool is_whitespace(char);

struct PPTokenList {
  size_t length;
  struct PPToken *pp_tokens;
};

#endif
// vim: set ft=c ts=2 sw=2 et:
