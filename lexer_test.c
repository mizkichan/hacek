#include "greatest.h"
#include "test.h"

#include "lexer.c"

SUITE(lexer);

TEST test_reconstruct_lines(void) {
  char src[] = "foo\\\nbar\nhogefuga\n";

  reconstruct_lines(src);
  ASSERT_STR_EQ("foobar\nhogefuga\n", src);
  PASS();
}

TEST test_replace_comments_multiline(void) {
  char src[] = "foo/*comment\ncomment*/bar\n";
  struct PPToken **pp_tokens = tokenize(src);
  replace_comments(src, pp_tokens);

  ASSERT_STR_EQ("foo bar\n", src);
  ASSERT(pp_tokens != NULL);

  ASSERT(pp_tokens[0] != NULL);
  ASSERT_ENUM_EQ(PP_IDENTIFIER, pp_tokens[0]->kind, pp_token_kind_str);
  ASSERT_EQ(src, pp_tokens[0]->position.begin);
  ASSERT_EQ(src + 3, pp_tokens[0]->position.end);

  ASSERT_ENUM_EQ(PP_IDENTIFIER, pp_tokens[1]->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(PP_IDENTIFIER, pp_tokens[1]->kind, pp_token_kind_str);
  ASSERT_EQ(src + 4, pp_tokens[1]->position.begin);
  ASSERT_EQ(src + 7, pp_tokens[1]->position.end);

  for (struct PPToken **pp_token = pp_tokens; *pp_token; ++pp_token) {
    FREE(*pp_token);
  }
  FREE(pp_tokens);
  PASS();
}

TEST test_replace_comments_oneline(void) {
  char src[] = "foo//comment\nbar\n";
  struct PPToken **pp_tokens = tokenize(src);
  replace_comments(src, pp_tokens);

  ASSERT_STR_EQ("foo \nbar\n", src);

  for (struct PPToken **pp_token = pp_tokens; *pp_token; ++pp_token) {
    FREE(*pp_token);
  }
  FREE(pp_tokens);
  PASS();
}

TEST test_tokenize(void) {
  char *src = "  foo\t'a'+123\n";
  struct PPToken **result = tokenize(src);

  ASSERT(result != NULL);

  ASSERT(result[0] != NULL);
  ASSERT_ENUM_EQ(PP_IDENTIFIER, result[0]->kind, pp_token_kind_str);
  ASSERT_STR_RANGE_EQ("foo", result[0]->position.begin,
                      result[0]->position.end);

  ASSERT(result[1] != NULL);
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, result[1]->kind, pp_token_kind_str);
  ASSERT_STR_RANGE_EQ("a", result[1]->position.begin, result[1]->position.end);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_NONE,
                 result[1]->character_constant_prefix,
                 character_constant_prefix_str);

  ASSERT(result[2] != NULL);
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, result[2]->kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(PLUS, result[2]->punctuator, punctuator_str);

  ASSERT(result[3] != NULL);
  ASSERT_ENUM_EQ(PP_NUMBER, result[3]->kind, pp_token_kind_str);
  ASSERT_STR_RANGE_EQ("123", result[3]->position.begin,
                      result[3]->position.end);

  ASSERT(result[4] != NULL);
  ASSERT_ENUM_EQ(PP_NEWLINE, result[4]->kind, pp_token_kind_str);

  ASSERT(result[5] == NULL);

  for (struct PPToken **pp_token = result; *pp_token; ++pp_token) {
    FREE(*pp_token);
  }
  FREE(result);
  PASS();
}

