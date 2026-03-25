CC = gcc
CFLAGS = -Wall -Wextra -std=c99

all: rate edf

rate: main.c schedulling.c io.c schedulling.h io.h
	$(CC) main.c schedulling.c io.c -o rate $(CFLAGS)

edf: main.c schedulling.c io.c schedulling.h io.h
	$(CC) main.c schedulling.c io.c -o edf $(CFLAGS)

clean:
	rm -f rate edf *.out