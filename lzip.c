#include <lauxlib.h>
#include <errno.h>
#include "zip.h"
#include "lzip.h"

#define BUFSIZE 1024 * 2


static struct zip *get_zip_ref(lua_State *L, int n) {
    return lua_getuserdata(L, lua_getparam(L, n));  // implicit cast
}


/* open zip */
static void lzip_open(lua_State *L) {
    const char *path = luaL_check_string(L, 1); // file path

     // zip flags
    int flags = (int) lua_getnumber(L, lua_getparam(L, 2));
    flags = flags  == 0 ? ZIP_CHECKCONS : flags;

    // open status
    int zep = 0;

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
    struct zip * zip_s = get_zip_ref(L, 1);

    int ret = zip_close(zip_s);

    lua_pushnumber(L, ret);
}


static struct luaL_reg lzip[] = {
    {"zip_open", lzip_open},
    {"zip_close", lzip_close},
};


#define set_table(L, obj, name, value) \
    lua_pushobject(L, obj); \
    lua_pushstring(L, name); \
    lua_pushnumber(L, value); \
    lua_settable(L);


int LUA_LIBRARY lua_lzipopen(lua_State *L) {
    luaL_openlib(L, lzip, (sizeof(lzip)/sizeof(lzip[0])));

    // zip open modes
    lua_Object modes = lua_createtable(L);

    // create the global table flags
    lua_pushobject(L, modes);
    lua_setglobal(L, "zip_open_flags");

    // ZIP_CHECKCONS: Perform additional stricter consistency checks on the archive, and error if they fail.
    set_table(L, modes, "ZIP_CHECKCONS", ZIP_CHECKCONS);

    // ZIP_CREATE: Create the archive if it does not exist.
    set_table(L, modes, "ZIP_CREATE", ZIP_CREATE);

    // ZIP_EXCL: Error if archive already exists.
    set_table(L, modes, "ZIP_EXCL", ZIP_EXCL);

    // ZIP_TRUNCATE: If archive exists, ignore its current contents. In other words, handle it the same way as an empty archive.
    set_table(L, modes, "ZIP_TRUNCATE", ZIP_TRUNCATE);

    // ZIP_RDONLY: Open archive in read-only mode.
    // set_table(L, modes, "ZIP_RDONLY:", ZIP_RDONLY);

    lua_pushobject(L, modes); // end table
    return 0;
}