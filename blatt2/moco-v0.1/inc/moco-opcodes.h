/**
 ** MoCo -- Model Computer
 **
 **
 ** Prof. Dr. Robert Kaiser
 ** Hochschule RheinMain, DCSM
 **
 ** Opcodes supported by the MoCo computer
 */

enum opcode
{
    HALT    = 0,
    IN      = 1,
    OUT     = 2,
    LDN     = 3,
    ADD     = 4,
    SUB     = 5,
    MUL     = 6,
    DIV     = 7,
    ADDN    = 8,
    SUBN    = 9,
    MULN    = 10,
    DIVN    = 11,
    JMP     = 12,
    JMPZ    = 13,
    JMPNZ   = 14,
    JMPP    = 15,
    JMPN    = 16,
    LOAD    = 17,
    LOADB   = 18,
    STOR    = 19,
    STORB   = 20,
    PUSH    = 21,
    POP     = 22,
    JSR     = 23,
    RET     = 24,
    NUM_INSTRUCTIONS = 25,
};
