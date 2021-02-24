// 有四个线程1、2、3、4。线程1的功能就是输出1，线程2的功能就是输出2，以此类推.........
// 现在有四个文件ABCD。初始都为空。现要让四个文件呈如下格式：
// A：1 2 3 4 1 2....
// B：2 3 4 1 2 3....
// C：3 4 1 2 3 4....
// D：4 1 2 3 4 1....
// 
// 
// 
#if 0
#include <process.h>
#include <Windows.h>
#include <stdio.h>

#include <stdio.h>
#include <stdlib.h>


enum{MAXHANDLE = 4};
int const LOOP = 10;

HANDLE g_hThreadEvent[MAXHANDLE];


unsigned int __stdcall ThreadPrint1(LPVOID);
unsigned int __stdcall ThreadPrint2(LPVOID);
unsigned int __stdcall ThreadPrint3(LPVOID);
unsigned int __stdcall ThreadPrint4(LPVOID);

int main()
{
	g_hThreadEvent[0] = CreateEvent(NULL, FALSE, TRUE, NULL);
	g_hThreadEvent[1] = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hThreadEvent[2] = CreateEvent(NULL, FALSE, FALSE, NULL);
	g_hThreadEvent[3] = CreateEvent(NULL, FALSE, FALSE, NULL);

	HANDLE handle[MAXHANDLE];

	handle[0] = (HANDLE)_beginthreadex(NULL, 0, ThreadPrint1, NULL, 0, NULL);
	handle[1] = (HANDLE)_beginthreadex(NULL, 0, ThreadPrint2, NULL, 0, NULL);
	handle[2] = (HANDLE)_beginthreadex(NULL, 0, ThreadPrint3, NULL, 0, NULL);
	handle[3] = (HANDLE)_beginthreadex(NULL, 0, ThreadPrint4, NULL, 0, NULL);


	WaitForMultipleObjects(MAXHANDLE, handle, TRUE, INFINITE);

	for (int i = 0; i < MAXHANDLE; ++i)
	{
		CloseHandle(handle[i]);
	}

}


unsigned int __stdcall ThreadPrint1(LPVOID)
{
	WaitForSingleObject(g_hThreadEvent[0], INFINITE);
	printf(" 1");
	SetEvent(g_hThreadEvent[1]);

	return 0;
}
unsigned int __stdcall ThreadPrint2(LPVOID)
{
	WaitForSingleObject(g_hThreadEvent[1], INFINITE);
	printf(" 2");
	SetEvent(g_hThreadEvent[2]);

	return 0;
}
unsigned int __stdcall ThreadPrint3(LPVOID)
{
	WaitForSingleObject(g_hThreadEvent[2], INFINITE);
	printf(" 3");
	SetEvent(g_hThreadEvent[3]);
	
	return 0;
}
unsigned int __stdcall ThreadPrint4(LPVOID)
{
	WaitForSingleObject(g_hThreadEvent[3], INFINITE);
	printf(" 4");
	SetEvent(g_hThreadEvent[0]);

	return 0;
}


#endif