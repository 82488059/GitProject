// SVS.cpp : ���� DLL Ӧ�ó���ĵ���������
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