ASMFILES=a2.3a_testByte.asm a2.3a_testWord.asm \
	 a2.3b_testPP.asm a2.3c_testJSR.asm \
	 hellomoco.asm
OBJS=$(ASMFILES:.asm=.moco)
ASM=../moco-v0.1/ass/moco-ass
SIM=../moco-v0.1/sim/moco-sim

.PHONY: all clean clobber run

all:  $(OBJS)

%.moco: %.asm
	$(ASM) $<  $@

clean:
	rm -f $(OBJS)

clobber: clean
	rm -f *~

run: $(OBJS)
	@echo "Testing Byte Store/Load-Instructions............."
	@echo "In <0>        256"
	@echo 256 | $(SIM) a2.3a_testByte.moco
	@sleep 1
	@echo "Testing Word Store/Load-Instructions............."
	@echo "In <0>        256256256"
	@echo 256256256 | $(SIM) a2.3a_testWord.moco
	@sleep 1
	@echo "Testing Push/Pop-Instructions...................."
	@echo "In <0>        256256256"
	@echo 256256256 | $(SIM) a2.3b_testPP.moco
	@sleep 1
	@echo "Testing Jump-Subroutine/Return-Instructions......"
	@echo "In <0>        256256256 + 3 = ?"
	@echo 256256256 | $(SIM) a2.3c_testJSR.moco
	@sleep 1
	@echo "Testing Hellomoco-Skript........................."
	@echo "In <1>        Joendhard"
	@echo Joendhard | $(SIM) hellomoco.moco