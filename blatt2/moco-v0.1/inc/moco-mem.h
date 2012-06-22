/**
 ** MoCo -- Model Computer
 **
 ** MoCo Memory interface
 **
 ** Prof. Dr. Robert Kaiser
 ** Hochschule RheinMain, DCSM
 **
 **
 */

/** create a new address space */
extern int M_CreateSpace(char *filename, unsigned int size);

/** delete an address space */
extern void M_DeleteSpace(int ID);

/** check if address is valid in data memory */
extern int M_DataAccessWord(int Address);

/** check if address is valid in data memory for byte access */
extern int M_DataAccessByte(int Address);

/** read a word from data memory */
extern int M_ReadDataWord(int Address);

/** read a byte from data memory */
extern int M_ReadDataByte(int Address);

/** write a word to data memory */
extern void M_WriteDataWord(int Address, int Data);

/** write a byte to data memory */
extern void M_WriteDataByte(int Address, char Data);

/** check if address is valid in stack memory */
extern int M_StackAccessWord(int Address);

/** read a word from data memory */
extern int M_ReadStackWord(int Address);

/** write a word to stack memory */
extern void M_WriteStackWord(int Address, int Data);

/** Check if address is valid in program memory */
extern int M_ProgramAccessWord(int Address);

/** read a word from program memory */
extern int M_ReadProgramWord(int Address);

