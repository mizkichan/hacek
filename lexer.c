#include "defs.h"
#include "error.h"
#include "lexer.h"
#include "utils.h"
#include <ctype.h>

static void match_white_space_characters(char **) __attribute__((nonnull));
static bool match_header_name(char **, struct PPToken *)
    __attribute__((nonnull));
static bool match_identifier(char **, struct PPToken *)
    __attribute__((nonnull));
static bool match_pp_number(char **, struct PPToken *) __attribute__((nonnull));
static bool match_character_constant(char **, struct PPToken *)
    __attribute__((nonnull));
static bool match_string_literal(char **, struct PPToken *)
    __attribute__((nonnull));
static bool match_punctuator(char **, struct PPToken *)
    __attribute__((nonnull));
static bool match_nwsc(char **, struct PPToken *) __attribute__((nonnull));
static bool is_include_directive(struct PPToken *,
                                 struct PPToken *); // params can be null
static bool is_nondigit(char) __attribute__((const));
static bool is_digit(char) __attribute__((const));
static bool is_whitespace(char) __attribute__((const));

struct Line **split_source_into_lines(const char *src) {
  const char *c = src;
  const char *begin = src;
  struct Line **lines = NULL;
  struct Line *line;
  size_t count = 0;

  while (*c) {
    if (c[0] == '\n') {
      line = MALLOC(sizeof(struct Line));
      line->linenum = count + 1;
      line->value = clone_str(begin, c);
      PUSH_BACK(struct Line *, lines, count, line);
      begin = c + 1;
    }
    ++c;
  }

  return lines;
}

void reconstruct_lines(struct Line **lines) {
  for (size_t i = 0; lines[i]; ++i) {
    size_t last = length_str(lines[i]->value) - 1;
    if (lines[i]->value[last] == '\\') {
      ERROR_IF(!lines[i + 1], "The last line should not end with backslash.");

      lines[i]->value[last] = ' ';
      lines[i]->value = append_str(lines[i]->value, lines[i + 1]->value);
      FREE(lines[i + 1]->value);
      FREE(lines[i + 1]);
      ++i;

      for (size_t j = i; lines[j]; ++j) {
        lines[j] = lines[j + 1];
      }
    }
  }
}

void replace_comments(struct Line **lines) {
  bool next_line_is_comment = false;

  // process each line
  for (size_t i = 0; lines[i]; ++i) {
    char *line = lines[i]->value;
    char *comment_begin, *comment_end;

    if (next_line_is_comment) {
      if ((comment_end = search_str(line, "*/"))) {
        next_line_is_comment = false;
        lines[i]->value = clone_str(comment_end + 2, NULL);
        FREE(line);
        line = lines[i]->value;
      } else {
        FREE(lines[i]->value);
        FREE(lines[i]);
        for (size_t j = i; lines[j]; ++j) {
          lines[j] = lines[j + 1];
        }
        continue;
      }
    }

    if ((comment_begin = search_str(line, "//"))) {
      *comment_begin = '\0';
      continue;
    }

    while (true) {
      if ((comment_begin = search_str(line, "/*"))) {
        if ((comment_end = search_str(line, "*/"))) {
          *comment_begin = ' ';
          erase_str(comment_begin + 1, comment_end + 2);
        } else {
          next_line_is_comment = true;
          *comment_begin = '\0';
          break;
        }
      } else {
        break;
      }
    }
  }
}

struct PPTokenLine **tokenize(struct Line **lines) {
  struct PPTokenLine **pp_token_lines = NULL;
  size_t pp_token_lines_count = 0;
  struct PPTokenLine *pp_token_line;

  for (size_t i = 0; lines[i]; ++i) {
    size_t pp_tokens_count = 0;
    struct PPToken **pp_tokens = NULL;
    struct PPToken *last = NULL, *one_before_last = NULL;
    struct PPToken *buf;
    char *line = lines[i]->value;

    match_white_space_characters(&line);

    while (*line) {
      buf = MALLOC(sizeof(struct PPToken));
      PANIC_IF(!(
          // header-name
          (is_include_directive(one_before_last, last) &&
           match_header_name(&line, buf)) ||
          // identifier
          match_identifier(&line, buf) ||
          // pp-number
          match_pp_number(&line, buf) ||
          // character-constant
          match_character_constant(&line, buf) ||
          // string-literal
          match_string_literal(&line, buf) ||
          // punctuator
          match_punctuator(&line, buf) ||
          // "each non-white-space character that cannot be one of the above"
          match_nwsc(&line, buf) ||
          //
          false));

      PUSH_BACK(struct PPToken *, pp_tokens, pp_tokens_count, buf);
      one_before_last = last;
      last = pp_tokens[pp_tokens_count - 1];

      match_white_space_characters(&line);
    }

    pp_token_line = MALLOC(sizeof(struct PPTokenLine));
    pp_token_line->linenum = lines[i]->linenum;
    pp_token_line->pp_tokens = pp_tokens;
    PUSH_BACK(struct PPTokenLine *, pp_token_lines, pp_token_lines_count,
              pp_token_line);
  }

