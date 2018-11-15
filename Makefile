CFLAGS += -std=c17 -Weverything -Wno-disabled-macro-expansion -Wno-padded -Wno-unused-command-line-argument
CPPFLAGS += -D_POSIX_C_SOURCE=200809L
LDFLAGS +=
LDLIBS +=

PROGRAMS = hacek
PROGRAM_SRCS = alloc.c error.c hacek.c lexer.c parser.c preprocessor.c token.c utils.c
PROGRAM_OBJS = $(PROGRAM_SRCS:.c=.o)
PROGRAM_DEPS = $(PROGRAM_SRCS:.c=.d)

TESTS = test
TEST_SRCS = test.c lexer_test.c preprocessor_test.c utils_test.c parser_test.c \
	    alloc.c error.c token.c
TEST_OBJS = $(TEST_SRCS:.c=.o)
TEST_DEPS = $(TEST_SRCS:.c=.d)

DOXYGEN_DIR = docs

.PHONY: help
help:
	@echo 'Targets:'
	@echo '  all       - Build all targets'
	@echo '  check     - Run tests'
	@echo '  html      - Generate documents'
	@echo '  gh-pages  - Publish documents to gh-pages branch on GitHub'
	@echo '  clean     - Remove generated files'
	@echo '  help      - Display this information'
	@echo
	@echo 'Variables:'
	@echo '  CC        = $(CC)'
	@echo '  CFLAGS    = $(CFLAGS)'
	@echo '  CPPFLAGS  = $(CPPFLAGS)'
	@echo '  LDFLAGS   = $(LDFLAGS)'
	@echo '  LDLIBS    = $(LDLIBS)'

.PHONY: all
all: $(PROGRAMS)

.PHONY: check
check: $(TESTS)
	./$<

.PHONY: html
html: $(DOXYGEN_DIR)

.PHONY: gh-pages
gh-pages: $(DOXYGEN_DIR)
	gh-pages -d $(DOXYGEN_DIR)

.PHONY: clean
clean:
	$(RM) $(PROGRAMS) $(PROGRAM_OBJS) $(PROGRAM_DEPS)
	$(RM) $(TESTS) $(TEST_OBJS) $(TEST_DEPS)
	$(RM) -r $(DOXYGEN_DIR)

$(PROGRAMS): CPPFLAGS += -DDEBUG
$(PROGRAMS): $(PROGRAM_OBJS)

$(TESTS): $(TEST_OBJS)

$(DOXYGEN_DIR):
	doxygen

%.o: %.c
	$(CC) $(CFLAGS) $(CPPFLAGS) -c -MMD -MP -o $@ $<

%: %.o
	$(CC) $(LDFLAGS) $(LDLIBS) -o $@ $^

-include $(PROGRAM_DEPS)
-include $(TEST_DEPS)

.SUFFIXES:
