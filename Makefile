CFLAGS += -std=c17 -Weverything -Wno-padded
CPPFLAGS += -D_POSIX_C_SOURCE=200809L
LDFLAGS +=

TARGETS = hacek test
SRCS = utils.c error.c cpp.c
OBJS = $(SRCS:.c=.o)

SHELL = /bin/sh

.PHONY: debug
debug: CFLAGS += -g -O0
debug: all

.PHONY: release
release: CFLAGS += -O3 -march=native
release: all

.PHONY: all
all: hacek

$(TARGETS): %: %.o $(OBJS)

.PHONY: clean
clean:
	$(RM) $(TARGETS) *.o
