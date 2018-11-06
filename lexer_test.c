#include "greatest.h"
#include "token.h"

#include "lexer.c"

SUITE(lexer);

TEST test_split_source_into_lines(void) {
  struct Line **lines = split_source_into_lines("foo\nbar\n");

  ASSERT(lines != NULL);

  ASSERT(lines[0] != NULL);
  ASSERT_EQ(1, lines[0]->linenum);
  ASSERT_STR_EQ("foo", lines[0]->value);

  ASSERT(lines[1] != NULL);
  ASSERT_EQ(2, lines[1]->linenum);
  ASSERT_STR_EQ("bar", lines[1]->value);

  ASSERT(lines[2] == NULL);

  PASS();
}

TEST test_reconstruct_lines(void) {
  struct Line **lines = split_source_into_lines("foo\\\nbar\nbaz\n");

  reconstruct_lines(lines);
  ASSERT(lines != NULL);

  ASSERT(lines[0] != NULL);
  ASSERT_EQ(1, lines[0]->linenum);
  ASSERT_STR_EQ("foo bar", lines[0]->value);

  ASSERT(lines[1] != NULL);
  ASSERT_EQ(3, lines[1]->linenum);
  ASSERT_STR_EQ("baz", lines[1]->value);

  ASSERT(lines[2] == NULL);

  PASS();
}

TEST test_replace_comments_multiline(void) {
  struct Line **lines = split_source_into_lines("foo/*comment\ncomment*/bar\n");
  ASSERT_MEM_EQ("comment*/bar", lines[1]->value, 13);
  replace_comments(lines);

  ASSERT(lines != NULL);

  ASSERT(lines[0] != NULL);
  ASSERT_EQ(1, lines[0]->linenum);
  ASSERT_STR_EQ("foo", lines[0]->value);

  ASSERT(lines[1] != NULL);
  ASSERT_EQ(2, lines[1]->linenum);
  ASSERT_MEM_EQ("bar", lines[1]->value, 4);

  ASSERT(lines[2] == NULL);

  PASS();
}

TEST test_replace_comments_oneline(void) {
  struct Line **lines = split_source_into_lines("foo//comment\nbar\n");
  ASSERT_MEM_EQ("bar", lines[1]->value, 4);
  replace_comments(lines);

  ASSERT(lines[0] != NULL);
  ASSERT_EQ(1, lines[0]->linenum);
  ASSERT_STR_EQ("foo", lines[0]->value);

  ASSERT(lines[1] != NULL);
  ASSERT_EQ(2, lines[1]->linenum);
  ASSERT_MEM_EQ("bar", lines[1]->value, 4);

  ASSERT(lines[2] == NULL);

  PASS();
}

TEST test_replace_comments_oneline2(void) {
  struct Line **lines = split_source_into_lines("foo/*comment*/bar\n");
  replace_comments(lines);

  ASSERT(lines[0] != NULL);
  ASSERT_EQ(1, lines[0]->linenum);
  ASSERT_STR_EQ("foo bar", lines[0]->value);

  ASSERT(lines[1] == NULL);

  PASS();
}

TEST test_tokenize(void) {
  struct Line **lines = split_source_into_lines("  foo\t'a'+123\n");
  struct PPTokenLine **pp_token_lines = tokenize(lines);
  struct PPToken **pp_tokens;

  ASSERT(pp_token_lines != NULL);
  ASSERT(pp_token_lines[0] != NULL);
  pp_tokens = pp_token_lines[0]->pp_tokens;
  ASSERT(pp_tokens != NULL);

  ASSERT(pp_tokens[0] != NULL);
  ASSERT_ENUM_EQ(PP_IDENTIFIER, pp_tokens[0]->kind, pp_token_kind_str);
  ASSERT_STR_EQ("foo", pp_tokens[0]->identifier->value);

  ASSERT(pp_tokens[1] != NULL);
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, pp_tokens[1]->kind, pp_token_kind_str);
  ASSERT_STR_EQ("a", pp_tokens[1]->character_constant->value);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_NONE,
                 pp_tokens[1]->character_constant->prefix,
                 character_constant_prefix_str);

  ASSERT(pp_tokens[2] != NULL);
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, pp_tokens[2]->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(PLUS, pp_tokens[2]->punctuator->kind, punctuator_kind_str);

  ASSERT(pp_tokens[3] != NULL);
  ASSERT_ENUM_EQ(PP_NUMBER, pp_tokens[3]->kind, pp_token_kind_str);
  ASSERT_STR_EQ("123", pp_tokens[3]->number->value);

  ASSERT(pp_tokens[4] == NULL);

  PASS();
}

