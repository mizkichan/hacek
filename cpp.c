#include "cpp.h"
#include "error.h"
#include "utils.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define TMP_PATHNAME "/tmp/hacek"

struct PPTokenList preprocess(char *input) {
  FILE *fp = popen(
      "clang -E -std=c17 -Weverything - | sed 's/^#.*$//' > " TMP_PATHNAME,
      "w");
  PANIC_IF(fp == (FILE *)NULL);

  for (char *c = input; *c != '\0'; ++c) {
    ERROR_IF((fputc(*c, fp) == EOF), "fputc() returned EOF");
  }

  WARN_IF(pclose(fp) == -1, "failed to close pipe");

  char *buf = read_from_file(TMP_PATHNAME);
  ERROR_IF(buf == (char *)NULL, TMP_PATHNAME);

  WARN_IF(unlink(TMP_PATHNAME) == -1, TMP_PATHNAME);

  return cpp_tokenize(buf);
}

struct PPTokenList cpp_tokenize(char *input) {
  size_t token_count = 0;
  struct PPToken *pp_tokens = NULL;
  struct PPToken tokenbuf;

  while (*input != '\0') {
    skip_whitespaces(&input);

    if (match_identifier(&input, &tokenbuf)) {
      push_back(pp_tokens, token_count, &tokenbuf, sizeof(struct PPToken));
      ++token_count;
      continue;
    }

    if (match_pp_number(&input, &tokenbuf)) {
      push_back(pp_tokens, token_count, &tokenbuf, sizeof(struct PPToken));
      ++token_count;
      continue;
    }

    if (match_punctuator(&input, &tokenbuf)) {
      push_back(pp_tokens, token_count, &tokenbuf, sizeof(struct PPToken));
      ++token_count;
      continue;
    }

    ERROR("Unexpected character: '%c' (0x%x)", *input, *input);
  }

  return (struct PPTokenList){.length = token_count, .pp_tokens = pp_tokens};
}

void skip_whitespaces(char **c) {
  while (is_whitespace(**c)) {
    (*c) += 1;

    if ((*c)[0] == '/') {
      if ((*c)[1] == '/') {
        // one-line comment
        (*c) += 2;
        while (**c != '\n') {
          (*c) += 1;
        }
      } else if ((*c)[1] == '*') {
        // multi-line comment
        (*c) += 2;
        while ((*c)[-2] != '*' && (*c)[-1] != '/') {
          (*c) += 1;
        }
      }
    }
  }
}

bool match_identifier(char **c, struct PPToken *buf) {
  size_t length;
  ptrdiff_t diff;
  char *begin = *c;

  if (!is_nondigit(**c)) {
    return false;
  }

  do {
    (*c) += 1;
    // TODO handle universal-character-name
  } while (is_nondigit(**c) || is_digit(**c));

  diff = *c - begin;
  assert(diff > 0);
  length = (size_t)diff;

  char *chars = checked_malloc(sizeof(char) * length + 1);
  strncpy(chars, begin, length);
  chars[length] = '\0';

  buf->kind = PP_IDENTIFIER;
  buf->chars = chars;
  return true;
}

bool match_pp_number(char **c, struct PPToken *buf) {
  size_t length;
  ptrdiff_t diff;
  char *begin = *c;

  if (is_digit((*c)[0])) {
    (*c) += 1;
  } else if (((*c)[0] == '.') && is_digit((*c)[1])) {
    (*c) += 2;
  } else {
    return false;
  }

  // TODO handle universal-character-name
  while (is_digit(**c) || is_nondigit(**c) || **c == 'e' || **c == 'E' ||
         **c == 'p' || **c == 'P' || **c == '.') {
    (*c) += 1;
  }

  diff = *c - begin;
  assert(diff > 0);

  length = (size_t)diff;
  char *chars = checked_malloc(sizeof(char) * length + 1);
  strncpy(chars, begin, length);
  chars[length] = '\0';

  buf->kind = PP_NUMBER;
  buf->chars = chars;
  return true;
}

bool match_punctuator(char **c, struct PPToken *buf) {
  enum Punctuator punctuator;

  if ((*c)[0] == '%' && (*c)[1] == ':' && (*c)[2] == '%' && (*c)[3] == ':') {
    punctuator = DIGRAPH_DOUBLE_SIGN;
    (*c) += 4;
  }

  else if ((*c)[0] == '.' && (*c)[0] == '.' && (*c)[0] == '.') {
    punctuator = ELLIPSE;
    (*c) += 3;
  } else if ((*c)[0] == '<' && (*c)[0] == '<' && (*c)[0] == '=') {
    punctuator = LEFT_SHIFT_ASSIGN;
    (*c) += 3;
  } else if ((*c)[0] == '>' && (*c)[0] == '>' && (*c)[0] == '=') {
    punctuator = RIGHT_SHIFT_ASSIGN;
    (*c) += 3;
  }

  else if ((*c)[0] == '!' && (*c)[1] == '=') {
    punctuator = NOT_EQUAL;
    (*c) += 2;
  } else if ((*c)[0] == '#' && (*c)[1] == '#') {
    punctuator = DOUBLE_SIGN;
    (*c) += 2;
  } else if ((*c)[0] == '%' && (*c)[1] == ':') {
    punctuator = DIGRAPH_SIGN;
    (*c) += 2;
  } else if ((*c)[0] == '%' && (*c)[1] == '=') {
    punctuator = REMIND_ASSIGN;
    (*c) += 2;
  } else if ((*c)[0] == '%' && (*c)[1] == '>') {
    punctuator = DIGRAPH_RIGHT_BRACE;
    (*c) += 2;
  } else if ((*c)[0] == '&' && (*c)[1] == '&') {
    punctuator = LOGICAL_AND;
    (*c) += 2;
  } else if ((*c)[0] == '&' && (*c)[1] == '=') {
    punctuator = AND_ASSIGN;
    (*c) += 2;
  } else if ((*c)[0] == '*' && (*c)[1] == '=') {
    punctuator = MULTIPLY_ASSIGN;
    (*c) += 2;
  } else if ((*c)[0] == '+' && (*c)[1] == '+') {
    punctuator = INCREMENT;
    (*c) += 2;
  } else if ((*c)[0] == '+' && (*c)[1] == '=') {
    punctuator = ADD_ASSIGN;
    (*c) += 2;
  } else if ((*c)[0] == '-' && (*c)[1] == '-') {
    punctuator = DECREMENT;
    (*c) += 2;
  } else if ((*c)[0] == '-' && (*c)[1] == '=') {
    punctuator = SUBTRACT_ASSIGN;
    (*c) += 2;
  } else if ((*c)[0] == '-' && (*c)[1] == '>') {
    punctuator = ARROW;
    (*c) += 2;
  } else if ((*c)[0] == '/' && (*c)[1] == '=') {
    punctuator = DIVIDE_ASSIGN;
    (*c) += 2;
  } else if ((*c)[0] == ':' && (*c)[1] == '>') {
    punctuator = DIGRAPH_RIGHT_BRACKET;
    (*c) += 2;
  } else if ((*c)[0] == '<' && (*c)[1] == '%') {
    punctuator = DIGRAPH_LEFT_BRACE;
    (*c) += 2;
  } else if ((*c)[0] == '<' && (*c)[1] == ':') {
    punctuator = DIGRAPH_LEFT_BRACKET;
    (*c) += 2;
  } else if ((*c)[0] == '<' && (*c)[1] == '<') {
    punctuator = LEFT_SHIFT;
    (*c) += 2;
  } else if ((*c)[0] == '<' && (*c)[1] == '=') {
    punctuator = LESS_EQUAL;
    (*c) += 2;
  } else if ((*c)[0] == '=' && (*c)[1] == '=') {
    punctuator = EQUAL;
    (*c) += 2;
  } else if ((*c)[0] == '>' && (*c)[1] == '=') {
    punctuator = GREATER_EQUAL;
    (*c) += 2;
  } else if ((*c)[0] == '>' && (*c)[1] == '>') {
    punctuator = RIGHT_SHIFT;
    (*c) += 2;
  } else if ((*c)[0] == '^' && (*c)[1] == '=') {
    punctuator = EXCLUSIVE_OR_ASSIGN;
    (*c) += 2;
  } else if ((*c)[0] == '|' && (*c)[1] == '=') {
    punctuator = INCLUSIVE_OR_ASSIGN;
    (*c) += 2;
  } else if ((*c)[0] == '|' && (*c)[1] == '|') {
    punctuator = LOGICAL_OR;
    (*c) += 2;
  }

  else if ((*c)[0] == '!') {
    punctuator = EXCLAMATION;
    (*c) += 1;
  } else if ((*c)[0] == '#') {
    punctuator = SIGN;
    (*c) += 1;
  } else if ((*c)[0] == '%') {
    punctuator = REMIND;
    (*c) += 1;
  } else if ((*c)[0] == '&') {
    punctuator = AMPASAND;
    (*c) += 1;
  } else if ((*c)[0] == '(') {
    punctuator = LEFT_PAREN;
    (*c) += 1;
  } else if ((*c)[0] == ')') {
    punctuator = RIGHT_PAREN;
    (*c) += 1;
  } else if ((*c)[0] == '*') {
    punctuator = ASTERISK;
    (*c) += 1;
  } else if ((*c)[0] == '+') {
    punctuator = PLUS;
    (*c) += 1;
  } else if ((*c)[0] == ',') {
    punctuator = COMMA;
    (*c) += 1;
  } else if ((*c)[0] == '-') {
    punctuator = MINUS;
    (*c) += 1;
  } else if ((*c)[0] == '.') {
    punctuator = DOT;
    (*c) += 1;
  } else if ((*c)[0] == '/') {
    punctuator = DIVIDE;
    (*c) += 1;
  } else if ((*c)[0] == ':') {
    punctuator = COLON;
    (*c) += 1;
  } else if ((*c)[0] == ';') {
    punctuator = SEMICOLON;
    (*c) += 1;
  } else if ((*c)[0] == '<') {
    punctuator = LESS_THAN;
    (*c) += 1;
  } else if ((*c)[0] == '=') {
    punctuator = ASSIGN;
    (*c) += 1;
  } else if ((*c)[0] == '>') {
    punctuator = GREATER_THAN;
    (*c) += 1;
  } else if ((*c)[0] == '?') {
    punctuator = QUESTION;
    (*c) += 1;
  } else if ((*c)[0] == '[') {
    punctuator = LEFT_BRACKET;
    (*c) += 1;
  } else if ((*c)[0] == ']') {
    punctuator = RIGHT_BRACKET;
    (*c) += 1;
  } else if ((*c)[0] == '^') {
    punctuator = EXCLUSIVE_OR;
    (*c) += 1;
  } else if ((*c)[0] == '{') {
    punctuator = LEFT_BRACE;
    (*c) += 1;
  } else if ((*c)[0] == '|') {
    punctuator = INCLUSIVE_OR;
    (*c) += 1;
  } else if ((*c)[0] == '}') {
    punctuator = RIGHT_BRACE;
    (*c) += 1;
  } else if ((*c)[0] == '~') {
    punctuator = NEGATE;
    (*c) += 1;
  }

  else {
    return false;
  }

  buf->kind = PP_PUNCTUATOR;
  buf->punctuator = punctuator;
  return true;
}

bool is_nondigit(char c) { return isalpha(c) || c == '_'; }
bool is_digit(char c) { return isdigit(c); }
bool is_whitespace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f';
}

// vim: set ft=c ts=2 sw=2 et:
