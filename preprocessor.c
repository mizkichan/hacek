#include "alloc.h"
#include "error.h"
#include "preprocessor.h"
#include "utils.h"
#include <ctype.h>

static void unescape(char *, char *) __attribute__((nonnull));
static bool str_to_keyword(const char *, const char *, enum Keyword *)
    __attribute__((nonnull));

void execute_pp_directives(struct PPToken **pp_tokens) {
  ERROR("Not implemented yet");
}

void convert_escape_sequences(struct PPToken **pp_tokens) {
  // FIXME only simple-escape-sequence is implemented

  for (struct PPToken *pp_token = *pp_tokens; pp_token; ++pp_token) {
    if (pp_token->kind != PP_CHARACTER_CONSTANT ||
        pp_token->kind != PP_STRING_LITERAL) {
      break;
    }

    unescape(pp_token->begin, pp_token->end);
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

      if (str_to_keyword(pp_token->begin, pp_token->end, &buf->keyword)) {
        buf->kind = TOKEN_KEYWORD;
      } else {
        // NOTE THAT THIS STUFF CAN BE ENUMERATION CONSTANT!
        buf->kind = TOKEN_IDENTIFIER;
        buf->begin = pp_token->begin;
        buf->end = pp_token->end;
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
      break;
    }

    PUSH_BACK(struct Token *, tokens, token_count, buf);
    buf = MALLOC(sizeof(struct PPToken));
  }

  FREE(buf);
  return tokens;
}

static void unescape(char *begin, char *end) {
  char *c = begin;

  while (c < end) {
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

  remove_str(begin, DEL);
}

bool str_to_keyword(const char *begin, const char *end, enum Keyword *buf) {
  if (str_range_equals("_Alignas", begin, end)) {
    *buf = ALIGNAS;
  } else if (str_range_equals("_Alignof", begin, end)) {
    *buf = ALIGNOF;
  } else if (str_range_equals("_Atomic", begin, end)) {
    *buf = ATOMIC;
  } else if (str_range_equals("auto", begin, end)) {
    *buf = AUTO;
  } else if (str_range_equals("_Bool", begin, end)) {
    *buf = BOOL;
  } else if (str_range_equals("break", begin, end)) {
    *buf = BREAK;
  } else if (str_range_equals("case", begin, end)) {
    *buf = CASE;
  } else if (str_range_equals("char", begin, end)) {
    *buf = CHAR;
  } else if (str_range_equals("_Complex", begin, end)) {
    *buf = COMPLEX;
  } else if (str_range_equals("const", begin, end)) {
    *buf = CONST;
  } else if (str_range_equals("continue", begin, end)) {
    *buf = CONTINUE;
  } else if (str_range_equals("default", begin, end)) {
    *buf = DEFAULT;
  } else if (str_range_equals("do", begin, end)) {
    *buf = DO;
  } else if (str_range_equals("double", begin, end)) {
    *buf = DOUBLE;
  } else if (str_range_equals("else", begin, end)) {
    *buf = ELSE;
  } else if (str_range_equals("enum", begin, end)) {
    *buf = ENUM;
  } else if (str_range_equals("extern", begin, end)) {
    *buf = EXTERN;
  } else if (str_range_equals("float", begin, end)) {
    *buf = FLOAT;
  } else if (str_range_equals("for", begin, end)) {
    *buf = FOR;
  } else if (str_range_equals("_Generic", begin, end)) {
    *buf = GENERIC;
  } else if (str_range_equals("goto", begin, end)) {
    *buf = GOTO;
  } else if (str_range_equals("if", begin, end)) {
    *buf = IF;
  } else if (str_range_equals("_Imaginary", begin, end)) {
    *buf = IMAGINARY;
  } else if (str_range_equals("inline", begin, end)) {
    *buf = INLINE;
  } else if (str_range_equals("int", begin, end)) {
    *buf = INT;
  } else if (str_range_equals("long", begin, end)) {
    *buf = LONG;
  } else if (str_range_equals("_Noreturn", begin, end)) {
    *buf = NORETURN;
  } else if (str_range_equals("register", begin, end)) {
    *buf = REGISTER;
  } else if (str_range_equals("restrict", begin, end)) {
    *buf = RESTRICT;
  } else if (str_range_equals("return", begin, end)) {
    *buf = RETURN;
  } else if (str_range_equals("short", begin, end)) {
    *buf = SHORT;
  } else if (str_range_equals("signed", begin, end)) {
    *buf = SIGNED;
  } else if (str_range_equals("sizeof", begin, end)) {
    *buf = SIZEOF;
  } else if (str_range_equals("static", begin, end)) {
    *buf = STATIC;
  } else if (str_range_equals("_Static_assert", begin, end)) {
    *buf = STATIC_ASSERT;
  } else if (str_range_equals("struct", begin, end)) {
    *buf = STRUCT;
  } else if (str_range_equals("switch", begin, end)) {
    *buf = SWITCH;
  } else if (str_range_equals("_Thread_local", begin, end)) {
    *buf = THREAD_LOCAL;
  } else if (str_range_equals("typedef", begin, end)) {
    *buf = TYPEDEF;
  } else if (str_range_equals("union", begin, end)) {
    *buf = UNION;
  } else if (str_range_equals("unsigned", begin, end)) {
    *buf = UNSIGNED;
  } else if (str_range_equals("void", begin, end)) {
    *buf = VOID;
  } else if (str_range_equals("volatile", begin, end)) {
    *buf = VOLATILE;
  } else if (str_range_equals("while", begin, end)) {
    *buf = WHILE;
  } else {
    return false;
  }
  return true;
}

// vim: set ft=c ts=2 sw=2 et:
