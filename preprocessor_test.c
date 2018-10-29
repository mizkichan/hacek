#include "alloc.h"
#include "greatest.h"
#include "token.h"

#include "preprocessor.c"

SUITE(preprocessor);

TEST test_unescape(void) {
  char src[] = "hoge\\nfuga\\tpiyo";
  unescape(src);
  ASSERT_STR_EQ("hoge\nfuga\tpiyo", src);
  PASS();
}

SUITE(preprocessor) { RUN_TEST(test_unescape); }

// vim: set ft=c ts=2 sw=2 et:
