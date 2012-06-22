#include <stdio.h>
#include <assert.h>
#include "include/uth.h"

static volatile int run[5];


static void sayhello(const char *name)
{
	printf("Hallo Welt, hier ist der Thread \"%s\". Mein Handle ist: %p\n", name, uth_get_handle());
}

static void saygoodbye(const char *name)
{
	printf("Hier ist der Thread \"%s\". Beende mich jetzt...\n", name);
}

void thread1(void)
{
	sayhello("1");
	while(run[1])
	{
		printf("1 blockiert jetzt\n");
		uth_thread_next(0);
		printf("1 wieder aktiv\n");
	}
	saygoodbye("1");
#ifndef UTH_AUTO_TERMINATE
	uth_suicide();
	assert(0);
#endif
}

void thread2(void)
{
	sayhello("2");
	while(run[2])
	{
		printf("2 blockiert jetzt\n");
		uth_thread_next(0);
		printf("2 wieder aktiv\n");
	}
	saygoodbye("2");
#ifndef UTH_AUTO_TERMINATE
	uth_suicide();
	assert(0);
#endif
}

void thread3(void)
{
	sayhello("3");
	while(run[3])
	{
		printf("3 blockiert jetzt\n");
		uth_thread_next(0);
		printf("3 wieder aktiv\n");
	}
	saygoodbye("3");
#ifndef UTH_AUTO_TERMINATE
	uth_suicide();
	assert(0);
#endif
}

void thread4(void)
{
	sayhello("4");
	while(run[4])
	{
		printf("4 blockiert jetzt\n");
		uth_thread_next(0);
		printf("4 wieder aktiv\n");
	}
	saygoodbye("4");
#ifndef UTH_AUTO_TERMINATE
	uth_suicide();
	assert(0);
#endif
}


void init_thread(void)
{
	void *handle[5];
	int i;
	sayhello("Init");
	for(i = 0; i < 5; i++)
	{
		run[i] = 1;
	}
	printf("Init: Blockiere ein paar mal... noch gibt es keine anderen Threads\n");
	for(i = 0; i < 5; i++)
		uth_thread_next(0);
	printf("Init: Erzeuge Threads 1 bis 3\n");
	handle[1] = uth_thread_create(thread1);
	handle[2] = uth_thread_create(thread2);
	handle[3] = uth_thread_create(thread3);
	for(i = 1; i <= 3; i++)
		printf("Thread %d: Handle %p\n", i, handle[i]);
	printf("Init: Blockiere ein paar mal... Threads 1 bis 3 sollten laufen\n");
	for(i = 0; i < 5; i++)
		uth_thread_next(0);
	printf("Init: Erzeuge Thread 4\n");
	handle[4] = uth_thread_create(thread4);
	printf("Init: Blockiere ein paar mal... Alle Threads sollten laufen\n");
	for(i = 0; i < 5; i++)
		uth_thread_next(0);
	printf("Init: Lasse Threads 3 und 4 terminieren\n");
	for(i = 3; i < 5; i++)
		run[i] = 0;
	printf("Init: Blockiere ein paar mal... Threads 1 und 2 sollten laufen, 3 und 4 sollten terminieren\n");
	for(i = 0; i < 5; i++)
		uth_thread_next(0);
	printf("Init: Beende Threads 1 und 2\n");
	for(i = 1; i < 3; i++)
		uth_thread_terminate(handle[i]);
	printf("Init: Blockiere ein paar mal... es gibt keine anderen Threads mehr\n");
	for(i = 0; i < 5; i++)
		uth_thread_next(0);
	printf("Init: Beende mich selbst -> Terminiere Thread-System\n");
#ifndef UTH_AUTO_TERMINATE
	uth_suicide();
	assert(0);
#endif
}

main()
{
	printf("Main: Starte ersten Thread!\n");
	uth_init(5, init_thread);
	printf("Main: Thread-System beendet!\n");
}
