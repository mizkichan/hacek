#ifndef TOKEN_H
#define TOKEN_H

#include "common_token.h"
#include <stdbool.h>
#include <stddef.h>

enum Keyword {
  ALIGNAS,
  ALIGNOF,
  ATOMIC,
  AUTO,
  BOOL,
  BREAK,
  CASE,
  CHAR,
  COMPLEX,
  CONST,
  CONTINUE,
  DEFAULT,
  DO,
  DOUBLE,
  ELSE,
  ENUM,
  EXTERN,
  FLOAT,
  FOR,
  GENERIC,
  GOTO,
  IF,
  IMAGINARY,
  INLINE,
  INT,
  LONG,
  NORETURN,
  REGISTER,
  RESTRICT,
  RETURN,
  SHORT,
  SIGNED,
  SIZEOF,
  STATIC,
  STATIC_ASSERT,
  STRUCT,
  SWITCH,
  THREAD_LOCAL,
  TYPEDEF,
  UNION,
  UNSIGNED,
  VOID,
  VOLATILE,
  WHILE,
};

struct Constant {
  enum ConstantKind {
    INTEGER_CONSTANT,
    FLOATING_CONSTANT,
    ENUMERATION_CONSTANT,
    CHARACTER_CONSTANT,
  } kind;
  char *chars;
};

struct Token {
  char *begin;
  char *end;
  enum TokenKind {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_CONSTANT,
    TOKEN_STRING_LITERAL,
    TOKEN_PUNCTUATOR,
  } kind;
  union {
    enum Keyword keyword;
    enum Punctuator punctuator;
    char *chars;
  };
};

const char *keyword_str(int);
const char *constant_kind_str(int);
const char *token_kind_str(int);

#endif
// vim: set ft=c ts=2 sw=2 et:
