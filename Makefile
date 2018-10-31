CFLAGS += -std=c17 -Weverything -Wno-disabled-macro-expansion -Wno-padded -Wno-unused-command-line-argument
CPPFLAGS += -D_POSIX_C_SOURCE=200809L
LDFLAGS += -Wno-unused-command-line-argument

PROGRAMS = hacek
PROGRAM_SRCS = hacek.c utils.c preprocessor.c parser.c lexer.c
PROGRAM_OBJS = $(PROGRAM_SRCS:.c=.o)

TESTS = test
TEST_SRCS = test.c preprocessor_test.c utils_test.c lexer_test.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

COMMON_SRCS = alloc.c error.c token.c
COMMON_OBJS = $(COMMON_SRCS:.c=.o)

SHELL = /bin/sh

.PHONY: debug
debug: CFLAGS += -g -O0
debug: all

.PHONY: release
release: CFLAGS += -O3 -march=native
release: CPPFLAGS += -DNDEBUG -D_FORTIFY_SOURCE=2
release: all

.PHONY: all
all: $(PROGRAMS)

$(PROGRAMS): $(PROGRAM_OBJS) $(COMMON_OBJS)

$(TESTS): CFLAGS += -g -O0 --coverage
$(TESTS): LDFLAGS += -g -O0 --coverage
$(TESTS): $(TEST_OBJS) $(COMMON_OBJS)

.PHONY: clean
clean:
	$(RM) $(PROGRAMS) $(TESTS) *.o *.gcno *.gcda *.gcov
