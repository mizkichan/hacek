#include "error.h"
#include "preprocessor.h"
#include "utils.h"
#include <ctype.h>

struct PPTokenList preprocess(char *input) {
  return pp_tokenize(input);
}

struct PPTokenList pp_tokenize(char *input) {
  struct PPTokenList result = {0, NULL};
  struct PPToken buf, *last, *one_before_last;

  while (*input != '\0') {
    skip_whitespaces(&input);

    last = NULL;
    one_before_last = NULL;
    if (result.length >= 2) {
      last = &result.pp_tokens[result.length - 1];
      one_before_last = &result.pp_tokens[result.length - 2];
    }
    if (!(((last != NULL && one_before_last != NULL) &&
           (one_before_last->kind == PP_PUNCTUATOR &&
            one_before_last->punctuator == SIGN) &&
           (last->kind == PP_IDENTIFIER &&
            str_equals(last->chars, "include")) &&
           match_header_name(&input, &buf)) ||

          match_identifier(&input, &buf) || match_pp_number(&input, &buf) ||
          match_character_constant(&input, &buf) ||
          match_string_literal(&input, &buf) ||
          match_punctuator(&input, &buf))) {
      if (*input == '\n') {
        // new-line character is retained
        buf.kind = PP_NEWLINE;
      } else {
        // non-white-space character
        buf.kind = PP_NWSC;
        buf.nwsc = *input;
      }
      ++input;
    }
    result.pp_tokens = push_back(result.pp_tokens, result.length, &buf,
                                 sizeof(struct PPToken));
    ++result.length;
  }

  return result;
}

void pp_concat_string_literals(struct PPTokenList *pp_token_list) {
  struct PPToken *end, *current, *next;
  end = pp_token_list->pp_tokens + pp_token_list->length;

  for (size_t i = 0; i < pp_token_list->length - 1; ++i) {
    current = &pp_token_list->pp_tokens[i];
    next = &pp_token_list->pp_tokens[i + 1];

    if (current->kind == PP_STRING_LITERAL && next->kind == PP_STRING_LITERAL) {
      current->string_literal.chars =
          append_str(current->string_literal.chars, next->string_literal.chars);
    }

    erase(end, next, next + 1);
    --pp_token_list->length;
  }
}

struct TokenList pp_convert_into_token(struct PPTokenList *pp_token_list) {
  struct TokenList result = {0, NULL};
  struct PPToken *pp_token;
  struct Token buf;

  for (size_t i = 0; i < pp_token_list->length; ++i) {
    pp_token = &pp_token_list->pp_tokens[i];

    switch (pp_token->kind) {
    case PP_IDENTIFIER:
      // identifier can be either keyword, identifier or enumeration constant.

      if (str_to_keyword(pp_token->chars, &buf.keyword)) {
        buf.kind = TOKEN_KEYWORD;
      } else {
        // NOTE THAT THIS STUFF CAN BE ENUMERATION CONSTANT!
        buf.kind = TOKEN_IDENTIFIER;
        buf.chars = pp_token->chars;
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

    result.tokens =
        push_back(result.tokens, result.length++, &buf, sizeof(struct Token));
  }

  return result;
}

void skip_whitespaces(char **c) {
  bool was_there_whitespace;

  do {
    was_there_whitespace = false;

    while (is_whitespace(**c) && **c != '\n') {
      (*c) += 1;
      was_there_whitespace = true;
    }

    if (starts_with(*c, "//")) {
      // one-line comment
      (*c) += 2;
      while (**c != '\n') {
        (*c) += 1;
      }
      was_there_whitespace = true;
    } else if (starts_with(*c, "/*")) {
      // multi-line comment
      (*c) += 2;
      while (!starts_with(*c, "*/")) {
        (*c) += 1;
      }
      (*c) += 2;
      was_there_whitespace = true;
    }
  } while (was_there_whitespace);
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
  while (true) {
    if (((*c)[0] == 'e' || (*c)[0] == 'E' || (*c)[0] == 'p' ||
         (*c)[0] == 'P') &&
        ((*c)[1] == '+' || (*c)[1] == '-')) {
      (*c) += 2;
    } else if (is_digit(**c) || is_nondigit(**c) || **c == '.') {
      (*c) += 1;
    } else {
      break;
    }
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
  (*c) += 1;

  char *chars = NULL;
  size_t length = 0;
  while (**c != '\'') {
    EXIT_MESSAGE_IF(
        **c == '\n',
        "Newline character must not be appear in a character constant.");
    chars = push_back_char(chars, length++, read_char(c));
  }
  chars = push_back_char(chars, length, '\0');
  (*c) += 1;

  buf->kind = PP_CHARACTER_CONSTANT;
  buf->character_constant.prefix = prefix;
  buf->character_constant.chars = chars;
  return true;
}

bool match_string_literal(char **c, struct PPToken *buf) {
  enum EncodingPrefix encoding_prefix;

  if ((*c)[0] == 'u') {
    if ((*c)[1] == '8') {
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
  (*c) += 1;

  char *chars = NULL;
  size_t length = 0;
  while (**c != '"') {
    EXIT_MESSAGE_IF(
        **c == '\n',
        "Newline character must not be appear in a string literal.");
    chars = push_back_char(chars, length++, read_char(c));
  }
  chars = push_back_char(chars, length, '\0');
  (*c) += 1;

  buf->kind = PP_STRING_LITERAL;
  buf->string_literal.encoding_prefix = encoding_prefix;
  buf->string_literal.chars = chars;
  return true;
}

bool match_punctuator(char **c, struct PPToken *buf) {
  if (starts_with(*c, "%:%:")) {
    buf->punctuator = DIGRAPH_DOUBLE_SIGN;
    (*c) += 4;
  }

  else if (starts_with(*c, "...")) {
    buf->punctuator = ELLIPSE;
    (*c) += 3;
  } else if (starts_with(*c, "<<=")) {
    buf->punctuator = LEFT_SHIFT_ASSIGN;
    (*c) += 3;
  } else if (starts_with(*c, ">>=")) {
    buf->punctuator = RIGHT_SHIFT_ASSIGN;
    (*c) += 3;
  }

  else if (starts_with(*c, "->")) {
    buf->punctuator = ARROW;
    (*c) += 2;
  } else if (starts_with(*c, "++")) {
    buf->punctuator = INCREMENT;
    (*c) += 2;
  } else if (starts_with(*c, "--")) {
    buf->punctuator = DECREMENT;
    (*c) += 2;
  } else if (starts_with(*c, "<<")) {
    buf->punctuator = LEFT_SHIFT;
    (*c) += 2;
  } else if (starts_with(*c, ">>")) {
    buf->punctuator = RIGHT_SHIFT;
    (*c) += 2;
  } else if (starts_with(*c, "<=")) {
    buf->punctuator = LESS_EQUAL;
    (*c) += 2;
  } else if (starts_with(*c, ">=")) {
    buf->punctuator = GREATER_EQUAL;
    (*c) += 2;
  } else if (starts_with(*c, "==")) {
    buf->punctuator = EQUAL;
    (*c) += 2;
  } else if (starts_with(*c, "!=")) {
    buf->punctuator = NOT_EQUAL;
    (*c) += 2;
  } else if (starts_with(*c, "&&")) {
    buf->punctuator = LOGICAL_AND;
    (*c) += 2;
  } else if (starts_with(*c, "||")) {
    buf->punctuator = LOGICAL_OR;
    (*c) += 2;
  } else if (starts_with(*c, "*=")) {
    buf->punctuator = MULTIPLY_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "/=")) {
    buf->punctuator = DIVIDE_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "%=")) {
    buf->punctuator = REMIND_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "+=")) {
    buf->punctuator = ADD_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "-=")) {
    buf->punctuator = SUBTRACT_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "&=")) {
    buf->punctuator = AND_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "^=")) {
    buf->punctuator = EXCLUSIVE_OR_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "|=")) {
    buf->punctuator = INCLUSIVE_OR_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "##")) {
    buf->punctuator = DOUBLE_SIGN;
    (*c) += 2;
  } else if (starts_with(*c, "<:")) {
    buf->punctuator = DIGRAPH_LEFT_BRACKET;
    (*c) += 2;
  } else if (starts_with(*c, ":>")) {
    buf->punctuator = DIGRAPH_RIGHT_BRACKET;
    (*c) += 2;
  } else if (starts_with(*c, "<%")) {
    buf->punctuator = DIGRAPH_LEFT_BRACE;
    (*c) += 2;
  } else if (starts_with(*c, "%>")) {
    buf->punctuator = DIGRAPH_RIGHT_BRACE;
    (*c) += 2;
  } else if (starts_with(*c, "%:")) {
    buf->punctuator = DIGRAPH_SIGN;
    (*c) += 2;
  }

  else if (starts_with(*c, "[")) {
    buf->punctuator = LEFT_BRACKET;
    (*c) += 1;
  } else if (starts_with(*c, "]")) {
    buf->punctuator = RIGHT_BRACKET;
    (*c) += 1;
  } else if (starts_with(*c, "(")) {
    buf->punctuator = LEFT_PAREN;
    (*c) += 1;
  } else if (starts_with(*c, ")")) {
    buf->punctuator = RIGHT_PAREN;
    (*c) += 1;
  } else if (starts_with(*c, "{")) {
    buf->punctuator = LEFT_BRACE;
    (*c) += 1;
  } else if (starts_with(*c, "}")) {
    buf->punctuator = RIGHT_BRACE;
    (*c) += 1;
  } else if (starts_with(*c, ".")) {
    buf->punctuator = DOT;
    (*c) += 1;
  } else if (starts_with(*c, "&")) {
    buf->punctuator = AMPASAND;
    (*c) += 1;
  } else if (starts_with(*c, "*")) {
    buf->punctuator = ASTERISK;
    (*c) += 1;
  } else if (starts_with(*c, "+")) {
    buf->punctuator = PLUS;
    (*c) += 1;
  } else if (starts_with(*c, "-")) {
    buf->punctuator = MINUS;
    (*c) += 1;
  } else if (starts_with(*c, "~")) {
    buf->punctuator = NEGATE;
    (*c) += 1;
  } else if (starts_with(*c, "!")) {
    buf->punctuator = EXCLAMATION;
    (*c) += 1;
  } else if (starts_with(*c, "/")) {
    buf->punctuator = DIVIDE;
    (*c) += 1;
  } else if (starts_with(*c, "%")) {
    buf->punctuator = REMIND;
    (*c) += 1;
  } else if (starts_with(*c, "<")) {
    buf->punctuator = LESS_THAN;
    (*c) += 1;
  } else if (starts_with(*c, ">")) {
    buf->punctuator = GREATER_THAN;
    (*c) += 1;
  } else if (starts_with(*c, "^")) {
    buf->punctuator = EXCLUSIVE_OR;
    (*c) += 1;
  } else if (starts_with(*c, "|")) {
    buf->punctuator = INCLUSIVE_OR;
    (*c) += 1;
  } else if (starts_with(*c, "?")) {
    buf->punctuator = QUESTION;
    (*c) += 1;
  } else if (starts_with(*c, ":")) {
    buf->punctuator = COLON;
    (*c) += 1;
  } else if (starts_with(*c, ";")) {
    buf->punctuator = SEMICOLON;
    (*c) += 1;
  } else if (starts_with(*c, "=")) {
    buf->punctuator = ASSIGN;
    (*c) += 1;
  } else if (starts_with(*c, ",")) {
    buf->punctuator = COMMA;
    (*c) += 1;
  } else if (starts_with(*c, "#")) {
    buf->punctuator = SIGN;
    (*c) += 1;
  }

  else {
    return false;
  }

  buf->kind = PP_PUNCTUATOR;
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
