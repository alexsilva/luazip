//
// Created by alex on 19/08/2015.
//

#ifndef LUAZIP_ZIP_UTILS_H
#define LUAZIP_ZIP_UTILS_H

#include <sys/types.h>

void join(char*, const char*, const char*);
int create_dir(const char *, mode_t);
int mkdirs(const char *, mode_t);

#endif //LUAZIP_ZIP_UTILS_H