  return pp_token_lines;
}

static void match_white_space_characters(char **c) {
  bool was_there_whitespace = false;

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

static bool match_header_name(char **c, struct PPToken *buf) {
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

  if ((kind == H_CHAR_SEQUENCE && **c != '>') ||
      (kind == Q_CHAR_SEQUENCE && **c != '"')) {
    return false;
  }
  end = *c;
  ++(*c);

  buf->kind = PP_HEADER_NAME;
  buf->header_name =
      MALLOC(sizeof(struct HeaderName) + (size_t)(end - begin) + 1);
  buf->header_name->kind = kind;
  copy_str(buf->header_name->value, begin, end);
  return true;
}

static bool match_identifier(char **c, struct PPToken *buf) {
  char *begin = *c, *end;

  if (!is_nondigit(**c)) {
    return false;
  }

  do {
    (*c) += 1;
    // TODO handle universal-character-name
  } while (is_nondigit(**c) || is_digit(**c));
  end = *c;

  buf->kind = PP_IDENTIFIER;
  buf->identifier =
      MALLOC(sizeof(struct Identifier) + (size_t)(end - begin) + 1);
  copy_str(buf->identifier->value, begin, end);
  return true;
}

static bool match_pp_number(char **c, struct PPToken *buf) {
  char *begin = *c, *end;

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
  end = *c;

  buf->kind = PP_NUMBER;
  buf->number = MALLOC(sizeof(struct PPNumber) + (size_t)(end - begin) + 1);
  copy_str(buf->number->value, begin, end);
  return true;
}

static bool match_character_constant(char **c, struct PPToken *buf) {
  enum CharacterConstantPrefix prefix;
  char *begin, *end;

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
  begin = *c;

  while (**c != '\'') {
    ERROR_IF(**c == '\n',
             "Newline character must not be appear in a character constant.");
    ++(*c);
  }
  end = *c;
  ++(*c);

  buf->kind = PP_CHARACTER_CONSTANT;
  buf->character_constant =
      MALLOC(sizeof(struct CharacterConstant) + (size_t)(end - begin) + 1);
  buf->character_constant->prefix = prefix;
  copy_str(buf->character_constant->value, begin, end);
  return true;
}

static bool match_string_literal(char **c, struct PPToken *buf) {
  enum StringLiteralPrefix prefix;
  char *begin, *end;

  if ((*c)[0] == 'u') {
    if ((*c)[1] == '8') {
      prefix = STRING_LITERAL_PREFIX_UTF8;
      (*c) += 2;
    } else {
      prefix = STRING_LITERAL_PREFIX_CHAR16;
      (*c) += 1;
    }
  } else if ((*c)[0] == 'U') {
    prefix = STRING_LITERAL_PREFIX_CHAR32;
    (*c) += 1;
  } else if ((*c)[0] == 'L') {
    prefix = STRING_LITERAL_PREFIX_WCHAR;
    (*c) += 1;
  } else {
    prefix = STRING_LITERAL_PREFIX_NONE;
  }

  if (**c != '"') {
    return false;
  }
  ++(*c);

  begin = *c;
  while (**c != '"') {
    ++(*c);
  }
  end = *c;
  ++(*c);

  buf->kind = PP_STRING_LITERAL;
  buf->string_literal =
      MALLOC(sizeof(struct StringLiteral) + (size_t)(end - begin) + 1);
  buf->string_literal->prefix = prefix;
  copy_str(buf->string_literal->value, begin, end);
  return true;
}

static bool match_punctuator(char **c, struct PPToken *buf) {
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

static bool match_nwsc(char **c, struct PPToken *buf) {
  buf->kind = PP_NWSC;
  buf->nwsc = **c;
  return true;
}

static bool is_include_directive(struct PPToken *one_before_last,
                                 struct PPToken *last) {
  return one_before_last && last && one_before_last->kind == PP_PUNCTUATOR &&
         one_before_last->punctuator == SIGN && last->kind == PP_IDENTIFIER &&
         str_equals("include", last->identifier->value, NULL);
}

static bool is_nondigit(char c) { return isalpha(c) || c == '_'; }

static bool is_digit(char c) { return isdigit(c); }

static bool is_whitespace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f';
}

// vim: set ft=c ts=2 sw=2 et:
