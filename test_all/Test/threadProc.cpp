#include <windows.h>
#include <stdio.h>
#include <process.h>

struct ThreadNum{
	int num;
};

int tally = 0;//glable

unsigned int __stdcall ThreadProc(PVOID pm)
{
	for(int i = 1; i <= 50; i++)
	{
		tally += 1;
	}

	printf("tally=%d\n", tally);

	_endthreadex(0);
	return 0;
}

unsigned int __stdcall ThreadNumOff(PVOID pm)
{
	static int nIndex = 0;
	printf("第%2d个创建子线程，第%2d个输出，子线程ID号是%5d\n", ((ThreadNum *)pm)->num, InterlockedIncrement((LPLONG)&nIndex), GetCurrentThreadId());
	return 0;
}

DWORD WINAPI ThreadFun(LPVOID pM)
{
// 	enum{MAXHANDLE = 1};
// 	HANDLE handle[MAXHANDLE];
// 	handle[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadProc, NULL, 0, NULL);
// 	WaitForMultipleObjects(MAXHANDLE, handle, TRUE, INFINITE);
// 	CloseHandle(handle[0]);

	static int nIndex = 0;
	printf("第%d个子线程ID号是%d\n", ++nIndex, GetCurrentThreadId());
	return 0;
}



int main4()
{
	enum { MAXHANDLE = 10 };
	HANDLE handle[MAXHANDLE];

	struct ThreadNum nIndexThread[10];

	for (int i = 0; i < MAXHANDLE; ++i)
	{
		nIndexThread[i].num = i + 1;
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
