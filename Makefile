CC = clang
CFLAGS += -std=c17 -g -O0 -Werror -Weverything -Wno-vla -Wno-padded

TARGETS = hacek test
SRCS = utils.c
OBJS = $(SRCS:.c=.o)

.PHONY: all
all: hacek

$(TARGETS): %: %.o $(OBJS)

.PHONY: clean
clean:
	$(RM) $(TARGETS) *.o
