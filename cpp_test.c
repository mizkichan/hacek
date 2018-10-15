#include "cpp.h"
#include "greatest/greatest.h"
#include "tokens.h"

SUITE(cpp);

TEST test_cpp_tokenize(void) {
  char *src = "#include <stdio.h>\n"
              "#if 0\n"
              "#include \"hello.h\"\n"
              "#endif\n"
              "\n"
              "int main(void) {\n"
              "  printf(\"Hello, world!\");\n"
              "  putchar('\\n');\n"
              "}\n"
              "`@\n";
  struct PPTokenList results = cpp_tokenize(src);
  struct PPToken actual;
  size_t i = 0;

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SIGN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("include", actual.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_HEADER_NAME, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(H_CHAR_SEQUENCE, actual.header_name.kind,
                 header_name_kind_str);
  ASSERT_STR_EQ("stdio.h", actual.header_name.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NEWLINE, actual.kind, pp_token_kind_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SIGN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("if", actual.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NUMBER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("0", actual.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NEWLINE, actual.kind, pp_token_kind_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SIGN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("include", actual.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_HEADER_NAME, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(Q_CHAR_SEQUENCE, actual.header_name.kind,
                 header_name_kind_str);
  ASSERT_STR_EQ("hello.h", actual.header_name.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NEWLINE, actual.kind, pp_token_kind_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SIGN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("endif", actual.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NEWLINE, actual.kind, pp_token_kind_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NEWLINE, actual.kind, pp_token_kind_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("int", actual.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("main", actual.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(LEFT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("void", actual.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(RIGHT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(LEFT_BRACE, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NEWLINE, actual.kind, pp_token_kind_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("printf", actual.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(LEFT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(ENCODING_PREFIX_NONE, actual.string_literal.encoding_prefix,
                 encoding_prefix_str);
  ASSERT_STR_EQ("Hello, world!", actual.string_literal.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(RIGHT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SEMICOLON, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NEWLINE, actual.kind, pp_token_kind_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("putchar", actual.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(LEFT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(CHARACTER_CONSTANT_PREFIX_NONE,
                 actual.character_constant.prefix,
                 character_constant_prefix_str);
  ASSERT_STR_EQ("\n", actual.character_constant.chars);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(RIGHT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SEMICOLON, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NEWLINE, actual.kind, pp_token_kind_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(RIGHT_BRACE, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NEWLINE, actual.kind, pp_token_kind_str);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NWSC, actual.kind, pp_token_kind_str);
  ASSERT_EQ('`', actual.nwsc);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NWSC, actual.kind, pp_token_kind_str);
  ASSERT_EQ('@', actual.nwsc);

  actual = results.pp_tokens[i++];
  ASSERT_ENUM_EQ(PP_NEWLINE, actual.kind, pp_token_kind_str);

  ASSERT_EQ(i++, results.length);

  PASS();
}

TEST test_cpp_concat_string_literals(void){
  char *src = "\"foo\" \"bar\"";
  struct PPTokenList token_list = cpp_tokenize(src);
  cpp_concat_string_literals(&token_list);

  ASSERT_EQ(1, token_list.length);
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, token_list.pp_tokens[0].kind, pp_token_kind_str);
  ASSERT_STR_EQ("foobar", token_list.pp_tokens[0].string_literal.chars);

  PASS();
}

SUITE(cpp) {
  RUN_TEST(test_cpp_tokenize);
  RUN_TEST(test_cpp_concat_string_literals);
}

// vim: set ft=c ts=2 sw=2 et:
