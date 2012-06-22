#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include "libmemprobe/include/memprobe.h"
#define  MEMSIZE 10

int main(void); /* need handle in is_code */

static unsigned long long page_size, max_page, max_address;
static char *heap;  /* heap=malloc(sizeof(char)*MEMSIZE) */
static char data[MEMSIZE];

static char *string1  = "Hallo Welt";
static char string2[] = "Hallo Welt";


/* These are separate just for convenience */
static inline int is_code(long long from, long long to)
{
    return from <= (long long)&main && (long long)&main < to;
}

static inline int is_data(long long from, long long to)
{
    return from <= (long long)&data && (long long)&data < to;
}

static inline int is_heap(long long from, long long to)
{
    return from <= (long long)&heap && (long long)&heap < to;
}

static inline int is_stck(long long from, long long to, void *frame)
{
    return from <= (long long)frame && (long long)frame < to;
}

/* Format access codes nicely and try to make an educated guess regarding memory-region */
static void access_output(int acc, long long from, long long to, void *stack_frame)
{
    if (acc == 1)      /*  read  only  */
    {
	if (is_code(from, to))
	    printf("(CODE) ");
	else
	    printf("(UNKN) ");

	printf("read access from \t \t 0x%08llx to 0x%08llx\n", from, to);

    }
    else if (acc == 2) /* read & write */
    {
	if (is_data(from, to))
	    printf("(DATA) ");
	else if (is_heap(from, to))
            printf("(HEAP) ");
	else if (is_stck(from, to, stack_frame))
            printf("(STCK) ");
	else
	    printf("(UNKN) ");

        printf("read and write access from \t 0x%08llx to 0x%08llx\n", from, to);
    }
    return;
}

int main(void)
{
    volatile unsigned long long stack = 0;         /* must be first line */
    unsigned long long i, act_address, old_address, act_access, old_access;


    max_address = 1ULL << 32;
    page_size   = getpagesize();
    max_page    = max_address / page_size;
    heap        = malloc(sizeof(char)*MEMSIZE);
    act_access  = 0;

    printf("int \t \t \t \t \t %lu\n", sizeof(int));
    printf("void* \t \t \t \t \t %lu\n", sizeof(void*));
    printf("max_address \t \t \t \t %llx\n", max_address);
    printf("max_page \t \t \t \t %llu\n", max_page);
    printf("page_size \t \t \t \t %llu\n", page_size);

    printf("-----------------------------------------------------------------");
    printf("\n");

    for (i=0; i<max_page; ++i)
    {
        old_access  = act_access;
        act_address = i*page_size;
        act_access  = memprobe((void*)act_address);

	/* access code changed from 0 to 1 or from 1 to 2 */
	if (act_access != old_access && act_access != 0)
	{
	    access_output(act_access,old_address,act_address,((void*)&stack));
	    old_address = act_address;
	}
    }

    printf("-----------------------------------------------------------------");
    printf("\n");

    /* char *string = 'Hello World'; */
    access_output(memprobe((void*)&string1), \
		  (long long)&string1,       \
		  (long long)(&string1+9),  \
		  ((void*)&stack));

    /* char string[] = 'Hello World'; */
    access_output(memprobe((void*)&string2), \
		  (long long)&string2,       \
		  (long long)(&string2+9),  \
		  ((void*)&stack));

    free(heap);
    return 0;
}
