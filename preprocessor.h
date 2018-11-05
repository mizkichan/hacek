#ifndef PREPROCESSOR_H
#define PREPROCESSOR_H

void execute_pp_directives(struct PPTokenLine **) __attribute__((nonnull));
void convert_escape_sequences(struct PPToken **) __attribute__((nonnull));
struct PPToken **concatenate_pp_token_lines(struct PPTokenLine **)
    __attribute__((returns_nonnull, nonnull));
void concatenate_adjacent_string_literals(struct PPToken **)
    __attribute__((nonnull));
struct Token **convert_pp_tokens_into_tokens(struct PPToken **)
    __attribute__((returns_nonnull, nonnull));

#endif
// vim: set ft=c ts=2 sw=2 et:
