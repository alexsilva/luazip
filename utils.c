//
// Created by alex on 19/08/2015.
//

#include "utils.h"
#include <string.h>
#include <io.h>
#include <errno.h>
#include <assert.h>
#include <sys/stat.h>
#include <libgen.h>

/* Creates a new directory considering the execution platform */
int create_dir(const char *dir, mode_t mode) {
#ifdef __linux__
    return mkdir(dir, mode);
#else
    return mkdir(dir);
#endif
}

#define PATH_SEP '/'

/* Create a directory tree based on the given path */
int mkdirs(const char *dir, mode_t mode) {
    assert(dir && *dir);
    char* p;
    for (p=strchr(dir +1, PATH_SEP); p; p=strchr(p+1, PATH_SEP)) {
        *p='\0';
        if (create_dir(dir, mode) == -1) {
            if (errno!=EEXIST) {
                *p= PATH_SEP; return -1;
            }
        }
        *p= PATH_SEP;
    }
    return 0;
}

/* Joins two paths considering the platform sep (\\|/)*/
void join(char* destination, const char* path1, const char* path2) {
    if(path1 == NULL && path2 == NULL) {
        strcpy(destination, "");
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

/* Checks whether the directory exists in the file system. */
bool dir_exists(const char *dir) {
    struct stat sb;
    return stat(dir, &sb) == 0 && S_ISDIR(sb.st_mode);
}

/* Create the absolute path directory if it does not exist. Must be path to a file (/home/user/file.zip) */
int create_required_missing_dir(char *abspath) {
    int slen = strlen(abspath);
    char dirpath[slen];

    strcpy(&dirpath[0], abspath);
    char *filedir = dirname(dirpath);

    if (filedir != NULL && !dir_exists(filedir)) {
        slen = strlen(filedir);
        if (filedir[slen - 1] != '/') {
            char fddest[slen + 2];
            strcpy(&fddest[0], filedir);

            fddest[slen] = '/';
            fddest[slen + 1] = '\0';

            return mkdirs(fddest, UNZIP_DMODE);
        }
    }
    return -1;
}