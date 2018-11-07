CFLAGS += -std=c17 -Weverything -Wno-disabled-macro-expansion -Wno-padded -Wno-unused-command-line-argument
CPPFLAGS += -D_POSIX_C_SOURCE=200809L
LDFLAGS +=

PROGRAMS = hacek
PROGRAM_SRCS = alloc.c error.c hacek.c lexer.c parser.c preprocessor.c token.c utils.c
PROGRAM_OBJS = $(PROGRAM_SRCS:.c=.o)
PROGRAM_DEPS = $(PROGRAM_SRCS:.c=.d)

TESTS = test
TEST_SRCS = test.c lexer_test.c preprocessor_test.c utils_test.c \
	    alloc.c error.c token.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_DEPS = $(TEST_SRCS:.c=.d)

.PHONY: help
help:
	@echo 'Targets:'
	@echo '  all       - Build all targets'
	@echo '  check     - Run tests'
	@echo '  clean     - Remove generated files'
	@echo '  help      - Display this information'
	@echo
	@echo 'Variables:'
	@echo '  CC        = $(CC)'
	@echo '  CFLAGS    = $(CFLAGS)'
	@echo '  CPPFLAGS  = $(CPPFLAGS)'
	@echo '  LDFLAGS   = $(LDFLAGS)'

.PHONY: all
all: $(PROGRAMS)

.PHONY: check
check: $(TESTS)
	./$<

.PHONY: clean
clean:
	$(RM) $(PROGRAMS) $(PROGRAM_OBJS) $(PROGRAM_DEPS)
	$(RM) $(TESTS) $(TEST_OBJS) $(TEST_DEPS)

$(PROGRAMS): CPPFLAGS += -DDEBUG
$(PROGRAMS): $(PROGRAM_OBJS)

$(TESTS): $(TEST_OBJS)

%.o: %.c
	$(CC) $(CPPFLAGS) $(CFLAGS) -c -MMD -MP -o $@ $<

%: %.o
	$(CC) $(LDFLAGS) $(CFLAGS) -o $@ $^

-include $(PROGRAM_DEPS)
-include $(TEST_DEPS)

.SUFFIXES:
