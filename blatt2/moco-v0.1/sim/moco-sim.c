/**
 ** MoCo -- Model Computer Simulation
 **
 ** Usage: moco-sim <program.exe>
 **
 ** Prof. Dr. Robert Kaiser
 ** Hochschule RheinMain, DCSM
 **
 **
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "moco-opcodes.h"
#include "moco-mem.h"

#define NUM_REGISTERS  16

static struct ProcessorState
{
    int Register[NUM_REGISTERS];
    int PC;
    enum { NEG, POS, ZERO} F;
} Processor;


/** Throw an exception: for now this just terminates the processor */
static void exception(char* text)
{
    printf("******* Exception: %s *******\n", text);
    exit(1);
}

/** Update flags according to last arithmetic result */
static void update_F(int result)
{
    if(result < 0)
        Processor.F = NEG;
    else if (result > 0)
        Processor.F = POS;
    else /** if (result == 0) */
        Processor.F = ZERO;
}

/** Stop instruction: Halt processor */
static void I_halt(void)
{
    printf("******* Program ended *******\n");
}

/************************* blatt2 a2.3 - BEGIN ********************************/
/**      von Tino Landmann, Sören Kröll, Christian Caspers, David Gens       **/
/******************************************************************************/
#define SP 15

/** a2.3: check if register address ra is valid **/
static int R_Access(int ra)
{
  return (ra >= 0 && ra < NUM_REGISTERS);
}


/** a2.3a: Load instruction: load content of memory-address ra into rt **/
static void I_load(int ra, int rt)
{
  if (!R_Access(rt) || !R_Access(ra))
    exception("invalid register!");
  if (!M_DataAccessWord(Processor.Register[ra]))
    exception("invalid memory address!");
  update_F(Processor.Register[rt] = M_ReadDataWord(Processor.Register[ra]));
}

/** a2.3a: Load-Byte instruction:
 **        load byte content of memory-address ra+ro into rt **/
static void I_loadb(int ra, int ro, int rt)
{
  if (!R_Access(ra) || !R_Access(ro) || !R_Access(rt))
    exception("invalid register!");
  if (!M_DataAccessByte(Processor.Register[ra]+Processor.Register[ro]))
    exception("invalid memory address!");
  update_F(Processor.Register[rt] = 
	     M_ReadDataByte(Processor.Register[ra]+Processor.Register[ro])
  );
}

/** a2.3a: Store instruction: store value of rs into memory-address ra **/
static void I_stor(int rs, int ra)
{
  if (!R_Access(rs) || !R_Access(ra))
    exception("invalid register!");
  if (!M_DataAccessWord(Processor.Register[ra]))
    exception("invalid memory address!");
  M_WriteDataWord(Processor.Register[ra],Processor.Register[rs]);
}

/** a2.3a: Store-Byte instruction: store value of rs into ra+ro **/
static void I_storb(int rs, int ra, int ro)
{
  if (!R_Access(rs) || !R_Access(ra) || !R_Access(ro))
    exception("invalid register!");
  if (!M_DataAccessByte(Processor.Register[ra]+Processor.Register[ro]))
    exception("invalid memory address!");
  M_WriteDataByte(
    Processor.Register[ra]+Processor.Register[ro],
    Processor.Register[rs]
  );
}

/** a2.3b: pushes content of rs onto the stack **/
static void I_push(int rs)
{
  if (!R_Access(rs))
    exception("invalid register: I_push");
  if (!M_DataAccessWord(Processor.Register[SP] - sizeof(int)))
    exception("stack overflow: I_push");
  Processor.Register[SP] -= sizeof(int);  /* increments (!) stack-index */
  M_WriteDataWord(Processor.Register[SP],Processor.Register[rs]);
}

/** a2.3b: pops top of stack into rt **/
static void I_pop(int rt)
{
  if (!R_Access(rt))
    exception("invalid register: I_pop");
  update_F(Processor.Register[rt] = M_ReadDataWord(Processor.Register[SP]));
  //  if (!M_DataAccessWord(Processor.Register[SP] + sizeof(int)))
  //    exception("stack empty: I_pop");
  Processor.Register[SP] += sizeof(int); /* decrements (!) stack-index */
}

/** a2.3c: jumps to address a **/
static void I_jsr(int a)
{
  if (!M_DataAccessWord(a))
    exception("invalid memory address: I_jsr");
  if (!M_DataAccessWord(Processor.Register[SP] - sizeof(int)))
    exception("stack overflow: I_jsr");
  Processor.Register[SP] -= sizeof(int);
  M_WriteDataWord(Processor.Register[SP],Processor.PC);
  Processor.PC = a;
}

/** a2.3c: returns to caller **/
static void I_ret(void)
{
  if (!M_DataAccessWord(Processor.Register[SP]))
    exception("stack empty: I_ret");
  Processor.PC = M_ReadDataWord(Processor.Register[SP]);
  //  if (!M_DataAccessWord(Processor.Register[SP] + sizeof(int)))
  //    exception("stack empty: I_ret");
  Processor.Register[SP] += sizeof(int); /* decrements (!) stack-index */
}
/*************************** blatt2 a2.3 - END ********************************/
/**      von Tino Landmann, Sören Kröll, Christian Caspers, David Gens       **/
/******************************************************************************/


/** In instruction: Read decimal number into register */
static void I_in(int s, int rt)
{
    static int new_line = 1;
    if(s == 0) /** decimal number input */
    {
        printf("In <%d>", s);
        fflush(stdout);
        if(scanf("%d", &Processor.Register[rt])!= 1)
	    fflush(stdin);
    }
    else if(s == 1) /** text input */
    {   /** print prompt only if beginning a new line */
	if(new_line)
	{
	    printf("In <%d>", s);
	    fflush(stdout);
	    new_line = 0;
	}
        if((Processor.Register[rt] = getchar()) == '\n')
	{
	    Processor.Register[rt] = '\0';
	    new_line = 1;
	}
    }
}

/** Out instruction: Output register contents */
static void I_out(int rs, int s)
{
    static int new_line = 1;
    if(s == 0) /** decimal number input */
    {
	printf("Out <%d> %d\n", s, Processor.Register[rs]);
    }
    else if(s == 1) /** text output */
    {
        if(new_line)
	{   /** print prompt only if a new line has been started */
	    printf("Out <%d> %c", s, Processor.Register[rs]);
	    new_line = 0;
	}
	else
	{
	    putchar(Processor.Register[rs]);
            if(Processor.Register[rs] == '\n')
	      new_line = 0;
	}
    }
}


/** Ldn instruction: Load constant into register */
static void I_ldn(int k, int rt)
{
    if(rt < 0 || rt >= NUM_REGISTERS)
    {
        exception("invalid register!");
    }
    update_F(Processor.Register[rt] = k);
}

/** Add instruction: adds contents of registers rs1 and rs2,
    stores result into register rt (rt = rs1 + rs2) */
static void I_add(int rs1, int rs2, int rt)
{
    if(rs1 < 0 || rs1 >= NUM_REGISTERS ||
       rs2 < 0 || rs2 >= NUM_REGISTERS ||
       rt  < 0 || rt  >= NUM_REGISTERS)
    {
        exception("invalid register!");
    }
    update_F(Processor.Register[rt] = 
             Processor.Register[rs1] + Processor.Register[rs2]);
}

/** Sub instruction: subtracts contents of register rs2 from rs1,
    stores result in register rt (rt = rs1 - rs2) */
static void I_sub(int rs1, int rs2, int rt)
{
    if(rs1 < 0 || rs1 >= NUM_REGISTERS ||
       rs2 < 0 || rs2 >= NUM_REGISTERS ||
       rt  < 0 || rt  >= NUM_REGISTERS)
    {
        exception("invalid register!");
    }
    update_F(Processor.Register[rt] = 
             Processor.Register[rs1] - Processor.Register[rs2]);
}

/** Mul instruction: multiplies contents of registers rs1 and rs2,
    stores result in register rt (rt = rs1 * rs2) */
static void I_mul(int rs1, int rs2, int rt)
{
    if(rs1 < 0 || rs1 >= NUM_REGISTERS ||
       rs2 < 0 || rs2 >= NUM_REGISTERS ||
       rt  < 0 || rt  >= NUM_REGISTERS)
    {
        exception("invalid register!");
    }
    update_F(Processor.Register[rt] = 
             Processor.Register[rs1] * Processor.Register[rs2]);
}

/** Div instruction: divides contents of register rs1 by rs2,
    stores result in register rt (rt = rs1 / rs2) */
static void I_div(int rs1, int rs2, int rt)
{
    if(rs1 < 0 || rs1 >= NUM_REGISTERS ||
       rs2 < 0 || rs2 >= NUM_REGISTERS ||
       rt  < 0 || rt  >= NUM_REGISTERS)
    {
        exception("invalid register!");
    }
    else if(Processor.Register[rs2] == 0)
    {
        exception("division by zero!");
    }
    update_F(Processor.Register[rt] = 
             Processor.Register[rs1] / Processor.Register[rs2]);
}

/** Addn instruction: adds constant k to register rs,
    stores result in register rt (rt = rs + k) */
static void I_addn(int rs, int k, int rt)
{
    if(rs < 0 || rs >= NUM_REGISTERS ||
       rt < 0 || rt  >= NUM_REGISTERS)
    {
        exception("invalid register!");
    }
    update_F(Processor.Register[rt] = Processor.Register[rs] + k);
}

