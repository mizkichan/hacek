#include "alloc.h"
#include "greatest.h"
#include "utils.h"

SUITE(utils);

TEST test_remove_str(void) {
  char src[] = "foobarbaz";
  remove_str(src, 'a');
  ASSERT_STR_EQ("foobrbz", src);
  remove_str(src, 'o');
  ASSERT_STR_EQ("fbrbz", src);

  PASS();
}

TEST test_clone_str_range(void) {
  char *src = "foobar2000";
  char *result;

  result = clone_str_range(&src[0], &src[3]);
  ASSERT_STR_EQ("foo", result);
  FREE(result);

  result = clone_str_range(&src[3], &src[7]);
  ASSERT_STR_EQ("bar2", result);
  FREE(result);

  result = clone_str_range(&src[7], &src[10]);
  ASSERT_STR_EQ("000", result);
  FREE(result);

  PASS();
}

SUITE(utils) {
  RUN_TEST(test_remove_str);
  RUN_TEST(test_clone_str_range);
}

// vim: set ft=c ts=2 sw=2 et:
