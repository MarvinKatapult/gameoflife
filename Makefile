
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
BIN = gameoflife
INC_RAYLIB = -Lraylib/src -lraylib -Iraylib/src

.PHONY: raylib

all: raylib cvecs.o
	$(CC) -o $(BIN) $(CFLAGS) gameoflife.c cvecs.o $(INC_RAYLIB) -I./ -Iexternal -lm

cvecs.o:
	$(CC) -c cvecs.o $(CFLAGS) external/cvecs/cvecs.c -I./external

raylib:
	cd raylib/src && $(MAKE) -j5

clean:
	rm -f $(BIN)
	rm -f *.o
	cd raylib/src && $(MAKE) clean
