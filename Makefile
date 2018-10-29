CFLAGS += -std=c17 -Weverything -Wno-disabled-macro-expansion -Wno-padded -Wno-unused-command-line-argument
CPPFLAGS += -D_POSIX_C_SOURCE=200809L
LDFLAGS += -Wno-unused-command-line-argument

PROGRAMS = hacek
PROGRAM_SRCS = utils.c preprocessor.c parser.c lexer.c
PROGRAM_OBJS = $(PROGRAM_SRCS:.c=.o)

TESTS = test
TEST_SRCS = preprocessor_test.c utils_test.c lexer_test.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

COMMON_SRCS = alloc.c error.c token.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

SHELL = /bin/sh

.PHONY: debug
debug: CFLAGS += -g -O0
debug: all

.PHONY: release
release: CFLAGS += -O3 -march=native
release: CPPFLAGS += -D_FORTIFY_SOURCE=2
release: all

.PHONY: all
all: $(PROGRAMS)

$(PROGRAMS): %: %.o $(PROGRAM_OBJS) $(COMMON_OBJS)

$(TESTS): CFLAGS += -g -O0 --coverage
$(TESTS): LDFLAGS += --coverage
$(TESTS): %: %.o $(TEST_OBJS) $(COMMON_OBJS)

.PHONY: clean
clean:
	$(RM) $(PROGRAMS) $(PROGRAMS:=.o) $(PROGRAM_OBJS)
	$(RM) $(TESTS) $(TESTS:=.o) $(TEST_OBJS)
	$(RM) $(COMMON_OBJS)
	$(RM) *.gcno *.gcda *.gcov
