#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "token.h"
#include <stdbool.h>

struct PPTokenList preprocess(char *) __attribute__((nonnull));

struct PPTokenList pp_tokenize(char *) __attribute__((nonnull));

void pp_concat_string_literals(struct PPTokenList *) __attribute__((nonnull));
struct TokenList pp_convert_into_token(struct PPTokenList *)
    __attribute__((nonnull));

void skip_whitespaces(char **) __attribute__((nonnull));
char read_char(char **) __attribute__((nonnull));

bool match_header_name(char **, struct PPToken *) __attribute__((nonnull));
bool match_identifier(char **, struct PPToken *) __attribute__((nonnull));
bool match_pp_number(char **, struct PPToken *) __attribute__((nonnull));
bool match_character_constant(char **, struct PPToken *)
    __attribute__((nonnull));
bool match_string_literal(char **, struct PPToken *) __attribute__((nonnull));
bool match_punctuator(char **, struct PPToken *) __attribute__((nonnull));

bool is_nondigit(char) __attribute__((const));
bool is_digit(char) __attribute__((const));
bool is_whitespace(char) __attribute__((const));

#endif
// vim: set ft=c ts=2 sw=2 et:
