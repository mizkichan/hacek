CFLAGS += -std=c17 -Weverything -Wno-unused-command-line-argument -Wno-padded
CPPFLAGS +=
LDFLAGS +=

PROGRAMS = hacek
PROGRAM_SRCS = utils.c error.c cpp.c tokens.c
PROGRAM_OBJS = $(PROGRAM_SRCS:.c=.o)

TESTS = test
TEST_SRCS = $(PROGRAM_SRCS) cpp_test.c utils_test.c
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
