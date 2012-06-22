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

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "moco-mem.h"

/**
** Memory Image:
*/
static int *TheMemory;
static int MemorySize;

/** check if address is valid in data memory */
int M_DataAccessWord(int Address)
{
    return(Address >= 0 && Address < MemorySize && (Address & 3) == 0);
}

/** check if address is valid in data memory for byte access */
int M_DataAccessByte(int Address)
{
    return(Address >= 0 && Address  < MemorySize);
}	

/** read a word from data memory */
int M_ReadDataWord(int Address)
{
    return TheMemory[Address / sizeof(int)];
}

/** read a byte from data memory */
int M_ReadDataByte(int Address)
{
    int shift = 8 * (Address % sizeof(int));
    return (TheMemory[Address / sizeof(int)] >> shift) & 0xff;
}

/** write a word to data memory */
void M_WriteDataWord(int Address, int Data)
{
    TheMemory[Address / sizeof(int)] = Data;
}

/** write a byte to data memory */
void M_WriteDataByte(int Address, char Data)
{
    int shift = 8 * (Address % sizeof(int));
    int mask  = ~0 ^ (0xff << shift);
    int data  = TheMemory[Address / sizeof(int)];
    data = (data & mask) | (Data << shift);
    TheMemory[Address / sizeof(int)] = data;
}

/** Check if address is valid in program memory */
int M_ProgramAccessWord(int Address)
{   /* why not use last two bits? */
    return(Address >= 0 && Address < MemorySize && (Address & 3) == 0);
}

/** read a word from program memory */
int M_ReadProgramWord(int Address)
{
    return TheMemory[Address / sizeof(int)];
}

/** create a new address space */
int M_CreateSpace(char *filename, unsigned int size)
{
    int fd;
    if((fd = open(filename, O_RDONLY)) < 0)
    {
        perror("open");
        return -1;
    }

    TheMemory = (int*)malloc(size+4);
    MemorySize = size;

    if(read(fd, TheMemory, size) < size)
    {
        perror("read");
        return -1;
    }
    close(fd);
    return 0;
}

/** free the address space (why parameterized?) */
void M_DeleteSpace(int ID) 
{
    free(TheMemory);
    TheMemory = NULL;
}
