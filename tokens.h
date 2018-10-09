#ifndef CPP_H
#define CPP_H

#include "utils.h"
#include <stddef.h>
#include <stdint.h>

struct Position {
  size_t line;
  size_t column;
};

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

struct Token {
  enum {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_CONSTANT,
    TOKEN_STRING_LITERAL,
    TOKEN_PUNCTUATOR,
  } kind;
  struct Position position;
  union {
    enum Keyword keyword;
    struct Identifier identifier;
    struct StringLiteral string_literal;
    enum Punctuator punctuator;
  };
};

struct PreprocessingToken {
  enum {
    PP_HEADER_NAME,
    PP_IDENTIFIER,
    PP_NUMBER,
    PP_CHARACTER_CONSTANT,
    PP_STRING_LITERAL,
    PP_PUNCTUATOR,
    PP_NWSC,
  } kind;
  union {
    struct HeaderName header_name;
    struct Identifier identifier;
    struct PP_Number number;
    struct CharacterConstant character_constant;
    struct StringLiteral string_literal;
    enum Punctuator punctuator;
    char nwsc;
  };
};

struct Identifier {
  struct String value;
};

struct Constant {
  enum {
    INTEGER_CONSTANT,
    FLOATING_CONSTANT,
    ENUMERATION_CONSTANT,
    CHARACTER_CONSTANT,
  } kind;
  union {
    struct IntegerConstant integer_constant;
    struct FloatingConstant floating_constant;
    struct EnumerationConstant enumeration_constant;
    struct CharacterConstant character_constant;
  };
};

struct IntegerConstant {
  struct String value;
};

struct FloatingConstant {
  struct String value;
};

struct EnumerationConstant {
  struct String value;
};

struct CharacterConstant {
  struct String value;
};

struct StringLiteral {
  struct String value;
};

struct HeaderName {
  enum { H_CHAR_SEQUENCE, Q_CHAR_SEQUENCE } hq;
  struct String value;
};

struct PP_Number {
  struct String value;
};

#endif
// vim: set ft=c ts=2 sw=2 et:
