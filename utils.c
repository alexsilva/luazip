//
// Created by alex on 19/08/2015.
//

#include "utils.h"
#include <string.h>
#include <io.h>
#include <errno.h>
#include <assert.h>

/* Creates a new directory considering the execution platform */
int create_dir(const char *dir, mode_t mode) {
#ifdef __linux__
    return mkdir(dir, mode);
#else
    return mkdir(dir);
#endif
}

/* Create a directory tree based on the given path */
int mkdirs(const char *dir, mode_t mode) {
    assert(dir && *dir);
    char* p;
    for (p=strchr(dir +1, '/'); p; p=strchr(p+1, '/')) {
        *p='\0';
        if (create_dir(dir, mode) == -1) {
            if (errno!=EEXIST) {
                *p='/'; return -1;
            }
        }
        *p='/';
    }
    return 0;
}

/* Joins two paths considering the platform sep (\\|/)*/
void join(char* destination, const char* path1, const char* path2)
{
    if(path1 == NULL && path2 == NULL) {
        strcpy(destination, "");;
    }
    else if(path2 == NULL || strlen(path2) == 0) {
        strcpy(destination, path1);
    }
    else if(path1 == NULL || strlen(path1) == 0) {
        strcpy(destination, path2);
    }
    else {
        char directory_separator[] = "/";
#ifdef _MSC_VER
        directory_separator[0] = '\\';
#endif
        const char *last_char = path1;
        while(*last_char != '\0')
            last_char++;
        int append_directory_separator = 0;
        if(strcmp(last_char, directory_separator) != 0) {
            append_directory_separator = 1;
        }
        strcpy(destination, path1);
        if(append_directory_separator)
            strcat(destination, directory_separator);
        strcat(destination, path2);
    }
}