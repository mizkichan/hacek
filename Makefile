CC = clang
CFLAGS += -std=c17 -g -O0 -Werror -Weverything -Wno-padded -D_POSIX_C_SOURCE=200809L

TARGETS = hacek test
SRCS = utils.c cpp.c
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: hacek

$(TARGETS): %: %.o $(OBJS)

.PHONY: clean
clean:
	$(RM) $(TARGETS) *.o
