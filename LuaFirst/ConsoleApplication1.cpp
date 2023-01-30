#include <stdio.h>
#include <Windows.h>
#include "LUA/InitLua.h"

extern "C" int _Printf(lua_State * L)
{
    const char* data = luaL_optstring(L, 1, 0);
    //printf("%s \n", data);   이렇게 lua 스크립트를 수정하지 않고 기능을 제한할 수 있다.
    printf("%s \n", data);

    return 0;
}

int main()
{
    lua_State* L = lua_open();
    luaL_openlibs(L);
    printf("Lua Version : %s \n", LUA_VERSION);

    luaL_reg Luafunc[] =
    {
        {"printf", _Printf},
    };

    int Luafunc_Size = sizeof(Luafunc) / sizeof(luaL_reg);
    for (int i = 0;i < 1;i++)
    {
        lua_register(L, Luafunc[i].name, Luafunc[i].func);
    }

    int state = luaL_loadfile(L, "test.lua");
    lua_pcall(L, 0, 0, 0);

    lua_getglobal(L, "GetFunc");
    lua_pushnumber(L, 13);  //실수값.  정수형은 pushinteger
    lua_pushnumber(L, 12);
    lua_call(L, 2, 2);
    float result = (float)luaL_optnumber(L, 1, 0);
    float result1 = (float)luaL_optnumber(L, 2, 0);
    printf("%0.2f, %0.2f \n", result, result1);
    
    
    lua_close(L);

    return 0;
}


