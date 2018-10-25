#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

#include "token.h"
#include <stdbool.h>

void reconstruct_lines(char *) __attribute__((nonnull));
struct PPToken **tokenize(char *) __attribute__((returns_nonnull, nonnull));
void execute_pp_directives(struct PPToken **) __attribute__((nonnull));

void convert_escape_sequences(struct PPToken **) __attribute__((nonnull));
void concatenate_adjacent_string_literals(struct PPToken **)
    __attribute__((nonnull));
struct Token **convert_pp_tokens_into_tokens(struct PPToken **)
    __attribute__((returns_nonnull, nonnull));

bool match_white_space_characters(char **, struct PPToken *)
    __attribute__((nonnull));
bool match_header_name(char **, struct PPToken *) __attribute__((nonnull));
bool match_identifier(char **, struct PPToken *) __attribute__((nonnull));
bool match_pp_number(char **, struct PPToken *) __attribute__((nonnull));
bool match_character_constant(char **, struct PPToken *)
    __attribute__((nonnull));
bool match_string_literal(char **, struct PPToken *) __attribute__((nonnull));
bool match_punctuator(char **, struct PPToken *) __attribute__((nonnull));
bool match_nwsc(char **, struct PPToken *) __attribute__((nonnull));
char match_escape_sequence(char **) __attribute__((nonnull));

bool is_include_directive(struct PPToken *, struct PPToken *);

bool is_nondigit(char) __attribute__((const));
bool is_digit(char) __attribute__((const));
bool is_whitespace(char) __attribute__((const));

#endif
// vim: set ft=c ts=2 sw=2 et:
