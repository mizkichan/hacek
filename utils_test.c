#include "alloc.h"
#include "greatest.h"

#include "utils.c"

SUITE(utils);

TEST test_remove_str(void) {
  char src[] = "foobarbaz";
  remove_str(src, 'a');
  ASSERT_STR_EQ("foobrbz", src);
  remove_str(src, 'o');
  ASSERT_STR_EQ("fbrbz", src);

  PASS();
}

TEST test_erase_str(void) {
  char src[] = "foobarbaz";
  erase_str(src, src + 2);
  ASSERT_STR_EQ("obarbaz", src);
  erase_str(src + 5, src + 7);
  ASSERT_STR_EQ("obarb", src);
  erase_str(src + 2, src + 3);
  ASSERT_STR_EQ("obrb", src);
  erase_str(src, src);
  ASSERT_STR_EQ("obrb", src);
  erase_str(src + 4, src + 4);
  ASSERT_STR_EQ("obrb", src);
  erase_str(src + 1, src + 1);
  ASSERT_STR_EQ("obrb", src);
  erase_str(src + 2, src + 4);
  ASSERT_STR_EQ("ob", src);
  erase_str(src, src + 2);
  ASSERT_STR_EQ("", src);

  PASS();
}

TEST test_str_equals(void) {
  char *str = "foobar2000";

  ASSERT(str_equals("foobar2000", str, str + 10));
  ASSERT(str_equals("foobar2000", str, NULL));
  ASSERT(!str_equals("foobar", str, str + 10));
  ASSERT(str_equals("foobar", str, str + 6));
  ASSERT(!str_equals("foobaz", str, str + 6));

  PASS();
}

TEST test_clone_str(void) {
  char *str = "foobar2000";
  char *cloned;

  cloned = clone_str(str, str);
  ASSERT_STR_EQ("", cloned);
  FREE(cloned);

  cloned = clone_str(str, str + 10);
  ASSERT_STR_EQ("foobar2000", cloned);
  FREE(cloned);

  cloned = clone_str(str + 10, str + 10);
  ASSERT_STR_EQ("", cloned);
  FREE(cloned);

  cloned = clone_str(str, NULL);
  ASSERT_STR_EQ("foobar2000", cloned);
  FREE(cloned);

  PASS();
}

SUITE(utils) {
  RUN_TEST(test_remove_str);
  RUN_TEST(test_erase_str);
  RUN_TEST(test_clone_str);
  RUN_TEST(test_str_equals);
}

// vim: set ft=c ts=2 sw=2 et:
