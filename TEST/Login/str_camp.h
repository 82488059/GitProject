#pragma once
#include <tchar.h>
#include <stdlib.h>
// #include <ms/tstring.h>

//stdlib.h
class cstr_camp{
public:
	cstr_camp(const char* p):str(p){}
	~cstr_camp(){;}

	operator int()
	{
        return _tstoi(str);
	}
// 	operator unsigned int()
// 	{
//         return _tstoi(str);
// 	}
    operator long()
    {
        return _tstol(str);
    }
    operator std::string()
    {
        return str;
    }
    operator short()
    {
        return _tstoi(str);
    }
    operator unsigned short()
    {
        return _tstoi(str);
    }

	static cstr_camp str_camp(const char* str)
	{
		return cstr_camp(str);
	}

private:
    const char * str;
};

//cstr_camp str_camp(const char* str);
