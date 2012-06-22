#include <stdio.h>
#include <setjmp.h>

static jmp_buf main_context;
static const int rek_depth = 5;

void rek(int i)
{
    if (rek_depth < ++i)
	longjmp(main_context, 1);
    printf("Rekursionstiefe %d von %d\n", i, rek_depth);
    rek(i);
}

int main(void)
{
    if (!setjmp(main_context))
	rek(0);
    printf("Rekursion beendet.\n");
    return 0;
}
