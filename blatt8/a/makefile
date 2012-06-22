CC=gcc
CFLAGS=-Wall -pedantic -ansi -D_XOPEN_SOURCE
H_DIR=inc
OBJ=soundex-test.o soundex.o
BIN=soundex-test

all: soundex-test

debug: CFLAGS += -DEBUG -g
debug: all

soundex-test: soundex-test.o soundex.o $(H_DIR)/soundex-test.h $(H_DIR)/soundex.h
	$(CC) $(CFLAGS) soundex-test.o soundex.o -o $@

soundex.o: soundex.c $(H_DIR)/soundex.h
	$(CC) $(CFLAGS) -c $< -o $@

soundex-test.o: soundex-test.c $(H_DIR)/soundex-test.h $(H_DIR)/soundex.h
	$(CC) $(CFLAGS) -c $< -o $@

test: all

.PHONY:	clean run

run : soundex-test
	./soundex-test

clean:
	rm -rf $(BIN) $(OBJ) *~
