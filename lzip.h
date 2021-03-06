//
// Created by alex on 18/08/2015.
//

#ifndef LUAZIP_LZIP_H
#define LUAZIP_LZIP_H

#include <lua.h>

#if defined(_MSC_VER)
    //  Microsoft
    #define LUA_LIBRARY __declspec(dllexport)
#else
//  GCC
#define LUA_LIBRARY __attribute__((visibility("default")))
#endif

#define UNZIP_SUCCESS  0
#define UNZIP_ERROR   -1

struct unzip_st {
    char *msg;
    int code;
};

int LUA_LIBRARY lua_lzipopen(lua_State *);

#endif //LUAZIP_LZIP_H