TEST test_match_header_name_q(void) {
  struct PPToken *buf;
  char *src = "\"hello.h\"";

  buf = match_header_name(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_HEADER_NAME, buf->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(Q_CHAR_SEQUENCE, buf->header_name->kind, header_name_kind_str);
  ASSERT_STR_EQ("hello.h", buf->header_name->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_header_name_h(void) {
  struct PPToken *buf;
  char *src = "<stdio.h>";

  buf = match_header_name(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_HEADER_NAME, buf->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(H_CHAR_SEQUENCE, buf->kind, header_name_kind_str);
  ASSERT_STR_EQ("stdio.h", buf->header_name->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_identifier(void) {
  struct PPToken *buf;
  char *src = "foobar2000";

  buf = match_identifier(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_IDENTIFIER, buf->kind, pp_token_kind_str);
  ASSERT_STR_EQ("foobar2000", buf->identifier->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_pp_number(void) {
  struct PPToken *buf;
  char *src = ".01ae+E-p+P-.";

  buf = match_pp_number(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_NUMBER, buf->kind, pp_token_kind_str);
  ASSERT_STR_EQ(".01ae+E-p+P-.", buf->number->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant(void) {
  struct PPToken *buf;
  char *src = "'foobar2000'";

  buf = match_character_constant(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_NONE,
                 buf->character_constant->prefix,
                 character_constant_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf->character_constant->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant_wchar(void) {
  struct PPToken *buf;
  char *src = "L'foobar2000'";

  buf = match_character_constant(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_WCHAR,
                 buf->character_constant->prefix,
                 character_constant_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf->character_constant->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant_char16(void) {
  struct PPToken *buf;
  char *src = "u'foobar2000'";

  buf = match_character_constant(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_CHAR16,
                 buf->character_constant->prefix,
                 character_constant_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf->character_constant->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant_char32(void) {
  struct PPToken *buf;
  char *src = "U'foobar2000'";

  buf = match_character_constant(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_CHAR32,
                 buf->character_constant->prefix,
                 character_constant_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf->character_constant->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal(void) {
  struct PPToken *buf;
  char *src = "\"foobar2000\"";

  buf = match_string_literal(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(STRING_LITERAL_PREFIX_NONE, buf->string_literal->prefix,
                 string_literal_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf->string_literal->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_utf8(void) {
  struct PPToken *buf;
  char *src = "u8\"foobar2000\"";

  buf = match_string_literal(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(STRING_LITERAL_PREFIX_UTF8, buf->string_literal->prefix,
                 string_literal_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf->string_literal->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_char16(void) {
  struct PPToken *buf;
  char *src = "u\"foobar2000\"";

  buf = match_string_literal(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(STRING_LITERAL_PREFIX_CHAR16, buf->string_literal->prefix,
                 string_literal_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf->string_literal->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_char32(void) {
  struct PPToken *buf;
  char *src = "U\"foobar2000\"";

  buf = match_string_literal(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(STRING_LITERAL_PREFIX_CHAR32, buf->string_literal->prefix,
                 string_literal_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf->string_literal->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_wchar(void) {
  struct PPToken *buf;
  char *src = "L\"foobar2000\"";

  buf = match_string_literal(&src);
  ASSERT(buf != NULL);
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(STRING_LITERAL_PREFIX_WCHAR, buf->string_literal->prefix,
                 string_literal_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf->string_literal->value);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_punctuator(void) {
  struct PPToken *buf;
  struct {
    char *src;
    enum PunctuatorKind expected;
  } cases[54] = {
      {"[", LEFT_BRACKET},
      {"]", RIGHT_BRACKET},
      {"(", LEFT_PAREN},
      {")", RIGHT_PAREN},
      {"{", LEFT_BRACE},
      {"}", RIGHT_BRACE},
      {".", DOT},
      {"->", ARROW},
      {"++", INCREMENT},
      {"--", DECREMENT},
      {"&", AMPASAND},
      {"*", ASTERISK},
      {"+", PLUS},
      {"-", MINUS},
      {"~", NEGATE},
      {"!", EXCLAMATION},
      {"/", DIVIDE},
      {"%", REMIND},
      {"<<", LEFT_SHIFT},
      {">>", RIGHT_SHIFT},
      {"<", LESS_THAN},
      {">", GREATER_THAN},
      {"<=", LESS_EQUAL},
      {">=", GREATER_EQUAL},
      {"==", EQUAL},
      {"!=", NOT_EQUAL},
      {"^", EXCLUSIVE_OR},
      {"|", INCLUSIVE_OR},
      {"&&", LOGICAL_AND},
      {"||", LOGICAL_OR},
      {"?", QUESTION},
      {":", COLON},
      {";", SEMICOLON},
      {"...", ELLIPSE},
      {"=", ASSIGN},
      {"*=", MULTIPLY_ASSIGN},
      {"/=", DIVIDE_ASSIGN},
      {"%=", REMIND_ASSIGN},
      {"+=", ADD_ASSIGN},
      {"-=", SUBTRACT_ASSIGN},
      {"<<=", LEFT_SHIFT_ASSIGN},
      {">>=", RIGHT_SHIFT_ASSIGN},
      {"&=", AND_ASSIGN},
      {"^=", EXCLUSIVE_OR_ASSIGN},
      {"|=", INCLUSIVE_OR_ASSIGN},
      {",", COMMA},
      {"#", SIGN},
      {"##", DOUBLE_SIGN},
      {"<:", DIGRAPH_LEFT_BRACKET},
      {":>", DIGRAPH_RIGHT_BRACKET},
      {"<%", DIGRAPH_LEFT_BRACE},
      {"%>", DIGRAPH_RIGHT_BRACE},
      {"%:", DIGRAPH_SIGN},
      {"%:%:", DIGRAPH_DOUBLE_SIGN},
  };

  for (size_t i = 0; i < sizeof(cases) / sizeof(cases[i]); ++i) {
    buf = match_punctuator(&cases[i].src);
    ASSERT(buf != NULL);
    ASSERT_ENUM_EQ(PP_PUNCTUATOR, buf->kind, pp_token_kind_str);
    ASSERT_ENUM_EQ(cases[i].expected, buf->punctuator->kind,
                   punctuator_kind_str);
    ASSERT_EQ(*cases[i].src, '\0');
  }

  PASS();
}

SUITE(lexer) {
  RUN_TEST(test_split_source_into_lines);
  RUN_TEST(test_reconstruct_lines);
  RUN_TEST(test_tokenize);
  RUN_TEST(test_replace_comments_multiline);
  RUN_TEST(test_replace_comments_oneline);
  RUN_TEST(test_replace_comments_oneline2);

  RUN_TEST(test_match_header_name_q);
  RUN_TEST(test_match_header_name_h);
  RUN_TEST(test_match_identifier);
  RUN_TEST(test_match_pp_number);
  RUN_TEST(test_match_character_constant);
  RUN_TEST(test_match_character_constant_char16);
  RUN_TEST(test_match_character_constant_char32);
  RUN_TEST(test_match_character_constant_wchar);
  RUN_TEST(test_match_string_literal);
  RUN_TEST(test_match_string_literal_utf8);
  RUN_TEST(test_match_string_literal_char16);
  RUN_TEST(test_match_string_literal_char32);
  RUN_TEST(test_match_string_literal_wchar);
  RUN_TEST(test_match_punctuator);
}

// vim: set ft=c ts=2 sw=2 et:
