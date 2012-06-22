ASMDIR=moco-v0.1/ass/
SIMDIR=moco-v0.1/sim/
TESDIR=test/
ASM=$(ASMDIR)moco-ass
SIM=$(SIMDIR)moco-sim

.PHONY: all clean clobber run test

all:  $(ASM) $(SIM) test

test:
	@cd $(TESDIR); make

clean:
	@cd $(ASMDIR); make clean
	@cd $(SIMDIR); make clean
	@cd $(TESDIR); make clean

clobber: clean
	rm -f *~
	@cd $(ASMDIR); make clobber
	@cd $(SIMDIR); make clobber
	@cd $(TESDIR); make clobber

$(ASM):
	cd $(ASMDIR); make
$(SIM):
	cd $(SIMDIR); make

run:	$(ASM) $(SIM)
	cd $(TESDIR); make run