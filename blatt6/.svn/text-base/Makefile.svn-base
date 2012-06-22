CC=gcc
CFLAGS=-g -Wall -pedantic -ansi
H_DIR=inc
OBJ=fisch.o
BIN=fisch

all: fisch

fisch: fisch.o $(H_DIR)/fisch.h
	$(CC) $(CFLAGS) $< -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

test: all
	./fisch -n toni landmann
	./fisch -n toni scheidewind
	./fisch -l

	./fisch -n david deins
	./fisch -n david dzwei
	./fisch -z david 2
	./fisch -l

	./fisch -l toni
	./fisch -l david

	./fisch -d toni
	./fisch -d david
	./fisch -l

clean:
	rm -rf $(BIN) $(OBJ) *~

clobber: clean
	rm -rf fischfile.dat tempfile.dat