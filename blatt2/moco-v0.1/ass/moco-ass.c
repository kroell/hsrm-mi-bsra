/**
 ** MoCo -- Model Computer Assembler
 **
 ** Usage: moco-ass <program.asm> [<program.exe>]
 **
 ** Prof. Dr. Robert Kaiser
 ** Hochschule RheinMain, DCSM
 **
 **
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "moco-opcodes.h"

#define NUM_REGISTERS  16

/** Additional pseudo-opcodes understood by the assembler */
enum pseudo_opcode
{
    DC      = NUM_INSTRUCTIONS,
    DS      = NUM_INSTRUCTIONS+1,
    DB      = NUM_INSTRUCTIONS+2,
};

/**
 *  Structure defining instructions and their expected parameters
 */
struct instruction
{
    char *Name; /** Name string */
    enum opcode  Opcode; /** Opcode */
    enum optype { T_NONE, T_REG, T_NUM, T_STR } op[3]; /** expected operands */
};

/** Instruction set */
static struct instruction TheInstructionSet[] =
{
    {"HALT" , HALT , {T_NONE, T_NONE, T_NONE}},
    {"IN"   , IN   , {T_NUM,  T_REG,  T_NONE}},
    {"OUT"  , OUT  , {T_REG,  T_NUM,  T_NONE}},
    {"LDN"  , LDN  , {T_NUM,  T_REG,  T_NONE}},
    {"ADD"  , ADD  , {T_REG,  T_REG,  T_REG }},
    {"SUB"  , SUB  , {T_REG,  T_REG,  T_REG }},
    {"MUL"  , MUL  , {T_REG,  T_REG,  T_REG }},
    {"DIV"  , DIV  , {T_REG,  T_REG,  T_REG }},
    {"ADDN" , ADDN , {T_REG,  T_NUM,  T_REG }},
    {"SUBN" , SUBN , {T_REG,  T_NUM,  T_REG }},
    {"MULN" , MULN , {T_REG,  T_NUM,  T_REG }},
    {"DIVN" , DIVN , {T_REG,  T_NUM,  T_REG }},
    {"JMP"  , JMP  , {T_NUM,  T_NONE, T_NONE}},
    {"JMPZ" , JMPZ , {T_NUM,  T_NONE, T_NONE}},
    {"JMPNZ", JMPNZ, {T_NUM,  T_NONE, T_NONE}},
    {"JMPP" , JMPP , {T_NUM,  T_NONE, T_NONE}},
    {"JMPN" , JMPN , {T_NUM,  T_NONE, T_NONE}},
    {"LOAD" , LOAD , {T_REG,  T_REG,  T_NONE}},
    {"LOADB", LOADB, {T_REG,  T_REG,  T_REG }},
    {"STOR" , STOR , {T_REG,  T_REG,  T_NONE}},
    {"STORB", STORB, {T_REG,  T_REG,  T_REG }},
    {"PUSH" , PUSH , {T_REG,  T_NONE, T_NONE}},
    {"POP"  , POP  , {T_REG,  T_NONE, T_NONE}},
    {"JSR"  , JSR  , {T_NUM,  T_NONE, T_NONE}},
    {"RET"  , RET  , {T_NONE, T_NONE, T_NONE}},
    {"DC"   , DC   , {T_NUM,  T_NONE, T_NONE}},
    {"DS"   , DS   , {T_NUM,  T_NONE, T_NONE}},
    {"DB"   , DB   , {T_STR,  T_NONE, T_NONE}},
};

#define NUM_OPCODES sizeof(TheInstructionSet)/sizeof(struct instruction)

/** Symbol table entry: */
struct symrec
{
       char *name;  /** name of symbol              */
       int value;   /** associated value */
       struct symrec *next;    /** link field    */
};

typedef struct symrec symrec;

/** The symbol table: a chain of `struct symrec'.  */
symrec *sym_table = (symrec *)0;
     
/** append an entry to the symbol table */
static symrec *putsym(char *sym_name, int sym_val)
{
       symrec *ptr;
       ptr = (symrec *) malloc (sizeof(symrec));
       ptr->name = (char *) malloc (strlen(sym_name)+1);
       strcpy (ptr->name,sym_name);
       ptr->value = sym_val;
       ptr->next = (struct symrec *)sym_table;
       sym_table = ptr;
       return ptr;
}

/** lookup a symbol by name */
static symrec *getsym(char *sym_name)
{
       symrec *ptr;
       for (ptr = sym_table; ptr != (symrec *) 0;
            ptr = (symrec *)ptr->next)
         if (strcmp (ptr->name,sym_name) == 0)
           return ptr;
       return NULL;
}

static void init_table (void)  /** puts any predefined constants in table. */
{
       int i;
       symrec *ptr;
       ptr = putsym("_start", 0);
}

static struct instruction *parse_instruction(char *s)
{
    int i, len;
    struct instruction *instr;
    for(i = 0; i < NUM_OPCODES; i++)
    {
        instr = &TheInstructionSet[i];
        len = strlen(instr->Name);
        if(!strncmp(s, instr->Name, len) && (s[len] == '\0' || s[len] == ' ' || s[len] == '\t'))
            return instr;
    }
    return NULL;
}

static char* parse_reg(char *s, int *num)
{
    int regnum;
    char *sep;

    for(sep = s; *sep != '\0' && *sep != ',' && *sep != ' ' && *sep != '\t'; ++sep)
        ;
    if(*sep != '\0')
        *sep++ = '\0';

    if(*s == 'R')
    {
        ++s;
        if(sscanf(s, "%d", &regnum) == 1)
        {
            if(regnum >= 0 && regnum < NUM_REGISTERS)
            {
                *num = regnum;
                return sep;
            }
        }
    }
    return NULL;
}

static char *parse_num(char *s, int *num, int pass)
{
    symrec *sym;
    int number;
    char *sep;

    for(sep = s; *sep != '\0' && *sep != ',' && *sep != ' ' && *sep != '\t'; ++sep)
        ;
    if(*sep != '\0')
        *sep++ = '\0';


    if(sscanf(s, "%d", &number) == 1)
    {
        *num = number;
        return sep;
    }
    else if(pass == 1)
    {
        return sep;
    }
    else
    {
        if((sym = getsym(s)) != NULL)
        {
            *num = sym->value;
            return sep;
        }
        else
        {
             printf("Unknown symbol: '%s' ", s);
        }
    }
    return NULL;
}

static char *parse_string(char *s, int *len)
{
    int number;
    char *sep, *stor, *ret;

    for(sep = s; *sep != '\0' && *sep != '"'; ++sep)
        ;
    if(*sep == '\0')
        return(NULL);
    else
        ++sep;

    ret = stor = sep;
    for(number = 0; *sep != '"' && *sep != '\0'; number++)
    {
        if(*sep == '\\')
	{ /** escape char: replace it by actual code */
	    switch(*(sep + 1))
	    {
	      case '\0':
		return NULL; /** error: unterminated string */
		break;
	      case 'n':
		*stor++ = '\n';
		sep += 2;
		break;
	      case 'r':
		*stor++ = '\r';
		sep += 2;
		break;
	      case 't':
		*stor++ = '\t';
		sep += 2;
		break;
	      case 'b':
		*stor++ = '\b';
		sep += 2;
		break;
	      case '\\':
		*stor++ = '\\';
		sep += 2;
		break;
	      case '"':
		*stor++ = '"';
		sep += 2;
		break;
	      default:
		*stor++ = *(sep + 1);
		sep += 2;
		break;
	    }
	}
	else
	{
	    *stor++ = *sep++;
	}
    }
    if(*sep == '"')
    {
	*stor++ = '\0';
	*len = number;
	return ret;
    }
    return NULL;
}


static char *extract_label(char*s)
{
    char *sep, *blank, *tab;;

    sep   = strchr(s, ':');
    blank = strchr(s, ' ');
    tab   = strchr(s, '\t');

    if(sep) /** ":" in text */
    {   /** Check if ":" is preceeded by " " or "\t" */
        if(blank && sep > blank)
            return NULL; 
        else if(tab && sep > tab)
            return NULL;
        else
            return sep; /** sep points to ":" */
    }
    return NULL;
}


static int assemble(FILE *in, FILE* out, int pass)
{
     int location = 0;
     int linecount = 0;
     int errorcount = 0;
     int len, i, num;
     char *s, *sep;
     struct instruction *instr;
     char linebuffer[512];
     /** read line by line */
     while(fgets(linebuffer, 512, in) != NULL)
     {
         ++linecount;
         /** Skip empty lines */
         if((len = strlen(linebuffer)) == 0)
             continue;
         /** Delete linefeed at end of line */
         if(linebuffer[len-1] == '\n')
         {
             linebuffer[len-1] = '\0';
             if(--len == 0)
                continue;
         }
         /** Skip leading blanks */
         for(s = linebuffer; *s == ' ' || *s == '\t'; s++)
             ;
         if(sep = strchr(s, ';'))
         {
             *sep = '\0'; /** Separate comment (begins with ";") from rest */
         }
         if((len = strlen(s)) == 0)
             continue; /** Nothing left -> next line */

         /** Does line start with a label? */
         if((sep = extract_label(s)))
         {
             *sep = '\0'; /** Separate label from rest (overwrite ":") */
             if(pass == 1)
                 putsym(s, location); /** put label into symbol table */
             /** continue after ":"  */
             /** Skip leading blanks */
             for(s = sep + 1; *s == ' ' || *s == '\t'; s++)
                 ;
             /** if remainder of line is empty: read next line */
             if((len = strlen(s)) == 0)
                 continue;
         }

         /** After all this fuss, the assembly language instruction is now ready
	    for parsing: Look it up in table */
         if((instr = parse_instruction(s)) == NULL)
         {
             printf("Line %d: Unknown assembly instruction!\n", linecount);
             ++errorcount;
             continue;
         }
         if(instr->Opcode < NUM_INSTRUCTIONS)
         {   /** Do not issue pseudo instructions */
             if(pass == 2) /** Write opcode into output file */
                 fwrite(&instr->Opcode, sizeof instr->Opcode, 1, out);
             location += sizeof(int);
         }

         /** point to first operand */
         for(s += strlen(instr->Name); *s == ' ' || *s == '\t'; ++s)
             ;
         /** analyse operand/write to output file */
         for(i = 0; i < 3; i++)
         {
             /** Skip possible blanks */
             for(; *s == ' ' || *s == '\t'; ++s)
                 ;
             switch(instr->op[i])
             {
                 case T_NONE:
                     if(*s != '\0')
                     {  /** Error: */
                         printf("Line %d: unexpected operand!\n", linecount);
                         ++errorcount;
                     }
                     break;
                 case T_NUM:
                     if((sep = parse_num(s, &num, pass)) == NULL)
                     {  /** Error: */
                         printf("Line %d: operand #%d unsuitable!\n", linecount, i+1);
                         ++errorcount;
                         *s = '\0';
                     }
                     else
                     {
                         if(instr->Opcode < NUM_INSTRUCTIONS)
                         {
                             if(pass == 2) /** write operand to output file */
                                 fwrite(&num, sizeof num, 1, out);
                             location += sizeof(int);
                         }
                         else if(instr->Opcode == DC)
                         {   /** DC = define constant */
                             if(pass == 2) /** write operand to output file */
                                 fwrite(&num, sizeof num, 1, out);
                             location += sizeof(int);
                         }
                         else if(instr->Opcode == DS)
                         {   /** DS = Define Storage */
                             int j, null = 0;
                             for(j = 0; j < num; j++)
                             {
                                 if(pass == 2) /** write operand to output file */
                                     fwrite(&null, sizeof null, 1, out);
                                 location += sizeof(int);
                             }
                         }
                         /** point to next operand */
                         s = sep;
                     }
                     break;
                 case T_REG:
                     if((sep = parse_reg(s, &num)) == NULL)
                     {  /** Error: */
                         printf("Line %d: operand #%d unsuitable !\n", linecount, i+1);
                         ++errorcount;
                         *s = '\0';
                     }
                     else
                     {
                         if(pass == 2) /** write operand to output file */
                             fwrite(&num, sizeof num, 1, out);
                         location += sizeof(int);
                         /** point to next operand */
                         s = sep;
                     }
                     break;
                 case T_STR:
                     if((sep = parse_string(s, &num)) == NULL)
                     {  /** Error: */
                         printf("Line %d: Bad string !\n", linecount);
                         ++errorcount;
                         *s = '\0';
                     }
                     else if(instr->Opcode == DB)
                     {   /** DB = define string */
			while(num >= sizeof(int))
			{
			    if(pass == 2) /** write operand to output file */
			    {
                                fwrite(sep, sizeof(int), 1, out);
			    }
			    num -= sizeof(int);
			    sep += sizeof(int);
                            location += sizeof(int);
			}
			len = sizeof(int) - num;
			while(num > 0)
			{
			    if(pass == 2) /** write operand to output file */
			    {
                                fwrite(sep, 1, 1, out);
			    }
			    num--;
			    sep++;
			}
			while(len > 0)
			{
			    if(pass == 2) /** fill remainder with zeroes */
			    {
                                fputc(0, out);
			    }
			    len--;
			}
                        location += sizeof(int);
                        *s = '\0';
                     }
                     break;
                 default:
                     printf("Line %d: internal error!\n", linecount);
                     exit(1);
                     break;
             }
         }
     }
     return errorcount;
}


main (int argc, char *argv[])
{
  char *infile, *outfile;
  FILE *in, *out;
  unsigned int len;
  if (argc < 2)
  {
      printf ("Usage: %s <input file> [<output file>]\n", argv[0]);
      exit(1);
  }
  else
  {
      infile = argv[1];
  }
  if (argc == 3)
      outfile = argv[2];
  else
      outfile = "out.exe";

  in  = fopen (infile, "r");
  out = fopen (outfile, "w");
  if(in == NULL)
  {
    printf("Cannot open file '%s'\n", infile);
    exit(1);
  }
  if(out == NULL)
  {
    printf("Cannot create file '%s'\n", outfile);
    exit(1);
  }

  init_table();

  /** Pass #1: Build symbol table */
  if(assemble(in, NULL, 1) != 0)
  {
      printf("Error during assmbly\n");
      exit(1);
  }

  rewind(in);

  /** Pass #2: generate code */
  if(assemble(in, out, 2) != 0)
  {
      printf("Assembly error in Pass #2\n");
      exit(1);
  }
  return(0);
}
