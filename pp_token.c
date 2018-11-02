#include "error.h"
#include "pp_token.h"

const char *pp_token_kind_str(int x) {
  switch ((enum PPTokenKind)x) {
  case PP_HEADER_NAME:
    return "PP_HEADER_NAME";
  case PP_IDENTIFIER:
    return "PP_IDENTIFIER";
  case PP_NUMBER:
    return "PP_NUMBER";
  case PP_CHARACTER_CONSTANT:
    return "PP_CHARACTER_CONSTANT";
  case PP_STRING_LITERAL:
    return "PP_STRING_LITERAL";
  case PP_PUNCTUATOR:
    return "PP_PUNCTUATOR";
  case PP_NWSC:
    return "PP_NWSC";
  case PP_NEWLINE:
    return "PP_NEWLINE";
  }
  ERROR("unknown enum variant: %d", x);
}

const char *header_name_kind_str(int x) {
  switch ((enum HeaderNameKind)x) {
  case H_CHAR_SEQUENCE:
    return "H_CHAR_SEQUENCE";
  case Q_CHAR_SEQUENCE:
    return "Q_CHAR_SEQUENCE";
  }
  ERROR("unknown enum variant: %d", x);
}

const char *string_literal_prefix_str(int x) {
  switch ((enum StringLiteralPrefix)x) {
  case STRING_LITERAL_PREFIX_UTF8:
    return "STRING_LITERAL_PREFIX_UTF8";
  case STRING_LITERAL_PREFIX_CHAR16:
    return "STRING_LITERAL_PREFIX_CHAR16";
  case STRING_LITERAL_PREFIX_CHAR32:
    return "STRING_LITERAL_PREFIX_CHAR32";
  case STRING_LITERAL_PREFIX_WCHAR:
    return "STRING_LITERAL_PREFIX_WCHAR";
  case STRING_LITERAL_PREFIX_NONE:
    return "STRING_LITERAL_PREFIX_NONE";
  }
  ERROR("unknown enum variant: %d", x);
}

const char *character_constant_prefix_str(int x) {
  switch ((enum CharacterConstantPrefix)x) {
  case CHARACTER_CONSTANT_PREFIX_WCHAR:
    return "CHARACTER_CONSTANT_PREFIX_WCHAR";
  case CHARACTER_CONSTANT_PREFIX_CHAR16:
    return "CHARACTER_CONSTANT_PREFIX_CHAR16";
  case CHARACTER_CONSTANT_PREFIX_CHAR32:
    return "CHARACTER_CONSTANT_PREFIX_CHAR32";
  case CHARACTER_CONSTANT_PREFIX_NONE:
    return "CHARACTER_CONSTANT_PREFIX_NONE";
  }
  ERROR("unknown enum variant: %d", x);
}

// vim: set ft=c ts=2 sw=2 et:
