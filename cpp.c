#include "cpp.h"
#include "error.h"
#include "tokens.h"
#include "utils.h"
#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

struct PPTokenList preprocess(char *input) {
  return cpp_tokenize(input);
}

struct PPTokenList cpp_tokenize(char *input) {
  size_t token_count = 0;
  struct PPToken *pp_tokens = NULL;
  struct PPToken tokenbuf;

  while (*input != '\0') {
    skip_whitespaces(&input);

    struct PPToken *last = NULL, *one_before_last = NULL;
    if (token_count >= 2) {
      last = &pp_tokens[token_count - 1];
      one_before_last = &pp_tokens[token_count - 2];
    }
    if (!(((last != NULL && one_before_last != NULL) &&
           (one_before_last->kind == PP_PUNCTUATOR &&
            one_before_last->punctuator == SIGN) &&
           (last->kind == PP_IDENTIFIER &&
            strcmp(last->chars, "include") == 0) &&
           match_header_name(&input, &tokenbuf)) ||

          match_identifier(&input, &tokenbuf) ||
          match_pp_number(&input, &tokenbuf) ||
          match_character_constant(&input, &tokenbuf) ||
          match_string_literal(&input, &tokenbuf) ||
          match_punctuator(&input, &tokenbuf))) {
      if (*input == '\n') {
        // new-line character is retained
        tokenbuf.kind = PP_NEWLINE;
      } else {
        // non-white-space character
        tokenbuf.kind = PP_NWSC;
        tokenbuf.nwsc = *input;
      }
      ++input;
    }
    pp_tokens =
        push_back(pp_tokens, token_count, &tokenbuf, sizeof(struct PPToken));
    ++token_count;
  }

  return (struct PPTokenList){.length = token_count, .pp_tokens = pp_tokens};
}

void cpp_concat_string_literals(struct PPTokenList *token_list) {
  struct PPToken *end, *current, *next;
  end = token_list->pp_tokens + token_list->length;

  for (size_t i = 0; i < token_list->length - 1; ++i) {
    current = &token_list->pp_tokens[i];
    next = &token_list->pp_tokens[i + 1];

    if (current->kind == PP_STRING_LITERAL && next->kind == PP_STRING_LITERAL) {
      current->string_literal.chars =
          append_str(current->string_literal.chars, next->string_literal.chars);
    }

    erase(end, next, next + 1);
    --token_list->length;
  }
}

struct TokenList cpp_convert_into_token(struct PPTokenList *token_list) {
  struct PPToken *pp_token;
  struct Token tokenbuf;
  struct Token *tokens = NULL;
  size_t length = 0;

  for (size_t i = 0; i < token_list->length; ++i) {
    pp_token = &token_list->pp_tokens[i];

    switch (pp_token->kind) {
    case PP_IDENTIFIER:
      // identifier can be either keyword, identifier or enumeration constant.

      if (str_to_keyword(pp_token->chars, &tokenbuf.keyword)) {
        tokenbuf.kind = TOKEN_KEYWORD;
      } else {
        // NOTE THAT THIS STUFF CAN BE ENUMERATION CONSTANT!
        tokenbuf.kind = TOKEN_IDENTIFIER;
        tokenbuf.chars = pp_token->chars;
      }
      break;

    case PP_NUMBER:
      // number can be either integer constant or floating constant.
      ERROR("Not implemented yet");

    case PP_CHARACTER_CONSTANT:
      // character constant can only be character constant.
      ERROR("Not implemented yet");

    case PP_STRING_LITERAL:
      // string literal can only be string literal.
      ERROR("Not implemented yet");

    case PP_PUNCTUATOR:
      // punctuator can only be punctuator.
      ERROR("Not implemented yet");

    case PP_HEADER_NAME:
    case PP_NWSC:
    case PP_NEWLINE:
      ERROR("Preprocessing token `%s` must not be appeared at this phase.",
            pp_token_kind_str((int)pp_token->kind));
    }

    tokens = push_back(tokens, length++, &tokenbuf, sizeof(struct Token));
  }

  return (struct TokenList){.length = length, .tokens = tokens};
}

void skip_whitespaces(char **c) {
  while (is_whitespace(**c) && **c != '\n') {
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

bool match_header_name(char **c, struct PPToken *buf) {
  char *begin, *end;
  enum HeaderNameKind kind;

  if (**c == '<') {
    kind = H_CHAR_SEQUENCE;
  } else if (**c == '"') {
    kind = Q_CHAR_SEQUENCE;
  } else {
    return false;
  }

  (*c) += 1;
  begin = *c;

  while (**c != '\n' && ((kind == H_CHAR_SEQUENCE && **c != '>') ||
                         (kind == Q_CHAR_SEQUENCE && **c != '"'))) {
    (*c) += 1;
  }

  end = *c;

  if ((kind == H_CHAR_SEQUENCE && **c != '>') ||
      (kind == Q_CHAR_SEQUENCE && **c != '"')) {
    return false;
  }
  (*c) += 1;

  char *chars = clone_str_range(begin, end);

  buf->kind = PP_HEADER_NAME;
  buf->header_name.kind = kind;
  buf->header_name.chars = chars;
  return true;
}

bool match_identifier(char **c, struct PPToken *buf) {
  char *begin = *c;

  if (!is_nondigit(**c)) {
    return false;
  }

  do {
    (*c) += 1;
    // TODO handle universal-character-name
  } while (is_nondigit(**c) || is_digit(**c));

  char *chars = clone_str_range(begin, *c);

  buf->kind = PP_IDENTIFIER;
  buf->chars = chars;
  return true;
}

bool match_pp_number(char **c, struct PPToken *buf) {
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

  char *chars = clone_str_range(begin, *c);

  buf->kind = PP_NUMBER;
  buf->chars = chars;
  return true;
}

bool match_character_constant(char **c, struct PPToken *buf) {
  enum CharacterConstantPrefix prefix;

  if ((*c)[0] == 'L') {
    prefix = CHARACTER_CONSTANT_PREFIX_WCHAR;
    (*c) += 1;
  } else if ((*c)[0] == 'u') {
    prefix = CHARACTER_CONSTANT_PREFIX_CHAR16;
    (*c) += 1;
  } else if ((*c)[0] == 'U') {
    prefix = CHARACTER_CONSTANT_PREFIX_CHAR32;
    (*c) += 1;
  } else {
    prefix = CHARACTER_CONSTANT_PREFIX_NONE;
  }

  if (**c != '\'') {
    return false;
  }
  ++(*c);

  char *chars = NULL;
  size_t length = 0;
  while (**c != '\'') {
    EXIT_MESSAGE_IF(
        **c == '\n',
        "Newline character must not be appear in a character constant.\n");
    chars = push_back_char(chars, length++, read_char(c));
  }
  chars = push_back_char(chars, length, '\0');
  ++(*c);

  buf->kind = PP_CHARACTER_CONSTANT;
  buf->character_constant.prefix = prefix;
  buf->character_constant.chars = chars;
  return true;
}

bool match_string_literal(char **c, struct PPToken *buf) {
  enum EncodingPrefix encoding_prefix;

  if ((*c)[0] == 'u') {
    if ((*c)[1] == 8) {
      encoding_prefix = ENCODING_PREFIX_UTF8;
      (*c) += 2;
    } else {
      encoding_prefix = ENCODING_PREFIX_CHAR16;
      (*c) += 1;
    }
  } else if ((*c)[0] == 'U') {
    encoding_prefix = ENCODING_PREFIX_CHAR32;
    (*c) += 1;
  } else if ((*c)[0] == 'L') {
    encoding_prefix = ENCODING_PREFIX_WCHAR;
    (*c) += 1;
  } else {
    encoding_prefix = ENCODING_PREFIX_NONE;
  }

  if (**c != '"') {
    return false;
  }
  ++(*c);

  char *chars = NULL;
  size_t length = 0;
  while (**c != '"') {
    EXIT_MESSAGE_IF(
        **c == '\n',
        "Newline character must not be appear in a string literal.\n");
    chars = push_back_char(chars, length++, read_char(c));
  }
  chars = push_back_char(chars, length, '\0');
  ++(*c);

  buf->kind = PP_STRING_LITERAL;
  buf->string_literal.encoding_prefix = encoding_prefix;
  buf->string_literal.chars = chars;
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

char read_char(char **c) {
  // FIXME only simple-escape-sequence is implemented
  char tmp;

  tmp = **c;
  (*c) += 1;

  if (tmp != '\\') {
    return tmp;
  }

  tmp = **c;
  (*c) += 1;
  switch (tmp) {
  case '\'':
  case '"':
  case '?':
  case '\\':
    return **c;

  case 'a':
    return '\a';
  case 'b':
    return '\b';
  case 'f':
    return '\f';
  case 'n':
    return '\n';
  case 'r':
    return '\r';
  case 't':
    return '\t';
  case 'v':
    return '\v';

  default:
    EXIT_MESSAGE("Invalid escape sequence: \\%c (\\x%x)", **c, **c);
  }
}

bool is_nondigit(char c) { return isalpha(c) || c == '_'; }
bool is_digit(char c) { return isdigit(c); }
bool is_whitespace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f';
}

// vim: set ft=c ts=2 sw=2 et:
