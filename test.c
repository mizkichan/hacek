#include "greatest.h"

SUITE_EXTERN(preprocessor);
SUITE_EXTERN(utils);
SUITE_EXTERN(lexer);
SUITE_EXTERN(parser);

GREATEST_MAIN_DEFS();
int main(int argc, char **argv) {
  GREATEST_MAIN_BEGIN();

  RUN_SUITE(utils);
  RUN_SUITE(lexer);
  RUN_SUITE(preprocessor);
  RUN_SUITE(parser);

  GREATEST_MAIN_END();
}

// vim: set ft=c ts=2 sw=2 et:
