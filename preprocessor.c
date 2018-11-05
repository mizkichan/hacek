#include "alloc.h"
#include "defs.h"
#include "error.h"
#include "preprocessor.h"
#include "utils.h"
#include <ctype.h>

static void unescape(char *) __attribute__((nonnull));
static bool str_to_keyword(const char *, const char *, enum Keyword *)
    __attribute__((nonnull(1, 3)));
static struct Token *convert_pp_identifier(struct Identifier *)
    __attribute__((nonnull));
static struct Token *convert_pp_number(struct PPNumber *)
    __attribute__((nonnull));

void execute_pp_directives(struct PPTokenLine **pp_token_lines) {
  for (size_t i = 0; pp_token_lines[i]; ++i) {
    for (size_t j = 0; pp_token_lines[i]->pp_tokens[j]; ++j) {
      struct PPToken *pp_token = pp_token_lines[i]->pp_tokens[j];
      ERROR_IF(pp_token->kind == PP_PUNCTUATOR &&
                   (pp_token->punctuator->kind == SIGN),
               "Not implemented yet");
    }
  }
}

void convert_escape_sequences(struct PPToken **pp_tokens) {
  // FIXME only simple-escape-sequence is implemented

  for (size_t i = 0; pp_tokens[i]; ++i) {
    if (pp_tokens[i]->kind == PP_CHARACTER_CONSTANT) {
      unescape(pp_tokens[i]->character_constant->value);
    } else if (pp_tokens[i]->kind == PP_STRING_LITERAL) {
      unescape(pp_tokens[i]->string_literal->value);
    }
  }
}

struct PPToken **
concatenate_pp_token_lines(struct PPTokenLine **pp_token_lines) {
  struct PPToken **result = NULL;
  size_t count = 0;

  for (size_t i = 0; pp_token_lines[i]; ++i) {
    for (size_t j = 0; pp_token_lines[i]->pp_tokens[j]; ++j) {
      PUSH_BACK(struct PPToken *, result, count,
                pp_token_lines[i]->pp_tokens[j]);
    }
  }

  return result;
}

void concatenate_adjacent_string_literals(struct PPToken **pp_tokens) {
  while (pp_tokens[1]) {
    size_t size, size0, size1;
    char *dest;

    if (pp_tokens[0]->kind != PP_STRING_LITERAL ||
        pp_tokens[1]->kind != PP_STRING_LITERAL) {
      ++pp_tokens;
      continue;
    }

    size0 = length_str(pp_tokens[0]->string_literal->value);
    size1 = length_str(pp_tokens[1]->string_literal->value);
    size = sizeof(struct StringLiteral) + size0 + size1 + 1;
    pp_tokens[0]->string_literal = REALLOC(pp_tokens[0]->string_literal, size);
    dest = pp_tokens[0]->string_literal->value + size0;
    copy_str(dest, pp_tokens[1]->string_literal->value, NULL);
    FREE(pp_tokens[1]->string_literal);
    FREE(pp_tokens[1]);

    for (struct PPToken **pt = pp_tokens; pt[1]; ++pt) {
      pt[1] = pt[2];
    }
  }
}

struct Token **convert_pp_tokens_into_tokens(struct PPToken **pp_tokens) {
  struct Token **result = NULL;
  size_t count = 0;

  while (*pp_tokens) {
    struct Token *buf = NULL;

    switch ((*pp_tokens)->kind) {
    case PP_IDENTIFIER:
      buf = convert_pp_identifier((*pp_tokens)->identifier);
      break;

    case PP_NUMBER:
      buf = convert_pp_number((*pp_tokens)->number);
      break;

    case PP_CHARACTER_CONSTANT:
      buf = MALLOC(sizeof(struct PPToken));
      buf->kind = TOKEN_CONSTANT;
      buf->constant = MALLOC(sizeof(struct Constant));
      buf->constant->kind = CHARACTER_CONSTANT;
      buf->constant->character_constant = (*pp_tokens)->character_constant;
      break;

    case PP_STRING_LITERAL:
      buf = MALLOC(sizeof(struct PPToken));
      buf->kind = TOKEN_STRING_LITERAL;
      buf->string_literal = (*pp_tokens)->string_literal;
      break;

    case PP_PUNCTUATOR:
      buf = MALLOC(sizeof(struct PPToken));
      buf->kind = TOKEN_PUNCTUATOR;
      buf->punctuator = (*pp_tokens)->punctuator;
      break;

    case PP_NWSC:
    case PP_HEADER_NAME:
      PANIC();
    }

    if (buf) {
      PUSH_BACK(struct Token *, result, count, buf);
    }
  }

  return result;
}

static void unescape(char *const str) {
  char *c = str;

  while (*c) {
    if (*c != '\\') {
      ++c;
      continue;
    }

    c[0] = DEL;
    switch (c[1]) {
    case '\'':
    case '\"':
    case '\?':
    case '\\':
      break;

    case 'a':
      c[1] = '\a';
      break;
    case 'b':
      c[1] = '\b';
      break;
    case 'f':
      c[1] = '\f';
      break;
    case 'n':
      c[1] = '\n';
      break;
    case 'r':
      c[1] = '\r';
      break;
    case 't':
      c[1] = '\t';
      break;
    case 'v':
      c[1] = '\v';
      break;

    default:
      ERROR("Invalid escape sequence: \\%c (%s\\x%x)", c[1],
            (c[1] < 0 ? "-" : ""), (unsigned)c[1]);
    }

    c += 2;
  }

  remove_str(str, DEL);
}

bool str_to_keyword(const char *begin, const char *end, enum Keyword *buf) {
  if (str_equals("_Alignas", begin, end)) {
    *buf = ALIGNAS;
  } else if (str_equals("_Alignof", begin, end)) {
    *buf = ALIGNOF;
  } else if (str_equals("_Atomic", begin, end)) {
    *buf = ATOMIC;
  } else if (str_equals("auto", begin, end)) {
    *buf = AUTO;
  } else if (str_equals("_Bool", begin, end)) {
    *buf = BOOL;
  } else if (str_equals("break", begin, end)) {
    *buf = BREAK;
  } else if (str_equals("case", begin, end)) {
    *buf = CASE;
  } else if (str_equals("char", begin, end)) {
    *buf = CHAR;
  } else if (str_equals("_Complex", begin, end)) {
    *buf = COMPLEX;
  } else if (str_equals("const", begin, end)) {
    *buf = CONST;
  } else if (str_equals("continue", begin, end)) {
    *buf = CONTINUE;
  } else if (str_equals("default", begin, end)) {
    *buf = DEFAULT;
  } else if (str_equals("do", begin, end)) {
    *buf = DO;
  } else if (str_equals("double", begin, end)) {
    *buf = DOUBLE;
  } else if (str_equals("else", begin, end)) {
    *buf = ELSE;
  } else if (str_equals("enum", begin, end)) {
    *buf = ENUM;
  } else if (str_equals("extern", begin, end)) {
    *buf = EXTERN;
  } else if (str_equals("float", begin, end)) {
    *buf = FLOAT;
  } else if (str_equals("for", begin, end)) {
    *buf = FOR;
  } else if (str_equals("_Generic", begin, end)) {
    *buf = GENERIC;
  } else if (str_equals("goto", begin, end)) {
    *buf = GOTO;
  } else if (str_equals("if", begin, end)) {
    *buf = IF;
  } else if (str_equals("_Imaginary", begin, end)) {
    *buf = IMAGINARY;
  } else if (str_equals("inline", begin, end)) {
    *buf = INLINE;
  } else if (str_equals("int", begin, end)) {
    *buf = INT;
  } else if (str_equals("long", begin, end)) {
    *buf = LONG;
  } else if (str_equals("_Noreturn", begin, end)) {
    *buf = NORETURN;
  } else if (str_equals("register", begin, end)) {
    *buf = REGISTER;
  } else if (str_equals("restrict", begin, end)) {
    *buf = RESTRICT;
  } else if (str_equals("return", begin, end)) {
    *buf = RETURN;
  } else if (str_equals("short", begin, end)) {
    *buf = SHORT;
  } else if (str_equals("signed", begin, end)) {
    *buf = SIGNED;
  } else if (str_equals("sizeof", begin, end)) {
    *buf = SIZEOF;
  } else if (str_equals("static", begin, end)) {
    *buf = STATIC;
  } else if (str_equals("_Static_assert", begin, end)) {
    *buf = STATIC_ASSERT;
  } else if (str_equals("struct", begin, end)) {
    *buf = STRUCT;
  } else if (str_equals("switch", begin, end)) {
    *buf = SWITCH;
  } else if (str_equals("_Thread_local", begin, end)) {
    *buf = THREAD_LOCAL;
  } else if (str_equals("typedef", begin, end)) {
    *buf = TYPEDEF;
  } else if (str_equals("union", begin, end)) {
    *buf = UNION;
  } else if (str_equals("unsigned", begin, end)) {
    *buf = UNSIGNED;
  } else if (str_equals("void", begin, end)) {
    *buf = VOID;
  } else if (str_equals("volatile", begin, end)) {
    *buf = VOLATILE;
  } else if (str_equals("while", begin, end)) {
    *buf = WHILE;
  } else {
    return false;
  }
  return true;
}

static struct Token *convert_pp_identifier(struct Identifier *identifier) {
  struct Token *result = MALLOC(sizeof(struct Token));
  enum Keyword keyword;

  if (str_to_keyword(identifier->value, NULL, &keyword)) {
    result->kind = TOKEN_KEYWORD;
    result->keyword = keyword;
  } else {
    result->kind = TOKEN_IDENTIFIER;
    result->identifier = identifier;
  }
  return result;
}

static struct Token *convert_pp_number(struct PPNumber *pp_number) {
  struct Token *result = MALLOC(sizeof(struct Token));

  result->kind = TOKEN_CONSTANT;
  result->constant = MALLOC(sizeof(struct Constant));

  // I think the code below should be moved into another function
  if (match_decimal_constant(pp_number->value)) {
    result->constant->kind = INTEGER_CONSTANT;
    result->constant->integer_constant = MALLOC(sizeof(struct IntegerConstant));
    result->constant->integer_constant->value = /* FIXME */;
  } else if (match_octal_constant(pp_number->value)) {
    result->constant->kind = INTEGER_CONSTANT;
    result->constant->integer_constant = MALLOC(sizeof(struct IntegerConstant));
    result->constant->integer_constant->value = /* FIXME */;
  } else if (match_hexdecimal_constant(pp_number->value)) {
    result->constant->kind = INTEGER_CONSTANT;
    result->constant->integer_constant = MALLOC(sizeof(struct IntegerConstant));
    result->constant->integer_constant->value = /* FIXME */;
  } else {
    PANIC();
  }
}

// vim: set ft=c ts=2 sw=2 et:
