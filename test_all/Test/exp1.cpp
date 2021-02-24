#include <stdio.h>
#include <stdlib.h>
#include "exp1.h"

void swap(int &a, int &b)
{
	a ^= b; // a = a^b
	b ^= a; // b = b^(a^b)
	a ^= b; // a = (a^b)^(b^((a^b))
}

int SignReversal(int a)
{
	return ~a + 1;
}

int my_abs(int a)
{
	return (a^(a >> ((sizeof(int) << 3) - 1))) + 1;
}

int test_exp1()
{
	int a = 11, b = 12;
	printf("a=%d, b=%d\n", a, b);
	swap(a, b);
	printf("a=%d, b=%d\n", a, b);
	int c = -11;
	int d = -15;
	printf("%d\n%d", SignReversal(c), SignReversal(d));
	d = my_abs(d);
	printf("abs=%d\n", d);

	return 0;
}