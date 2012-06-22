PROG=moco-sim
CFILES=$(PROG).c moco-mem.c

OBJS=$(CFILES:.c=.o)
DEPS=depend

CFLAGS=-g -I../inc
# CFLAGS=-O2 -Wall -I../inc
DEPCC=$(CC) -MM

.PHONY: all clean clobber

all:  $(PROG)

-include $(DEPS)

$(PROG): $(OBJS)
	$(CC) -o $@ $(OBJS)

$(OBJS):

$(DEPS):
	$(DEPCC) $(CFLAGS) $(CFILES) >$(DEPS)

clean:
	rm -f $(OBJS) $(DEPS)

clobber: clean
	rm -f *~ $(PROG)

