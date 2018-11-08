#include "greatest.h"
#include "lexer.h"
#include "token.h"

#include "preprocessor.c"

SUITE(preprocessor);

TEST test_unescape(void) {
  char src[] = "hoge\\nfuga\\tpiyo";
  unescape(src);
  ASSERT_STR_EQ("hoge\nfuga\tpiyo", src);
  PASS();
}

TEST test_concatenate_pp_token_lines(void) {
  char *src = "hoge\nfuga\n";
  struct Line **lines = split_source_into_lines(src);
  struct PPTokenLine **pp_token_lines = tokenize(lines);
  struct PPToken **pp_tokens = concatenate_pp_token_lines(pp_token_lines);

  ASSERT(pp_tokens != NULL);

  ASSERT(pp_tokens[0] != NULL);
  ASSERT_ENUM_EQ(PP_IDENTIFIER, pp_tokens[0]->kind, pp_token_kind_str);
  ASSERT_STR_EQ("hoge", pp_tokens[0]->identifier->value);

  ASSERT(pp_tokens[1] != NULL);
  ASSERT_ENUM_EQ(PP_IDENTIFIER, pp_tokens[1]->kind, pp_token_kind_str);
  ASSERT_STR_EQ("fuga", pp_tokens[1]->identifier->value);

  ASSERT(pp_tokens[2] == NULL);

  PASS();
}

TEST test_concatenate_adjacent_string_literals(void) {
  char *src = "\"foo\" \"bar\"\"2000\"\n";
  struct Line **lines = split_source_into_lines(src);
  struct PPTokenLine **pp_token_lines = tokenize(lines);
  struct PPToken **pp_tokens = concatenate_pp_token_lines(pp_token_lines);
  concatenate_adjacent_string_literals(pp_tokens);

  ASSERT(pp_tokens != NULL);
  ASSERT(pp_tokens[0] != NULL);
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, pp_tokens[0]->kind, pp_token_kind_str);
  ASSERT_STR_EQ("foobar2000", pp_tokens[0]->string_literal->value);
  ASSERT(pp_tokens[1] == NULL);

  PASS();
}

TEST test_convert_pp_identifier(void) {}

TEST test_convert_pp_number(void) {
  char *str = "42";
  struct PPNumber *pp_number =
      new_pp_number("foobar2000.c", 0, 0, str, str + 2);
  struct Token *result = convert_pp_number(pp_number);

  ASSERT(result != NULL);
  ASSERT_ENUM_EQ(result->kind, TOKEN_CONSTANT, token_kind_str);

  ASSERT(result->constant != NULL);
  ASSERT_ENUM_EQ(result->constant->kind, INTEGER_CONSTANT, constant_kind_str);

  ASSERT(result->constant->integer_constant != NULL);
  ASSERT_ENUM_EQ(result->constant->integer_constant->type, INTEGER_CONSTANT_INT,
                 integer_constant_type_str);

  ASSERT_EQ(result->constant->integer_constant->value_int, 42);
  PASS();
}

TEST test_match_decimal_constant(void) {
  const char *str;
  uintmax_t result;

  str = "42";
  ASSERT(match_decimal_constant(&str, &result));
  ASSERT_EQ('\0', *str);
  ASSERT_EQ(42, result);

  str = "4200000000";
  ASSERT(match_decimal_constant(&str, &result));
  ASSERT_EQ('\0', *str);
  ASSERT_EQ(4200000000, result);
  PASS();
}

SUITE(preprocessor) {
  RUN_TEST(test_unescape);
  RUN_TEST(test_concatenate_pp_token_lines);
  RUN_TEST(test_concatenate_adjacent_string_literals);
  RUN_TEST(test_match_decimal_constant);
  RUN_TEST(test_convert_pp_identifier);
  RUN_TEST(test_convert_pp_number);
}

// vim: set ft=c ts=2 sw=2 et:
