#include "error.h"
#include "preprocessor.h"
#include "utils.h"
#include <ctype.h>
#include <string.h>

void reconstruct_lines(char *c) {
  while (*c) {
    if (starts_with(c, "\\\n")) {
      erase_str(c, 0, 2);
    }
    ++c;
  }
}

struct PPToken **tokenize(char *input) {
  size_t pp_tokens_count = 0;
  struct PPToken **pp_tokens = NULL;
  struct PPToken *buf = checked_malloc(sizeof(struct PPToken));
  struct PPToken *last = NULL, *one_before_last = NULL;

  if (match_white_space_characters(&input, buf)) {
    PUSH_BACK(struct PPToken *, pp_tokens, pp_tokens_count, buf);
    buf = checked_malloc(sizeof(struct PPToken));
  }

  while (*input) {
    if (*input == '\n') {
      buf->kind = PP_NEWLINE;
      PUSH_BACK(struct PPToken *, pp_tokens, pp_tokens_count, buf);
      buf = checked_malloc(sizeof(struct PPToken));
      ++input;
    }

    // header-name
    PANIC_IF(
        !((is_include_directive(one_before_last, last) &&
           match_header_name(&input, buf)) ||
          // identifier
          match_identifier(&input, buf) ||
          // pp-number
          match_pp_number(&input, buf) ||
          // character-constant
          match_character_constant(&input, buf) ||
          // string-literal
          match_string_literal(&input, buf) ||
          // punctuator
          match_punctuator(&input, buf) ||
          // "each non-white-space character that cannot be one of the above"
          match_nwsc(&input, buf)));

    PUSH_BACK(struct PPToken *, pp_tokens, pp_tokens_count, buf);
    buf = checked_malloc(sizeof(struct PPToken));
    one_before_last = last;
    last = pp_tokens[pp_tokens_count - 1];

    if (match_white_space_characters(&input, buf)) {
      PUSH_BACK(struct PPToken *, pp_tokens, pp_tokens_count, buf);
      buf = checked_malloc(sizeof(struct PPToken));
    }
  }

  buf->kind = PP_NEWLINE;
  PUSH_BACK(struct PPToken *, pp_tokens, pp_tokens_count, buf);
  buf = checked_malloc(sizeof(struct PPToken));

  return pp_tokens;
}

struct Token **convert_pp_tokens_into_tokens(struct PPToken **pp_tokens) {
  size_t token_count = 0;
  struct Token **tokens = NULL;
  struct Token *buf = checked_malloc(sizeof(struct Token));

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
    case PP_WHITE_SPACE_CHARACTERS:
        // do nothing.
        ;
    }

    PUSH_BACK(struct Token *, tokens, token_count, buf);
    buf = checked_malloc(sizeof(struct PPToken));
  }

  return tokens;
}

bool match_white_space_characters(char **c, struct PPToken *buf) {
  bool result = false;
  bool was_there_whitespace = false;

  do {
    result = was_there_whitespace;
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

  if (result) {
    buf->kind = PP_WHITE_SPACE_CHARACTERS;
  }
  return result;
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

  buf->kind = PP_HEADER_NAME;
  buf->header_name.kind = kind;
  buf->header_name.chars = clone_str_range(begin, end);
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

  buf->kind = PP_IDENTIFIER;
  buf->chars = clone_str_range(begin, *c);
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

  buf->kind = PP_NUMBER;
  buf->chars = clone_str_range(begin, *c);
  return true;
}

bool match_character_constant(char **c, struct PPToken *buf) {
  enum CharacterConstantPrefix prefix;
  char *chars;

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

  chars = checked_malloc(sizeof(char));
  *chars = '\0';
  while (**c != '\'') {
    EXIT_MESSAGE_IF(
        **c == '\n',
        "Newline character must not be appear in a character constant.");
    chars = append_char(chars, **c);
    ++(*c);
  }
  (*c) += 1;

  buf->kind = PP_CHARACTER_CONSTANT;
  buf->character_constant.prefix = prefix;
  buf->character_constant.chars = chars;
  return true;
}

bool match_string_literal(char **c, struct PPToken *buf) {
  enum EncodingPrefix encoding_prefix;
  char *chars;

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

  chars = checked_malloc(sizeof(char));
  *chars = '\0';
  while (**c != '"') {
    EXIT_MESSAGE_IF(
        **c == '\n',
        "Newline character must not be appear in a string literal.");
    chars = append_char(chars, **c);
    ++(*c);
  }
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

bool match_nwsc(char **c, struct PPToken *buf) {
  buf->kind = PP_NWSC;
  buf->nwsc = **c;
  return true;
}

void convert_escape_sequences(struct PPToken **pp_tokens) {
  // FIXME only simple-escape-sequence is implemented

  for (struct PPToken *pp_token = *pp_tokens; pp_token; ++pp_token) {
    char *c;
    if (pp_token->kind == PP_CHARACTER_CONSTANT) {
      c = pp_token->character_constant.chars;
    } else if (pp_token->kind == PP_STRING_LITERAL) {
      c = pp_token->string_literal.chars;
    } else {
      break;
    }

    while (*c) {
      if (*c != '\\') {
        continue;
      }

      switch (*(c + 1)) {
      case '\'':
      case '"':
      case '?':
      case '\\':
        *c = '\x7f';
        break;

      case 'a':
        *c = '\a';
        break;
      case 'b':
        *c = '\b';
        break;
      case 'f':
        *c = '\f';
        break;
      case 'n':
        *c = '\n';
        break;
      case 'r':
        *c = '\r';
        break;
      case 't':
        *c = '\t';
        break;
      case 'v':
        *c = '\v';
        break;
      default:
        EXIT_MESSAGE("Invalid escape sequence: \\%c (\\x%x)", *(c + 1),
                     *(c + 1));
      }

      erase_str(c, 1, 2);
      ++c;
    }
  }
}

bool is_include_directive(struct PPToken *one_before_last,
                          struct PPToken *last) {
  return one_before_last && last && one_before_last->kind == PP_PUNCTUATOR &&
         one_before_last->punctuator == SIGN && last->kind == PP_IDENTIFIER &&
         str_equals(last->chars, "include");
}

bool is_nondigit(char c) { return isalpha(c) || c == '_'; }
bool is_digit(char c) { return isdigit(c); }
bool is_whitespace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f';
}

// vim: set ft=c ts=2 sw=2 et:
