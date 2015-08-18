#include <lauxlib.h>
#include <errno.h>
#include "zip.h"
#include "lua.h"
#include "lzip.h"

#define BUFSIZE 1024 * 2


/* open zip */
static void lzip_open(lua_State *L) {
    const char *path = luaL_check_string(L, 1); // file path

    int flags = 0; // zip flags
    int zep = 0; // open status

    // open the file
    struct zip * zip_s;

    zip_s = zip_open(path, flags, &zep);

    // push status code
    lua_pushnumber(L, zep);

    // check for errors
    if (zep > 0) {
        char buf[BUFSIZE];
        int slen = zip_error_to_str(buf, sizeof(buf), zep, errno);
        // push error message
        lua_pushlstring(L, buf, slen);
    }

    // push zip object opened
    lua_pushuserdata(L, zip_s);
}


/* zip file close */
static void lzip_close(lua_State *L) {
    lua_Object ref = lua_getparam(L, 1);

    struct zip * zip_s;
    zip_s = (struct zip *) lua_getuserdata(L, ref);

    int ret = zip_close(zip_s);

    lua_pushnumber(L, ret);
}


static struct luaL_reg lzip[] = {
    {"zip_open", lzip_open},
    {"zip_close", lzip_close},
};


int LUA_LIBRARY lua_lzipopen(lua_State *L) {
    luaL_openlib(L, lzip, (sizeof(lzip)/sizeof(lzip[0])));
    return 0;
}