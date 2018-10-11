#include "greatest/greatest.h"
#include "cpp.h"
#include "tokens.h"

SUITE(cpp);

TEST test_preprocess(void) {
  char *src = "foobar2000\n";
  struct PPTokenList results = preprocess(src);
  struct PPToken actual = results.pp_tokens[0];

  ASSERT_ENUM_EQ(PP_IDENTIFIER, actual.kind, pp_token_kind_str);
  ASSERT_STR_EQ("foobar2000", actual.chars);

  PASS();
}

SUITE(cpp) { RUN_TEST(test_preprocess); }

// vim: set ft=c ts=2 sw=2 et:
