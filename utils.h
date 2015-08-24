//
// Created by alex on 19/08/2015.
//

#ifndef LUAZIP_ZIP_UTILS_H
#define LUAZIP_ZIP_UTILS_H

#include <sys/types.h>
#include <stdbool.h>

#define UNZIP_DMODE 0755

void join(char*, const char*, const char*);
int create_dir(const char *, mode_t);
int mkdirs(const char *, mode_t);
bool dir_exists(const char *);
int create_required_missing_dir(char *);

#endif //LUAZIP_ZIP_UTILS_H
