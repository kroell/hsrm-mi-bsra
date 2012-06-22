#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
	printf("usage: ./assert-demo <number>\n");
	return 1;
    }

    /* should always be true */
    assert(atoi(argv[1]) - atoi(argv[1]) == 0);
    printf("indeed %s - %s == 0 !\n",argv[1],argv[1]);

    /* should always be false */
    assert(atoi(argv[1]) - (atoi(argv[1])-1) == 0);
    printf("wow    %s - %d == 0 !\n",argv[1],atoi(argv[1])-1);
    printf("Try compiling without -DNDEBUG..\n");

    return 0;
}
