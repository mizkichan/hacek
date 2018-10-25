#ifndef TOKEN_H
#define TOKEN_H

#include <stdbool.h>
#include <stddef.h>

struct Position {
  char *name;
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

struct Constant {
  enum ConstantKind {
    INTEGER_CONSTANT,
    FLOATING_CONSTANT,
    ENUMERATION_CONSTANT,
    CHARACTER_CONSTANT,
  } kind;
  char *chars;
};

struct HeaderName {
  enum HeaderNameKind { H_CHAR_SEQUENCE, Q_CHAR_SEQUENCE } kind;
  char *chars;
};

struct CharacterConstant {
  enum CharacterConstantPrefix {
    CHARACTER_CONSTANT_PREFIX_WCHAR,
    CHARACTER_CONSTANT_PREFIX_CHAR16,
    CHARACTER_CONSTANT_PREFIX_CHAR32,
    CHARACTER_CONSTANT_PREFIX_NONE
  } prefix;
  char *chars;
};

struct StringLiteral {
  enum EncodingPrefix {
    ENCODING_PREFIX_UTF8,
    ENCODING_PREFIX_CHAR16,
    ENCODING_PREFIX_CHAR32,
    ENCODING_PREFIX_WCHAR,
    ENCODING_PREFIX_NONE
  } encoding_prefix;
  char *chars;
};

struct Token {
  struct Position position;
  enum TokenKind {
    TOKEN_KEYWORD,
    TOKEN_IDENTIFIER,
    TOKEN_CONSTANT,
    TOKEN_STRING_LITERAL,
    TOKEN_PUNCTUATOR,
  } kind;
  union {
    enum Keyword keyword;
    struct StringLiteral header_name;
    enum Punctuator punctuator;
    char *chars;
  };
};

struct PPToken {
  struct Position position;
  enum PPTokenKind {
    PP_HEADER_NAME,
    PP_IDENTIFIER,
    PP_NUMBER,
    PP_CHARACTER_CONSTANT,
    PP_STRING_LITERAL,
    PP_PUNCTUATOR,
    PP_NWSC,
    PP_WHITE_SPACE_CHARACTERS,
    PP_NEWLINE
  } kind;
  union {
    struct HeaderName header_name;
    struct CharacterConstant character_constant;
    struct StringLiteral string_literal;
    enum Punctuator punctuator;
    char nwsc;
    char *chars;
  };
};

bool str_to_keyword(const char *, enum Keyword *);

const char *keyword_str(int);
const char *punctuator_str(int);
const char *constant_kind_str(int);
const char *header_name_kind_str(int);
const char *token_kind_str(int);
const char *pp_token_kind_str(int);
const char *encoding_prefix_str(int);
const char *character_constant_prefix_str(int);

#endif
// vim: set ft=c ts=2 sw=2 et:
