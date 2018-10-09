CFLAGS += -std=c17 -Weverything -Wno-unused-command-line-argument -Wno-padded
CPPFLAGS += -D_POSIX_C_SOURCE=200809L
LDFLAGS +=

PROGRAMS = hacek test
SRCS = utils.c error.c cpp.c
OBJS = $(SRCS:.c=.o)

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

$(PROGRAMS): %: %.o $(OBJS)

.PHONY: clean
clean:
	$(RM) $(PROGRAMS) *.o
