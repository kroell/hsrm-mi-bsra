/**
 *  @file       jmp.c
 *  @brief      setjmp/longjmp imlementation
 *
 * This file provides a simple implementation of the C-standard
 * setjmp()/longjmp() functions. The reason not to use the functions
 * provided by the C environment is because we need additional knowledge
 * about the jmp_buf structure: we need to manipulate a jmp_buf's stack
 * and instruction pointer in order to be able to create new threads.
 *
 */

#include <assert.h>
#include "../include/jmp.h"


#if defined ( __i386__ )
/**
 * This is the intel x86 implementation
 */


/**
 * @brief Save caller's context
 *
 * Saves all callee-saved registers in a structure.
 *
 * @param env jmp_buf structure to save the context in
 * @retval 0 returning directly
 * @retval nonzero returning via longjmp()
 */
static void __attribute__((unused)) __setjmp__dummy(__jmp_buf env)
{
    __asm__(
#ifndef __APPLE__			/* Apples uses different asm->C label mapping */
"	.globl	__setjmp\n"
"__setjmp: movl    4(%%esp),%%eax\n"	/* get argument: jmp_buf pointer */
#else
"	.globl	___setjmp\n"
"___setjmp: movl    4(%%esp),%%eax\n"	/* get argument: jmp_buf pointer */
#endif
        "movl    %%ebx,0(%%eax)\n"	/* store registers */
        "movl    %%esi,4(%%eax)\n"
        "movl    %%edi,8(%%eax)\n"
        "movl    %%ebp,12(%%eax)\n"
        "movl    0(%%esp),%%ebx\n"	/* get return address */
        "movl    %%ebx,20(%%eax)\n"	/* store it */
        "leal    4(%%esp),%%ebx\n"	/* get the stack pointer */
        "movl    %%ebx,16(%%eax)\n"	/* store it */
        "movl    0(%%eax),%%ebx\n"	/* restore ebx to orig. value */
        "xorl    %%eax,%%eax\n"		/* return zero */
        "ret\n"
    ::);
}

/**
 * @brief Restore a context previously saved with setjmp()
 *
 * @param  env jmp_buf context to be restored
 * @param  val value to be returned to caller of setjmp()
 * @retval none
 */
void __attribute__((noreturn)) __longjmp(__jmp_buf env, int val)
{
    __asm__(
        "movl    %0,%%ecx\n"		/* get pointer to jmp_buf */
        "movl    %1,%%eax\n"		/* get return value */
        "movl    0(%%ecx),%%ebx\n"	/* restore registers */
        "movl    4(%%ecx),%%esi\n"
        "movl    8(%%ecx),%%edi\n"
        "movl    12(%%ecx),%%ebp\n"
        "movl    16(%%ecx),%%esp\n"
        "jmp     *(20)(%%ecx)\n"	/* jump to return address */
    :: "m" (env), "m" (val));
    assert(0);
}

#elif defined ( __x86_64 )

/**
 * This is the same as above but for AMD64
 */

static void __attribute__((unused)) __setjmp__dummy(__jmp_buf env)
{
    __asm__(
#ifndef __APPLE__			/* Apples uses different asm->C label mapping */
"    .globl	__setjmp\n"
"__setjmp: "
#else
    ".globl	___setjmp\n"
"___setjmp:"	
#endif
"     movq    0(%%rsp),%%rdx\n"            /* ger return address from 
stack */
"     movq    %%rdx, 0(%%rdi)\n"           /* store return address */
"     movq    %%rbx, 8(%%rdi)\n"           /* 1; rbx */
"     movq    %%rsp,16(%%rdi)\n"           /* 2; rsp */
"     movq    %%rbp,24(%%rdi)\n"           /* 3; rbp */
"     movq    %%r12,32(%%rdi)\n"           /* 4; r12 */
"     movq    %%r13,40(%%rdi)\n"           /* 5; r13 */
"     movq    %%r14,48(%%rdi)\n"           /* 6; r14 */
"     movq    %%r15,56(%%rdi)\n"           /* 7; r15 */
"     xorq    %%rax,%%rax\n"
"     ret\n"
    ::);
}

void __attribute__((unused)) __longjmp_dummy(__jmp_buf env, int val)
{
    __asm__(
#ifndef __APPLE__			/* Apples uses different asm->C label mapping */
"    .globl	__longjmp\n"
"__longjmp: "
#else
"   .globl	___longjmp\n"
"___longjmp:"	
#endif
"     movq    %%rdi,%%rax\n"              /* get return value */
"     movq    0(%%rdi),%%rcx\n"           /* return address */
"     movq    8(%%rdi),%%rbx\n"           /* 1; rbx */
"     movq    16(%%rdi),%%rsp\n"          /* 2; rsp */
"     movq    24(%%rdi),%%rbp\n"          /* 3; rbp */
"     movq    32(%%rdi),%%r12\n"          /* 4; r12 */
"     movq    40(%%rdi),%%r13\n"          /* 5; r13 */
"     movq    48(%%rdi),%%r14\n"          /* 6; r14 */
"     movq    56(%%rdi),%%r15\n"          /* 7; r15 */
"     movq    %%rcx,0(%%rsp)\n"           /* overwrite return address */
"     ret\n"                              /* ... and jump to it */
    ::);
}
#else  /* ifdef architecture */
 
#error "Jmp.c must be adapted to this architecture"

#endif

