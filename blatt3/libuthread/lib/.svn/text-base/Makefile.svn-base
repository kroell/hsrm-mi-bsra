DEPCC=$(CC) -MM
CFLAGS=-g

FILES=jmp.c uth.c
OBJS=$(FILES:.c=.o)
DEPS=depend
LIB=libuth.a

# Biliothek erzeugen: "$@" erzeugt das
# "Target" der Make-Regel (hier: "$(LIB)")
# und damit letztendlich (s.o) "libsp.a". 
$(LIB): $(OBJS)
	$(AR) -rv $@ $(OBJS)

-include $(DEPS)

$(OBJS):

$(DEPS):
	$(DEPCC) $(FILES) >$(DEPS)

clean:
	rm -f $(OBJS) $(DEPS) res_op.s res_op.o

clobber: clean
	rm -f $(LIB) *~

