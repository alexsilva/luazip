#include <lauxlib.h>
#include <errno.h>
#include "zip.h"
#include "lzip.h"

#define BUFSIZE 1024 * 2


static struct zip *get_zip_ref(lua_State *L, int n) {
    struct zip * zip_s = lua_getuserdata(L, lua_getparam(L, n));  // implicit cast
    if (!zip_s)
        lua_error(L, "You need to pass the zip object as the first parameter." \
                     "\nCall 'zip_open' to initialize the object.");
    return zip_s;
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
    } else if (!zip_s) {
        // unknown error.
        lua_pushstring(L, "Zip can not be initialized. Unknown error.");
    } else {
        // push zip object opened
        lua_pushuserdata(L, zip_s);
    }
}


/* zip file close */
static void lzip_close(lua_State *L) {
    struct zip * zip_s = get_zip_ref(L, 1);
    int ret = zip_close(zip_s);
    lua_pushnumber(L, ret);
}

/* close zip archive and discard changes */
static void lzip_discard(lua_State *L) {
    struct zip * zip_s = get_zip_ref(L, 1);
    zip_discard(zip_s);
}

/* create new directories in the zip file */
static void lzip_add_dir(lua_State *L) {
    struct zip * zip_s = get_zip_ref(L, 1);
    const char *dirname = luaL_check_string(L, 2);
    zip_int64_t index = zip_add_dir(zip_s, dirname);
    lua_pushnumber(L, index);
}

/* create new files in the zip file */
static void lzip_add_file(lua_State *L) {
    struct zip * zip_s = get_zip_ref(L, 1);

    const char *sfilepath = luaL_check_string(L, 2);
    const char *tfilepath = luaL_check_string(L, 3);

    struct zip_source * source_t;
    source_t = zip_source_file(zip_s, sfilepath, 0, -1);

    zip_int64_t index = zip_file_add(zip_s, tfilepath, source_t, ZIP_FL_ENC_UTF_8);

    lua_pushnumber(L, index);
}

/* Function returns the number of files in archive. */
static void lzip_get_num_entries(lua_State *L) {
    struct zip * zip_s = get_zip_ref(L, 1);
    zip_int64_t num = zip_get_num_entries(zip_s, ZIP_FL_UNCHANGED);
    lua_pushnumber(L, num);
}

/* Returns the index of the file named fname or -1, if archive does not contain an entry of that name. */
static void lzip_name_locate(lua_State *L) {
    struct zip *zip_s = get_zip_ref(L, 1);

    const char *filename = luaL_check_string(L, 2);

    unsigned int flag = (unsigned int) lua_getnumber(L, lua_getparam(L, 3));
    flag = flag == 0 ? ZIP_FL_ENC_GUESS : flag;

    zip_int64_t index = zip_name_locate(zip_s, filename, flag);

    lua_pushnumber(L, index); // index of file or -1
}

static struct luaL_reg lzip[] = {
    {"zip_open", lzip_open},
    {"zip_close", lzip_close},
    {"zip_discard", lzip_discard},
    {"zip_add_dir", lzip_add_dir},
    {"zip_add_file", lzip_add_file},
    {"zip_get_num_entries", lzip_get_num_entries},
    {"zip_name_locate", lzip_name_locate}
};


#define set_table(L, obj, name, value) \
    lua_pushobject(L, obj); \
    lua_pushstring(L, name); \
    lua_pushnumber(L, value); \
    lua_settable(L);


int LUA_LIBRARY lua_lzipopen(lua_State *L) {
    luaL_openlib(L, lzip, (sizeof(lzip)/sizeof(lzip[0])));

    // zip open flags
    lua_Object flags = lua_createtable(L);

    // create the global table flags
    lua_pushobject(L, flags);
    lua_setglobal(L, "zip_open_flags");

    // ZIP_CHECKCONS: Perform additional stricter consistency checks on the archive, and error if they fail.
    set_table(L, flags, "ZIP_CHECKCONS", ZIP_CHECKCONS);

    // ZIP_CREATE: Create the archive if it does not exist.
    set_table(L, flags, "ZIP_CREATE", ZIP_CREATE);

    // ZIP_EXCL: Error if archive already exists.
    set_table(L, flags, "ZIP_EXCL", ZIP_EXCL);

    // ZIP_TRUNCATE: If archive exists, ignore its current contents. In other words, handle it the same way as an empty archive.
    set_table(L, flags, "ZIP_TRUNCATE", ZIP_TRUNCATE);

    // ZIP_RDONLY: Open archive in read-only mode.
    // set_table(L, flags, "ZIP_RDONLY:", ZIP_RDONLY);

    lua_pushobject(L, flags); // end table

    // create the global table encoding
    lua_Object encoding = lua_createtable(L);

    lua_pushobject(L, encoding);
    lua_setglobal(L, "zip_file_encoding");

    // ZIP_FL_NOCASE: Ignore case distinctions. (Will only work well if the file names are ASCII.)
    set_table(L, encoding, "ZIP_FL_NOCASE", ZIP_FL_NOCASE);

    // ZIP_FL_NODIR: Ignore directory part of file name in archive.
    set_table(L, encoding, "ZIP_FL_NODIR", ZIP_FL_NODIR);

    // ZIP_FL_ENC_RAW: Compare against the unmodified names as it is in the ZIP archive.
    set_table(L, encoding, "ZIP_FL_ENC_RAW", ZIP_FL_ENC_RAW);

    // ZIP_FL_ENC_GUESS: (Default.) Guess the encoding of the name in the ZIP archive and convert it to UTF-8,
    // if necessary, before comparing.
    set_table(L, encoding, "ZIP_FL_ENC_GUESS", ZIP_FL_ENC_GUESS);

    // ZIP_FL_ENC_STRICT: Follow the ZIP specification and expect CP-437 encoded names in the ZIP archive
    // (except if they are explicitly marked as UTF-8). Convert it to UTF-8 before comparing.
    // Note: ASCII is a subset of both CP-437 and UTF-8.
    set_table(L, encoding, "ZIP_FL_ENC_STRICT", ZIP_FL_ENC_STRICT);
    return 0;
}