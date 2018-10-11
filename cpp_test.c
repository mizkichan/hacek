#include "greatest/greatest.h"
#include "cpp.h"
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

  actual = results.pp_tokens[0];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SIGN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[1];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("include", actual.chars);

  actual = results.pp_tokens[2];
  ASSERT_ENUM_EQ(PP_HEADER_NAME, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(H_CHAR_SEQUENCE, actual.header_name.kind,
                 header_name_kind_str);
  ASSERT_STR_EQ("stdio.h", actual.header_name.chars);

  actual = results.pp_tokens[3];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SIGN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[4];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("if", actual.chars);

  actual = results.pp_tokens[5];
  ASSERT_ENUM_EQ(PP_NUMBER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("0", actual.chars);

  actual = results.pp_tokens[6];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SIGN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[7];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("include", actual.chars);

  actual = results.pp_tokens[8];
  ASSERT_ENUM_EQ(PP_HEADER_NAME, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(Q_CHAR_SEQUENCE, actual.header_name.kind,
                 header_name_kind_str);
  ASSERT_STR_EQ("hello.h", actual.header_name.chars);

  actual = results.pp_tokens[9];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SIGN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[10];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("endif", actual.chars);

  actual = results.pp_tokens[11];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("int", actual.chars);

  actual = results.pp_tokens[12];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("main", actual.chars);

  actual = results.pp_tokens[13];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(LEFT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[14];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("void", actual.chars);

  actual = results.pp_tokens[15];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(RIGHT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[16];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(LEFT_BRACE, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[17];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("printf", actual.chars);

  actual = results.pp_tokens[18];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(LEFT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[19];
  ASSERT_ENUM_EQ(PP_STRING_LITERAL, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("Hello, world!", actual.chars);

  actual = results.pp_tokens[20];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(RIGHT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[21];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SEMICOLON, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[22];
  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("putchar", actual.chars);

  actual = results.pp_tokens[23];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(LEFT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[24];
  ASSERT_ENUM_EQ(PP_CHARACTER_CONSTANT, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("\n", actual.chars);

  actual = results.pp_tokens[25];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(RIGHT_PAREN, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[26];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(SEMICOLON, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[27];
  ASSERT_ENUM_EQ(PP_PUNCTUATOR, actual.kind, pp_token_kind_str);
  ASSERT_ENUM_EQ(RIGHT_BRACE, actual.punctuator, punctuator_str);

  actual = results.pp_tokens[28];
  ASSERT_ENUM_EQ(PP_NWSC, actual.kind, pp_token_kind_str);
  ASSERT_EQ('`', actual.nwsc);

  actual = results.pp_tokens[29];
  ASSERT_ENUM_EQ(PP_NWSC, actual.kind, pp_token_kind_str);
  ASSERT_EQ('@', actual.nwsc);

  PASS();
}

SUITE(cpp) { RUN_TEST(test_cpp_tokenize); }

// vim: set ft=c ts=2 sw=2 et:
