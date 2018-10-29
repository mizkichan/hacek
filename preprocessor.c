#include "alloc.h"
#include "error.h"
#include "preprocessor.h"
#include "utils.h"
#include <ctype.h>

static void unescape(char *) __attribute__((nonnull));
static bool str_to_keyword(const char *, enum Keyword *)
    __attribute__((nonnull));

void execute_pp_directives(struct PPToken **pp_tokens) {
  ERROR("Not implemented yet");
}

void convert_escape_sequences(struct PPToken **pp_tokens) {
  // FIXME only simple-escape-sequence is implemented

  for (struct PPToken *pp_token = *pp_tokens; pp_token; ++pp_token) {
    char *str;
    if (pp_token->kind == PP_CHARACTER_CONSTANT) {
      str = pp_token->character_constant.chars;
    } else if (pp_token->kind == PP_STRING_LITERAL) {
      str = pp_token->string_literal.chars;
    } else {
      break;
    }

    unescape(str);
  }
}

void concatenate_adjacent_string_literals(struct PPToken **pp_tokens) {
  ERROR("Not implemented yet");
}

struct Token **convert_pp_tokens_into_tokens(struct PPToken **pp_tokens) {
  size_t token_count = 0;
  struct Token **tokens = NULL;
  struct Token *buf = MALLOC(sizeof(struct Token));

  for (struct PPToken *pp_token = *pp_tokens; pp_token; ++pp_token) {
    switch (pp_token->kind) {
    case PP_IDENTIFIER:
      // identifier can be either keyword, identifier or enumeration constant.

      if (str_to_keyword(pp_token->chars, &buf->keyword)) {
        buf->kind = TOKEN_KEYWORD;
      } else {
        // NOTE THAT THIS STUFF CAN BE ENUMERATION CONSTANT!
        buf->kind = TOKEN_IDENTIFIER;
        buf->chars = pp_token->chars;
      }
      break;

    case PP_NUMBER:
      // number can be either integer constant or floating constant.
      ERROR("Not implemented yet");
      break;

    case PP_CHARACTER_CONSTANT:
      // character constant can only be character constant.
      ERROR("Not implemented yet");
      break;

    case PP_STRING_LITERAL:
      // string literal can only be string literal.
      ERROR("Not implemented yet");
      break;

    case PP_PUNCTUATOR:
      // punctuator can only be punctuator.
      ERROR("Not implemented yet");
      break;

    case PP_HEADER_NAME:
    case PP_NWSC:
    case PP_NEWLINE:
      ERROR("Preprocessing token `%s` must not be appeared at this phase.",
            pp_token_kind_str((int)pp_token->kind));
    case PP_WHITE_SPACE_CHARACTERS:
        // do nothing.
        ;
    }

    PUSH_BACK(struct Token *, tokens, token_count, buf);
    buf = MALLOC(sizeof(struct PPToken));
  }

  FREE(buf);
  return tokens;
}

static void unescape(char *str) {
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

bool str_to_keyword(const char *str, enum Keyword *buf) {
  if (str_equals(str, "_Alignas")) {
    *buf = ALIGNAS;
  } else if (str_equals(str, "_Alignof")) {
    *buf = ALIGNOF;
  } else if (str_equals(str, "_Atomic")) {
    *buf = ATOMIC;
  } else if (str_equals(str, "auto")) {
    *buf = AUTO;
  } else if (str_equals(str, "_Bool")) {
    *buf = BOOL;
  } else if (str_equals(str, "break")) {
    *buf = BREAK;
  } else if (str_equals(str, "case")) {
    *buf = CASE;
  } else if (str_equals(str, "char")) {
    *buf = CHAR;
  } else if (str_equals(str, "_Complex")) {
    *buf = COMPLEX;
  } else if (str_equals(str, "const")) {
    *buf = CONST;
  } else if (str_equals(str, "continue")) {
    *buf = CONTINUE;
  } else if (str_equals(str, "default")) {
    *buf = DEFAULT;
  } else if (str_equals(str, "do")) {
    *buf = DO;
  } else if (str_equals(str, "double")) {
    *buf = DOUBLE;
  } else if (str_equals(str, "else")) {
    *buf = ELSE;
  } else if (str_equals(str, "enum")) {
    *buf = ENUM;
  } else if (str_equals(str, "extern")) {
    *buf = EXTERN;
  } else if (str_equals(str, "float")) {
    *buf = FLOAT;
  } else if (str_equals(str, "for")) {
    *buf = FOR;
  } else if (str_equals(str, "_Generic")) {
    *buf = GENERIC;
  } else if (str_equals(str, "goto")) {
    *buf = GOTO;
  } else if (str_equals(str, "if")) {
    *buf = IF;
  } else if (str_equals(str, "_Imaginary")) {
    *buf = IMAGINARY;
  } else if (str_equals(str, "inline")) {
    *buf = INLINE;
  } else if (str_equals(str, "int")) {
    *buf = INT;
  } else if (str_equals(str, "long")) {
    *buf = LONG;
  } else if (str_equals(str, "_Noreturn")) {
    *buf = NORETURN;
  } else if (str_equals(str, "register")) {
    *buf = REGISTER;
  } else if (str_equals(str, "restrict")) {
    *buf = RESTRICT;
  } else if (str_equals(str, "return")) {
    *buf = RETURN;
  } else if (str_equals(str, "short")) {
    *buf = SHORT;
  } else if (str_equals(str, "signed")) {
    *buf = SIGNED;
  } else if (str_equals(str, "sizeof")) {
    *buf = SIZEOF;
  } else if (str_equals(str, "static")) {
    *buf = STATIC;
  } else if (str_equals(str, "_Static_assert")) {
    *buf = STATIC_ASSERT;
  } else if (str_equals(str, "struct")) {
    *buf = STRUCT;
  } else if (str_equals(str, "switch")) {
    *buf = SWITCH;
  } else if (str_equals(str, "_Thread_local")) {
    *buf = THREAD_LOCAL;
  } else if (str_equals(str, "typedef")) {
    *buf = TYPEDEF;
  } else if (str_equals(str, "union")) {
    *buf = UNION;
  } else if (str_equals(str, "unsigned")) {
    *buf = UNSIGNED;
  } else if (str_equals(str, "void")) {
    *buf = VOID;
  } else if (str_equals(str, "volatile")) {
    *buf = VOLATILE;
  } else if (str_equals(str, "while")) {
    *buf = WHILE;
  } else {
    return false;
  }
  return true;
}

// vim: set ft=c ts=2 sw=2 et:
