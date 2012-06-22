PROG=moco-ass
CFILES=$(PROG).c

OBJS=$(CFILES:.c=.o)
DEPS=depend

CFLAGS=-g -I../inc
# CFLAGS=-g -DNDEBUG
DEPCC=$(CC) -MM

.PHONY: all clean clobber

all:  $(CFILES:.c=)

-include $(DEPS)

$(OBJS):

$(DEPS):
	$(DEPCC) $(CFLAGS) $(CFILES) >$(DEPS)

clean:
	rm -f $(OBJS) $(DEPS)

clobber: clean
	rm -f *~ $(PROG)

