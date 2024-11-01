
CC = gcc
CFLAGS = -Wall -Wextra -pedantic -g
BIN = gameoflife
INC_RAYLIB = -Lraylib/src -lraylib -Iraylib/src

.PHONY: raylib

all: raylib 
	$(CC) -o $(BIN) $(CFLAGS) gameoflife.c $(INC_RAYLIB) -I./ -Iexternal -lm

raylib:
	cd raylib/src && $(MAKE) -j5

clean:
	rm -f $(BIN)
	rm -f *.o
	cd raylib/src && $(MAKE) clean
