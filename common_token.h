#ifndef COMMON_TOKEN_H
#define COMMON_TOKEN_H

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

const char *punctuator_str(int);

#endif
// vim: set ft=c ts=2 sw=2 et:
