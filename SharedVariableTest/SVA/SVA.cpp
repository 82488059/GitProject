// SVA.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"

typedef int(__stdcall * Get)();
typedef void(__stdcall *Set)(int v);

HMODULE hm = NULL;
extern "C" int _stdcall GetValue()
{
    hm = LoadLibrary(L"SVS.DLL");
    if (NULL == hm)
    {
        return -1;
    }
    Get GetS = GetProcAddress(hm, "GetValue");
    if (NULL == GetS)
    {
        return -1;
    }

    return GetS();
}

extern "C" void _stdcall SetValue(int v)
{
    hm = LoadLibrary(L"SVS.DLL");
    if (NULL == hm)
    {
        return;
    }
    Set SetS = (Set)GetProcAddress(hm, "SetValue");
    if (NULL == SetS)
    {
        return;
    }

    return SetS(v);
}