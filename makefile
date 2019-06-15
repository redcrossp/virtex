CC=gcc
CFLAGS=-Wall -pedantic -std=c99
FILES=virtex.o format.o
BUILD=build

.PHONY: all clean

all: virtex

clean:
	rm -f virtex *.o

virtex: $(FILES)
	$(CC) $(CFLAGS) -o "${build}$@" $^

%.o: %.c
	$(CC) $(CFLAGS) -c "$(build)$<"
