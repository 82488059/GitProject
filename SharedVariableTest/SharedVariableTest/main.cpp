#include <iostream>
#include "windows.h"

typedef int(__stdcall * Get)();
typedef void(__stdcall * Set)(int v);

int main()
{
    using std::cout;
    using std::endl;

    HMODULE ha = LoadLibrary(L"SVA.DLL");
    if (NULL == ha)
    {
        return 1;
    }
    Get GetA = GetProcAddress(ha, "GetValue");
    if (NULL == GetA)
    {
        return 1;
    }
    Set SetA = (Set)GetProcAddress(ha, "SetValue");
    if (NULL == SetA)
    {
        return 1;
    }
    HMODULE hb = LoadLibrary(L"SVB.DLL");
    if (NULL == hb)
    {
        return 1;
    }
    Get GetB = GetProcAddress(hb, "GetValue");
    if (NULL == GetB)
    {
        return 1;
    }
    Set SetB = (Set)GetProcAddress(hb, "SetValue");
    if (NULL == SetB)
    {
        return 1;
    }
    cout << "GetA:" << GetA() << endl;
    cout << "GetB:" << GetB() << endl;
    SetA(5);
    cout << "GetB:" << GetB() << endl;
    SetB(10);
    cout << "GetA:" << GetA() << endl;

    system("pause");
    return 0;
}