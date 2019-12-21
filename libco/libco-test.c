#include <stdlib.h>
#include <stdio.h>
#include "libco.h"

int fail = 0;
int step = 0;
int finish = 0;

cothread_t t_main;
cothread_t t_sub;

void inner()
{
	int n = 123;
	
	step++;
	printf("In child thread, step %d\n", step);
	n++;
	co_switch(t_main);
	if (n != 124) fail = 1;
	
	step++;
	printf("In child thread, step %d\n", step);
	n++;
	co_switch(t_main);
	if (n != 125) fail = 1;
}

void inner2()
{
	int n = 1234;
	
	step++;
	printf("In child thread, step %d\n", step);
	n++;
	co_switch(t_main);
	if (n != 1235) fail = 1;
	
	step++;
	printf("In child thread, step %d\n", step);
	n++;
	co_switch(t_main);
	if (n != 1236) fail = 1;
}

void outer()
{
	int n = 12345;
	
	inner();
	if (n != 12345) fail = 1;
	n++;
	
	inner2();
	if (n != 12346) fail = 1;
	n++;
	
	inner();
	if (n != 12347) fail = 1;
	n++;
	
	finish = 1;
	step++;
	printf("In child thread, step %d\n", step);
	co_switch(t_main);
	puts("Called child thread too many times");
	exit(1);
}

int main()
{
	int i;
	
	t_main = co_active();
	t_sub = co_create(sizeof(void*) * 65536, outer);
	
	for (i=0; i<7; i++)
	{
		printf("In main thread, step %d\n", i+1);
		co_switch(t_sub);
		if (step != i+1) fail = 1;
	}
	co_delete(t_sub);
	
	if (!finish) fail = 1;
	if (fail) puts("Tests failed");
	
	return fail;
}
