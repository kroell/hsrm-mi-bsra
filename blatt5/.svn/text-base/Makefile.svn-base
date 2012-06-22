CC=gcc
CFLAGS=-g
LIBDIR=libmemprobe/lib
BIN=assert-demo assert-demo-ndebug setjmp-demo memprobe memprobe1
OBJ=memprobe.o memprobe1.o assert-demo.i

all: assert-demo assert-demo-ndebug setjmp-demo memprobe memprobe1

assert-demo:
	cpp assert-demo.c assert-demo.i
	$(CC) $(CFLAGS) $@.c -o $@

assert-demo-ndebug:
	$(CC) $(CFLAGS) -DNDEBUG assert-demo.c -o $@

setjmp-demo:
	$(CC) $(CFLAGS) $@.c -o $@

memprobe: memprobe.o libmemprobe.a
	$(CC) $(CFLAGS) $@.o -o $@ -L$(LIBDIR) -l$@

memprobe.o: memprobe.c
	$(CC) $(CFLAGS) -c $(@:.o=.c) -o $@

memprobe1: memprobe1.o libmemprobe.a
	$(CC) $(CFLAGS) $@.o -o $@ -L$(LIBDIR) -lmemprobe

memprobe1.o: memprobe1.c
	$(CC) $(CFLAGS) -c $(@:.o=.c) -o $@

libmemprobe.a:
	$(MAKE) -C $(LIBDIR)

clean:
	rm -f $(BIN) $(OBJ) *~
	$(MAKE) -C $(LIBDIR) $@