TEST test_match_header_name_q(void) {
  struct PPToken buf;
  char *src = "\"hello.h\"";

  ASSERT(match_header_name(&src, &buf));
  ASSERT_ENUM_EQ(PP_HEADER_NAME, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(Q_CHAR_SEQUENCE, buf.header_name_kind, header_name_kind_str);
  ASSERT_STR_RANGE_EQ("hello.h", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_header_name_h(void) {
  struct PPToken buf;
  char *src = "<stdio.h>";

  ASSERT(match_header_name(&src, &buf));
  ASSERT_ENUM_EQ(PP_HEADER_NAME, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(H_CHAR_SEQUENCE, buf.header_name_kind, header_name_kind_str);
  ASSERT_STR_RANGE_EQ("stdio.h", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_identifier(void) {
  struct PPToken buf;
  char *src = "foobar2000";

  ASSERT(match_identifier(&src, &buf));
  ASSERT_ENUM_EQ(PP_IDENTIFIER, buf.kind, pp_token_kind_str);
  ASSERT_STR_RANGE_EQ("foobar2000", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_pp_number(void) {
  struct PPToken buf;
  char *src = ".01ae+E-p+P-.";

  ASSERT(match_pp_number(&src, &buf));
  ASSERT_ENUM_EQ(PP_NUMBER, buf.kind, pp_token_kind_str);
  ASSERT_STR_RANGE_EQ(".01ae+E-p+P-.", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant(void) {
  struct PPToken buf;
  char *src = "'foobar2000'";

  ASSERT(match_character_constant(&src, &buf));
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_NONE, buf.character_constant_prefix,
                 character_constant_prefix_str);
  ASSERT_STR_RANGE_EQ("foobar2000", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant_wchar(void) {
  struct PPToken buf;
  char *src = "L'foobar2000'";

  ASSERT(match_character_constant(&src, &buf));
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_WCHAR, buf.character_constant_prefix,
                 character_constant_prefix_str);
  ASSERT_STR_RANGE_EQ("foobar2000", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant_char16(void) {
  struct PPToken buf;
  char *src = "u'foobar2000'";

  ASSERT(match_character_constant(&src, &buf));
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_CHAR16,
                 buf.character_constant_prefix, character_constant_prefix_str);
  ASSERT_STR_RANGE_EQ("foobar2000", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant_char32(void) {
  struct PPToken buf;
  char *src = "U'foobar2000'";

  ASSERT(match_character_constant(&src, &buf));
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_CHAR32,
                 buf.character_constant_prefix, character_constant_prefix_str);
  ASSERT_STR_RANGE_EQ("foobar2000", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal(void) {
  struct PPToken buf;
  char *src = "\"foobar2000\"";

  ASSERT(match_string_literal(&src, &buf));
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(STRING_LITERAL_PREFIX_NONE, buf.string_literal_prefix,
                 string_literal_prefix_str);
  ASSERT_STR_RANGE_EQ("foobar2000", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_utf8(void) {
  struct PPToken buf;
  char *src = "u8\"foobar2000\"";

  ASSERT(match_string_literal(&src, &buf));
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(STRING_LITERAL_PREFIX_UTF8, buf.string_literal_prefix,
                 string_literal_prefix_str);
  ASSERT_STR_RANGE_EQ("foobar2000", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_char16(void) {
  struct PPToken buf;
  char *src = "u\"foobar2000\"";

  ASSERT(match_string_literal(&src, &buf));
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(STRING_LITERAL_PREFIX_CHAR16, buf.string_literal_prefix,
                 string_literal_prefix_str);
  ASSERT_STR_RANGE_EQ("foobar2000", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_char32(void) {
  struct PPToken buf;
  char *src = "U\"foobar2000\"";

  ASSERT(match_string_literal(&src, &buf));
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(STRING_LITERAL_PREFIX_CHAR32, buf.string_literal_prefix,
                 string_literal_prefix_str);
  ASSERT_STR_RANGE_EQ("foobar2000", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_wchar(void) {
  struct PPToken buf;
  char *src = "L\"foobar2000\"";

  ASSERT(match_string_literal(&src, &buf));
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(STRING_LITERAL_PREFIX_WCHAR, buf.string_literal_prefix,
                 string_literal_prefix_str);
  ASSERT_STR_RANGE_EQ("foobar2000", buf.position.begin, buf.position.end);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_punctuator(void) {
  struct PPToken buf;
  struct {
    char *src;
    enum Punctuator expected;
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
    ASSERT(match_punctuator(&cases[i].src, &buf));
    ASSERT_ENUM_EQ(PP_PUNCTUATOR, buf.kind, pp_token_kind_str);
    ASSERT_ENUM_EQ(cases[i].expected, buf.punctuator, punctuator_str);
    ASSERT_EQ(*cases[i].src, '\0');
  }

  PASS();
}

SUITE(lexer) {
  RUN_TEST(test_reconstruct_lines);
  RUN_TEST(test_tokenize);
  RUN_TEST(test_replace_comments_multiline);
  RUN_TEST(test_replace_comments_oneline);

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
