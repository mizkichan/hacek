#include "greatest.h"
#include "utils.h"

SUITE(utils);

TEST test_erase(void) {
  {
    int actual[] = {1, 2, 3, 4, 5};
    int expected[] = {1, 4, 5};
    erase(&actual[5], &actual[1], &actual[3]);
    ASSERT_MEM_EQ(expected, actual, 3);
  }

  {
    int actual[] = {1, 2, 3, 4, 5};
    int expected[] = {3, 4, 5};
    erase(&actual[5], &actual[0], &actual[2]);
    ASSERT_MEM_EQ(expected, actual, 3);
  }

  {
    int actual[] = {1, 2, 3, 4, 5};
    int expected[] = {1, 2, 3};
    erase(&actual[5], &actual[3], &actual[5]);
    ASSERT_MEM_EQ(expected, actual, 3);
  }

  PASS();
}

SUITE(utils) { RUN_TEST(test_erase); }

// vim: set ft=c ts=2 sw=2 et:
