CC = gcc
SOURCES = striptimes.c
CFLAGS = -Wall -Wextra -ansi -pedantic-errors -O3 -s

all: striptimes

striptimes: $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	$(RM) striptimes

.PHONY: clean