/** Subn instruction: subtracts constant k from register rs,
    stores result in register rt (rt = rs - k) */
static void I_subn(int rs, int k, int rt)
{
    if(rs < 0 || rs >= NUM_REGISTERS ||
       rt < 0 || rt >= NUM_REGISTERS)
    {
        exception("invalid register!");
    }
    update_F(Processor.Register[rt] = Processor.Register[rs] - k);
}

/** Muln instruction: multiplies constant k and register rs,
    stores result in register rt (rt = rs * k) */
static void I_muln(int rs, int k, int rt)
{
    if(rs < 0 || rs >= NUM_REGISTERS ||
       rt < 0 || rt >= NUM_REGISTERS)
    {
        exception("invalid register!");
    }
    update_F(Processor.Register[rt] = Processor.Register[rs] * k);
}

/** Div instruction: divides contents of register rs by constant k,
    stores result in register rt (rt = rs / k) */
static void I_divn(int rs, int k, int rt)
{
    if(rs < 0 || rs >= NUM_REGISTERS ||
       rt < 0 || rt >= NUM_REGISTERS)
    {
        exception("invalid register!");
    }
    else if(k == 0)
    {
        exception("Division durch Null!");
    }
    update_F(Processor.Register[rt] = Processor.Register[rs] / k);
}


/** Jmp instruction: jump to address */
static void I_jmp(int a)
{
    Processor.PC = a;
}

/** Jmpz instruction: jump to address if F = Zero  */
static void I_jmpz(int a)
{
    if(ZERO == Processor.F)
        Processor.PC = a;
}

/** Jmpnz instruction: jump to address if F != Zero  */
static void I_jmpnz(int a)
{
    if(ZERO != Processor.F)
        Processor.PC = a;
}

/** Jmpp instruction: jump to address if F = POS */
static void I_jmpp(int a)
{
    if(POS == Processor.F)
        Processor.PC = a;
}

/** Jmpn instruction: jump to address if F = NEG */
static void I_jmpn(int a)
{
    if(NEG == Processor.F)
        Processor.PC = a;
}


struct instruction
{
    void (*instruction)();
    int num_ops;
};

struct instruction InstructionSet[NUM_INSTRUCTIONS] = 
{
    { I_halt,     0},
    { I_in,       2},
    { I_out,      2},
    { I_ldn,      2},
    { I_add,      3},
    { I_sub,      3},
    { I_mul,      3},
    { I_div,      3},
    { I_addn,     3},
    { I_subn,     3},
    { I_muln,     3},
    { I_divn,     3},
    { I_jmp,      1},
    { I_jmpz,     1},
    { I_jmpnz,    1},
    { I_jmpp,     1},
    { I_jmpn,     1},
    { I_load,     2},
    { I_loadb,    3},
    { I_stor,     2},
    { I_storb,    3},
    { I_push,     1},
    { I_pop,      1},
    { I_jsr,      1},
    { I_ret,      0},
};


/** The Processor: */
static void TheProcessor()
{
    int OpCode;
    void (*Instruction)();
    int Operands[3];
    int NumOperands;
    int i,j = 0;

    Processor.PC = 0;

    while(1)
    {
        ++j;
        if(!M_ProgramAccessWord(Processor.PC))
        {
             exception("invalid memory address!");
        }

        /** fetch next instruction */
        OpCode = M_ReadProgramWord(Processor.PC);

	Processor.PC += sizeof(int);

	if(OpCode < 0 || OpCode >= NUM_INSTRUCTIONS)
        {
            exception("invalid opcode!");
        }

        /** check number of operands (max. 3) */
        NumOperands = InstructionSet[OpCode].num_ops;
        if(NumOperands > 3)
        {
            exception("invalid number of operands!");
            exit(1);
        }
	
        /** Fetch operands */
        for(i = 0; i < NumOperands; i++)
	{
	    if(!M_ProgramAccessWord(Processor.PC))
	    {
		exception("invalid memory address!");
	    }
            Operands[i] = M_ReadProgramWord(Processor.PC);
            Processor.PC += sizeof(int);
	}
        /** Execute instruction */
        Instruction = InstructionSet[OpCode].instruction;
        Instruction(Operands[0], Operands[1], Operands[2]);
        if(OpCode == HALT)
            return;
    }
}

int main(int argc, char *argv[])
{
    struct stat statbuf;

    if(argc != 2)
    {
        printf("Usage: moco-sim <program file>\n");
        exit(1);
    }
    if(stat(argv[1], &statbuf) != 0)
    {
        perror("stat");
        exit(1);
    }
    if(M_CreateSpace(argv[1], statbuf.st_size) < 0)
        return 1;
    TheProcessor();
    M_DeleteSpace(0);
    return 0;
}
