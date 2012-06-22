#include <unistd.h>
#include <stdio.h>
#include <limits.h>
#include "libmemprobe/include/memprobe.h"

int main(void)
{
    unsigned int i, act_access, old_access, old_address, \
                 act_address, page_size, max_page;
    unsigned long long max_address;

    max_address = (1ULL << 32); /* 1 << 32 would cause integer overflow */
    page_size   = getpagesize();
    max_page    = max_address / page_size;

    printf("int \t \t \t \t %lu\n", sizeof(int));
    printf("void* \t \t \t \t %lu\n", sizeof(void*));
    printf("max_address \t \t \t %llu\n", max_address);
    printf("max_page \t \t \t %u\n", max_page);
    printf("page_size \t \t \t %d\n", page_size);
    printf("-------------------------------------------\n");

    for (i=0; i<max_page; ++i)
    {
        old_access  = act_access;
        act_address = i*page_size;
        act_access  = memprobe((void*)act_address);

	if ( act_access != old_access )
	{
	    access_output(act_access,old_address,act_address);
	    old_address = act_address;
	}
    }

    return 0;
}

void access_output(int acc, int from, int to)
{
    if (acc == 1)
        printf("read access from \t \t 0x%08x to 0x%08x\n", from, to);
    else if (acc == 2)
        printf("read and write access from \t 0x%08x to 0x%08x\n", from, to);
    return;
}
