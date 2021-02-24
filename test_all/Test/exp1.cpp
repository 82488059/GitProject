#include <stdio.h>
#include <stdlib.h>
#include "exp1.h"

int output()
{
	for (int i = 0; i < 100; ++i)  
		if (i & 1)  
			printf("%d ", i);  
	printf("\n");
	return 0;
}

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