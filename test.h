#ifndef TEST_H
#define TEST_H

#define ASSERT_STR_RANGE_EQ(EXPECTED, BEGIN, END)                              \
  ASSERT_STRN_EQ(EXPECTED, BEGIN, (size_t)(END - BEGIN))

#endif
// vim: set ft=c ts=2 sw=2 et:
