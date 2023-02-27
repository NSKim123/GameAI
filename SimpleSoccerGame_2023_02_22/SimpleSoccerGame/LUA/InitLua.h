
#ifndef INITLUA_H__
#define INITLUA_H__

extern "C" 
{
    FILE* __cdecl __iob_func(void)
    {
        FILE _iob[] = { *stdin, *stdout, *stderr };
        return _iob;
    }
#include "include\lua.h"
#include "include\lauxlib.h"
#include "include\lualib.h"
}

#pragma comment (lib, "legacy_stdio_definitions.lib")
#pragma comment (lib, "LUA/lua5.1.lib")



#endif

