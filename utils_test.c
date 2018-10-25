#include "greatest.h"
#include "utils.h"

SUITE(utils);

TEST test_erase_str(void) {
  {
    char src[] = "abcde";
    erase_str(src, 1, 3);
    ASSERT_STR_EQ("ade", src);
  }

  {
    char src[] = "abcde";
    erase_str(src, 0, 2);
    ASSERT_STR_EQ("cde", src);
  }

  {
    char src[] = "abcde";
    erase_str(src, 3, 5);
    ASSERT_STR_EQ("abc", src);
  }

  {
    char src[] = "abcde";
    erase_str(src, 3, 3);
    ASSERT_STR_EQ("abcde", src);
  }

  PASS();
}

TEST test_clone_str_range(void) {
  char *src = "foobar2000";

  ASSERT_STR_EQ("foo", clone_str_range(&src[0], &src[3]));
  ASSERT_STR_EQ("bar2", clone_str_range(&src[3], &src[7]));
  ASSERT_STR_EQ("000", clone_str_range(&src[7], &src[10]));
  PASS();
}

SUITE(utils) {
  RUN_TEST(test_erase_str);
  RUN_TEST(test_clone_str_range);
}

// vim: set ft=c ts=2 sw=2 et:
