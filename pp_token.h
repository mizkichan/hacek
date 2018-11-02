#ifndef PP_TOKEN_H
#define PP_TOKEN_H

#include "common_token.h"

struct PPToken {
  char *begin;
  char *end;
  enum PPTokenKind {
    PP_HEADER_NAME,
    PP_IDENTIFIER,
    PP_NUMBER,
    PP_CHARACTER_CONSTANT,
    PP_STRING_LITERAL,
    PP_PUNCTUATOR,
    PP_NWSC,
    PP_NEWLINE
  } kind;
  union {
    enum HeaderNameKind { H_CHAR_SEQUENCE, Q_CHAR_SEQUENCE } header_name_kind;
    enum CharacterConstantPrefix {
      CHARACTER_CONSTANT_PREFIX_WCHAR,
      CHARACTER_CONSTANT_PREFIX_CHAR16,
      CHARACTER_CONSTANT_PREFIX_CHAR32,
      CHARACTER_CONSTANT_PREFIX_NONE
    } character_constant_prefix;
    enum StringLiteralPrefix {
      STRING_LITERAL_PREFIX_UTF8,
      STRING_LITERAL_PREFIX_CHAR16,
      STRING_LITERAL_PREFIX_CHAR32,
      STRING_LITERAL_PREFIX_WCHAR,
      STRING_LITERAL_PREFIX_NONE
    } string_literal_prefix;
    enum Punctuator punctuator;
    char nwsc;
  };
};

const char *pp_token_kind_str(int);
const char *header_name_kind_str(int);
const char *string_literal_prefix_str(int);
const char *character_constant_prefix_str(int);

#endif
// vim: set ft=c ts=2 sw=2 et:
