#include "tokens.h"
#include "error.h"
#include <string.h>

bool str_to_keyword(const char *str, enum Keyword *buf) {
  if (strcmp(str, "_Alignas") == 0) {
    *buf = ALIGNAS;
  } else if (strcmp(str, "_Alignof") == 0) {
    *buf = ALIGNOF;
  } else if (strcmp(str, "_Atomic") == 0) {
    *buf = ATOMIC;
  } else if (strcmp(str, "auto") == 0) {
    *buf = AUTO;
  } else if (strcmp(str, "_Bool") == 0) {
    *buf = BOOL;
  } else if (strcmp(str, "break") == 0) {
    *buf = BREAK;
  } else if (strcmp(str, "case") == 0) {
    *buf = CASE;
  } else if (strcmp(str, "char") == 0) {
    *buf = CHAR;
  } else if (strcmp(str, "_Complex") == 0) {
    *buf = COMPLEX;
  } else if (strcmp(str, "const") == 0) {
    *buf = CONST;
  } else if (strcmp(str, "continue") == 0) {
    *buf = CONTINUE;
  } else if (strcmp(str, "default") == 0) {
    *buf = DEFAULT;
  } else if (strcmp(str, "do") == 0) {
    *buf = DO;
  } else if (strcmp(str, "double") == 0) {
    *buf = DOUBLE;
  } else if (strcmp(str, "else") == 0) {
    *buf = ELSE;
  } else if (strcmp(str, "enum") == 0) {
    *buf = ENUM;
  } else if (strcmp(str, "extern") == 0) {
    *buf = EXTERN;
  } else if (strcmp(str, "float") == 0) {
    *buf = FLOAT;
  } else if (strcmp(str, "for") == 0) {
    *buf = FOR;
  } else if (strcmp(str, "_Generic") == 0) {
    *buf = GENERIC;
  } else if (strcmp(str, "goto") == 0) {
    *buf = GOTO;
  } else if (strcmp(str, "if") == 0) {
    *buf = IF;
  } else if (strcmp(str, "_Imaginary") == 0) {
    *buf = IMAGINARY;
  } else if (strcmp(str, "inline") == 0) {
    *buf = INLINE;
  } else if (strcmp(str, "int") == 0) {
    *buf = INT;
  } else if (strcmp(str, "long") == 0) {
    *buf = LONG;
  } else if (strcmp(str, "_Noreturn") == 0) {
    *buf = NORETURN;
  } else if (strcmp(str, "register") == 0) {
    *buf = REGISTER;
  } else if (strcmp(str, "restrict") == 0) {
    *buf = RESTRICT;
  } else if (strcmp(str, "return") == 0) {
    *buf = RETURN;
  } else if (strcmp(str, "short") == 0) {
    *buf = SHORT;
  } else if (strcmp(str, "signed") == 0) {
    *buf = SIGNED;
  } else if (strcmp(str, "sizeof") == 0) {
    *buf = SIZEOF;
  } else if (strcmp(str, "static") == 0) {
    *buf = STATIC;
  } else if (strcmp(str, "_Static_assert") == 0) {
    *buf = STATIC_ASSERT;
  } else if (strcmp(str, "struct") == 0) {
    *buf = STRUCT;
  } else if (strcmp(str, "switch") == 0) {
    *buf = SWITCH;
  } else if (strcmp(str, "_Thread_local") == 0) {
    *buf = THREAD_LOCAL;
  } else if (strcmp(str, "typedef") == 0) {
    *buf = TYPEDEF;
  } else if (strcmp(str, "union") == 0) {
    *buf = UNION;
  } else if (strcmp(str, "unsigned") == 0) {
    *buf = UNSIGNED;
  } else if (strcmp(str, "void") == 0) {
    *buf = VOID;
  } else if (strcmp(str, "volatile") == 0) {
    *buf = VOLATILE;
  } else if (strcmp(str, "while") == 0) {
    *buf = WHILE;
  } else {
    return false;
  }
  return true;
}

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

const char *punctuator_str(int x) {
  switch ((enum Punctuator)x) {
  case LEFT_BRACKET:
    return "LEFT_BRACKET";
  case RIGHT_BRACKET:
    return "RIGHT_BRACKET";
  case LEFT_PAREN:
    return "LEFT_PAREN";
  case RIGHT_PAREN:
    return "RIGHT_PAREN";
  case LEFT_BRACE:
    return "LEFT_BRACE";
  case RIGHT_BRACE:
    return "RIGHT_BRACE";
  case DOT:
    return "DOT";
  case ARROW:
    return "ARROW";
  case INCREMENT:
    return "INCREMENT";
  case DECREMENT:
    return "DECREMENT";
  case AMPASAND:
    return "AMPASAND";
  case ASTERISK:
    return "ASTERISK";
  case PLUS:
    return "PLUS";
  case MINUS:
    return "MINUS";
  case NEGATE:
    return "NEGATE";
  case EXCLAMATION:
    return "EXCLAMATION";
  case DIVIDE:
    return "DIVIDE";
  case REMIND:
    return "REMIND";
  case LEFT_SHIFT:
    return "LEFT_SHIFT";
  case RIGHT_SHIFT:
    return "RIGHT_SHIFT";
  case LESS_THAN:
    return "LESS_THAN";
  case GREATER_THAN:
    return "GREATER_THAN";
  case LESS_EQUAL:
    return "LESS_EQUAL";
  case GREATER_EQUAL:
    return "GREATER_EQUAL";
  case EQUAL:
    return "EQUAL";
  case NOT_EQUAL:
    return "NOT_EQUAL";
  case EXCLUSIVE_OR:
    return "EXCLUSIVE_OR";
  case INCLUSIVE_OR:
    return "INCLUSIVE_OR";
  case LOGICAL_AND:
    return "LOGICAL_AND";
  case LOGICAL_OR:
    return "LOGICAL_OR";
  case QUESTION:
    return "QUESTION";
  case COLON:
    return "COLON";
  case SEMICOLON:
    return "SEMICOLON";
  case ELLIPSE:
    return "ELLIPSE";
  case ASSIGN:
    return "ASSIGN";
  case MULTIPLY_ASSIGN:
    return "MULTIPLY_ASSIGN";
  case DIVIDE_ASSIGN:
    return "DIVIDE_ASSIGN";
  case REMIND_ASSIGN:
    return "REMIND_ASSIGN";
  case ADD_ASSIGN:
    return "ADD_ASSIGN";
  case SUBTRACT_ASSIGN:
    return "SUBTRACT_ASSIGN";
  case LEFT_SHIFT_ASSIGN:
    return "LEFT_SHIFT_ASSIGN";
  case RIGHT_SHIFT_ASSIGN:
    return "RIGHT_SHIFT_ASSIGN";
  case AND_ASSIGN:
    return "AND_ASSIGN";
  case EXCLUSIVE_OR_ASSIGN:
    return "EXCLUSIVE_OR_ASSIGN";
  case INCLUSIVE_OR_ASSIGN:
    return "INCLUSIVE_OR_ASSIGN";
  case COMMA:
    return "COMMA";
  case SIGN:
    return "SIGN";
  case DOUBLE_SIGN:
    return "DOUBLE_SIGN";
  case DIGRAPH_LEFT_BRACKET:
    return "DIGRAPH_LEFT_BRACKET";
  case DIGRAPH_RIGHT_BRACKET:
    return "DIGRAPH_RIGHT_BRACKET";
  case DIGRAPH_LEFT_BRACE:
    return "DIGRAPH_LEFT_BRACE";
  case DIGRAPH_RIGHT_BRACE:
    return "DIGRAPH_RIGHT_BRACE";
  case DIGRAPH_SIGN:
    return "DIGRAPH_SIGN";
  case DIGRAPH_DOUBLE_SIGN:
    return "DIGRAPH_DOUBLE_SIGN";
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

const char *header_name_kind_str(int x) {
  switch ((enum HeaderNameKind)x) {
  case H_CHAR_SEQUENCE:
    return "H_CHAR_SEQUENCE";
  case Q_CHAR_SEQUENCE:
    return "Q_CHAR_SEQUENCE";
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

const char *encoding_prefix_str(int x) {
  switch ((enum EncodingPrefix)x) {
  case ENCODING_PREFIX_UTF8:
    return "ENCODING_PREFIX_UTF8";
  case ENCODING_PREFIX_CHAR16:
    return "ENCODING_PREFIX_CHAR16";
  case ENCODING_PREFIX_CHAR32:
    return "ENCODING_PREFIX_CHAR32";
  case ENCODING_PREFIX_WCHAR:
    return "ENCODING_PREFIX_WCHAR";
  case ENCODING_PREFIX_NONE:
    return "ENCODING_PREFIX_NONE";
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
