#include <iostream>

extern "C"
{
#include "lua.h"  
#include "lauxlib.h"  
#include "lualib.h"  
}
using namespace std;

int main()
{
    lua_State *L = luaL_newstate();

    lua_pushstring(L, "I am so cool~");
    lua_pushnumber(L, 20);

    //3.取值操作  
    if (lua_isstring(L, 1)){             //判断是否可以转为string  
        cout << lua_tostring(L, 1) << endl;  //转为string并返回  
    }
    if (lua_isnumber(L, 2)){
        cout << lua_tonumber(L, 2) << endl;
    }

    lua_close(L);

    return 0;
}