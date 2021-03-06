#include "defs.h"
#include "error.h"
#include "lexer.h"
#include "token.h"
#include "utils.h"
#include <ctype.h>

static void match_white_space_characters(char **) __attribute__((nonnull));
static struct PPToken *match_header_name(char **) __attribute__((nonnull));
static struct PPToken *match_identifier(char **) __attribute__((nonnull));
static struct PPToken *match_pp_number(char **) __attribute__((nonnull));
static struct PPToken *match_character_constant(char **)
    __attribute__((nonnull));
static struct PPToken *match_string_literal(char **) __attribute__((nonnull));
static struct PPToken *match_punctuator(char **) __attribute__((nonnull));
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
    struct PPToken *token;
    char *line = lines[i]->value;

    match_white_space_characters(&line);

    while (*line) {
      PANIC_IF(!(
          // header-name
          (is_include_directive(one_before_last, last) &&
           (token = match_header_name(&line))) ||
          // identifier
          (token = match_identifier(&line)) ||
          // pp-number
          (token = match_pp_number(&line)) ||
          // character-constant
          (token = match_character_constant(&line)) ||
          // string-literal
          (token = match_string_literal(&line)) ||
          // punctuator
          (token = match_punctuator(&line)) ||
          //
          false));

      PUSH_BACK(struct PPToken *, pp_tokens, pp_tokens_count, token);
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

static struct PPToken *match_header_name(char **c) {
  char *begin, *end;
  enum HeaderNameKind kind;

  if (**c == '<') {
    kind = H_CHAR_SEQUENCE;
  } else if (**c == '"') {
    kind = Q_CHAR_SEQUENCE;
  } else {
    return NULL;
  }

  (*c) += 1;
  begin = *c;

  while (**c != '\n' && ((kind == H_CHAR_SEQUENCE && **c != '>') ||
                         (kind == Q_CHAR_SEQUENCE && **c != '"'))) {
    (*c) += 1;
  }

  if ((kind == H_CHAR_SEQUENCE && **c != '>') ||
      (kind == Q_CHAR_SEQUENCE && **c != '"')) {
    return NULL;
  }
  end = *c;
  ++(*c);

  return new_pp_token(PP_HEADER_NAME,
                      new_header_name("foobar2000", 0, 0, kind, begin, end));
}

static struct PPToken *match_identifier(char **c) {
  char *begin = *c, *end;

  if (!is_nondigit(**c)) {
    return NULL;
  }

  do {
    (*c) += 1;
    // TODO handle universal-character-name
  } while (is_nondigit(**c) || is_digit(**c));
  end = *c;

  return new_pp_token(PP_IDENTIFIER,
                      new_identifier("foobar2000", 0, 0, begin, end));
}

static struct PPToken *match_pp_number(char **c) {
  char *begin = *c, *end;

  if (is_digit((*c)[0])) {
    (*c) += 1;
  } else if (((*c)[0] == '.') && is_digit((*c)[1])) {
    (*c) += 2;
  } else {
    return NULL;
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

  return new_pp_token(PP_NUMBER, new_pp_number("foobar2000", 0, 0, begin, end));
}

static struct PPToken *match_character_constant(char **c) {
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
    return NULL;
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

  return new_pp_token(
      PP_CHARACTER_CONSTANT,
      new_character_constant("foobar2000", 0, 0, prefix, begin, end));
}

static struct PPToken *match_string_literal(char **c) {
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
    return NULL;
  }
  ++(*c);

  begin = *c;
  while (**c != '"') {
    ++(*c);
  }
  end = *c;
  ++(*c);

  return new_pp_token(
      PP_STRING_LITERAL,
      new_string_literal("foobar2000", 0, 0, prefix, begin, end));
}

static struct PPToken *match_punctuator(char **c) {
  enum PunctuatorKind kind;

  if (starts_with(*c, "%:%:")) {
    kind = DIGRAPH_DOUBLE_SIGN;
    (*c) += 4;
  }

  else if (starts_with(*c, "...")) {
    kind = ELLIPSE;
    (*c) += 3;
  } else if (starts_with(*c, "<<=")) {
    kind = LEFT_SHIFT_ASSIGN;
    (*c) += 3;
  } else if (starts_with(*c, ">>=")) {
    kind = RIGHT_SHIFT_ASSIGN;
    (*c) += 3;
  }

  else if (starts_with(*c, "->")) {
    kind = ARROW;
    (*c) += 2;
  } else if (starts_with(*c, "++")) {
    kind = INCREMENT;
    (*c) += 2;
  } else if (starts_with(*c, "--")) {
    kind = DECREMENT;
    (*c) += 2;
  } else if (starts_with(*c, "<<")) {
    kind = LEFT_SHIFT;
    (*c) += 2;
  } else if (starts_with(*c, ">>")) {
    kind = RIGHT_SHIFT;
    (*c) += 2;
  } else if (starts_with(*c, "<=")) {
    kind = LESS_EQUAL;
    (*c) += 2;
  } else if (starts_with(*c, ">=")) {
    kind = GREATER_EQUAL;
    (*c) += 2;
  } else if (starts_with(*c, "==")) {
    kind = EQUAL;
    (*c) += 2;
  } else if (starts_with(*c, "!=")) {
    kind = NOT_EQUAL;
    (*c) += 2;
  } else if (starts_with(*c, "&&")) {
    kind = LOGICAL_AND;
    (*c) += 2;
  } else if (starts_with(*c, "||")) {
    kind = LOGICAL_OR;
    (*c) += 2;
  } else if (starts_with(*c, "*=")) {
    kind = MULTIPLY_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "/=")) {
    kind = DIVIDE_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "%=")) {
    kind = REMIND_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "+=")) {
    kind = ADD_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "-=")) {
    kind = SUBTRACT_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "&=")) {
    kind = AND_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "^=")) {
    kind = EXCLUSIVE_OR_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "|=")) {
    kind = INCLUSIVE_OR_ASSIGN;
    (*c) += 2;
  } else if (starts_with(*c, "##")) {
    kind = DOUBLE_SIGN;
    (*c) += 2;
  } else if (starts_with(*c, "<:")) {
    kind = DIGRAPH_LEFT_BRACKET;
    (*c) += 2;
  } else if (starts_with(*c, ":>")) {
    kind = DIGRAPH_RIGHT_BRACKET;
    (*c) += 2;
  } else if (starts_with(*c, "<%")) {
    kind = DIGRAPH_LEFT_BRACE;
    (*c) += 2;
  } else if (starts_with(*c, "%>")) {
    kind = DIGRAPH_RIGHT_BRACE;
    (*c) += 2;
  } else if (starts_with(*c, "%:")) {
    kind = DIGRAPH_SIGN;
    (*c) += 2;
  }

  else if (starts_with(*c, "[")) {
    kind = LEFT_BRACKET;
    (*c) += 1;
  } else if (starts_with(*c, "]")) {
    kind = RIGHT_BRACKET;
    (*c) += 1;
  } else if (starts_with(*c, "(")) {
    kind = LEFT_PAREN;
    (*c) += 1;
  } else if (starts_with(*c, ")")) {
    kind = RIGHT_PAREN;
    (*c) += 1;
  } else if (starts_with(*c, "{")) {
    kind = LEFT_BRACE;
    (*c) += 1;
  } else if (starts_with(*c, "}")) {
    kind = RIGHT_BRACE;
    (*c) += 1;
  } else if (starts_with(*c, ".")) {
    kind = DOT;
    (*c) += 1;
  } else if (starts_with(*c, "&")) {
    kind = AMPASAND;
    (*c) += 1;
  } else if (starts_with(*c, "*")) {
    kind = ASTERISK;
    (*c) += 1;
  } else if (starts_with(*c, "+")) {
    kind = PLUS;
    (*c) += 1;
  } else if (starts_with(*c, "-")) {
    kind = MINUS;
    (*c) += 1;
  } else if (starts_with(*c, "~")) {
    kind = NEGATE;
    (*c) += 1;
  } else if (starts_with(*c, "!")) {
    kind = EXCLAMATION;
    (*c) += 1;
  } else if (starts_with(*c, "/")) {
    kind = DIVIDE;
    (*c) += 1;
  } else if (starts_with(*c, "%")) {
    kind = REMIND;
    (*c) += 1;
  } else if (starts_with(*c, "<")) {
    kind = LESS_THAN;
    (*c) += 1;
  } else if (starts_with(*c, ">")) {
    kind = GREATER_THAN;
    (*c) += 1;
  } else if (starts_with(*c, "^")) {
    kind = EXCLUSIVE_OR;
    (*c) += 1;
  } else if (starts_with(*c, "|")) {
    kind = INCLUSIVE_OR;
    (*c) += 1;
  } else if (starts_with(*c, "?")) {
    kind = QUESTION;
    (*c) += 1;
  } else if (starts_with(*c, ":")) {
    kind = COLON;
    (*c) += 1;
  } else if (starts_with(*c, ";")) {
    kind = SEMICOLON;
    (*c) += 1;
  } else if (starts_with(*c, "=")) {
    kind = ASSIGN;
    (*c) += 1;
  } else if (starts_with(*c, ",")) {
    kind = COMMA;
    (*c) += 1;
  } else if (starts_with(*c, "#")) {
    kind = SIGN;
    (*c) += 1;
  }

  else {
    return NULL;
  }

  return new_pp_token(PP_PUNCTUATOR, new_punctuator("foobar2000", 0, 0, kind));
}

static bool is_include_directive(struct PPToken *one_before_last,
                                 struct PPToken *last) {
  return one_before_last && last && one_before_last->kind == PP_PUNCTUATOR &&
         one_before_last->punctuator->kind == SIGN &&
         last->kind == PP_IDENTIFIER &&
         str_equals("include", last->identifier->value, NULL);
}

static bool is_nondigit(char c) { return isalpha(c) || c == '_'; }

static bool is_digit(char c) { return isdigit(c); }

static bool is_whitespace(char c) {
  return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f';
}

// vim: set ft=c ts=2 sw=2 et:
