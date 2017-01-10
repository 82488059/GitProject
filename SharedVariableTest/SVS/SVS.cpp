// SVS.cpp : 定义 DLL 应用程序的导出函数。
//

#include "stdafx.h"



int value = 0;

extern "C" int GetValue()
{
    return value;
}

extern "C" void SetValue(int v)
{
    value = v;
}