CFLAGS += -std=c17 -Weverything -Wno-disabled-macro-expansion -Wno-padded -Wno-unused-command-line-argument
CPPFLAGS += -D_POSIX_C_SOURCE=200809L
LDFLAGS +=

PROGRAMS = hacek
PROGRAM_SRCS = utils.c error.c preprocessor.c token.c
PROGRAM_OBJS = $(PROGRAM_SRCS:.c=.o)

TESTS = test
TEST_SRCS = $(PROGRAM_SRCS) preprocessor_test.c utils_test.c
TEST_OBJS = $(TEST_SRCS:.c=.o)

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

$(PROGRAMS): %: %.o $(PROGRAM_OBJS)

$(TESTS): CFLAGS += -g -O0
$(TESTS): %: %.o $(TEST_OBJS)

.PHONY: clean
clean:
	$(RM) $(PROGRAMS) $(PROGRAM_OBJS)
	$(RM) $(TESTS) $(TEST_OBJS)
