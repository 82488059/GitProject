#include <iostream>
#include <list>
#include "test_list_sort.h"
#include "exp1.h"


int main()
{
	test_exp1();
	return 0;
}
 



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






