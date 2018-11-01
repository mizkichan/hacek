#ifndef TOKEN_H
#define TOKEN_H

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

enum Punctuator {
  LEFT_BRACKET,          // [
  RIGHT_BRACKET,         // ]
  LEFT_PAREN,            // (
  RIGHT_PAREN,           // )
  LEFT_BRACE,            // {
  RIGHT_BRACE,           // }
  DOT,                   // .
  ARROW,                 // ->
  INCREMENT,             // ++
  DECREMENT,             // --
  AMPASAND,              // &
  ASTERISK,              // *
  PLUS,                  // +
  MINUS,                 // -
  NEGATE,                // ~
  EXCLAMATION,           // !
  DIVIDE,                // /
  REMIND,                // %
  LEFT_SHIFT,            // <<
  RIGHT_SHIFT,           // >>
  LESS_THAN,             // <
  GREATER_THAN,          // >
  LESS_EQUAL,            // <=
  GREATER_EQUAL,         // >=
  EQUAL,                 // ==
  NOT_EQUAL,             // !=
  EXCLUSIVE_OR,          // ^
  INCLUSIVE_OR,          // |
  LOGICAL_AND,           // &&
  LOGICAL_OR,            // ||
  QUESTION,              // ?
  COLON,                 // :
  SEMICOLON,             // ;
  ELLIPSE,               // ...
  ASSIGN,                // =
  MULTIPLY_ASSIGN,       // *=
  DIVIDE_ASSIGN,         // /=
  REMIND_ASSIGN,         // %=
  ADD_ASSIGN,            // +=
  SUBTRACT_ASSIGN,       // -=
  LEFT_SHIFT_ASSIGN,     // <<=
  RIGHT_SHIFT_ASSIGN,    // >>=
  AND_ASSIGN,            // &=
  EXCLUSIVE_OR_ASSIGN,   // ^=
  INCLUSIVE_OR_ASSIGN,   // |=
  COMMA,                 // ,
  SIGN,                  // #
  DOUBLE_SIGN,           // ##
  DIGRAPH_LEFT_BRACKET,  // <:
  DIGRAPH_RIGHT_BRACKET, // :>
  DIGRAPH_LEFT_BRACE,    // <%
  DIGRAPH_RIGHT_BRACE,   // %>
  DIGRAPH_SIGN,          // %:
  DIGRAPH_DOUBLE_SIGN,   // %:%:
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
    // struct StringLiteral header_name;
    enum Punctuator punctuator;
    char *chars;
  };
};

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

const char *keyword_str(int);
const char *punctuator_str(int);
const char *constant_kind_str(int);
const char *header_name_kind_str(int);
const char *token_kind_str(int);
const char *pp_token_kind_str(int);
const char *string_literal_prefix_str(int);
const char *character_constant_prefix_str(int);

#endif
// vim: set ft=c ts=2 sw=2 et:
