//
// Created by alex on 19/08/2015.
//

#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <io.h>
#include <errno.h>
#include <stdlib.h>

void safe_create_dir(const char *dir) {
#ifdef __linux__
    if (mkdir(dir, 0755) < 0) {
#else
    if (mkdir(dir) < 0) {
#endif
        if (errno != EEXIST) {
            perror(dir);
            exit(1);
        }
    }
}

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