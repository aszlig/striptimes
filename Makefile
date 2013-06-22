CC = gcc
SOURCES = patch.c
CSTD = -ansi
CFLAGS = -Wall -Wextra $(CSTD) -pedantic-errors -O3
LDFLAGS = -s

PROGRAMS = striptimes

all: striptimes

%.o: %.c %.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(PROGRAMS): %: %.c $(SOURCES:.c=.o)
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $^

clean:
	$(RM) $(PROGRAMS) $(SOURCES:.c=.o)

.PHONY: clean
