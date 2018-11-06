#ifndef DEFS_H
#define DEFS_H

#include <stddef.h>

struct Position {
  char *file;
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

struct Token {
  enum TokenKind {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_CONSTANT,
    TOKEN_STRING_LITERAL,
    TOKEN_PUNCTUATOR,
  } kind;
  union {
    enum Keyword keyword;
    struct Punctuator *punctuator;
    char *chars;
  };
};

struct HeaderName {
  struct Position pos;
  enum HeaderNameKind { H_CHAR_SEQUENCE, Q_CHAR_SEQUENCE } kind;
  char value[];
};

struct Identifier {
  struct Position pos;
  char value[];
};

struct PPNumber {
  struct Position pos;
  char value[];
};

struct CharacterConstant {
  struct Position pos;
  enum CharacterConstantPrefix {
    CHARACTER_CONSTANT_PREFIX_WCHAR,
    CHARACTER_CONSTANT_PREFIX_CHAR16,
    CHARACTER_CONSTANT_PREFIX_CHAR32,
    CHARACTER_CONSTANT_PREFIX_NONE
  } prefix;
  char value[];
};

struct StringLiteral {
  struct Position pos;
  enum StringLiteralPrefix {
    STRING_LITERAL_PREFIX_UTF8,
    STRING_LITERAL_PREFIX_CHAR16,
    STRING_LITERAL_PREFIX_CHAR32,
    STRING_LITERAL_PREFIX_WCHAR,
    STRING_LITERAL_PREFIX_NONE
  } prefix;
  char value[];
};

struct Punctuator {
  struct Position pos;
  enum PunctuatorKind {
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
  } kind;
};

struct PPToken {
  enum PPTokenKind {
    PP_HEADER_NAME,
    PP_IDENTIFIER,
    PP_NUMBER,
    PP_CHARACTER_CONSTANT,
    PP_STRING_LITERAL,
    PP_PUNCTUATOR,
    PP_NWSC,
  } kind;
  union {
    struct HeaderName *header_name;
    struct Identifier *identifier;
    struct PPNumber *number;
    struct CharacterConstant *character_constant;
    struct StringLiteral *string_literal;
    struct Punctuator *punctuator;
    char *nwsc;
  };
};

struct Line {
  size_t linenum;
  char *value;
};

struct PPTokenLine {
  size_t linenum;
  struct PPToken **pp_tokens;
};

#endif
// vim: set ft=c ts=2 sw=2 et:
