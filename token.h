#ifndef PP_TOKEN_H
#define PP_TOKEN_H

#include "defs.h"

const char *character_constant_prefix_str(int);
const char *header_name_kind_str(int);
const char *keyword_str(int);
const char *pp_token_kind_str(int);
const char *punctuator_kind_str(int);
const char *string_literal_prefix_str(int);
const char *token_kind_str(int);
const char *constant_kind_str(int);
const char *integer_constant_type_str(int);

struct HeaderName *new_header_name(const char *, size_t, size_t,
                                   enum HeaderNameKind, const char *,
                                   const char *)
    __attribute__((returns_nonnull));
struct Identifier *new_identifier(const char *, size_t, size_t, const char *,
                                  const char *)
    __attribute__((returns_nonnull));
struct PPNumber *new_pp_number(const char *, size_t, size_t, const char *,
                               const char *) __attribute__((returns_nonnull));
struct CharacterConstant *new_character_constant(const char *, size_t, size_t,
                                                 enum CharacterConstantPrefix,
                                                 const char *, const char *)
    __attribute__((returns_nonnull));
struct StringLiteral *new_string_literal(const char *, size_t, size_t,
                                         enum StringLiteralPrefix, const char *,
                                         const char *)
    __attribute__((returns_nonnull));
struct Punctuator *new_punctuator(const char *, size_t, size_t,
                                  enum PunctuatorKind)
    __attribute__((returns_nonnull));
struct PPToken *new_pp_token(enum PPTokenKind, void *)
    __attribute__((returns_nonnull));

#endif
// vim: set ft=c ts=2 sw=2 et:
