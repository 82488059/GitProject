#pragma once
#include <string>

class enterkey
{
public:
	enterkey();
	virtual ~enterkey();
    static bool input(char ch);
    static bool EnterStr(const std::string& str);
    static bool Enter30Backspace();
    static bool EnterBackspace(DWORD times);
    static DWORD select_vk(char ch);
    static int tab[];
};

