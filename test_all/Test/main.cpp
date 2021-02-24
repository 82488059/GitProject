#include <stdio.h>
#include <stdlib.h>

#include "main.h"
#include <process.h>  
#include <windows.h>

// #include"threadProc.h"

struct ThreadNum{
	int num;
};

// 
// int main()
// {
// 	
// 	getchar();
// 	return 0;
// }
// 


#if 0
int main4()
{
	enum{MAXHANDLE = 10};
	HANDLE handle[MAXHANDLE];

	struct ThreadNum nIndexThread[10];
	
	for (int i = 0; i < MAXHANDLE; ++i)
	{
		nIndexThread[i].num = i+1;
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadNumOff, (void*)&nIndexThread[i], 0, NULL);
	}

	WaitForMultipleObjects(MAXHANDLE, handle, TRUE, INFINITE);

	for (int i = 0; i < MAXHANDLE; ++i)
	{
		CloseHandle(handle[i]);
	}

	getchar();
	return 0;
}
#endif
#if 0
int main3()
{
	enum{MAXHANDLE = 10};
	HANDLE handle[MAXHANDLE];

	for (int i = 0; i < MAXHANDLE; ++i)
	{
		handle[i] = (HANDLE)_beginthreadex(NULL, 0, ThreadNumOff, NULL, 0, NULL);
	}

	WaitForMultipleObjects(MAXHANDLE, handle, TRUE, INFINITE);

	for (int i = 0; i < MAXHANDLE; ++i)
	{
		CloseHandle(handle[i]);
	}

	getchar();
	return 0;
}
#endif



#if 0
int main2()
{
	enum{MAXHANDLE = 2};
	// 	HANDLE handle[MAXHANDLE];
	// 	handle[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, NULL);
	// 	handle[1] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, NULL);
	// 	WaitForMultipleObjects(MAXHANDLE, handle, TRUE, INFINITE);
	// 	_endthreadex


	HANDLE handle1 = CreateThread(NULL, 0, ThreadFun, NULL, 0, NULL); 
	HANDLE handle2 = CreateThread(NULL, 0, ThreadFun, NULL, 0, NULL); 

	WaitForSingleObject(handle1, INFINITE);
	WaitForSingleObject(handle2, INFINITE);
	CloseHandle(handle1);
	CloseHandle(handle2);

	getchar();
	return 0;
}
#endif



#if 0
int main1()
{
	output();
	int a=11,b=12;
	printf("a=%d, b=%d\n", a, b);
	swap(a, b);
	printf("a=%d, b=%d\n", a, b);

	int c = -11;
	int d = -15;
	printf("%d\n%d", SignReversal(c), SignReversal(d));

	d = my_abs(d);
	
	getchar();
	return 0;
}
#endif
