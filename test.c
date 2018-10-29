#include "alloc.h"
#include "greatest.h"

SUITE_EXTERN(preprocessor);
SUITE_EXTERN(utils);
SUITE_EXTERN(lexer);

GREATEST_MAIN_DEFS();
int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();
  alloc_init();

  RUN_SUITE(lexer);
  RUN_SUITE(preprocessor);
  RUN_SUITE(utils);

  GREATEST_MAIN_END();
}

// vim: set ft=c ts=2 sw=2 et:
