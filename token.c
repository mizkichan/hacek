#include "error.h"
#include "token.h"
#include "utils.h"

const char *keyword_str(int x) {
  switch ((enum Keyword)x) {
  case ALIGNAS:
    return "ALIGNAS";
  case ALIGNOF:
    return "ALIGNOF";
  case ATOMIC:
    return "ATOMIC";
  case AUTO:
    return "AUTO";
  case BOOL:
    return "BOOL";
  case BREAK:
    return "BREAK";
  case CASE:
    return "CASE";
  case CHAR:
    return "CHAR";
  case COMPLEX:
    return "COMPLEX";
  case CONST:
    return "CONST";
  case CONTINUE:
    return "CONTINUE";
  case DEFAULT:
    return "DEFAULT";
  case DO:
    return "DO";
  case DOUBLE:
    return "DOUBLE";
  case ELSE:
    return "ELSE";
  case ENUM:
    return "ENUM";
  case EXTERN:
    return "EXTERN";
  case FLOAT:
    return "FLOAT";
  case FOR:
    return "FOR";
  case GENERIC:
    return "GENERIC";
  case GOTO:
    return "GOTO";
  case IF:
    return "IF";
  case IMAGINARY:
    return "IMAGINARY";
  case INLINE:
    return "INLINE";
  case INT:
    return "INT";
  case LONG:
    return "LONG";
  case NORETURN:
    return "NORETURN";
  case REGISTER:
    return "REGISTER";
  case RESTRICT:
    return "RESTRICT";
  case RETURN:
    return "RETURN";
  case SHORT:
    return "SHORT";
  case SIGNED:
    return "SIGNED";
  case SIZEOF:
    return "SIZEOF";
  case STATIC:
    return "STATIC";
  case STATIC_ASSERT:
    return "STATIC_ASSERT";
  case STRUCT:
    return "STRUCT";
  case SWITCH:
    return "SWITCH";
  case THREAD_LOCAL:
    return "THREAD_LOCAL";
  case TYPEDEF:
    return "TYPEDEF";
  case UNION:
    return "UNION";
  case UNSIGNED:
    return "UNSIGNED";
  case VOID:
    return "VOID";
  case VOLATILE:
    return "VOLATILE";
  case WHILE:
    return "WHILE";
  }
  ERROR("unknown enum variant: %d", x);
}

const char *constant_kind_str(int x) {
  switch ((enum ConstantKind)x) {
  case INTEGER_CONSTANT:
    return "INTEGER_CONSTANT";
  case FLOATING_CONSTANT:
    return "FLOATING_CONSTANT";
  case ENUMERATION_CONSTANT:
    return "ENUMERATION_CONSTANT";
  case CHARACTER_CONSTANT:
    return "CHARACTER_CONSTANT";
  }
  ERROR("unknown enum variant: %d", x);
}

const char *token_kind_str(int x) {
  switch ((enum TokenKind)x) {
  case TOKEN_KEYWORD:
    return "TOKEN_KEYWORD";
  case TOKEN_IDENTIFIER:
    return "TOKEN_IDENTIFIER";
  case TOKEN_CONSTANT:
    return "TOKEN_CONSTANT";
  case TOKEN_STRING_LITERAL:
    return "TOKEN_STRING_LITERAL";
  case TOKEN_PUNCTUATOR:
    return "TOKEN_PUNCTUATOR";
  }
  ERROR("unknown enum variant: %d", x);
}

// vim: set ft=c ts=2 sw=2 et:
