CC=gcc
CFLAGS=-Wall -pedantic -ansi -D_XOPEN_SOURCE
H_DIR=inc
OBJ=souls.o soundex.o
BIN=souls

all: souls

debug: CFLAGS += -DEBUG -g
debug: all

souls: souls.o soundex.o $(H_DIR)/souls.h $(H_DIR)/soundex.h
	$(CC) $(CFLAGS) souls.o soundex.o -o $@

soundex.o: soundex.c $(H_DIR)/soundex.h
	$(CC) $(CFLAGS) -c $< -o $@

souls.o: souls.c $(H_DIR)/souls.h $(H_DIR)/soundex.h
	$(CC) $(CFLAGS) -c $< -o $@

test: all

.PHONY:	clean run

run : souls
	./souls

clean:
	rm -rf $(BIN) $(OBJ) *~
