#include <process.h>
#include <Windows.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>

#if 0

struct ThreadNum{
	char num;
};

int const LOOP = 10;
HANDLE g_hThreadEvent[3];


unsigned int __stdcall ThreadPrint(LPVOID);
unsigned int __stdcall ThreadPrintA(LPVOID);
unsigned int __stdcall ThreadPrintB(LPVOID);
unsigned int __stdcall ThreadPrintC(LPVOID);

int main(_In_ int _Argc, char* Argv[])
{
	struct ThreadNum TNum[3];
	TNum[0].num = 'A';
	TNum[1].num = 'B';
	TNum[2].num = 'C';

	g_hThreadEvent[0] = CreateEvent(NULL, FALSE, TRUE, NULL);
	g_hThreadEvent[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hThreadEvent[2] = CreateEvent(NULL, FALSE, FALSE, NULL);

	enum{MAXHANDLE = 3};
	HANDLE handle[MAXHANDLE];

	handle[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadPrint, (LPVOID)&TNum[0], 0, NULL);
	handle[1] = (HANDLE)_beginthreadex(NULL, 0, ThreadPrint, (LPVOID)&TNum[1], 0, NULL);
	handle[2] = (HANDLE)_beginthreadex(NULL, 0, ThreadPrint, (LPVOID)&TNum[2], 0, NULL);



	WaitForMultipleObjects(MAXHANDLE, handle, TRUE, INFINITE);

	for (int i = 0; i < MAXHANDLE; ++i)
	{
		CloseHandle(handle[i]);
	}
// 	g_hThreadEvent[0] = CreateEvent(NULL, FALSE, TRUE, NULL);
// 	g_hThreadEvent[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
// 	g_hThreadEvent[2] = CreateEvent(NULL, FALSE, FALSE, NULL);
// 	
// 	enum{MAXHANDLE = 3};
// 	HANDLE handle[MAXHANDLE];
// 
// 	handle[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadPrintA, NULL, 0, NULL);
// 	handle[1] = (HANDLE)_beginthreadex(NULL, 0, ThreadPrintB, NULL, 0, NULL);
// 	handle[2] = (HANDLE)_beginthreadex(NULL, 0, ThreadPrintC, NULL, 0, NULL);
// 
// 	WaitForMultipleObjects(MAXHANDLE, handle, TRUE, INFINITE);
// 
// 	for (int i = 0; i < MAXHANDLE; ++i)
// 	{
// 		CloseHandle(handle[i]);
// 	}

	getchar();
	return 0;
}

unsigned int __stdcall ThreadPrint(LPVOID p)
{
	for (int i = 0 ; i < LOOP; ++i)
	{
		WaitForSingleObject(g_hThreadEvent[((ThreadNum*)p)->num - 'A'], INFINITE);

		printf("%c, 线程ID%5d\n", ((ThreadNum*)p)->num, GetCurrentThreadId());

		SetEvent(g_hThreadEvent[(((ThreadNum*)p)->num - 'A' + 1)%3]);
	}

	_endthreadex(0);
	return 0;
}

unsigned int __stdcall ThreadPrintA(LPVOID)
{

	for (int i = 0 ; i < LOOP; ++i)
	{
		WaitForSingleObject(g_hThreadEvent[0], INFINITE);

		printf("A, 线程ID%5d\n", GetCurrentThreadId());

		SetEvent(g_hThreadEvent[1]);
	}

	_endthreadex(0);
	return 0;
}
unsigned int __stdcall ThreadPrintB(LPVOID)
{

	for (int i = 0 ; i < LOOP; ++i)
	{
		WaitForSingleObject(g_hThreadEvent[1], INFINITE);

		printf("B, 线程ID%5d\n", GetCurrentThreadId());

		SetEvent(g_hThreadEvent[2]);
	}

	_endthreadex(0);
	return 0;
}
unsigned int __stdcall ThreadPrintC(LPVOID)
{

	for (int i = 0 ; i < LOOP; ++i)
	{
		WaitForSingleObject(g_hThreadEvent[2], INFINITE);

		printf("C, 线程ID%5d\n", GetCurrentThreadId());

		SetEvent(g_hThreadEvent[0]);
	}

	_endthreadex(0);
	return 0;
}

#endif


#if 0
// 事件


int const LOOP = 100;
HANDLE g_hThreadEvent1;
HANDLE g_hThreadEvent2;

unsigned int __stdcall ThreadLoop(LPVOID);

int main(_In_ int _Argc, char* Argv[])
{
	g_hThreadEvent1 = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hThreadEvent2 = CreateEvent(NULL, FALSE, TRUE, NULL);


	HANDLE handle1 = (HANDLE)_beginthreadex(NULL, 0, ThreadLoop, NULL, 0, NULL);
	
	for (int i = 0; i < LOOP; ++i)
	{
	    WaitForSingleObject(g_hThreadEvent1, INFINITE);

		for (int j = 0; j < 5; ++j)
		{
			printf("主线程第次%d运行，循环次数%d\n", i+1, j+1);
		}
		SetEvent(g_hThreadEvent2);
	
	}



	getchar();
	return 0;
}

unsigned int __stdcall ThreadLoop(LPVOID)
{
	
	for (int i = 0 ; i < LOOP; ++i)
	{
		WaitForSingleObject(g_hThreadEvent2, INFINITE);

		for (int j = 0 ; j < 2; ++j)
		{
			printf("子线程第%d次运行，正在循环%d次\n", i+1, j+1);
		}
		SetEvent(g_hThreadEvent1);
	}

	_endthreadex(0);
	return 0;
}

#endif