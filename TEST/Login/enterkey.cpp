#include "stdafx.h"
#include "enterkey.h"
#include <vector>
int enterkey::tab[500] = {

};

// VK_SHIFT;

enterkey::enterkey()
{

}


enterkey::~enterkey()
{
}
#define press_down(T) keybd_event((T), MapVirtualKey((T), 2), 0, 0);

#define press_up(T) keybd_event((T), MapVirtualKey((T), 2), KEYEVENTF_KEYUP, 0);

bool enterkey::input(char ch)
{
    DWORD pre = select_vk(ch);
    if (-1 != pre)
    {
        return false;
    }
    press_down(pre);
    Sleep(20);
    press_up(pre);
    return true;
}

DWORD enterkey::select_vk(char ch)
{
    if (ch >= '0' && ch <='9')
    {
        return 0x30 + ch - '0';
    }
    if (ch >= 'A' && ch<='Z')
    {
        return 0x41 + ch - 'A';
    }
    if (ch >= 'a' && ch <= 'z')
    {
        return 0x61 + ch - 'a';
    }
    return -1;

    DWORD ret = -1;
    switch (ch)
    {
    case 0:
        ret = 0x48;

        break;
    default:
        break;
    }
    return ret;
}

bool enterkey::EnterStr(const std::string& str)
{
    std::vector<DWORD> v;

    for (int i = 0; i < str.size(); ++i)
    {
        int nkey = select_vk(str[i]);
        if (-1 == nkey)
        {
            return false;
        }
        v.push_back(nkey);
    }

    for (auto &it : v)
    {
        press_down(it);
        Sleep(20+rand()%10);
        press_up(it);
        Sleep(150+rand()%50);
    }

    return true;
}

bool enterkey::Enter30Backspace()
{
	for (int i = 0; i < 30; ++i)
	{
		press_down(0x08);
		Sleep(30+rand()%10);
		press_up(0x08);
		Sleep(50+rand()%20);
	}
	return true;
}

bool enterkey::EnterBackspace(DWORD times)
{
    press_down(0x08);
    DWORD del = GetTickCount()+times;
    do 
    {
        Sleep(100);
    } while (GetTickCount()<del);

    press_up(0x08);

    return true;
}
