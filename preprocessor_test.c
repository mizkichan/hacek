#include "greatest/greatest.h"
#include "preprocessor.h"
#include "token.h"

SUITE(preprocessor);

TEST test_match_header_name_q(void) {
  struct PPToken buf;
  char *src;

  src = "\"hello.h\"";
  ASSERT(match_header_name(&src, &buf));
  ASSERT_ENUM_EQ(PP_HEADER_NAME, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(Q_CHAR_SEQUENCE, buf.header_name.kind, header_name_kind_str);
  ASSERT_STR_EQ("hello.h", buf.header_name.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_header_name_h(void) {
  struct PPToken buf;
  char *src;

  src = "<stdio.h>";
  ASSERT(match_header_name(&src, &buf));
  ASSERT_ENUM_EQ(PP_HEADER_NAME, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(H_CHAR_SEQUENCE, buf.header_name.kind, header_name_kind_str);
  ASSERT_STR_EQ("stdio.h", buf.header_name.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_identifier(void) {
  struct PPToken buf;
  char *src;

  src = "foobar2000";
  ASSERT(match_identifier(&src, &buf));
  ASSERT_ENUM_EQ(PP_IDENTIFIER, buf.kind, pp_token_kind_str);
  ASSERT_STR_EQ("foobar2000", buf.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_pp_number(void) {
  struct PPToken buf;
  char *src;

  src = ".01ae+E-p+P-.";
  ASSERT(match_pp_number(&src, &buf));
  ASSERT_ENUM_EQ(PP_NUMBER, buf.kind, pp_token_kind_str);
  ASSERT_STR_EQ(".01ae+E-p+P-.", buf.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant(void) {
  struct PPToken buf;
  char *src;

  src = "'foobar2000'";
  ASSERT(match_character_constant(&src, &buf));
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_NONE, buf.character_constant.prefix,
                 character_constant_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf.character_constant.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant_wchar(void) {
  struct PPToken buf;
  char *src;

  src = "L'foobar2000'";
  ASSERT(match_character_constant(&src, &buf));
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_WCHAR, buf.character_constant.prefix,
                 character_constant_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf.character_constant.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant_char16(void) {
  struct PPToken buf;
  char *src;

  src = "u'foobar2000'";
  ASSERT(match_character_constant(&src, &buf));
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_CHAR16,
                 buf.character_constant.prefix, character_constant_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf.character_constant.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_character_constant_char32(void) {
  struct PPToken buf;
  char *src;

  src = "U'foobar2000'";
  ASSERT(match_character_constant(&src, &buf));
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_CHAR32,
                 buf.character_constant.prefix, character_constant_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf.character_constant.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal(void) {
  struct PPToken buf;
  char *src;

  src = "\"foobar2000\"";
  ASSERT(match_string_literal(&src, &buf));
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(ENCODING_PREFIX_NONE, buf.string_literal.encoding_prefix,
                 encoding_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf.string_literal.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_utf8(void) {
  struct PPToken buf;
  char *src;

  src = "u8\"foobar2000\"";
  ASSERT(match_string_literal(&src, &buf));
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(ENCODING_PREFIX_UTF8, buf.string_literal.encoding_prefix,
                 encoding_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf.string_literal.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_char16(void) {
  struct PPToken buf;
  char *src;

  src = "u\"foobar2000\"";
  ASSERT(match_string_literal(&src, &buf));
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(ENCODING_PREFIX_CHAR16, buf.string_literal.encoding_prefix,
                 encoding_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf.string_literal.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_char32(void) {
  struct PPToken buf;
  char *src;

  src = "U\"foobar2000\"";
  ASSERT(match_string_literal(&src, &buf));
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(ENCODING_PREFIX_CHAR32, buf.string_literal.encoding_prefix,
                 encoding_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf.string_literal.chars);
  ASSERT_EQ(*src, '\0');

  PASS();
}

TEST test_match_string_literal_wchar(void) {
  struct PPToken buf;
  char *src;

  src = "L\"foobar2000\"";
  ASSERT(match_string_literal(&src, &buf));
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, buf.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(ENCODING_PREFIX_WCHAR, buf.string_literal.encoding_prefix,
                 encoding_prefix_str);
  ASSERT_STR_EQ("foobar2000", buf.string_literal.chars);
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

TEST test_pp_concat_string_literals(void) {
  char *src = "\"foo\" \"bar\"";
  struct PPTokenList token_list = pp_tokenize(src);
  pp_concat_string_literals(&token_list);

  ASSERT_EQ(1, token_list.length);
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, token_list.pp_tokens[0].kind,
                 pp_token_kind_str);
  ASSERT_STR_EQ("foobar", token_list.pp_tokens[0].string_literal.chars);

  PASS();
}

SUITE(preprocessor) {
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
  RUN_TEST(test_pp_concat_string_literals);
}

// vim: set ft=c ts=2 sw=2 et:
