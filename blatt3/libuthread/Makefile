# Hier gibt es nix neues. Die Aenderungen sind
# im Makefile im "lib"-Verzeichnis
DEPCC=$(CC) -MM
# Aufgabe 3.3 (b): Threads sollen automatisch terminieren:
CFLAGS=-g -DUTH_AUTO_TERMINATE
#CFLAGS=-g

DEPS=depend
PROG=main
LIBDIR=lib

LIB=$(LIBDIR)/libuth.a


$(PROG): main.o $(LIB)
	$(CC) $(CFLAGS) -o $@ main.o -L$(LIBDIR) -luth

-include $(DEPS)

$(LIB):
	$(MAKE) -C $(LIBDIR) CFLAGS="$(CFLAGS)"

$(DEPS):
	$(DEPCC) $(PROG).c >$(DEPS)

doc:
	doxygen doxyscript

clean:
	rm -f $(PROG).o  $(DEPS)

clobber: clean
	rm -f $(PROG) *~
	rm -rf man html
	$(MAKE) -C $(LIBDIR) $@

