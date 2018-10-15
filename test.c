#pragma GCC diagnostic ignored "-Wdisabled-macro-expansion"
#include "greatest/greatest.h"

SUITE_EXTERN(cpp);
SUITE_EXTERN(utils);

GREATEST_MAIN_DEFS();
int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();

  RUN_SUITE(cpp);
  RUN_SUITE(utils);

  GREATEST_MAIN_END();
}

// vim: set ft=c ts=2 sw=2 et:
