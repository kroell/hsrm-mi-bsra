CC=gcc
CFLAGS=-Wall -pedantic -ansi -D_XOPEN_SOURCE
H_DIR=inc
OBJ=fisch.o dijkstra.o
BIN=fisch

all: fisch

debug: CFLAGS += -DEBUG -g
debug: all

fisch: fisch.o dijkstra.o $(H_DIR)/fisch.h $(H_DIR)/dijkstra.h
	$(CC) $(CFLAGS) fisch.o dijkstra.o -o $@

dijkstra.o: dijkstra.c $(H_DIR)/dijkstra.h
	$(CC) $(CFLAGS) -c $< -o $@

fisch.o: fisch.c $(H_DIR)/fisch.h $(H_DIR)/dijkstra.h
	$(CC) $(CFLAGS) -c $< -o $@

test: all

clean:
	rm -rf $(BIN) $(OBJ) *~

clobber: clean
	rm -rf fischfile.dat tempfile.dat
