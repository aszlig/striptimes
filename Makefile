CC = gcc
SOURCES = patch.c
CSTD = -ansi
CFLAGS = -Wall -Wextra $(CSTD) -pedantic-errors -O3
LDFLAGS = -s

PROGRAMS = striptimes test

all: striptimes

check: test; ./test

test: private CSTD = -std=c99 -D_XOPEN_SOURCE=500
$(PROGRAMS): %: %.c $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(PROGRAMS) $(SOURCES:.c=.o)

.PHONY: all clean check
