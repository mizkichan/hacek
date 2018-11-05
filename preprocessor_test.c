#include "alloc.h"
#include "greatest.h"

#include "preprocessor.c"

SUITE(preprocessor);

TEST test_unescape(void) {
  char src[] = "hoge\\nfuga\\tpiyo";
  unescape(src);
  ASSERT_STR_EQ("hoge\nfuga\tpiyo", src);
  PASS();
}

TEST test_concatenate_adjacent_string_literals(void) { FAIL(); }

SUITE(preprocessor) {
  RUN_TEST(test_unescape);
  RUN_TEST(test_concatenate_adjacent_string_literals);
}

// vim: set ft=c ts=2 sw=2 et:
